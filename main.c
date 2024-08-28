#include "dbms.h"
#include <stdio.h>

int main() {
    Database* db = dbms_create();

    dbms_insert(db, 1, "Data 1");
    dbms_insert(db, 2, "Data 2");
    dbms_insert(db, 3, "Data 3");

    dbms_print_index(db);

    dbms_save(db, "database.db");

    dbms_delete(db, 2);

    dbms_print_index(db);

    dbms_load(db, "database.db");

    dbms_print_index(db);

    dbms_commit(db);

    dbms_rollback(db);

    dbms_print_index(db);

    dbms_delete(db, 1);

    dbms_print_index(db);

    dbms_destroy(db);

    return 0;
}