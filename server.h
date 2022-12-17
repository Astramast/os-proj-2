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

void sigint_handler(int signal_num);
/**
 * @brief: detect if a sigint signal is received
 * 
 * @param signal_num: sigint signal
 */

void sigusr1_handler(int signal_num);
/**
 * @brief: detect if a sigusr1 signal is received
 * @param signal_num: sigusr1 signal 
 */

void server_output(query_result_t *query, data_storage* data_thread, int query_number);
/**
 * @brief: prepare the result of the query for the client by adding each student asked by the client in a string
 * and writing it in data_thread->server_answer
 * 
 * @param query_number: type of query(select, delete , insert, update) 
 */

void execute_request(char user_query[1024], int query_number, data_storage* data_thread);
/**
 * @brief: Execute the query and create a log file with the result of the query.
 * @param user_query: the query to execute
 */

void* handle_connection(void* data);
/**
 * @brief: receive a query, identify it, take care of not having a concurrency between the threads(mutex)
 * and send the server's answer to the client socket

 * @param data: data storage structure sended into the thread
 */

void client_receiver(int* socket_server, database_t* db, const char* save_path);
/**
 * @brief: create a new thread for each new client and manage the signals
 */
int server_handler(database_t* db, const char* save_path);
/**
 * @brief: create the server socket
 */

#endif
