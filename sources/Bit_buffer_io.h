// Bit_buffer_io.h

/**
 \file		Bit_buffer_io.h
 \dir		/source/
 \date		25/06/2013
 \author	Tamburini Matteo <mattetambu@gmail.com>
 \brief		Funzioni necessarie per creare, inizializzare e distruggere un bit buffer e gestire il file ad esso associato.
*/

/**
 \fn		bit_buffer_t* bit_buffer_create ()
 \return	Oggetto bit buffer di tipo \e bit_buffer_t, o \e NULL in caso di fallimento.
 \brief		Crea un oggetto bit buffer.
*/

/**
 \fn		int bit_buffer_initialize (bit_buffer_t* bit_buffer, char* file_name, int file_open_mode)
 \param		[out]	bit_buffer		Bit buffer da inizializzare.
 \param		[in]	file_name		Nome del file da associare al bit buffer.
 \param		[in]	file_open_mode	Modalità di apertura del file (lettura/scritttura).
 \return	Risultato dell'inizializzazione del bit buffer, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Inizializza un oggetto bit buffer con i parametri in ingresso.
*/

/**
 \fn		int bit_buffer_destroy (bit_buffer_t* bit_buffer)
 \param		[in]	bit_buffer		Bit buffer da distruggere.
 \return	Risultato della distruzione del bit buffer, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Distrugge un oggetto bit buffer precedentemente creato.
*/


/**
 \fn		FILE* file_open (char *file_name, char *mode);
\param		[in]	file_name		Nome del file da associare al bit buffer.
 \param		[in]	mode			Modalità di apertura del file (lettura/scritttura).
 \return	Risultato dell'apertura del file, \c puntatore al file in caso di successo e \c NULL altrimenti.
 \brief		Procede all'apertura del file specificato richiedendo la conferma di sostituzione all'utente se il file esiste e deve essere aperto in scrittura.
*/

/**
 \fn		data_t host_to_little_endian (data_t data)
 \param		[in]	data			Dato da convertire da formato host a formato little endian.
 \return	Dato convertito in formato little endian.
 \brief		Converte il dato fornito in ingresso da formato host a formato little endian.
*/

/**
 \fn		data_t little_endian_to_host (data_t data)
 \param		[in]	data			Dato da convertire da formato little endian a formato host.
 \return	Dato convertito in formato host.
 \brief		Converte il dato fornito in ingresso da formato little endian a formato host.
*/

/**
 \fn		int bit_buffer_complete_writing (bit_buffer_t *bit_buffer)
 \param		[in]	bit_buffer			Bit buffer che contiene i dati da scrivere nel file di output.
 \return	Risultato della scrittura sul file, \c 0 in caso di successo e \e -1 altrimenti.
 \brief		Richiama la bit_buffer_flush per scrivere nel file di output tutti i dati contenuti nel buffer (e lo svuota).
*/

/**
 \fn		int bit_buffer_write (bit_buffer_t* bit_buffer, data_t data, int length)
 \param		[in]	bit_buffer			Bit buffer dove scrivere il dato.
 \param		[in]	data				Dato da inserire nel buffer.
 \param		[in]	length				Numero di bits di \e data da scrivere nel buffer.
 \return	Totale dei bits scritti nel buffer, in caso di successo coincide con \e length.
 \brief		Inserisce nel buffer i primi \e length bit dal dato passato come parametro (preventivamente trasformato in formato little endian).
*/

/**
 \fn		int bit_buffer_read (bit_buffer_t* bit_buffer, data_t *data, int length)
 \param		[in]	bit_buffer			Bit buffer dove leggere il valore da inserire in \e data.
 \param		[out]	data				Dato da ritornare al chiamante.
 \param		[in]	length				Numero di bits del buffer da scrivere in \e data.
 \return	Totale dei bits letti dal buffer, in caso di successo coincide con \e length.
 \brief		Inserisce in \e data un totale di \e length bit letti dal buffer passato come parametro (e ricorvente quanto letto in formato host).
*/

/**
 \fn		int bit_buffer_flush (bit_buffer_t *bit_buffer, int bytes_to_write)
 \param		[in]	bit_buffer			Bit buffer dove prendere i dati da scrivere nel file di output.
 \param		[in]	bytes_to_write		Bytes da scrivere nel file di output.
 \return	Risultato della scrittura su file, \e 0 in caso di successo e \e -1 altrimenti.
 \brief		Scrive sul file i primi \e bytes_to_write bytes del buffer e lo svuota.
*/

/**
 \fn		int bit_buffer_fill (bit_buffer_t *bit_buffer)
 \param		[in]	bit_buffer			Bit buffer dove scrivere i dati letti dal file di input.
 \return	Risultato della lettura da file, \e 0 in caso di successo e \e -1 altrimenti.
 \brief		Legge da file quanti più bytes possibili (pari al minimo tra la dimesione del buffer e il numero di bytes ancora da leggere) e li inserisce nel buffer.
*/

/**
 \fn		int safe_write (FILE* file_descriptor, buffer_t *buffer, int length)
 \param		[in]	file_descriptor		Descrittore del file dove scrivere i dati dal buffer.
 \param		[in]	buffer				Buffer che contiene i dati da scrivere.
 \param		[in]	length				Numero di bytes del buffer da scrivere nel file.
 \return	Totale dei bytes scritti nel file, in caso di successo coincide con \e length.
 \brief		Scrive su file i primi \e length bytes presente nel buffer.
*/

/**
 \fn		int safe_read (FILE* file_descriptor, buffer_t *buffer, int length)
 \param		[in]	file_descriptor		Descrittore del file dove leggere i dati da scrivere nel buffer.
 \param		[in]	buffer				Buffer dove inserire i dati letti dal file.
 \param		[in]	length				Numero di bytes da leggere dal file.
 \return	Totale dei bytes letti dal file, in caso di successo coincide con \e length.
 \brief		Scrive nel buffer \e length bytes letti dal file passato come parametro.
*/


#ifndef BIT_BUFFER_IO_H_
#define BIT_BUFFER_IO_H_

	#include "Common.h"
	#include <endian.h>

	
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

	FILE* file_open (char*, char*);
	
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