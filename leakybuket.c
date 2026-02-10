#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PacketCount 10

struct Bucket {
  int bucket[100];
  int bkSize;
  int remSize;
  int transTime;
  int op;
  int clk;
  int rate;
};

int main() {
  srand(time(NULL));

  struct Bucket bk;
  int packets[PacketCount], i = 0;

  bk.remSize = 0;

  for (i = 0; i < PacketCount; i++) {
    packets[i] = (rand() % 6 + 1) * 10;
    printf("%d ", packets[i]);
  }
  printf("\nEnter the output rate: ");
  scanf("%d", &bk.rate);
  printf("Enter the bucketSize: ");
  scanf("%d", &bk.bkSize);

  printf("\n-----------------------------------------\n");

  i = 0;
  while (i < PacketCount || bk.remSize > 0) {
    if (i < PacketCount) {
      if (packets[i] + bk.remSize > bk.bkSize)
        printf("\n\nBucket Size exceeded! Packet %d overflow\n\n", packets[i]);
      else {
        bk.remSize += packets[i];
        printf("\nIncoming Packet size: %d", packets[i]);
        printf("\nBytes to transmite remaining: %d\n", bk.remSize);
      }
      i++;
    }
    bk.transTime = (rand() % 4 + 1) * 10;
    printf("\nTime left to transmit: %d\n", bk.transTime);
    for (bk.clk = 10; bk.clk <= bk.transTime; bk.clk += 10) {
      sleep(1);
      if (bk.remSize > 0) {
        if (bk.remSize < bk.rate) {
          bk.op = bk.remSize;
          bk.remSize = 0;
        } else {
          bk.op = bk.rate;
          bk.remSize -= bk.rate;
        }
        printf("\nPacket of %d transmitted", bk.op);
        printf("\nBytes remaining to transmit: %d \n", bk.remSize);
      } else {
        printf("\nNo Packets to transmit!\n");
        break;
      }
    }
  }
  return 0;
}
