#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

typedef unsigned __int128 u128_t ;
typedef unsigned long long u64_t ;
typedef unsigned int u32_t;
typedef unsigned char u8_t;

#define ROUNDS 10
#define SIZE 0xffffff
size_t NBTHREAD = 1;

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



u32_t permutation( u32_t m ){
    static const u32_t pbox[24] = {
        0,4,8,12,16,20,1,5,9,13,17,21,2,6,10,14,18,22,3,7,11,15,19,23
    };

    u32_t result = 0;
    for ( int i = 23; i >= 0; i-- )
    { 
        result <<= 1;
        if ( m & (1 << pbox[i]) )
            result |= 1;        
    }
    return result;

}

u32_t subsitution( u32_t m ){
    static const u32_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };

    u32_t m2 = 0;
    for(int i = 5; i >= 0; i-- ){
        m2 = m2 << 4 ;
        m2 = m2 | sbox[ 15 & ( m >> ( i * 4 ) ) ];
    }
    return m2;

}


u32_t encryption_24( u32_t * keys, u32_t m ){
    u32_t state = m;
    for( int i = 0; i < ROUNDS; i++ ) {
        state = state ^ keys[i];
        state = subsitution(state);
        state = permutation(state);   
    }
    state = state ^ keys[ROUNDS];
    return state & 0xffffff;
}


u32_t permutation_inv( u32_t m ){
    static const u32_t xbop[24] = {
        0,6,12,18,1,7,13,19,2,8,14,20,3,9,15,21,4,10,16,22,5,11,17,23
    };

    u32_t result = 0;
    for ( int i = 23; i >= 0; i-- )
    { 
        
        result <<= 1;
        if ( m & (1 << xbop[i]) )
            result |= 1;
            
    }
    return result;

}

u32_t subsitution_inv( u32_t m ){
    static const u32_t xbos[16] = {
        0x5,0xe,0xf,0x8,0xc,0x1,0x2,0xd,0xb,0x4,0x6,0x3,0x0,0x7,0x9,0xa
    };

    u32_t m2 = 0;
    for(int i = 5; i >= 0; i-- ){
        m2 = m2 << 4 ;
        m2 = m2 | xbos[ 15 & ( m >> ( i * 4 ) ) ];
    }
    return m2;

}

u32_t decryption_24( u32_t * keys, u32_t m ){
    u32_t state = m;
    state = state ^ keys[ROUNDS];
    for( int i = ROUNDS - 1 ; i >= 0; i-- ) {
        state = permutation_inv(state);
        state = subsitution_inv(state);
        state = state ^ keys[i];
    }
    
    return state & 0xffffff;
}


u32_t * keyScheduling( u32_t key, u32_t * keys_24 ){


    static const u32_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };


 

    u128_t cache = ((( ~((u128_t) 0)) << 48 ) >> 48) ;
    u128_t mainKey = ((u128_t) key) << 56 ;
    u128_t K = mainKey;

    keys_24[ 0 ] = (K >> 16) & 0xffffff;
    for(int i = 1; i < ROUNDS + 1; i++){
        K =  ( K << 61 | K >> 19 ) & cache;
        K = ( (((u128_t) sbox[ K >> 76 ]) << 76 ) | ( K & ~((u128_t) 0b1111 << 76) ) ) & cache;
        K = (( (((K >> 15) & 31 ) ^ (i) ) << 15 ) | ( K & ~((u128_t)0b11111000000000000000) ) ) & cache;;
        keys_24[ i ] = (K >> 16) & 0xffffff;
    }

    
    return keys_24;
}



int  partition(u64_t * vect1, u32_t * vect2, int deb, int fin, int pivot ){
    u64_t tmp1 = vect1[pivot];
    vect1[pivot] = vect1[fin];
    vect1[fin] = tmp1;

    u32_t tmp2 = vect2[pivot];
    vect2[pivot] = vect2[fin];
    vect2[fin] = tmp2;


    int j = deb;
    for(int i = deb; i <= fin - 1; i++){
        if( vect1[i] <= vect1[fin]){
            tmp1 = vect1[i];
            vect1[i] = vect1[j];
            vect1[j] = tmp1;

            tmp2 = vect2[i];
            vect2[i] = vect2[j];
            vect2[j] = tmp2;

            j++;
        }
    }

    tmp1 = vect1[fin];
    vect1[fin] = vect1[j];
    vect1[j] = tmp1;

    tmp2 = vect2[fin];
    vect2[fin] = vect2[j];
    vect2[j] = tmp2;

    return j;

}

void quickSort( u64_t * vect1, u32_t * vect2, int deb, int fin){
    if(deb < fin){
        int pivot = deb; // choix pivot
        pivot = partition(vect1, vect2, deb, fin, pivot);
        quickSort(vect1, vect2, deb, pivot - 1 );
        quickSort(vect1, vect2, pivot + 1 , fin );
    }
}

void sort(u64_t * msgs, u32_t * keys, u32_t nbele){
    if(nbele >= 0x7FFFFFFF){
        printf("ERROR"); //should never happend
        exit(-1);
        }

    quickSort(msgs, keys, 0, (int) nbele - 1);
}




u32_t dichotomie(u64_t * tab, u64_t mot, u32_t size){
    int a = 0;
    int b = size;
    int m;

    while (a <= b){
        m = (a + b) / 2;
        if(tab[m] == mot){
            printf("\n\n mot: %llx \t tab[m]: %llx ", mot, tab[m]);
            printf("OUI\n");
            return (u32_t) (m + 1);
            }
        else if (tab[m] < mot)
            a = m + 1;
        else
            b = m - 1;
        }
    return 0;
}




