

#ifndef _RADIX__H
#define _RADIX__H

#include <stddef.h>
#include <stdint.h>

bool radix_sort_asc( void * in_out_ptr, size_t num, size_t size, uint64_t (* get_value)( void * input ) );

#endif