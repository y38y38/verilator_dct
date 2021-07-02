#include <iostream>
#include <verilated.h>
#include "Vcalc.h"
#include <verilated_fst_c.h> 
#include <math.h>

#define MAX_X   (8)
#define MAX_Y   (8)
#if 0
int org[MAX_X * MAX_Y] =
{
-172, -172, -172, -173, -175, -170, -158, -131, 
-171, -172, -173, -173, -170, -159, -137, -117, 
-172, -172, -171, -166, -154, -136, -117, -103, 
-172, -170, -164, -152, -133, -115, -98 , -94 ,
-170, -165, -153, -136, -113, -96 , -87 , -96 ,
-160, -150, -139, -122, -103, -93 , -91 , -104, 
-145, -134, -125, -115, -107, -102, -104, -114, 
-130, -119, -113, -111, -112, -114, -118, -125, 
};

#else
#if 1
int org[MAX_X * MAX_Y] =
{
93 ,77, 52, 64, 77, 63, 65, 72, 
60, 64, 42, 45, 69, 68, 64, 69 ,
-40, -27, -38, -36, -8, 11, 18, 24, 
33, 38, 33, 24, 23, 21, 13, 8, 
89, 92, 92, 84, 78, 70, 59, 57, 
70, 72, 76, 74, 78, 86, 88, 92, 
63, 63, 68, 67, 65, 67, 67, 65, 
64, 65, 70, 70, 68, 65, 64, 66, 
};


#else
int org[MAX_X * MAX_Y] =
{
0, 1 ,2, 3, 4, 5, 6, 7, 
8, 9, 10, 11, 12, 13, 14, 15,
16, 17, 18,19,20,21,22,23,
24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,
40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,
56,57,58,59,60,61,62,63,
};
#endif
#endif
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
	Vcalc *dut = new Vcalc();
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