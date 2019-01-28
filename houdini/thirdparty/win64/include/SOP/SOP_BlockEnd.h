/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BlockEnd.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_BlockEnd_h__
#define __SOP_BlockEnd_h__

#include "SOP_API.h"
#include "SOP_Node.h"

class UT_InfoTree;

class sop_PieceExtractor;

class SOP_API SOP_BlockEnd : public SOP_Node
{
public:
	     SOP_BlockEnd(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BlockEnd();

    virtual void	 PRIMARYPATH(UT_String &str, fpreal t) const = 0;

    // Determine all nodes or indirect inputs in this network level that
    // are logically contained by the for's feedback.
    // Not guaranteed to be accurate, as how things actually cook
    // may only be determined at run time.
    virtual void	 findEnclosedItems(OP_EnclosedItems &items) const;

    // We enforce that blocked pairs have the same color
    virtual bool    	 setColor(const UT_Color &col);

    // Should all inputs be treated as enclosed?
    virtual bool	 encloseInputs() const = 0;

protected:
    /// Find enclosed items that are our own ancestors, ignoring any
    /// linked outputs.
    void		 findOwnEnclosedItems(OP_EnclosedItems &items) const;
};

#endif
