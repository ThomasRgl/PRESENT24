#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "present.h"

int main(int argc, char const *argv[])
{   
    u128_t key = 0x1234567890;
    u64_t m  =  0x0987654321;
    u64_t enc, dec;
    enc = encryption_64( key, m );
    printf("\n\n");
    dec = decryption_64( key, enc );

    printf("enc :  %llx\n", m);
    printf("enc :  %llx\n", enc);
    printf("dec :  %llx\n", dec);
    return 0;
}
