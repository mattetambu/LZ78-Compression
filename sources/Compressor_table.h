// Compressor_table.h

#ifndef COMPRESSOR_TABLE_H
#define COMPRESSOR_TABLE_H

	#include "Common.h"
	#include "Bit_buffer_io.h"
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	
	#define DOUBLE_HASHING	false	// Want to use Double hashing
	#define EXPANSION_FACTOR 4		// Expansion factor for the hash table 
	#define HASH_TABLE_ROWS	(EXPANSION_FACTOR * MAX_CHILDS)	// Row number in the hash table 

	struct _compressor_table_row_t {
		hindex_t parent;
		hvalue_t value;
		hindex_t label;
	};

	typedef struct _compressor_table_row_t compressor_table_row_t;
	typedef struct _compressor_table_row_t compressor_table_t;

	hindex_t inline compute_hash (hindex_t, hvalue_t);	// Knuth Multiplicative hashing
	hindex_t inline compute_step_hash (hindex_t, hvalue_t);	//Division Method
	
	compressor_table_t* compressor_table_create ();
	int compressor_table_initialize (compressor_table_t*);
	int compressor_table_destroy (compressor_table_t*);
	int compressor_table_rebuild (compressor_table_t*);
	
	int compressor_table_insert (compressor_table_t*, hindex_t, hvalue_t, hindex_t);
	hindex_t compressor_table_search (compressor_table_t*, hindex_t, hvalue_t);

#endif