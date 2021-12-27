#include "present.h"

u64_t * keyScheduling( u128_t key, u64_t * keys_64 ){


    static const u128_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };


 

    u128_t cache = ((( ~((u128_t) 0)) << 48 ) >> 48) ;
    u128_t mainKey = ((u128_t) key) << 56 ;
    u128_t K = mainKey;

    keys_64[ 0 ] = (K ) ;
    for(int i = 1; i < ROUNDS + 1; i++){
        K =  ( K << 61 | K >> 19 ) & cache;
        K = ( (((u128_t) sbox[ K >> 76 ]) << 76 ) | ( K & ~((u128_t) 0b1111 << 76) ) ) & cache;
        K = (( (((K >> 15) & 31 ) ^ (i) ) << 15 ) | ( K & ~((u128_t)0b11111000000000000000) ) ) & cache;;
        keys_64[ i ] = (u64_t)(K >> 16) ;
    }

    
    return keys_64;
}