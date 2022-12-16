#include "query_handler.h"

int identify_query(char* query){
	char* temp = (char*)malloc(strlen(query)+1);
	strcpy(temp, query);
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

bool is_valid_insert(student_t* student, data_storage* data){

return true;
}

bool is_valid_selectors(data_storage* data){

return true;
}

bool is_valid_update(data_storage* data){

return true;
}

void execute_query(int query_number, data_storage* data, query_result_t* query){
	bool everything_fine=true;
	struct tm birthdate;
	if (query_number == INSERT){
	
		if (parse_insert(data->query_parsing, data->fname, data->lname, &data->id, data->section, &birthdate)){
			student_t student;
			student.id = data->id;
			strcpy(student.fname, data->fname);
			strcpy(student.lname, data->lname);
			strcpy(student.section, data->section);
			student.birthdate=birthdate;

			printf("query parsing: %s, fname: %s, lname: %s, id: %i, section: %s, birthdate: %i/%i/%i\n",
			data->query_parsing, student.fname,student.lname,student.id,student.section,
			student.birthdate.tm_year+1900, student.birthdate.tm_mon+1, student.birthdate.tm_mday);

			insert(&student, data->db, query);
		}
		else {everything_fine = false;}
	}

	else if (query_number == SELECT){

		if (parse_selectors(data->query_parsing, data->field, data->value)){
			printf("query_parsing: %s, field: %s, value: %s\n",data->query_parsing,data->field, data->value);
			select(data->field, data->value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number == DELETE){

		if (parse_selectors(data->query_parsing, data->field, data->value)){
			delete_function(data->field, data->value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number == UPDATE){

		if (parse_update(data->query_parsing, data->field, data->value, data->field_to_update, data->update_value)){
			printf("field: %s, value: %s, field to update: %s, update value: %s\n", 
			data->field, data->value, data->field_to_update, data->update_value);
			update(data->field, data->value, data->field_to_update, data->update_value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if(query_number == -1){strcpy(data->error_msg,"Error: Wrong query. Use insert, select, delete, update\n");}

	else{everything_fine=false;}
	if (!everything_fine){strcpy(data->error_msg,"Error: Wrong query argument to parse was given.\n");}
}
