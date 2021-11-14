#include "present.h"

u32_t * keyScheduling( u32_t key, u32_t * keys_24, u128_t * keys_80 ){

    static const u32_t pbox[24] = {
        0,6,12,18,1,7,13,19,2,8,14,20,3,9,15,21,4,10,16,22,5,11,17,23
    };
    
    static const u32_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };


    // u32_t * keys_24;
    // keys_24 = malloc( (ROUNDS + 1) * sizeof(u32_t) );

    // u128_t * keys_80;
    // keys_80 = malloc( (ROUNDS + 1) * sizeof(u128_t) );

    u128_t cache = ((( ~((u128_t) 0)) << 48 ) >> 48) ;
    // u128_t cache = (u128_t) 10000000 ;
    u128_t mainKey = ((u128_t) key) << 56 ;
    u128_t K = mainKey;
    keys_80[ 0 ] = K;
    keys_24[ 0 ] = (K >> 16) & 0xffffff;
    for(int i = 1; i < ROUNDS + 1; i++){
        K =  ( K << 61 | K >> 19 ) & cache;
        // printf("K : %llx %llx \n", (u64_t) (K >> 64), (u64_t) K);
        // printf("K : %llx %llx \n", (u64_t) sbox[ K >> 76 ], (u64_t) sbox[ K >> 76 ]);
        K = ( (((u128_t) sbox[ K >> 76 ]) << 76 ) | ( K & ~((u128_t) 0b1111 << 76) ) ) & cache;
        // printf("K : %llx %llx \n", (u64_t) (K >> 64), (u64_t) K);
        K = (( (((K >> 15) & 31 ) ^ (i) ) << 15 ) | ( K & ~((u128_t)0b11111000000000000000) ) ) & cache;;
        // printf("K : %llx %llx \n", (u64_t) (K >> 64), (u64_t) K);
        // printf("\n\n");
        keys_80[ i ] = K;
        keys_24[ i ] = (K >> 16) & 0xffffff;
    }

    // printf("cache : %llx %llx \n", (u64_t) (cache >> 64), (u64_t) cache);
    // printf("mainKey : %llx %llx \n", (u64_t) (mainKey >> 64), (u64_t) mainKey);
    // for(int i = 0; i < ROUNDS + 1; i++){
    //     printf("key %d : %llx %llx\n", i, (u64_t) (keys_80[i] >> 64), (u64_t) keys_80[i]);
    // }

    // printf("\n\n\n\n");
    // for(int i = 0; i < ROUNDS + 1; i++){
    //     printf("key %d : %llx \n", i, (u64_t) (keys_24[i]));
    // }

    // free(keys_80);
    return keys_24;
}