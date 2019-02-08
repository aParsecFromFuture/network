#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc,char **argv){
	if(argc != 3){
		printf("usage : ./main <server_address> <port_number>\n");
		return 1;
	}

	int sock,read_size;
	char request[] = "GET /index.html HTTP/1.1\n\n\r";
	char response[1024] = {0};
	struct sockaddr_in server;
	struct hostent* host = NULL;

	host = gethostbyname(argv[1]);
	if(host == NULL){
		printf("host not found!\n");
		return 1;
	}
	printf("host found\n");

	sock = socket(AF_INET,SOCK_STREAM,0);

	memcpy(&server.sin_addr.s_addr,host->h_addr,host->h_length);
	server.sin_port = htons(atoi(argv[2]));
	server.sin_family = AF_INET;

	if(connect(sock,(struct sockaddr*)&server,sizeof(server)) != 0){
		printf("connection failed\n");
		return 1;
	}
	printf("connected\n");
	
	send(sock,request,sizeof(request),0);

	while((read_size = recv(sock,response,sizeof(response),0)) > 0){
		response[read_size] = '\0';
		printf("%s",response);
	}

	return 0;
}
