#include "server.h"
//WARNING: PASSER PAR LE HEADER AU LIEU D INCLURE DIRECTEMENT LES LIBRAIRIES PEUT PROVOQUER DES PROBLEMES
//JE SAIS PAS POURQUOI FUCK PROGRA
void* handle_connection(void* data){
  char user_query[1024];
  int lu;

  data_storage* data_thread= (data_storage*)data;
  
  std::cout<<"pointeur data: "<<data_thread->p_client<<std::endl;
  int client_socket= *data_thread->p_client;//seg fault vient d ici
  std::cout<<"pas pointeur: "<<client_socket<<std::endl;
  //free(data_thread->p_client);

  while ((lu = read(client_socket, user_query, 1024)) > 0) {
    printf("Request readed: %s", user_query);
    write(client_socket, user_query, lu);
  }

  close(client_socket);
  printf("Closing connection with client number: %i\n",client_socket);
  return NULL;
}

void client_receiver(int* socket_server,data_storage* data){

  while(true){
    struct sockaddr_in client_adrr;
    size_t addrlen = sizeof(client_adrr);
    int client_socket = accept(*socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)&addrlen);

    printf("Accepted connection: %d\n", client_socket);  

    pthread_t client_thread;

    int *p_client=(int*)malloc(sizeof(int));
    *p_client=client_socket;
    data->p_client=p_client;
    std::cout<<"data client: "<<*data->p_client<<std::endl;
    pthread_create(&client_thread,NULL,handle_connection,&data);
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

    int check_bind = bind(socket_server, (struct sockaddr *)&address, sizeof(address));
    //ne bind pas une fois sur 2 ca met bien
    if(check_bind == -1){
      printf("Failed bind. Trying to bind again\n");
      bind(socket_server, (struct sockaddr *)&address, sizeof(address));
      printf("%d",errno);
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