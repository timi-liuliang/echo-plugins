/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Breakpoint.h ( GA Library, C++)
 *
 * COMMENTS:	A simple class representing a breakpoint on a spline
 *          	(curve or surface).
 *
 */

#ifndef __GA_Breakpoint_h__
#define __GA_Breakpoint_h__

#include "GA_API.h"
#include "GA_Primitive.h"

class GA_LoadMap;
class GA_SaveMap;

class UT_JSONParser;
class UT_JSONWriter;


class GA_API GA_Breakpoint
{
public:
     GA_Breakpoint(const GA_Primitive *prim = 0, int uidx = -1, int vidx = -1)
	: mySpline(prim), myUidx(uidx), myVidx(vidx) {}
     GA_Breakpoint(const GA_Breakpoint &b)
	: mySpline(b.mySpline), myUidx(b.myUidx), myVidx(b.myVidx) {}
    ~GA_Breakpoint() {}

    bool		 operator==(const GA_Breakpoint &b) const
			 {
			     return (this == &b ||
				     (mySpline == b.mySpline	&&
				      myUidx   == b.myUidx	&&
				      myVidx   == b.myVidx	 ));
			 }

    bool		 isPrimary() const
			    { return mySpline ? mySpline->isPrimary() : false; }

    // Data Access Methods
    int			 u() const
			 { return myUidx; }
    int			 v() const
			 { return myVidx; }
    int			 getU() const
			 { return myUidx; }
    int			 getV() const
			 { return myVidx; }
    void		 setU(int u)
			 { myUidx = u; }
    void		 setV(int v)
			 { myVidx = v; }

    const GA_Primitive	*getSpline() const
			 { return mySpline; }
    void		 setSpline(const GA_Primitive *p)
			 { mySpline = p; } 

    /// Save data to a JSON stream.
    /// @section JSON-GA_Breakpoint JSON Schema: GA_Breakpoint
    /// @code
    /// {
    ///     "name"          : "GA_Breakpoint",
    ///     "description"   : "An tuple describing the breakpoint.
    ///                        (prim_number, uindex, vindex)",
    ///     "type"          : "array",
    ///     "items"         : "integer",
    ///     "minitems"      : 3,
    ///     "maxitems"      : 3,
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool		jsonSave(UT_JSONWriter &w, const GA_SaveMap &map) const;

    /// Load from a JSON stream
    bool		jsonLoad(UT_JSONParser &p, const GA_LoadMap &map);

private:
    const GA_Primitive	*mySpline;
    int			 myUidx, myVidx;
};

#endif
