// LZ78_Compressor.h

/**
 \file		LZ78_Compressor.h
 \dir		/source/
 \date		25/06/2013
 \author	Tamburini Matteo <mattetambu@gmail.com>
 \brief		Funzioni necessarie per creare, inizializzare e distruggere un compressore e per effettuare la compressione di un file in ingresso.
*/

/**
 \fn		lz78_compressor_t* lz78_compressor_create ()
 \return	Oggetto compressore di tipo \e lz78_compressor_t, o \e NULL in caso di fallimento.
 \brief		Crea un oggetto compressore.
*/

/**
 \fn		int lz78_compressor_initialize (lz78_compressor_t *compressor, char *input_file_name, char *output_file_name)
 \param		[out]	compressor			Compressore da inizializzare.
 \param		[in]	input_file_name		Nome del file in ingresso da comprimere.
 \param		[in]	output_file_name	Nome del file da creare come risultato della compressione.
 \return	Risultato dell'inizializzazione del compressore, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Inizializza un oggetto compressore con i parametri passati in ingresso.
*/

/**
 \fn		int lz78_compressor_destroy (lz78_compressor_t *compressor)
 \param		[in]	compressor			Compressore da distruggere.
 \return	Risultato della distruzione del compressore, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Distrugge un oggetto compressore precedentemente creato.
*/

/**
 \fn		int lz78_compressor_compress (lz78_compressor_t *compressor)
 \param		[in]	compressor			Compressore precedentemente inizializzato.
 \return	Risultato della compressione, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Comprime il file di input e scrive il risultato della compressione nel file di output.
*/


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