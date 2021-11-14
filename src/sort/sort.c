#include "sort.h"

void sort(u64_t * msgs, u32_t * keys, u32_t nbele){
    if(nbele >= 0x7FFFFFFF){
        printf("ERROR"); //should never happend
        exit(-1);
        }

    quickSort(msgs, keys, 0, (int) nbele - 1);
}


void quickSort( u64_t * vect1, u32_t * vect2, int deb, int fin){
    if(deb < fin){
        int pivot = deb; // choix pivot
        pivot = partition(vect1, vect2, deb, fin, pivot);
        quickSort(vect1, vect2, deb, pivot - 1 );
        quickSort(vect1, vect2, pivot + 1 , fin );
    }
}




int  partition(u64_t * vect1, u32_t * vect2, int deb, int fin, int pivot ){
    u64_t tmp1 = vect1[pivot];
    vect1[pivot] = vect1[fin];
    vect1[fin] = tmp1;

    u32_t tmp2 = vect2[pivot];
    vect2[pivot] = vect2[fin];
    vect2[fin] = tmp2;


    int j = deb;
    for(int i = deb; i <= fin - 1; i++){
        if( vect1[i] <= vect1[fin]){
            tmp1 = vect1[i];
            vect1[i] = vect1[j];
            vect1[j] = tmp1;

            tmp2 = vect2[i];
            vect2[i] = vect2[j];
            vect2[j] = tmp2;

            j++;
        }
    }

    tmp1 = vect1[fin];
    vect1[fin] = vect1[j];
    vect1[j] = tmp1;

    tmp2 = vect2[fin];
    vect2[fin] = vect2[j];
    vect2[j] = tmp2;

    return j;

}