#pragma once
#include <stdint.h>
#include <mysql.h>
#include <pthread.h>
int insert_user(DBConnectionPool* pool, uint64_t user_id, const char* name,
    const char* phone, int age, const char* school, const char* gender);
int batch_insert_users(DBConnectionPool* pool, int start_id, int count);