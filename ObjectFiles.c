#include <stdio.h> 
#include "LC4.h"

int ReadByte (FILE *src) {
	return fgetc(src)*256 + fgetc(src);
}

int ReadObjectFile (char *filename, MachineState *theMachineState) {
	int header_read;
	int address;
	int num_words;

 	FILE *src_file = fopen (filename, "r"); 
 	if (src_file == NULL) { return 1 ; } 
	do {
		header_read = ReadByte(src_file);

		// end of file
		if (header_read == EOF) {
			return 0;
		} else if (header_read == 0xCADE) {
			address = ReadByte(src_file);
			num_words = ReadByte(src_file);

			int i = 0;
			while (num_words > 0) {
				theMachineState->memory[address + i] = ReadByte(src_file);
				i++;
			}
		} else if (header_read == 0xDADA) {
			address = ReadByte(src_file);
			num_words = ReadByte(src_file);

			int i = 0;
			while (num_words > 0) {
				theMachineState->memory[address + i] = ReadByte(src_file);
				i++;
			}
		} else if (header_read == 0xC3B7) {
			address = ReadByte(src_file);
			num_words = ReadByte(src_file);

			int i = 0;
			while (num_words > 0) {
				fgetc(src_file);
				i++;
			}
		} else if (header_read == 0xF17E) {
			num_words = ReadByte(src_file);

			int i = 0;
			while (num_words > 0) {
				fgetc(src_file);
				i++;
			}
		} else if (header_read == 0x715E) {
			ReadByte(src_file);
			ReadByte(src_file);
			ReadByte(src_file);
		} else {
			return 1;
		}
		
	} while (1);
 
	fclose (src_file);
	return 0;
}
