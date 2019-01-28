/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Algorithm.h
 *
 * COMMENTS:
 *	Simply too hideous to be lurking about in RU_Algorithm. Uggh.
 *
 *	Leave now, while you still have eyes.
 */
#ifndef RU_ALGORITHM_MACROS_H
#define RU_ALGORITHM_MACROS_H

#include <SYS/SYS_Types.h>

#define DECLARE_GENERATOR_OP(name)		                              \
    virtual RU_GeneratorOp<unsigned char,0>  *allocateGenerator(unsigned char,int); \
    virtual RU_GeneratorOp<unsigned short,0> *allocateGenerator(unsigned short,int);\
    virtual RU_GeneratorOp<unsigned int,0>   *allocateGenerator(unsigned int,int);  \
    virtual RU_GeneratorOp<float,0>          *allocateGenerator(float,int); \
    virtual RU_GeneratorOp<fpreal16,0>     *allocateGenerator(fpreal16,int); \
    virtual RU_GeneratorOp<unsigned char,1>  *allocateGenerator(unsigned char); \
    virtual RU_GeneratorOp<unsigned short,1> *allocateGenerator(unsigned short);\
    virtual RU_GeneratorOp<unsigned int,1>   *allocateGenerator(unsigned int);      \
    virtual RU_GeneratorOp<float,1>          *allocateGenerator(float); \
    virtual RU_GeneratorOp<fpreal16,1>     *allocateGenerator(fpreal16);


#define DECLARE_PIXEL_OP(name)		                                   \
    virtual RU_PixelOp<unsigned char,0>  *allocatePixel(unsigned char,int);\
    virtual RU_PixelOp<unsigned short,0> *allocatePixel(unsigned short,int);\
    virtual RU_PixelOp<unsigned int,0>   *allocatePixel(unsigned int,int); \
    virtual RU_PixelOp<float,0>          *allocatePixel(float,int);	   \
    virtual RU_PixelOp<fpreal16,0>     *allocatePixel(fpreal16,int);   \
    virtual RU_PixelOp<unsigned char,1>  *allocatePixel(unsigned char);	   \
    virtual RU_PixelOp<unsigned short,1> *allocatePixel(unsigned short);   \
    virtual RU_PixelOp<unsigned int,1>	 *allocatePixel(unsigned int);     \
    virtual RU_PixelOp<float,1>          *allocatePixel(float);		   \
    virtual RU_PixelOp<fpreal16,1>     *allocatePixel(fpreal16);  

#define DECLARE_FILTER_OP(name)		                                   \
    virtual RU_FilterOp<unsigned char,0>  *allocateFilter(unsigned char,int);\
    virtual RU_FilterOp<unsigned short,0> *allocateFilter(unsigned short,int);\
    virtual RU_FilterOp<unsigned int,0>   *allocateFilter(unsigned int,int); \
    virtual RU_FilterOp<float,0>          *allocateFilter(float,int);	   \
    virtual RU_FilterOp<fpreal16,0>     *allocateFilter(fpreal16,int); \
    virtual RU_FilterOp<unsigned char,1>  *allocateFilter(unsigned char);  \
    virtual RU_FilterOp<unsigned short,1> *allocateFilter(unsigned short); \
    virtual RU_FilterOp<unsigned int,1>	  *allocateFilter(unsigned int);   \
    virtual RU_FilterOp<float,1>          *allocateFilter(float);	   \
    virtual RU_FilterOp<fpreal16,1>     *allocateFilter(fpreal16);


#define DECLARE_BINARY_OP(name)					\
    virtual RU_BinaryOp<unsigned char,0>  *allocateBinary(unsigned char,int); \
    virtual RU_BinaryOp<unsigned short,0> *allocateBinary(unsigned short,int);\
    virtual RU_BinaryOp<unsigned int,0>	  *allocateBinary(unsigned int,int);  \
    virtual RU_BinaryOp<float,0>	  *allocateBinary(float,int);	      \
    virtual RU_BinaryOp<fpreal16,0>     *allocateBinary(fpreal16, int);   \
    virtual RU_BinaryOp<unsigned char,1>  *allocateBinary(unsigned char);     \
    virtual RU_BinaryOp<unsigned short,1> *allocateBinary(unsigned short);    \
    virtual RU_BinaryOp<unsigned int,1>	  *allocateBinary(unsigned int);      \
    virtual RU_BinaryOp<float,1>	  *allocateBinary(float);	      \
    virtual RU_BinaryOp<fpreal16,1>     *allocateBinary(fpreal16);


#define IMPLEMENT_GENERATOR_OP(classname,name)	                             \
     RU_GeneratorOp<unsigned char,0>  *classname::allocateGenerator(unsigned char,int) \
		{ return new name<unsigned char,0>(this); }                   \
     RU_GeneratorOp<unsigned short,0> *classname::allocateGenerator(unsigned short,int)\
		{ return new name<unsigned short,0>(this); }                  \
     RU_GeneratorOp<unsigned int,0> *classname::allocateGenerator(unsigned int,int)  \
		{ return new name<unsigned int,0>(this); }                    \
     RU_GeneratorOp<float,0> *classname::allocateGenerator(float,int)         \
		{ return new name<float,0>(this); }			      \
     RU_GeneratorOp<fpreal16,0> *classname::allocateGenerator(fpreal16,int)\
		{ return new name<fpreal16,0>(this); } 	              \
     RU_GeneratorOp<unsigned char,1>  *classname::allocateGenerator(unsigned char) \
		{ return new name<unsigned char,1>(this); }                   \
     RU_GeneratorOp<unsigned short,1> *classname::allocateGenerator(unsigned short)\
		{ return new name<unsigned short,1>(this); }                  \
     RU_GeneratorOp<unsigned int,1>   *classname::allocateGenerator(unsigned int) \
		{ return new name<unsigned int,1>(this); }                    \
     RU_GeneratorOp<float,1> *classname::allocateGenerator(float)	      \
		{ return new name<float,1>(this); }			      \
     RU_GeneratorOp<fpreal16,1> *classname::allocateGenerator(fpreal16)   \
		{ return new name<fpreal16,1>(this); }


