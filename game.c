#include "defs.h"

/*
 * Function: get_letter_guess
 * Purpose: Asks the player for a letter (keeps asking until it's acceptable), 
 * checks if the letter is in the word. If the guess was correct, returns C_OK 
 * else adds letter to incorrect array and returns C_NOK
 * in: game struct (has the solution, incorrect array and word) 
 * out: letter guess
 */
int get_letter_guess(Game *g, char *letter_buffer)
{
    //Get the players letter guess (keep asking until it is acceptable)
    do
    {
        safe_string_input(letter_buffer,"What is the letter you wish to guess?");
    } while(check_letter_guess(g, letter_buffer) != C_OK);

    char guess = letter_buffer[0];

    //Check if letter is in the word
    int i = 0;
    int num_correct = 0;
    while (g->them_word[i] != '\0')
    {
        if(g->them_word[i] == guess)
        {
            //Make change to soltuion
            g->us_solution[i] = guess;
            //Increment counter
            num_correct ++;
        }
        i++;
    }

    //Was the guess correct?
    if (num_correct > 0)
    {
        return C_OK;
    }
    else
    {
        return C_NOK;
    }
}

/*
 * Function: get_word_guess
 * Purpose: Asks player for a word guess (continues asking until it is acceptable). 
 * Checks if the word is correct and returns C_OK otherwise returns C_NOK 
 * in: game struct 
 * out: word that player guessed
 */
int get_word_guess(Game *g, char *word_buffer)
{
    //Get the players letter guess (keep asking until it is acceptable)
    do
    {
        safe_string_input(word_buffer,"What is the word you wish to guess?");
    } while(check_word_input(word_buffer) == C_NOK);

    //Check if the word is correct
    if (strcmp(word_buffer, g->them_word) == 0)
    {
        //They guessed correctly
        return C_OK;
    }
    else
    {
        //Their guess is incorrect 
        return C_NOK;
    }
}

/*
 * Function: check_letter_guess
 * Purpose: Checks if the letter guess is acceptable (i.e. It's a character
 * that is in the alphabet (not a special character) and it has not been 
 * guessed already
 * in: game struct 
 * in: letter guess
 */
int check_letter_guess(Game *g, char *guess)
{
    //Check if it's not a character
    if (strlen(guess) != 1)
    {
        printf("That is not a single character\n");
        return C_NOK;
    }
    
    //Check if the input is invalid
    if (!isalpha(guess[0]))
    {
        printf("That letter is not alphabetical\n");
        return C_NOK;
    }

    return C_OK;
}

/*
 * Function: check_word_input
 * Purpose: Checks if the string is valid (does not contain any special 
 * characters or numbers) and returns C_OK if it's valid and C_NOK if
 * it's invalid (note: spaces are okay)
 * in: word guess
 */
int check_word_input(char *guess)
{
    //Check if the string contains special characters or numbers
    int i = 0;
    while(guess[i] != '\0')
    {
        if (!isalpha(guess[i]) && guess[i] != ' ')
        {
            return C_NOK;
        }
        i++;
    }

    return C_OK;
}

/*
 * Function: turn
 * Purpose: This function plays one turn in the game. First the player
 * chooses between guessing a word or a letter (1 or 2). Then they
 * submit their guess. If they are correct they win the game and the
 * state changes, otherwise the state changes to WAITING_FOR_TURN
 * in: game struct 
 */
void turn(Game *g)
{
    int selection;
    char buffer[MAX_STR];
    int correct = -1;

    //Will player guess a letter or word?
    safe_integer_input(buffer, "What would you like to guess?\n\t(1) a letter\n\t(2) a word\n", 1, 2, &selection);

    if (selection == 1)
    {
        //Letter
        if (get_letter_guess(g, buffer) == C_OK)
        {
            //Guessed letter is in the word
            correct = C_OK;

            //Check if the word is complete
            if (strcmp(g->us_solution, g->them_word) == 0)
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
            correct = C_NOK;
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
            correct = C_OK;
            display_message_winner(g);
            g->state = WIN;
        }
        else
        {
            //Guessed word is not correct
            correct = C_NOK;
            display_message_turn(g, buffer, C_NOK);
            g->state = WAITING_FOR_TURN;
        }
    }
    //Networking
    end_turn(g, buffer, correct);
}

/*
 * Function: display_game_status
 * Purpose: Prints the game status (player and opponents solutions)
 * in: game struct
 */
void display_game_status(Game *g)
{
    //Prints the game status
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
    printf("\n__________________________________________________________________\n");

}

/*
 * Function: display_message_turn
 * Purpose: Prints the players guess (correct or incorrect)
 * in: game struct
 * in: players guess (letter or word)
 * in: correct boolean (0 or 1)
 */
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
    display_game_status(g);
}

/*
 * Function: display_message_waiting
 * Purpose: Prints the opponenets guess while you are waiting for 
 * their turn to be over
 * in: game struct
 * in: players guess ( letter or word)
 * in: correct boolean (0 or 1)
 */
void display_message_waiting(Game *g, char *guess, int correct)
{
    //They guessed correctly
    if (correct)
    {
        printf("Opponent guessed '%s' which is correct\n", guess);
    }
    //They guessed incorrectly
    else
    {
        printf("Opponent guessed '%s' which is incorrect\n", guess);
    }
}

/*
 * Function: display_message_winner
 * Purpose: Prints winner message 
 * in: game struct
 */
void display_message_winner(Game *g)
{
    //Winner
    printf("WINNER!!! The opponent's word was %s\n", g->us_word);
}


/*
 * Function: fill_underscores_except_spaces
 * Purpose: Fills buffer "to" with underscores except when it encounters a
 *          space in which case it fills the "to" buffer location with a space.
 * in: A buffer to read from
 * out: A buffer to write underscores to except spaces
 */
void fill_underscores_except_spaces(const char *from, char *to)
{
    int soln_len = strlen(from);
    int i = 0;
    while (i < soln_len)
    {
        if (from[i] == ' ')
        {
            to[i] = ' ';
        }
        else
        {
            // A 'blank'
            to[i] = '_';
        }
        i++;
    }

    to[i] = 0;
}

/*
 * Function: do_setup
 * Purpose: Does setup tasks: ask the user for their word, check that input,
 *          gets the other word from the network, and fills in solutions.
 * in/out: the Game to modify and read from (we read the word from it after
           writing it in, so it's technically an output and input parameter)
 */
void do_setup(Game *g)
{
    // Ask the user for the word
    do
    {
        safe_string_input(g->us_word, "What is the word you want your opponent to guess?");
    } while (check_word_input(g->us_word) == C_NOK);

    do_network_setup(g);

    fill_underscores_except_spaces(g->them_word, g->us_solution);
    fill_underscores_except_spaces(g->us_word, g->them_solution);
}
