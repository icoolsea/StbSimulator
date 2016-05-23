/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include "../../include/blccore.h"

#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>

#define BLCCORE_BUFFER_SIZE 4400

void *BlcCore_Malloc(size_t size) { return malloc(size); }

void *BlcCore_Realloc(void *mem, size_t size) { return realloc(mem, size); }

void BlcCore_Free(void *ptr) { free(ptr); }

int32_t BlcCore_ThreadCreate(int *thread_id, void (*entry_func)(void *),
                             void *arg, uint32_t stack_size,
                             uint32_t priority) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stack_size);

    return pthread_create((pthread_t *)thread_id, &attr, (void *)entry_func, arg);
}

int32_t BlcCore_ThreadDelete(int thread_id) {
    return pthread_cancel(thread_id);
}

int32_t BlcCore_ThreadDelay(uint32_t millisecond) {
    usleep(1000 * millisecond);
    return 0;
}

/* Semaphore API */
int32_t BlcCore_SemCreate(int *sem_id, uint32_t sem_initial_value) {
    sem_t *p_sem = BlcCore_Malloc(sizeof(sem_t));
    if (p_sem == NULL)
        return -1;

    int ret = sem_init((sem_t *)p_sem, 0, sem_initial_value);

    *sem_id = (int)p_sem;
    return ret;
}

int32_t BlcCore_SemPost(int sem_id) {
    int ret = sem_post((sem_t *)sem_id);

    return ret;
}

int32_t BlcCore_SemWait(int sem_id) {
    int ret = sem_wait((sem_t *)sem_id);

    return ret;
}

//返回0，成功；返回-1超时；返回-2失败
int32_t BlcCore_SemWaitTime(int sem_id, uint32_t microsecs) {
    struct timespec abstime;
    BlcTime time;

    BlcCore_GetLocalTime(&time);

    abstime.tv_sec = time.seconds + microsecs * 1000 / (1000 * 1000 * 1000);
    abstime.tv_nsec =
            time.microsecs * 1000 + microsecs * 1000 % (1000 * 1000 * 1000);

    int ret = sem_timedwait((sem_t *)sem_id, &abstime);

    if (ret == 0)
        return 0;
    else if (ret == -1) {
        if (errno == ETIMEDOUT)
            return -1;
        else
            return -2;
    } else
        return -2;
}

int32_t BlcCore_SemDelete(int sem_id) {
    int ret = sem_destroy((sem_t *)sem_id);

    BlcCore_Free((sem_t *)sem_id);

    return ret;
}

/* OS Time/Tick related API */
int32_t BlcCore_GetLocalTime(BlcTime *time) {
    struct timeval tv;
    struct timezone tz;
    int status;

    if (NULL == time)
        return -1;

    status = gettimeofday(&tv, &tz);

    time->seconds = tv.tv_sec;
    time->microsecs = tv.tv_usec;

    if (status == 0)
        return 0;
    else
        return -2;
}

/* Abstraction for printf statements */
void BlcCore_printf(const char *string, ...) {
    va_list ptr;
    char msg_buffer[BLCCORE_BUFFER_SIZE];
    va_start(ptr, string);
    vsnprintf(msg_buffer, BLCCORE_BUFFER_SIZE, string, ptr);
    va_end(ptr);
    msg_buffer[BLCCORE_BUFFER_SIZE - 1] = '\0';

    struct timeval tv;
    char buf[128];
    gettimeofday(&tv, NULL);
    struct tm *ptm = localtime(&tv.tv_sec);
    sprintf(buf, "[%4d-%02d-%02d %02d:%02d:%02d:%07.3f] ", (1900 + ptm->tm_year),
            (1 + ptm->tm_mon), ptm->tm_mday, ptm->tm_hour, ptm->tm_min,
            ptm->tm_sec, tv.tv_usec / 1000.0);

    printf("\033[0;31m[blcplug] %s%s\033[0m", buf, msg_buffer);
}

void *BlcCore_memset(void *s, int ch, size_t n) { return memset(s, ch, n); }
void *BlcCore_memcpy(void *dest, const void *src, size_t n) {
    return memcpy(dest, src, n);
}
int BlcCore_memcmp(const void *buf1, const void *buf2, unsigned int count) {
    return memcmp(buf1, buf2, count);
}

unsigned int BlcCore_strlen(char *s) { return strlen(s); }
char *BlcCore_strchr(const char *s, char c) { return strchr(s, c); }
int BlcCore_strcmp(char *str1, char *str2) { return strcmp(str1, str2); }
int BlcCore_strncmp(char *str1, char *str2, int maxlen) {
    return strncmp(str1, str2, maxlen);
}

char *BlcCore_strncpy(char *dest, char *src, size_t maxlen) {
    return strncpy(dest, src, maxlen);
}

int BlcCore_sscanf(const char *buffer, const char *format, ...) {
    int ret = -1;
    va_list ptr;
    va_start(ptr, format);
    ret = vsscanf(buffer, format, ptr);
    va_end(ptr);

    return ret;
}
int BlcCore_sprintf(char *buffer, const char *format, ...) {
    int ret = -1;
    va_list ptr;
    va_start(ptr, format);
    ret = vsprintf(buffer, format, ptr);
    va_end(ptr);

    return ret;
}

int BlcCore_atoi(const char *nptr) { return atoi(nptr); }
