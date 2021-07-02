#include <iostream>
#include <verilated.h>
#include "Vdct.h"
#include <verilated_fst_c.h> 
#include <math.h>

#define MAX_X   (8)
#define MAX_Y   (8)
int org[MAX_X * MAX_Y];


int time_counter = 0;
int main(int argc, char** argv) {
	
	Verilated::commandArgs(argc, argv);

	FILE *in=fopen(argv[1], "r");
	if (in == NULL) {
		printf("argv[1] %s", argv[1]);
		return -1;
	} 
	FILE *out=fopen(argv[2], "w");
	if (out == NULL) {
		printf("argv[2] %s", argv[2]);
		return -1;
	}
	char buf[1024];
	char *ptr;
	for(int i=0;i<64;i++) {
		ptr = fgets(buf,1024, in);
		org[i] = atoi(buf);
	}
	fclose(in);
	// Instantiate DUT
	Vdct *dut = new Vdct();
	// Trace DUMP ON
	Verilated::traceEverOn(true);
	VerilatedFstC* tfp = new VerilatedFstC;

	dut->trace(tfp, 100);  // Trace 100 levels of hierarchy
	tfp->open("simx.fst");

	// Format
	dut->RESET = 0;
	dut->CLOCK = 0;

	// Reset Time
	while (time_counter < 10) {
		dut->CLOCK = !dut->CLOCK; // Toggle clock
		dut->eval();
		tfp->dump(time_counter);  // 波形ダンプ用の記述を追加
		time_counter++;
	}
	// Release reset
	dut->RESET = 1;
//	dut->val=0;
	int state = 0;
	while (time_counter < 74 && !Verilated::gotFinish()) {
		dut->CLOCK = !dut->CLOCK; // Toggle clock
		if (dut->CLOCK) {
			int i,j;
			for(i=0;i<8;i++) {
				for(j=0;j<8;j++) {
					dut->INPUT_DATA[i][j] = org[(i*8)+j];

				}
			}
			dut->INPUT_DATA_ENABLE=1;
		}
//			dut->val++;
		// Evaluate DUT
		dut->eval();
		if (dut->CLOCK) {
			if (dut->OUTPUT_DATA_ENABLE != 0) {
				static int first = 1;
				if (first) {

					int i,j;
					for(i=0;i<8;i++) {
						for(j=0;j<8;j++) {
//							printf ("%d\n", dut->OUTPUT_DATA[i][j]);
							fprintf(out, "%d\n", dut->OUTPUT_DATA[i][j]);
						}
//						printf("\n");
					}
					first = 0;
				}
			}
//			printf("%d %x\n", dut->val, dut->out_val);
		}
		tfp->dump(time_counter);  // 波形ダンプ用の記述を追加
		time_counter++;

	}
	fclose(out);
	dut->final();
	tfp->close(); 
}