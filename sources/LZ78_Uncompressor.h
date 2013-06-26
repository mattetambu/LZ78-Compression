// LZ78_Uncompressor.h

#ifndef LZ78_UNCOMPRESSOR_H
#define LZ78_UNCOMPRESSOR_H

	#include "Common.h"
	#include "Bit_buffer_io.h"
	#include "Uncompressor_table.h"
	
	struct _lz78_uncompressor_t {
		uncompressor_table_t* hash_table;
		bit_buffer_t* bit_buffer;
		FILE *file_descriptor;
		//char *input_file_name;
		//char *output_file_name;
	};

	typedef struct _lz78_uncompressor_t lz78_uncompressor_t;
	
	lz78_uncompressor_t* lz78_uncompressor_create ();
	int lz78_uncompressor_initialize (lz78_uncompressor_t*, char*, char*);
	int lz78_uncompressor_destroy (lz78_uncompressor_t*);
	
	int lz78_uncompressor_uncompress (lz78_uncompressor_t*);
	
#endif
