/* ==================================================================== *
 *								        *
 *  block_dgemm.c -- Implemant a block matrix multiplication routine    *
 *                                                                      *
 * ==================================================================== */

#include "square_dgemm.h"

#include <stdio.h>
#include <stdlib.h>

/* block parameter ... */
#ifndef BLOCK_SIZE
#  define BLOCK_SIZE ((unsigned) 24)
#endif

#define min(a,b) ((a) < (b) ? (a) : (b))

/**
 *  square_dgemm -- multiply two block matrices A and B adding result to C, result is C = C + A*B
 */
void square_dgemm (const double  *A, const double  *B,  double  *C, const unsigned  M)
{
	/* TODO: implement the blocked matrix-matrix multiplication */
	unsigned  i, j, k;
	unsigned io, jo, ko; 
	for (io = 0; io < M; io += BLOCK_SIZE)
	{
		for (jo = 0; jo < M; jo += BLOCK_SIZE)
		{
			for (ko = 0; ko < M; ko += BLOCK_SIZE)
			{
				for (i = io; i < min((io + BLOCK_SIZE), M); ++i)
				{
					for (j = jo; j < min((jo + BLOCK_SIZE), M); ++j)
					{
						double  cij   = C[i * M + j];
						for (k = ko; k < min((ko + BLOCK_SIZE), M); ++k)
						{
							cij += A[i*M+k] * B[k*M+j];
						}
						C[i * M + j] = cij;
					}	
				}
			}
		}
	}
}

