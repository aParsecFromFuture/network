#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc,char **argv){
	int sock_dest,client_sock,c,read_size;
	struct sockaddr_in server,client;

	if(argc != 2){
		printf("usage : ./server <port_number>\n");
		return 1;
	}

	sock_dest = socket(AF_INET,SOCK_STREAM,0);
	if(sock_dest == -1){
		printf("socket couln't created\n");
		return 1;
	}
	puts("socket created");	
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));

	if(bind(sock_dest,(struct sockaddr*)&server,sizeof(server)) < 0){
		printf("bind failed\n");
		return 1;
	}
	puts("bind done\n");

	listen(sock_dest,3);

	puts("waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);

	client_sock = accept(sock_dest,(struct sockaddr*)&client,(socklen_t*)&c);
	if(client_sock < 0){
		puts("failed\n");
		return 1;
	}

	puts("the client connected\n");

	char msg_send[1024] = {0},msg_recv[1024] = {0};
	while((read_size = recv(client_sock,msg_recv,sizeof(msg_recv),0))){
		printf("Client : %s\n",msg_recv);
		printf("Server : ");
		scanf("%s",msg_send);
		send(client_sock,msg_send,strlen(msg_send) + 1,0);
	}

	return 0;
}
