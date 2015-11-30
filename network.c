#include "defs.h"
#include <stdlib.h>
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

}
