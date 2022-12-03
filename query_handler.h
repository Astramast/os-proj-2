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

struct data_storage{

	char fname[64];
	char lname[64];
	char section[64];
	char field[256];
	char value[256];
	char field_to_update[256];
	char update_value[256];
	unsigned id;
	char query_parsing[256];
	database_t db;
	int *p_client;

};

bool SIG=false;
bool USR1=false;

void sigint_handler(int received);

void sigusr1_handler(int received);

int identify_query(query_result_t query);

void execute_query(int query_number, data_storage* data, database_t* db, query_result_t* query);


#endif