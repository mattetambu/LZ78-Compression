// Uncompressor_table.c

#include "Uncompressor_table.h"


uncompressor_table_t* uncompressor_table_create () {
	uncompressor_table_t* hash_table = (uncompressor_table_t*) calloc (MAX_CHILDS, sizeof (uncompressor_table_row_t));
	if (hash_table == NULL) {
		fprintf (stderr, "Can't allocate the memory for the compressor table\n");
		errno = ENOMEM;
	}
	
	return hash_table;
}

int uncompressor_table_initialize (uncompressor_table_t* hash_table) {
	int i = 0;
	for (; i < ROOT_INDEX; i++)
		if (uncompressor_table_insert (hash_table, ROOT_INDEX, (hvalue_t) i, (hindex_t) i) != 0) return -1;

	return 0;
}

int uncompressor_table_destroy (uncompressor_table_t* hash_table) {
	if (hash_table == NULL) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}
	
	memset (hash_table, 0, MAX_CHILDS * sizeof(uncompressor_table_row_t));
	free (hash_table);
	return 0;
}


int uncompressor_table_insert (uncompressor_table_t* hash_table, hindex_t parent_label, hvalue_t value, hindex_t label) {
	hash_table[label].parent = parent_label;
	hash_table[label].value = value;

	return 0;
}

hvalue_t uncompressor_table_lookup (uncompressor_table_t* hash_table, hindex_t label, FILE* file_descriptor) {
	if (hash_table[label].parent == ROOT_INDEX) {
		if (fputc (hash_table[label].value, file_descriptor) == -1) return -1;
		return hash_table[label].value;	
	}
	
	hvalue_t value = uncompressor_table_lookup (hash_table, hash_table[label].parent, file_descriptor);
	if (fputc (hash_table[label].value, file_descriptor) == -1) return -1;
	return value;
}
