#include "defs.h"

#define PORT_NUMBER 60003

// recv, but retrying after interrupt.
/*
 * Function: hardened_recv
 * Purpose: This function attempts to stop recv from being interrupted by a
 *          ctrl-C, as returning to a system call after a signal gives a
 *          EINTR.
 * in: All the parameters of recv. See man page.
 */
void hardened_recv(int sockfd, void *buf, size_t len, int flags)
{
    do
    {
        recv(sockfd, buf, len, flags);
    } while (errno != EINTR);
}

/*
 * Function: do_network_setup
 * Purpose: Does the setup for a connection with the other side (spooky!).
 *          This involves sending our word across the socket then receiving
 *          back the other side's word.
 * in/out: the game structure to operate on. Must have us_word populated.
 */
void do_network_setup(Game *g) 
{
    send(game_socket, g->us_word, sizeof(g->us_word), 0);
    hardened_recv(game_socket, g->them_word, sizeof(g->them_word), 0);
}

/*
 * Function: activate_socket_server
 * Purpose: Checks the value of *server_socket. If it is -1, creates a server
 *          socket and stores its fd in *server_socket. Then listens on either
 *          the newly created socket or on the one already in *server_socket.
 * in/out: a place to store the server socket when it is created, or a pointer
 *         containing the fd of the server socket (-1 value indicates no socket
 *         has been created yet).
 */
void activate_socket_server(int *server_socket)
{
    // Socket for binding on and return codes of various
    // socket functions
    int ret;
    struct sockaddr_in addr;

    printf("Waiting to connect...\n");

    if (*server_socket == -1)
    {
        // Create the socket so that we can listen - note that this
        // isn't where communication will happen, just where we
        // we listen for clients.
        *server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (*server_socket < 0) 
        {
            printf("Couldn't open socket.\n");
            exit(1);
        }

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons((unsigned short) PORT_NUMBER);

        ret = bind(*server_socket,
                   (struct sockaddr *) &addr,
                   sizeof(addr));
        if (ret < 0) 
        {
            printf("Bind failed :-(\n");
            exit(1);
        }
    }

    ret = listen(*server_socket, 5);
    if (ret < 0) 
    {
        printf("Couldn't listen! :-(\n");
        exit(1);
    }

    size_t addr_size = sizeof(addr);

    game_socket = accept(*server_socket, 
                         (struct sockaddr *) &addr,
                         &addr_size);
    if (game_socket < 0) 
    {
        printf("Couldn't accept connection!\n");
        exit(1);
    }
}

/*
 * Function: wait_for_turn
 * Purpose: Waits for a turn to finish by receiving updates about the other
 *          side's turn.
 * in/out: the game structure to operate on. Must be fully initialized.
 */
void wait_for_turn(Game *g)
{
    printf("Waiting for opponent's turn!\n");
    char buffer_recv[MAX_STR];
    enum gamestate other_state;
    int correct;

    hardened_recv(game_socket, buffer_recv, sizeof(buffer_recv), 0);
    hardened_recv(game_socket, &other_state, sizeof(enum gamestate), 0);
    hardened_recv(game_socket, &(g->them_solution), sizeof(g->them_solution), 0);
    hardened_recv(game_socket, &correct, sizeof(correct), 0);

    display_message_waiting(g, buffer_recv, correct);

    if (other_state == WIN) 
    {
        g->state = LOSE;
    }
    else 
    {
        g->state = TURN;
    }
}

/*
 * Function: end_turn
 * Purpose: Sends the updates for this turn, checks if we won, and if we didn't,
 *          calls wait_for_turn to wait for our next turn.
 * in/out: the game structure to operate on. Must be fully initialized.
 * in: The buffer containing the user's guess
 * in: If the guess above was correct or not.
 */
void end_turn(Game *g, char *buffer, int correct)
{
    send(game_socket, buffer, MAX_STR, 0);
    send(game_socket, &(g->state), sizeof(g->state), 0);
    send(game_socket, &(g->us_solution), sizeof(g->us_solution), 0);
    send(game_socket, &correct, sizeof(correct), 0);

    if (g->state != WIN)
    {
        wait_for_turn(g);
    }
}

/*
 * Function: connect_client
 * Purpose: Creates a client socket and stores it in the global variable
 *          game_socket. Uses the address `address`
 * in/out: the address of the server.
 */
void connect_client(char *address)
{
    struct sockaddr_in addr;
    int ret_code;

    game_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (game_socket < 0)
    {
        printf("Couldn't create socket. :-(\n");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_port = htons((unsigned short)PORT_NUMBER);

    ret_code = connect(game_socket,
                       (struct sockaddr *) &addr,
                       sizeof(addr));
    if (ret_code < 0)
    {
        printf("Couldn't connect to server! :(\n");
        exit(1);
    }
}

/*
 * Function: send_replay
 * Purpose: Send an integer across the wire which indicates if the winner
 *          wishes to continue playing the game.
 * in: the integer to send. 1 for "I want to replay", 0 for "I want to quit"
 */
void send_replay(int replay)
{
    send(game_socket, &replay, sizeof(int), 0);
}

/*
 * Function: wait_replay
 * Purpose: Waits for a reply from the server about if it wants to replay the
 *          game. If it's a 1, the winner wants to continue. If it's a 0,
 *          the winner wants to quit, and the loser should go into server
 *          mode and wait for a new person to come along.
 * out: the value from the other side.
 */
int wait_replay()
{
    int replay;
    hardened_recv(game_socket, &replay, sizeof(int), 0);
    return replay;
}
