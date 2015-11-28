#define MAX_STR 32
#define NUM_LETTERS 26

/*  Structures  */
typedef struct
{
    enum gamestate state;
    char p1_word[MAX_STR];
    char p2_word[MAX_STR];
    char p1_incorrect[NUM_LETTERS + 1];
    char p2_incorrect[NUM_LETTERS + 1];
    char p1_solution[MAX_STR];
    char p2_solution[MAX_STR];
} Game;

/* Game functions */
int getLetterGuess(Game *g);
int getWordGuess(Game *g);
int checkLetterGuess(Game *g, char guess[]);
int checkWordGuess(Game *g, char guess[]);

/* User input utility functions */
void safe_string_input(char buf[MAX_STR], const char *prompt);
void safe_integer_input(char buf[MAX_STR], const char *prompt, int min, int max, int *into);
