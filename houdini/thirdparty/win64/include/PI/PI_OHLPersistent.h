/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_OHLPersistent.h.h ( PI Library, C++)
 *
 * COMMENTS:
 *
 *	Abstraction for a persistent PI.
 */

#ifndef __PI_OHLPersistent__
#define __PI_OHLPersistent__

#include "PI_API.h"
#include <OP/OP_Value.h>
#include "PI_OpHandleLink.h"
#include "PI_SettingList.h"

class PI_Manager;

class BM_OpHandleLink;

class PI_API PI_OHLPersistent : public PI_OpHandleLink
{
public:
    const UT_String	&getName() const;
    void		 setName(const char *name);

    const UT_String	&getPaneMask() const;
    void		 setPaneMask(const char *mask);

    bool		 getExpose() const;
    void		 setExpose(bool expose);

    bool		 getDisplayInParent() const;
    void		 setDisplayInParent(bool display);
    bool		 getDisplayInChild() const;
    void		 setDisplayInChild(bool display);
    bool		 getDisplayInSibling() const;
    void		 setDisplayInSibling(bool display);
    bool		 getDisplayAlways() const;
    void		 setDisplayAlways(bool display);

    // This allows people to extract the BM handle link which owns this
    // persistent PI.  The OHLPersistent is never the MPI or IMP you
    // eventually define, but is owned by it.  However, the PI_Manager
    // only has a list of OHLs.  Thus this suspicious upcast.
    void		 setBMOwner(BM_OpHandleLink *bmowner)
			 { myBMOwner = bmowner; }
    BM_OpHandleLink	*getBMOwner() const { return myBMOwner; }

    virtual void	 setActiveColor(const UT_Color &c);

    const PI_SettingList &getSettingList() const { return mySettingList; }
	  PI_SettingList &getSettingList()	 { return mySettingList; }
    void		 setSettings(const char *setting_string);
    void		 clearSettingList() { mySettingList.clear(); }

    virtual int		 setEqual(const PI_OpHandleLink &pi);

protected:
    virtual void	 handleOpChange(OP_Node *op, OP_EventType etype,
					void *data);

private:
	     PI_OHLPersistent(PI_Manager *manager, const char *name,
			      const PI_PITemplate &templ);
    virtual ~PI_OHLPersistent();

    PI_Manager		*myManager;
    UT_String		 myName;
    UT_String		 myPaneMask;
    bool		 myExpose;

    bool		 myNetDisplayInParent;
    bool		 myNetDisplayInChild;
    bool		 myNetDisplayInSibling;
    bool 		 myNetDisplayAlways;

    PI_SettingList	 mySettingList;

    BM_OpHandleLink	*myBMOwner;

    friend class PI_Manager;
};

#endif

