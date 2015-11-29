#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <signal.h>

#define MAX_STR 32
#define NUM_LETTERS 26
#define C_OK 1
#define C_NOK 0

extern int game_socket;

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
int checkLetterGuess(Game *g, char *guess);
int checkWordGuess(Game *g, char *guess);

/* User input utility functions */
void safe_string_input(char buf[MAX_STR], const char *prompt);
void safe_integer_input(char buf[MAX_STR], const char *prompt, int min, int max, int *into);

/* Networking */
// Make a server socket and bind to a port (2401)
void activate_socket_server();
// Connect to the server at `address` and stick it in the global socket.
void connect_client(char *address);
// Sends the string in Game g to the server and sets up the other side's
// string in the Game.
void do_setup(Game *g);
// Send networking update for end of turn
void end_turn(Game *g);
// Wait for next turn
void wait_turn(Game *g);
