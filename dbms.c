#include "dbms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Database* dbms_create() {
    Database* db = malloc(sizeof(Database));
    db->index = NULL;
    db->data = malloc(10 * sizeof(void*)); // initial data array size
    db->data_size = 0;
    pthread_mutex_init(&db->lock, NULL);
    db->transaction_log = fopen("transaction_log.txt", "w+");
    return db;
}

void dbms_insert(Database* db, int key, void* data) {
    pthread_mutex_lock(&db->lock);
    db->index = cluster_insert(db->index, key);
    db->data[db->data_size++] = data;
    fprintf(db->transaction_log, "INSERT %d %p\n", key, data);
    pthread_mutex_unlock(&db->lock);
}

void* dbms_search(Database* db, int key) {
    pthread_mutex_lock(&db->lock);
    Cluster* found = cluster_search(db->index, key);
    if (found) {
        return db->data[found->key - 1]; // return data associated with the key
    } else {
        return NULL;
    }
    pthread_mutex_unlock(&db->lock);
}

void dbms_delete(Database* db, int key) {
    pthread_mutex_lock(&db->lock);
    db->index = cluster_delete(db->index, key);
    // remove data associated with the key
    for (int i = 0; i < db->data_size; i++) {
        if (db->data[i] != NULL && *(int*)db->data[i] == key) {
            db->data[i] = NULL;
            break;
        }
    }
    fprintf(db->transaction_log, "DELETE %d\n", key);
    pthread_mutex_unlock(&db->lock);
}

void dbms_commit(Database* db) {
    pthread_mutex_lock(&db->lock);
    rewind(db->transaction_log);
    fprintf(db->transaction_log, "COMMIT\n");
    fflush(db->transaction_log);
    pthread_mutex_unlock(&db->lock);
}

void dbms_rollback(Database* db) {
    pthread_mutex_lock(&db->lock);
    rewind(db->transaction_log);
    char line[256];
    while (fgets(line, 256, db->transaction_log)) {
        if (strncmp(line, "INSERT", 6) == 0) {
            // undo insert operation
            int key;
            void* data;
            sscanf(line, "INSERT %d %p", &key, &data);
            dbms_delete(db, key);
        } else if (strncmp(line, "DELETE", 6) == 0) {
            // undo delete operation
            int key;
            sscanf(line, "DELETE %d", &key);
            dbms_insert(db, key, NULL);
        }
    }
    pthread_mutex_unlock(&db->lock);
}

void dbms_print_index(Database* db) {
    pthread_mutex_lock(&db->lock);
    printf("Index: ");
    cluster_inorder(db->index);
    printf("\n");
    pthread_mutex_unlock(&db->lock);
}
void dbms_save(Database* db, const char* filename) {
    pthread_mutex_lock(&db->lock);
    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "INDEX\n");
        cluster_inorder_file(db->index, file);
        fprintf(file, "DATA\n");
        for (int i = 0; i < db->data_size; i++) {
            if (db->data[i] != NULL) {
                fprintf(file, "%d %p\n", i + 1, db->data[i]);
            }
        }
        fclose(file);
    }
    pthread_mutex_unlock(&db->lock);
}

void dbms_load(Database* db, const char* filename) {
    pthread_mutex_lock(&db->lock);
    FILE* file = fopen(filename, "r");
    if (file) {
        char line[256];
        while (fgets(line, 256, file)) {
            if (strncmp(line, "INDEX", 5) == 0) {
                db->index = cluster_load(file);
            } else if (strncmp(line, "DATA", 4) == 0) {
                while (fgets(line, 256, file)) {
                    int key;
                    void* data;
                    sscanf(line, "%d %p", &key, &data);
                    db->data[key - 1] = data;
                }
            }
        }
        fclose(file);
    }
    pthread_mutex_unlock(&db->lock);
}