#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PACKET_COUNT 10
#define WINDOW_SIZE 3

void main() {
  int sockfd;
  struct sockaddr_in server_addr;
  char buffer[1024];
  int acked[PACKET_COUNT] = {0};
  int base = 0, next = 0;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("[-][Client] Socket Error \n");
    exit(1);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(5555);

  printf("[+] Client Started \n");

  while (base < PACKET_COUNT) {
    while (next < base + WINDOW_SIZE && next < PACKET_COUNT) {
      sprintf(buffer, "%d", next + 1);
      printf("[o]Client - Sending Packet %d\n", next + 1);

      sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr,
             sizeof(server_addr));
      next++;
    }

    bzero(buffer, sizeof(buffer));
    recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

    int pkt;
    sscanf(buffer, "ACK %d", &pkt);
    printf("[+]Client - ACK Received for packet %d\n", pkt);
    acked[pkt - 1] = 1;

    while (base < PACKET_COUNT && acked[base])
      base++;
  }
  close(sockfd);
}
