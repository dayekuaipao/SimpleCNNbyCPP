#include "operator.h"
#include "mat.h"
#include "matmul.h"
#include <cmath>

Blob3d batch_norm(const Blob3d& input)
{
	Blob3d output(input.channel,input.height,input.width);
	for(size_t i=0;i<input.channel;i++)
	{
		DATA_TYPE mean = 0;
		DATA_TYPE fatctor = 1.0/input.height/input.width;
		for(size_t j=0;j<input.height;j++)
		{
			for(size_t k=0;k<input.width;k++)
			{
				mean+=input.at(i,j,k)*fatctor;
			}
		}
		DATA_TYPE var = 0;
		for(size_t j=0;j<input.height;j++)
		{
			for(size_t k=0;k<input.width;k++)
			{
				var+=(input.at(i,j,k)-mean)*(input.at(i,j,k)-mean)*fatctor;
			}
		}

		for(size_t j=0;j<input.height;j++)
		{
			for(size_t k=0;k<input.width;k++)
			{
				output(i,j,k) = (input.at(i,j,k)-mean)/sqrt(var);
			}
		}
	}
	return output;
}

Blob3d relu(const Blob3d& input)
{
	Blob3d output(input.channel,input.height,input.width);
	memcpy(output.data,input.data,input.size*sizeof(DATA_TYPE));
	for(size_t i=0;i<output.size;i++)
	{
		if(output.data[i]<0)
		{
			output.data[i]=0;
		}
	}
	return output;
}

Blob1d relu(const Blob1d& input)
{
	Blob1d output(input.size);
	memcpy(output.data,input.data,input.size*sizeof(DATA_TYPE));
	for(size_t i=0;i<output.size;i++)
	{
		if(output.data[i]<0)
		{
			output.data[i]=0;
		}
	}
	return output;
}

Blob3d max_pool(const Blob3d& input,size_t kernel_size,size_t stride,size_t pad)
{
	size_t out_height = (input.height+2*pad-kernel_size)/stride+1;
	size_t out_width = (input.width+2*pad-kernel_size)/stride+1;
	Blob3d output(input.channel,out_height,out_width);
	for(size_t i=0;i<output.channel;i++)
	{
		for(size_t j=0;j<output.height;j++)
		{
			for(size_t k=0;k<output.width;k++)
			{
				DATA_TYPE max = input.nInfPaddingAt(i, j*stride-pad, k*stride-pad);
				for(size_t x=0;x<kernel_size;x++)
				{
					for(size_t y=0;y<kernel_size;y++)
					{
						if(input.nInfPaddingAt(i, j*stride-pad+x, k*stride-pad+y)>max)
						{
							max = input.nInfPaddingAt(i, j*stride-pad+x, k*stride-pad+y);
						}
					}
				}
				output(i,j,k) = max;
			}
		}
	}
	return output;
}

Blob1d softmax(const Blob1d& input)
{
	DATA_TYPE max = input.data[0];
	for(size_t i=1;i<input.size;i++)
	{
		if(max<input.data[i])
			max=input.data[i];
	}
	Blob1d output(input.size);
	DATA_TYPE sum;
	for(int i=0;i<input.size;i++)
	{
		sum += exp(input.data[i]-max);
	}
	for(int i=0;i<input.size;i++)
	{
		output.data[i] = exp(input.data[i]-max)/sum;
	}
	return output;
}

Mat blob2dToMat(const Blob2d& blob)
{
	Mat mat = createMat(blob.height,blob.width);
	memcpy(mat.data, blob.data, blob.size*sizeof(DATA_TYPE));
	return mat;
}

Blob2d matToBlob2d(const Mat& mat)
{
	Blob2d blob(mat.rows,mat.cols);
	memcpy(blob.data, mat.data, blob.size*sizeof(DATA_TYPE));
	return blob;
}

Blob2d matmul(const Blob2d& a,const Blob2d& b)
{
	Mat ma = blob2dToMat(a);
	Mat mb = blob2dToMat(b);
	Mat out = matmul_fast(&ma, &mb);
	Blob2d output = matToBlob2d(out);
	deleteMat(&ma);
	deleteMat(&mb);
	deleteMat(&out);
	return output;
}

Blob2d img2col(const Blob3d& img,const conv_param& param)
{
	size_t out_height = (img.height+param.pad*2-param.kernel_size)/param.stride+1;
	size_t out_width = (img.width+param.pad*2-param.kernel_size)/param.stride+1;
	Blob2d col(param.kernel_size*param.kernel_size*param.in_channels,out_height*out_width);
	for(size_t i=0;i<param.kernel_size*param.kernel_size*param.in_channels;i++)
	{
		for(size_t j=0;j<out_height*out_width;j++)
		{
			size_t c = i/(param.kernel_size*param.kernel_size);
			size_t dx = i%(param.kernel_size*param.kernel_size)/param.kernel_size;
			size_t dy = i%(param.kernel_size*param.kernel_size)%param.kernel_size;
			
			size_t ix = j/out_width;
			size_t iy = j%out_width;

			size_t sx = param.stride*ix-param.pad;
			size_t sy = param.stride*iy-param.pad;

			col(i,j) = img.zeroPaddingAt(c, sx+dx, sy+dy);
		}
	}
	return col;
}

Blob3d conv(const Blob3d& input,const conv_param& param)
{
	size_t out_height = (input.height+param.pad*2-param.kernel_size)/param.stride+1;
	size_t out_width = (input.width+param.pad*2-param.kernel_size)/param.stride+1;
	Blob2d in_col = img2col(input,param);

	Mat a;
	a.rows = param.out_channels;
	a.cols = param.kernel_size*param.kernel_size*param.in_channels;
	a.data = param.p_weight;

	Mat b;
	b.rows = in_col.height;
	b.cols = in_col.width;
	b.data = in_col.data;
	
	Mat result = matmul_fast(&a,&b);	

	Blob3d output = matToBlob2d(result).reshape(param.out_channels, out_height, out_width);

	for(size_t i=0;i<output.channel;i++)
	{
		for(size_t j=0;j<output.height;j++)
		{
			for(size_t k=0;k<output.width;k++)
			{
				output(i,j,k)+=param.p_bias[i];
			}
		}
	}

	deleteMat(&result);
	return output;
}

DATA_TYPE mean(Blob1d input)
{
	DATA_TYPE sum = 0;
	for(size_t i=0;i<input.size;i++)
	{

		sum+=input.data[i];
	}
	return sum/input.size;
}