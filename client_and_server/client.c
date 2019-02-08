#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	int sock = socket(AF_INET,SOCK_STREAM,0);
	char msg_send[1024] = {0},msg_recv[1024] = {0};
	struct sockaddr_in server;

	if(argc != 3){
		printf("usage : ./client <server_address> <port_number>\n");
		return 1;
	}

	if(sock == -1){
		printf("socket couldn't created\n");
		return 1;
	}
	puts("socket created");

	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&server,sizeof(server)) < 0){
		printf("connection failed\n");
		return 1;
	}else{
		printf("connected\n");
		
		printf("Client : ");
		scanf("%s",msg_send);

		while(send(sock,msg_send,strlen(msg_send) + 1,0)){;
			recv(sock,msg_recv,sizeof(msg_recv),0);
			printf("Server : %s\n",msg_recv);
			printf("Client : ");
			scanf("%s",msg_send);
		}

		return 0;
	}
}
