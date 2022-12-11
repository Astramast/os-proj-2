#include "server.h"
#include "client.h"
/*
Authors : 
Installé Arthur Matricule 495303
Kevin Issa Matricule 514550
*/

int main(int argc, char const *argv[]){
    printf("STARTING\n");
	if (strcmp(argv[1], "")==0){
		printf("No file given. Usage : ./smalldb <db_file_path>");
		exit(-1);
	}
	const char *db_path = argv[1];
	printf("Entered path : %s\n", db_path);
    database_t db;
    db_init(&db);
    db_load(&db, db_path);
    
    server_handler(&db);

    db_save(&db, db_path);
	printf("Bye bye!\n");
    return 0;
}
