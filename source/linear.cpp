#include "linear.h"
#include "blob.h"
#include "mat.h"
#include "matmul.h"
#include "operator.h"

#include <iostream>
using namespace::std;

Linear::Linear(fc_param param):param(param)
{
	
}

Blob1d Linear::forward(const Blob1d& input)
{
	Mat a;
	a.rows = param.out_features;
	a.cols = param.in_features;
	a.data = param.p_weight;

	Mat b;
	b.rows = input.size;
	b.cols = 1;
	b.data = input.data;

	Mat c = matmul(&a, &b);
	Blob2d output = matToBlob2d(c);
	deleteMat(&c);
	for(size_t i=0;i<output.size;i++)
	{
		output.data[i] += param.p_bias[i];
	}
	return output;
}