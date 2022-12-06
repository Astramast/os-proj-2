#include "server.h"
//faire struct server
void* handle_connection(void* data){
  char user_query[1024];
  int lu;
  data_storage* data_thread=(data_storage*) data;//nom a changer ptetre
  cout<<"data pointeur: "<<data_thread->p_client<<endl;
  cout<<"data sans pointeur: "<<data_thread->p_client<<endl;
  int client_socket= *data_thread->p_client;
  
  while ((lu = read(client_socket, user_query, 1024)) >= 0) {

		query_result_t query;
    query_result_init(&query, user_query);

    printf("Running query %s\n", query.query);
    strcpy(data_thread->query_parsing, query.query);

    int query_number=identify_query(query);
    execute_query(query_number,data_thread, &data_thread->db, &query);
    query.query[strcspn(query.query, "\n")]=0;

    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    query.end_ns = now.tv_nsec + 1e9 * now.tv_sec;
    log_query(&query);

    write(client_socket, user_query, lu);
  }

  close(client_socket);
  printf("Closing connection with client number: %i\n",client_socket);
  return NULL;
}

void client_receiver(int* socket_server,data_storage* data){

  while(true){
    struct sockaddr_in client_adrr;
    size_t addrlen = sizeof(socket_server);
    cout<<"vas ici"<<endl;
    int client_socket = accept(*socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)addrlen);
    printf("Accepted connection: %d\n", client_socket);
    fflush(stdout);
    //create the thread and store the client socket in data storage
    pthread_t client_thread;
    cout<<"thread crée"<<endl;
    int *p_client=(int*)malloc(sizeof(int));
    *p_client=client_socket;
    data->p_client=p_client;
    cout<<data->p_client<<endl;
    free(p_client);
    pthread_create(&client_thread,NULL,handle_connection,&data);
  }   

}

void server_handler(data_storage* data){

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
    listen(socket_server, 40); 
    printf("Listening...\n");
    
    client_receiver(&socket_server,data);
}