#ifndef MATRIXMGMT_H
#define MATRIXMGMT_H

void clearMatrix(int** mat, int rows, int cols);
int** allocateMatrix(int rows, int cols);
void deallocateMatrix(int** mat, int rows);


#endif// MATRIXMGMT_H