/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_SpecHandle_h__
#define __HUSD_SpecHandle_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_Array.h>

class HUSD_API HUSD_SpecHandle
{
public:
			 HUSD_SpecHandle();
			 HUSD_SpecHandle(const UT_StringHolder &identifier);
			 HUSD_SpecHandle(const UT_StringHolder &identifier,
				const UT_StringHolder &prim_path,
				const UT_StringHolder &prim_name);
			~HUSD_SpecHandle();

    const UT_StringHolder	&identifier() const
				 { return myIdentifier; }
    const UT_StringHolder	&path() const
				 { return myPrimPath; }
    const UT_StringHolder	&name() const
				 { return myPrimName; }

    UT_StringHolder	 getSpecType() const;

    void		 getChildren(UT_Array<HUSD_SpecHandle> &children) const;

private:
    UT_StringHolder	 myPrimPath;
    UT_StringHolder	 myPrimName;
    UT_StringHolder	 myIdentifier;
};

#endif

