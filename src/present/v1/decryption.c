#include "../present.h"


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