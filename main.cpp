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

/*
Authors : 
Installé Arthur Matricule 495303
Kevin Issa Matricule 514550
*/

bool END=false;
bool USR1=false;

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

};

void sigint_handler(int received){
	if (received == SIGINT){
		END=true;
	}
}

void sigusr1_handler(int received){
	if (received == SIGUSR1){
		USR1=true;
	}
}

int identify_query(query_result_t query){
	char* temp;
	temp = query.query;
	char* query_first_word = strtok_r(NULL, " ", &temp);
	if (strcmp(query_first_word, "insert")==0){
		return 0;
	}
	if (strcmp(query_first_word, "select")==0){
		return 1;
	}
	if (strcmp(query_first_word, "delete")==0){
		return 2;
	}
	if (strcmp(query_first_word, "update")==0){
		return 3;
	}
	return -1;
}

void execute_query(int query_number, data_storage* data, database_t* db, query_result_t* query){
	bool everything_fine=true;
	struct tm birthdate;
	if (query_number==0){
	
		if (parse_insert(data->query_parsing, data->fname, data->lname, &data->id, data->section, &birthdate)){
			student_t student;
			student.id = data->id;
			strcpy(student.fname, data->fname);
			strcpy(student.lname, data->lname);
			strcpy(student.section, data->section);
			student.birthdate=birthdate;
			insert(&student, db, query);
		}
		else {everything_fine = false;}
	}

	else if (query_number==1){
		if (parse_selectors(data->query_parsing, data->field, data->value)){
			select(data->field, data->value, db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number==2){
		if (parse_selectors(data->query_parsing, data->field, data->value)){
			delete_function(data->field, data->value, db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number==3){
		if (parse_update(data->query_parsing, data->field, data->value, data->field_to_update, data->update_value)){
			update(data->field, data->value, data->field_to_update, data->update_value, db, query);
		}
		else{everything_fine = false;}
	}

	else{everything_fine=false;}
	if (!everything_fine){printf("Wrong query argument given. Failed.\n");}
}

int main(int argc, char const *argv[]) {
	printf("STARTING\n");
	if (strcmp(argv[1], "")==0){
		printf("No file given. Usage : ./tinydb <db_file_path>");
		exit(-1);
	}
	const char *db_path = argv[1];
	printf("Entered path : %s\n", db_path);
	database_t db;
    db_init(&db);
    db_load(&db, db_path);

	int pid;
	for (int i=0; i<4; i++){
		pid = fork();
		if (pid < 0){
			perror("Error during fork(): ");
			exit(1);
		}
	}
	signal(SIGINT, sigint_handler);
	if (pid!=0){//father
		signal(SIGUSR1, sigusr1_handler);
		char user_query[256];
		while (fgets(user_query, 256, stdin)){
			if (END){
				break;
			}

			if (USR1){
				db_save(&db, db_path);
			}

			query_result_t query;
			query_result_init(&query, user_query);
			int query_number = identify_query(query);

			if (query_number == -1){
				printf("E: Wrong query. Use insert, select, delete, update\n");
			}

				
		}
	//End of program procedure
	/*
	for (int i=0; i<4; i++){
		kill(sons[i], SIGINT);
		int temp=EOF;
		write(pipes[2*i][1], &temp, sizeof(int));
		close(pipes[2*i][1]);
		close(pipes[2*i+1][0]);
		int state;
		wait(&state);
	}
	*/
	db_save(&db, db_path);
	printf("Bye bye!\n");
	}

	else{//son
		while (!END){
			data_storage data;
			query_result_t query;
			if (!END){
				printf("Running query %s\n", query.query);
				strcpy(data.query_parsing, query.query);

				int query_number=identify_query(query);//a garder
				execute_query(query_number,&data, &db, &query);

				query.query[strcspn(query.query, "\n")]=0;
				struct timespec now;
				clock_gettime(CLOCK_REALTIME, &now);
				query.end_ns = now.tv_nsec + 1e9 * now.tv_sec;
				log_query(&query);
			}
		}
	}
    return 0;
}
