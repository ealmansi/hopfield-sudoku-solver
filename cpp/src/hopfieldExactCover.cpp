#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std;

#include "auxiliaryMacros.hpp"
#include "hopfieldExactCover.hpp"
#include "matrixExactCover.hpp"

void* threadRoutine( void* arguments );
int computeErrorFunction( int** mat, int rows, int cols, int* state );

bool solveExactCover( int** mat, int rows, int cols, int* state, HopfieldExactCoverParams* params ) {
    //compute coefficients matrix ( mat * transpose(mat) )
    int** coeffMat = allocateMatrix( rows, rows );

    for( int i = 0; i < rows; i++ ) {
        for( int j = 0; j <= i; j++ ) {
            int coeff = 0;

            for( int k = 0; k < cols; k++ ) {
                coeff += ( mat[i][k] * mat[j][k] );
            }

            coeffMat[i][j] = coeffMat[j][i] = coeff;
        }
    }

    //init state
    for( int i = 0; i < rows; i++ )
        state[i] = ( ( double )rand() / RAND_MAX ) + 0.1;

    //init thread vars
    pthread_t threads[params->numThreads];
    ThreadData threadData[params->numThreads];
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );


    //run threads
    for( int i = 0; i < params->numThreads; i++ ) {
        threadData[i].params = params;
        threadData[i].state = state;
        threadData[i].coeffMat = coeffMat;
        threadData[i].rows = rows;
        int threadCreateRet = pthread_create( &threads[i], NULL, threadRoutine, ( void* )&threadData[i] );

        if( threadCreateRet != 0 )
            EXIT_WITH_ERROR_MSG( "Unable to create thread." );
    }

    // free attribute and wait for the other threads
    pthread_attr_destroy( &attr );
    void* joinStatus;

    for( int i = 0; i < params->numThreads; i++ ) {
        int threadJoinRet = pthread_join( threads[i], &joinStatus );

        if( threadJoinRet )
            EXIT_WITH_ERROR_MSG( "Unable to join thread." );
    }

    int errorFunction = computeErrorFunction( mat, rows, cols, state );
    if(errorFunction != 0) {
        cout << "Unable to find a solution to the puzzle. ";
        cout << "The minimum value achieved for the error function was: " << errorFunction << endl;
    }

    deallocateMatrix( coeffMat, rows );

    return errorFunction == 0;
}

/*threadRoutine*/

void* threadRoutine( void* arguments ) {
    ThreadData* data = ( ThreadData* ) arguments;

    int numIterations = data->params->numIterations;
    double hCoeff = data->params->hCoeff;
    double thCoeff = data->params->thCoeff;
    double tempCoeff = data->params->tempCoeff;
    int* state = data->state;
    int** coeffMat = data->coeffMat;
    int rows = data->rows;

    for( int it = 0; it < numIterations; it++ ) {
        //update random neuron
        int i = rand() % rows;

        //get the incidence of every neuron on the i-th neuron
        int w[rows];

        for( int j = 0; j < rows; j++ )
            w[j] = -coeffMat[i][j];

        w[i] = 0;

        //get the activation level for the i-th neuron
        int h = 0;

        for( int j = 0; j < rows; j++ )
            h += w[j] * state[j];

        h <<= 1;

        // every row has four 1's, so constant threshold for all neurons
        int threshold = -4;

        //if activation level is above threshold, make neuron active
        double randomTerm = tempCoeff * ( ( rand() % 101 - 50 ) * 0.02 );
        double diff = hCoeff * h - thCoeff * threshold + randomTerm;

        if( diff > 0 )
            state[i] = 1;
        else if( diff < 0 )
            state[i] = 0;
    }

    pthread_exit( NULL );
}

/*computeErrorFunction*/

int computeErrorFunction( int** mat, int rows, int cols, int* state ) {
    int errorFunction = 0;

    for( int j = 0; j < cols; j++ ) {
        int columnSum = 0;

        for( int i = 0; i < rows; i++ )
            columnSum += state[i] * mat[i][j];

        columnSum--;
        columnSum = columnSum * columnSum;
        errorFunction += columnSum;
    }

    return errorFunction;
}