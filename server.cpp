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

void* handle_connection(void* p_client_socket){
  char buffer[1024];
  int lu;
  int client_socket=*(int*)p_client_socket;
  free(p_client_socket);
  
  while ((lu = read(client_socket, buffer, 1024)) > 0) {
    printf("Request readed: %s", buffer);
    write(client_socket, buffer, lu);
  }

  close(client_socket);
  printf("Closing connection...");
  return NULL;
}

void client_receiver(int socket_server){

  while(true){
    
    struct sockaddr_in client_adrr;
    size_t addrlen = sizeof(client_adrr);
    int client_socket = accept(socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)&addrlen);

    printf("Accepted connection: %d\n", client_socket);  

    pthread_t client_thread;
    int *p_client=(int*)malloc(sizeof(int));
    *p_client=client_socket;
    pthread_create(&client_thread,NULL,handle_connection,p_client);
  }   

}

int server_handler(){
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
    listen(socket_server, 3); 
    printf("Listening...\n");
    
    client_receiver(socket_server);
    return 0;
}

int main(){
  server_handler();
  return 0;
}
