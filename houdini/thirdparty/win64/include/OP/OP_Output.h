/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Input.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _OP_Output_h_
#define _OP_Output_h_

#include "OP_API.h"
#include <UT/UT_Defines.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include "OP_ConnectorId.h"
#include "OP_DataTypes.h"

class OP_NetworkBoxItem;

class OP_API OP_Output
{
public:
     OP_Output(OP_ConnectorId& new_id);
    ~OP_Output();

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myOutputs.getMemoryUsage(false);
        return mem;
    }

    int getNumOutputs() const;
    OP_NetworkBoxItem *getOutput(int index);
    const OP_NetworkBoxItem *getOutput(int index) const;

    void appendOutput(OP_NetworkBoxItem *item);
    bool remove(OP_NetworkBoxItem *item);

    void getId(int& id);
    void setId(int id);

    OP_ConnectorId getId()
    { return myId; }

private:
    OP_NetworkBoxItemList	 myOutputs;
    OP_ConnectorId		 myId;
};

#endif

