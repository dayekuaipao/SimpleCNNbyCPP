#ifndef BLOB_H_
#define BLOB_H_
#include "mat.h"

template<typename T>
struct Blob{
	T* data;
	size_t* ref_count;
	size_t size;
	Blob();
	Blob(size_t size);
	Blob(const Blob<T>& other);
	Blob<T> operator=(const Blob<T>& other);
	virtual ~Blob();
};

typedef Blob<DATA_TYPE> Blob1d;
struct Blob3d;
struct Blob2d:Blob1d
{
	size_t height;
	size_t width;
	Blob2d():height(0),width(0),Blob1d(){};
	Blob2d(size_t height,size_t width):Blob1d(height*width),height(height),width(width){};
	Blob2d(const Blob2d& other):Blob1d(other),height(other.height),width(other.width){};
	Blob2d operator=(const Blob2d& other);
	DATA_TYPE& operator()(size_t i,size_t j) {assert(i<height&&j<width);return data[i*width+j];}
	DATA_TYPE at(size_t i,size_t j) const{assert(i<height&&j<width);return data[i*width+j];}
	DATA_TYPE zeroPaddingAt(size_t i,size_t j) const {return (i<height&&j<width)?at(i,j):0;}
	DATA_TYPE nInfPaddingAt(size_t i,size_t j) const {return (i<height&&j<width)?at(i,j):NINF;}
	Blob3d reshape(size_t channel,size_t height,size_t width) const;
	~Blob2d(){};
};

struct Blob3d:Blob1d
{
	size_t height;
	size_t width;
	size_t channel;
	Blob3d():channel(0),height(0),width(0),Blob1d(){};
	Blob3d(size_t channel,size_t height,size_t width):Blob(height*width*channel),channel(channel),height(height),width(width){};
	Blob3d(const Blob3d& other):Blob1d(other),channel(other.channel),height(other.height),width(other.width){};
	Blob3d operator=(const Blob3d& other);
	DATA_TYPE& operator()(size_t i,size_t j,size_t k) {assert(i<channel&&j<height&&k<width);return data[i*width*height+j*width+k];}
	DATA_TYPE at(ssize_t i,size_t j,size_t k) const{assert(i<channel&&j<height&&k<width);return data[i*width*height+j*width+k];}
	DATA_TYPE zeroPaddingAt(size_t i,size_t j,size_t k) const {return (i<channel&&j<height&&k<width)?at(i,j,k):0;}
	DATA_TYPE nInfPaddingAt(size_t i,size_t j,size_t k) const {return (i<channel&&j<height&&k<width)?at(i,j,k):NINF;}
	Blob2d reshape(size_t height,size_t width) const;
	~Blob3d(){}
};

Blob3d imread(const char* path);
#endif