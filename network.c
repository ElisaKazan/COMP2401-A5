#include "defs.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUMBER 2401

void activate_socket_server() {
    // Socket for binding on and return codes of various
    // socket functions
    int server_socket, ret;
    struct sockaddr_in addr;

    // Create the socket so that we can listen - note that this
    // isn't where communication will happen, just where we
    // we listen for clients.
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket < 0) {
        printf("Couldn't open socket.\n");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short) PORT_NUMBER);

    ret = bind(server_socket,
               (struct sockaddr *) &addr,
               sizeof(addr));
    if (ret < 0) {
        printf("Bind failed :-(\n");
        exit(1);
    }

    ret = listen(server_socket, 5);
    if (ret < 0) {
        printf("Couldn't listen! :-(\n");
        exit(1);
    }

    size_t addr_size = sizeof(addr);

    game_socket = accept(server_socket, 
                         (struct sockaddr *) &addr,
                         &addr_size);
    if (game_socket < 0) {
        printf("Couldn't accept connection!\n");
        exit(1);
    }
}

void end_turn(Game *g, char *buffer) {
    send(game_socket, buffer, strlen(buffer)+1, 0);
    send(game_socket, &(g->state), sizeof(g->state), 0);
    send(game_socket, &(g->us_incorrect), sizeof(g->us_incorrect), 0);
    send(game_socket, &(g->us_solution), sizeof(g->us_incorrect), 0);

    char buffer_recv[MAX_STR];
    enum gamestate other_state;

    recv(game_socket, buffer_recv, sizeof(buffer_recv), 0);
    recv(game_socket, &other_state, sizeof(enum gamestate), 0);
    recv(game_socket, &(g->them_incorrect), sizeof(g->them_incorrect), 0);
    recv(game_socket, &(g->them_solution), sizeof(g->them_solution), 0);

    display_message_waiting(g, buffer_recv, other_state == WIN);

    if (other_state == WIN) {
        g->state = LOSE;
    }
    else {
        g->state = TURN;
    }
}
