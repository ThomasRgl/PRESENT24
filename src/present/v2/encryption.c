#include "../present.h"





u8_t subsitution_8(u8_t m) {
    u8_t sbox[16] =
      {0xc,0x5,0x6,0xb,0x9,0x0,0xa,0xd,
       0x3,0xe,0xf,0x8,0x4,0x7,0x1,0x2 };
    
    return (sbox[(m & 0xf0)>>4]<<4) | sbox[(m & 0x0f)];
}




u32_t encryption_24( u32_t * keys, u32_t m ){
    u32_t state = m;
    for( int i = 0; i < ROUNDS; i++ ) {
        
        state = state ^ keys[i];
        for(u8_t j = 0; j < 3; j++  )
             ((u8_t *)&state)[j] = subsitution_8( ((u8_t *)&state)[j] );
        // printf("sub %d : %llx \n", i, (u64_t) state );
        //manual permutation

        u32_t r = 0x120c0600; //magic number 
        u32_t t = 0;
        for(u8_t j = 0; j < 24; j++  ) {
            // printf("r %d : %d %d \n", i, j,  (r & 255) );
            t |= ( ( state >> j ) & 1 ) << ( r & 255 );
            r = ( (r + 1) >> 8 ) | ( (r + 1) << (32 - 8) );   
        }
        state = t;
    }
    
    state = state ^ keys[ROUNDS];
    
    return state & 0xffffff;
}

