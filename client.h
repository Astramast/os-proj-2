#ifndef _CLIENT_H
#define _CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


int request_reader(int client_socket);
/**
 * @brief: Wait a query on stdin and send it to the server and read the response
 */

void client_handler();
/**
 * @brief Create the socket server
 * 
 */
#endif