#include "defs.h"

void handle_sigint(int signal_code);

int main(int argc, char **argv)
{
    Game game;

    // Set up a signal handler
    signal(SIGINT, handle_sigint);

    printf("Testing program!\n");

    safe_string_input(game.us_word, "What is our word?");
    safe_string_input(game.them_word, "What is their word?");
}

void handle_sigint(int signal_name)
{
    printf("\"You can check out any time you like, but you can never leave.\"\n");
}

