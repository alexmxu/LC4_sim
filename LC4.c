#include <stdio.h>
#include "LC4.h"

#define INSN_OP(I) ((I) >> 12)
#define INSN_11(I) (((I) >> 11) & 0x1)
#define INSN_11_9(I) (((I) >> 9) & 0x7)
#define INSN_8_7(I) (((I) >> 7) & 0x3)
#define INSN_8_6(I) (((I) >> 6) & 0x7)
#define INSN_5_4(I) (((I) >> 3) & 0x1)
#define INSN_5_3(I) (((I) >> 3) & 0x7)
#define INSN_2_0(I) (I & 0x7)

int DecodeCurrentInstruction (unsigned short int INSN,
	ControlSignals *theControls) {
	// default all control signals to 0, and Privelege.CTL = 2
	theControls->PCMux_CTL = 0;
	theControls->rsMux_CTL = 0;
	theControls->rtMux_CTL = 0;
	theControls->rdMux_CTL = 0;
	theControls->regFile_WE = 0;
	theControls->regInputMux_CTL = 0;
	theControls->Arith_CTL = 0;
	theControls->ArithMux_CTL = 0;
	theControls->LOGIC_CTL = 0;
	theControls->LogicMux_CTL = 0;
	theControls->SHIFT_CTL = 0;
	theControls->CONST_CTL = 0;
	theControls->CMP_CTL = 0;
	theControls->ALUMux_CTL = 0;
	theControls->NZP_WE = 0;
	theControls->DATA_WE = 0;
	theControls->Privilege_CTL = 2;

	//printf("%x\n", INSN_OP(INSN));
	// set correct control signals according to opcode
	switch (INSN_OP(INSN)) {
		// BR_----------------
		case 0: //0000
			if (INSN_11_9(INSN) == 0x000) { // NOP
				theControls->PCMux_CTL = 1;
			}
			// else all other BR_
			break;
		// ARITH--------------
		case 1: //0001
			theControls->PCMux_CTL = 1;
			theControls->regFile_WE = 1;
			theControls->NZP_WE = 1;
			switch (INSN_5_3(INSN)) {
				case 0: // ADD 000
					break;
				case 1: // MUL 001
					theControls->Arith_CTL = 1;
					break;
				case 2: // SUB 010
					theControls->Arith_CTL = 2;
					break;
				case 3: // DIV 011
					theControls->Arith_CTL = 3;
					break;
				//CHECK FOR ERRORS!11111
				default: // ADD IMM
					theControls->ArithMux_CTL = 1;
					break;
			}
			break;
		// CMP----------------
		case 2: //0010
			theControls->PCMux_CTL = 1;
			theControls->rsMux_CTL = 2;
			theControls->ALUMux_CTL = 4;
			theControls->NZP_WE = 1;
			switch (INSN_8_7(INSN)) {
				case 0: // CMP 00
					break;
				case 1: // CMPU 01
					theControls->CMP_CTL = 1;
					break;
				case 2: // CMPI 10
					theControls->CMP_CTL = 2;
					break;
				case 3: // CMPIU 11
					theControls->CMP_CTL = 3;
					break;
			}
			break;
		//--------------------
		case 4: //0100
			theControls->rdMux_CTL = 1;
			theControls->regFile_WE = 1;
			theControls->regInputMux_CTL = 2;
			theControls->NZP_WE = 1;
			if (INSN_11(INSN) == 1) { // JSR
				theControls->PCMux_CTL = 5;
			} else { // JSRR
				theControls->PCMux_CTL = 3;
			}
			break;
		// LOGIC--------------
		case 5: //0101
			theControls->PCMux_CTL = 1;
			theControls->regFile_WE = 1;
			theControls->ALUMux_CTL = 1;
			theControls->NZP_WE = 1;
			switch (INSN_5_3(INSN)) {
				case 0: // AND 000
					break;
				case 1: // NOT 001
					theControls->LOGIC_CTL = 1;
					break;
				case 2: // OR 010
					theControls->LOGIC_CTL = 2;
					break;
				case 3: // XOR 011
					theControls->LOGIC_CTL = 3;
					break;
				//CHECK FOR ERROR !!!!!!!!!!!!!
				default: // AND IMM
					theControls->LogicMux_CTL = 1;
					break;
			}
			break;
		//--------------------
		case 6: // LDR 0110
			theControls->PCMux_CTL = 1;
			theControls->regFile_WE = 1;
			theControls->regInputMux_CTL = 1;
			theControls->ArithMux_CTL = 2;
			theControls->NZP_WE = 1;
			break;
		case 7: // STR 0111
			theControls->PCMux_CTL = 1;
			theControls->rtMux_CTL = 1;
			theControls->ArithMux_CTL = 2;
			theControls->DATA_WE = 1;
			break;
		//--------------------
		case 8: // RTI 1000
			theControls->PCMux_CTL = 3;
			theControls->rsMux_CTL = 1;
			theControls->Privilege_CTL = 0;
			break;
		case 9: // CONST 1001
			theControls->PCMux_CTL = 1;
			theControls->regFile_WE = 1;
			theControls->ALUMux_CTL = 3;
			theControls->NZP_WE = 1;
			break;
		// SHIFT--------------
		case 10: //1010
			theControls->regFile_WE = 1;
			theControls->NZP_WE = 1;
			switch (INSN_5_4(INSN)) {
				case 0: // SLL 00
					theControls->ALUMux_CTL = 2;
					break;
				case 1: // SRA 01
					theControls->SHIFT_CTL = 1;
					theControls->ALUMux_CTL = 2;
					break;
				case 2: // SRL 10
					theControls->SHIFT_CTL = 2;
					theControls->ALUMux_CTL = 2;
					break;
				case 3: // MOD 11
					theControls->Arith_CTL = 4;
					break;
			}
			break;
		// JMP----------------
		case 12: //1100
			if (INSN_11(INSN) == 0) { // JMPR
				theControls->PCMux_CTL = 3;
			}
			else { // JMP
				theControls->PCMux_CTL = 2;
			}
			break;
		//--------------------
		case 13: // HICONST 1101
			theControls->PCMux_CTL = 1;
			theControls->regFile_WE = 1;
			theControls->CONST_CTL = 1;
			theControls->ALUMux_CTL = 3;
			theControls->NZP_WE = 1;
			break;
		//--------------------
		case 15: // TRAP 1111
			theControls->PCMux_CTL = 4;
			theControls->rdMux_CTL = 1;
			theControls->regFile_WE = 1;
			theControls->regInputMux_CTL = 2;
			theControls->NZP_WE = 1;
			theControls->Privilege_CTL = 1;
			break;
		//--------------------
		default:
			// illegal code
			printf("Illegal opcode");
			return 1;
			break;
	}
	return 0;
}

