
#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>

#include <inttypes.h>
#include <time.h>
#include "radix.h"

// George Marsaglia's XOR shift random generator from wikipedia

unsigned int state = 123;
unsigned int xorshift32( void )
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void GenerateRandomData(unsigned int * arr, int count, int seed )
{
    state = seed;
    for (int i=0; i < count; i++)
    {
        arr[i] = xorshift32();
    }
}


// Print an array
void printArray(void * input, size_t num_entries, uint8_t bytes ) 
{
    uint64_t value;
    uint8_t * array = input;

    for (int i = 0; i < num_entries; i++) 
    {
        value = 0;
        for (uint8_t j=0; j<bytes; j++)
        {
            value += (((uint64_t)array[(i*bytes) + j])<<8*j );
        }
        printf("%" PRIu64 ", ", value);
    }
    printf("\n");
}

uint64_t get_value1( void * data_ptr )
{
    return *(uint16_t *)data_ptr;
}

uint64_t get_value2( void * data_ptr )
{
    return *(uint32_t *)data_ptr;
}

uint64_t get_value3( void * data_ptr )
{
    return *(uint64_t *)data_ptr;
}

// Driver code
int main() 
{
    uint16_t array[] = {121, 432, 564, 23, 1, 45, 788, 1, 2, 3,4,5,8,13};
    int n = sizeof(array) / sizeof(array[0]);
    radix_sort_asc(array, n, sizeof(array[0]), get_value1);
    printArray(array, n, sizeof(array[0]));

    uint32_t array2[] = { 399999999, 4, 92, 65535, 4000000, 26000};
    n = sizeof(array2)/sizeof(array2[0]);
    radix_sort_asc(array2, n, sizeof(array2[0]), get_value2);
    printArray(array2, n, sizeof(array2[0]));

    uint64_t array3[] = { 57, 3000000221, 3751002, 4, 27, 3283 };
    n = sizeof(array3)/sizeof(array3[0]);
    radix_sort_asc(array3, n, sizeof(array3[0]), get_value3);
    printArray(array3, n, sizeof(array3[0]));



    // time sorting a big list
    long int count = 100000000;
    unsigned int * arr = calloc( count, 4u );

    long startTime = clock();
    GenerateRandomData( arr, count, 123);
    radix_sort_asc( arr, count, sizeof(unsigned int), get_value2 );
    long finishTime = clock();

    printf( "time = %lu\n", finishTime - startTime );

}