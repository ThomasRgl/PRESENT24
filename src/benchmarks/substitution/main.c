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


u8_t subsitution_8(u8_t m) {
    u8_t sbox[16] =
      {0xc,0x5,0x6,0xb,0x9,0x0,0xa,0xd,
       0x3,0xe,0xf,0x8,0x4,0x7,0x1,0x2 };
    
    return (sbox[(m & 0xf0)>>4]<<4) | sbox[(m & 0x0f)];
}

u32_t subsitution( u32_t m ){
    static const u32_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };

    u32_t m2 = 0;
    for(int i = 5; i >= 0; i-- ){
        m2 = m2 << 4 ;
        m2 = m2 | sbox[ 15 & ( m >> ( i * 4 ) ) ];
    }
    return m2;

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


    //test subsitution_8
    t0 = clock();
    for(u32_t m = 0x0; m < 0xffffff; m++){
        for(u8_t j = 0; j < 3; j++  )
            ((u8_t *)&res)[j] = subsitution_8( ((u8_t *)&m)[j] );
        
        trash = trash^res;

    }
    t1 = clock();
    dt = t1 - t0;
    fprintf(fp, "1, %*d,\n", 0, (int) dt );
    printf( "subsitution_8, , %d,\n", (int) dt );
    printf( " %d,\n", (int) trash );

    //test subsitution_32
    t0 = clock();
    trash = 0x0;
    for(u32_t m = 0x0; m < 0xffffff; m++){
        res = subsitution(m);
        trash = trash^res;
    }
    t1 = clock();
    dt = t1 - t0;
    fprintf(fp, "3, %*d,\n", 0, (int) dt );
    printf( "subsitution_24, %d,\n", (int) dt );
    printf( " %d,\n", (int) trash );

    printf("\n");

    fclose(fp);

    return 0;
}
