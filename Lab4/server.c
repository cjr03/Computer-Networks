#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

void write_file(int fd, int server_fd, struct sockaddr_in *client_addr){
  char buff[1024];
  socklen_t size = sizeof(*client_addr);
  ssize_t br;
  while((br = recvfrom(server_fd, buff, sizeof(buff), 0, (struct sockaddr*)client_addr, &size)) > 0){
    if(br == 0){
      printf("|^| File transfer complete.\n");
      break;
    }
    printf("|^| Received %zd bytes.\n", br);
    if (write(fd, buff, br) == -1){
      printf("|v| Error writing file.\n");
      break;
    }
  }
  if (br == -1){
    perror("|v| Error receiving data\n");
  }
}

int main(int argc, char* argv[]){
  if(argc != 4){
    perror("Correct Usage: <IP> <PORT> <DST FILE>\n");
    return 1;
  }
  printf("|^| Server program starting.\n");

  int fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0664);
  if(fd == -1){
    printf("|v| Error on file open.\n");
    return 1;
  }
  printf("|^| File opened for data writing: %s.\n", argv[3]);

  int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_fd < 0){
    perror("|v| Socket error.\n");
    return 1;
  }
  printf("|^| Socket connected.\n");

  struct sockaddr_in server_addr, client_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
    perror("|v| Error on bind.\n");
    close(server_fd);
    close(fd);
    return 1;
  }
  printf("|^| Server started.\n");
  
  write_file(fd, server_fd, &client_addr);
  close(fd);
  close(server_fd);
  printf("|^| File transfer complete, server closed.\n");

  return 0;
}