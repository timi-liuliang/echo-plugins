/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_OHLGroup.h.h ( PI Library, C++)
 *
 * COMMENTS:
 *
 *	Abstraction for a group of PIs.
 */

#ifndef __PI_OHLGroup__
#define __PI_OHLGroup__

#include "PI_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>

class PI_Manager;
class PI_OHLPersistent;

class PI_API PI_OHLGroup
{
public:
    int			 getNumPIs() const;
    PI_OHLPersistent	*getPI(int index) const;

    bool		 contains(const PI_OHLPersistent *o) const;

    const UT_String	&getName() const;
    void		 setName(const char *name);

    void		 hidePIs() const;
    void		 exposePIs() const;

private:
	     PI_OHLGroup(PI_Manager *manager, const char *name);
    virtual ~PI_OHLGroup();

    // This function is for reordering the PIs. Only the PI_Manager should
    // call it. And the inputs must be guaranteed valid.
    void		 swapPIs(int idx1, int idx2);

    PI_Manager				*myManager;
    UT_String				 myName;
    UT_ValArray<PI_OHLPersistent *>	 myOpHandleLinks;

    friend class PI_Manager;
};

#endif

