// Bit_buffer_io.h

#ifndef BIT_BUFFER_IO_H_
#define BIT_BUFFER_IO_H_

	#include "Common.h"
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <endian.h>
	#include <fcntl.h>
	#include <unistd.h>
	
	#define BUFFER_SIZE		4096					// Buffer max size [bits]
	#define CELL_SIZE		32						// Buffer cell size [bits]
	
	#define data_t			hindex_t				// Type hindex_t
	
	#if CELL_SIZE == 64
		#define buffer_t	uint64_t				//(CELL_SIZE == 64)? uint64_t : (CELL_SIZE == 32)? uint32_t : (CELL_SIZE == 16)? uint16_t : uint8_t
	#elif CELL_SIZE == 32
		#define buffer_t	uint32_t
	#elif CELL_SIZE == 16
		#define buffer_t	uint16_t
	#else
		#define buffer_t	uint8_t
	#endif
	
	
	struct _bit_buffer_t {
		FILE* file_descriptor;	// File descriptor
		int file_open_mode;		// File open mode
		int bytes_to_process;	// Remaining bytes to process - Inizialized as file_size
		int position;			// Next bit free
		buffer_t buffer [BUFFER_SIZE / CELL_SIZE];	// Buffer - Buffer max number of elements is BUFFER_SIZE[bits]/CELL_SIZE[bits]
	};

	typedef struct _bit_buffer_t bit_buffer_t;

	FILE* file_open (char *file_name, char *mode);
	
	bit_buffer_t* bit_buffer_create ();
	int bit_buffer_initialize (bit_buffer_t*, char*, int);
	int bit_buffer_destroy (bit_buffer_t*);

	data_t host_to_little_endian (data_t data);
	data_t little_endian_to_host (data_t data);
	
	int bit_buffer_complete_writing (bit_buffer_t*);
	int bit_buffer_write (bit_buffer_t*, data_t, int);
	int bit_buffer_read (bit_buffer_t*, data_t*, int);
	int bit_buffer_flush (bit_buffer_t*, int);
	int bit_buffer_fill (bit_buffer_t*);
	
	int safe_write (FILE*, buffer_t*, int);
	int safe_read (FILE*, buffer_t*, int);
	
#endif