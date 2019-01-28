/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Take_h__
#define __HOM_Take_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <string>

class HOM_Node;
class HOM_ParmTuple;

SWIGOUT(%rename(Take) HOM_Take;)

class HOM_API HOM_Take
{
public:
    HOM_Take()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Take(const HOM_Take &event)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Take()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_Take> take) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_Take> take) = 0;

    virtual std::string __repr__() = 0;

    virtual bool isCurrent() = 0;

    virtual std::string name() = 0;
    virtual void setName(const char *name) = 0;

    SWIGOUT(%newobject parent;)
    virtual HOM_Take *parent() = 0;
    virtual std::string path() = 0;

    SWIGOUT(%newobject addChildTake;)
    SWIGOUT(%kwargs addChildTake;)
    virtual HOM_Take *addChildTake(const char *name=NULL) = 0;
    SWIGOUT(%newobject insertTakeAbove);
    SWIGOUT(%kwargs insertTakeAbove;)
    virtual HOM_Take *insertTakeAbove(const char *name=NULL) = 0;

    virtual void addNodeDisplayFlag(HOM_Node *node) = 0;
    virtual void removeNodeDisplayFlag(HOM_Node *node) = 0;
    virtual void addNodeBypassFlag(HOM_Node *node) = 0;
    virtual void removeNodeBypassFlag(HOM_Node *node) = 0;
    virtual void addNodeRenderFlag(HOM_Node *node) = 0;
    virtual void removeNodeRenderFlag(HOM_Node *node) = 0;

    virtual bool hasParmTuple(HOM_ParmTuple *parm_tuple) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_ParmTuple> > parmTuples() = 0;
    virtual void addParmTuple(HOM_ParmTuple *parm_tuple) = 0;
    virtual void removeParmTuple(HOM_ParmTuple *parm_tuple) = 0;
    SWIGOUT(%kwargs addParmTuplesFromTake;)
    virtual void addParmTuplesFromTake(HOM_Take *take, bool overwrite_existing=true) = 0;
    virtual void addParmTuplesFromNode(HOM_Node *node) = 0;
    virtual void removeParmTuplesFromNode(HOM_Node *node) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Take> > children() = 0;

    SWIGOUT(%kwargs saveToFile;)
    virtual void saveToFile(const char *filename, bool recurse=false) = 0;
    SWIGOUT(%newobject loadChildTakeFromFile);
    virtual HOM_Take *loadChildTakeFromFile(const char *filename) = 0;

    virtual void moveUnderTake(HOM_Take *take) = 0;

    SWIGOUT(%kwargs destroy;)
    virtual void destroy(bool recurse=false) = 0;
};

#endif
