#include <stdio.h>
#include "ObjectFiles.h"

int main(int argc, char *argv[]) {
	MachineState *machine;
	int i = 1;
	// not enough arguments
	if (argc < 2) {
		return 1;
	}
	while (i < argc) {
		if (ReadObjectFile(argv[i], machine) == 1) {
			return 1;
		}
		i++;
	}
}