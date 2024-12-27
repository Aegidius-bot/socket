#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<math.h>
#define SOCKET_PATH "/tmp/mysocket"

int main(){
	int server_fd, new_socket;
	struct sockaddr_un address;
	ssize_t valread;
	int opt = 1;
	char buffer[1024];
	char hello[] = "Hello\n";
	char* tok;
	char* arg1;
	char* arg2;
	char* ans = (char*)malloc(sizeof(char) * 50);

	if((server_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
		perror("Socket fail");
		exit(1);
	}

	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(1);
	}
	
	
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_LOCAL;
	strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
		perror("bind");
		exit(1);
	}

	if(listen(server_fd, 3) < 0){
		perror("listen");
		close(server_fd);
		exit(1);
	}

	if((new_socket = accept(server_fd, NULL, NULL)) < 0){
		perror("Accept");
		exit(1);
	}
	
	while(1){
		valread = read(new_socket, buffer, 1024 - 1);
		
		//Parse the command from the client
		
		tok = strtok(buffer, " ");
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL," ");
		
		printf("\n**** %s ****\n", tok);

		if(strcmp(tok,"kill") == 0){
			sprintf(ans, "%s", "\0");
			send(new_socket, ans, strlen(ans), 0);
			break;
		}else if(strcmp(tok,"add") == 0){
			sprintf(ans, "%d\n", atoi(arg1) + atoi(arg2));
			send(new_socket, ans, strlen(ans), 0);
			memset(ans, 0, 50);
		}else if(strcmp(tok,"abs") == 0){

			sprintf(ans, "%d\n", abs(atoi(arg1)));
			send(new_socket, ans, strlen(ans), 0);
			memset(ans, 0, 50);
		}else if(strcmp(tok, "mul") == 0){

			sprintf(ans, "%d\n", atoi(arg1) * atoi(arg2));
			send(new_socket, ans, strlen(ans), 0);
			memset(ans, 0, 50);
		}else{
			send(new_socket, hello, strlen(hello), 0);
		}
	}
	
	free(ans);
	close(new_socket);
	close(server_fd);
	unlink(SOCKET_PATH);
	return 0;
}
