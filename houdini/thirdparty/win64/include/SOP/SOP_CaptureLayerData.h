/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __SOP_CaptureLayerData_h__
#define __SOP_CaptureLayerData_h__

#include "SOP_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_NTStreamUtil.h>
#include <ostream>

class OP_SaveFlags;

class SOP_API SOP_CaptureLayerData
{
public:
    SOP_CaptureLayerData();
    SOP_CaptureLayerData(SOP_CaptureLayerData &other);
    ~SOP_CaptureLayerData();

    int		 getStencilSize() const;
    void	 setStencilSize(int size);

    void	 clear();
    float	 getStencilValue(int snum, int ptnum) const;
    void	 setStencilValue(int snum, int ptnum, float value);
    int		 getNumStencils() const;
    void	 removeStencil(int snum);
    bool	 hasStencil(int snum) const
		 {
		     return (snum >= 0 && snum < myStencils.size())
			    ? (myStencils(snum) != nullptr) : false;
		 }

    int		 save(std::ostream &os, const OP_SaveFlags &flags,
		      const char *name, const char *path_prefix);
    int		 save(std::ostream &os, const OP_SaveFlags &flags,
		      const char *name, const char *path_prefix,
		      bool compress);
    bool	 load(UT_IStream &is, const char *ext);

private:
    int				 myStencilSize;
    UT_ValArray<float *>	 myStencils;
};

#endif
