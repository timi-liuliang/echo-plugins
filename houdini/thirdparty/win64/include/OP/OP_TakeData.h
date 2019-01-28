/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_TakeData.h ( OP Library, C++)
 *
 * COMMENTS:	Take data for OPs
 */

#ifndef __OP_TakeData__
#define __OP_TakeData__

#include "OP_API.h"
#include <TAKE/TAKE_Data.h>

class OP_Node;

class OP_API OP_TakeData : public TAKE_Data 
{
public:
	     OP_TakeData(const TAKE_DataId &id, int priority);
    virtual ~OP_TakeData();

    virtual TAKE_Data	*duplicate() const = 0;

    virtual void	 getOpIdString(UT_String &str);
    virtual void	 applyData();
    virtual bool	 isValid() const;

    virtual void	 storeCurrent() = 0;
    virtual void	 generateCommand(UT_WorkBuffer &buf) = 0;
    virtual void	 getDescription(UT_WorkBuffer &buf, int brief) = 0;

    // We override the following method to make sure that take data is not
    // saved for locked assets in the master take.
    virtual bool	 canSaveInMasterTake() const;

protected:
    OP_Node		*lookupNode() const;
};
#endif