int SimulateDatapath (ControlSignals *theControls, MachineState 
 *theMachineState, DatapathSignals *theDatapath) {
 	unsigned short int PC = theMachineState->PC;
 	unsigned short int addr;
 	signed int input;
 	int imm;
 	int output;

 	// set RS
 	switch (theControls->rsMux_CTL) {
 		case 0: // I[8:6]
 			addr = INSN_8_6(theMachineState->memory[PC]);
 			theDatapath->RS = theMachineState->R[addr];
 			break;
 		case 1: // 0X07
 			theDatapath->RS = theMachineState->R[0x07];
 			break;
 		case 2: // I[11:9]
 			addr = INSN_11_9(theMachineState->memory[PC]);
 			theDatapath->RS = theMachineState->R[addr];
 			break;
 	}
 	// set RT
 	switch (theControls->rtMux_CTL) {
 		case 0:
 			addr = INSN_2_0(theMachineState->memory[PC]);
 			theDatapath->RT = theMachineState->R[addr];
 			break;
 		case 1:
 			addr = INSN_11_9(theMachineState->memory[PC]);
 			theDatapath->RT = theMachineState->R[addr];
 			break;
  	}
 	// set correct input into arith module
 	switch (theControls->ArithMux_CTL) {
 		case 0:
 			input = theDatapath->RT;
 			break;
 		case 1: // SEXT[IMM5]
 			input = (signed short int)(theMachineState->memory[PC] << 11) >> 11;
 			break;
 		case 2: // SEXT[IMM6]
 		 	input = (signed short int)(theMachineState->memory[PC] << 10) >> 10;
 			break;
 	}
 	// set output from arith module
 	switch (theControls->Arith_CTL) {
 		case 0: // ADD
 			/*
 			printf("adding ");
 			printf("%x", theDatapath->RS);
 			printf(" by ");
 			printf("%x", input);
 			printf("\n");
 			*/

 			theDatapath->ArithmeticOps = theDatapath->RS + input;
 			break;
 		case 1: // MUL
 		 	theDatapath->ArithmeticOps = theDatapath->RS * input;
 			break;
 		case 2: // SUB
  			theDatapath->ArithmeticOps = theDatapath->RS - input;
 			break;
 		case 3: // DIV
 			/*
 			printf("dividing ");
 			printf("%d\n", (unsigned)theDatapath->RS);
 			printf(" by ");
 			printf("%d\n",  (unsigned)input);
 			printf(". RT is ");
 			printf("%d\n", theDatapath->RT);
  			*/
  			theDatapath->ArithmeticOps =
  				(int) ((unsigned)theDatapath->RS / (unsigned)input);
 			break;
 		case 4: // MOD
 			theDatapath->ArithmeticOps =
 				(unsigned)theDatapath->RS % (unsigned)input;
 			break;
 	}
 	// set correct input into logic module
 	switch (theControls->LogicMux_CTL) {
 		case 0:
 			input = theDatapath->RT;
 			break;
 		case 1: // SEXT[IMM5]
 			input = (signed short int)(theMachineState->memory[PC] << 11) >> 11;
 			break;
 	}
 	// set output from logic module
 	switch (theControls->LOGIC_CTL) {
 		case 0: // AND
 			theDatapath->LogicalOps = theDatapath->RS & input;
 			break;
 		case 1: // NOT
 		 	theDatapath->LogicalOps = ~ theDatapath->RS;
 			break;
 		case 2: // OR
  			theDatapath->LogicalOps = theDatapath->RS | input;
 			break;
 		case 3: // XOR
  			theDatapath->LogicalOps = theDatapath->RS ^ input;
 			break;
 	}
 	// set output from shifter module
 	switch (theControls->SHIFT_CTL) {
 		imm = theMachineState->memory[PC] & 0xF; // [0:3]
 		case 0: // SLL
 			theDatapath->Shifter = theDatapath->RS << imm;
 			break;
 		case 1: // SRA
 			theDatapath->Shifter = ((signed short int)theDatapath->RS) >> imm;
 			break;
 		case 2: // SRL
 			theDatapath->Shifter = theDatapath->RS >> imm;
 			break;
 	}
 	// set output from const module
 	switch (theControls->CONST_CTL) {
 		case 0: // IMM
 		// SEXT(IMM9)
 			theDatapath->Constants =
 				(signed short int)(theMachineState->memory[PC] << 7) >> 7;
 			break;
 		case 1: // (RS&xFF)|(UIMM8<<8)
 			theDatapath->Constants =
 				(theDatapath->RS & 0XFF) | (theMachineState->memory[PC] << 8);
 			break; 
 	}
 	switch (theControls->CMP_CTL) {
 		// [0:6]
 		int imm = (signed short int)(theMachineState->memory[PC] << 9) >> 9;
 		case 0:
 			if (theDatapath->RS < theDatapath->RT) {
 				theDatapath->Comparator = -1;
 			} else if (theDatapath->RS == theDatapath->RT) {
 				theDatapath->Comparator = 0;
 			} else {
 				theDatapath->Comparator = 1;
 			}
 			break;
 		case 1:
 		 	if ((unsigned)theDatapath->RS < (unsigned)theDatapath->RT) {
 				theDatapath->Comparator = -1;
 			} else if ((unsigned)theDatapath->RS == (unsigned)theDatapath->RT) {
 				theDatapath->Comparator = 0;
 			} else {
 				theDatapath->Comparator = 1;
 			}
 			break;
 		case 2:
 			if (theDatapath->RS < imm) {
 				theDatapath->Comparator = -1;
 			} else if (theDatapath->RS == imm) {
 				theDatapath->Comparator = 0;
 			} else {
 				theDatapath->Comparator = 1;
 			}
 			break;
 		case 3:
 			if ((unsigned)theDatapath->RS < (unsigned)imm) {
 				theDatapath->Comparator = -1;
 			} else if ((unsigned)theDatapath->RS == (unsigned)imm) {
 				theDatapath->Comparator = 0;
 			} else {
 				theDatapath->Comparator = 1;
 			}
 			break;
 	}
 	switch (theControls->ALUMux_CTL) {
 		case 0: // arithmetic
 			theDatapath->ALUMux = theDatapath->ArithmeticOps;
 			break;
 		case 1: // logical
 			theDatapath->ALUMux = theDatapath->LogicalOps;
 			break;
 		case 2: // shifter
 			theDatapath->ALUMux = theDatapath->Shifter;
 			break;
 		case 3: // constants
 			theDatapath->ALUMux = theDatapath->Constants;
 			break;
 		case 4: // comparator
 			theDatapath->ALUMux = theDatapath->Comparator;
 			break;
 	}
 	switch (theControls->regInputMux_CTL) {
 		case 0: // ALU
 			theDatapath->regInputMux = theDatapath->ALUMux;
 			break;
 		case 1: // data memory
 			theDatapath->regInputMux =
 				theMachineState->memory[theDatapath->ALUMux];
 			break;
 		case 2: // PC + 1
 			theDatapath->regInputMux = PC + 1;
 			break;
 	}
 	switch (theControls->PCMux_CTL) {
 		case 0: // BR_
 			// NZP = PSR[2:0]. check if any of the N,Z,P bits are the same
 			if ((INSN_11_9(theMachineState->memory[PC]) &
 				INSN_2_0(theMachineState->PSR)) != 0) {
 				// SEXT(IMM9)
 				theDatapath->PCMux = PC + 1 +
 					((signed short int)(theMachineState->memory[PC] << 7) >> 7);
 			} else {
 				theDatapath->PCMux = PC + 1;
 			}
 			break;
 		case 1: // PC + 1
 			theDatapath->PCMux = PC + 1;
 			break;
 		case 2: // PC + 1 + SEXT(IMM11)
 			// SEXT(IMM11)
			theDatapath->PCMux = PC + 1 +
				((signed short int)(theMachineState->memory[PC] << 5) >> 5);
 			break;
 		case 3: // RS
 			theDatapath->PCMux = theDatapath->RS;
 			break;
 		case 4:
 			// UIMM8 = [7:0]
 			theDatapath->PCMux = 0x8000 | (theMachineState->memory[PC] & 0xFF);
 			break;
 		case 5:
 			theDatapath->PCMux = (PC & 0x8000) |
 				((signed short int)(theMachineState->memory[PC] << 5) >> 5) << 4;
 			break;
 	}
	return 0;
}

