#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  char *ip = "127.0.0.1";
  int port = 5556;

  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];
  socklen_t addr_size;

  int expected_seq = 0;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket error");
    exit(1);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind error");
    exit(1);
  }

  printf("[SERVER] Go-Back-N ARQ Server running...\n");

  while (1) {
    memset(buffer, 0, sizeof(buffer));
    addr_size = sizeof(client_addr);

    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr,
             &addr_size);

    int seq;
    sscanf(buffer, "DATA:%d", &seq);

    if (seq == expected_seq) {
      printf("[SERVER] Received DATA seq=%d (Accepted)\n", seq);

      if (rand() % 5 != 0) { 
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "ACK:%d", seq);
        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&client_addr, sizeof(client_addr));
        printf("[SERVER] Sent ACK seq=%d\n", seq);
      } else {
        printf("[SERVER] Simulated ACK Loss for seq=%d\n", seq);
      }
      expected_seq++;
    } else {
      printf("[SERVER] Received DATA seq=%d (Discarded, expected seq=%d)\n",
             seq, expected_seq);
      if (expected_seq > 0) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "ACK:%d", expected_seq - 1);
        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&client_addr, sizeof(client_addr));
        printf("[SERVER] Resent ACK seq=%d\n", expected_seq - 1);
      }
    }
  }

  close(sockfd);
  return 0;
}

