#include "defs.h"

// Our communication socket with the other side.
int game_socket;

void handle_sigint(int signal_code);

int main(int argc, char **argv)
{
    Game game;
    // Start the server socket off as a number it can't be 
    // if it is initialized.
    int server_socket = -1;

    // 1 if there's no arguments, 0 if there are.
    game.server = (argc == 1);

    printf("Waiting to connect...");
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

    // Set up a signal handler
    signal(SIGINT, handle_sigint);


    do_setup(&game);

    // If we're the client, we go first!
    if (!game.server)
    {
        game.state = TURN;
    }
    // Otherwise, we have to let the 
    // other player go.
    else
    {
        game.state = WAITING_FOR_TURN;
    }


    while (game.state != WIN && game.state != LOSE) 
    {
        display_game_status(&game);
        if (game.state == WAITING_FOR_TURN) 
        {
            wait_for_turn(&game);
        }
        else if (game.state == TURN) 
        {
            turn(&game);
        }
    }

    // Winning/losing logic
}

void handle_sigint(int signal_name)
{
    printf("\"You can check out any time you like, but you can never leave.\"\n");
}

