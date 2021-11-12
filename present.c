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


// typedef struct Key{
//     u_int16_t k1; //poid fort
//     u_int64_t k2; //poid faible
// }Key;

// void generate_round_keys(Key * k ){
//     Key ** keys ;
//     k = malloc(32 * sizeof(Key));
//     keys[0] = k; //k1 = k

//     Key * ti;
//     for(size_t i = 1; i <= 31; i++){
//         // ti = keys[i-1][19:80] + keys[i-1][0:19]
//         // keys[i] = ti[0:15] + (ti[16:20] ^ i) + ti[21:75] + S(ti[76:80])
//     }

// }

// u_int64_t enc_64bits( u_int64_t bloc, Key * k ){
//     // k =
// }


u32_t * keyScheduling( u32_t key ){

    static const u32_t pbox[24] = {
        0,6,12,18,1,7,13,19,2,8,14,20,3,9,15,21,4,10,16,22,5,11,17,23
    };
    
    static const u32_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };


    u32_t * keys_24;
    keys_24 = malloc( (ROUNDS + 1) * sizeof(u32_t) );

    u128_t * keys_80;
    keys_80 = malloc( (ROUNDS + 1) * sizeof(u128_t) );

    u128_t cache = ((( ~((u128_t) 0)) << 48 ) >> 48) ;
    // u128_t cache = (u128_t) 10000000 ;
    u128_t mainKey = ((u128_t) key) << 56 ;
    u128_t K = mainKey;
    keys_80[ 0 ] = K;
    keys_24[ 0 ] = (K >> 16) & 0xffffff;
    for(int i = 1; i < ROUNDS + 1; i++){
        K =  ( K << 61 | K >> 19 ) & cache;
        printf("K : %llx %llx \n", (u64_t) (K >> 64), (u64_t) K);
        printf("K : %llx %llx \n", (u64_t) sbox[ K >> 76 ], (u64_t) sbox[ K >> 76 ]);
        K = ( (((u128_t) sbox[ K >> 76 ]) << 76 ) | ( K & ~((u128_t) 0b1111 << 76) ) ) & cache;
        printf("K : %llx %llx \n", (u64_t) (K >> 64), (u64_t) K);
        K = (( (((K >> 15) & 31 ) ^ (i) ) << 15 ) | ( K & ~((u128_t)0b11111000000000000000) ) ) & cache;;
        printf("K : %llx %llx \n", (u64_t) (K >> 64), (u64_t) K);
        printf("\n\n");
        keys_80[ i ] = K;
        keys_24[ i ] = (K >> 16) & 0xffffff;
    }

    printf("cache : %llx %llx \n", (u64_t) (cache >> 64), (u64_t) cache);
    printf("mainKey : %llx %llx \n", (u64_t) (mainKey >> 64), (u64_t) mainKey);
    for(int i = 0; i < ROUNDS + 1; i++){
        printf("key %d : %llx %llx\n", i, (u64_t) (keys_80[i] >> 64), (u64_t) keys_80[i]);
    }

    printf("\n\n\n\n");
    for(int i = 0; i < ROUNDS + 1; i++){
        printf("key %d : %llx \n", i, (u64_t) (keys_24[i]));
    }

    
    return keys_24;
}

//https://stackoverflow.com/questions/23073978/bit-permutation-tables-in-c

u32_t permutation( u32_t m ){
    // static const u32_t pbox[24] = {
    //     0,6,12,18,1,7,13,19,2,8,14,20,3,9,15,21,4,10,16,22,5,11,17,23
    // };
    static const u32_t pbox[24] = {
        0,4,8,12,16,20,1,5,9,13,17,21,2,6,10,14,18,22,3,7,11,15,19,23
    };
    // printf("m  : %llx \n",  (u64_t) m );
    u32_t result = 0;
    for ( int i = 23; i >= 0; i-- )
    { 
        
        result <<= 1;
        if ( m & (1 << pbox[i]) )
            result |= 1;
            
        // printf("sub  : %llx \n",  (u64_t) result );
    }
    return result;

}

