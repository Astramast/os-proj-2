#include "server.h"

void execute_request(char user_query[1024], data_storage* data_thread){

  query_result_t query;
  query_result_init(&query, user_query);

  strcpy(data_thread->query_parsing, query.query);
  printf("Running query %s", data_thread->query_parsing);

  int query_number = identify_query(query);
  execute_query(query_number, data_thread, &query);
  query.query[strcspn(query.query, "\n")]=0;

  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  query.end_ns = now.tv_nsec + 1e9 * now.tv_sec;
  log_query(&query);

}

void* handle_connection(void* data){
  char user_query[1024];
  int lu;
  data_storage data_thread= *(data_storage*)data;
  free(data);
  int socket_client = data_thread.socket_data;

  while ((lu = read(socket_client, user_query, 1024)) > 0) {

    printf("Request readed from client number %i: %s", socket_client, user_query);
    execute_request(user_query, &data_thread);

    write(socket_client, user_query, lu);
  }

  close(socket_client);
  printf("Closing connection with client number: %i\n",socket_client);
  return NULL;
}

void client_receiver(int* socket_server, database_t* db){

  while(true){

    struct sockaddr_in client_adrr;
    size_t addrlen = sizeof(client_adrr);
    int client_socket = accept(*socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)&addrlen);

    printf("Accepted connection: %d\n", client_socket);  

    pthread_t client_thread;
    data_storage* client_data=(data_storage*)malloc(sizeof(client_data));

    client_data->socket_data=client_socket;
    client_data->db=db;
    pthread_create(&client_thread,NULL,handle_connection,client_data);
  }  

}

int server_handler(database_t* db){
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
      ::bind(socket_server, (struct sockaddr *)&address, sizeof(address));
    }

    printf("\nBind: %d\n", socket_server);
    listen(socket_server, 20); 
    printf("Listening...\n");
    
    client_receiver(&socket_server, db);
    close(socket_server);
    return 0;
}