#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#include "process.h"
#include "util.h"

#define DEBUG 0			//change this to 1 to enable verbose output

/**
 * Signature for an function pointer that can compare
 * You need to cast the input into its actual 
 * type and then compare them according to your
 * custom logic
 */
typedef int (*Comparer) (const void *a, const void *b);

/**
 * compares 2 processes
 * You can assume: 
 * - Process ids will be unique
 * - No 2 processes will have same arrival time
 */
int my_comparer(const void *this, const void *that)
{
	Process* pthis = (Process*) this;
	Process* pthat = (Process*) that;
	int arrival_time = pthis->arrival_time - pthat->arrival_time;
	int priority = pthis->priority - pthat->priority;
	if (priority != 0){
		return priority;
	}else{
		return arrival_time;
	}
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
		   fprintf(stderr, "Usage: ./func-ptr <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	}

	FILE *input_file = fopen(argv[1], "r");
	if (!input_file) {
		   fprintf(stderr, "Error: Invalid filepath\n");
		   fflush(stdout);
		   return 1;
	}

	Process *processes = parse_file(input_file);
	Comparer process_comparer = &my_comparer;

#if DEBUG
	for (int i = 0; i < P_SIZE; i++) {
		    printf("%d (%d, %d) ",
				processes[i].pid,
				processes[i].priority, processes[i].arrival_time);
	}
	printf("\n");
#endif

	qsort(processes, P_SIZE, sizeof(Process), process_comparer);

	for (int i = 0; i < P_SIZE; i++) {
		    printf("%d (%d, %d)\n",
				processes[i].pid,
				processes[i].priority, processes[i].arrival_time);
	}
	fflush(stdout);
	fflush(stderr);
	free(processes);
	fclose(input_file);
	return 0;
}