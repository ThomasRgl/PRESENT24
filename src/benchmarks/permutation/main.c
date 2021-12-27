#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef unsigned __int128 u128_t ;
typedef unsigned long long u64_t ;
typedef unsigned int u32_t;
typedef unsigned char u8_t;


u32_t pboxPermutation( u32_t m ){
    static const u32_t pbox[24] = {
        0,4,8,12,16,20,1,5,9,13,17,21,2,6,10,14,18,22,3,7,11,15,19,23
    };

    u32_t result = 0;
    for ( int i = 23; i >= 0; i-- )
    { 
        result <<= 1;
        if ( m & (1 << pbox[i]) )
            result |= 1;        
    }
    return result;

}

u32_t magicPermutation( u32_t m ){
        u32_t r = 0x120c0600; //magic number 
        u32_t t = 0;
        for(u8_t j = 0; j < 24; j++  ) {
            // printf("r %d : %d %d \n", i, j,  (r & 255) );
            t |= ( ( m >> j ) & 1 ) << ( r & 255 );
            r = ( (r + 1) >> 8 ) | ( (r + 1) << (32 - 8) );   
        }
        return t;

}


int main(int argc, char const *argv[])
{
    if(argc < 2){
        printf("ERROR - FICHIER MANQUANT\n");
        return 0 ;
    }

    char const *fileName = argv[1];
    FILE *fp;
    fp = fopen(fileName, "w");


    clock_t t0, t1, dt, t_null;
    t0 = clock();
    t_null = t0 - t0;


    u32_t trash = 0x0;
    u32_t res = 0x0;


    //test magicPermutation
    t0 = clock();
    for(u32_t m = 0x0; m < 0xffffff; m++){
        res = magicPermutation( m );
        trash = trash^res;

    }
    t1 = clock();
    dt = t1 - t0;
    fprintf(fp, "1, %*d,\n", 0, (int) dt );
    printf( "magicPermutation, , %d,\n", (int) dt );
    printf( " %d,\n", (int) trash );

    //test pboxPermutation
    t0 = clock();
    trash = 0x0;
    for(u32_t m = 0x0; m < 0xffffff; m++){
        res = pboxPermutation(m);
        trash = trash^res;
    }
    t1 = clock();
    dt = t1 - t0;
    fprintf(fp, "3, %*d,\n", 0, (int) dt );
    printf( "pboxPermutation, %d,\n", (int) dt );
    printf( " %d,\n", (int) trash );

    printf("\n");

    fclose(fp);

    return 0;
}
