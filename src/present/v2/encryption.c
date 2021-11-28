#include "../present.h"



// u32_t permutation( u32_t m ){
//     static const u32_t pbox[24] = {
//         0,4,8,12,16,20,1,5,9,13,17,21,2,6,10,14,18,22,3,7,11,15,19,23
//     };

//     u32_t result = 0;
//     for ( int i = 23; i >= 0; i-- )
//     { 
//         result <<= 1;
//         if ( m & (1 << pbox[i]) )
//             result |= 1;        
//     }
//     return result;

// }



u8_t subsitution_8(u8_t m) {
    u8_t sbox[16] =
      {0xc,0x5,0x6,0xb,0x9,0x0,0xa,0xd,
       0x3,0xe,0xf,0x8,0x4,0x7,0x1,0x2 };
    
    return (sbox[(m & 0xf0)>>4]<<4) | sbox[(m & 0x0f)];
}

// u32_t subsitution( u32_t m ){
//     static const u32_t sbox[16] = {
//         12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
//     };

//     u32_t m2 = 0;
//     for(int i = 5; i >= 0; i-- ){
//         m2 = m2 << 4 ;
//         m2 = m2 | sbox[ 15 & ( m >> ( i * 4 ) ) ];
//     }
//     return m2;

// }


u32_t encryption_24( u32_t * keys, u32_t m ){
    u32_t state = m;
    for( int i = 0; i < ROUNDS; i++ ) {
        
        // printf("start %d : %llx \n", i, (u64_t) state );
        state = state ^ keys[i];
        // printf("xor %d : %llx \n", i, (u64_t) state );
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
        // printf("perm %d : %llx \n", i, (u64_t) state );
    }
    
    state = state ^ keys[ROUNDS];
    
    return state & 0xffffff;
}

