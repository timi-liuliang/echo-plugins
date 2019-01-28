/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_Python.h (DOP Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __DOP_Python_h__
#define __DOP_Python_h__

#include "DOP_API.h"
#include "DOP_Node.h"
#include <SIM/SIM_ObjectArray.h>
class OP_OTLDefinition;
class OP_ScriptOperator;

class DOP_API DOP_Python : public DOP_Node
{
public:
    static OP_Node *myConstructor(
	OP_Network *net, const char *name, OP_Operator *entry);

    static OP_ScriptOperator *createScriptOperator(const OP_OTLDefinition &);

    // Returns whether or not this sop is currently processing objects.  It's
    // used by HOM to decide if it may modify dop data.
    bool isRunning() const
    { return myIsRunning; }

    const SIM_ObjectArray &objectsToProcess() const
    { return myObjectsToProcess; }

protected:
    DOP_Python(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~DOP_Python();

    virtual void processObjectsSubclass(
	fpreal time, int for_output_idx, const SIM_ObjectArray &objects,
	DOP_Engine &engine);

private:
    SIM_ObjectArray myObjectsToProcess;
    bool myIsRunning;
};

#endif
