#include <stdio.h>
#include <stdlib.h>

#include "test_utility.h"


#define MAX_X   (8)
#define MAX_Y   (8)
int org[MAX_X * MAX_Y];

FILE *in;
FILE *out;

void posedge_clock_result(Vwrapper *dut){
	static int first = 1;
	if (first==11) {
		int i,j;
		for(i=0;i<8;i++) {
			for(j=0;j<8;j++) {
				fprintf(out, "%d ", dut->OUTPUT_DATA[i][j]);
//			printf( "%d %d\n", first, dut->OUTPUT_DATA[i][j]);
			}
		}
	}
		first++;
}

void posedge_clock(Vwrapper *dut){
	int i,j;
	for(i=0;i<8;i++) {
		for(j=0;j<8;j++) {
			dut->INPUT_DATA[i][j] = org[(i*8)+j];
			}
	}
	#if 0
		static int first =1;
		if (first) {
		int i,j;
		for(i=0;i<8;i++) {
			for(j=0;j<8;j++) {
				printf("%d ", dut->INPUT_DATA[i][j]);

			}
		}
		printf("\n");
		first=0;
		}
		#endif

}


void init_test(Vwrapper *dut) {
}

void end_test(Vwrapper *dut) {
	fclose(out);
}

void reset_test(Vwrapper *dut) {
	dut->RESET = 0;
	dut->CLOCK = 0;
}
void unreset_test(Vwrapper *dut) {
	dut->RESET = 1;
}

int init_param(int argc, char** argv) {
	in=fopen(argv[1], "r");
	if (in == NULL) {
		printf("argv[1] %s", argv[1]);
		return -1;
	} 
	out=fopen(argv[2], "w");
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
	return 0;
}


void toggle_clock(Vwrapper *dut) {
	dut->CLOCK = !dut->CLOCK; // Toggle clock
	dut->eval();

}
