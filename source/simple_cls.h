#ifndef SIMPLE_CLS_H_
#define SIMPLE_CLS_H_
#include "blob.h"
#include "conv_bn_relu.h"
#include "face_binary_cls.h"
#include "linear.h"

#define CONV_NUMS 3
#define FC_NUMS 1

class SimpleCLS{
public:
	SimpleCLS(conv_param conv_params[],fc_param fc_param[]);
	Blob1d forward(const Blob3d& input);
private:
	ConvBnRelu conv_bn_relus[CONV_NUMS];
	Linear fcs[FC_NUMS];

};

#endif