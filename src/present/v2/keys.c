#include "../present.h"

// u8_t keys_substitution( u8_t m ) {

//     return (sbox[m & 0xf0] << 4) | sbox[m & 0x0f];
// }

u32_t * keyScheduling( u32_t key, u32_t * keys_24, u128_t * keys_80 ){

    static const u8_t sbox[16] = {
        0x0c,0x05,0x06,0x0b,0x09,0x00,0x0a,0x0d,0x03,0x0e,0x0f,0x08,0x04,0x07,0x01,0x02
    };

    
    u128_t cache = ((( ~((u128_t) 0)) << 48 ) >> 48) ;
    // u128_t cache = (u128_t) 10000000 ;
    u128_t mainKey = ((u128_t) key) << 56 ;
    u128_t K = mainKey;
    keys_80[ 0 ] = K;
    keys_24[ 0 ] = (K >> 16) & 0xffffff;
    
    for(int i = 1; i < ROUNDS + 1; i++){
        K =  ( K << 61 | K >> 19 )  & cache;
        ((u8_t *)&K)[9] = (sbox[ ((u8_t *)&K)[9] >> 4] << 4) |  ( 0x0f & ((u8_t *)&K)[9] ) ; 
        // K = ( (((u128_t) sbox[ K >> 76 ]) << 76 ) | ( K & ~((u128_t) 0b1111 << 76) ) ) & cache;
        K = (((u128_t)(((K >> 15) ^ ((u128_t)i) ) << 15 )) | ( K & ((u128_t)0b00000111111111111111) ))  & cache;
        //
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

    return keys_24;
}


