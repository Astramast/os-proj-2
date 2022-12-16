#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include "student.h"

void db_save(database_t *db, const char *path) {
    FILE *f = fopen(path, "wb");

    if (fwrite(db->data, sizeof(student_t), db->lsize, f) < 0) {
        perror("Could not write in the DB file");
        exit(1);
    }
    fclose(f);
	printf("%ld students saved.\n", db->lsize);
}

void db_load(database_t *db, const char *path) {
	printf("Entered db_load\n");
    FILE *file = fopen(path, "rb");
    if (!file) {
        printf("%s\n", "DB file does not exist. New file will be created at saving.");
        return;
    }
    student_t student;
    while (fread(&student, sizeof(student_t), 1, file)) {
        db_add(db, student);
    }
    fclose(file);
	printf("%ld students loaded.\n", db->lsize);
}

void db_init(database_t *db) {
	printf("Entered db_init\n");
	db->data = (student_t*) malloc(sizeof(student_t)*10000);
	if (db->data == NULL){
		perror("DB's size too large for memory-chan TwT'");
	}
	db->lsize = 0;
	db->psize = sizeof(student_t)*10000;
}

void db_add(database_t *db, student_t student) {
	if (db->lsize*sizeof(student_t) == db->psize){db_extend_memory(db);}
	db->data[db->lsize] = student;
	db->lsize++;
}

void db_extend_memory(database_t *db){
	student_t* temp;
	temp = (student_t*) malloc(10*(db->psize));
	memcpy(temp, db->data, sizeof(student_t)*db->lsize);
	free(db->data);
	db->data = temp;
	db->psize = 10*db->psize;
}

void db_remove(database_t* db, int indice){
	for (size_t i = indice+1; i<=db->lsize; i++){
		db->data[i-1] = db->data[i];
	}
	db->lsize--;
}
