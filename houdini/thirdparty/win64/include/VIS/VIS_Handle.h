/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VIS_Handle.h ( GR Library, C++)
 *
 * COMMENTS:
 *      A VIS_Mount specifies the location, scopy and other local settings of
 *      a visualizer.
 */

#ifndef __VIS_Handle__
#define __VIS_Handle__

#include "VIS_API.h"
#include "VIS_Defines.h"
#include "VIS_Mount.h"
#include <SYS/SYS_Hash.h>

class PRM_ParmList;
class VIS_Visualizer;
class VIS_Handle;

VIS_API extern VIS_Mount *VISlookupHandleMount(VIS_Handle vh);


class VIS_API VIS_Handle
{
public:
    VIS_Handle(int unique_id = -1) :
	myUniqueId(unique_id)
    {}

    bool 		 isValid()
			    { return getMount(); }

    inline bool 	 operator==(const VIS_Handle vh) const
			    { return myUniqueId == vh.myUniqueId; }

    inline bool 	 operator!=(const VIS_Handle vh) const
			    { return myUniqueId != vh.myUniqueId; }


    /// Returns a the mount's unique id (this doesn't change if the
    /// mounted visualizer is substituted).
    int 		 getUniqueId() const;

    /// Returns the type id of the mounted visualizer.
    int			 getTypeId();

    /// returns the parameter list of the mounted visualizer.
    PRM_ParmList 	*getParmList();

    /// Returns the mounted visualizer.
    VIS_Visualizer	*getVisualizer();

    /// Returns the serial number of the mounted visualizer.
    int			 getSerialNum();

    /// Returns the table ref of the VIS_Tables that owns the mount.
    VIS_TableRef	 getTableRef() const;

    /// Returns the index of the viuslaizer in the owning VIS_Table.
    int 		 getIndex() const;
    const char		*getName() const;
    const char		*getLabel() const;
    const char		*getIcon() const;

    bool 		 isActiveWhenUnselected() const;
    bool 		 isActiveWhenSelected() const;
    bool 		 isActiveWhenGhosted() const;
    bool 		 isActiveWhenDisplayed() const;
    bool 		 isActiveWhenCurrent() const;
    bool 		 isActiveWhenTemplated() const;

    bool		 showInToolbar() const;

    VIS_VisualizerScope  getScope() const;
    inline VIS_Mount	*getMount() const
			    { return VIS_Mount::lookupHandleMount(*this); }
private:

    int			 myUniqueId;
};

inline size_t hash_value(const VIS_Handle &vh)
{
    return SYShash(vh.getUniqueId());
}

#define VIS_INVALID_HANDLE	VIS_Handle(-1)


#endif
