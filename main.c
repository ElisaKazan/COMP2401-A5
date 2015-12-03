#include "defs.h"

// Our communication socket with the other side.
int game_socket;

void handle_sigint(int signal_code);

int main(int argc, char **argv)
{
    Game game;

    if (argc == 1)
    {
        activate_socket_server();
    }
    else if (argc == 2)
    {
        connect_client(argv[0]);
    }
    else
    {
        printf("Please pass only one or zero parameters!\n");
        exit(1);
    }

    // Set up a signal handler
    signal(SIGINT, handle_sigint);


    do_setup(&game);

    game.state = TURN;


    while (game.state != WIN && game.state != LOSE) 
    {
        display_game_status(&game);
        turn(&game);
    }

    // Winning/losing logic
}

void handle_sigint(int signal_name)
{
    printf("\"You can check out any time you like, but you can never leave.\"\n");
}

