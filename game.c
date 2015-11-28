#include "defs.h"

int getLetterGuess(Game *g)
{
    char guess_str[MAX_STR];

    //Get the players letter guess (keep asking until it is acceptable)
    do
    {
        safe_string_input(guess_str,"What is the letter you wish to guess?");
    } while(checkLetterGuess(g, guess_str) != C_OK);

    char guess = guess_str[0];

    //Check if letter is in the word
    int i;
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

int getWordGuess(Game *g)
{

}

int checkLetterGuess(Game *g, char *guess)
{
    //Check if it's not a character
    if (strlen(guess) != 2)
    {
        return C_NOK;
    }
    
    //Check if the input is invalid
    if (!isalpha(guess))
    {
        return C_NOK;
    }
    //Check if guess has already been guessed
    int i;
    for(i = 0; i < NUM_LETTERS + 1; i++)
    {
        if (g->us_incorrect[i] == guess[0])
        {
            return C_NOK;
        }
    }

    return C_OK;
}

int checkWordGuess(Game *g, char *guess)
{
}
