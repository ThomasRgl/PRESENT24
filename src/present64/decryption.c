#include "present.h"





u64_t permutation_inv( u64_t m ){
        u64_t r = 0x0030002000100000; //magic number 
        u64_t t = 0;
        for(u8_t j = 0; j < 64; j++  ) {
            t |= ( ( m >> ( r & 255 ) ) & 1 ) <<  j ;
            r = ( (r + 1) >> 16 ) | ( (r + 1) << (64 - 16) ); 
        }
        return t;

}



u64_t subsitution_inv( u64_t m ){
    static const u8_t xbos[16] = {
        0x5,0xe,0xf,0x8,0xc,0x1,0x2,0xd,0xb,0x4,0x6,0x3,0x0,0x7,0x9,0xa
    };

    u64_t m2 = 0;
    for(int i = 15; i >= 0; i-- ){
        m2 = m2 << 4 ;
        m2 = m2 | xbos[ 15 & ( m >> ( i * 4 ) ) ];
    }
    return m2;

}

u64_t decryption_64( u128_t key, u64_t m ){
    u64_t * keys;
    keys = (u64_t *) malloc( (ROUNDS + 1) * sizeof(u64_t) );

    keys = keyScheduling( key, keys );
    u64_t state = m;
    state = state ^ keys[ROUNDS];
    for( int i = ROUNDS - 1 ; i >= 0; i-- ) {
        state = permutation_inv(state);
        state = subsitution_inv(state);
        state = state ^ keys[i];
    }

    free(keys);
    // return (state<<(128-80)>>128-80);
    return state;
}