/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Handle.h (SOP library, C++)
 *
 * COMMENTS:
 *		This class will be the interface between sop parameters
 *		and sage's viewdata. It should be abstracted at the
 *		UI level, but I'll put it here in SOP for now.
 *
 *		IDEA:
 *			There will be some guide geometry associated with
 *			this handle. Whenever the user clicks on the guide
 *			geometry in the ViewData and drags, doDrag will be 
 *			called with the deltax and deltay passed in.
 *			The member function will make appropriate changes to
 *			the sop parameters.
 *
 */

#ifndef __SOP_Handle__
#define __SOP_Handle__

#include "SOP_API.h"
class PRM_ParmList;

class SOP_API SOP_Handle
{
public:
    SOP_Handle();
    virtual ~SOP_Handle() {}

    virtual void	 doDrag(int modifierkeys, int mousebutton,
				int dx, int dy);
private:
    void		 doXY(int dx, int dy);
    PRM_ParmList	*myParmListPtr;
};

#endif
