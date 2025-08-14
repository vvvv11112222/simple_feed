#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read.h"

MYSQL* conn;
void read_data_user(int db_num,int table_num,char* db_prefix,char* table_prefix) {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
	}    for (int db = 0; db < db_num; db++) {
        char db_name[20];
        sprintf(db_name, "%s%d", db_prefix, db);
        if (!mysql_real_connect(conn, "192.168.215.130", "root", "rootpass", db_name, 3307, NULL, 0)) {
            printf("�������ݿ� %s ʧ�ܣ�%s\n", db_name, mysql_error(conn));
            continue;
        }
        for (int tbl = 0; tbl < table_num; tbl++) {
            char table_name[30];
            sprintf(table_name, "%s%d", table_prefix, tbl);
            char sql[200];
            sprintf(sql, "SELECT * FROM %s;", table_name);  
            if (mysql_query(conn, sql) != 0) {
                printf("��ѯ�� %s ʧ�ܣ�%s\n", table_name, mysql_error(conn));
                continue; 
            }

            MYSQL_RES* result = mysql_store_result(conn);
            if (!result) {
                printf("�� %s �޽�����ѯ����\n", table_name);
                continue;
			}
            int row_count = 0;
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result)) != NULL) {
                row_count++;
            }
            // �����жϣ��������Ϊ0��ֱ���������
            if (row_count == 0) {
                mysql_free_result(result);
                continue;  // ������ձ�
            }

            printf("\n===== ���ݿ� %s �еı� %s ������ =====\n", db_name, table_name);

            int fields = mysql_num_fields(result);
            MYSQL_FIELD* field_info = mysql_fetch_fields(result);
            for (int i = 0; i < fields; i++) {
                printf("%-10s", field_info[i].name);
            }
            printf("\n");

            mysql_data_seek(result, 0);  
            while ((row = mysql_fetch_row(result)) != NULL) {
                for (int i = 0; i < fields; i++) {
					printf("%-10s", row[i] ? row[i] : "NULL");
                }
                printf("\n");
            }

            printf("�ñ� %d ������\n", row_count);
            mysql_free_result(result);
           
        }
        mysql_close(conn);
        conn = mysql_init(NULL);  
    }

    mysql_close(conn);
}

void read_data_other(int db_num, int table_num, char* db_prefix, char* table_prefix) {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }    for (int db = 0; db < db_num; db++) {
        char db_name[20];
        sprintf(db_name, "%s%d", db_prefix, db);
        if (!mysql_real_connect(conn, "192.168.215.130", "root", "rootpass", db_name, 3307, NULL, 0)) {
            printf("�������ݿ� %s ʧ�ܣ�%s\n", db_name, mysql_error(conn));
            continue;
        }
        for (int tbl = table_num * db; tbl < table_num * (db + 1) - 1; tbl++) {
            char table_name[30];
            sprintf(table_name, "%s%d", table_prefix, tbl);
            char sql[200];
            sprintf(sql, "SELECT * FROM %s;", table_name);
            printf("%s", sql);
            if (mysql_query(conn, sql) != 0) {
                printf("��ѯ�� %s ʧ�ܣ�%s\n", table_name, mysql_error(conn));
                continue;
            }

            MYSQL_RES* result = mysql_store_result(conn);
            if (!result) {
                printf("�� %s �޽�����ѯ����\n", table_name);
                continue;
            }
            int row_count = 0;
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result)) != NULL) {
                row_count++;
            }
            // �����жϣ��������Ϊ0��ֱ���������
            if (row_count == 0) {
                mysql_free_result(result);
                continue;  // ������ձ�
            }

            printf("\n===== ���ݿ� %s �еı� %s ������ =====\n", db_name, table_name);

            int fields = mysql_num_fields(result);
            MYSQL_FIELD* field_info = mysql_fetch_fields(result);
            for (int i = 0; i < fields; i++) {
                printf("%-10s", field_info[i].name);
            }
            printf("\n");

            mysql_data_seek(result, 0);
            while ((row = mysql_fetch_row(result)) != NULL) {
                for (int i = 0; i < fields; i++) {
                    printf("%-10s", row[i] ? row[i] : "NULL");
                }
                printf("\n");
            }

            printf("�ñ� %d ������\n", row_count);
            mysql_free_result(result);

        }
        mysql_close(conn);
        conn = mysql_init(NULL);
    }

    mysql_close(conn);
}