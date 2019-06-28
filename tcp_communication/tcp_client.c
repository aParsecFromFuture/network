#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	int sock;
	char msg_send[1024],msg_recv[1024];
	struct sockaddr_in server;
	
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		fprintf(stderr,"Socket couldn't create\n");
		return 1;
	}
	printf("Socket created\n");

	if(connect(sock,(struct sockaddr*)&server,sizeof(server)) == -1){
		fprintf(stderr,"Connection failed\n");
		return 1;
	}
	printf("Connected\n");
	while(1){
		printf("Client : ");
		scanf("%s",msg_send);
		if(send(sock,msg_send,strlen(msg_send) + 1,0) == -1){
			break;
		}
		if(recv(sock,msg_recv,sizeof(msg_recv),0) == -1){
			break;
		}
		printf("Server : %s\n",msg_recv);
	}
	close(sock);
	return 0;
}
