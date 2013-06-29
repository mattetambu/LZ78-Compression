// Bit_buffer_io.c

#include "Bit_buffer_io.h"

extern int force_overwrite_flag;

FILE* file_open (char *file_name, char *mode) {
	char response;
	FILE *file_descriptor;
	
	if (!force_overwrite_flag && 
		strrchr (mode, 'w') != NULL &&
		(file_descriptor = fopen (file_name, "r")) != NULL) {
			fclose (file_descriptor);
			
			printf ("The file %s already exists.\nOverwrite [Y/N]? ", file_name);
			do {response = getchar();}
			while (response != 'Y' && response != 'y' && response != 'N' && response != 'n');
			printf ("\n");
			
			if (response == 'N' || response == 'n') return NULL;
	}
	
	return fopen (file_name, mode);
}

bit_buffer_t* bit_buffer_create () {
	bit_buffer_t* bit_buffer = (bit_buffer_t*) calloc (1, sizeof (bit_buffer_t));
	if (bit_buffer == NULL) {
		fprintf (stderr, "Can't allocate the memory for the bit_buffer\n");
		errno = ENOMEM;
	}
	
	return bit_buffer;
}

int bit_buffer_initialize (bit_buffer_t* bit_buffer, char* file_name, int file_open_mode) {
	if ((file_open_mode != _WRITE && file_open_mode != _READ) ||
		file_name == NULL || file_name[0] == '\0' ||
		bit_buffer == NULL) {
			fprintf (stderr, "Invalid parameter - ");
			errno = EINVAL;
			return -1;
	}
	
	bit_buffer->file_open_mode = file_open_mode;
	if ((bit_buffer->file_descriptor = file_open (file_name, (file_open_mode == _WRITE)? "w" : "r")) == NULL) {
		fprintf (stderr, "Can't open the file %s\n", file_name);
		return -1;
	}
	
	struct stat file_stat;
	if (stat (file_name, &file_stat) == -1) {
		fprintf (stderr, "Can't access the file %s\n", file_name);
		return -1;
	}
	bit_buffer->bytes_to_process = file_stat.st_size;
	
	return 0;
}

int bit_buffer_destroy (bit_buffer_t* bit_buffer) {
	if (bit_buffer == NULL) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}
	
	if (bit_buffer->file_descriptor != NULL) fclose (bit_buffer->file_descriptor);
	
	memset (bit_buffer->buffer, 0, BUFFER_SIZE/8);
	memset (bit_buffer, 0, sizeof (bit_buffer_t));
	free (bit_buffer);
	
	return 0;
}


int bit_buffer_complete_writing (bit_buffer_t* bit_buffer) {
	if (bit_buffer == NULL) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}
	
	if (bit_buffer->file_open_mode == _WRITE && bit_buffer->position > 0) {
		if (bit_buffer_flush (bit_buffer, (bit_buffer->position + 7)/8) == -1) {
			fprintf (stderr, "Error writing the output file\n");
			return -1;
		}	
	}
	
	return 0;
}

data_t host_to_little_endian (data_t data) {
	return	(CELL_SIZE == 64)? htole64 (data) :
			(CELL_SIZE == 32)? htole32 (data) :
			(CELL_SIZE == 16)? htole16 (data) : data;
}

data_t little_endian_to_host (data_t data) {
	return	(CELL_SIZE == 64)? le64toh (data) :
			(CELL_SIZE == 32)? le32toh (data) :
			(CELL_SIZE == 16)? le16toh (data) : data;
}

