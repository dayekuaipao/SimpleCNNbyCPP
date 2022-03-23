#ifndef CONV_BN_RELU_H_
#define CONV_BN_RELU_H_
#include "face_binary_cls.h"
#include "blob.h"

class ConvBnRelu{
public:
	ConvBnRelu()=default;
	ConvBnRelu(conv_param param);
	Blob3d forward(const Blob3d& input);
private:
	conv_param param;
};
#endif