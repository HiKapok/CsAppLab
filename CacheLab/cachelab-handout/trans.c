/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	unsigned int i=0;
	unsigned int j=0;
	unsigned int k=0;
	unsigned int temp = M/8;
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;
	int temp5 = 0;
	int temp6 = 0;
	int temp7 = 0;
	int temp8 = 0;
	if(1){
		for(k=0;k<temp;k++){
			for(i=0;i<N-7;i+=8){
				for(j=k*8;(j<k*8+8);j++){
					if(i!=j)temp1 = A[i][j];
					if((i+1)!=j)temp2 = A[i+1][j];
					if((i+2)!=j)temp3 = A[i+2][j];
					if((i+3)!=j)temp4 = A[i+3][j];
					if(i!=j)B[j][i]=temp1;
					if((i+1)!=j)B[j][i+1]=temp2;
					if((i+2)!=j)B[j][i+2]=temp3;
					if((i+3)!=j)B[j][i+3]=temp4;
					if((i+4)!=j)temp5 = A[i+4][j];
					if((i+5)!=j)temp6 = A[i+5][j];
					if((i+6)!=j)temp7 = A[i+6][j];
					if((i+7)!=j)temp8 = A[i+7][j];
					if((i+4)!=j)B[j][i+4]=temp5;
					if((i+5)!=j)B[j][i+5]=temp6;
					if((i+6)!=j)B[j][i+6]=temp7;
					if((i+7)!=j)B[j][i+7]=temp8;
				
				}
				/*B[i][i]=A[i][i];
				B[i+1][i+1]=A[i+1][i+1];
				B[i+2][i+2]=A[i+2][i+2];
				B[i+3][i+3]=A[i+3][i+3];
				B[i+4][i+4]=A[i+4][i+4];
				B[i+5][i+5]=A[i+5][i+5];
				B[i+6][i+6]=A[i+6][i+6];
				B[i+7][i+7]=A[i+7][i+7];*/
			}
		}
		for(i=0;i<N-7;i+=8){
			temp1=A[i][i];
			temp2=A[i+1][i+1];
			temp3=A[i+2][i+2];
			temp4=A[i+3][i+3];
			temp5=A[i+4][i+4];
			temp6=A[i+5][i+5];
			temp7=A[i+6][i+6];
			temp8=A[i+7][i+7];

			B[i][i]=temp1;
			B[i+1][i+1]=temp2;
			B[i+2][i+2]=temp3;
			B[i+3][i+3]=temp4;
			B[i+4][i+4]=temp5;
			B[i+5][i+5]=temp6;
			B[i+6][i+6]=temp7;
			B[i+7][i+7]=temp8;
		}
	/*	for(k=0;k<temp;k++){
			for(i=0;i<N-1;i+=2){
				for(j=k*8;j<k*8+8;j++){
					int temp1 = A[i][j];
					int temp2 = A[i+1][j];
	
					B[j][i]=temp1;
					B[j][i+1]=temp2;
				}
			}
		}*/
	}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

