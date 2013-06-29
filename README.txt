=============================
LZ78 Compressor
=============================

 Progetto:
	LZ78 Compressor
	
 Autore:
	Tamburini Matteo <mattetambu@gmail.com>
	
 Descrizione: 
	Realizzare un compressore/decompressore basato sull'algoritmo LZ78.

 Compilazione dell'applicazione:
	Per la compilazione è presente un makefile quindi è sufficiente eseguire il comando "make".
	La lista degli obiettivi presenti nel makefile risulta:
		make [all]: Build the executable
		make archive: Archive the executable, the makefile and all *.c and *.h files
		make clean: Remove the excutable, *.lz78 and *.o files
		make help: Show this info
		
 Esecuzione dell'applicazione:
	Per lanciare l'applicazione è necessario eseguire l'eseguibile "lz78" utilizzando le seguenti opzioni:
		-c --compress
			Compress specified file
		-d -u --decompress --uncompress
			Uncompress specified file
		-o -n --output --name
			Set the name for the output file
		-h --help
			Show the help
		-v --verbose
			Display more messages
		-f --force
			Silent overwrite if the output file already exists
		-e --extension
			Use the input file extension for the output file
		-p --path
			Use the input file path for the output file
