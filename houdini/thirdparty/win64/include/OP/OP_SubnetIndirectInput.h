/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_SubnetIndirectInput.h (C++)
 *
 * COMMENTS: OP_SubnetIndirectInput is used to maintain an indirect input
 *	for a sub-network. The input is a reference to one
 *	of the inputs to the owner of this node. The owner will be
 *	the sub-net.
 */

#ifndef _OP_SubnetIndirectInput_h_
#define _OP_SubnetIndirectInput_h_

#include "OP_API.h"
#include "OP_IndirectInput.h"
#include <iosfwd>

class OP_API OP_SubnetIndirectInput : public OP_IndirectInput
{
public:
    virtual OP_ItemType	 getItemType() const
			 { return OP_ITEMTYPE_INDIRECT; }
    virtual fpreal	 getW() const
			 { return 0.8; }
    virtual fpreal	 getH() const
			 { return 0.3; }
    virtual const UT_String	&getItemName() const;
    virtual bool		 setItemName(const UT_String &name);

    virtual OP_NetworkBoxItem *getInputItem() const;
    virtual int		 getInputItemOutputIndex() const;

    int			 getInputIndex() const
			 { return myInputIndex; }

    // Functions to get hip-file-unique ids for any item type.
    static OP_SubnetIndirectInput *lookupSubnetIndirectInput(int64 unique_id);
    int64		 getUniqueId() const;
    virtual int64	 getItemUniqueId() const
			 { return getUniqueId(); } 

    // Return the amount of memory owned by this OP_SubnetIndirectInput
    int64		 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += OP_IndirectInput::getMemoryUsage(false);
        return mem;
    }

private:
			 OP_SubnetIndirectInput(OP_Network &owner,
				 int input_index);
    virtual		~OP_SubnetIndirectInput();

    void		 initFromSubnetIndirect(
				const OP_SubnetIndirectInput &src);
    int			 save(std::ostream &os) const;
    bool		 load(UT_IStream &is);

    short		 myInputIndex;
    UT_String		 myName;

    friend class	 OP_Network;
};

#endif
