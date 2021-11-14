#include "present.h"



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
        // printf("xor state %d : %llx \n", i+2, (u64_t) state );
        state = subsitution(state);
        // printf("sub state %d : %llx \n", i+2, (u64_t) state );
        state = permutation(state);
        // printf("per state %d : %llx \n", i+2, (u64_t) state );
        // printf("\n\n\n");
        
    }
    state = state ^ keys[ROUNDS];
    return state & 0xffffff;
}
