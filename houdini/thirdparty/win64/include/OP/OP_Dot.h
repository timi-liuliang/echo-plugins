/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Dot.h (OP Library, C++)
 *
 * COMMENTS: A "dot" in a network that can have one input and many outputs.
 *	     It is simply a placeholder for an incomplete (or completed
 *	     connection between two nodes.
 */

#ifndef __OP_Dot__
#define __OP_Dot__

#include "OP_API.h"
#include "OP_Input.h"
#include "OP_IndirectInput.h"
#include <UT/UT_String.h>
#include <UT/UT_Array.h>

class OP_DotList;

class OP_API OP_Dot : public OP_IndirectInput
{
public:
    virtual OP_ItemType	 getItemType() const
			 { return OP_ITEMTYPE_DOT; }
    virtual fpreal	 getW() const
			 { return 0.0; }
    virtual fpreal	 getH() const
			 { return 0.0; }
    virtual const UT_String	&getItemName() const;
    virtual bool		 setItemName(const UT_String &name);

    // Establishes a connection to our input. May come from a node, a subnet
    // indirect input, or another dot.
    void		 setInput(OP_NetworkBoxItem *item,
				int outputidx,
				bool for_delete_old_input = false);
    OP_Input		*getInputReference();
    const OP_Input	*getInputReferenceConst() const;
    virtual OP_NetworkBoxItem *getInputItem() const;
    virtual int		 getInputItemOutputIndex() const;

    // Accessors for the "pinned" state of this dot. Pinned dots are treated
    // differently by the UI, but otherwise are the same as unpinned dots.
    // setPinned returns true if the value was changed.
    bool		 isPinned() const;
    bool		 setPinned(bool pinned);

    /// Functions to get hip-file-unique ids for any item type.
    static OP_Dot	*lookupDot(int id);
    int			 getUniqueId() const;
    virtual int64	 getItemUniqueId() const
			 { return getUniqueId(); }

    /// Returns the amount of memory owned by this OP_Dot
    /// (JUST this class; not subclasses)
    int64		 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += OP_IndirectInput::getMemoryUsage(false);
        return mem;
    }

private:
			 OP_Dot(OP_Network &parent,
				OP_DotList &dotlist,
				const char *name);
    virtual		~OP_Dot();

    void		 setUniqueId(int id);

    OP_DotList		&myDotList;
    OP_Input		*myInput;
    UT_String		 myName;
    int			 myUniqueId;
    bool		 myPinned;

    // The OP_DotList and OP_Input classes are responsible for managing the
    // connection references and the lifetime of dot objects.
    friend class	 OP_DotList;
};

#endif

