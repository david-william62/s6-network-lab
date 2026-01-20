#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <pthread.h>

void main() {
	char *ip = "127.0.0.100";
	int port = 5555;
	pthread_t timeoutThread;
	int sockfd;
	struct sockaddr_in addr;
	char buffer[1024];
	socklen_t addr_size;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd<0) {
    perror("[-] Socket error");
    exit(1);
  }
  memset(&addr,'\0',sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_port=htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
  int packetCount = 0;
  while(packetCount <= 10) {
    bzero(buffer, 1024);
    printf("Enter data packet >> ");
    scanf("%s",buffer);
    sprintf(buffer,"%d",packetCount);
    printf("[+]Client >> Sent Packet: %s\n",buffer);
    sendto(sockfd,buffer, 1024, 0, (struct sockaddr *)&addr,sizeof(addr));
    bzero(buffer, 1024);
    addr_size = sizeof(addr);
    int rec = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&addr, &addr_size);
    if(rec<0) {
      printf("[-]Client >> Timeout error Sending packet again\n");
    } else {
      printf("[+]Client >> Recieved acknowledgement for packet %s\n",buffer);
      sleep(1);
      packetCount++;
    }
    sleep(1);
  }
}
