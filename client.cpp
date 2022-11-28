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

int client_creator(){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_adrr;
    client_adrr.sin_family = AF_INET;
    client_adrr.sin_port = htons(28772);

    // Conversion de string vers IPv4 ou IPv6 en binaire
    inet_pton(AF_INET, "127.0.0.1", &client_adrr.sin_addr);
    connect(client_socket, (const struct sockaddr *)&client_adrr, sizeof(client_adrr));
    printf("Connected.\n");

    char buffer[1024];
    uint32_t lenght;

    while (fgets(buffer, 1024, stdin) != NULL) {
        lenght = strlen(buffer) + 1;
        printf("Sending...\n");
        
        // Comme on envoie sur le réseau une valeur
        // binaire encodée sur plusieurs octets, il
        // faut faire attention au boutisme.
        // Nous utilisons la technique standard pour
        // ça : l'envoi converti en big-endian (htonl())
        // et la réception converti du big-endian vers
        // le boutisme local (ntohl())
        lenght = htonl(lenght);
        write(client_socket, &lenght, 4);
        
        lenght = ntohl(lenght);
        write(client_socket, buffer, lenght);
        
        if (!precise_read(client_socket, buffer, lenght)) {
            return 1;
        }
        
        printf("Received : %s\n", buffer);
   }
  
  close(client_socket);
  printf("Closing server connexion...\n");
  return 0;
}