#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define WINDOW_SIZE 4

int main() {
  char *ip = "127.0.0.1";
  int port = 5556;

  int sockfd;
  struct sockaddr_in addr;
  char buffer[1024];
  socklen_t addr_size;

  int base = 0;
  int next_seq_num = 0;
  int ack_count = 0;

  int total_packets = 10;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket error");
    exit(1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  struct timeval timeout;
  timeout.tv_sec = 2;
  timeout.tv_usec = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  while (ack_count < total_packets) {
   while (next_seq_num < base + 4 && next_seq_num < total_packets) {
      memset(buffer, 0, sizeof(buffer));
      sprintf(buffer, "DATA:%d", next_seq_num);

      printf("[CLIENT] Sending DATA seq=%d\n", next_seq_num);
      sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&addr,
             sizeof(addr));
      next_seq_num++;
    }

    addr_size = sizeof(addr);
    int rec = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                       (struct sockaddr *)&addr, &addr_size);

    if (rec < 0) {
      printf("[CLIENT] Timeout! Resending window from seq=%d\n", base);
      next_seq_num = base;
    } else {
      int ack;
      sscanf(buffer, "ACK:%d", &ack);
      printf("[CLIENT] Received ACK seq=%d\n", ack);

      if (ack >= base) {
        ack_count += (ack - base + 1);
        base = ack + 1;
      }
    }
  }

  printf("[CLIENT] All packets sent and acknowledged.\n");
  close(sockfd);
  return 0;
}

