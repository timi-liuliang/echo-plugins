/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_AttribEvalGT.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_AttribEvalGT__
#define __RAY_AttribEvalGT__

#include "BRAY_API.h"
#include "BRAY_GTHandle.h"
#include <UT/UT_Array.h>
#include <GT/GT_Primitive.h>

class UT_JSONWriter;

/// Class to keep evaluation handles for GT primitives.  Note that the order of
/// the attributes evalautors in the list will be:
///	- getVertexAttributes()
///	- getPointAttributes()
///	- getUniformAttributes()
///	- getDetailAttributes()
/// So, when primitives build the BRAY_AttribList, the order should match
class BRAY_API BRAY_AttribEvalGT
{
public:
    BRAY_AttribEvalGT(const UT_Array<BRAY_GTHandle> &handles);
    ~BRAY_AttribEvalGT();

    static const BRAY_AttribEvalGT	*findEval(const GT_PrimitiveHandle &p);

    exint		 size() const { return myHandles.size(); }
    const BRAY_GTHandle	&handle(int i) const { return myHandles[i]; }
    const BRAY_GTHandle	&operator[](int i) const { return myHandles[i]; }
    void	dump() const;
    void	dump(UT_JSONWriter &s) const;

private:
    UT_Array<BRAY_GTHandle>	myHandles;
};

#endif
