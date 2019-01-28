/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        MSS_ReusableSelector.h ( Direct Manipulation Library, C++)
 *
 * COMMENTS:
 * 	Custom selector meant to create a new node only if one isn't already
 *	present. If the node exists, it reuses it.
 */

#ifndef __MSS_ReusableSelector_h__
#define __MSS_ReusableSelector_h__

#include "MSS_API.h"
#include <UT/UT_String.h>
#include <OP3D/OP3D_GenericSelector.h>

class MSS_API MSS_ReusableSelector : public OP3D_GenericSelector
{
public:
	     MSS_ReusableSelector(OP3D_View &viewer,
				  PI_SelectorTemplate &templ,
				  const char *sopname,
				  const char *groupparm = 0,
				  const char *grouptypeparm = 0,
				  bool allowblankgroup = false);
    virtual ~MSS_ReusableSelector();

    virtual const char	*className() const;

    // Generate special SOPs and Merge SOPs as necessary and fill in
    // group parameters in the SOPs to enable editing of the
    // selected geometry. edit_node is set to the current working SOP node.
    virtual bool	 connectSops(bool branch_off, bool request_new,
				     OP_Node *&edit_node);

    // If myInlineSelection is true, then we will set the selection into our
    // edit_node even if the selection is not on it in connectSops().
    void		 setInlineSelection(bool set_inline)
			    { myInlineSelection = set_inline; }
    bool		 getInlineSelection(bool set_inline)
			    { return myInlineSelection; }

    // Synchronize the current component type from the node.  This method is
    // intended to be called before startSelecting().
    void		 syncGeometryTypeFromNode(OP_Node *node);

protected:
    virtual void	 setGroupParm(OP_Node *node, const char *value);
    virtual void	 setGroupTypeParm(OP_Node *node, int value);

    /// Returns the name of the reusable SOP.
    UT_String		 getSopName()
			    { return mySopName; }

private:
    UT_String	 mySopName;
    UT_String	 myGroupParm;
    UT_String	 myGroupTypeParm;
    bool	 myAllowBlankGroup;
    bool	 myInlineSelection;
};

#endif
