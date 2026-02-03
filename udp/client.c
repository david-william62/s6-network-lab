#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void main() {
  char *ip = "127.0.0.1";
  int port = 5555;
  int sockfd;

  struct sockaddr_in addr;
  char buffer[1024];
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("[-]Socket Creation Failed");
    exit(0);
  }
  printf("[CLIENT] UDP socket created\n");

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  memset(buffer, 0, sizeof(buffer));
  printf("[CLIENT] Enter the data: ");
  scanf(" %[^\n]", buffer);

  sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&addr,
         sizeof(addr));
  printf("[CLIENT] Sent to server: %s\n", buffer);

  memset(buffer, 0, sizeof(buffer));
  addr_size = sizeof(addr);

  recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr,
           &addr_size);
  printf("[CLIENT] Received from server: %s\n", buffer);

  close(sockfd);
}
