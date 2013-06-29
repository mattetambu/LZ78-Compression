// Uncompressor_table.h

/**
 \file		Uncompressor_table.h
 \dir		/source/
 \date		25/06/2013
 \author	Tamburini Matteo <mattetambu@gmail.com>
 \brief		Funzioni necessarie per creare, inizializzare e distruggere la tabella hash relativa al decompressore e per effettuare ricerca e inserimento di un elemento all'interno della tabella.
*/

/**
 \fn		uncompressor_table_t* uncompressor_table_create ()
 \return	Oggetto tabella hash di tipo \e uncompressor_table_t, o \e NULL in caso di fallimento.
 \brief		Crea un oggetto tabella hash.
*/

/**
 \fn		int uncompressor_table_initialize (uncompressor_table_t* hash_table)
 \param		[out]	hash_table			Tabella hash da inizializzare.
 \return	Risultato dell'inizializzazione della tabella hash, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Inizializza un oggetto tabella hash inserendo i primi 256 nodi.
*/

/**
 \fn		int uncompressor_table_destroy (uncompressor_table_t* hash_table)
 \param		[in]	hash_table			Tabella hash da distruggere.
 \return	Risultato della distruzione della tabella hash, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Distrugge un oggetto tabella hash precedentemente creato.
*/

/**
 \fn		int uncompressor_table_insert (uncompressor_table_t* hash_table, hindex_t parent_label, hvalue_t value, hindex_t label)
 \param		[in]	hash_table			Tabella hash dove inserire il nodo.
 \param		[in]	parent_label		Indice del nodo padre del nodo da inserire.
 \param		[in]	value				Valore del nodo da inserire.
 \param		[in]	label				Indice del nodo da inserire.
 \return	Risultato dell'inserimento nella tabella hash, \c 0 in caso di successo (non può fallire).
 \brief		Inserisce un nodo all'interno della tabella hash fornita in ingresso.
*/

/**
 \fn		hvalue_t uncompressor_table_lookup (uncompressor_table_t* hash_table, hindex_t label, FILE* file_descriptor)
 \param		[in]	hash_table			Tabella hash dove cercare il nodo.
 \param		[in]	label				Indice del nodo dal quale iniziare la risalita verso la radice.
 \param		[in]	file_descriptor		File descriptor relativo al file di output.
 \return	Valore del nodo figlio della radice ottenuto risalendo l'intero albero.
 \brief		Scrive nel file di output la sequenza di caratteri relativa all'ultimo codifica ricevuta e ritorna il simbolo relativo a tale codifica (per il suo inserimento).
*/


#ifndef UNCOMPRESSOR_TABLE_H
#define UNCOMPRESSOR_TABLE_H

	#include "Common.h"
	#include "Bit_buffer_io.h"
	
	
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