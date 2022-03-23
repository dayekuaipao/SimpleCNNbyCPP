#include "simple_cls.h"
#include "blob.h"
#include "conv_bn_relu.h"
#include "linear.h"
#include "mat.h"
#include "operator.h"

SimpleCLS::SimpleCLS(conv_param conv_params[],fc_param fc_params[])
{
	for(size_t i=0;i<CONV_NUMS;i++)
	{
		conv_bn_relus[i] = ConvBnRelu(conv_params[i]);
	}
	for(size_t i=0;i<FC_NUMS;i++)
	{
		fcs[i] = Linear(fc_params[i]);
	}
}

Blob1d SimpleCLS::forward(const Blob3d& input)
{
	Blob3d output = input;
	for(size_t i=0;i<CONV_NUMS-1;i++)
	{
		output = conv_bn_relus[i].forward(output);	
		output = max_pool(output);
	}
	output = conv_bn_relus[CONV_NUMS-1].forward(output);
	Blob1d output2 = output;

	for(size_t i=0;i<FC_NUMS-1;i++)
	{
		output2 = fcs[i].forward(output2);
		output2 = relu(output2);
	}
	
	output2 = fcs[FC_NUMS-1].forward(output2);
	output2 = softmax(output2);
	return output2;
}