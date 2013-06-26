// Common.h

#ifndef _COMMON_H_
#define _COMMON_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <errno.h>
	#include <math.h>

	#include <sys/stat.h>
	#include <unistd.h>
	#include <fcntl.h>

	
	#define false	0
	#define true	1
	#define _READ	0
	#define _WRITE	1
		
	#define ROOT_INDEX	256
	#define MAX_CHILDS	1048576		// Max number of childs of the hash table - 2^20
	
	/*The hash multiplication method:
	 - Multiply the key by a constant A, 0 < A < 1,
	 - Extract the fractional part of the product,
	 - Multiply this value by m.
	 
	Thus the hash function is:
			h(k) = floor(m * (kA - floor(kA)))

	In this case, the value of m is not critical and we typically choose a power of 2 so that we can get the following efficient procedure on most digital computers:
	 - Choose m = 2p.
	 - Multiply the w bits of k by floor(A * 2w) to obtain a 2w bit product.
	 - Extract the p most significant bits of the lower half of this product.

	It seems that:
			A = (sqrt(5)-1)/2 = 0.6180339887 -> KNUTH_NUMBER
	is a good choice (see Knuth, "Sorting and Searching", v. 3 of "The Art of Computer Programming").*/
	#define KNUTH_NUMBER 0.6180339887	//(sqrt(5)-1)/2
	
	#define label_length(label) (ceil(log(label)/log(2)))	// Return a value between 9 (log2(ROOT_INDEX+1)) and 20 (log2(MAX_CHILDS))
	#define min(x1, x2) ((x1 < x2)? x1 : x2)

	
	typedef unsigned char uint8_t;
	typedef short unsigned int uint16_t;
	typedef unsigned int uint32_t;
	typedef unsigned long uint64_t;

	typedef uint32_t hindex_t;
	typedef uint8_t hvalue_t;

#endif
