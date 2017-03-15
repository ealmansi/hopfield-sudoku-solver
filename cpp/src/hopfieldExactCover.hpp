#ifndef HOPFIELD_EXACT_COVER_H
#define HOPFIELD_EXACT_COVER_H

typedef struct {
   int numThreads;
   int numIterations;
   double hCoeff;
   double thCoeff;
   double tempCoeff;
} HopfieldExactCoverParams;

typedef struct {
   HopfieldExactCoverParams *params;
   int *state;
   int** coeffMat;
   int rows;
} ThreadData;

bool solveExactCover(int** mat, int rows, int cols, int* solution, HopfieldExactCoverParams* params);

#endif //HOPFIELD_EXACT_COVER_H