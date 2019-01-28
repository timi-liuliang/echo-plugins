/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_ParmIterator.h ( Operator Library, C++)
 *
 * COMMENTS:
 *		This class implements an object that can be used to iterate
 *		through parms (and their subindices) for a given node.
 */
#ifndef __OP_ParmIterator_h__
#define __OP_ParmIterator_h__

#include "OP_API.h"
#include <UT/UT_String.h>

class PRM_Parm;
class OP_Parameters;

enum OP_ParmIteratorMask
{
    OP_PIM_NONE		    = 0x00,	    // No parms will pass this
    OP_PIM_ORDINAL	    = 0x01,	    // ordinal parms
    OP_PIM_STRING	    = 0x02,	    // string parms
    OP_PIM_ANIMATED	    = 0x04,	    // Animatable parms
    OP_PIM_CHANNELS	    = 0x08,	    // Parms with CH_Channels present
    OP_PIM_AUTOSCOPED	    = 0x10,	    // Parm with the auto-scope flag
    OP_PIM_ALL		    = 0xff
};

enum OP_ParmIteratorExcludeMask
{
    OP_PIEM_NONE	    = 0x00,	    // No exclusions
    OP_PIEM_LOCKED	    = 0x01,	    // Exclude locked parms
    OP_PIEM_ALL		    = 0x0f
};

inline OP_ParmIteratorMask 
operator|(OP_ParmIteratorMask a, OP_ParmIteratorMask b)
{
    return (OP_ParmIteratorMask)((unsigned int)a | (unsigned int)b);
}

inline OP_ParmIteratorExcludeMask 
operator|(OP_ParmIteratorExcludeMask a, OP_ParmIteratorExcludeMask b)
{
    return (OP_ParmIteratorExcludeMask)((unsigned int)a | (unsigned int)b);
}

class OP_API OP_ParmIterator
{
public:
	        OP_ParmIterator(OP_Parameters &node, 
				OP_ParmIteratorMask mask = OP_PIM_ALL,
				OP_ParmIteratorExcludeMask exclude_mask =
								OP_PIEM_NONE);
	       ~OP_ParmIterator();

    // Returns true when the iterator has finished iterating through all
    // the parameters.
    bool	atEnd() const  { return myAtEnd; }

    // This methods gets the name of the next channel in the list.  The reason 
    // this is called getNextChannel because this method returns names of 
    // channels not parm names.  This does not imply that the name returned will
    // actually have a CH_Channel associated with it.
    // It will return true if successful and false otherwise.  (It shouldn't
    // fail if you check for atEnd before you call it).
    bool	getNextChannel(UT_String &channame, UT_String *chanalias=NULL);

    // This methods gets the parm pointer and subindex of the next one in the
    // list.
    bool	getNextParm(PRM_Parm * &parm, int &vi);

    // This method will return true if the parm passes the mask in this
    // iterator. 
    bool		     passesMasks(PRM_Parm *parm, int subindex);
protected:

    void	scrollToNext(bool first_time);


private:
    OP_Parameters		&myNode;
    OP_ParmIteratorMask		 myMask;
    OP_ParmIteratorExcludeMask	 myExcludeMask;
    bool			 myAtEnd;
    int				 myCurrentParmIndex;
    int				 myCurrentParmSubIndex;
    UT_String			 myCurrentToken;
    UT_String			 myCurrentAlias;
};

#endif
