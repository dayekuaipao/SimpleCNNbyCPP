#include "conv_bn_relu.h"
#include "operator.h"

ConvBnRelu::ConvBnRelu(conv_param param):param(param)
{

}

Blob3d ConvBnRelu::forward(const Blob3d& input)
{
	Blob3d output = conv(input,param);
	output = relu(output);
	return output;
}