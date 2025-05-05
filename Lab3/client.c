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
void fileRequest(int server_fd, char* src_fn, char* dst_fn){
        if ( send(server_fd, src_fn, sizeof(src_fn)+1, 0) == -1 ) {
                printf("|v| Unable to request %s for transfer.\n", src_fn);
                return;
        } else {
                printf("|^| Requested %s for transfer.\n", src_fn);
        }
        int dst_fd = open(dst_fn, O_WRONLY | O_CREAT | O_TRUNC);
        if(dst_fd < 0){
                printf("|v| Unable to open %s for writing.\n", dst_fn);
                return;
        } else {
                printf("|^| Opened %s for writing.\n", dst_fn);
        }
        char buff[1024];
        int bytes_recv;
        while((bytes_recv = recv(server_fd, buff, 1024, 0)) > 0) {
                if (write(dst_fd, buff, bytes_recv) == -1) {
                        printf("|v| Error writing to file %s\n", dst_fn);
                        break;
                }
        }
        if(bytes_recv < 0){
                printf("|v| Error receiving file %s\n", src_fn);
        } else {
		printf("|^| File %s successfully stored in %s.\n", src_fn, dst_fn);
	}
        close(dst_fd);
        return;
}

int main(int argc, char* argv[]){
        if(argc != 5){
                printf("|v| Correct Usage: <IP> <PORT> <SRC> <DST>\n");
                return 1;
        }
        int client_fd;
        struct sockaddr_in server_addr;
        if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printf("|v| Cannot create socket.\n");
                return 1;
        } else {
                printf("|^| Created client socket.\n");
        }
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(argv[1]);
        server_addr.sin_port = htons(atoi(argv[2]));
        if(connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0){
                printf("|v| Cannot connect to server.\n");
                return 1;
        } else {
                printf("|^| Connected to server.\n");
        }
        fileRequest(client_fd, argv[3], argv[4]);
        close(client_fd);
        return 0;
}

