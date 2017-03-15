#include "matrixExactCover.hpp"

void clearMatrix(int** mat, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			mat[i][j] = 0;
}

int** allocateMatrix(int rows, int cols)
{
	int** mat = new int*[rows];
	for (int i = 0; i < rows; i++)
		mat[i] = new int[cols];
	clearMatrix(mat, rows, cols);
	return mat;
}

void deallocateMatrix(int** mat, int rows)
{
	for (int i = 0; i < rows; i++)
		delete mat[i];
	delete mat;
}