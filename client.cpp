#include "client.h"

int request_reader(int client_socket){
    char buffer[256];
    int lenght, i, ret;

    while (fgets(buffer, 256, stdin) != NULL) {
        lenght = strlen(buffer) + 1;
        write(client_socket, buffer, strlen(buffer) + 1);
        printf("Sending...\n");

        i = 0;
        while (i < lenght) {
            ret = read(client_socket, buffer, lenght - i);
            if (ret <= 0) {
                if (ret < 0)
                    perror("read");
                else
                    printf("Closing server connection...\n");
                    return 1;
            }

            i += ret;
        }

        printf("Received: %s\n", buffer);
    }
    close(client_socket);
    pthread_exit(0);
    return 0;
}

int client_handler(){
    
    signal(SIGPIPE, SIG_IGN);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_adrr;
    client_adrr.sin_family = AF_INET;
    client_adrr.sin_port = htons(28772);

    // Conversion de string vers IPv4 ou IPv6 en binaire
    inet_pton(AF_INET, "127.0.0.1", &client_adrr.sin_addr);
    connect(client_socket, (const struct sockaddr *)&client_adrr, sizeof(client_adrr));
    printf("Connected.\n");
    request_reader(client_socket);
    return 0;
}