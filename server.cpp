#include <sys/types.h>
#include "iostream"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int server_creator(){
    // Permet que write() retourne 0 en cas de réception
    // du signal SIGPIPE.
    signal(SIGPIPE, SIG_IGN);
    int socket_server= socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(28772);   

    bind(socket_server, (struct sockaddr *)&address, sizeof(address));
    printf("Bind: %d\n", socket_server);
    listen(socket_server, 8); 
    printf("Listening\n");

    pthread_t client_id[4];
    char buffer[1024];
    int lu;

    for(int i=0; i<3;i++){
      pthread_create(&client_id[i],NULL,NULL,NULL);//besoin d une fonction pour test
      
      struct sockaddr_in client_adrr;
      size_t addrlen = sizeof(client_adrr);
      int client_socket = accept(socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)&addrlen);

      printf("Accepted connexion: %d\n", client_socket);

      while ((lu = read(client_socket, buffer, 1024)) > 0) {
        write(client_socket, buffer, lu);
      }
    }

    for(int i=0; i<3; i++){
      pthread_join(client_id[i],NULL);
    }
    
    close(socket_server);
    printf("Closing server...\n");
    return 0;
}

int main(){
  server_creator();
  return 1;
}
