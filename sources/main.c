// main.c

/**
 \file		main.c
 \dir		/source/
 \date		25/06/2013
 \author	Tamburini Matteo <mattetambu@gmail.com>
*/

/**
 \fn		int check_arguments (int argc, char **argv, int *command, char *input_file_name[], char *output_file_name[])
 \param		[in]	argc				Numero di argomenti passati in ingresso al programma.
 \param		[in]	argv				Argomenti passati in ingresso al programma.
 \param		[out]	command				Comando richiesto.
 \param		[out]	input_file_name		Nome del file in ingresso.
 \param		[out]	output_file_name	Nome del file di output.
 \return	Risultato della verifica degli argomenti forniti al programma, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Verifica la correttezza dei parametri forniti e setta le variabili di ambiente necessarie al programma.
*/

/**
 \fn		int main (int argc, char **argv)
 \param		[in]	argc				Numero di argomenti passati in ingresso al programma.
 \param		[in]	argv				Argomenti passati in ingresso al programma.
 \return	Risultato dell'esecuzione del programma, \c 0 in caso di successo e \c -1 altrimenti.
 \brief		Esegue il comando richiesto dall'utente: help, compress o uncompress.
*/


#include "Common.h"
#include "LZ78_Compressor.h"
#include "LZ78_Uncompressor.h"
#include <getopt.h>

#define COMPRESSED_FILE_EXTENSION ".lz78"

int force_overwrite_flag = false;

int check_arguments (int argc, char **argv, int *command, char *input_file_name[], char *output_file_name[]) {
	int extension_flag = false, path_flag = false, command_set = false, output_file_name_set = false;
	int choosed_name_length = 0, option_index = 0, option_character;
	char *choosed_name = NULL;
	
	static struct option long_options[] = {
		{"compress",	required_argument,	0,	'c'},
		{"decompress",	required_argument,	0,	'd'},
		{"extension",	no_argument,		0,	'e'},
		{"force",		no_argument,		0,	'f'},
		{"help",		no_argument,		0,	'h'},
		{"name",		required_argument,	0,	'n'},
		{"output",		required_argument,	0,	'o'},
		{"path",		no_argument,		0,	'p'},
		{"uncompress",	required_argument,	0,	'u'},
		{"verbose",		no_argument,		0,	'v'}
	};
 
	while ((option_character = getopt_long (argc, argv, "c:d:u:o:n:vhefp", long_options, &option_index)) != -1) {
		switch (option_character) {
			case 'c': case 'd': case 'u':
				*input_file_name = strdup (optarg);

			case 'h': //set the command for options -h, -c and -d
				if (command_set) {
					if (*command == option_character) fprintf (stderr, "The -%c option can not be set more than once\n", (char) option_character);
					else fprintf (stderr, "Options -%c and -%c can not be used together\n", (char) option_character, (char) *command);
					return -1;
				}
				
				*command = option_character;
				command_set = true;
				break;

			case 'o': case 'n': //set output file name
				if (output_file_name_set) {
					fprintf (stderr, "The -o option can not be set more than once\n");
					return -1;
				}
				
				choosed_name = strdup (optarg);
				choosed_name_length = strlen (choosed_name);
				output_file_name_set = true;
				break;

			case 'v': //set verbose_flag
				setenv ("VERBOSE", "", 0);
				break;
				
			case 'f': //set overwrite_flag
				force_overwrite_flag = true;
				break;
			
			case 'e': //set extension_flag
				extension_flag = true;
				break;
			
			case 'p': //set extension_flag
				path_flag = true;
				break;
			
			case '?': default: // getopt_long already printed an error message
				return -1;
		}
	}
	
	if (optind < argc) { //output of any remaining command line arguments (not options)
		fprintf (stderr, "Not-option elements: ");
		while (optind < argc) fprintf (stderr, "%s ", argv[optind++]);
		fprintf (stderr, "\n");
		return -1;
	}	
	
	if (!command_set) {
		fprintf (stderr, "No executable command specified\n");
	}
	else if (*command != 'h') {
		int output_file_name_length = 0, input_file_name_length = strlen(*input_file_name);
		int file_extension_length, file_path_length, file_name_length;
		char *file_extension, *file_name = NULL, *file_path = NULL;
		
		file_extension = strrchr (*input_file_name, '.');
		file_extension_length = (file_extension == NULL)? 0 : strlen(file_extension);
		
		file_path_length = (strrchr (*input_file_name, '/') == NULL)? 0 : input_file_name_length - strlen(strrchr (*input_file_name, '/')) + 1;
		if (file_path_length > 0) {
			file_path = (char*) malloc (file_path_length + 1);
			if (file_path == NULL) {
				fprintf (stderr, "Can't allocate the memory for the file path string\n");
				errno = ENOMEM;
				return -1;
			}
			strncpy (file_path, *input_file_name, file_path_length);
			file_path[file_path_length] = '\0';
		}
		
		file_name_length = input_file_name_length - file_path_length - file_extension_length;
		if (file_name_length <= 0) { // hidden files whit no extension (file_name_length=0) or files whit no extension and a '.' in the path (file_name_length<0)
			file_extension = NULL;
			file_extension_length = 0;
			file_name_length = input_file_name_length - file_path_length;
		}
		if (file_name_length > 0) {
			file_name = (char*) malloc (file_name_length + 1);
			if (file_name == NULL) {
				fprintf (stderr, "Can't allocate the memory for the file name string\n");
				errno = ENOMEM;
				return -1;
			}
			strncpy (file_name, *input_file_name + file_path_length, file_name_length);
			file_name[file_name_length] = '\0';
		}
		
		if (*command != 'c') {
			if (file_extension == NULL || strcmp (file_extension, COMPRESSED_FILE_EXTENSION) != 0) {
				fprintf (stderr, "File extension %s not supported (need *'%s')\n", file_extension, COMPRESSED_FILE_EXTENSION);
				return -1;
			}
			file_extension = (file_name_length == 0)? NULL : strrchr (file_name, '.');
			file_extension_length = (file_extension == NULL)? 0 : strlen(file_extension);
			file_name_length -= file_extension_length;
		}
		
		if (path_flag) output_file_name_length += file_path_length;
		if (output_file_name_set) output_file_name_length += choosed_name_length;
		else output_file_name_length += file_name_length;
		if (extension_flag || !output_file_name_set) output_file_name_length += file_extension_length;
		if (*command == 'c') output_file_name_length += strlen(COMPRESSED_FILE_EXTENSION);
		
		*output_file_name = (char*) malloc (output_file_name_length + 1);
		if (*output_file_name == NULL) {
			fprintf (stderr, "Can't allocate the memory for the output file name\n");
			errno = ENOMEM;
			return -1;
		}
		output_file_name[output_file_name_length] = '\0';
		
		if (path_flag && file_path_length > 0) strcat (*output_file_name, file_path);
		if (output_file_name_set) strcat (*output_file_name, choosed_name);
		else if (file_name_length > 0) { //use the default file name if no option -o is setted			
			printf ("No name specified for the output file. It will be named '%s", file_name);
			if (*command == 'c' && file_extension_length > 0) printf ("%s'\n", file_extension);
			else printf ("'\n");
			
			strncat (*output_file_name, file_name, file_name_length);
		}
		if ((extension_flag || !output_file_name_set) && file_extension_length > 0) strcat (*output_file_name, file_extension);
		if (*command == 'c') strcat (*output_file_name, COMPRESSED_FILE_EXTENSION);
		
		printf ("Input file: %s\n", *input_file_name);
		if (getenv("VERBOSE")){
			if (*command != 'c') printf ("Input file extension: %s\n", COMPRESSED_FILE_EXTENSION);
			file_name[file_name_length] = '\0';
			printf ("Source file path: %s\n", file_path);
			printf ("Source file name: %s\n", file_name);
			printf ("Source file extension: .%s\n\n", file_extension+1);
			printf ("Output file path: %s\n", (path_flag && file_path_length > 0)? file_path : "./");
		}
		printf ("Output file name: %s\n\n", *output_file_name);
		
		free (file_name);
		free (file_path);
	}
	
	return (command_set == 0)? -1 : 0;
}

