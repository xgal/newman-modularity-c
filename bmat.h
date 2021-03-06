
#ifndef CPROJECT_UTILS_H
#define CPROJECT_UTILS_H

#include "spmat.h"

/**
@file bmat.h
**Date:**  18.9.2020
* Summary:
* This is the B matrix header file, contains the B matrix struct, and various functions for different calculations with B
 * we maintain a B matrix for each subgroup (community) of vertices
 * the struct 'bmat' is a struct containing all components of the B matrix from the theoretical modularity calculations
 * Functions:
 * readGraphB - reads the graph into a initial B matrix
 * allocateB - allocates a B struct
 * randomizeVec - returns a random vector
 * initOneValVec - initializes a vector, all values are equal to the value input of the function
 * error - returns an error based on the input (prints the error)
*/


/**
 * The B struct
 * @param sp : a pointer to the Sparse matrix associated with the B matrix
 * @param vecF : a vector containing the sum of colums for each row in the matrix
 * @param n : the size of the subgroup associated with B
 * @param M : the sum of all graph vertice ranks
 * @param shifting : the shifting value, we shift the matrix with this value to get positive eigenvalues
 * @function free : frees the struct
 * @function Bv : multiplies BxV
 * @function getBIterator - gets an iterator that iterates upon a specific row
 * @function iterHasNext - checks if the iterator can get a next value
 * @function getKPtr - returns a pointer to the K array (vertice rank to index)
 * @function updateFields - updates B struct fields
 * @function splitGraphB - splits a B struct into two subgroups, two new B matrices (after a split is found)
 * @function powerIter - Power Iteration function, to find leading eigenvector (function has an upper iteration limit)
 * @function eigenValue - function to calculate eigenvalue
 * @function modularityCalc - function to calculate matrix modularity
 */
typedef struct _bmat {
    spmat *sp;
    double *vecF;
    int n;
    int M;
    double shifting;


    /**
    * frees the B matrix allocated memory
    * @param B matrix
    */
    void (*free)(struct _bmat *B);

    /**
    * Outer function to be used outside of Bmat module, calls Bv multiplication function
    * @param B : the B matrix
    * @param vec : the vector to be multiplied
    * @param res : the result, saved
    */
    void (*Bv)(struct _bmat *B, double *vec, double *res);

    /**
    * Outer function to be used outside of Bmat module,
    * returns an iterator to iterate upon row values of a B matrix
    * @param B : B matrix
    * @param i : row index
    * @return  : pointer to first row value
    */
    int *(*getBIterator)(struct _bmat *B, int i);

    /**
    * iterates upon a row of B matrix
    * @param B : B matrix
    * @param i : row index
    * @param j  : the last index in the row
    * @param ptr : pointer to the current value
    * @return 1 if the iterator should move, zero o/w
    */
    int (*iterHasNext)(struct _bmat *B, int i, int j, int *ptr);

    /**
    * Outer function, returns pointer to K array (vertice ranks)
    * @param B : the B matrix
    * @return pointer to K array
    */
    int *(*getKPtr)(struct _bmat *B);

    /**
    * updates B struct fields (with vector F and shifting value)
    * @param B : B matrix
    * @param vecF : vector F (sum of columns)
    */
    void (*updateFields)(struct _bmat *B, double *vecF);

    /**
    * Splits a B struct into two subgroups, including inner implementation of A sparse matrix
    * @param currB :current B struct
    * @param s : division vector
    * @param g : current group
    * @param g1Size : new group 1 size
    * @param g2Size : new group 2 size
    * @return : a pointer to the second group, first group is saved in original pointer
    */
    struct _bmat *(*splitGraphB)(struct _bmat *B, double *s, int *g, int g1Size, int g2Size);

    /**
     * Power iteration method, multiplying a random vector with the matrix B, to find max eigenvector.
     * this method has a maximum iteration counter(5000*size + 80,000), when met an error will be given
     * @param B : the B matrix
     * @param b0  : initial random vector
     * @param result : the vector result of the power iteration
     * @return the power iteration result into the result vector
     */
    void (*powerIter)(struct _bmat *B, double *b0, double *result);

    /**
     * Calculates the eigenvalue of a vector
     * @param B : B matrix
     * @param vec : the eigenvector
     * @param tmp : a temporary vector to assist calculation
     * @return : the eigenvalue
     */
    double (*eigenValue)(struct _bmat *B, double *vec, double *tmp);

    /**
    * calculates the modularity of a division vector for a subgroup
    * @param B : B matrix
    * @param vec : the eigenvector
    * @param tmp : a temporary vector to assist calculation
    * @param vecF : an array containing the sum of values for each column
    * @return the modularity of the division
     */
    double (*modularityCalc)(struct _bmat *B, double *vec, double *tmp);

} BMat;

/**
 * reads into a initial B struct the input graph from the binary file
 * @param input : the input file pointer
 * @return a pointer to the bmat struct
 */
BMat *readGraphB(FILE *input);

/**
 * allocates a new B matrix
 * @return a pointer to the matrix
 */
BMat *allocateB();

/**
 * inserts random variables into an initialized vector
 * @param vec : the initialized vector
 * @param groupSize : the number of values inserted, inserted into the first indexes
 */
void randomizeVec(double *vec, int groupSize);

/**
 * initializes a vector with all values identical
 * @param unitVec : the vector to be initialized
 * @param n : the size of the vector
 * @param val : the value to be inserted
 */
void initOneValVec(double *unitVec, int n, int val);

/**
 *  A method to print the type of error event in runtime
 * @param errorCode : error to be printed
 */
void error(int errorCode);

#endif