int UpdateMachineState (ControlSignals *theControls,
	MachineState *theMachineState, DatapathSignals *theDatapath) {
	int RD;

	// set RD
	if (theControls->rdMux_CTL == 0) {
		RD = INSN_11_9(theMachineState->memory[theMachineState->PC]);
	} else {
		RD = 0x07;
	}

	if (theControls->regFile_WE == 1) {
		theMachineState->R[RD] = theDatapath->regInputMux;
	}
	if (theControls->NZP_WE == 1) {
		if (theDatapath->regInputMux < 0) {
			// set last three bits to 100
			theMachineState->PSR = (theMachineState->PSR & 0xFFF8) | 0x4;
		} else if (theDatapath->regInputMux == 0) {
			// set last three bits to 010
			theMachineState->PSR = (theMachineState->PSR & 0xFFF8) | 0x2;
		} else {
			// set last three bits to 001
			theMachineState->PSR = (theMachineState->PSR & 0xFFF8) | 0x1;
		}
	}
	if (theControls->DATA_WE == 1) {
		theMachineState->memory[theDatapath->ALUMux] = theDatapath->RT;
	}

	if (theControls->Privilege_CTL == 0) {
		// set privelege bit to 0 - user mode
		// PSR & 0111 1111 1111 1111
		theMachineState->PSR = theMachineState->PSR & 0x7FFF;
	} else if (theControls->Privilege_CTL == 1) {
		// set privelege bit to 1 - os mode
		// PSR | 1000 0000 0000 0000
		theMachineState->PSR = theMachineState->PSR | 0x8000;
	}
	theMachineState->PC = theDatapath->PCMux;

	// return any errors
	return 0;
}

void Reset (MachineState *theMachineState) {
	int i = 0;
	while (i < 8) {
		theMachineState->memory[i] = 0;
		i++;
	}
	theMachineState->PC = 0x8200;
	theMachineState->PSR = 0X8002;
}