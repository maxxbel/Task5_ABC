#include <semaphore.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>

// Semaphores that represent different states of the table
sem_t paperAndTobacco;
sem_t tobaccoAndMatches;
sem_t matchesAndPaper;
sem_t tableIsEmpty;
// A random seed, for generating random numbers
unsigned int seed = 80;
// functions representing different smokers
void *Smoker1(void *);
void *Smoker2(void *);
void *Smoker3(void *);
// A regulator function
void *Regulator(void *);
int main() {
    srand(seed);
    // initialization of the semaphores and threads
    sem_init(&matchesAndPaper, 0, 0);
    sem_init(&tobaccoAndMatches, 0, 0);
    sem_init(&paperAndTobacco, 0, 0);
    sem_init(&tableIsEmpty, 0, 1);

    pthread_t smoker1;
    pthread_t smoker2;
    pthread_t smoker3;
    pthread_t regulator;
    pthread_create(&smoker1, nullptr, Smoker1, nullptr);
    pthread_create(&smoker2, nullptr, Smoker2, nullptr);
    pthread_create(&smoker3, nullptr, Smoker3, nullptr);
    pthread_create(&regulator, nullptr, Regulator, nullptr);
    // Makes the main thread wait for the regulator, which causes program to run forever
    Regulator(NULL);
    return 0;
}

// A smoker with tobacco
void *Smoker1(void *) {
    while (1) {
        sem_wait(&matchesAndPaper);
        printf("Smoker with tobacco is smoking!\n");
        sleep(1);
        sem_post(&tableIsEmpty);
    }
}

// A smoker with paper
void *Smoker2(void *) {
    while (1) {
        // waits for the right ingredients
        sem_wait(&tobaccoAndMatches);
        printf("Smoker with paper is smoking!\n");
        // Smokes here
        sleep(1);
        sem_post(&tableIsEmpty);
    }
}

// Other smokers are the same
// A smoker with matches
void *Smoker3(void *) {
    while (1) {
        sem_wait(&paperAndTobacco);
        printf("Smoker with matches is smoking!\n");
        sleep(1);
        sem_post(&tableIsEmpty);
    }
}
// A facilitator
void *Regulator(void *) {
    while (1) {
        // waits when the smoker has smoked
        sem_wait(&tableIsEmpty);
        // randomly chooses ingredients
        switch(rand() % 3) {
            case 0:
                printf("Matches and paper are now on the table!\n");
                sem_post(&matchesAndPaper);
                break;
            case 1:
                printf("Tobacco and matches are now on the table!\n");
                sem_post(&tobaccoAndMatches);
                break;
            case 2:
                printf("Paper and tobacco are now on the table!\n");
                sem_post(&paperAndTobacco);
                break;
        }
    }
}




