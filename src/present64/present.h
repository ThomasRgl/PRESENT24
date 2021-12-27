
#ifndef PRESENT_H
    #define PRESENT_H

    #include <time.h>
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>

    
    #define ROUNDS 32
    typedef unsigned __int128 u128_t ;
    typedef unsigned long long u64_t ;
    typedef unsigned int u32_t;
    typedef unsigned char u8_t;

    #define R_32 (v,n)( ((v) >> (n)) | ((v) << (32  -(n))) )
    #define R_64 (v,n)( ((v) >> (n)) | ((v) << (64  -(n))) )
    #define R_128(v,n)( ((v) >> (n)) | ((v) << (128 -(n))) )


    //
    u64_t * keyScheduling( u128_t key, u64_t * keys_64  );

    //
    u64_t permutation( u64_t m );
    u64_t subsitution( u64_t m );
    u64_t encryption_64( u128_t key, u64_t m );

    //
    u64_t permutation_inv( u64_t m );
    u64_t subsitution_inv( u64_t m );
    u64_t decryption_64( u128_t  key, u64_t m );

    
#endif