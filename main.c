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

    game.state = TURN;
    strcpy(game.us_word, "Hello");
    strcpy(game.them_word, "Happy");
    strcpy(game.us_incorrect, "");
    strcpy(game.them_incorrect, "");
    strcpy(game.us_solution, "_____");
    strcpy(game.them_solution, "_____");

    while (game.state != WIN && game.state != LOSE) 
    {
        display_game_status(&game);
        turn(&game);
    }
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
        if (get_letter_guess(g, buffer) == C_OK)
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
                display_message_turn(g, buffer, C_OK);
                g->state = WAITING_FOR_TURN;
            }
        }
        else
        {
            //Guessed letter is not in the word
            display_message_turn(g, buffer, C_NOK);
            g->state = WAITING_FOR_TURN;
        }
    }
    else if (selection == 2)
    {
        //Word
        if (get_word_guess(g, buffer) == C_OK)
        {
            //WINNER (guessed word is correct)
            display_message_winner(g);
            g->state = WIN;
        }
        else
        {
            //Guessed word is not correct
            display_message_turn(g, buffer, C_NOK);
            g->state = WAITING_FOR_TURN;
        }
    }

    //Networking
    end_turn(g, buffer);
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
        i++;
    }

    //Line
    printf("\n__________________________________________________________________\n");

    ///Opponents Status
    printf("Opponent:\n\n");

    i = 0;
    while(g->them_solution[i] != '\0')
    {
        printf("%c ", g->them_solution[i]);
        i++;
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
