#include "defs.h"

// Our communication socket with the other side.
int game_socket;

void handle_sigint(int signal_code);
static int do_game(Game *game, int *server_socket);

// The main function.
int main(int argc, char **argv)
{
    Game game;
    // Start the server socket off as a number it can't be 
    // if it is initialized.
    int server_socket = -1;

    // Set up a signal handler
    signal(SIGINT, handle_sigint);

    // 1 if there's no arguments, 0 if there are.
    game.server = (argc == 1);

    if (argc == 1)
    {
        activate_socket_server(&server_socket);
    }
    else if (argc == 2)
    {
        connect_client(argv[1]);
    }
    else
    {
        printf("Please pass only one or zero parameters!\n");
        exit(1);
    }

    int end_game = C_NOK;

    while (end_game == C_NOK)
    {
        end_game = do_game(&game, &server_socket);
    }

    if (server_socket != -1)
    {
        close(server_socket);
    }
}

/*
 * Function: do_game
 * Purpose: Performs the main game logic for the main loop.
 * in/out: the Game structure. Doesn't need to be initialized.
 * in/out: the space for keeping the server socket. Used for when we
 *         want to become the server.
 * return: C_OK if the game should terminate, C_NOK if not.
 */
static int do_game(Game *game, int *server_socket)
{
    char str[MAX_STR];

    do_setup(game);

    // If we're the client, we go first!
    if (!game->server)
    {
        game->state = TURN;
    }
    // Otherwise, we have to let the 
    // other player go.
    else
    {
        game->state = WAITING_FOR_TURN;
    }

    while (game->state != WIN && game->state != LOSE) 
    {
        display_game_status(game);
        if (game->state == WAITING_FOR_TURN) 
        {
            wait_for_turn(game);
        }
        else if (game->state == TURN) 
        {
            turn(game);
        }
    }

    if (game->state == WIN)
    {
        while (1)
        {
            // See if the user wants to play again
            safe_string_input(str, "You won! Do you want to play again (Y/n)?");
            if (str[0] == 'Y' || str[0] == 'y')
            {
                printf("Like my iPod stuck on replay, replay...\n");
                send_replay(1);
                break;
            }
            else if (str[0] == 'N' || str[0] == 'n')
            {
                printf("Bye!\n");
                send_replay(0);
                close(game_socket);
                // Terminate the game
                return C_OK;
            }
        }
    }
    else
    {
        printf("You lost. The opponents word was %s\n", game->them_word);
        printf("You lost... Waiting for the response of your opponent.\n");
        int replay = wait_replay();
        if (replay)
        {
            printf("Winner wants to replay. Let's go again!\n");
        }
        else
        {
            printf("The winner was satisfied with your defeat... Time to wait for another player.\n");
            game->server = 1;
            // Listen for another client
            activate_socket_server(server_socket);
        }
    }

    // The game continues
    return C_NOK;
}

/*
 * Function: handle_sigint
 * Purpose: Handles a interrupt signal (printing out a message), then puts
 *          itself back in as a signal handler (this seems to be a necessary
 *          step). This is to block Ctrl-C.
 * in: the number of the signal that fired (we only register SIGINT)
 */
void handle_sigint(int signal_number)
{
    printf("\n\"You can check out any time you like, but you can never leave.\"\n");
    signal(SIGINT, handle_sigint);
}

