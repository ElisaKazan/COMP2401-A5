#define MAX_STR 32
#define NUM_LETTERS 26
#define C_OK 1
#define C_NOK 0

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

/*  Functions  */
int getLetterGuess(Game *g);
int getWordGuess(Game *g);
int checkLetterGuess(Game *g, char guess[]);
int checkWordGuess(Game *g, char guess[]);
