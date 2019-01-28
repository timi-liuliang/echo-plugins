/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	POP_BlindData.h ( POP Library, C++)
 *
 * COMMENTS:	This is a class which allows POPs to add "blind" data to the
 *		context.
 *
 *	Since POPs may be cooked in different contexts (i.e. the POP network
 *	may be referenced by multiple SOPs, or the viewer etc.), state
 *	dependent information should not be stored with the POP_Node sub-class.
 *	Instead, the POP_Node should add blind data to the context.  This can
 *	be used to store state information from cook-to-cook.
 *
 *	The POP_Node will typically have code like:
 *
 *	   class MyBlindData : public POP_BlindData {...};
 *
 *	   MyPop::cook(OP_Context &context) {
 *		POP_ContextData *cdata = (POP_ContextData *)context.getData();
 *		MyBlindData	*mydata;
 *
 *		mydata = cdata->getBlindData(this);
 *		if (!mydata)
 *		{
 *		    mydata = new MyBlindData(*this);
 *		    cdata->addBlindData(mydata);
 *		}
 *		    
 *	Since contexts are copied on occasion, it is important to implement the
 *	"duplicate" method.  The copy of the context data is typically stored
 *	as a "cache" version of the POP context.  For example, when the calling
 *	SOP says to cache N frames, there will be up to N context data's lying
 *	around.  Each one built for a different OP_Context.
 *
 *	NOTE:  This class is really only needed if the custom POP needs to keep
 *	around state information from frame to frame.  Most data can be safely
 *	stored with the POP.
 *
 *	When the simulation gets reset, all blind data is destroyed, and should
 *	be re-created on the next cook of the OP which needs it.
 */

#ifndef __POP_BlindData__
#define __POP_BlindData__

#include "POP_API.h"
class POP_Node;
class POP_ContextData;

class POP_API POP_BlindData {
public:
	     POP_BlindData(POP_Node *node);
    virtual ~POP_BlindData();

    //
    // The duplicate() method should create a new copy of the blind data.
    //  Please see above.
    // When the copy of the data is made, the source's owner can be used in the
    //  constructor.
    // The src POP_ContextData is the source context from which this blind data
    //  is being duplicated from.
    virtual POP_BlindData	*duplicate(const POP_ContextData *src) = 0;

    //
    // When the simulation get's reset, the following method is called.  If the
    // method returns 1, then the blind data will be deleted and reset inside
    // the context.  This can be used to detect a reset without having to
    // reconstruct expensive data structures on the next cook.
    // The default behaviour is to return 1 (i.e. delete itself on reset of
    // the simulation).
    virtual int			 resetSimulation();

    //
    // This is the owner of the blind data (i.e. the POP_Node which was passed
    // in the constructor.
    const POP_Node	*getOwner() const;
    POP_Node		*getOwner();
protected:

private:
    int			 myOwnerId;
};

#endif
