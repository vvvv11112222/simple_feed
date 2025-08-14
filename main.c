#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "D:/develop/mysql-8.4.6-winx64/build/create_db_table.h"
#include "D:/develop/mysql-8.4.6-winx64/build/hash.h"
#include "D:/develop/mysql-8.4.6-winx64/build/read.h"
int main() {
    create_all_databases_and_tables();
    /*insert_data();
    read_data_user(8,32,"user_","user_info_");*/
    read_data_other(8, 8, "feed_", "ids_");
    return 0;
}
