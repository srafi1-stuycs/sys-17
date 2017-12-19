#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
    while (1) {
        int well_known = server_setup();
        if (!fork()) {
            subserver(well_known);
        }
    }
}

void subserver(int from_client) {
    int to_client = server_connect(from_client);
    while (1) {
        char buffer[HANDSHAKE_BUFFER_SIZE];
        read(from_client, buffer, HANDSHAKE_BUFFER_SIZE);
        printf("Received: [%s]\n", buffer);
        process(buffer);
        write(to_client, buffer, HANDSHAKE_BUFFER_SIZE);
        printf("Sent\n");
    }
}

void process(char * s) {
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (s[i] != 0) {
            s[i] = toupper(s[i]);
        }
    }
}
