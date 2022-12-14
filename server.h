#ifndef _SERVER_H
#define _SERVER_H
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
#include "query_handler.h"

void execute_request(char user_query[1024], int query_number, data_storage* data_thread);

void* handle_connection(void* data);
/**
 * @brief: receive a query, initiate it, identify it, parse it and execute it. 
 * Then create a log file with the result and send the result to the client
 * @param data: data storage structure sended into the thread
 */

void client_receiver(int* socket_server,database_t* db);
/**
 * @brief: create a new thread for each new client
 */
int server_handler(database_t* db);
/**
 * @brief: create the server socket
 */

#endif
