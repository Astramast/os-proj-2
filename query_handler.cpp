#include "query_handler.h"

bool SIG;
bool USR1;

void sigint_handler(int received){
	if (received == SIGINT){
		SIG=true;
	}
}

void sigusr1_handler(int received){
	if (received == SIGUSR1){
		USR1=true;
	}
}

int identify_query(char* query){
	char* temp;
	temp = query;
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

void execute_query(int query_number, data_storage* data, query_result_t* query){
	bool everything_fine=true;
	struct tm birthdate;
	if (query_number == 0){
	
		if (parse_insert(data->query_parsing, data->fname, data->lname, &data->id, data->section, &birthdate)){
			student_t student;
			student.id = data->id;
			strcpy(student.fname, data->fname);
			strcpy(student.lname, data->lname);
			strcpy(student.section, data->section);
			student.birthdate=birthdate;
			insert(&student, data->db, query);
		}
		else {everything_fine = false;}
	}

	else if (query_number == 1){
		if (parse_selectors(data->query_parsing, data->field, data->value)){
			select(data->field, data->value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number == 2){
		if (parse_selectors(data->query_parsing, data->field, data->value)){
			delete_function(data->field, data->value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number == 3){
		if (parse_update(data->query_parsing, data->field, data->value, data->field_to_update, data->update_value)){
			update(data->field, data->value, data->field_to_update, data->update_value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if(query_number == -1){strcpy(data->error_msg,"Error: Wrong query. Use insert, select, delete, update\n");}

	else{everything_fine=false;}
	if (!everything_fine){strcpy(data->error_msg,"Error: Wrong query argument to parse was given.\n");}
}
