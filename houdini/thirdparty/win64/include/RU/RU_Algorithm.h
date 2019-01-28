/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Algorithm.h
 *
 * COMMENTS:
 *	Base classes for a templated Raster algorithm.
 */
#ifndef RU_ALGORITHM_H
#define RU_ALGORITHM_H

#include "RU_API.h"
#include "RU_AlgorithmMacros.h"
#include "RU_AlgorithmTemplates.h"

class TIL_TileList;
class TIL_Region;
class RU_Algorithm;


// Algorithm class manages the different ops. It should be used to contain
// parameter and static cook data.

class RU_API RU_Algorithm
{
public:
		 RU_Algorithm();
    virtual	~RU_Algorithm();

    // Algorithm Initialization
    //
    virtual int  initialize(float t);

    // Algorithm function types
    //
    int		generate   (TIL_TileList *output, float t,
			    void *ndata =0, int thread=-1, void *data=0);
    
    int		pixelAdjust(TIL_TileList *output,
			    const TIL_TileList *input,float t,
			    void *ndata =0, int thread=-1, void *data=0);
    
    int		filter     (TIL_TileList *output,
			    const TIL_Region *input, float t,
			    void *ndata =0, int thread=-1, void *data=0);

    int		filter	   (TIL_Region *output,
			    const TIL_Region *input, float t,
			    void *ndata =0, int thread=-1, void *data=0);
    
    int		binary     (TIL_TileList *output,
			    const TIL_Region *fore,
			    const TIL_Region *back, float t,
			    void *ndata =0, int thread=-1, void *data=0);

    int		binary     (TIL_Region *output,
			    const TIL_Region *fore,
			    const TIL_Region *back, float t,
			    void *ndata =0, int thread=-1, void *data=0);

    // Derived function allocators - at least one should be defined by
    // any derived algorithm. Any algorithm can define more than 1.
    // Pass the name of your class to the appropriate macro.
    //


    // Generators produce an output tile without any inputs.
    DECLARE_GENERATOR_OP(RU_GeneratorOp)

    // PixelOps produce an output tile with an input tile. Pixel ops cannot use
    // any pixel values other than themselves during the op. 
    DECLARE_PIXEL_OP(RU_PixelOp)

    // Filters produce an output tile using an input region. Operations may
    // use any pixel within the given region. 
    DECLARE_FILTER_OP(RU_FilterOp)

    // Binary ops take a background region input and any number of foreground
    // regions, and produce an output tile. The regions must be the same
    // format as the output.
    DECLARE_BINARY_OP(RU_BinaryOp)

    // call to check for an interrupt. You must pass the node data
    // and the thread index to generate/pixelAdjust/filter etc.. for this
    // to work.
    static bool interruptCheck(int thread);


    static void setInterruptCallback(bool (*cb)(void *, int));
    
private:
    static bool	(*copInterruptCheck)(void *node, int thread);
};




#endif