int bit_buffer_write (bit_buffer_t* bit_buffer, data_t data, int length) {
	uint8_t bits_written;
	data_t mask;
	int bits_to_write, total_written = 0, offset;
	
	if (length > 8*sizeof(data)) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}
	
	data = host_to_little_endian (data);
	
	while (length > 0) {									
		offset = bit_buffer->position / CELL_SIZE;
		bits_written = bit_buffer->position % CELL_SIZE;
		bits_to_write = min (CELL_SIZE - bits_written, length);
		
		mask = (bits_to_write == CELL_SIZE)? mask^(~mask) : ((data_t) 1 << bits_to_write) - 1;	//mask^(~mask) means all bits setted
		
		bit_buffer->buffer[offset] &= ((buffer_t) 1 << bits_written) - 1;
		bit_buffer->buffer[offset] |= (data & mask) << bits_written;
		if (getenv("VERBOSE")) printf ("\tOffset:%d\tBits_to_write:%d \tMask:%d\n", offset, bits_to_write, mask);
		
		length -= bits_to_write;
		bit_buffer->position += bits_to_write;
		data >>= bits_to_write;
		total_written += bits_to_write;
		
		if (bit_buffer->position >= BUFFER_SIZE) {
			if (bit_buffer_flush (bit_buffer, BUFFER_SIZE/8) == -1) {
				fprintf (stderr, "Error writing the output file\n");
				return -1;
			}
		}
	}
	if (getenv("VERBOSE")) printf ("\n");
	
	return total_written;
}

int bit_buffer_read (bit_buffer_t *bit_buffer, data_t *data, int length) {
	uint8_t bits_already_read, data_position = 0;
	buffer_t mask;
	int bits_to_read, offset = 0, bits_read = 0;
	
	if (length > 8*sizeof(*data)) {
		fprintf (stderr, "Invalid parameter - ");
		errno = EINVAL;
		return -1;
	}

	*data &= (data_t) 0;
	
	while (length > 0) {
		offset = bit_buffer->position / CELL_SIZE;
		bits_already_read = bit_buffer->position % CELL_SIZE;
		bits_to_read = min (CELL_SIZE - bits_already_read, length);
		
		mask = (bits_to_read == CELL_SIZE)? mask^(~mask) : (((buffer_t) 1 << bits_to_read) - 1) << bits_already_read;	//mask^(~mask) means all bits setted
		
		*data |= (data_t) ((mask & bit_buffer->buffer[offset]) >> bits_already_read) << data_position;
		if (getenv("VERBOSE")) printf ("\tOffset:%d\tBits_to_read:%d  \tMask:%d\n", offset, bits_to_read, mask);
		
		length -= bits_to_read;
		bit_buffer->position += bits_to_read;
		bits_read = data_position += bits_to_read;
		
		if (bit_buffer->position >= BUFFER_SIZE) {
			if (bit_buffer_fill (bit_buffer) == -1) {
				fprintf (stderr, "Error reading the input file\n");
				return -1;
			}
		}
	}
	
	*data = little_endian_to_host (*data);
	if (getenv("VERBOSE")) printf ("\n");
	
	return bits_read;
}

int bit_buffer_flush (bit_buffer_t* bit_buffer, int bytes_to_write) {
	if (safe_write (bit_buffer->file_descriptor, bit_buffer->buffer, bytes_to_write) == -1)
		return -1;
	
	memset (bit_buffer->buffer, 0, BUFFER_SIZE/8);
	bit_buffer->position = 0;
	return 0;
}

int bit_buffer_fill (bit_buffer_t* bit_buffer) {
	int total_read;
	
	memset (bit_buffer->buffer, 0, BUFFER_SIZE/8);
	if ((total_read = safe_read (bit_buffer->file_descriptor, bit_buffer->buffer, min (BUFFER_SIZE/8, bit_buffer->bytes_to_process))) == -1)
		return -1;

	bit_buffer->bytes_to_process -= total_read;
	bit_buffer->position = 0;
	return 0;
}


int safe_write (FILE* file_descriptor, buffer_t *buffer, int length) {
	int write_return_value, total_written;
	for (total_written = 0; total_written < length; total_written += write_return_value) 
		if ((write_return_value = fwrite (buffer + total_written, 1, length - total_written, file_descriptor)) == -1) return -1;
	
	return total_written;
}

int safe_read (FILE* file_descriptor, buffer_t *buffer, int length) {
	int bytes_read, total_read;
	for (total_read = 0; total_read < length; total_read += bytes_read) 
		if ((bytes_read = fread (buffer + total_read, 1, length - total_read, file_descriptor)) == -1) return -1;
	
	return total_read;
}