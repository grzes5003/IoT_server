/**
 * Code from https://nachtimwald.com/2019/04/12/thread-pool-in-c/
 */
#ifndef SERVER_TPOOL_H
#define SERVER_TPOOL_H

#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

struct tpool;
typedef struct tpool tpool_t;

typedef void (*thread_func_t)(void *arg);
tpool_t *tpool_create(size_t num);
void tpool_destroy(tpool_t *t);

int tpool_add_work(tpool_t *tm, thread_func_t func, void *arg);
void tpool_wait(tpool_t *tm);

typedef struct tpool_work {
    thread_func_t       func;
    void                *arg;
    struct tpool_work   *next;
} tpool_work_t;

struct tpool {
    tpool_work_t    *work_first;
    tpool_work_t    *work_last;
    pthread_mutex_t  work_mutex;
    pthread_cond_t   work_cond;
    pthread_cond_t   working_cond;
    size_t           working_cnt;
    size_t           thread_cnt;
    bool             stop;
};

#endif //SERVER_TPOOL_H
