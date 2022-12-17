#include "server.h"

int MAX_USERS = 30;
int SERVER_PORT = 28772;

bool SIGINT_FLAG = false;
bool SAVE_FLAG = false;

void sigint_handler(int signal_num){
	if (signal_num == SIGINT){
		SIGINT_FLAG = true;
	}
}

void sigusr1_handler(int signal_num){
	if (signal_num == SIGUSR1){
		SAVE_FLAG = true;
	}
}

//initiate all the variables for the mutex
pthread_mutex_t new_access = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_access = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reader_registration = PTHREAD_MUTEX_INITIALIZER;
int readers = 0;

void server_output(query_result_t *query, data_storage* data_thread, int query_number){
	if (strlen(data_thread->error_msg) != 0){
    
		data_thread->server_answer = (char*)malloc(sizeof(data_thread->error_msg));
		strcpy(data_thread->server_answer, data_thread->error_msg);
		data_thread->error_msg[0] = '\0';
	}
  
	else if (query_number == SELECT){
    data_thread->server_answer = (char*)malloc(sizeof(char)*((query->lsize)*(sizeof(student_t)+100)+32));

		for (size_t i=0; i<query->lsize; i++){
      //create a big string by concatenating all the student found by select that will be send to the client
			char student_str_temp[sizeof(student_t)+100];
			student_to_str(student_str_temp, &query->students[i]);
			strcat(data_thread->server_answer, student_str_temp);
		}

		char amount_str[32];
		snprintf(amount_str, 32,"%li student(s) selected\n", query->lsize);
		strcat(data_thread->server_answer, amount_str);
	}

	else if (query_number == INSERT){
		data_thread->server_answer = (char*)malloc(sizeof(char)*(sizeof(student_t)+100));
    //send the student inserted to the client
		char student_str_temp[sizeof(student_t)+100];
		student_to_str(student_str_temp, &query->students[0]);
		strcat(data_thread->server_answer, student_str_temp);
	}

	else if (query_number == DELETE){
		data_thread->server_answer = (char*)malloc(sizeof(char)*32);

		char amount_str[32];
		snprintf(amount_str, 32,"%li student(s) deleted", query->lsize);
		strcat(data_thread->server_answer, amount_str);
	}

	else if (query_number == UPDATE){
		data_thread->server_answer = (char*)malloc(sizeof(char)*32);

		char amount_str[32];
		snprintf(amount_str, 32,"%li student(s) updated", query->lsize);
		strcat(data_thread->server_answer, amount_str);
	}
}


void execute_request(char user_query[1024],int query_number, data_storage* data_thread){

  query_result_t query;
  query_result_init(&query, user_query);

  strcpy(data_thread->query_parsing, query.query);
  printf("Running query %s\n", data_thread->query_parsing);

  execute_query(query_number, data_thread, &query);
  query.query[strcspn(query.query, "\n")]=0;
  
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  query.end_ns = now.tv_nsec + 1e9 * now.tv_sec;
  
  server_output(&query, data_thread, query_number);
  log_query(&query);
}

void* handle_connection(void* data){
  char user_query[256];
  int lu = 0;
  data_storage data_thread= *(data_storage*)data;
  free(data);
  int socket_client = data_thread.socket_data;

  while ((lu = read(socket_client, user_query, 256)) > 0) {

	printf("Request readed from client number %i: %s", socket_client, user_query);
    int query_number = identify_query(user_query);

    if(query_number != 1){

	    pthread_mutex_lock(&new_access);
      pthread_mutex_lock(&write_access);
      pthread_mutex_unlock(&new_access);

      execute_request(user_query, query_number, &data_thread);

      pthread_mutex_unlock(&write_access);
    }

    else{
      
      pthread_mutex_lock(&new_access);
      pthread_mutex_lock(&reader_registration);

      if(readers == 0){
        pthread_mutex_lock(&write_access);
      }
      readers++;

      pthread_mutex_unlock(&new_access);
      pthread_mutex_unlock(&reader_registration);

      execute_request(user_query, query_number, &data_thread);

      pthread_mutex_lock(&reader_registration);
      readers--;

      if (readers == 0){
        pthread_mutex_unlock(&write_access);
      }

      pthread_mutex_unlock(&reader_registration);
    }
    
    size_t serv_answer_len = strlen(data_thread.server_answer)+1; // +1 for \0 char
    write(socket_client, &serv_answer_len, sizeof(size_t));
    write(socket_client, data_thread.server_answer, serv_answer_len);

    if (data_thread.server_answer){
      free(data_thread.server_answer);
      data_thread.server_answer = NULL;
	  }
  }

  close(socket_client);
  printf("Closing connection with client number: %i\n",socket_client);
  return NULL;
}

void client_receiver(int* socket_server, database_t* db, const char* save_path){

	int client_socket = 0;
	struct sigaction sigint_a;
	sigint_a.sa_handler = sigint_handler;
	sigemptyset(&sigint_a.sa_mask);
	sigint_a.sa_flags = 0;
	sigaction(SIGINT, &sigint_a, NULL);
	sigint_a.sa_handler = sigusr1_handler;
	sigaction(SIGUSR1, &sigint_a, NULL);

  while(SIGINT_FLAG == false){

    struct sockaddr_in client_adrr;
    size_t addrlen = sizeof(client_adrr);
    client_socket = accept(*socket_server, (struct sockaddr *)&client_adrr, (socklen_t *)&addrlen);
	
    if (client_socket > 0){
      printf("Accepted connection: %d\n", client_socket);  

      sigset_t mask;
      sigemptyset(&mask);
      sigaddset(&mask, SIGINT);
      sigprocmask(SIG_BLOCK, &mask, NULL);

      pthread_t client_thread;
      data_storage* client_data=(data_storage*)malloc(sizeof(data_storage));

      client_data->socket_data=client_socket;
      client_data->db=db;
      
	  pthread_create(&client_thread,NULL,handle_connection,client_data);
      sigprocmask(SIG_UNBLOCK, &mask, NULL);
    }

    if (SAVE_FLAG == true){
		  printf("%s\n", "SIGUSR1 signal received, saving database..."); //printf should not be called in signal handlers, see man 7 signal-safety 

    	db_save(db, save_path);
		  SAVE_FLAG = false;
    }

  } 
	printf("%s\n", "SIGINT signal received, closing server..."); //printf should not be called in signal handlers, see man 7 signal-safety
}

int server_handler(database_t* db, const char* save_path){
    int socket_server= socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT); 

    int opt = 1;
    setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    int check_bind = ::bind(socket_server, (struct sockaddr *)&address, sizeof(address));

    if(check_bind == -1){
      printf("Failed bind. Trying to bind again\n");
      ::bind(socket_server, (struct sockaddr *)&address, sizeof(address));
    }

    printf("\nBind: %d\n", socket_server);
    listen(socket_server, MAX_USERS); 
    printf("Listening...\n");
    
    client_receiver(&socket_server, db, save_path);
    close(socket_server);
    pthread_mutex_destroy(&new_access);
    pthread_mutex_destroy(&write_access);
    pthread_mutex_destroy(&reader_registration);
    return 0;
}
