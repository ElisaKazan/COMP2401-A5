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
    char guess[MAX_STR];

    //Get the players letter guess (keep asking until it is acceptable)
    do
    {
        safe_string_input(guess,"What is the word you wish to guess?");
    } while(checkWordGuess(g, guess) != C_OK);

    //Check if the word is correct
    if (strcmp(guess, g->them_word) == 0)
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

int checkLetterGuess(Game *g, char *guess)
{
    //Check if it's not a character
    if (strlen(guess) != 2)
    {
        printf("not a char");
        return C_NOK;
    }
    
    //Check if the input is invalid
    if (!isalpha(guess))
    {
        printf("not alphabet");
        return C_NOK;
    }

    //Check if guess has already been guessed
    int i;
    for(i = 0; i < NUM_LETTERS + 1; i++)
    {
        if (g->us_incorrect[i] == guess[0])
        {
            printf("already guessed");
            return C_NOK;
        }
    }
    printf("should work fine");
    return C_OK;
}

int checkWordGuess(Game *g, char *guess)
{
    //Check if the string contains special characters or numbers
    int i = 0;
    while(guess[i] != '\0')
    {
        if (!isalpha(guess[i]) || guess[i] != ' ')
        {
            return C_NOK;
        }
        i++;
    }
}
