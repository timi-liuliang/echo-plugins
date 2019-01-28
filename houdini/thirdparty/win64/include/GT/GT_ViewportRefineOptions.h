/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_ViewportRefineOptions.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_ViewportRefineOptions__
#define __GT_ViewportRefineOptions__

#include "GT_API.h"

/// This class is used to inform the viewport (GR) rendering code whether
/// changes in display options will affect the tesselation of primitives.
class GT_API GT_ViewportRefineOptions
{
public:
    // New options will be added to the end of the parameter list
    GT_ViewportRefineOptions(
		bool changes_with_lod = false
	)
	: myChangesWithLOD(changes_with_lod)
    {
    }
    ~GT_ViewportRefineOptions();

    /// @{
    /// Changes to LOD in the display options will affect refinement of
    /// primitives.
    bool	changesWithLOD() const		{ return myChangesWithLOD; }
    void	setChangesWithLOD(bool v)	{ myChangesWithLOD = v; }
    /// @}

private:
    bool	myChangesWithLOD;
};

#endif
