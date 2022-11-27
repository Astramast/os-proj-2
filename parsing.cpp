#include "parsing.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "student.h"

bool parse_update(char* query, char* field_filter, char* value_filter, char* field_to_update, char* update_value) {
    
    char* token = strtok_r(NULL, " ", &query);  // key=val filter
    if (token == NULL) {
        return false;
    }

	token = strtok_r(NULL, "=", &query);
	if (token == NULL) {
		return false;
	}
	strcpy(field_filter, token);

	token = strtok_r(NULL, " ", &query);
	if (token == NULL) {
		return false;
	}
	strcpy(value_filter, token);

	token = strtok_r(NULL, " ", &query);
	if (token == NULL) {
		return false;
	}
	
	token = strtok_r(NULL, "=", &query);
	if (token == NULL) {
		return false;
	}
	strcpy(field_to_update, token);

	token = strtok_r(NULL, "\n", &query);
	if (token == NULL) {
		return false;
	}
	strcpy(update_value, token);

    return true;
}

bool parse_insert(char* query, char* fname, char* lname, unsigned* id, char* section, struct tm* birthdate) {
    char* token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
	
	token = strtok_r(NULL, " ", &query);
	if (token == NULL) {
		return false;
	}
    strcpy(fname, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(lname, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    *id = (unsigned)atol(token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(section, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    if (strptime(token, "%d/%m/%Y", birthdate) == NULL) {//transforme un string en date
        return false;
    }
    return true;
}

bool parse_selectors(char* query, char* field, char* value) {
    char* token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }

    token = strtok_r(NULL, "=", &query);
	if (token == NULL) {
		return false;
	}

    strcpy(field, token);
    token = strtok_r(NULL, "\n", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(value, token);
    return true;
}
