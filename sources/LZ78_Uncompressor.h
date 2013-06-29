// LZ78_Ununcompressor.h

/**
 \file		LZ78_Uncompressor.h
 \dir		/source/
 \date		25/06/2013
 \author	Tamburini Matteo <mattetambu@gmail.com>
 \brief		Funzioni necessarie per creare, inizializzare e distruggere un decompressore e per effettuare la decompressione di un file in ingresso.
*/

/**
 \fn		lz78_uncompressor_t* lz78_uncompressor_create ()
 \return	Oggetto decompressore di tipo \e lz78_uncompressor_t, o \e NULL in caso di fallimento.
 \brief		Crea un oggetto decompressore.
*/

/**
 \fn		int lz78_uncompressor_initialize (lz78_uncompressor_t *uncompressor, char *input_file_name, char *output_file_name)
 \param		[out]	uncompressor		Decompressore da inizializzare.
 \param		[in]	input_file_name		Nome del file compresso in ingresso da decomprimere.
 \param		[in]	output_file_name	Nome del file da creare come risultato della decompressione.
 \return	Risultato dell'inizializzazione del decompressore, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Inizializza un oggetto decompressore con i parametri passati in ingresso.
*/

/**
 \fn		int lz78_uncompressor_destroy (lz78_uncompressor_t *uncompressor)
 \param		[in]	uncompressor		Decompressore da distruggere.
 \return	Risultato della distruzione del decompressore, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Distrugge un oggetto decompressore precedentemente creato.
*/

/**
 \fn		int lz78_uncompressor_uncompress (lz78_uncompressor_t *uncompressor)
 \param		[in]	uncompressor		Decompressore precedentemente inizializzato.
 \return	Risultato della decompressione, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		decomprime il file di input e scrive il risultato della decompressione nel file di output.
*/


#ifndef LZ78_UNCOMPRESSOR_H
#define LZ78_UNCOMPRESSOR_H

	#include "Common.h"
	#include "Bit_buffer_io.h"
	#include "Uncompressor_table.h"
	
	
	struct _lz78_uncompressor_t {
		uncompressor_table_t* hash_table;
		bit_buffer_t* bit_buffer;
		FILE *file_descriptor;
		//char *input_file_name;
		//char *output_file_name;
	};

	typedef struct _lz78_uncompressor_t lz78_uncompressor_t;
	
	lz78_uncompressor_t* lz78_uncompressor_create ();
	int lz78_uncompressor_initialize (lz78_uncompressor_t*, char*, char*);
	int lz78_uncompressor_destroy (lz78_uncompressor_t*);
	
	int lz78_uncompressor_uncompress (lz78_uncompressor_t*);
	
#endif
