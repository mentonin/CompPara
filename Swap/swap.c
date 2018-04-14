#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void swap(void* a, void* b, size_t size){
    void* temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

void main(){
    float a = 3.5;
    float b = 5.4;
    printf("a = %f; b = %f\n", a, b);
    swap(&a, &b, sizeof(a));
    printf("a = %f; b = %f\n", a, b);
}