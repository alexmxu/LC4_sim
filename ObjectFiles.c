#include <stdio.h>
#include "LC4.h"

int ReadObjectFile (char *filename, MachineState *theMachineState) {
	int header_read;
	int address;
	int num_words;

	// least significant 2 bytes
	int low;
	// most significant 2 bytes
	int hi;

 	FILE *src_file = fopen (filename, "r");
 	if (src_file == NULL) {
		printf("ERROR: Null file\n");
 		return 1 ;
 	}
 	
	do {
		// get 2-byte header
		hi = fgetc (src_file) ;
		low = fgetc (src_file) ;
		header_read = hi*256 + low;
		
		if (hi == EOF) {
			// found end of file
			return 0;
		} else if (header_read == 0xCADE) {

			// get <address>
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			address = hi*256 + low;

			// get <n>
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			num_words = hi*256 + low;
			
			// there are n words
			int i = 0;
			while (i < num_words) {
				hi = fgetc (src_file) ;
				low = fgetc (src_file) ;

				/*
				printf("NEW CADE WORD  ");
				printf("%x", address + i);
				printf("  ");
				printf("%x", hi*256 + low);
				*/
				
				//printf("%d\n", address + i);
				theMachineState->memory[address + i] = hi*256 + low;

				//printf("  ");
				//printf("%x\n", theMachineState->memory[address + i]);
				i++;
			}
		} else if (header_read == 0xDADA) {
			// get <address>
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			address = hi*256 + low;

			// get <n>
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			num_words = hi*256 + low;

			// there are n words
			int i = 0;
			while (i < num_words) {
				hi = fgetc (src_file) ;
				low = fgetc (src_file) ;
				theMachineState->memory[address + i] = hi*256 + low;
				i++;
			}
		} else if (header_read == 0xC3B7) {
			// skip past <address>
			fgetc (src_file) ;
			fgetc (src_file) ;
			
			// get <n>
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			num_words = hi*256 + low;

			// get n 1-byte chars
			int i = 0;
			while (i < num_words) {
				fgetc (src_file) ;
				i++;
			}
		} else if (header_read == 0xF17E) {
			// get <n>
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			num_words = hi*256 + low;

			// get n 1-byte chars
			int i = 0;
			while (i < num_words) {
				fgetc (src_file) ;
				i++;
			}
		} else if (header_read == 0x715E) {
			//skip <addr>, <line>, <file-index>
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;
		} else {
			// not a recognized header
			printf("Not recognized header\n");
			return 1;
		}
	// loop until end of file
	} while (1);
	
	fclose (src_file);
	// success if program reaches here
	return 0;
}
