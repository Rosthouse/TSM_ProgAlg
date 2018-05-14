#pragma once

typedef int real_t;

struct matrix
{
	int size1, size2, tda;
	real_t* data;
};

typedef struct matrix matrix_t;

#define matrix(m,i,j) m.data[(i)*m.tda + j]

matrix_t matrix_alloc(int h, int w, const real_t* data);
matrix_t matrix_alloc(int h, int w);
void matrix_free(matrix_t m);
void matrix_mul(matrix_t c, matrix_t a, matrix_t b);
