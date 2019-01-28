/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveTypeId.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveTypeId__
#define __GA_PrimitiveTypeId__

#include "GA_API.h"

#include <SYS/SYS_Inline.h>

/// Each primitive type is given a unique ID at run time.  There are a number
/// of pre-defined factory types (see GEO_PrimType.h)
class GA_API GA_PrimitiveTypeId 
{
public:
    /// Default c-tor
    SYS_FORCE_INLINE
    GA_PrimitiveTypeId()
	: myId(-1)
    {}
    /// Copy c-tor
    SYS_FORCE_INLINE
    GA_PrimitiveTypeId(const GA_PrimitiveTypeId &src)
	: myId(src.myId)
    {}
    /// Assign from a factory type
    SYS_FORCE_INLINE
    GA_PrimitiveTypeId(int id)
	: myId(id)
    {}
    /// Destructor
    SYS_FORCE_INLINE
    ~GA_PrimitiveTypeId()
    {}

    /// Assignment operator
    SYS_FORCE_INLINE
    GA_PrimitiveTypeId &operator=(const GA_PrimitiveTypeId &src)
			{
			    myId = src.myId;
			    return *this;
			}

    /// Return the integer value for the identifier.  This can be used in
    /// switches for factory types.
    SYS_FORCE_INLINE
    int		get() const	{ return myId; }

    /// Returns true if this is a valid primitive id.
    SYS_FORCE_INLINE
    bool	isValid() const { return myId >= 0; }

    /// @{
    /// Compare the id to a "factory" primitive type (see GEO_PrimType.h)
    SYS_FORCE_INLINE
    bool	operator==(int factory_id) const
		    { return myId == factory_id; }
    SYS_FORCE_INLINE
    bool	operator!=(int factory_id) const
		    { return myId != factory_id; }
    /// @}
    /// @{
    /// Compare to other primitive id's
    SYS_FORCE_INLINE
    bool	operator==(const GA_PrimitiveTypeId &src) const
		    { return myId == src.myId; }
    SYS_FORCE_INLINE
    bool	operator!=(const GA_PrimitiveTypeId &src) const
		    { return myId != src.myId; }
    /// @}

private:
    SYS_FORCE_INLINE
    void	set(int id)	{ myId = id; }

    int		myId;
};

#endif
