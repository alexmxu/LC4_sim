#include <stdio.h>
#include "ObjectFiles.h"

int main(int argc, char *argv[]) {
	MachineState machine;
	MachineState *mchptr;

	mchptr = &machine;

	// start from obj arguments
	int i = 2;
	
	// not enough arguments
	if (argc < 2) {
		printf ("ERROR: Not enough arguments");
		return 1;
	}
	while (i < argc) {
		//printf ("%s", argv[i]);
		if (ReadObjectFile(argv[i], mchptr) == 1) {
			//printf ("ERROR: Invalid file");
			//printf ("ERROR: Invalid file");
			//return 1;
		}
		//printf ("Loaded another obj file");
		i++;
	}
}