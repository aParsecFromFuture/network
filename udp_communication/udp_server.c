#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	int sock,len;
	char msg_send[1024],msg_recv[1024];
	struct sockaddr_in server,client;

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));

	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1){
		fprintf(stderr,"Socket couldn't create\n");
		return 1;
	}
	printf("Socket created\n");

	if(bind(sock,(struct sockaddr*)&server,sizeof(server)) == -1){
		fprintf(stderr,"Bind failed\n");
		return 1;
	}
	printf("Bind done\n");

	while(1){
		if(recvfrom(sock,msg_recv,sizeof(msg_recv),MSG_WAITALL,(struct sockaddr*)&client,&len) == -1)break;
		printf("Client : %s\n",msg_recv);
		printf("Server : ");
		scanf("%s",msg_send);
		if(sendto(sock,msg_send,strlen(msg_send) + 1,0,(struct sockaddr*)&client,len) == -1)break;
	}
	close(sock);
	return 0;
}
