#include <assert.h>
#include "mat.h"
#include "matmul.h"

Mat matmul(const Mat* a,const Mat* b)
{
    assert(a!=NULL&&b!=NULL);
    assert(a->data!=NULL&&b->data!=NULL);
    assert(a->cols==b->rows);
    Mat c = createMat(a->rows, b->cols);
    for(size_t i=0;i<c.rows;i++)
    {
        for(size_t j=0;j<c.cols;j++)
        {
            for(size_t k=0;k<a->cols;k++)
                (*getElementPointer(&c, i, j)) += getElement(a, i, k)*getElement(b, k, j);
        }
    }
    return c;
}

void AddDot1x4(size_t k,const DATA_TYPE* a, size_t lda,const DATA_TYPE* b, size_t ldb,DATA_TYPE* c, size_t ldc)
{  
    register DATA_TYPE 
        c_00_reg,   c_01_reg,   c_02_reg,   c_03_reg, 
        a_0p_reg,
        b_p0_reg,
        b_p1_reg,
        b_p2_reg,
        b_p3_reg;

    const DATA_TYPE 
        *a_0p_ptr, *b_p0_ptr; 


    c_00_reg = 0.0;   c_01_reg = 0.0;   c_02_reg = 0.0;   c_03_reg = 0.0;
    
    a_0p_ptr = a;

    for(size_t p=0;p<k;p++)
    {
        a_0p_reg = *a_0p_ptr++;

        b_p0_ptr = b+p*ldb;
        b_p0_reg = *b_p0_ptr++;
        b_p1_reg = *b_p0_ptr++;
        b_p2_reg = *b_p0_ptr++;
        b_p3_reg = *b_p0_ptr++;

        c_00_reg += a_0p_reg * b_p0_reg;
        c_01_reg += a_0p_reg * b_p1_reg;
        c_02_reg += a_0p_reg * b_p2_reg;
        c_03_reg += a_0p_reg * b_p3_reg;
    }
    
    *c += c_00_reg; *(c+1) += c_01_reg; *(c+2) += c_02_reg; *(c+3) += c_03_reg;
}

void AddDot4x1(size_t k,const DATA_TYPE* a, size_t lda,const DATA_TYPE* b, size_t ldb,DATA_TYPE* c, size_t ldc)
{  
    register DATA_TYPE 
        c_00_reg, 
        c_10_reg, 
        c_20_reg, 
        c_30_reg, 
        a_0p_reg,
        a_1p_reg,
        a_2p_reg,
        a_3p_reg,
        b_p0_reg;

    const DATA_TYPE 
        *a_0p_ptr, *a_1p_ptr, *a_2p_ptr, *a_3p_ptr, *b_p0_ptr; 


    c_00_reg = 0.0;   
    c_10_reg = 0.0;   
    c_20_reg = 0.0;   
    c_30_reg = 0.0;   
    
    a_0p_ptr = a;
    a_1p_ptr = a_0p_ptr+lda;
    a_2p_ptr = a_1p_ptr+lda;
    a_3p_ptr = a_2p_ptr+lda;

    for(size_t p=0;p<k;p++)
    {
        a_0p_reg = *a_0p_ptr++;
        a_1p_reg = *a_1p_ptr++;
        a_2p_reg = *a_2p_ptr++;
        a_3p_reg = *a_3p_ptr++;

        b_p0_ptr = b+p*ldb;
        b_p0_reg = *b_p0_ptr;

        c_00_reg += a_0p_reg * b_p0_reg;
        c_10_reg += a_1p_reg * b_p0_reg;
        c_20_reg += a_2p_reg * b_p0_reg;
        c_30_reg += a_3p_reg * b_p0_reg;
    }
    
    *c += c_00_reg;           
    *(c+ldc) += c_10_reg;     
    *(c+ldc*2) += c_20_reg;   
    *(c+ldc*3) += c_30_reg;   
}

void AddDot(size_t k,const DATA_TYPE* a, size_t lda,const DATA_TYPE* b, size_t ldb,DATA_TYPE* c, size_t ldc)
{  
    register DATA_TYPE c_reg,a_reg,b_reg;
    c_reg = 0.0;  
    for(size_t p=0;p<k;p++)
    {
        a_reg = *(a+p);
        b_reg = *(b+p*ldb);
        c_reg += a_reg * b_reg;
    }
    
    *c += c_reg; 
}

