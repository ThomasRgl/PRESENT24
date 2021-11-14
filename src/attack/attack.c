
#include "attack.h"

#define SIZE 0xffffff

void attack(u32_t m1, u32_t enc1, u32_t m2, u32_t enc2){
    
    // tabs
    u32_t * enc_keys = malloc( sizeof(u32_t) * (0xffffff + 0x1 ));
    u64_t * enc_msgs = malloc( sizeof(u64_t) * (0xffffff + 0x1 ));

    u32_t * dec_keys = malloc( sizeof(u32_t) * (0xffffff + 0x1 ));
    u64_t * dec_msgs = malloc( sizeof(u64_t) * (0xffffff + 0x1 ));
    
    //keys 
    u32_t * keys;
    keys = malloc( (ROUNDS + 1) * sizeof(u32_t) );

    u128_t * keys_80;
    keys_80 = malloc( (ROUNDS + 1) * sizeof(u128_t) );

    //
    for(u32_t key = 0x0; key <= 0xffffff; key ++ ){
        keys = keyScheduling( key, keys, keys_80 );
        //
        enc_msgs[key] = ( ((u64_t) encryption_24(keys, m1)) << 32) | ((u64_t) encryption_24(keys, m2)) ;
        // enc_msgs[key] =  ((u64_t) encryption_24(keys, m1)) ;

        enc_keys[key] = key;

        //
        dec_msgs[key] = ( ((u64_t) decryption_24(keys, enc1)) << 32) | ((u64_t) decryption_24(keys, enc2)) ;
        dec_keys[key] = key;
        // printf("%x %x\n", key, key & 0x00ffff);
        if((key & 0x00ffff) == 0x00ffff ){
            printf("%x\n", key);
        }
    }

    //
    sort(enc_msgs, enc_keys, 0xffffff + 0x1);
    sort(dec_msgs, dec_keys, 0xffffff + 0x1);
    printf("sorted ! \n");

    //
    for(u32_t i = 0x0; i <= 0xff; i ++ ){
        printf("%*llx - %*x\n", 10, enc_msgs[i], 10, enc_keys[i]  );
    }

    u64_t msg;
    for(u32_t i = 0x0; i <= 0xffffff; i ++ ){
        msg = enc_msgs[i];
        u32_t indice = dichotomie(dec_msgs, msg, 0xffffff);
        if(indice != 0){
            printf("k1: %x   k2: %x \n", enc_keys[i], enc_keys[indice - 1]);
        }
    }
    



    //
    free(enc_keys);
    free(enc_msgs);
    free(dec_keys);
    free(dec_msgs);
    free(keys);
    free(keys_80);
}


u32_t dichotomie(u64_t * tab, u32_t mot, u32_t size){
    int a = 0;
    int b = size;
    int m;

    while (a <= b){
        m = (a + b) / 2;
        if(tab[m] == mot){
            printf("OUI\n");
            return (u32_t) (m + 1);
            }
        else if (tab[m] < mot)
            a = m + 1;
        else
            b = m - 1;
        }
    return 0;
}
