#include "defs.h"

// Our communication socket with the other side.
int game_socket;

void handle_sigint(int signal_code);
static int do_game(Game *game);

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

    int end_game = C_NOK;

    while (end_game == C_NOK) {
        end_game = do_game(&game);
    }

    if (server_socket != -1) {
        close(server_socket);
    }
}

static int do_game(Game *game)
{
    char str[MAX_STR];

    // Set up a signal handler
    signal(SIGINT, handle_sigint);


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
        // See if the user wants to play again
        safe_string_input(str, "You won! Do you want to play again (Y/n)?");
        if (str[0] != 'Y' || str[0] != 'y')
        {
            printf("Like my iPod stuck on replay, replay...\n");
        }
        else if (str[0] != 'N' || str[0] != 'n')
        {
            printf("Bye!\n");
            close(game_socket);
            return C_NOK;
        }
    }
    else
    {
        printf("You lost...\n");
    }

    return C_OK;
}

void handle_sigint(int signal_name)
{
    printf("\"You can check out any time you like, but you can never leave.\"\n");
}

