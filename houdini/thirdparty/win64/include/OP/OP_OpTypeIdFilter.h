/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OpTypeIdFilter.h, OP Library (C++)
 *
 * COMMENTS:	This is a filter class which filters nodes by their
 *		operator type ids.
 *
 */

#ifndef __OP_OpTypeIdFilter_h__
#define __OP_OpTypeIdFilter_h__

#include "OP_API.h"

#include "OP_Node.h"

/// This is a filter class which filters nodes by their
/// operator type ids.
class OP_API OP_OpTypeIdFilter
{
public:

    /// Constructor.
    OP_OpTypeIdFilter();

    /// Copy constructor.
    OP_OpTypeIdFilter(const OP_OpTypeIdFilter &filter);

    /// Destructor.
    virtual ~OP_OpTypeIdFilter();

    /// Returns true if the given node is accepted by the filter.
    /// Returns false otherwise.
    bool			 isAcceptableNode(OP_Node *node) const;

    /// Returns true if the the filter has the operator type id.
    /// Returns false otherwise.
    bool			 hasOpTypeId(OP_OpTypeId opType) const;

    /// Sets whether we should be checking a network's child op type
    /// when determining if it is accepted by this filter.
    /// This logic is used in isAcceptableNode().
    void			 setCheckChildOpTypeId(bool doChildCheck)
				    { myDoChildCheck = doChildCheck; }

    /// Sets the mask for the operator type id.
    void			 setFilterMask(OP_OpTypeId opType,
					       bool doMask=true);

    /// Resets the filter by setting each mask to the reset value.
    void			 resetFilter(bool resetValue=true);

    /// Assignment operator.
    OP_OpTypeIdFilter		&operator=(const OP_OpTypeIdFilter &str);

private:

    /// Operator type id masks.
    /// If myOpTypeIdMasks[id] is true, then it means that nodes with
    /// "id" type should be kept, otherwise, they should be filtered out.
    bool			 myOpTypeIdMasks[NUM_MANAGERS];

    /// If true, then we also check a network's child op type
    /// when determining whether a node is accepted by this filter.
    bool			 myDoChildCheck;
};

#endif
