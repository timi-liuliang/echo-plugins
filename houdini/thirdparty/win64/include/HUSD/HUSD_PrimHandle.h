/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_PrimHandle_h__
#define __HUSD_PrimHandle_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_Overrides.h"
#include "HUSD_Utils.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_ArrayStringSet.h>
#include <UT/UT_Array.h>
#include <UT/UT_Options.h>
#include <SYS/SYS_Inline.h>

enum HUSD_PrimAttribState {
    HUSD_FALSE,
    HUSD_TRUE,
    HUSD_ANIMATED_FALSE,
    HUSD_ANIMATED_TRUE,
    HUSD_OVERRIDDEN_FALSE,
    HUSD_OVERRIDDEN_TRUE,
    HUSD_NOTAPPLICABLE
};

SYS_FORCE_INLINE bool
HUSDstateAsBool(HUSD_PrimAttribState state)
{
    return (state == HUSD_TRUE ||
	    state == HUSD_OVERRIDDEN_TRUE ||
	    state == HUSD_ANIMATED_TRUE);
}

class HUSD_TimeCode;

// This class is a standalone wrapper around a specific primitice in a USD
// stage wrapped in an HUSD_DataHandle. It's purpose is to serve as the data
// accessor for tree nodes in the Scene Graph Tree. It should not be used for
// any other purpose, as it is extremely inefficient. Each function call locks
// the HUSD_DataHandle, queries its information, then unlocks it again. This
// is a matter of convenience for the calling pattern of the scene graph tree.
// Because it is inefficient the scene graph tree caches any information that
// comes out of this object.
//
// Anyone else tempted to use this object should use HUSD_Info instead.
class HUSD_API HUSD_PrimHandle
{
public:
			 HUSD_PrimHandle();
			 HUSD_PrimHandle(
				const HUSD_DataHandle &data_handle);
			 HUSD_PrimHandle(
				const HUSD_DataHandle &data_handle,
				const HUSD_ConstOverridesPtr &overrides);
			 HUSD_PrimHandle(
				const HUSD_DataHandle &data_handle,
				const UT_StringHolder &prim_path,
				const UT_StringHolder &prim_name);
			 HUSD_PrimHandle(
				const HUSD_DataHandle &data_handle,
				const HUSD_ConstOverridesPtr &overrides,
				const UT_StringHolder &prim_path,
				const UT_StringHolder &prim_name);
			~HUSD_PrimHandle();

    const HUSD_DataHandle	&dataHandle() const
				 { return myDataHandle; }
    const HUSD_ConstOverridesPtr&overrides() const
				 { return myOverrides; }
    const UT_StringHolder	&path() const
				 { return myPrimPath; }
    const UT_StringHolder	&name() const
				 { return myPrimName; }

    enum Classification {
	HASARCS,
	INSTANCE,
	INMASTER,
	NORMAL,
	ROOT,
	UNKNOWN
    };
    Classification	 getClassification() const;
    UT_StringHolder	 getPrimType() const;
    UT_StringHolder	 getVariantInfo() const;
    UT_StringHolder	 getKind() const;
    UT_StringHolder	 getDrawMode(bool *has_override = nullptr) const;
    HUSD_PrimAttribState getActive() const;
    HUSD_PrimAttribState getVisible(const HUSD_TimeCode &timecode) const;
    bool		 hasAnyOverrides() const;

    bool		 hasChildren(HUSD_PrimTraversalDemands demands) const;
    void		 getChildren(UT_Array<HUSD_PrimHandle> &children,
				HUSD_PrimTraversalDemands demands) const;

    void		 updateOverrides(
				const HUSD_ConstOverridesPtr &overrides);

    // Debugging only... Do not use in production code.
    void		 getAttributeNames(
				UT_ArrayStringSet &attrib_names) const;
    void		 extractAttributes(
				const UT_ArrayStringSet &which_attribs,
				const HUSD_TimeCode &tc,
				UT_Options &values);

private:
    HUSD_DataHandle		 myDataHandle;
    HUSD_ConstOverridesPtr	 myOverrides;
    UT_StringHolder		 myPrimPath;
    UT_StringHolder		 myPrimName;
};

#endif

