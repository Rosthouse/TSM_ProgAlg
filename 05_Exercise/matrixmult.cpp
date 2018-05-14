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
void matMultCPU(const int* a, const int* b, const int* c, int n)
{
	// TODO: your parallel CPU implementation

	// Calculate difference to power of two
	//	const int nextPowerOfTwo = round(pow(ceil(log2(n)), 2));
	//	const int diffToPowerOfTwo = nextPowerOfTwo - n;
	// Padd with zeros until filled
	const matrix_t matA = matrix_alloc(n, n, a);
	const matrix_t matB = matrix_alloc(n, n, b);
	const matrix_t matC = matrix_alloc(n, n, c);

	matrix_mul(matA, matB, matC);
}

matrix_t matrix_alloc(int h, int w, const real_t* data)
{
	matrix_t m = matrix_alloc(h, w);
	memcpy(m.data, data, (h + 1) * m.tda * sizeof(real_t));
	return m;
}


matrix_t matrix_alloc(int h, int w)
{
	matrix_t m;
	m.size1 = h;
	m.size2 = w;
	m.tda = w + 1 + !(w & 1);
	m.data = static_cast<int*>(malloc((h + 1) * m.tda * sizeof(real_t)));
	//std::aligned_storage<(h + 1) * m.tda * sizeof(real_t)>(&m.data);
	return m;
}

void matrix_free(matrix_t m)
{
	free(m.data);
}

void matrix_add(matrix_t c, matrix_t a, matrix_t b)
{
	int i, j;
	for (i = 0; i < a.size1; i++)
		for (j = 0; j < a.size2; j++)
			matrix(c,i,j) = matrix(a,i,j) + matrix(b,i,j);
}

void matrix_sub(matrix_t c, matrix_t a, matrix_t b)
{
	int i, j;
	for (i = 0; i < a.size1; i++)
		for (j = 0; j < a.size2; j++)
			matrix(c,i,j) = matrix(a,i,j) - matrix(b,i,j);
}

void matrix_mul_simple(matrix_t c, matrix_t a, matrix_t b)
{
	int i, j, k;
	real_t x;

	for (i = 0; i < a.size1; i++)
	{
		for (j = 0; j < b.size2; j++)
		{
			x = 0;
			for (k = 0; k < a.size2; k++)
				x += matrix(a,i,k) * matrix(b,k,j);
			matrix(c,i,j) = x;
		}
	}
}

void submatrices(matrix_t a, matrix_t* a11, matrix_t* a12, matrix_t* a21, matrix_t* a22)
{
	int hh = a.size1 >> 1;
	a11->size1 = a12->size1 = hh;
	a21->size1 = a22->size1 = hh; //a.size1-hh;
	int wh = a.size2 >> 1;
	a11->size2 = a21->size2 = wh;
	a12->size2 = a22->size2 = wh; //a.size2-wh;
	a11->tda = a12->tda = a21->tda = a22->tda = a.tda;
	a11->data = &matrix(a,0,0);
	a12->data = &matrix(a,0,wh);
	a21->data = &matrix(a,hh,0);
	a22->data = &matrix(a,hh,wh);
}

// P = (A1 + A2) * (B1 + B2)
void strassen_1(matrix_t a1, matrix_t a2, matrix_t b1, matrix_t b2, matrix_t p)
{
	matrix_t f = matrix_alloc(a1.size1, a1.size2);
	matrix_t s = matrix_alloc(b1.size1, b1.size2);
#pragma omp parallel sections
	{
#pragma omp section
		matrix_add(f, a1, a2);
#pragma omp section
		matrix_add(s, b1, b2);
	}
	matrix_mul(p, f, s);
	matrix_free(f);
	matrix_free(s);
}

// P = (A1 + A2) * B
void strassen_2(matrix_t a1, matrix_t a2, matrix_t b, matrix_t p)
{
	matrix_t f = matrix_alloc(a1.size1, a1.size2);
	matrix_add(f, a1, a2);
	matrix_mul(p, f, b);
	matrix_free(f);
}

// P = A * (B1 - B2)
void strassen_3(matrix_t a, matrix_t b1, matrix_t b2, matrix_t p)
{
	matrix_t s = matrix_alloc(b1.size1, b1.size2);
	matrix_sub(s, b1, b2);
	matrix_mul(p, a, s);
	matrix_free(s);
}

