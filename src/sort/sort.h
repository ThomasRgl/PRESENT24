#ifndef SORT_H
    #define SORT_H

    #include <time.h>
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include "../present/present.h"

    void sort( u64_t * msgs, u32_t * keys, u32_t nbele );
    void quickSort( u64_t * vect1, u32_t * vect2, int deb, int fin);
    int partition(u64_t * vect1, u32_t * vect2, int deb, int fin, int pivot);

    

#endif