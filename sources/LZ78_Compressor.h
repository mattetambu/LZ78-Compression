// LZ78_Compressor.h

#ifndef LZ78_COMPRESSOR_H_
#define LZ78_COMPRESSOR_H_

	#include "Common.h"
	#include "Bit_buffer_io.h"
	#include "Compressor_table.h"
	
	struct _lz78_compressor_t {
		compressor_table_t* hash_table;
		bit_buffer_t* bit_buffer;
		FILE *file_descriptor;
		int input_file_size;
		//char *input_file_name;
		//char *output_file_name;
	};

	typedef struct _lz78_compressor_t lz78_compressor_t;

	lz78_compressor_t* lz78_compressor_create ();
	int lz78_compressor_initialize (lz78_compressor_t*, char*, char*);
	int lz78_compressor_destroy (lz78_compressor_t*);
	
	int lz78_compressor_compress (lz78_compressor_t*);
	
#endif