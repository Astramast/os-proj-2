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

	bool is_valid = true;
	size_t index = 0;

	while (is_valid == true and index < strlen(student->fname)){
		if(!isdigit(student->fname[index])){
			is_valid = false;
		}
		index++;
	}

	index=0;
	while (is_valid == true and index < strlen(student->lname)){
		if(!isdigit(student->lname[index])){
			is_valid = false;
		}
		index++;
	}

	index=0;
	while (is_valid == true and index < strlen(student->section)){
		if(!isdigit(student->section[index])){
			is_valid = false;
		}
		index++;
	}

	if(is_valid == false){strcpy(data->error_msg, "Error: wrong argument to insert(numbers are not allowed outside of id and birthdate)\n");}
	return is_valid;
}

bool is_valid_selectors(data_storage* data){
	bool is_valid = true;
	size_t index = 0;

	int strcmp_id=strcmp(data->field, "id");
	int strcmp_fname=strcmp(data->field, "fname");
	int strcmp_lname=strcmp(data->field, "lname");
	int strcmp_section=strcmp(data->field, "section");

	if(strcmp_id == 0){

		while(is_valid == true and index < strlen(data->value)){
			if(isalpha(data->value[index]) == true){
				is_valid = false;
				strcpy(data->error_msg, "Error only numbers are allowed for id\n");
			}
			index++;
		}
	}

	else if(strcmp_fname == 0 or strcmp_lname == 0 or strcmp_section == 0){

		while(is_valid == true and index < strlen(data->value)){
			if(isdigit(data->value[index] == true)){
				is_valid = false;
				strcpy(data->error_msg, "Error: numbers are not allowed for fname, lname and section\n");
			}
			index++;
		}
	}

	else if(strcmp(data->field, "birthdate") == 0){

		char student_bd_temp[256];
		strcpy(student_bd_temp, data->value);

		if (strptime(student_bd_temp, "%d/%m/%Y", &data->birthdate) == NULL) {//transforme un string en date
			is_valid = false;
			strcpy(data->error_msg, "Error: birthdate is not complete\n");
    	}
	}

	else{
		strcpy(data->error_msg, "Error: The filter that you want doesn't exist.\n");
		is_valid = false;
	}

	return is_valid;
}

bool is_valid_update(data_storage* data){
	bool is_valid = true;
	if(is_valid_selectors(data)){

		size_t index=0;
		int strcmp_id=strcmp(data->field_to_update, "id");
		int strcmp_fname=strcmp(data->field_to_update, "fname");
		int strcmp_lname=strcmp(data->field_to_update, "lname");
		int strcmp_section=strcmp(data->field_to_update, "section");

		if(strcmp_id == 0){

			while(is_valid == true and index < strlen(data->update_value)){
				if(isalpha(data->update_value[index]) == true){
					is_valid = false;
					strcpy(data->error_msg, "Error only numbers are allowed for id\n");
				}
				index++;
			}
		}

		else if(strcmp_fname == 0 or strcmp_lname == 0 or strcmp_section == 0){

			while(is_valid == true and index < strlen(data->update_value)){
				if(isdigit(data->update_value[index] == true)){
					is_valid = false;
					strcpy(data->error_msg, "Error: numbers are not allowed for fname, lname and section\n");
				}
				index++;
			}
		}

		else if(strcmp(data->field, "birthdate") == 0){

			char student_bd_temp[256];
			strcpy(student_bd_temp, data->update_value);

			if (strptime(student_bd_temp, "%d/%m/%Y", &data->birthdate) == NULL) {//transforme un string en date
				is_valid = false;
				strcpy(data->error_msg, "Error: birthdate is not complete\n");
			}
		}

		else{
			strcpy(data->error_msg,"Error: The data that you want to change doesn't exist.\n");
			is_valid = false; 
		}
	}

	return is_valid;	
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
			if(is_valid_insert(&student, data))
				insert(&student, data->db, query);
		}
		else {everything_fine = false;}
	}

	else if (query_number == SELECT){
		if (parse_selectors(data->query_parsing, data->field, data->value)){
			printf("query_parsing: %s, field: %s, value: %s\n",data->query_parsing,data->field, data->value);
			if(is_valid_selectors(data))
				select(data->field, data->value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number == DELETE){

		if (parse_selectors(data->query_parsing, data->field, data->value)){
			if(is_valid_selectors(data))
				delete_function(data->field, data->value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if (query_number == UPDATE){

		if (parse_update(data->query_parsing, data->field, data->value, data->field_to_update, data->update_value)){
			printf("field: %s, value: %s, field to update: %s, update value: %s\n", 
			data->field, data->value, data->field_to_update, data->update_value);
			if(is_valid_update(data))
				update(data->field, data->value, data->field_to_update, data->update_value, data->db, query);
		}
		else{everything_fine = false;}
	}

	else if(query_number == -1){strcpy(data->error_msg,"Error: Wrong query. Use insert, select, delete, update\n");}

	else{everything_fine=false;}
	if (!everything_fine){strcpy(data->error_msg,"Error: Wrong query argument to parse was given.\n");}
}
