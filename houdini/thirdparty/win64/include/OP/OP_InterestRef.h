/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_InterestRef.h (OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_INTERESTREF_H_INCLUDED__
#define __OP_INTERESTREF_H_INCLUDED__

#include "OP_API.h"
#include "OP_DataTypes.h"

class OP_Node;

class OP_API OP_InterestRef
{
public:
    struct EvalChannelTag	{};
    static EvalChannelTag	EvalChannel;

			OP_InterestRef()
			    : myNode(0)
			    , myParmIndex(-1)
			    , myParmSubIndex(-1)
			    , myInterestType(OP_INTEREST_DATA)
			{
			}

    explicit		OP_InterestRef(
				OP_Node &node,
				OP_InterestType type = OP_INTEREST_DATA)
			    : myNode(&node)
			    , myParmIndex(-1)
			    , myParmSubIndex(-1)
			    , myInterestType(type)
			{
			}

			// parm_sub_index may be -1 to indicate TYPE_PARM
    explicit		OP_InterestRef(
				OP_Node &node,
				int parm_index,
				int parm_sub_index)
			    : myNode(&node)
			    , myParmIndex(parm_index)
			    , myParmSubIndex(parm_sub_index)
			    , myInterestType(OP_INTEREST_DATA)
			{
			}

			OP_InterestRef(EvalChannelTag, int thread);

    OP_Node *		node() const		{ return myNode; }
    void		setNode(OP_Node *node)	{ myNode = node; }
    int			parmIndex() const	{ return myParmIndex; }
    void		setParmIndex(int pi)	{ myParmIndex = pi; }
    int			parmSubIndex() const	{ return myParmSubIndex; }
    void		setParmSubIndex(int vi)	{ myParmSubIndex = vi; }
    OP_InterestType	interestType() const	{ return myInterestType; }
    void		setInterestType(OP_InterestType t)
						{ myInterestType = t; }

    bool		isValid() const		{ return (myNode != 0); }

			enum Type
			{
			    TYPE_NONE,
			    TYPE_NODE,
			    TYPE_PARM,
			    TYPE_CHANNEL
			};

    Type		type() const
			{
			    if (myNode)
			    {
				if (myParmIndex >= 0)
				{
				    return (myParmSubIndex >= 0) ?
						TYPE_CHANNEL : TYPE_PARM;
				}
				return TYPE_NODE;
			    }
			    return TYPE_NONE;
			}

private:
    OP_Node *		myNode;
    int			myParmIndex;
    int			myParmSubIndex;
    OP_InterestType	myInterestType;
};

#endif // __OP_INTERESTREF_H_INCLUDED__
