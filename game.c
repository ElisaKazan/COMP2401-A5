#include "defs.h"

int get_letter_guess(Game *g, char *letter_buffer)
{
    //Get the players letter guess (keep asking until it is acceptable)
    do
    {
        safe_string_input(letter_buffer,"What is the letter you wish to guess?");
    } while(check_word_guess(g, letter_buffer) != C_OK);

    char guess = letter_buffer[0];

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

int check_letter_guess(Game *g, char *guess)
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

int check_word_guess(Game *g, char *guess)
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
