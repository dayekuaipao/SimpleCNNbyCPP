#ifndef OPERATOR_H_
#define OPERATOR_H_
#include "blob.h"
#include "face_binary_cls.h"

Blob3d batch_norm(const Blob3d& input);
Blob3d relu(const Blob3d& input);
Blob1d relu(const Blob1d& input);
Blob3d max_pool(const Blob3d& input,size_t kernel_size = 2,size_t stride = 2,size_t pad = 0);
Blob1d softmax(const Blob1d& input);
Blob2d matmul(const Blob2d& a,const Blob2d& b);
Blob2d img2col(const Blob3d& img,const conv_param& param);
Blob3d conv(const Blob3d& input,const conv_param& param);
Mat blob2dToMat(const Blob2d& blob);
Blob2d matToBlob2d(const Mat& mat);
DATA_TYPE mean(Blob1d input);
#endif