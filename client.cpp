#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int request_reader(int client_socket){
    char buffer[256];
	char* answer = NULL;
    int ret;
	size_t i=0;
	size_t length=0;

    while (fgets(buffer, 256, stdin) != NULL) {//the socket is waiting that the user send something on stdin
        length = strlen(buffer) + 1;
        write(client_socket, buffer, length);//send the query to the server
        printf("Sending...\n");

		read(client_socket, &length, sizeof(size_t));//read the size of the server's response
		answer = (char*)malloc(length);//allocate the char*
        i = 0;
        while (i < length) {
            ret = read(client_socket, answer, length - i);//read the server's response
            if (ret <= 0) {
                if (ret < 0){
                    perror("read");
				}
				else{
                    printf("Closing server connection...\n");
                    return 1;
				}
            }

            i += ret;
        }

        printf("Received: %s", answer);
		free(answer);
		answer = NULL;
		printf("%s","\nEnter your query: ");
    }
    close(client_socket);
    pthread_exit(0);
    return 0;
}

void client_handler(char* ip){
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_adrr;
    client_adrr.sin_family = AF_INET;
    client_adrr.sin_port = htons(28772);

    // Conversion de string vers IPv4 ou IPv6 en binaire
    inet_pton(AF_INET, ip, &client_adrr.sin_addr);
    connect(client_socket, (const struct sockaddr *)&client_adrr, sizeof(client_adrr));
	printf("%s", "Connected.\nEnter your query: ");
    request_reader(client_socket);
}

int main(int argc, char* argv[]){
    if (argc == 1){
		printf("%s\n", "No IP given. Usage : ./client [ip]");
		exit(-1);
	}

    client_handler(argv[1]);
    return 0;
}
