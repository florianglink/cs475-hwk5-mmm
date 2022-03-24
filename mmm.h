#ifndef MMM_H_
#define MMM_H_

void mmm_init(int size);
void mmm_reset(double **matrix);
void mmm_freeup();
void mmm_seq();
void *mmm_par(void *args);
double mmm_verify();

double **A;
double **B;
double **C;
double **D;
int currSize;

typedef struct
{
    int tid;
    int start;
    int end;
} thread_args;

#endif /* MMM_H_ */
