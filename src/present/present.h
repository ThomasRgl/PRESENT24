
#ifndef PRESENT_H
    #define PRESENT_H

    #include <time.h>
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>

    
    #define ROUNDS 10
    typedef unsigned __int128 u128_t ;
    typedef unsigned long long u64_t ;
    typedef unsigned int u32_t;


    //
    u32_t * keyScheduling( u32_t key, u32_t * keys_24, u128_t * keys_80  );

    //
    u32_t permutation( u32_t m );
    u32_t subsitution( u32_t m );
    u32_t encryption_24( u32_t * keys, u32_t m );

    //
    u32_t permutation_inv( u32_t m );
    u32_t subsitution_inv( u32_t m );
    u32_t decryption_24( u32_t * keys, u32_t m );

    
#endif