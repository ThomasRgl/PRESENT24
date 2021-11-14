#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sort/sort.h"
#include "present/present.h"
#include "attack/attack.h"

void  main(){
    // u32_t key = 0x0;
    // u32_t m = 0xffffff;
    // u32_t * keys;
    // keys = keyScheduling( key );
    // u32_t enc = encryption_24(keys, m);
    // u32_t dec = decryption_24(keys, enc);
    // // printf("m :   : %llx \n",  (u64_t) m );
    // printf("enc :   : %llx \n",  (u64_t) enc );
    // printf("dec :   : %lx \n",  sizeof(u64_t) );
    // u64_t test =  0xffffffee;
    // test = test << 32;
    // test = test | 0x12345678;
    // printf("dec :   : %llx \n", test  );

    attack(0xef333a, 0x250324, 0xfb432f, 0xa71982);
    

}