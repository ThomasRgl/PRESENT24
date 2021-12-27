#include "../present.h"



u8_t subsitution_inv_8(u8_t m) {
    u8_t sbox[16] =
      {0x5,0xe,0xf,0x8,0xc,0x1,0x2,0xd,0xb,0x4,0x6,0x3,0x0,0x7,0x9,0xa};
    
    return (sbox[(m & 0xf0)>>4]<<4) | sbox[(m & 0x0f)];
}



u32_t decryption_24( u32_t * keys, u32_t m ){
    u32_t state = m;
    state = state ^ keys[ROUNDS];
    for( int i = ROUNDS - 1 ; i >= 0; i-- ) {
        
        // //manual permutation
        u32_t r = 0x120c0600; //magic number 
        u32_t t = 0;
        for(u8_t j = 0; j < 24; j++  ) {
            t |= ( ( state >> ( r & 255 ) ) & 1 ) <<  j ;
            r = ( (r + 1) >> 8 ) | ( (r + 1) << (32 - 8) );   
        }
        state = t;


        for(u8_t j = 0; j < 3; j++  )
            ((u8_t *)&state)[j] = subsitution_inv_8( ((u8_t *)&state)[j] );


        state = state ^ keys[i];
        
    }
    
    return state & 0xffffff;
}