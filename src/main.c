#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAIN_FILE


#include "attack2/attack.h"


#include "sort/sort.h"
#include "present/present.h"
#include "attack/attack.h"



double measure_time(struct timespec *before, struct timespec *after) {
    return (double)(after->tv_sec - before->tv_sec) +  (double)(after->tv_nsec - before->tv_nsec) / 1000000000;
}

int  main(){
    // u32_t key = 0x0;
    u32_t key1 = 0x000000;
    u32_t key2 = 0x042069;
    u32_t m1   = 0x000000;
    u32_t m2   = 0x123456;

    //keys 
    u32_t * keys1;
    keys1 = malloc( (ROUNDS + 1) * sizeof(u32_t) );
    u32_t * keys2;
    keys2 = malloc( (ROUNDS + 1) * sizeof(u32_t) );

    keys1 = keyScheduling( key1, keys1 );
    keys2 = keyScheduling( key2, keys2 );
    // u32_t tmp = encryption_24(keys1, m1);
    // printf("DEC\n");
    // decryption_24(keys1, tmp);
    // return 0;
    u32_t enc1 = encryption_24(keys2, encryption_24(keys1, m1 ));
    u32_t enc2 = encryption_24(keys2, encryption_24(keys1, m2 ));

    // printf("key1  %x - key2 %x \n", key1, key2 );
    // printf("m1  %x - m2 %x \n", m1, m2 );
    // printf("enc1  %x - enc2 %x \n", enc1, enc2 );

    // t0 = clock();


    attack((u32_t) 0xef333a, (u32_t) 0x250324, (u32_t)0xfb432f, (u32_t) 0xa71982);

    // attack(m1, enc1, m2, enc2);

    return 0;

   
    

}






