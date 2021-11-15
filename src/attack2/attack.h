#ifndef ATTACK_H
    #define ATTACK_H

    #include <time.h>
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <pthread.h>

    #include "../present/present.h"
    #include "../sort/sort.h"

    typedef struct thread_args
    {
        u32_t m1;
        u32_t m2;

        u32_t enc1;
        u32_t enc2;

        u32_t * enc_keys;
        u64_t * enc_msgs;

        u32_t * dec_keys;
        u64_t * dec_msgs;
        
        u32_t start;
        u32_t end;

        u32_t num;

    }thread_args;

    pthread_barrier_t barrier1;
    pthread_barrier_t barrier2;

    void attack( u32_t m1, u32_t enc1, u32_t m2, u32_t enc2);
    void * threadAttack( void *voidArgs);
    u32_t dichotomie(u64_t * tab, u64_t mot, u32_t size);    

#endif