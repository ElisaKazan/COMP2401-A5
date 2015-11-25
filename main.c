#include <stdio.h>
#include <signal.h>

void handle_sigint(int signal_code);

int main(int argc, char **argv)
{
    // Set up a signal handler
    signal(SIGINT, handle_sigint);
}

void handle_sigint(int signal_name)
{
    printf("\"You can check out any time you like, but you can never leave.\"\n");
}

