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
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];
  socklen_t addr_size;
  int n;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("[-] Socket creation failed");
    exit(1);
  }
  printf("[SERVER] UDP socket created\n");

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (n < 0) {
    perror("[-] Bind failed");
    exit(1);
  }
  printf("[SERVER] Bound to %s:%d\n", ip, port);

  memset(buffer, 0, sizeof(buffer));
  addr_size = sizeof(client_addr);

  recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr,
           &addr_size);
  printf("[SERVER] Received from client: %s\n", buffer);

  strcpy(buffer, "Data successfully received");
  sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr,
         sizeof(client_addr));
  printf("[SERVER] Sent to client: %s\n", buffer);

  close(sockfd);
}