// P = (A1 - A2) * (B1 + B2)
void strassen_4(matrix_t a1, matrix_t a2, matrix_t b1, matrix_t b2, matrix_t p)
{
	matrix_t f = matrix_alloc(a1.size1, a1.size2);
	matrix_t s = matrix_alloc(b1.size1, b1.size2);
#pragma omp parallel sections
	{
#pragma omp section
		matrix_sub(f, a1, a2);
#pragma omp section
		matrix_add(s, b1, b2);
	}
	matrix_mul(p, f, s);
	matrix_free(f);
	matrix_free(s);
}

// P1 += P2 + (P3 - P4)
void strassen_5(matrix_t p1, matrix_t p2, matrix_t p3, matrix_t p4)
{
	matrix_t f = matrix_alloc(p1.size1, p1.size2);
#pragma omp parallel sections
	{
#pragma omp section
		matrix_add(p1, p1, p2);
#pragma omp section
		matrix_sub(f, p3, p4);
	}
	matrix_add(p1, p1, f);
	matrix_free(f);
}

void matrix_mul_strassen(matrix_t c, matrix_t a, matrix_t b)
{
	matrix_t p1, p2, p3, c11, c12, c21, c22;
	matrix_t a11, a12, a21, a22, b11, b12, b21, b22;

	int hp = a.size1 >> 1;
	int wp = b.size2 >> 1;
	p1 = matrix_alloc(hp, wp);
	p2 = matrix_alloc(hp, wp);
	p3 = matrix_alloc(hp, wp);

	submatrices(a, &a11, &a12, &a21, &a22);
	submatrices(b, &b11, &b12, &b21, &b22);
	submatrices(c, &c11, &c12, &c21, &c22);

#pragma omp parallel sections
	{
#pragma omp section
		// I: P = (A1 + A2) * (B1 + B2)
		strassen_1(a11, a22, b11, b22, p1);
#pragma omp section
		// II: P = (A1 + A2) * B
		strassen_2(a21, a22, b11, c21);
#pragma omp section
		// III
		strassen_2(a11, a12, b22, c12);
#pragma omp section
		// IV: P = A * (B1 - B2)
		strassen_3(a11, b12, b22, p2);
#pragma omp section
		// V
		strassen_3(a22, b21, b11, p3);
#pragma omp section
		// VI P = (A1 - A2) * (B1 + B2)
		strassen_4(a21, a11, b11, b12, c22);
#pragma omp section
		// VII
		strassen_4(a12, a22, b21, b22, c11);
	}

#pragma omp parallel sections
	{
		// P1 += P2 + (P3 - P4)
#pragma omp section
		strassen_5(c11, p1, p3, c12);
#pragma omp section
		strassen_5(c22, p1, p2, c21);
	}

#pragma omp parallel sections
	{
		// P1 += P2
#pragma omp section
		matrix_add(c12, c12, p2);
#pragma omp section
		matrix_add(c21, c21, p3);
	}

	int i, j, k;
	real_t x;
#pragma omp parallel sections
	{
#pragma omp section
		if (a.size1 & 1)
		{
			// (ultima fila de C) = (ultima fila de A) * B
			k = a.size1 - 1;
			for (j = 0; j < b.size2; j++)
			{
				x = 0;
				for (i = 0; i < b.size1; i++)
					x += matrix(a,k,i) * matrix(b,i,j);
				matrix(c,k,j) = x;
			}
		}
#pragma omp section
		if (b.size2 & 1)
		{
			// (ultima col de C) = A * (ultima col de B)
			k = b.size2 - 1;
			for (i = 0; i < a.size1; i++)
			{
				x = 0;
				for (j = 0; j < a.size2; j++)
					x += matrix(a,i,j) * matrix(b,j,k);
				matrix(c,i,k) = x;
			}
		}
	}
	if (a.size2 & 1)
	{
		// C += (ultima col de A) * (ultima fila de B)
		k = a.size2 - 1;
		for (i = 0; i < (c.size1 & ~1); i++)
		{
			for (j = 0; j < (c.size2 & ~1); j++)
				matrix(c,i,j) += matrix(a,i,k) * matrix(b,k,j);
		}
	}

	matrix_free(p1);
	matrix_free(p2);
	matrix_free(p3);
}

void matrix_mul(matrix_t c, matrix_t a, matrix_t b)
{
	assert(a.size2==b.size1 && c.size1==a.size1 && c.size2==b.size2);
	if (c.size1 * c.size2 < MIN_STRASSEN)
		matrix_mul_simple(c, a, b);
	else
		matrix_mul_strassen(c, a, b);
}
