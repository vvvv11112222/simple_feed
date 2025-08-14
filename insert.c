#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"
MYSQL* conn;
void init_insert() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(conn, "192.168.215.130", "root", "rootpass", NULL, 3307, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(1);
    }
}
// 插入数据（主方法）
void insert_data() {
    init_insert();
    srand(time(NULL));  // 初始化随机数生成器
    crc32_init();

    // 插入1000个用户数据
    for (long user_id = 100000000; user_id < 100001000; user_id++) {
        char name[50];
        snprintf(name, sizeof(name), "User %ld", user_id);
        unsigned int hash_value = crc32_hash(user_id);
        hash_value = hash_value % 256;
        printf("%u\n", hash_value);
        int db_index = hash_value % 8;
        printf("%d\n", db_index);
        int table_index = hash_value % 32;
        printf("%d\n", table_index);
        char query[1024];
        snprintf(query, sizeof(query),
            "INSERT INTO user_%d.user_info_%d (id, name, phone, age, school, gender, created_at) "
            "VALUES (%ld, '%s', '%s', 20, '%s', '%c', NOW());", db_index, table_index, user_id, name, "123456789", "school", 'M');
        printf("%s\n", query);
        if (mysql_query(conn, query)) {
            fprintf(stderr, "INSERT failed - user_id: %ld, db: %d, table: %d, error: %s\nSQL: %s\n",
                user_id, db_index, table_index, mysql_error(conn), query);
        }
       
    }
    // 插入 Feed 数据
    for (long feed_id = 1; feed_id <= 1000; feed_id++) {
        char content[255];
        long user_id = feed_id + 100000000;
        snprintf(content, sizeof(content), "This is content for feed %ld of user %ld", feed_id, user_id);
        unsigned int hash_value = crc32_hash(user_id);
        int table_index = hash_value % 64;
        int db_index = table_index / 8;
        char query1[1024];
        snprintf(query1, sizeof(query1),
            "INSERT INTO feed_%d.ids_%d (user_id, feed_id, created_at) "
            "VALUES (%ld, %ld, NOW());", db_index, table_index, user_id, feed_id);

        if (mysql_query(conn, query1)) {
            fprintf(stderr, "INSERT feed failed: %s\n", mysql_error(conn));
        }
        char query2[1024];
        snprintf(query2, sizeof(query2),
            "INSERT INTO feed_content_%d.content_%d (feed_id, content, created_at) "
            "VALUES (%ld, '%s', NOW());", db_index, table_index, feed_id, content);

        if (mysql_query(conn, query2)) {
            fprintf(stderr, "INSERT feed content failed: %s\n", mysql_error(conn));
        }
    }
       //// 插入 10 个关注的用户
    for (int user_id = 100000000; user_id < 100001000; user_id++) {
        for (int i = 0; i < 10; i++) {
            long friend_id = 100000000 + rand() % 1000;  // 随机生成一个用户作为朋友
            unsigned int hash_value = crc32_hash(user_id);
            int table_index = hash_value % 64;
            int db_index = table_index / 8;

            char query[1024];
            snprintf(query, sizeof(query),
                "INSERT INTO friends_%d.fri_%d (user_id, friend_id, created_at) "
                "VALUES (%ld, %ld, NOW());", db_index, table_index, user_id, friend_id);

            if (mysql_query(conn, query)) {
                fprintf(stderr, "INSERT friend failed: %s\n", mysql_error(conn));
            }
        }
    }
    mysql_close(conn);
}
