/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_IndirectInput.h (C++)
 *
 * COMMENTS: OP_IndirectInput is used to represent a potential link into
 *	one or more node. It esseitnally allows an output connection to be
 *	forwarded to an input connection, which the input or output side
 *	may not exist.
 */

#ifndef _OP_IndirectInput_h_
#define _OP_IndirectInput_h_

#include "OP_API.h"
#include "OP_Value.h"
#include "OP_NetworkBoxItem.h"
#include <UT/UT_Array.h>
#include <UT/UT_Color.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class OP_Node;
class OP_Network;
class OP_PropagateData;

class OP_API OP_IndirectInput : public OP_NetworkBoxItem
{
public:
			 OP_IndirectInput(OP_Network &owner);
    virtual		~OP_IndirectInput();

    // Get the item attached directly to this indirect input. May be a node,
    // or another indirect input.
    virtual OP_NetworkBoxItem *getInputItem() const = 0;
    virtual int		 getInputItemOutputIndex() const = 0;

    // Get the Node that this input refers to.  May return NULL
    // Follows non-node inputs recursively to find a node.
    OP_Node		*getNode();
    int			 getNodeOutputIndex();

    // We maintain a list of output nodes so that we can do things to
    // them, like inform them when we're being deleted.
    unsigned		 getNOutputs() const
			 { return myOutputs.entries(); }
    OP_NetworkBoxItem	*getOutput(unsigned idx) const
			 { return (idx < getNOutputs()) ? myOutputs(idx) : 0; }
    void		 clearOutputs(bool for_delete = false);

    // Get all nodes connected to this indirect input. if an output is another
    // indirect input, follow its outputs recursively to find more nodes.
    void		 getOutputNodes(UT_Array<OP_Node*> &nodes,
					bool into_subnets = false) const;
    // Does a depth first search looking for the first node connected to
    // this indirect input.
    OP_Node		*getFirstOutputNode() const;

    //Return the network this item resides in (i.e. getOwner())
    virtual OP_Network	*getParentNetwork() const;
    OP_Network		*getOwner() const;

    virtual int		 setPicked(int on_off, 
				   bool propagate_parent_event = true);
    virtual int		 getPicked() const;

    // Position is used by the UI.
    virtual void	 setXY(fpreal x, fpreal y);
    virtual fpreal	 getX() const
			 { return myPosX; }
    virtual fpreal	 getY() const
			 { return myPosY; }

    // Flag to indicate that the color for this item should be fetched from
    // the first output. The set function returns true if the value changed.
    virtual bool	 setColor(const UT_Color &color);
    bool		 setUseColorFromOutput(bool use_output_color);
    bool		 getUseColorFromOutput() const;
    bool		 setSymbolicColorName(const UT_StringHolder &name);
    const UT_StringHolder &getSymbolicColorName() const;

    // The default color for indirect inputs.
    static UT_Color	 getDefaultColor();

    // Return the amount of memory owned by this OP_IndirectInput
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += OP_NetworkBoxItem::getMemoryUsage(false);
        mem += myOutputs.getMemoryUsage(false);
        mem += mySymbolicColorName.getMemoryUsage(false);
        return mem;
    }

protected:
    // Functions for saving and loading.
    void		 initFromIndirect(const OP_IndirectInput &src);
    void		 initFromFile(const UT_StringHolder &clrstr,
				const UT_StringHolder &netboxname,
				fpreal x,
				fpreal y);
    void		 saveColorString(std::ostream &os) const;

    // Send change notifications to our outputs.
    void		 propagateModification(OP_Node *by_whom,
					       OP_EventType reason,
					       int parm_index,
					       OP_PropagateData &prop_data);

private:
    void		 addOutput(OP_NetworkBoxItem *item);
    void		 delOutput(OP_NetworkBoxItem *item);

    OP_Network			&myOwner;
    UT_StringHolder		 mySymbolicColorName;
    OP_NetworkBoxItemList	 myOutputs;
    fpreal			 myPosX;
    fpreal			 myPosY;
    bool			 myPicked;
    bool			 myUseColorFromOutput;

    friend class		 OP_Input;
    friend class		 OP_Network;
};

#endif
