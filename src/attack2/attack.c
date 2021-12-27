
#include "attack.h"
// #include "../common.h"

#define SIZE 0xffffff
#define NBTHREAD 10

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

    if(args->num == 0){
        sort(enc_msgs, enc_keys, SIZE );
    }
    else if( args->num == 1){
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


// no*n
//(oui)*