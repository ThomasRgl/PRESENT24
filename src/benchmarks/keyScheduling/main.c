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


u32_t * keySchedulingOpt( u32_t key, u32_t * keys_24 ){

    static const u8_t sbox[16] = {
        0x0c,0x05,0x06,0x0b,0x09,0x00,0x0a,0x0d,0x03,0x0e,0x0f,0x08,0x04,0x07,0x01,0x02
    };

    
    u128_t cache = ((( ~((u128_t) 0)) << 48 ) >> 48) ;
    // u128_t cache = (u128_t) 10000000 ;
    u128_t mainKey = ((u128_t) key) << 56 ;
    u128_t K = mainKey;

    keys_24[ 0 ] = (K >> 16) & 0xffffff;
    
    for(int i = 1; i < 11; i++){
        K =  ( K << 61 | K >> 19 )  & cache;
        ((u8_t *)&K)[9] = (sbox[ ((u8_t *)&K)[9] >> 4] << 4) |  ( 0x0f & ((u8_t *)&K)[9] ) ; 
        // K = ( (((u128_t) sbox[ K >> 76 ]) << 76 ) | ( K & ~((u128_t) 0b1111 << 76) ) ) & cache;
        K = (((u128_t)(((K >> 15) ^ ((u128_t)i) ) << 15 )) | ( K & ((u128_t)0b00000111111111111111) ))  & cache;
        //
        keys_24[ i ] = (K >> 16) & 0xffffff;
    }


    return keys_24;
}


u32_t * keyScheduling( u32_t key, u32_t * keys_24 ){

    static const u32_t pbox[24] = {
        0,6,12,18,1,7,13,19,2,8,14,20,3,9,15,21,4,10,16,22,5,11,17,23
    };
    
    static const u32_t sbox[16] = {
        12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2
    };



    u128_t cache = ((( ~((u128_t) 0)) << 48 ) >> 48) ;
    u128_t mainKey = ((u128_t) key) << 56 ;
    u128_t K = mainKey;
    keys_24[ 0 ] = (K >> 16) & 0xffffff;
    for(int i = 1; i < 11; i++){
        K =  ( K << 61 | K >> 19 ) & cache;
        K = ( (((u128_t) sbox[ K >> 76 ]) << 76 ) | ( K & ~((u128_t) 0b1111 << 76) ) ) ;
        K = (( (((K >> 15) & 31 ) ^ (i) ) << 15 ) | ( K & ~((u128_t)0b11111000000000000000) ) ) & cache;;

   
        keys_24[ i ] = (K >> 16) & 0xffffff;
    }

    return keys_24;
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
    u32_t res = 0;

    u32_t * keys;
    keys = malloc( (11) * sizeof(u32_t) );

    //test basique
    t0 = clock();
    for(u32_t m = 0x0; m < 0xffffff; m++){
        keys = keyScheduling(m, keys);
        trash = trash^keys[10];

    }
    t1 = clock();
    dt = t1 - t0;
    fprintf(fp, "1, %*d,\n", 0, (int) dt );
    printf( "keyScheduling,  %d,\n", (int) dt );
    printf( " %d,\n", (int) trash );

    //test pboxPermutation
    t0 = clock();
    trash = 0x0;
    for(u32_t m = 0x0; m < 0xffffff; m++){
        keys = keySchedulingOpt(m, keys);
        trash = trash^keys[10];
    }
    t1 = clock();
    dt = t1 - t0;
    fprintf(fp, "3, %*d,\n", 0, (int) dt );
    printf( "keyScheduling, %d,\n", (int) dt );
    printf( " %d,\n", (int) trash );

    printf("\n");

    fclose(fp);

    return 0;
}
