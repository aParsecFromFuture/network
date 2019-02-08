#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
	struct sockaddr_in addr;
	int *client_list;
	int sock;
	int capacity;
}server;

void *recv_message(void*);
server* create_server(short);
void listen_clients(server*,int);

server *my_server;

int main(int argc,char*argv[]){	
	if(argc != 2){
		printf("usage : ./server <port_number\n");
		return 1;
	}
	my_server = create_server(atoi(argv[1]));
	listen_clients(my_server,4);	
}

void* recv_message(void *arg){
	int *p = (int*)arg;
	struct sockaddr_in client;

	int c = sizeof(struct sockaddr_in);

	*p = accept(my_server->sock,(struct sockaddr*)&client,(socklen_t*)&c);

	printf("client connected!\n");
	
	char str_send[1024],str_recv[1024],ip[INET_ADDRSTRLEN];

	struct sockaddr_in *pv4addr = (struct sockaddr_in*)&client;
	struct in_addr ipaddr = pv4addr->sin_addr;
	inet_ntop(AF_INET,&ipaddr,ip,INET_ADDRSTRLEN);

	memset(str_send,0,sizeof(str_send));
	memset(str_recv,0,sizeof(str_recv));
	int read_size = 0;
	while((read_size = recv(*p,str_recv,sizeof(str_recv),0)) > 0){
		str_recv[read_size] = '\0';

		sprintf(str_send,"client (%s) :",ip);
		strcat(str_send,str_recv);

		for(int i = 0;i < my_server->capacity;i++){
			if(p != &my_server->client_list[i]){
				send(my_server->client_list[i],str_send,strlen(str_send),0);
			}
		}
	}
}

server* create_server(short port){
	server *s = malloc(sizeof(server));
	s->sock = socket(AF_INET,SOCK_STREAM,0);
	if(s->sock == -1){
		printf("socket couldn't created\n");
		exit(1);
	}

	printf("socket created\n");

	s->addr.sin_family = AF_INET;
	s->addr.sin_addr.s_addr = INADDR_ANY;
	s->addr.sin_port = htons(port);
	if(bind(s->sock,(struct sockaddr*)&s->addr,sizeof(s->addr)) == -1){
		printf("bind failed\n");
		exit(1);
	}
	printf("bind done\n");

	return s;
}

void listen_clients(server *s,int count){
	s->capacity = count;
	s->client_list = malloc(sizeof(int) * count);
	pthread_t *threads = malloc(sizeof(pthread_t) * count);

	listen(s->sock,count);

	for(int i = 0;i < count;i++){
		pthread_create(&threads[i],NULL,recv_message,&s->client_list[i]);
	}

	pthread_exit(NULL);
}


