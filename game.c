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
        //Add to incorrect array
        strcat(g->us_incorrect, guess);
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
    } while(check_word_guess(g, word_buffer) != C_OK);

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

    //Check if guess has already been guessed
    int i;
    for(i = 0; i < NUM_LETTERS + 1; i++)
    {
        if (g->us_incorrect[i] == guess[0])
        {
            printf("That was already guessed\n");
            return C_NOK;
        }
    }
    return C_OK;
}

/*
 * Function: check_word_guess
 * Purpose: Checks if the string is valid (does not contain any special 
 * characters or numbers) and returns C_OK if it's valid and C_NOK if
 * it's invalid (note: spaces are okay)
 * in: game struct
 * in: word guess
 */
int check_word_guess(Game *g, char *guess)
{
    //Check if the string contains special characters or numbers
    int i = 0;
    while(guess[i] != '\0')
    {
        if (!isalpha(guess[i]) || guess[i] == ' ')
        {
            return C_NOK;
        }
        i++;
    }

    return C_OK;
}
