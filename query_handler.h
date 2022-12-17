#ifndef _QUERY_HANDLER_H
#define _QUERY_HANDLER_H

#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include <signal.h>
#include <cstring>
#include "query.h"
#include "parsing.h"
#include "utils.h"
#include <sys/wait.h>
#include <sys/mman.h>

const int INSERT = 0;
const int SELECT = 1;
const int DELETE = 2;
const int UPDATE = 3;

struct data_storage{

	char fname[64]="";
	char lname[64]="";
	char section[64]="";
	char field[256]="";
	char value[256]="";
	char field_to_update[256]="";
	char update_value[256]="";
	struct tm birthdate;
	unsigned id;
	char query_parsing[256]="";
	char error_msg[128]="";
	char* server_answer = NULL;
	database_t* db;
	int socket_data;
};

bool is_valid_insert(student_t* student, data_storage* data);

bool is_valid_selectors(data_storage* data);

bool is_valid_update(data_storage* data);

int identify_query(char* query);
/**
 * @brief: identify wich query is received
 * 
 */
void execute_query(int query_number, data_storage* data, query_result_t* query);
/**
 * @brief: parse and execute the query
 * @param data: a pointer to the data storage where every parameter needed for the parsing and the execution is
 */

#endif