void AddDot4x4(size_t k,const DATA_TYPE* a, size_t lda,const DATA_TYPE* b, size_t ldb,DATA_TYPE* c, size_t ldc)
{  
    register DATA_TYPE 
        c_00_reg,   c_01_reg,   c_02_reg,   c_03_reg,  
        c_10_reg,   c_11_reg,   c_12_reg,   c_13_reg,  
        c_20_reg,   c_21_reg,   c_22_reg,   c_23_reg,  
        c_30_reg,   c_31_reg,   c_32_reg,   c_33_reg,
        a_0p_reg,
        a_1p_reg,
        a_2p_reg,
        a_3p_reg,
        b_p0_reg,
        b_p1_reg,
        b_p2_reg,
        b_p3_reg;

    const DATA_TYPE 
        *a_0p_ptr, *a_1p_ptr, *a_2p_ptr, *a_3p_ptr, *b_p_ptr; 


    c_00_reg = 0.0;   c_01_reg = 0.0;   c_02_reg = 0.0;   c_03_reg = 0.0;
    c_10_reg = 0.0;   c_11_reg = 0.0;   c_12_reg = 0.0;   c_13_reg = 0.0;
    c_20_reg = 0.0;   c_21_reg = 0.0;   c_22_reg = 0.0;   c_23_reg = 0.0;
    c_30_reg = 0.0;   c_31_reg = 0.0;   c_32_reg = 0.0;   c_33_reg = 0.0;
    
    a_0p_ptr = a;
    a_1p_ptr = a_0p_ptr+lda;
    a_2p_ptr = a_1p_ptr+lda;
    a_3p_ptr = a_2p_ptr+lda;

    for(size_t p=0;p<k;p++)
    {
        a_0p_reg = *a_0p_ptr++;
        a_1p_reg = *a_1p_ptr++;
        a_2p_reg = *a_2p_ptr++;
        a_3p_reg = *a_3p_ptr++;

        b_p_ptr = b+p*ldb;
        b_p0_reg = *b_p_ptr++;
        b_p1_reg = *b_p_ptr++;
        b_p2_reg = *b_p_ptr++;
        b_p3_reg = *b_p_ptr++;

        c_00_reg += a_0p_reg * b_p0_reg;
        c_10_reg += a_1p_reg * b_p0_reg;

        c_01_reg += a_0p_reg * b_p1_reg;
        c_11_reg += a_1p_reg * b_p1_reg;

        c_02_reg += a_0p_reg * b_p2_reg;
        c_12_reg += a_1p_reg * b_p2_reg;

        c_03_reg += a_0p_reg * b_p3_reg;
        c_13_reg += a_1p_reg * b_p3_reg;

        c_20_reg += a_2p_reg * b_p0_reg;
        c_30_reg += a_3p_reg * b_p0_reg;

        c_21_reg += a_2p_reg * b_p1_reg;
        c_31_reg += a_3p_reg * b_p1_reg;

        c_22_reg += a_2p_reg * b_p2_reg;
        c_32_reg += a_3p_reg * b_p2_reg;

        c_23_reg += a_2p_reg * b_p3_reg;
        c_33_reg += a_3p_reg * b_p3_reg;

    }
    
    *c += c_00_reg;           *(c+1) += c_01_reg;         *(c+2) += c_02_reg; *(c+3) += c_03_reg;
    *(c+ldc) += c_10_reg;     *(c+ldc+1) += c_11_reg;     *(c+ldc+2) += c_12_reg;     *(c+ldc+3) += c_13_reg;
    *(c+ldc*2) += c_20_reg;   *(c+ldc*2+1) += c_21_reg;   *(c+ldc*2+2) += c_22_reg;   *(c+ldc*2+3) += c_23_reg;
    *(c+ldc*3) += c_30_reg;   *(c+ldc*3+1) += c_31_reg;   *(c+ldc*3+2) += c_32_reg;   *(c+ldc*3+3) += c_33_reg;
}

Mat matmul_fast(const Mat* a,const Mat* b)
{
    assert(a!=NULL&&b!=NULL);
    assert(a->data!=NULL&&b->data!=NULL);
    assert(a->cols==b->rows);
    Mat c = createMat(a->rows, b->cols);
    size_t i = 0;
    for(;i<c.rows-c.rows%4;i+=4)
    {
        size_t j=0;
        for(;j<c.cols-c.cols%4;j+=4)
        {
            AddDot4x4(a->cols, getConstElementPointer(a, i, 0), a->cols, getConstElementPointer(b, 0, j), b->cols, getElementPointer(&c, i, j), c.cols);
        }
        for(;j<c.cols;j++)
        {
            AddDot4x1(a->cols, getConstElementPointer(a, i, 0), a->cols, getConstElementPointer(b, 0, j), b->cols, getElementPointer(&c, i, j), c.cols);
        }
    }
    for(;i<c.rows;i++)
    {
        size_t j=0;
        for(;j<c.cols-c.cols%4;j+=4)
        {
            AddDot1x4(a->cols,  getConstElementPointer(a, i, 0), a->cols, getConstElementPointer(b, 0, j), b->cols, getElementPointer(&c, i, j), c.cols);
        }
        for(;j<c.cols;j++)
        {
            AddDot(a->cols,  getConstElementPointer(a, i, 0), a->cols, getConstElementPointer(b, 0, j), b->cols, getElementPointer(&c, i, j), c.cols);
        }

    }
    return c;
}