#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

/* globals (anything here would be shared with all threads) */

int main(int argc, char *argv[])
{
	if(argv[1] == NULL){
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
	}
	if(strcmp(argv[1], "S") == 0 || strcmp(argv[1], "P") == 0){
		if(strcmp(argv[1], "S") == 0) {
			if(argv[2] == NULL){
				printf("Usage: ./mmm <mode> [num threads] <size>\n");
			}
			else {
				int size = atoi(argv[2]);
				currSize = size;
				mmm_init(size);
				double avgTimeSeq = 0;
				double totalTimeSeq = 0;
				for(int l=0; l<4; l++){
					double seqclockstart = rtclock();
					mmm_seq();
					double seqclockend = rtclock();
					if(l!=0){
						totalTimeSeq += (seqclockend-seqclockstart);
					}
					mmm_reset(C);
				}
				avgTimeSeq = totalTimeSeq/3;
				printf("========\n");
				printf("mode: sequential\n");
				printf("thread count: 1\n");
				printf("size: %u\n", size);
				printf("========\n");
				printf("Sequential Time: %.6f sec\n", avgTimeSeq);
			}
		}
		else {
			if(argv[2] == NULL || argv[3] == NULL) {
				printf("Usage: ./mmm <mode> [num threads] <size>\n");
			}
			else {
				int tnum = atoi(argv[2]);
				int size = atoi(argv[3]);
				currSize = size;
				double avgTimePar = 0;
				double totalTimePar = 0;
				mmm_init(size);
				for(int m=0; m<4; m++) {
					double parclockstart = rtclock();
					pthread_t threads[tnum];
					int i;
					thread_args args[tnum];
					for(i=0; i<tnum; i++) {
						args[i].tid = i;
						args[i].start = i*size/tnum+1;
						args[i].end = (i+1)*size/tnum;
						pthread_create(&threads[i], NULL, mmm_par, &args[i]);
					}
					for(i=0; i<tnum; i++) {
						pthread_join(threads[i], NULL);
					}
					double parclockend = rtclock();
					if(m!=0) {
						totalTimePar += (parclockend-parclockstart);
					}
					mmm_reset(D);
				}
				avgTimePar = totalTimePar/3;

				double avgTimeSeqP = 0;
				double totalTimeSeqP = 0;
				for(int l=0; l<4; l++){
					double seqclockstartP = rtclock();
					mmm_seq();
					double seqclockendP = rtclock();
					if(l!=0){
						totalTimeSeqP += (seqclockendP-seqclockstartP);
					}
					mmm_reset(C);
				}
				avgTimeSeqP = totalTimeSeqP/3;
				double error = mmm_verify();

				printf("========\n");
				printf("mode: parallel\n");
				printf("thread count: %u\n", tnum);
				printf("size: %u\n", size);
				printf("========\n");
				printf("Sequential Time: %.6f sec\n", avgTimeSeqP);
				printf("Parallel Time: %.6f secs\n", avgTimePar);
				printf("Speedup: %.6f\n", avgTimeSeqP/avgTimePar);
				printf("Verifying... largest error between parallel and sequential matrix: %.6f\n", error);
			}
		}
	}
	//printf("%s\n", argv[1]);
	// double clockstart, clockend;
	// clockstart = rtclock(); // start clocking

	// // start: stuff I want to clock

	// // end: stuff I want to clock

	// clockend = rtclock(); // stop clocking
	// printf("Time taken: %.6f sec\n", (clockend - clockstart));

	// return 0;
}
