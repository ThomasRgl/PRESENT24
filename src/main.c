#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "present/present.h"


void  main(){
    u32_t key = 0x0;
    u32_t m = 0xffffff;
    u32_t * keys;
    keys = keyScheduling( key );
    u32_t enc = encryption_24(keys, m);
    u32_t dec = decryption_24(keys, enc);
    printf("m :   : %llx \n",  (u64_t) m );
    printf("enc :   : %llx \n",  (u64_t) enc );
    printf("dec :   : %llx \n",  (u64_t) dec );
    

}