#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	int sock,client_sock;
	char msg_send[1024],msg_recv[1024];
	struct sockaddr_in server,client;

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		fprintf(stderr,"Socket coudln't create\n");
		return 1;
	}
	printf("Socket created\n");

	if(bind(sock,(struct sockaddr*)&server,sizeof(server)) == -1){
		fprintf(stderr,"Bind failed\n");
		return 1;
	}
	printf("Bind done\n");

	printf("Waiting for connection...\n");
	listen(sock,1);

	int c = sizeof(struct sockaddr_in);

	client_sock = accept(sock,(struct sockaddr*)&client,(socklen_t*)&c);
	if(client_sock == -1){
		fprintf(stderr,"Client couldn't connect\n");
		return 1;
	}
	printf("Client connected\n");

	while(1){
		if(recv(client_sock,msg_recv,sizeof(msg_recv),0) == -1){
			break;
		}
		printf("Client : %s\n",msg_recv);
		printf("Server : ");
		scanf("%s",msg_send);
		if(send(client_sock,msg_send,strlen(msg_send) + 1,0) == -1){
			break;
		}
	}
	close(sock);
	return 0;
}
