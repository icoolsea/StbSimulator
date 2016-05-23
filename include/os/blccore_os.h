#ifndef BLCCORE_OS_CORE
#define BLCCORE_OS_CORE

#ifdef __cplusplus
extern "C" {
#endif

#include "../blctype.h"
/* struct for BlcCore_GetLocalTime() */
typedef struct {
    uint32_t seconds;
    uint32_t microsecs;
} BlcTime;

#define BLCTHREAD_DEFAULT_PRIORITY 10

/* Thread API */
int BlcCore_ThreadCreate(int *thread_id, void (*entry_func)(void *), void *arg,
                         uint32_t stack_size, uint32_t priority);

int BlcCore_ThreadDelete(int thread_id);

int BlcCore_ThreadDelay(uint32_t millisecond);

int BlcCore_GetLocalTime(BlcTime *time);

/* Semaphore API */
int32_t BlcCore_SemCreate(int *sem_id, uint32_t sem_initial_value);
int32_t BlcCore_SemPost(int sem_id);
int32_t BlcCore_SemWaitTime(int sem_id, uint32_t microsecs);
int32_t BlcCore_SemWait(int sem_id);
int32_t BlcCore_SemDelete(int sem_id);

void BlcCore_printf(const char *string, ...);
int BlcCore_sscanf(const char *buffer, const char *format, ...);
int BlcCore_sprintf(char *buffer, const char *format, ...);

void *BlcCore_memset(void *s, int ch, size_t n);
void *BlcCore_memcpy(void *dest, const void *src, size_t n);
int BlcCore_memcmp(const void *buf1, const void *buf2, unsigned int count);

unsigned int BlcCore_strlen(char *s);
char *BlcCore_strchr(const char *s, char c);
int BlcCore_strcmp(char *str1, char *str2);
int BlcCore_strncmp(char *str1, char *str2, int maxlen);
char *BlcCore_strncpy(char *dest, char *src, size_t maxlen);

int BlcCore_atoi(const char *nptr);

#ifdef __cplusplus
}
#endif

#endif
