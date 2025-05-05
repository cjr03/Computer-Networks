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

/*
Receive client response for src filename
Open src file
Send src file to client socket

Send src filename to server socket
Open dst file
Write server response to dst file
*/
void fileTransfer(int client_fd){	
	char fn_buff[1024];
	int bytes_recv;
	bytes_recv = recv(client_fd, fn_buff, 1024, 0);
	if (bytes_recv <= 0){
		perror("|v| Unable to receive filename requested.\n");
		return;
	} else {
		printf("|^| Opening requested file.\n");
	}
	fn_buff[bytes_recv] = '\0';
	int src_fd = open(fn_buff, O_RDONLY);
	if(src_fd < 0) {
		printf("|v| Unable to open %s for reading.\n", fn_buff);
		return;
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
}

int main(int argc, char* argv[]){
	if(argc != 2) {
		perror("|v| Correct usage: <PORT>.\n");
		return 1;
	}
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size = sizeof(client_addr);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("|v| Cannot create socket.\n");
		return 1;
	} else {
		printf("|^| Created server socket.\n");
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi((argv[1])));
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0){
		perror("|v| Cannot bind to socket.\n");
		close(server_fd);
		return 1;
	} else {
		printf("|^| Binded to socket.\n");
	}
	if((listen(server_fd, 1)) < 0){
		perror("|v| Cannot listen to socket.\n");
		close(server_fd);
		return 1;
	} else {
		printf("|^| Listening to socket.\n");
	}
	if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size)) < 0){
		perror("|v| Cannot accept client to socket.\n");
		return 1;
	} else {
		printf("|^| Client accepted to socket.\n");
	}
	fileTransfer(client_fd);
	close(client_fd);
	close(server_fd);
}
