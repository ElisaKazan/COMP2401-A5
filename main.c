#include "defs.h"
// Our communication socket with the client.
int game_socket;

void handle_sigint(int signal_code);

int main(int argc, char **argv)
{
    Game game;


    printf("____ or _ _ _ _\n");
    printf("h _ p p y   d _ y _");

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

void turn(Game *g)
{
    int selection;
    char buffer[MAX_STR];

    //Will player guess a letter or word?
    safe_integer_input(buffer, "What would you like to guess?\n\t(1) a letter\n\t(2) a word\n", 1, 2, &selection);

    if (selection == 1)
    {
        //Letter
        if (getLetterGuess(g) == C_OK)
        {
            //Guessed letter is in the word
         
            //Check if the word is complete
            if (strcmp(g->them_solution, g->them_word) == 0)
            {
                //WINNER (completed the word)
                display_message_winner(g);
                g->state = WIN;
            }
            else
            {
                //display_message_turn(g, ___, C_OK);
                g->state = WAITING_FOR_TURN;
            }
        }
        else
        {
            //Guessed letter is not in the word
            //display_message_turn(g,___, C_NOK);
            g->state = WAITING_FOR_TURN;
        }
    }
    else if (selection == 2)
    {
        //Word
        if (getWordGuess(g) == C_OK)
        {
            //WINNER (guessed word is correct)
            display_message_winner(g);
            g->state = WIN;
        }
        else
        {
            //Guessed word is not correct
            //display_message_turn(g, ____, C_NOK);
            g->state = WAITING_FOR_TURN;
        }
    }

    //Networking
    end_turn(g);
}

void display_game_status(Game *g)
{
    //Prints the game status

    //Line
    printf("__________________________________________________________________\n");

    //Your status
    printf("You:\n\n");
    
    int i = 0;
    while(g->us_solution[i] != '\0')
    {
        printf("%c ", g->us_solution[i]);
    }

    //Line
    printf("\n__________________________________________________________________\n");

    ///Opponents Status
    printf("Opponent:\n\n");

    i = 0;
    while(g->them_solution[i] != '\0')
    {
        printf("%c ", g->them_solution[i]);
    }
    //Line
    printf("\n__________________________________________________________________\n");

}

void display_message_turn(Game *g, char *guess, int correct)
{
    //You guessed correctly
    if (correct)
    {
        printf("Correct! You guessed '%s'\n", guess);
    }
    //You guessed incorrectly
    else
    {
        printf("Incorrect! You guessed '%s'\n", guess);
    }
}

void display_message_waiting(Game *g, char *guess, int correct)
{
    //They guessed correctly
    if (correct)
    {
        printf("Correct! Opponent guessed '%s'\n", guess);
    }
    //They guessed incorrectly
    else
    {
        printf("Incorrect! Opponent guessed '%s'\n", guess);
    }
}

void display_message_winner(Game *g)
{
    //Winner
    printf("WINNER!!!\n\n");
    //Play again?
    //safe_string_input("Would you like to:\n\t(1) Play Again\n\t(2) Quit\n");
}














