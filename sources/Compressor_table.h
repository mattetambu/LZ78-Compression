// Compressor_table.h

/**
 \file		Compressor_table.h
 \dir		/source/
 \date		25/06/2013
 \author	Tamburini Matteo <mattetambu@gmail.com>
 \brief		Funzioni necessarie per creare, inizializzare e distruggere la tabella hash relativa al compressore e per effettuare ricerca e inserimento di un elemento all'interno della tabella.
*/

/**
 \fn		compressor_table_t* compressor_table_create ()
 \return	Oggetto tabella hash di tipo \e compressor_table_t, o \e NULL in caso di fallimento.
 \brief		Crea un oggetto tabella hash.
*/

/**
 \fn		int compressor_table_initialize (compressor_table_t* hash_table)
 \param		[out]	hash_table			Tabella hash da inizializzare.
 \return	Risultato dell'inizializzazione della tabella hash, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Inizializza un oggetto tabella hash inserendo i primi 256 nodi.
*/

/**
 \fn		int compressor_table_destroy (compressor_table_t* hash_table)
 \param		[in]	hash_table			Tabella hash da distruggere.
 \return	Risultato della distruzione della tabella hash, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Distrugge un oggetto tabella hash precedentemente creato.
*/

/**
 \fn		int compressor_table_rebuild (compressor_table_t* hash_table)
 \param		[in]	hash_table			Tabella hash da reinizializzare.
 \return	Risultato della reinizializzare della tabella hash, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Svuota la tabella hash fornita in ingresso e la inizializza nuovamente.
*/

/**
 \fn		hindex_t compressor_table_search (compressor_table_t* hash_table, hindex_t parent_label, hvalue_t value)
 \param		[in]	hash_table			Tabella hash dove cercare il nodo.
 \param		[in]	parent_label		Indice del nodo padre del nodo da ricercare.
 \param		[in]	value				Valore del nodo da ricercare.
 \return	Risultato della ricerca nella tabella hash, \c indice del nodo in caso di successo e \c -1 altrimenti.
 \brief		Ricerca un nodo all'interno della tabella hash fornita in ingresso.
*/

/**
 \fn		int compressor_table_insert (compressor_table_t* hash_table, hindex_t parent_label, hvalue_t value, hindex_t label)
 \param		[in]	hash_table			Tabella hash dove inserire il nodo.
 \param		[in]	parent_label		Indice del nodo padre del nodo da inserire.
 \param		[in]	value				Valore del nodo da inserire.
 \param		[in]	label				Indice da assegnare al nodo da inserire.
 \return	Risultato dell'inserimento nella tabella hash, \c 0 in caso di successo (non può fallire).
 \brief		Inserisce un nodo all'interno della tabella hash fornita in ingresso.
*/

/**
 \fn		inline hindex_t compute_hash (hindex_t parent_label, hvalue_t value)
 \param		[in]	parent_label		Indice del nodo padre.
 \param		[in]	value				Valore del nodo.
 \return	\c Indice del nodo ottenuto calcolando la funzione hash sui parametri in ingresso.
 \brief		Calcola l'indice del nodo a partire dal suo valore e dall'indice del padre (sfrutta il Multiplicative hashing).
*/

/**
 \fn		inline hindex_t compute_step_hash (hindex_t parent_label, hvalue_t value)
 \param		[in]	parent_label		Indice del nodo padre.
 \param		[in]	value				Valore del nodo.
 \return	\c Indice del nodo ottenuto calcolando la funzione hash sui parametri in ingresso.
 \brief		Calcola l'indice del nodo a partire dal suo valore e dall'indice del padre (sfrutta il Division Method).
 Questa funzione viene utilizzata per calcolare il passo di scansione della tabella e viene richiamata solo se DOUBLE_HASHING è abilitato. 
*/


#ifndef COMPRESSOR_TABLE_H
#define COMPRESSOR_TABLE_H

	#include "Common.h"
	#include "Bit_buffer_io.h"
	
	
	#define DOUBLE_HASHING	false	// Want to use Double hashing
	#define EXPANSION_FACTOR 4		// Expansion factor for the hash table 
	#define HASH_TABLE_ROWS	(EXPANSION_FACTOR * MAX_CHILDS)	// Row number in the hash table 

	struct _compressor_table_row_t {
		hindex_t parent;
		hvalue_t value;
		hindex_t label;
	};

	typedef struct _compressor_table_row_t compressor_table_row_t;
	typedef struct _compressor_table_row_t compressor_table_t;

	inline hindex_t compute_hash (hindex_t, hvalue_t);	// Knuth Multiplicative hashing
	inline hindex_t compute_step_hash (hindex_t, hvalue_t);	//Division Method
	
	compressor_table_t* compressor_table_create ();
	int compressor_table_initialize (compressor_table_t*);
	int compressor_table_destroy (compressor_table_t*);
	int compressor_table_rebuild (compressor_table_t*);
	
	int compressor_table_insert (compressor_table_t*, hindex_t, hvalue_t, hindex_t);
	hindex_t compressor_table_search (compressor_table_t*, hindex_t, hvalue_t);

#endif