#include <stdio.h>
#include <string.h>
#include "ObjectFiles.h"
#include "LC4.h"
#define BINARY_PATTERN "%d%d%d%d"
#define BINARY(num) (num & 0x08 ? 1 : 0), (num & 0x04 ? 1 : 0), \
  (num & 0x02 ? 1 : 0), (num & 0x01 ? 1 : 0) 

int main(int argc, char *argv[]) {
	// states representing the LC4
	MachineState machine;
	ControlSignals controls;
	DatapathSignals data;
	Reset(&machine);

	int i;	// counter for iterating over the arguments
	int s_flag = 0;	// 0 or 1, depending on whether -s argument is passed

	FILE *f_trace;	// destination file for TRACE
	unsigned short int arr[2];	// array for TRACE output
	FILE *f;	// destination file for ASCII output 

	// user needs to pass at least two arguments
	if (argc < 2) {
		printf ("ERROR: Not enough arguments\n");
		return 1;
	}
	// look for -s argument
	i = 0;
	while (i < argc) {
		if (strncmp(argv[i],"-s",2) == 0) {
			s_flag = 1;
		}
		i++;
	}


	// load each obj name, first possible location for obj is 2
	i = 2;
	while (i < argc) {
		// each argument from here is either -s or obj name
		if (strncmp(argv[i],"-s", 2) != 0) {
			printf("Loading object file " "%s", argv[i]);
			printf("\n");
			if (ReadObjectFile(argv[i], &machine) == 1) {
				printf ("ERROR: Invalid file\n");
				return 1;
				//EXCEPTION!!!!
			}
		}
		i++;
	}

	// generate TRACE file no matter what
	// second argument passed is filename
	// NOTE: binary trace file will have no file extension
	f_trace = fopen(argv[1], "w");
	if (f_trace == NULL)
	{
    	printf("Error: cannot write to file\n");
    	return(1);
	}
	while (machine.PC != 0x80FF) {
		// write current PC, instruction into array
		arr[0] = machine.PC;
		arr[1] = machine.memory[machine.PC];
		fwrite(arr, sizeof(short), 2, f_trace);

		// simulate one step
		DecodeCurrentInstruction (machine.memory[machine.PC], &controls);
		SimulateDatapath (&controls, &machine, &data);
		UpdateMachineState (&controls, &machine, &data);

		printf("%4x", machine.PC);
		printf (" ");
		printf("%x\n", machine.memory[machine.PC]);
		i = scanf();
		//printf (" ");
		//printf("%x", machine.PSR);
		//printf (" and RS = ");
		//printf("%x", data.RS);
		//printf(": ");
		/*
		printf (" ");
		printf("%x\n", machine.memory[machine.PC + count]);
		*/


		/*
			i = 0;
			while (i < 8) { // print out all register values
				printf("%x", machine.R[i]);
				printf(" ");
				i++;
			}
			printf ("\n");
		*/
	}
	// after completion, close binary output file
	fclose(f_trace);
		
	// generate ASCII output if -s argument passed
	if (s_flag == 1) {
		// second argument passed is filename
		f = fopen(strcat(argv[1], ".txt"), "w");
		if (f == NULL)
		{
    		printf("Error: cannot write to file\n");
    		return(1);
		}

		while (machine.PC != 0x80FF) {
			//printf("%04x", machine.PC);
			//printf (" ");
			//printf("%x", machine.memory[machine.PC]);
			//printf(": ");

			// write current PC
			fprintf(f, "%04x", machine.PC);

			// simulate one step
			DecodeCurrentInstruction (machine.memory[machine.PC], &controls);
			SimulateDatapath (&controls, &machine, &data);
			UpdateMachineState (&controls, &machine, &data);

			// write all the control signals
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.PCMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.rsMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.rtMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.rdMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.regFile_WE));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.regInputMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.Arith_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.ArithMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.LOGIC_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.LogicMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.SHIFT_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.CONST_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.CMP_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.ALUMux_CTL));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.NZP_WE));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.DATA_WE));
			fprintf(f, " " BINARY_PATTERN, BINARY(controls.Privilege_CTL));
			fprintf(f, "\n"); // new line
		}
		// after completion, close ASCII output file
		fclose(f);
	}

	// handle exceptions
	return 0;
}