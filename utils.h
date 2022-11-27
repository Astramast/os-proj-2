#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "query.h"

/**
 * @brief Effectue un `read` en vérifiant la valeur de retour.
 * Si la valeur de retour est négative, la fonction affiche l'erreur et termine
 * le processus avec `exit(1)`.
 *
 * @param fd le file descriptor depuis lequel lire
 * @param buffer le buffer dans lequel mettre les données lues
 * @param nbytes le nombre de bytes maximum à lire
 * @return size_t le nombre de bytes lus
 */
size_t safe_read(int fd, void *buffer, size_t nbytes);

/**
 * @brief Effectue un `write` en vérifiant la valeur de retour.
 * Si la valeur de retour est négative, la fonction affiche l'erreur et termine
 * le processus avec `exit(1)`.
 *
 * @param fd le file descriptor dans lequel écrire
 * @param buffer le buffer dans lequel se trouvent les données
 * @param nbytes le nombre de bytes à écrire
 * @return size_t le nombre de bytes écrits
 */
size_t safe_write(int fd, const void *buffer, size_t nbytes);

/**
 * @brief the result of a query.
 **/

int _checked(int ret, char* calling_function);

// The macro allows us to retrieve the name of the calling function
#define checked(call) _checked(call, #call)

// Même macro que checked mais pour write() (où 0 signifie
// aussi une erreur).
#define checked_wr(call) _checked(((call) - 1), #call)

bool precise_read(int fd, char* buffer, int size);

void log_query(query_result_t *result);
/**
 * @brief: Create a file with the data in bind with the query in the log folder.
 * 
 */
#endif  // _UTILS_H
