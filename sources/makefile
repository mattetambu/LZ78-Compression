CC = gcc
CFLAGS = -Wall -Werror -O2
LDFLAGS = -lm

SOURCES = main.c LZ78_Compressor.c Compressor_table.c LZ78_Uncompressor.c Uncompressor_table.c Bit_buffer_io.c
OBJECTS = $(SOURCES: .c = .o)
COMPRESSED_FILE_EXTENSION = lz78
EXECUTABLE = lz78


all:
	@clear
	@echo "LZ78_Compression"
	@echo ""
	@make -s $(EXECUTABLE)
	@echo ""

help:
	@echo "LZ78_Compression help"
	@echo ""
	@echo "make [all]		- Build the executable"
	@echo "make archive		- Archive the executable, the makefile and all *.c and *.h files"
	@echo "make clean		- Remove the excutable, *.lz78 and *.o files"
	@echo "make help		- Show this info"
	@echo ""
	
	
archive: 
	@clear
	@echo "LZ78_Compression"
	@echo ""
	@echo "Archiving the whole directory to ./BACKUP.zip"
	@zip ./BACKUP.zip makefile $(EXECUTABLE) *.c *.h 
	@echo "Archived."
	@echo ""
	
clean: 
	@clear
	@echo "LZ78_Compression"
	@echo ""
	@echo "Remove the excutable, *.lz78 and *.o files"
	@rm -f $(EXECUTABLE) *.$(COMPRESSED_FILE_EXTENSION) *.o
	@echo "Removed."
	@echo ""


$(EXECUTABLE): $(SOURCES)
	@echo "Compiling sources"
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Compiling process complete."
	@echo ""


lz78.o: LZ78_Compressor.h LZ78_Uncompressor.h Common.h
LZ78_Compressor.o: LZ78_Compressor.h Compressor_table.h Bit_buffer_io.h Common.h
LZ78_Uncompressor.o: LZ78_Uncompressor.h Uncompressor_table.h Bit_buffer_io.h Common.h
Compressor_table.o: Compressor_table.h Common.h
Uncompressor_table.o: Uncompressor_table.h Common.h
Bit_buffer_io.o: Bit_buffer_io.h Common.h
