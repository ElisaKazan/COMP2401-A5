#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <ctype.h>

#define MAX_STR 32
#define NUM_LETTERS 26
#define C_OK 1
#define C_NOK 0

extern int game_socket;

enum gamestate
{
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
    // 1 if we're the server, 0 if we're the client.
    char server;
} Game;

/* Game functions */
int get_letter_guess(Game *g, char *letter_buffer);
int get_word_guess(Game *g, char *word_buffer);
int check_letter_guess(Game *g, char *guess);
int check_word_input(char *guess);
void turn(Game *g);
void display_game_status(Game *g);
void display_message_turn(Game *g, char *guess, int correct);
void display_message_waiting(Game *g, char *guess, int correct);
void display_message_winner(Game *g);
void do_setup(Game *g);

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
void do_network_setup(Game *g);
// Send networking update for end of turn
void end_turn(Game *g, char *buffer, int correct);
// Wait for the other side to finish their turn
void wait_for_turn(Game *g);
