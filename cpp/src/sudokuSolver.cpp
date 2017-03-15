#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
using namespace std;

#include "auxiliaryMacros.hpp"
#include "matrixExactCover.hpp"
#include "hopfieldExactCover.hpp"

#define BLOCK_SIZE 3
#define SUDOKU_SIZE (BLOCK_SIZE*BLOCK_SIZE)
#define SQUARES_OFFSET  0
#define ROWS_OFFSET     (SUDOKU_SIZE*SUDOKU_SIZE + SQUARES_OFFSET)
#define COLS_OFFSET     (SUDOKU_SIZE*SUDOKU_SIZE + ROWS_OFFSET)
#define BLOCKS_OFFSET   (SUDOKU_SIZE*SUDOKU_SIZE + COLS_OFFSET)

/* CommandLineArguments */

struct CommandLineArguments {
    vector<string> filenames;
    HopfieldExactCoverParams params;
};

/* prototypes */

void parseArguments( int argc, char* argv[], CommandLineArguments* args );
void printSudoku( int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] );
void loadSudoku( string filename, int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] );
bool solveSudoku( int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], HopfieldExactCoverParams* params );

/* main */

int main( int argc, char* argv[] ) {
    // use current time as seed for random generator
    srand( time( 0 ) );

    CommandLineArguments args;
    parseArguments( argc, argv, &args );

    int sudoku[SUDOKU_SIZE][SUDOKU_SIZE];

    vector<string>::iterator it;

    for( it = args.filenames.begin(); it != args.filenames.end(); ++it ) {

        cout << "Loading Sudoku puzzle from file: " << *it << endl;
        loadSudoku( *it , sudoku );

        cout << "Sudoku puzzle loaded" << endl;
        printSudoku( sudoku );

        bool solved = solveSudoku( sudoku, &( args.params ) );

        if( solved )
            cout << "Sudoku puzzle solved successfully" << endl;
        else
            cout << "Sudoku puzzle not solved" << endl;

        printSudoku( sudoku );
    }

    return 0;
}

/* parseArguments */

void parseArguments( int argc, char* argv[], CommandLineArguments* args ) {
    if( argc < 7 )
        EXIT_WITH_ERROR_MSG( "Expecting at least 6 arguments: NUM_THREADS NUM_ITERATIONS H_COEFF TH_COEFF TEMP_COEFF FILENAME1 [FILENAME2] ..." );

    args->params.numThreads = atoi( argv[1] );
    args->params.numIterations = atoi( argv[2] );
    args->params.hCoeff = atof( argv[3] );
    args->params.thCoeff = atof( argv[4] );
    args->params.tempCoeff = atof( argv[5] );

    for( int i = 6; i < argc; ++i )
        args->filenames.push_back( string( argv[i] ) );

    return;
}

/* printSudoku */

void printSudoku( int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] ) {
    for( int i = 0; i < SUDOKU_SIZE; ++i ) {
        if( i != 0 && i % BLOCK_SIZE == 0 ) {
            for( int k = 0; k < SUDOKU_SIZE + 2; ++k )
                cout << ". ";

            cout << endl;
        }

        for( int j = 0; j < SUDOKU_SIZE; ++j ) {
            if( j != 0 && j % BLOCK_SIZE == 0 )
                cout << ". ";

            cout << sudoku[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/* loadSudoku */

void loadSudoku( string filename, int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] ) {
    ifstream inputFile( filename.c_str() );

    if( !inputFile.is_open() )
        EXIT_WITH_ERROR_MSG( "Unable to open file" );

    int n, index = 0, row, col;

    while( inputFile.good() && index < SUDOKU_SIZE * SUDOKU_SIZE ) {
        inputFile >> n;

        if( !IN_RANGE( 0, n, 9 ) )
            EXIT_WITH_ERROR_MSG( "Incorrect file format" );

        row = index / SUDOKU_SIZE;
        col = index % SUDOKU_SIZE;
        sudoku[row][col] = n;
        index++;
    }

    inputFile.close();

    if( index != SUDOKU_SIZE * SUDOKU_SIZE )
        EXIT_WITH_ERROR_MSG( "Incorrect file format" );

    return;
}

/* solveSudoku */

void fillRow( int** mat, int matIndex, int row, int col, int digit ) {
    int digitOffset = digit - 1;
    int squareOffset = row * SUDOKU_SIZE + col;
    int rowOffset = row * SUDOKU_SIZE;
    int colOffset = col * SUDOKU_SIZE;

    int blockRow = row / BLOCK_SIZE, blockCol = col / BLOCK_SIZE, block = blockRow * BLOCK_SIZE + blockCol;
    int blockOffset = block * SUDOKU_SIZE;

    mat[matIndex][SQUARES_OFFSET + squareOffset] = 1;
    mat[matIndex][ROWS_OFFSET + rowOffset + digitOffset] = 1;
    mat[matIndex][COLS_OFFSET + colOffset + digitOffset] = 1;
    mat[matIndex][BLOCKS_OFFSET + blockOffset + digitOffset] = 1;

    return;
}

int** generateExactCoverMatrix( int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int* rows, int* cols ) {
    //compute matrix size and allocate
    int emptySquares = 0, takenSquares = 0;

    for( int i = 0; i < SUDOKU_SIZE; i++ )
        for( int j = 0; j < SUDOKU_SIZE; j++ )
            if( sudoku[i][j] == 0 ) emptySquares++;
            else takenSquares++;

    *rows = emptySquares * SUDOKU_SIZE + takenSquares;
    *cols = SUDOKU_SIZE * SUDOKU_SIZE * 4;
    int** mat = allocateMatrix( *rows, *cols );

    //fill matrix
    int rowAppendIndex = 0;

    for( int i = 0; i < SUDOKU_SIZE; i++ )
        for( int j = 0; j < SUDOKU_SIZE; j++ )
            if( sudoku[i][j] == 0 )
                for( int digit = 1; digit <= SUDOKU_SIZE; digit++ )
                    fillRow( mat, rowAppendIndex++, i, j, digit );
            else
                fillRow( mat, rowAppendIndex++, i, j, sudoku[i][j] );

    return mat;
}

void decodeRow( int* row, int* i, int* j, int* digit ) {
    for( int c = ROWS_OFFSET; c < COLS_OFFSET; c++ )
        if( row[c] == 1 ) {
            *i = ( c - ROWS_OFFSET ) / SUDOKU_SIZE;
            *digit = ( c - ROWS_OFFSET ) % SUDOKU_SIZE + 1;
            break;
        }

    for( int c = COLS_OFFSET; c < BLOCKS_OFFSET; c++ )
        if( row[c] == 1 ) {
            *j = ( c - COLS_OFFSET ) / SUDOKU_SIZE;
            break;
        }
}

void fillSolvedSudoku( int** mat, int rows, int cols, int solution[], int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] ) {
    for( int r = 0; r < rows; ++r ) {
        if( solution[r] == 0 ) continue;

        int i, j, digit;
        decodeRow( mat[r], &i, &j, &digit );
        sudoku[i][j] = digit;
    }

    return;
}

bool solveSudoku( int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], HopfieldExactCoverParams* params ) {
    int rows, cols;
    int** exactCoverMat = generateExactCoverMatrix( sudoku, &rows, &cols );

    int solution[rows];
    bool solved = solveExactCover( exactCoverMat, rows, cols, solution, params );

    fillSolvedSudoku( exactCoverMat, rows, cols, solution, sudoku );

    deallocateMatrix( exactCoverMat, rows );

    return solved;
}
