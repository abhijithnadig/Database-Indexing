#ifndef DBMS_H
#define DBMS_H

#include "clustered_index.h"
#include <pthread.h>

typedef struct Database {
    Cluster* index; // AVL tree index
    void** data; // array of data pointers
    int data_size; // current size of data array
    pthread_mutex_t lock; // mutex for concurrency control
    FILE* transaction_log; // transaction log file
} Database;

Database* dbms_create();
void dbms_insert(Database* db, int key, void* data);
void* dbms_search(Database* db, int key);
void dbms_delete(Database* db, int key);
void dbms_commit(Database* db);
void dbms_rollback(Database* db);
void dbms_print_index(Database* db);

#endif // DBMS_H