void * threadAttack( void *voidArgs){
    thread_args * args = (thread_args *)voidArgs;

    u32_t m1 = args->m1;
    u32_t m2 = args->m2;
    
    u32_t enc1 = args->enc1 ;
    u32_t enc2 = args->enc2 ;

    u32_t * enc_keys = args->enc_keys;
    u64_t * enc_msgs = args->enc_msgs;

    u32_t * dec_keys = args->dec_keys;
    u64_t * dec_msgs = args->dec_msgs;


    u32_t mysize = (args->end - args->start) + 1;

    printf(" - start %x - end %x\n",  args->start, args->end);

    
    //keys 
    u32_t * keys;
    keys = malloc( (ROUNDS + 1) * sizeof(u32_t) );

    u128_t * keys_80;
    keys_80 = malloc( (ROUNDS + 1) * sizeof(u128_t) );

    //
    // u32_t key = args->start;
    for(u32_t key = args->start; key <= args->end;  key++){
        keys = keyScheduling( key, keys );
        
        //
        enc_msgs[key] = ( ((u64_t) encryption_24(keys, m1)) << 32) | ((u64_t) encryption_24(keys, m2)) ;
        enc_keys[key] = key;

        //
        dec_msgs[key] = ( ((u64_t) decryption_24(keys, enc1)) << 32) | ((u64_t) decryption_24(keys, enc2)) ;
        dec_keys[key] = key;
        
        if((key & 0x00ffff) == 0x00ffff ){
            // printf("%x\n", key);
        }
    }

    pthread_barrier_wait(&barrier1);

    if(args->num == 0 || NBTHREAD == 1 ){
        sort(enc_msgs, enc_keys, SIZE );
    }
    else if( args->num == 1 || NBTHREAD == 1){
        sort(dec_msgs, dec_keys, SIZE );
    }

    pthread_barrier_wait(&barrier2);

    u64_t msg;
    for(u32_t i = args->start; i <= args->end; i ++ ){
        msg = enc_msgs[i];
        u32_t indice = dichotomie(dec_msgs, msg, SIZE);
        if(indice != 0){
            printf("i: %x - start %x - end %x\n", i, args->start, args->end);
            // printf("enc msg : %*llx  key: %*x\n", 10, enc_msgs[i], 10, enc_keys[i] );
            //
            // printf("%*llx - %*x\n", 10, dec_msgs[indice - 1], 10, dec_keys[indice - 1]  );

            printf("k1: %x   k2: %x \n", enc_keys[i], dec_keys[indice - 1]);
        }
    }


    free(keys);
    free(keys_80);
    
    return 0;
}

void attack(u32_t m1, u32_t enc1, u32_t m2, u32_t enc2){
    
    // tabs
    u32_t * enc_keys = malloc( sizeof(u32_t) * SIZE );
    u64_t * enc_msgs = malloc( sizeof(u64_t) * SIZE );

    u32_t * dec_keys = malloc( sizeof(u32_t) * SIZE );
    u64_t * dec_msgs = malloc( sizeof(u64_t) * SIZE );

    pthread_t * idList = malloc(NBTHREAD * sizeof(pthread_t));
    thread_args * args = malloc( NBTHREAD * sizeof(thread_args) );

    pthread_barrier_init(&barrier1, NULL, NBTHREAD + 1);//TODO remove barrier global variable
    pthread_barrier_init(&barrier2, NULL, NBTHREAD + 1);


    u32_t threadSize = SIZE/NBTHREAD;

    for( size_t i = 0; i < NBTHREAD; i++){
        args[i].m1 = m1;
        args[i].m2 = m2;
        
        args[i].enc1 = enc1;
        args[i].enc2 = enc2;
        
        args[i].enc_keys = enc_keys;
        args[i].enc_msgs = enc_msgs;

        args[i].dec_keys = dec_keys;
        args[i].dec_msgs = dec_msgs;


        args[i].start = i * threadSize;
        args[i].end = args[i].start + threadSize - 1;

        args[i].num = i;

        if(i == (NBTHREAD-1))
            args[i].end += SIZE%NBTHREAD + 1;
        
        pthread_create(&idList[i], NULL, threadAttack, &args[i] );
        // getchar()
    }

    pthread_barrier_wait(&barrier1);
    printf("début tris!\n");
    pthread_barrier_wait(&barrier2);
    printf("fin tris!\n");
    
    for( size_t i = 0; i < NBTHREAD; i++){
        pthread_join(idList[i], NULL );
        printf("join! \n");
    }
    
    

    
    



    //
    free(enc_keys);
    free(enc_msgs);
    free(dec_keys);
    free(dec_msgs);
    free(args);
    free(idList);


    
}

double measure_time(struct timespec *before, struct timespec *after) {
    return (double)(after->tv_sec - before->tv_sec) +  (double)(after->tv_nsec - before->tv_nsec) / 1000000000;
}

int main(int argc, char const *argv[])
{   
    if(argc < 2){
        printf("ERROR - FICHIER MANQUANT\n");
        return 0 ;
    }

    char const *fileName = argv[1];
    FILE *fp;
    fp = fopen(fileName, "w");
    fprintf(fp, "nombre de thread,temps(s),\n" );
    struct timespec a1, a2;
    
    for(int i = 1; i < 20; i++){
        NBTHREAD = i;
        clock_gettime(CLOCK_MONOTONIC_RAW, &a1);
        attack((u32_t) 0xef333a, (u32_t) 0x250324, (u32_t)0xfb432f, (u32_t) 0xa71982);
        clock_gettime(CLOCK_MONOTONIC_RAW, &a2);
        fprintf(fp, "%d,%f,\n", i, measure_time(&a1, &a2 ) );
        printf("time : %f  \n", measure_time(&a1, &a2 )  );
        
    }
    return 0;
}
