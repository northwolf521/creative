#include "lock.h"

pthread_mutex::pthread_mutex() {
    pthread_mutex_init(&mtx, &attr);
}

pthread_mutex::~pthread_mutex() {
    pthread_mutex_destroy(&mtx);
}

void pthread_mutex::lock() {
    pthread_mutex_lock(&mtx);
}

void pthread_mutex::unlock() {
    pthread_mutex_unlock(&mtx);
}

void pthread_mutex::rdlock() {

}

void pthread_mutex::wrlock() {

}

pthread_rwlock::pthread_rwlock() {
    pthread_rwlock_init(&rwlock, &attr);
}

pthread_rwlock::~pthread_rwlock() {
    pthread_rwlock_destroy(&rwlock);
}

void pthread_rwlock::unlock() {
    pthread_rwlock_unlock(&rwlock);
}

void pthread_rwlock::rdlock() {
    pthread_rwlock_rdlock(&rwlock);
}

void pthread_rwlock::wrlock() {
    pthread_rwlock_wrlock(&rwlock);
}

void pthread_rwlock::lock() {

}
