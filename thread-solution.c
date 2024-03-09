#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Global Variable
#define MAX_UPDATES 2000000

/* Struct a shared variable to store result */
struct shared_data {
    int value;  
    int bonus;
};

/* Global shared variable */
struct shared_data *counter;
/* Mutex lock */
pthread_mutex_t mutex;

/* Thread1 function */
void *thread1() {
    int i = 0;
    while (i < MAX_UPDATES) {
        /*Entry Section*/
        if (pthread_mutex_trylock(&mutex) == 0){
            /*Critical Section*/
            if (counter->value%100==0) { //Thread1 needs to be designed in a way that every time it sees (counter−> value%100) == 0 it increments counter−> value by 100
                counter->value += 100;
                i+=100;
                counter->bonus++;
            }
        /*Exit Section*/
        counter->value++; //Increment shared count value by one
        pthread_mutex_unlock(&mutex);
        i++; //Increment thread1 update count
        }
    }
    /*Remainder Section*/
    printf("I'm thread1, I did %d updates and I got the bonus for %d times, counter = %d\n", i, counter->bonus, counter->value);
    return NULL;
}

/* Thread2 function */
void *thread2() {
    int i = 0;
    while (i < MAX_UPDATES) {
        /*Entry Section*/
        if (pthread_mutex_lock(&mutex) == 0){
            /*Critical Section*/
            counter->value++; //Increment shared count value by one
            /*Exit Section*/
            pthread_mutex_unlock(&mutex);
            i++; //Increment thread2 update count
        }
    }
    /*Remainder Section*/
    printf("I'm thread2, I did %d updates, counter = %d\n", i, counter->value);
    return NULL;
}


int main() {
    pthread_t tid[2];
    int rc;

    /* Allocate memory for shared data */
    counter = (struct shared_data *) malloc(sizeof(struct shared_data));
    counter->value = 0;
    counter->bonus = 0;

    /* Initialize mutex lock */
    if ((pthread_mutex_init(&mutex, NULL))) {
        printf("Error occured when initialize mutex lock.");
        exit(0);
    }

    pthread_attr_t attr;
    if ((pthread_attr_init(&attr))) {
        printf("Error occured when initialize pthread_attr_t.");
        exit(0);
    }

    /* Create thread1 */
    if ((rc = pthread_create(&tid[0], &attr, thread1, NULL))) {
        fprintf(stderr, "ERROR: pthread_create 1, rc: %d\n", rc);
        exit(0);
    }
    /* Create thread2 */
    if ((rc = pthread_create(&tid[1], &attr, thread2, NULL))) {
        fprintf(stderr, "ERROR: pthread_create 2, rc: %d\n", rc);
        exit(0);
    }

    /* Wait for threads to finish */
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    /* Parent print */
    printf("from parent counter = %d\n", counter->value);
 
    /* Clean up */
    pthread_mutex_destroy(&mutex);
    free(counter);
    pthread_exit(NULL);

    return 0;
}
