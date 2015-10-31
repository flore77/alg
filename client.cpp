#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Game.h"
#include "helpers.h"

#define N (37*37*4)

using namespace std;

/**
 *
 * Usage ./client ip port
 *
 */

int main(int argc, char *argv[]) {
  int serverFd, port, maxFd;
  struct sockaddr_in server;
  char ip[16], buffer[N];
  fd_set readFds, auxReadFds;

  // Setting the connection parameters.
  strcpy(ip, argv[1]);
  port = atoi(argv[2]);

  // Open a TCP socket.
  if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    error("Cannot open socket!");
  }

  // Preparing the server parameters.
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  inet_aton(ip, &server.sin_addr);

  // Connect to the server.
  if ((connect(serverFd, (struct sockaddr*) &server, sizeof(server))) < 0) {
    error("Cannot connect to server!");
  }

  // Initializing the read file descriptors.
  FD_ZERO(&readFds);

  // Add the file descriptor to read data from ther server.
  FD_SET(serverFd, &readFds);

  // Set the maximal value of a file descriptor in the read set.
  maxFd = serverFd;

	Game g;
	int bytes = 0, received = 0;

  while(1) {
    auxReadFds = readFds;

    if ((select(maxFd + 1, &auxReadFds, NULL, NULL, NULL)) < 0) {
      error("Cannot listen for incoming data!");
    }

    for (int i = 0; i <= maxFd; i++) {

      // If the i socket is in the read set, this means that there is some data
      // to be read on this socket.
      if (FD_ISSET(i, &auxReadFds)) {

        // If the socket descriptor is equal to the server descriptor, there is
        // data from our server.
        if (i == serverFd) {
					received = recv(i, buffer + bytes, N, 0);
					bytes += received;

          if (received < 0) {
						cout << "mare eroare " << received << endl;
          } else {
						if (bytes == 1040) {
							bytes = 0;
							g.readGameStateFromServerInput((int *)buffer);
							g.prettyPrint();
						} else {
							
						}
					}
        }
      }
    }
  }


}
