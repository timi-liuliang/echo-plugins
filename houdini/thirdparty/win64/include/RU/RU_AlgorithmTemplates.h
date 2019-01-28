/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Algorithm.h
 *
 * COMMENTS:
 *	Base classes for a templated Raster algorithm.
 *
 *	*** DO NOT INCLUDE THIS FILE IN ANY HEADER FILE ***
 */
#ifndef RU_ALGORITHM_TEMPLATES_H
#define RU_ALGORITHM_TEMPLATES_H

class TIL_TileList;
class RU_Algorithm;
class TIL_Region;

template<class Type, int fast> class RU_GeneratorOp
{
public:
		 RU_GeneratorOp(RU_Algorithm *alg) : myAlg(alg) { ; }
    virtual	~RU_GeneratorOp()  { ; }

    virtual int	 generate(TIL_TileList * /*output*/, float /*t*/,
			  int /*thread*/, void * /*data*/) { return 0; }
    
protected:
    RU_Algorithm	*myAlg;
};


// Pixel OPs take a single tile input and produce a single tile output.
//
template<class Type, int fast> class RU_PixelOp
{
public:
		 RU_PixelOp(RU_Algorithm *alg) : myAlg(alg) { ; }
    virtual	~RU_PixelOp() { ; }
    
    virtual int	 pixelAdjust(TIL_TileList * /*output*/,
			     const TIL_TileList * /*input*/,
			     float /*t*/, int /*thread*/, void * /*data*/)
		{ return 0; }
    
protected:
    RU_Algorithm	*myAlg;
};

// Filters take a region input and produce a tile output.
//
template<class Type, int fast> class RU_FilterOp
{
public:
		 RU_FilterOp(RU_Algorithm *alg) : myAlg(alg) { ; }
    virtual	~RU_FilterOp() { ; }
    
    virtual int	 filter(TIL_TileList * /*output*/,
			const TIL_Region * /*input*/,
			float /*t*/, int /*thread*/, void * /*data*/)
		{ return 0; }

    virtual int	 filter(TIL_Region * /*output*/,
			const TIL_Region * /*input*/,
			float /*t*/,  int /*thread*/, void * /*data*/)
		{ return 0; }
			
    
protected:
    RU_Algorithm	*myAlg;
};


// Binary Ops take two tile inputs and produce a single tile output.
//
template<class Type,int fast> class RU_BinaryOp
{
public:
		 RU_BinaryOp(RU_Algorithm *alg) : myAlg(alg) { ; }
    virtual	~RU_BinaryOp() { ; }

    virtual int	 binary(TIL_TileList * /*output*/, const TIL_Region * /*fore*/,
			const TIL_Region * /*back*/, float /*t*/,
			int /*thread*/, void * /*data*/) { return 0; }
    
    virtual int	 binary(TIL_Region * /*output*/, const TIL_Region * /*fore*/,
			const TIL_Region * /*back*/, float /*t*/,
			int /*thread*/, void * /*data*/) { return 0; }
protected:
    RU_Algorithm	*myAlg;
};


#endif
