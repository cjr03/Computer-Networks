/*
* Name: Cole Roberts
* Date: 15 April 2025
* Title: Lab3 - Part 1
* Description: This program is a TCP server that accepts a client
* connection for one file transfer.
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

typedef struct {
	int client_fd;
	struct sockaddr_in client_addr;
} client_data;

/*
Receive client response for src filename
Open src file
Send src file to client socket

Send src filename to server socket
Open dst file
Write server response to dst file
*/
void* handleClient(void* arg){	
	client_data* data = (client_data*) arg;
	int client_fd = data->client_fd;
	struct sockaddr_in client_addr = data->client_addr;
	free(arg);
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected from IP: %s and PORT: %d\n", client_ip, ntohs(client_addr.sin_port));
	char fn_buff[1024];
	int bytes_recv;
	bytes_recv = recv(client_fd, fn_buff, 1024, 0);
	if (bytes_recv <= 0){
		perror("|v| Unable to receive filename requested.\n");
		close(client_fd);
		return NULL;
	} else {
		printf("|^| Opening requested file.\n");
	}
	fn_buff[bytes_recv] = '\0';
	int src_fd = open(fn_buff, O_RDONLY);
	if(src_fd < 0) {
		printf("|v| Unable to open %s for reading.\n", fn_buff);
		close(client_fd);
		return NULL;
	} else {
		printf("|^| Opened %s for reading.\n", fn_buff);
	}
	char src_buff[1024];
	ssize_t bytes_read;
	while((bytes_read = read(src_fd, src_buff, sizeof(src_buff))) > 0){
		if( send(client_fd, src_buff, bytes_read, 0) == -1 ){
			perror("|v| Error sending file.\n");
			break;
		} 
	}
	if(bytes_read < 0){
		printf("|v| Error reading file.\n");
	} else {
		printf("|^| File sent successfully.\n");
	}
	close(src_fd);
	close(client_fd);
	printf("Client IP: %s and PORT: %d disconnected.\n", client_ip, ntohs(client_addr.sin_port));
	return NULL;
}

int main(int argc, char* argv[]){
	if(argc != 2) {
		perror("|v| Correct usage: <PORT>.\n");
		return 1;
	}
	int server_fd;
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("|v| Cannot create socket.\n");
		return 1;
	} else {
		printf("|^| Created server socket.\n");
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi((argv[1])));
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("|v| Cannot bind to socket.\n");
		close(server_fd);
		return 1;
	} else {
		printf("|^| Binded to socket.\n");
	}
	if((listen(server_fd, 5)) < 0){
		perror("|v| Cannot listen to socket.\n");
		close(server_fd);
		return 1;
	} else {
		printf("|^| Listening to socket.\n");
	}
	while(1){
		struct sockaddr_in client_addr;
		socklen_t addr_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
		if (client_fd < 0){
			perror("|v| Accept failed.");
			continue;
		}
		pthread_t t_id;
		client_data* data = malloc(sizeof(client_data));
		data->client_fd = client_fd;
		data->client_addr = client_addr;
		if(pthread_create(&t_id, NULL, handleClient, data) != 0){
			perror("|v| Thread creation failed");
			close(client_fd);
			free(data);
			continue;
		}
		pthread_detach(t_id);
	}
	close(server_fd);
}
