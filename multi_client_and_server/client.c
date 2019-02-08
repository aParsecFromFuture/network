#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

int sock;

void *send_message(void*);
void *recv_message(void*);

int main(int argc,char*argv[]){
	if(argc != 3){
		printf("usage : ./client <server_address> <port_number>\n");
		return 1;
	}
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		printf("socket couldn't created\n");
		return 1;
	}
	printf("socket created\n");

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&server,sizeof(server)) == -1){
		printf("connection failed\n");
		return 1;
	}
	printf("connected\n");
	
	pthread_t t1,t2;

	pthread_create(&t1,NULL,send_message,NULL);
	pthread_create(&t2,NULL,recv_message,NULL);

	pthread_exit(NULL);
}

void *send_message(void*arg){
	char str_send[1024];
	memset(str_send,0,sizeof(str_send));
	while(1){
		fgets(str_send,sizeof(str_send),stdin);
		send(sock,str_send,strlen(str_send),0);
	}
}

void *recv_message(void*arg){
	char str_recv[1024];
	memset(str_recv,0,sizeof(str_recv));
	int read_size = 0;
	while(1){
		read_size = recv(sock,str_recv,sizeof(str_recv),0);
		str_recv[read_size] = '\0';
		printf("%s",str_recv);
	}
}
