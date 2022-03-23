#ifndef LINEAR_H_
#define LINEAR_H_
#include "blob.h"
#include "face_binary_cls.h"
class Linear{
public:
	Linear()=default;
	Linear(fc_param param);
	Blob1d forward(const Blob1d& input);
private:
	fc_param param;
};
#endif