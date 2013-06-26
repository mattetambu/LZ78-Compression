//LZ78_Compressor.c

#include "LZ78_Compressor.h"

lz78_compressor_t* lz78_compressor_create () {
	lz78_compressor_t *compressor = (lz78_compressor_t*) calloc (1, sizeof (lz78_compressor_t));
	if (compressor == NULL) {
		fprintf (stderr, "Can't allocate the memory for the compressor\n");
		errno = ENOMEM;
		return NULL;
	}
	
	if ((compressor->hash_table = compressor_table_create ()) == NULL) {
		fprintf (stderr, "Can't create the hash table\n");
		memset (compressor, 0, sizeof (lz78_compressor_t));
		free (compressor);
		return NULL;
	}
	
	if ((compressor->bit_buffer = bit_buffer_create ()) == NULL){
		fprintf (stderr, "Can't create the structure to manage the output file\n");
		compressor_table_destroy (compressor->hash_table);
		memset (compressor, 0, sizeof (lz78_compressor_t));
		free (compressor);
		return NULL;
	}
	
	return compressor;
}

int lz78_compressor_initialize (lz78_compressor_t *compressor, char *input_file_name, char *output_file_name) {
	if (input_file_name == NULL || input_file_name[0] == '\0' ||
		output_file_name == NULL || output_file_name[0] == '\0' ||
		compressor == NULL) {
			fprintf (stderr, "Invalid parameter - ");
			errno = EINVAL;
			return -1;
	}
	
	if ((compressor->file_descriptor = file_open (input_file_name, "r")) == NULL) {
		fprintf (stderr, "Can't open the input file %s\n", input_file_name);
		return -1;	
	}
	//compressor->input_file_name = strdup (input_file_name);
	
	struct stat file_stat;
	if (stat (input_file_name, &file_stat) == -1) {
		fprintf (stderr, "Can't access the input file %s\n", input_file_name);
		return -1;
	}
	compressor->input_file_size = file_stat.st_size;
	
	if (compressor_table_initialize (compressor->hash_table) == -1) {
		fprintf (stderr, "Can't inizialize the hash table\n");
		return -1;
	}
	
	if (bit_buffer_initialize (compressor->bit_buffer, output_file_name, _WRITE) == -1){
		fprintf (stderr, "Can't initialize the structure to manage the output file\n");
		return -1;
	}
	//compressor->output_file_name = strdup (output_file_name);
	
	return 0;
}


int lz78_compressor_destroy (lz78_compressor_t *compressor) {
	if (compressor == NULL) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}
	
	if (compressor->file_descriptor != NULL) fclose (compressor->file_descriptor);
	if (compressor->hash_table != NULL) compressor_table_destroy (compressor->hash_table);
	if (compressor->bit_buffer != NULL) bit_buffer_destroy (compressor->bit_buffer);
	//if (compressor->input_file_name != NULL) free (compressor->input_file_name);
	//if (compressor->output_file_name != NULL) free (compressor->output_file_name);
	
	memset (compressor, 0, sizeof (lz78_compressor_t));
	free (compressor);
	
	return 0;
}


int lz78_compressor_compress (lz78_compressor_t *compressor) {
	hindex_t current_node_label, parent_node_label = ROOT_INDEX;
	hindex_t next_label = ROOT_INDEX + 1;
	hvalue_t current_value = 0;	// read character
	int bits_to_write, i = 0;
	
	for (; i < compressor->input_file_size; i++) {
		if ((current_value = fgetc (compressor->file_descriptor)) == -1) {
			fprintf (stderr, "Error reading the input file\n");
			return -1;
		}
		if (getenv("VERBOSE")) printf ("'%c'\tLabel:%d ", current_value, parent_node_label);
		
		if ((current_node_label = compressor_table_search (compressor->hash_table, parent_node_label, current_value)) == -1) { // no match
			bits_to_write = label_length (next_label);
			if (getenv("VERBOSE")) printf ("\tLabel_length:%d\n", bits_to_write);
			if (bit_buffer_write (compressor->bit_buffer, parent_node_label, bits_to_write) < bits_to_write) {	// emit code
				fprintf (stderr, "Error writing the output file\n");
				return -1;
			}
			
			compressor_table_insert (compressor->hash_table, parent_node_label, current_value, next_label);	// hash table update
			current_node_label = compressor_table_search (compressor->hash_table, ROOT_INDEX, current_value);
			
			if (++next_label == MAX_CHILDS) { // rebuild hash
				compressor_table_rebuild (compressor->hash_table);
				next_label = ROOT_INDEX + 1;
			}
		}
		else if (getenv("VERBOSE")) printf ("\n");
		
		parent_node_label = current_node_label;
	}
	
	bits_to_write = label_length (next_label);
	if (getenv("VERBOSE")) printf ("'%c'\tLabel:%d \tLabel_length:%d\n", current_value, parent_node_label, bits_to_write);
	if (bit_buffer_write (compressor->bit_buffer, parent_node_label, bits_to_write) < bits_to_write) {	// emit last code
			fprintf (stderr, "Error writing the output file\n");
			return -1;
	}
	if (getenv("VERBOSE")) printf ("'EOF'\tLabel:%d \tLabel_length:%d\n", ROOT_INDEX, bits_to_write);
	if (bit_buffer_write (compressor->bit_buffer, ROOT_INDEX, bits_to_write) < bits_to_write) {	// emit EOF code
			fprintf (stderr, "Error writing the output file\n");
			return -1;
	}
	
	return bit_buffer_complete_writing (compressor->bit_buffer);
}
