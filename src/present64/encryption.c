#include "present.h"





u64_t permutation( u64_t m ){
        u64_t r = 0x0030002000100000;; //magic number 
        u64_t t = 0;
        for(u8_t j = 0; j < 64; j++  ) {
            t |= ( ( m >> j ) & 1 ) << ( r & 255 );
            r = ( (r + 1) >> 16 ) | ( (r + 1) << (64 - 16) );   
        }
        return t;

}



u64_t subsitution( u64_t m ){
    static const u8_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };

    u64_t m2 = 0;
    for(int i = 15; i >= 0; i-- ){
        m2 = m2 << 4 ;
        m2 = m2 | sbox[ 15 & ( m >> ( i * 4 ) ) ];
    }
    return m2;

}

u64_t encryption_64( u128_t key, u64_t m ){
    u64_t * keys;
    keys = (u64_t *) malloc( (ROUNDS + 1) * sizeof(u64_t) );
    keys = keyScheduling( key, keys );


    u64_t state = m;
    for( int i = 0; i < ROUNDS; i++ ) {
        state = state ^ keys[i];
        state = subsitution(state);
        state = permutation(state);   
    }
    state = state ^ keys[ROUNDS];
    free(keys);
    // return (state<<(128-80)>>128-80);
    return state;
}
