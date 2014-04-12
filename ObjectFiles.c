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
		printf("%x\n", header_read);

		// get 2-byte header
		hi = fgetc (src_file) ;
		low = fgetc (src_file) ;
		header_read = hi*256 + low;

		if (header_read == 0xCADE) {
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
			fgetc (src_file) ;
			fgetc (src_file) ;
			
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			num_words = hi*256 + low;

			int i = 0;
			while (i < num_words) {
				fgetc (src_file) ;
				i++;
			}
		} else if (header_read == 0xF17E) {
			hi = fgetc (src_file) ;
			low = fgetc (src_file) ;
			num_words = hi*256 + low;

			int i = 0;
			while (i < num_words) {
				fgetc (src_file) ;
				i++;
			}
		} else if (header_read == 0x715E) {
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;
			fgetc (src_file) ;

		//	} else {
//			return 1;
			//}
		}
	} while (header_read != EOF);
	
	fclose (src_file);
	return 0;
}