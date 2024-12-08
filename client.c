#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>
#define SOCKET_PATH "/tmp/mysocket"

int main(){
    int client_fd, valread, status;
    struct sockaddr_un serv_addr;
    char hello[] = "Hello from client\n";
    char buffer[1024];

    if((client_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_LOCAL;
    strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);
    
    if((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_un)) < 0)){
        perror("Connection fail");
        exit(1);
    }

    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent.\n");

    valread = read(client_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);

    close(client_fd);
    return 0;
}