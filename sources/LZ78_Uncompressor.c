// LZ78_Uncompressor.c

#include "LZ78_Uncompressor.h"

lz78_uncompressor_t* lz78_uncompressor_create () {
	lz78_uncompressor_t *uncompressor = (lz78_uncompressor_t*) calloc (1, sizeof (lz78_uncompressor_t));
	if (uncompressor == NULL) {
		fprintf (stderr, "Can't allocate the memory for the uncompressor\n");
		errno = ENOMEM;
		return NULL;
	}
		
	if ((uncompressor->hash_table = uncompressor_table_create ()) == NULL) {
		fprintf (stderr, "Can't create the hash table\n");
		memset (uncompressor, 0, sizeof (lz78_uncompressor_t));
		free (uncompressor);
		return NULL;
	}
	
	if ((uncompressor->bit_buffer = bit_buffer_create ()) == NULL){
		fprintf (stderr, "Can't create the structure to manage the input file\n");
		uncompressor_table_destroy (uncompressor->hash_table);
		memset (uncompressor, 0, sizeof (lz78_uncompressor_t));
		free (uncompressor);
		return NULL;
	}
	
	return uncompressor;
}

int lz78_uncompressor_initialize (lz78_uncompressor_t *uncompressor, char *input_file_name, char *output_file_name) {
	if (input_file_name == NULL || input_file_name[0] == '\0' ||
		output_file_name == NULL || output_file_name[0] == '\0' ||
		uncompressor == NULL) {
			fprintf (stderr, "Invalid parameter - ");
			errno = EINVAL;
			return -1;
	}
	
	if ((uncompressor->file_descriptor = file_open (output_file_name, "w")) == NULL) {
		fprintf (stderr, "Can't open the output file %s\n", output_file_name);
		return -1;	
	}
	//uncompressor->output_file_name = strdup (output_file_name);
	
	if (uncompressor_table_initialize (uncompressor->hash_table) == -1) {
		fprintf (stderr, "Can't inizialize the hash table\n");
		return -1;
	}
	
	if ((bit_buffer_initialize (uncompressor->bit_buffer, input_file_name, _READ)) == -1 ||
		bit_buffer_fill (uncompressor->bit_buffer) == -1) {
			fprintf (stderr, "Can't initialize the structure to manage the input file\n");
			return -1;
	}
	//uncompressor->input_file_name = strdup (input_file_name);
	
	return 0;
}

int lz78_uncompressor_destroy (lz78_uncompressor_t *uncompressor) {
	if (uncompressor == NULL) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}
	
	if (uncompressor->file_descriptor != NULL) fclose (uncompressor->file_descriptor);
	if (uncompressor->hash_table != NULL) uncompressor_table_destroy (uncompressor->hash_table);
	if (uncompressor->bit_buffer != NULL) bit_buffer_destroy (uncompressor->bit_buffer);
	//if (uncompressor->input_file_name != NULL) free (uncompressor->input_file_name);
	//if (uncompressor->output_file_name != NULL) free (uncompressor->output_file_name);
	
	memset (uncompressor, 0, sizeof (lz78_uncompressor_t));
	free (uncompressor);
	
	return 0;
}


int lz78_uncompressor_uncompress (lz78_uncompressor_t *uncompressor) {
	hindex_t previous_node_label = -1, next_label = ROOT_INDEX + 1;
	hindex_t current_node_label;	// read label
	hvalue_t current_value;			// character to insert
	int bits_to_read;
	
	for (; 1; previous_node_label = current_node_label) {	
		bits_to_read = (next_label == MAX_CHILDS - 1)? 9 /*label_length(ROOT_INDEX + 1)*/ : label_length (next_label);
		if (bit_buffer_read (uncompressor->bit_buffer, &current_node_label, bits_to_read) < bits_to_read) {
			fprintf(stderr, "Error reading the input file\n");
			return -1;
		}
		if (current_node_label == ROOT_INDEX) break; // EOF
	
		// emit code
		if (next_label == current_node_label) {
			current_value = uncompressor_table_lookup (uncompressor->hash_table, previous_node_label, uncompressor->file_descriptor);
			if (current_value == -1 || fputc (current_value, uncompressor->file_descriptor) == -1) return -1;
		}
		else {
			current_value = uncompressor_table_lookup (uncompressor->hash_table, current_node_label, uncompressor->file_descriptor);
			if (current_value == -1) return -1;
		}
		
		if (getenv("VERBOSE")) printf ("'%c'\tLabel:%d\n", current_value, current_node_label);
		
		// hash table update
		if (previous_node_label == -1) continue; // first simbol read (no previous node)
		uncompressor_table_insert (uncompressor->hash_table, previous_node_label, current_value, next_label);			
		
		if (++next_label == MAX_CHILDS) { // rebuild hash table
			next_label = ROOT_INDEX + 1;
			current_node_label = -1;
		}
	}
	if (getenv("VERBOSE")) printf ("'EOF'\tLabel:%d\n\n", current_node_label);
	
	return 0;
}
