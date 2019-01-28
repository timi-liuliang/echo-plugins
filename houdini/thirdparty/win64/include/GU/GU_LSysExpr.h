/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_LSysExpr.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_LSysExpr_h__
#define __GU_LSysExpr_h__

#include "GU_API.h"
//
// GU_LSysExpr:
//
//   class to contain variable values and labels
//   used in lsystems code	
//   variable labels are always 1 character, so 
//   fixed array is used
//


#include	<limits.h>

#include <UT/UT_SymbolTable.h>
#include <UT/UT_StringArray.h>

class	GU_Detail;
class	IMG_Raster;

#define	LSYS_EXPR_UNUSED	(FLT_MAX)
#define LSYS_MAX_PARAMS		5

class GU_API GU_LSysExpr
{
public:

    GU_LSysExpr();

    float       &operator[](int i) { 
				    if (i < 0) { i += 128; }
				    UT_ASSERT(i >= 0 && i < 256);
				    return val[i]; 
				   }

    int	 	 evalExpr(char *src, char *dest, 
			  float *var[LSYS_MAX_PARAMS]);

    float	 eval(char *str);

    IMG_Raster	*picRaster;		 // used in 'pic' function
    GU_Detail   *meta;			 // used in 'in' function
    float	(*getChanValue)(int ind, void *data); // 'chan' function
    void	*callbackData;

    const char  *getString(int idx) const;
    int		 addString(const char *str);

private:
    UT_SymbolMap<int> myStringIndices;
    UT_StringArray myStrings;
    
    float	 val[256];

};

#endif

