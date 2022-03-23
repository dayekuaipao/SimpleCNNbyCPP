#ifndef MAT_H_
#define MAT_H_
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef float DATA_TYPE;
#define NINF (-FLT_MAX)

typedef struct _Mat
{
    size_t rows;
    size_t cols;
    DATA_TYPE* data;
} Mat;
DATA_TYPE getElement(const Mat* mat,size_t row,size_t col);
DATA_TYPE* getElementPointer(Mat* mat,size_t row,size_t col);
const DATA_TYPE* getConstElementPointer(const Mat* mat,size_t row,size_t col);
Mat createMat(size_t rows,size_t cols);
void deleteMat(Mat* mat);
void copyMat(const Mat* a,Mat* b);

#ifdef __cplusplus
}
#endif
#endif