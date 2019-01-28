/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_LayerOffset_h__
#define __HUSD_LayerOffset_h__

#include "HUSD_API.h"
#include <SYS/SYS_Types.h>

class HUSD_API HUSD_LayerOffset
{
public:
    explicit		 HUSD_LayerOffset(
				 fpreal64 offset = 0.0,
				 fpreal64 scale = 1.0)
			    : myOffset(offset),
			      myScale(scale)
			 { }

    fpreal64		 offset() const
			 { return myOffset; }
    void		 setOffset(fpreal64 offset)
			 { myOffset = offset; }
    fpreal64		 scale() const
			 { return myScale; }
    void		 setScale(fpreal64 scale)
			 { myScale = scale; }

    bool		 operator==(const HUSD_LayerOffset &other) const;
    bool		 isIdentity() const;
    bool		 isValid() const;
    HUSD_LayerOffset	 inverse() const;

private:
    fpreal64		 myOffset;
    fpreal64		 myScale;
};

#endif

