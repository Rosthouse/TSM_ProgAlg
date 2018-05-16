//////////////////////////////////////////////////////////////////////////////////////////////
// serial implementation with caching 
#include <cassert>
#include "matrix.h"
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <type_traits>
#include <cstring>


#define MIN_STRASSEN (64*64)
#define OMP_DYNAMIC TRUE

void matMultSeq(const int* a, const int* b, int* const c, const int n)
{
	int* crow = c;

	for (int i = 0; i < n; i++)
	{
		int bpos = 0;
		for (int j = 0; j < n; j++) crow[j] = 0;
		for (int k = 0; k < n; k++)
		{
			for (int j = 0; j < n; j++)
			{
				crow[j] += a[k] * b[bpos++];
			}
		}
		a += n;
		crow += n;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Your best CPU matrix multiplication algorithm
void matMultCPU(const int* a, const int* b, int* const c, const int n)
{
	auto dimension = n * n;


	int* crow = c;

	for (int i = 0; i < n; i++)
	{
		memset(crow, 0, n * sizeof(int));
#pragma omp parallel for
		for (int j = 0; j < n; j++)
		{
			//crow[j] = 0;
			for (int k = 0; k < n; k++)
			{
				crow[j] += a[k] * b[k * n + j];
			}
		}

		crow += n;
	}


//	for (int i = 0; i < n; i++)
//	{
//		int bpos = 0;
//		//for (int j = 0; j < n; j++) crow[j] = 0;
//		memset(crow, 0, n * sizeof(int)); // set memory of c to zero
//#pragma omp parallel for
//		for (int k = 0; k < n; k++)
//		{
//			for (int j = 0; j < n; j++)
//			{
//				crow[j] += a[k] * b[bpos++];
//			}
//		}
//		a += n;
//		crow += n;
//	}
}
