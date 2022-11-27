#include "student.h"
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <cstring>
using std::strcmp;

void student_to_str(char* buffer, student_t* student) {
  /**
   * function: store in the buffer a human readable version of the student information
   * parameter: buffer: variable to store dynamically the content of the student, student: all the informations 
   *                                                                                           about a student in the data base
   * 
   */
	snprintf(buffer, sizeof(student_t)+100, "Id : %u / Prénom : %s / Nom : %s / Section : %s / Birthdate : %i/%i/%i\n", student->id, student->fname, student->lname, student->section, student->birthdate.tm_year+1900, student->birthdate.tm_mon+1, student->birthdate.tm_mday);
}

int student_equals(student_t* student_1, student_t* student_2) {
/**
 * function: return 1 if both students are the same else return 0
 * paramater: student 1 and student 2, students in the data base
 * 
 */
  if(student_1->id == student_2->id){
    if(strcmp(student_1->fname,student_2->fname)==0){
      if(strcmp(student_1->lname,student_2->lname)==0){
        if(strcmp(student_1->section, student_2->section)==0){
          if(student_1->birthdate.tm_mday==student_2->birthdate.tm_mday){
            if(student_1->birthdate.tm_mon == student_2->birthdate.tm_mon){
              if(student_1->birthdate.tm_year == student_2->birthdate.tm_year){
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

