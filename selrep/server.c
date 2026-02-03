#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PACKET_COUNT 10
#define PORT 5555
#define true 1
#define false 0

void main() {
  int sockfd, n;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];
  int received[PACKET_COUNT] = {0};
  socklen_t addr_size = sizeof(client_addr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("[-]Server - Socket Error \n");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(PORT);

  n = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (n < 0) {
    perror("[-]Server - Bind Error \n");
    close(sockfd);
    exit(1);
  }
  printf("[+]Server - Server Started \n");

  while (true) {
    bzero(buffer, sizeof(buffer));
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr,
             &addr_size);

    int pkt = atoi(buffer);

    if (pkt < 1 || pkt > PACKET_COUNT)
      continue;
    if (!received[pkt - 1]) {
      printf("[+]Server - Received packet %d\n", pkt);
      received[pkt - 1] = 1;
    } else {
      printf("[o]Server - Duplicate Packet %d\n", pkt);
    }

    sprintf(buffer, "ACK %d\n", pkt);
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr,
           addr_size);
  }
  close(sockfd);
}
