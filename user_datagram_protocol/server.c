#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	int sock,len,read_size;
	
	if(argc != 2){
		printf("usage : ./server <port_number>\n");
		return 1;
	}

	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1){
		printf("socket couldn't created");
		exit(1);
	}
	printf("socket created\n");

	struct sockaddr_in server,client;
	memset(&client,0,sizeof(client));
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));

	if(bind(sock,(struct sockaddr*)&server,sizeof(server)) == -1){
		printf("bind failed");
		exit(1);
	}
	printf("bind done\n");
	printf("waiting for message...\n");

	char recv_message[1024];
	read_size = recvfrom(sock,recv_message,sizeof(recv_message),MSG_WAITALL,(struct sockaddr*)&client,&len);
	recv_message[read_size] = '\0';
	printf("client message : %s\n",recv_message);

	close(sock);
}
