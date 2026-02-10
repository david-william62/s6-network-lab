#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PACKET_COUNT 10
#define WINDOW_SIZE 3
#define TIMEOUT 10

typedef struct Packet {
  int id;
  int sent;
};

void sendPacket(Packet *packet) {}

void main() { Packet pkts[PACKET_COUNT]; }
