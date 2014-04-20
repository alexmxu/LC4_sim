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
			if (ReadObjectFile(argv[i], &machine) != 0) {
				printf ("ERROR: Invalid file\n");
				return 1;
			}
		}
		i++;
	}

	// generate TRACE file no matter what
	// second argument passed is filename
	// NOTE: binary trace file will have no file extension
	//strcat(argv[1], ".txt"), "w");
	f_trace = fopen(argv[1], "w");
	if (f_trace == NULL)
	{
    	printf("Error: cannot write to file\n");
    	return(1);
	}
	// generate ASCII output if -s argument passed
	if (s_flag == 1) {
		// second argument passed is filename
		f = fopen(strcat(argv[1], ".txt"), "w");
		if (f == NULL)
		{
    		printf("Error: cannot write to file\n");
    		return(1);
		}
	}
	// SIMULATE LC4!!!
	while (machine.PC != 0x80FF) {
		//printf("%04x", machine.PC);
		//printf (" ");
		//printf("%x\n", machine.memory[machine.PC]);

		// write TRACE output BEFORE executing instructions
		// write current PC, instruction into array
		arr[0] = machine.PC;
		arr[1] = machine.memory[machine.PC];

		// simulate one step and end simulation if exception found
		if (DecodeCurrentInstruction (machine.memory[machine.PC], &controls) != 0)
			break;
		if (SimulateDatapath (&controls, &machine, &data) != 0)
			break;
		if (UpdateMachineState (&controls, &machine, &data) != 0)
			break;

		// if no exceptions, actually write PC values
		fwrite(arr, sizeof(unsigned short int), 2, f_trace);
		if (s_flag == 1)
			fprintf(f, "%04x", machine.PC); // write current PC

		// write ASCII control signals output AFTER instructions
		if (s_flag == 1) {
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
			fprintf(f, "\r\n"); // new line
		}

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
	if (s_flag == 1)
		fclose(f);

	// return without exceptions
	return 0;
}