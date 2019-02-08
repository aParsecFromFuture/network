#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	if(argc != 4){
		printf("usage : ./client <server_address> <port_number> <message>\n");
		return 1;
	}

	int sock,len,read_size;
	char *message_send = argv[3];

	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1){
		printf("socket couldn't created");
		exit(1);
	}	
	printf("socket created\n");

	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));

	sendto(sock,message_send,strlen(message_send),MSG_CONFIRM,(struct sockaddr*)&server,sizeof(server));
	printf("the message sended!\n");

	close(sock);
}
