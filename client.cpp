#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Game.h"
#include "helpers.h"

#define IP "192.168.56.101"
#define PORT 10004

#define N (37*37*4)

using namespace std;

/**
 *
 * Usage ./client ip port
 *
 */

//FILE * mylog = fopen("mylog.txt", "a");

void log(string s) {
	//fprintf(mylog, "%s\n", (char *)s.c_str());
}

int send_all(int socket, const char *buffer, size_t length, int flags)
{
    ssize_t n;
    const char *p = buffer;
    while (length > 0)
    {
        n = send(socket, p, length, flags);
        if (n <= 0) break;
        p += n;
        length -= n;
    }
    return (n <= 0) ? -1 : 0;
}

int main(int argc, char *argv[]) {
  int serverFd, port, maxFd;
  struct sockaddr_in server;
  char ip[16], buffer[N];
  fd_set readFds, auxReadFds;

  // Setting the connection parameters.
  strcpy(ip, argv[1]);
  port = atoi(argv[2]);

	//strcpy(ip, IP);
	//port = PORT;

  // Open a TCP socket.
  if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    error("Cannot open socket!");
  }

  // Preparing the server parameters.
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  inet_aton(ip, &server.sin_addr);

	printf("before connect\n");

  // Connect to the server.
  if ((connect(serverFd, (struct sockaddr*) &server, sizeof(server))) < 0) {
    error("Cannot connect to server!");
  }

	printf("after connect\n");

  // Initializing the read file descriptors.
  FD_ZERO(&readFds);

  // Add the file descriptor to read data from ther server.
  FD_SET(serverFd, &readFds);

  // Set the maximal value of a file descriptor in the read set.
  maxFd = serverFd;

	Game g;
	int bytes = 0, received = 0;
	bool firstTime = true, secondTime = true;
	int bytesToRead = -1;

  while(1) {
    auxReadFds = readFds;

    if ((select(maxFd + 1, &auxReadFds, NULL, NULL, NULL)) < 0) {
      error("Cannot listen for incoming data!");
    }


      // If the i socket is in the read set, this means that there is some data
      // to be read on this socket.
      if (FD_ISSET(serverFd, &auxReadFds)) {

        // If the socket descriptor is equal to the server descriptor, there is
        // data from our server.
					
					if (firstTime) {
						while (received < 4) {
							received += recv(serverFd, buffer + received, 1, 0);
						}

						g.readId((int *) buffer);

						received = 0;
						firstTime = false;
					} else {
						if (secondTime) {
							while (received < 20) {
								received += recv(serverFd, buffer + received, 1, 0);
							}
							g.readHeader((int *)buffer);

							bytesToRead = (5 + g.getN() * g.getM()) * 4;
							
							while (received < bytesToRead) {
								received += recv(serverFd, buffer + received, 1, 0);
							}

							g.readMatrix((int *)(buffer + 20));
							g.prettyPrint();
								
							g.makeMove((int *)buffer);
							printf("trimit %d%d\n", *((int*)buffer),*((int*)buffer+1));

							secondTime = false;
						} else {
							received = 0;
							while (received < bytesToRead) {
								received += recv(serverFd, buffer + received, 1, 0);
								if (received == 0) {
									printf("S-a terminat cu bine!\n");
									return 0;
								}
							}
							g.readMatrix((int *)(buffer + 20));
							g.prettyPrint();
								
							g.makeMove((int *)buffer);
							printf("trimit %d%d\n", *((int*)buffer),*((int*)buffer+1));
							
							if (send_all(serverFd, buffer, 8, 0) < 0) {
								error("Eroare la send_all");
								return -1;
							}
						}
					}
					
					
					
					/*received = recv(serverFd, buffer + bytes, N, 0);
					bytes += received;

					if (received == 0) {
						log("Programul s-a terminat!");
						return 0;
					} else if (received < 0) {
						log("eroare de la server");
						error("eroare de la server");
						return -1;
					} else {
						if (firstTime) {
							g.readId((int *)buffer);
							firstTime = false;
							bytes = 0;
						} else {
							if (secondTime) {
								if (bytes < 20) continue;
								
								g.readHeader((int *)buffer);

								printf("N = %d\nM = %d\n", g.getN(), g.getM());

								bytesToRead = 20 + g.getN() * g.getM() * 4;
					
								if (bytes >= bytesToRead) {
								
									printf("In second time\n");
									printf("bytes = %d\nbytesToRead = %d\n", bytes, bytesToRead);

									bytes = 0;
									
									g.readMatrix((int *)(buffer + 20));
									g.prettyPrint();
								
									g.makeMove((int *)buffer);
									printf("trimit %d%d\n", *((int*)buffer),*((int*)buffer+1));
									if (send_all(serverFd, buffer, 8, 0) < 0) {
										log("Eroare la send_all");
										error("Eroare la send_all");
										return -1;
									}

									if (bytes > bytesToRead) {
										log("Ne-a dat mai mult decat matricea");
										int k = 0;
										for (int i = bytesToRead; i < bytes; ++i)
											buffer[k++] = buffer[i];
										bytes -= bytesToRead;
									}
								}

								secondTime = false;
							} else {
								printf("In third time\n");

								if (bytes >= bytesToRead) {
									bytes = 0;
									g.readHeader((int *)buffer);
									g.readMatrix((int *)(buffer + 20));
									g.prettyPrint();

									g.makeMove((int *)buffer);
									
									printf("trimit %d-%d\n", *((int*)buffer),*((int*)buffer+1));
									if (send_all(serverFd, buffer, 8, 0) < 0) {
										log("Eroare la send_all");
										error("Eroare la send_all");
										return -1;
									}
									
									if (bytes > bytesToRead) {
										log("Ne-a dat mai mult decat matricea");
										int k = 0;
										for (int i = bytesToRead; i < bytes; ++i)
											buffer[k++] = buffer[i];
										bytes -= bytesToRead;
									}
								}
							}	
						}*/
					}		
  }
}
