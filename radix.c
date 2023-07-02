#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

// Print an array
void printArray(uint8_t array[], size_t num_entries, uint8_t bytes ) 
{
    uint64_t value;

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

// counting sort with 256 radix and associated data
// dest_ptr output array
// src_ptr input array
// byte (0-7) which byte to radix sort on
// num the number of elements in the array
// size, the number of bytes of each element of the array
// get_value function to get the value from the array element
void count256asc( void * dest_ptr, void * src_ptr, uint8_t byte, size_t num, size_t size, uint64_t (* get_value)( void * input ) )
{
    uint16_t count[257];
    uint8_t shift = byte*8;
    uint64_t mask = (0xFFu<<shift);
    uint8_t * input = src_ptr;
    uint8_t * output = dest_ptr;

    memset( count, 0, sizeof(count) );
    // create count array
    for (int i = 0; i < num; i++)
    {
        count[ ((get_value( &input[ i*size ] ) & mask)>> shift) + 1]++;
    }

    // accumilate counts
    for (int i = 1; i < 256; i++)
    {
        count[ i ] += count[i-1u];
    }

    // Place the elements in sorted order
    for (int i = 0; i < num; i++) 
    {
        uint8_t count_index = ((get_value( &input[ i*size ] ) & mask)>> shift);
        uint8_t out_idx = count[ count_index ]++;
        memcpy( &output[ out_idx*size ], &input[ i* size ], size );
    }
}


// do a radix sort with associated data
// in_out_ptr pointer to the data to sort
// num  the number of elements in the array
// size the size in bytes of each element
// get_value  callback function to get a value from an array element
// return true on success, false on memeory allocation failure, can fallback to quicksort?
bool radix_sort_asc( void * in_out_ptr, size_t num, size_t size, uint64_t (* get_value)( void * input ) )
{
    uint8_t * space_ptr; 
    uint8_t * input_ptr = in_out_ptr;
    uint8_t * output_ptr;
    uint8_t * temp_ptr;
  
    printf("%s\n", __func__ );

    // allocate memory for the shuffle array
    space_ptr = calloc( num, size );
    if (!space_ptr)
    {
        return false;
    }
    output_ptr = space_ptr;


    // Apply counting sort to sort elements based on place value.
    // note we should probably find the largest element and sort up to that big
    for (uint8_t byte = 0; byte < sizeof(uint64_t); byte++)
    {
        count256asc( output_ptr, input_ptr, byte, num, size, get_value );
//        printArray( output_ptr, num, size );
        if ( byte <= sizeof(uint64_t)-1 )
        {
            // swap in and out pointers for next loop
            temp_ptr = input_ptr;
            input_ptr = output_ptr;
            output_ptr = temp_ptr;
        }
    }

    // make sure we output the data into the correct buffer
    if ( output_ptr != in_out_ptr )
    {
        memcpy( in_out_ptr, output_ptr, num*size );
    }

    // free memory
    free(space_ptr);

    return true;
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
}

