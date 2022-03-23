#include"mat.h"
#include <assert.h>
#include <string.h>

DATA_TYPE getElement(const Mat* mat,size_t row,size_t col)
{
    return mat->data[row*mat->cols+col];
}

DATA_TYPE* getElementPointer(Mat* mat,size_t row,size_t col)
{
    return &(mat->data[row*mat->cols+col]);
}

const DATA_TYPE* getConstElementPointer(const Mat* mat,size_t row,size_t col)
{
    return &(mat->data[row*mat->cols+col]);
}

Mat createMat(size_t rows,size_t cols)
{
    Mat mat;
	mat.rows = rows;
	mat.cols = cols;
	mat.data = (DATA_TYPE*)calloc(rows*cols,sizeof(DATA_TYPE));
    assert(mat.data!=NULL);
    return mat;
}

void deleteMat(Mat* mat)
{
	
    mat->cols = 0;
    mat->rows = 0;
    free(mat->data);
    mat->data=NULL;
}

void copyMat(const Mat* a,Mat* b)
{
	b->cols = a->cols;
	b->rows = a->rows;
	b->data = (DATA_TYPE*)realloc(b->data,a->cols*a->rows*sizeof(DATA_TYPE));
    assert(b->data!=NULL);
	memcpy(b->data, a->data, a->cols*a->rows*sizeof(DATA_TYPE));
}
