#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "utils.h"

int server_creator(){

    int socket_server= socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(28772);   

    bind(socket_server, (struct sockaddr *)&address, sizeof(address));
    printf("Bind: %d\n", socket_server);
    listen(socket_server, 5); 
    printf("Listening\n");

    char buffer[1024];
    uint32_t len = 0;

    struct sockaddr_in client_adrr;
    size_t addrlen = sizeof(client_adrr);
    int client_socket = accept(socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)&addrlen);
    printf("Accept\n");

    printf("Client: %d\n", client_socket);

  

    // Pour le serveur, on se contente de renvoyer
    // au client tout ce qui est reçu. Comme le
    // socket est SOCK_STREAM, plusieurs appels à
    // read() peuvent être nécessaires pour lire
    // le message en entier.
    while (precise_read(client_socket, (char*)&len, 4) /* lire la taille du message */
      && precise_read(client_socket, buffer, ntohl(len))) { /* lire le message */
      
      write(client_socket, buffer, ntohl(len));
  }

    close(socket_server);
    close(client_socket);
    printf("Closing server...\n");
    return 0;
}