u32_t subsitution( u32_t m ){
    static const u32_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };

    u32_t m2 = 0;
    // printf("sub  : %llx \n",  (u64_t) m2 );
    for(int i = 5; i >= 0; i-- ){
        m2 = m2 << 4 ;
        // printf("aa  : %llx \n",  (u64_t) m2);
        m2 = m2 | sbox[ 15 & ( m >> ( i * 4 ) ) ];
        // printf("aa  : %llx \n",  (u64_t) m2 );
    }
    // printf("sub  : %llx \n",  (u64_t) m2 );
    return m2;

}


u32_t encryption_24( u32_t * keys, u32_t m ){
    u32_t state = m;
    for( int i = 0; i < ROUNDS; i++ ) {
        state = state ^ keys[i];
        printf("xor state %d : %llx \n", i+2, (u64_t) state );
        state = subsitution(state);
        printf("sub state %d : %llx \n", i+2, (u64_t) state );
        state = permutation(state);
        printf("per state %d : %llx \n", i+2, (u64_t) state );
        printf("\n\n\n");
        
    }
    state = state ^ keys[ROUNDS];
    return state & 0xffffff;
}


////////////////////////////////////////////////////////////////////////

u32_t permutation_inv( u32_t m ){
    static const u32_t xbop[24] = {
        0,6,12,18,1,7,13,19,2,8,14,20,3,9,15,21,4,10,16,22,5,11,17,23
    };
    // static const u32_t pbox[24] = {
    //     0,4,8,12,16,20,1,5,9,13,17,21,2,6,10,14,18,22,3,7,11,15,19,23
    // };
    // printf("m  : %llx \n",  (u64_t) m );
    u32_t result = 0;
    for ( int i = 23; i >= 0; i-- )
    { 
        
        result <<= 1;
        if ( m & (1 << xbop[i]) )
            result |= 1;
            
        // printf("sub  : %llx \n",  (u64_t) result );
    }
    return result;

}

u32_t subsitution_inv( u32_t m ){
    // static const u32_t xbos[16] = {
    //     12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    // };
    static const u32_t xbos[16] = {
        0x5,0xe,0xf,0x8,0xc,0x1,0x2,0xd,0xb,0x4,0x6,0x3,0x0,0x7,0x9,0xa
    };

    u32_t m2 = 0;
    // printf("sub  : %llx \n",  (u64_t) m2 );
    for(int i = 5; i >= 0; i-- ){
        m2 = m2 << 4 ;
        // printf("aa  : %llx \n",  (u64_t) m2);
        m2 = m2 | xbos[ 15 & ( m >> ( i * 4 ) ) ];
        // printf("aa  : %llx \n",  (u64_t) m2 );
    }
    // printf("sub  : %llx \n",  (u64_t) m2 );
    return m2;

}

u32_t decryption_24( u32_t * keys, u32_t m ){
    u32_t state = m;
    printf(" state  : %llx \n", (u64_t) state );
    state = state ^ keys[ROUNDS];
    printf(" state  : %llx \n", (u64_t) state );
    for( int i = ROUNDS - 1 ; i >= 0; i-- ) {
        state = permutation_inv(state);
        state = subsitution_inv(state);
        state = state ^ keys[i];

        // printf("\n\n\n");
        
    }
    
    return state & 0xffffff;
}

/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

void  main(){
    u32_t key = 0x0;
    u32_t m = 0xffffff;
    u32_t * keys;
    keys = keyScheduling( key );
    u32_t enc = encryption_24(keys, m);
    u32_t dec = decryption_24(keys, enc);
    printf("m :   : %llx \n",  (u64_t) m );
    printf("enc :   : %llx \n",  (u64_t) enc );
    printf("dec :   : %llx \n",  (u64_t) dec );
    

}









// k0 =
// 1111111111
// 111111
// k1 =  0000
// 0000000000
// 0000000000
// 0000000000
// 0000000000
// 0000000000
// 0000000000
// 0000000000

// k1_ = k2 & 0xff;
// k2_ = k2 << 16;
// k2_3 = k2_ & 0x8;
// k2_ = k2 << 3;
// k2_ = k2_ + k0 + k2_3
