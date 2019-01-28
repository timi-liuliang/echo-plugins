/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_IntScale.h
 *
 * COMMENTS:
 *	Scales an image down by an integer factor.
 *	
 */
#ifndef TIL_INTSCALE_H
#define TIL_INTSCALE_H

#include "TIL_API.h"
#include "TIL_Defines.h"

class TIL_API TIL_IntScaleParms
{
public:
    TIL_DataFormat myType;
    unsigned int   myBlack, myWhite;
    
    const void	*mySource;
    int		 mySStride;
    
    const void  *myDest;
    int		 myDStride;
    int		 myDXOff;
    int		 myDYOff;

    int		 myFromXSize;
    int		 myFromYSize;
    int		 myReduction;

    TIL_IntScaleParms();
};

class TIL_API TIL_IntScale
{
public:
    static void	reduce(const TIL_IntScaleParms &parms);
};


#endif
