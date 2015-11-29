#include "defs.h"
// Our communication socket with the client.
int socket;

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

void turn(Game g)
{
    int selection;
    char buffer[MAX_STR];

    //Will player guess a letter or word?
    safe_integer_input(buffer, "What would you like to guess?\n\t(1) a letter\n\t(2) a word\n", 1, 2, &selection);

    if (selection == 1)
    {
        //Letter
        if (getLetterGuess(&g) == C_OK)
        {
            //Guessed letter is in the word
        }
        else
        {
            //Guessed letter is not in the word
        }
    }
    else if (selection == 2)
    {
        //Word
        if (getWordGuess(&g) == C_OK)
        {
            //WINNER (guessed word is correct)
            g.state = WIN;
        }
        else
        {
            //Guessed word is not correct
        }
    }

    //Networking
    end_turn(&g);
}
