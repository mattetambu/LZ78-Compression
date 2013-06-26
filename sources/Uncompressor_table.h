// Uncompressor_table.h

#ifndef UNCOMPRESSOR_TABLE_H
#define UNCOMPRESSOR_TABLE_H

	#include "Common.h"
	#include "Bit_buffer_io.h"
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	
	struct _uncompressor_table_row_t {
		hindex_t parent;
		hvalue_t value;
	};

	typedef struct _uncompressor_table_row_t uncompressor_table_row_t;
	typedef struct _uncompressor_table_row_t uncompressor_table_t;

	
	uncompressor_table_t* uncompressor_table_create ();
	int uncompressor_table_initialize (uncompressor_table_t*);
	int uncompressor_table_destroy (uncompressor_table_t*);
	
	int uncompressor_table_insert (uncompressor_table_t*, hindex_t, hvalue_t, hindex_t);
	hvalue_t uncompressor_table_lookup (uncompressor_table_t*, hindex_t, FILE*);

#endif