#define IMPLEMENT_PIXEL_OP(classname, name)				     \
     RU_PixelOp<unsigned char,0>  *classname::allocatePixel(unsigned char,int)\
		{ return new name<unsigned char,0>(this); }		     \
     RU_PixelOp<unsigned short,0> *classname::allocatePixel(unsigned short,int)\
		{ return new name<unsigned short,0>(this); }		     \
     RU_PixelOp<unsigned int,0>  *classname::allocatePixel(unsigned int,int) \
		{ return new name<unsigned int,0>(this); }		     \
     RU_PixelOp<float,0>       *classname::allocatePixel(float,int)	     \
		{ return new name<float,0>(this); }			     \
     RU_PixelOp<fpreal16,0>  *classname::allocatePixel(fpreal16,int)     \
		{ return new name<fpreal16,0>(this); }		     \
     RU_PixelOp<unsigned char,1>  *classname::allocatePixel(unsigned char)   \
		{ return new name<unsigned char,1>(this); }		     \
     RU_PixelOp<unsigned short,1> *classname::allocatePixel(unsigned short)  \
		{ return new name<unsigned short,1>(this); }		     \
     RU_PixelOp<unsigned int,1>	  *classname::allocatePixel(unsigned int)    \
		{ return new name<unsigned int,1>(this); }		     \
     RU_PixelOp<float,1>       *classname::allocatePixel(float)		     \
		{ return new name<float,1>(this); }			     \
     RU_PixelOp<fpreal16,1>  *classname::allocatePixel(fpreal16 )	     \
		{ return new name<fpreal16,1>(this); }


#define IMPLEMENT_FILTER_OP(classname, name)		                     \
     RU_FilterOp<unsigned char,0>  *classname::allocateFilter(unsigned char,int) \
		{ return new name<unsigned char,0>(this); }                  \
     RU_FilterOp<unsigned short,0> *classname::allocateFilter(unsigned short,int)\
		{ return new name<unsigned short,0>(this); }                 \
     RU_FilterOp<unsigned int,0>   *classname::allocateFilter(unsigned int,int)\
		{ return new name<unsigned int,0>(this); }                   \
     RU_FilterOp<float,0>          *classname::allocateFilter(float,int)     \
		{ return new name<float,0>(this); }			     \
     RU_FilterOp<fpreal16,0>     *classname::allocateFilter(fpreal16,int)\
		{ return new name<fpreal16,0>(this); }	             \
     RU_FilterOp<unsigned char,1>  *classname::allocateFilter(unsigned char) \
		{ return new name<unsigned char,1>(this); }                  \
     RU_FilterOp<unsigned short,1> *classname::allocateFilter(unsigned short)\
		{ return new name<unsigned short,1>(this); }                 \
     RU_FilterOp<unsigned int,1>   *classname::allocateFilter(unsigned int)  \
		{ return new name<unsigned int,1>(this); }                   \
     RU_FilterOp<float,1>          *classname::allocateFilter(float)         \
		{ return new name<float,1>(this); }			     \
     RU_FilterOp<fpreal16,1>     *classname::allocateFilter(fpreal16)    \
		{ return new name<fpreal16,1>(this); }


#define IMPLEMENT_BINARY_OP(classname, name)				\
     RU_BinaryOp<unsigned char,0> *classname::allocateBinary(unsigned char,int)\
	{ return new name<unsigned char,0>(this); }			\
     RU_BinaryOp<unsigned short,0> *classname::allocateBinary(unsigned short,int)\
	{ return new name<unsigned short,0>(this); }			\
     RU_BinaryOp<unsigned int,0> *classname::allocateBinary(unsigned int,int) \
	{ return new name<unsigned int,0>(this); }			\
     RU_BinaryOp<float,0> *classname::allocateBinary(float,int)		\
	{ return new name<float,0>(this); }				\
     RU_BinaryOp<fpreal16,0> *classname::allocateBinary(fpreal16,int)\
	{ return new name<fpreal16,0>(this); }			\
     RU_BinaryOp<unsigned char,1> *classname::allocateBinary(unsigned char)\
	{ return new name<unsigned char,1>(this); }			\
     RU_BinaryOp<unsigned short,1> *classname::allocateBinary(unsigned short)\
	{ return new name<unsigned short,1>(this); }			\
     RU_BinaryOp<unsigned int,1> *classname::allocateBinary(unsigned int)\
	{ return new name<unsigned int,1>(this); }			\
     RU_BinaryOp<float,1> *classname::allocateBinary(float)		\
	{ return new name<float,1>(this); }				\
     RU_BinaryOp<fpreal16,1> *classname::allocateBinary(fpreal16)	\
	{ return new name<fpreal16,1>(this); }			


#endif
