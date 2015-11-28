#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define MAX_STR 32
#define NUM_LETTERS 26
#define C_OK 1
#define C_NOK 0

enum gamestate
{
    CONNECTING,
    SETUP,
    TURN,
    WAITING_FOR_TURN,
    WIN,
    LOSE
};

/*  Structures  */
typedef struct
{
    enum gamestate state;
    char us_word[MAX_STR];
    char them_word[MAX_STR];
    char us_incorrect[NUM_LETTERS + 1];
    char them_incorrect[NUM_LETTERS + 1];
    char us_solution[MAX_STR];
    char them_solution[MAX_STR];
} Game;

/* Game functions */
int getLetterGuess(Game *g);
int getWordGuess(Game *g);
int checkLetterGuess(Game *g, char guess[]);
int checkWordGuess(Game *g, char guess[]);

/* User input utility functions */
void safe_string_input(char buf[MAX_STR], const char *prompt);
void safe_integer_input(char buf[MAX_STR], const char *prompt, int min, int max, int *into);
