// Compressor_table.c

#include "Compressor_table.h"

inline hindex_t compute_hash (hindex_t parent_label, hvalue_t value) {
	hindex_t key = (parent_label + 1) * (value + 1);
	
	return floor (HASH_TABLE_ROWS * (key * KNUTH_NUMBER - floor(key * KNUTH_NUMBER)));	//floor (HASH_TABLE_ROWS * ((key * KNUTH_NUMBER) % 1));
}

inline hindex_t compute_step_hash (hindex_t parent_label, hvalue_t value) {
	hindex_t key = (parent_label + 1) * (value + 1);
	
	return (key % (HASH_TABLE_ROWS - 1)) | 1; // | 1 because double hashing need the step is prime to HASH_TABLE_ROWS (par number)
}


compressor_table_t* compressor_table_create () {
	compressor_table_t* hash_table = (compressor_table_t*) calloc (HASH_TABLE_ROWS, sizeof (compressor_table_row_t));
	if (hash_table == NULL) {
		fprintf (stderr, "Can't allocate the memory for the compressor table\n");
		errno = ENOMEM;
	}
	
	return hash_table;
}

int compressor_table_initialize (compressor_table_t* hash_table) {
	int i = 0;
	for (; i < ROOT_INDEX; i++)
		if (compressor_table_insert (hash_table, ROOT_INDEX, (hvalue_t) i, (hindex_t) i) != 0) return -1;

	return 0;
}

int compressor_table_destroy (compressor_table_t* hash_table) {
	if (hash_table == NULL) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}
	
	memset (hash_table, 0, HASH_TABLE_ROWS * sizeof(compressor_table_row_t));
	free (hash_table);
	return 0;
}

int compressor_table_rebuild (compressor_table_t* hash_table) {
	memset (hash_table, 0, HASH_TABLE_ROWS * sizeof(compressor_table_row_t));
	
	return compressor_table_initialize (hash_table);
}


int compressor_table_insert (compressor_table_t* hash_table, hindex_t parent_label, hvalue_t value, hindex_t label) {
	hindex_t step = (DOUBLE_HASHING)? compute_step_hash (parent_label, value) : 1;
	hindex_t base_index = compute_hash (parent_label, value), index = base_index;	
	int i = 0;
	
	for (; hash_table[index].label != 0 || hash_table[index].parent == ROOT_INDEX; i++) {
		index = (base_index + step * i) % HASH_TABLE_ROWS;
	}
	
	hash_table[index].parent = parent_label;
	hash_table[index].value = value;
	hash_table[index].label = label;

	return 0;
}

hindex_t compressor_table_search (compressor_table_t* hash_table, hindex_t parent_label, hvalue_t value) {
	hindex_t step = (DOUBLE_HASHING)? compute_step_hash (parent_label, value) : 1;
	hindex_t base_index = compute_hash (parent_label, value), index = base_index;
	int i = 0;
	
	for (; hash_table[index].label != 0 || hash_table[index].parent == ROOT_INDEX; i++) {
		if (hash_table[index].value == value && hash_table[index].parent == parent_label) return hash_table[index].label;
		index = (base_index + step * i) % HASH_TABLE_ROWS;
	}
		
	return -1;
}
