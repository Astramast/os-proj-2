#include "server.h"

void* handle_connection(void* data){
  char user_query[1024];
  int lu;
  data_storage data_thread= *(data_storage*)data;
  free(data);
  int socket_client = data_thread.socket_data;

  while ((lu = read(socket_client, user_query, 1024)) > 0) {
    printf("Request readed: %s", user_query);
    write(socket_client, user_query, lu);
  }

  close(socket_client);
  printf("Closing connection with client number: %i\n",socket_client);
  return NULL;
}

void client_receiver(int* socket_server,data_storage* data){

  while(true){

    struct sockaddr_in client_adrr;
    size_t addrlen = sizeof(client_adrr);
    int client_socket = accept(*socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)&addrlen);

    printf("Accepted connection: %d\n", client_socket);  

    pthread_t client_thread;
    data_storage* client_data=(data_storage*)malloc(sizeof(client_data));
    
    client_data->socket_data=client_socket;
    pthread_create(&client_thread,NULL,handle_connection,client_data);
  }  

}

int server_handler(data_storage* data){
    int socket_server= socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(28772);   

    int check_bind = ::bind(socket_server, (struct sockaddr *)&address, sizeof(address));

    if(check_bind == -1){
      printf("Failed bind. Trying to bind again\n");
      bind(socket_server, (struct sockaddr *)&address, sizeof(address));
    }

    printf("Bind: %d\n", socket_server);
    listen(socket_server, 20); 
    printf("Listening...\n");
    
    client_receiver(&socket_server,data);
    close(socket_server);
    return 0;
}

int main(){
  data_storage data;
  server_handler(&data);
  return 0;
}