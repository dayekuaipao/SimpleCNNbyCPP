#include "blob.h"
#include "mat.h"
#include "stb_image.h"
#include <cstring>

template <typename T> 
Blob<T>::Blob():size(0)
{
	data = new T[size];
	ref_count = new size_t(1);
}

template <typename T> 
Blob<T>::Blob(size_t size):size(size)
{
	data = new T[size];
	ref_count = new size_t(1);
}

template <typename T>
Blob<T>::~Blob()
{
	if(data!=nullptr&&--(*ref_count)==0)
	{
		delete[] data;
		data = nullptr;
		delete ref_count;
	}
}

template <typename T>
Blob<T>::Blob(const Blob<T>& other):data(other.data),size(other.size),ref_count(other.ref_count)
{
	++(*ref_count);
}

template <typename T>
Blob<T> Blob<T>::operator=(const Blob<T>& other)
{
	if(this!=&other)
	{
		if(data!=nullptr&&--(*ref_count)==0)
		{
			delete[] data;
			data = nullptr;
			delete ref_count;
		}
		data = other.data;
		size = other.size;
		ref_count = other.ref_count;
		++(*ref_count);
	}
	return *this;
}

Blob2d Blob2d::operator=(const Blob2d& other)
{	
	if(this!=&other)
	{
		static_cast<Blob1d&>(*this)=other;
		this->height = other.height;
		this->width = other.width;
	}
	return *this;
}

Blob3d Blob2d::reshape(size_t channel,size_t height,size_t width) const
{
	assert(channel*height*width==this->height*this->width);
	Blob3d output = Blob3d(channel,height,width);
	memcpy(output.data,data,size*sizeof(DATA_TYPE));
	return output;
}

Blob3d Blob3d::operator=(const Blob3d& other)
{	
	if(this!=&other)
	{
		static_cast<Blob1d&>(*this)=other;
		this->channel = other.channel;
		this->height = other.height;
		this->width = other.width;
	}
	return *this;
}

Blob2d Blob3d::reshape(size_t height,size_t width) const
{
	assert(height*width==this->height*this->width*this->channel);
	Blob2d output = Blob2d(height,width);
	memcpy(output.data,data,size*sizeof(DATA_TYPE));
	return output;
}

Blob3d imread(const char* path)
{
	int w,h,n;
	unsigned char* image = stbi_load(path,&w,&h,&n,3);
	Blob3d img = Blob3d(3,h,w);
	for(size_t i=0;i<img.channel;i++)
	{
		for(size_t j=0;j<img.height;j++)
		{
			for(size_t k=0;k<img.width;k++)
			{
				img(i,j,k) = image[j*img.width*img.channel+k*img.channel+i]/255.;
			}
		}	
	}
	stbi_image_free(image);
	return img;
}

template class Blob<unsigned char>;
template class Blob<short>;
template class Blob<int>;
template class Blob<float>;
template class Blob<double>;