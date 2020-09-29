//
// Created by גל דאובר on 20/05/2020.
//

#include "Structs.h"
#include "RBTree.h"
#include "math.h"
#include <stdlib.h>
#include "string.h"
/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    return strcmp(a, b);

}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    strcat(pConcatenated, word);
    strcat(pConcatenated, "\n");
    if (pConcatenated != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    free(s);
}
/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *v1 = ((Vector *) a);
    Vector *v2 = (Vector *) b;
    int len;
    if (v1->len > v2->len)
    {
        len = v2->len;
    }
    else
    {
        len = v1->len;
    }
    for (int i = 0; i < len; i++)
    {
        if (v1->vector[i] != v2->vector[i])
        {
            if (v1->vector[i] > v2->vector[i])
            {
                return 1;
            }
            else if (v1->vector[i] < v2->vector[i])
            {
                return -1;
            }
        }
    }
    return v1->len - v2->len;

}
/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if (pVector == NULL)
    {
        return;
    }
    else
    {
        Vector *v1 = (Vector *) pVector;
        free(v1->vector);
        free(pVector);
    }

}
/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{

    Vector *v1 = ((Vector *) pVector);
    Vector *v2 = (Vector *) pMaxVector;
    int len1 = 0, len2 = 0;
    double norm1 = 0, norm2 = 0;
    double sum1 = 0, sum2 = 0;
    len1 = v1->len;
    len2 = v2->len;
    if (pVector == NULL)
    {
        return 1;
    }
    else if (v2->vector == NULL)
    {
        v2->len = len1;
        v2->vector = (double *) realloc(v2->vector, len1 * sizeof(double));
        for (int i = 0; i < v1->len; ++i)
        {
            v2->vector[i] = v1->vector[i];

        }
    }
    else
    {
        for (int i = 0; i < len1; i++)
        {

            sum1 += (v1->vector[i]) * (v1->vector[i]);
        }
        for (int i = 0; i < len2; i++)
        {

            sum2 += (v2->vector[i]) * (v2->vector[i]);
        }

        norm1 = sqrt(sum1);
        norm2 = sqrt(sum2);
        if (norm1 > norm2)
        {
            v2->len = len1;
            v2->vector = (double *) realloc(v2->vector, len1 * sizeof(double));
            for (int i = 0; i < v1->len; ++i)
            {
                v2->vector[i] = v1->vector[i];

            }
            return 1;

        }
    }
    return 1;


}
/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector *pMaxVector = (Vector *) calloc(1, sizeof(Vector));
    forEachRBTree(tree, copyIfNormIsLarger, pMaxVector);
    return pMaxVector;
}
