#include <pthread.h>
#include <assert.h>
#include <stdio.h>

void *run_func(void *arg) 
{
    for (int i = 0; i < 5; i++) 
    {
        printf("%s: %d\n", (char *)arg, i);
    }
    
    return NULL;
}

int main(void) 
{
    pthread_t p1, p2;
    int rc;

    printf("Launching threads\n");
    
    rc = pthread_create(&p1, NULL, run_func, "thread 1"); assert(rc == 0);
    rc = pthread_create(&p2, NULL, run_func, "thread 2"); assert(rc == 0);

    rc = pthread_join(p1, NULL); assert(rc == 0);
    rc = pthread_join(p2, NULL); assert(rc == 0);

    printf("Threads complete!\n");

    return 0;
}