int main (int argc, char **argv) {
	char *input_file_name = NULL, *output_file_name = NULL;
	int command = 0, exit_value = -1;
	
	if (check_arguments (argc, argv, &command, &input_file_name, &output_file_name) == -1) {
		fprintf (stderr, "Bad args - Can't run the program\n");
		fprintf (stderr, "Use the -h option to see the program help\n\n");
		return -1;
	}
	
	switch (command) {
		case 'c': 
			printf ("\nCompressing '%s' in '%s'\n", input_file_name, output_file_name);			
			lz78_compressor_t *compressor = lz78_compressor_create ();
			if (compressor == NULL) fprintf (stderr, "Can't create the compressor\n");
			else if (lz78_compressor_initialize (compressor, input_file_name, output_file_name) == -1) fprintf (stderr, "Can't inizialize the compressor\n");
			else if (lz78_compressor_compress (compressor) == -1) fprintf (stderr, "Can't compress the file\n");
			else {
				printf ("Compression completed successful\n");
				exit_value = 0;
			}
			
			if (lz78_compressor_destroy (compressor) == -1) fprintf (stderr, "Can't destroy the compressor\n");
			break;
			
		case 'd': case 'u':
			printf ("\nUncompressing '%s' in '%s'\n", input_file_name, output_file_name);
			lz78_uncompressor_t *uncompressor = lz78_uncompressor_create ();
			if (uncompressor == NULL) fprintf (stderr, "Can't create the uncompressor\n");
			else if (lz78_uncompressor_initialize (uncompressor, input_file_name, output_file_name) == -1) fprintf (stderr, "Can't inizialize the uncompressor\n");
			else if (lz78_uncompressor_uncompress (uncompressor) == -1) fprintf (stderr, "Can't uncompress the file\n");
			else {
				printf ("Uncompression completed successful\n");
				exit_value = 0;
			}
			
			if (lz78_uncompressor_destroy (uncompressor) == -1) fprintf (stderr, "Can't destroy the uncompressor\n");
			break;
			
		case 'h':
			printf ("USAGE:\n");
			printf ("   lz78 <options>\n");
			printf ("OPTIONS:\n");
			printf ("   -c --compress\n");
			printf ("\t Compress specified file\n");
			printf ("   -d -u --decompress --uncompress\n");
			printf ("\t Uncompress specified file\n");
			printf ("   -o -n --output --name\n");
			printf ("\t Set the name for the output file\n");
			printf ("   -h --help\n");
			printf ("\t Show the help\n");
			printf ("   -v --verbose\n");
			printf ("\t Display more messages\n");
			printf ("   -f --force\n");
			printf ("\t Silent overwrite if the output file already exists\n");
			printf ("   -e --extension\n");
			printf ("\t Use the input file extension for the output file\n");
			printf ("   -p --path\n");
			printf ("\t Use the input file path for the output file\n");
			printf ("\n");
			exit_value = 0;
			break;

		default:
			fprintf (stderr, "Unrecognized command - Use the option -h to see the program help\n");
			break;
	}
	
	if (command != 'h') { // free memory
		free (input_file_name);
		free (output_file_name);
	}
	
	if (getenv("VERBOSE")) printf ("exit value: %d\n", exit_value);
	printf ("\n\n");
	
	return exit_value;
}
