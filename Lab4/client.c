#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

void sendFile(int fd, int server_fd, struct sockaddr_in *server_addr){
	char buff[1024];
	ssize_t br;
	while((br = read(fd, buff, sizeof(buff)))) {
		if(br == -1){
			perror("|v| Error reading file.\n");
			break;
		}
		printf("|^| Sending %zd bytes...\n", br);
		if (sendto(server_fd, buff, br, 0, (struct sockaddr*)server_addr, sizeof(*server_addr)) == -1) {
			perror("|v| Error sending file.\n");
			break;
		}
	}
	if(sendto(server_fd, "", 0, 0, (struct sockaddr*)server_addr, sizeof(*server_addr)) == -1){
		perror("|v| Error sending end signal.\n");
	}
	printf("|^| Finished sending file.\n");
}

int main(int argc, char * argv[]){
	if(argc != 4){
		printf("Correct Usage: <IP> <PORT> <SRC FILE>\n");
		return 1;
	}
	printf("|^| Client program started.\n");

	int fd = open(argv[3], O_RDONLY);
  	if (fd == -1) {
  		printf("|v| Error reading file %s\n", argv[3]);
  		return 1;
  	}
  	printf("|^| Opened %s for reading.\n", argv[3]);

	int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_fd < 0) {
		perror("|v| Socket error.\n");
		close(fd);
		return 1;
	} 
	printf("|^| Socket created.\n");

  	struct sockaddr_in server_addr;
  	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
  	server_addr.sin_port = htons(atoi(argv[2]));
  	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  	
  	sendFile(fd, server_fd, &server_addr);
  	close(fd);
  	close(server_fd);
  	printf("|^| Disconnecting from the server.\n");

  	return 0;
}