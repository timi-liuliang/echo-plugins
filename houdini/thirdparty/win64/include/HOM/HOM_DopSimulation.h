/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DopSimulation_h__
#define __HOM_DopSimulation_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include "HOM_IterableList.h"
#include <string>
#include <vector>

class HOM_Node;
class HOM_DopData;
class HOM_DopObject;
class HOM_DopRelationship;

SWIGOUT(%rename(DopSimulation) HOM_DopSimulation;)

class HOM_API HOM_DopSimulation
{
public:
    HOM_DopSimulation()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_DopSimulation(const HOM_DopSimulation &simulation)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_DopSimulation()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject createObject;)
    virtual HOM_DopData *createObject(const char *name, bool solve_on_creation_frame) = 0;

    virtual void removeObject(HOM_DopObject &obj) = 0;

    SWIGOUT(%newobject createRelationship;)
    virtual HOM_DopData *createRelationship(const char *name) = 0;

    virtual void removeRelationship(HOM_DopRelationship &rel) = 0;

    SWIGOUT(%newobject dopNetNode;)
    virtual HOM_Node *dopNetNode() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> > objects() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> > relationships() = 0;

    SWIGOUT(%newobject findObject;)
    virtual HOM_DopData *findObject(const char *obj_spec) = 0;
    SWIGOUT(%newobject findRelationship;)
    virtual HOM_DopData *findRelationship(const char *rel_spec) = 0;
    SWIGOUT(%newobject findData;)
    virtual HOM_DopData *findData(const char *data_spec) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> >
	findAllObjects(const char *obj_spec) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> >
	findAllRelationships(const char *rel_spec) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> >
	findAllData(const char *data_spec) = 0;

    virtual double time() = 0;

    virtual size_t memoryUsage() = 0;
};

#endif

