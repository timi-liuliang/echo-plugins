/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DopData_h__
#define __HOM_DopData_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include "HOM_PtrOrNull.h"
#include "HOM_IterableList.h"
#include <string>
#include <vector>

class HOM_DopSimulation;
class HOM_DopRecord;

SWIGOUT(%rename(DopData) HOM_DopData;)

class HOM_API HOM_DopData
{
public:
    HOM_DopData()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_DopData(const HOM_DopData &data)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_DopData()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_DopData> data) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_DopData> data) = 0;

    SWIGOUT(%newobject freeze;)
    virtual HOM_DopData *freeze() = 0;
    virtual bool isFrozen() = 0;
    virtual std::string path() = 0;
    virtual std::string selectionPath() = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject dopNetNode;)
    virtual HOM_Node *dopNetNode() = 0;
    SWIGOUT(%newobject simulation;)
    virtual HOM_DopSimulation *simulation() = 0;
    SWIGOUT(%newobject creator;)
    virtual HOM_Node *creator() = 0;
    virtual std::string id() = 0;
    virtual std::string dataType() = 0;

    virtual std::vector<std::string> recordTypes() = 0;

    SWIGOUT(%newobject record;)
    virtual HOM_DopRecord *record(const char *record_type, int record_index = 0) = 0;

    SWIGOUT(%newobject records;)
    virtual HOM_IterableList<HOM_DopRecord> *records(const char *record_type) = 0;

    SWIGOUT(%newobject options;)
    virtual HOM_DopRecord *options() = 0;

    SWIGOUT(%newobject subData;)
    virtual std::map<std::string, HOM_ElemPtr<HOM_DopData> > subData() = 0;
    SWIGOUT(%newobject findSubData;)
    virtual HOM_DopData *findSubData(const char *data_spec) = 0;

    SWIGOUT(%kwargs findAllSubData;)
    virtual std::map<std::string, HOM_ElemPtr<HOM_DopData> >
	findAllSubData(const char *data_spec, bool recurse=false) = 0;


    SWIGOUT(%kwargs createSubData;)
    virtual HOM_DopData *createSubData(
	    const char *data_name, const char *data_type="SIM_EmptyData",
	    bool avoid_name_collisions=false) = 0;

    SWIGOUT(%kwargs attachSubData;)
    virtual void attachSubData(
	    HOM_DopData &hom_new_subdata, const char *new_data_name,
	    bool avoid_name_collisions=false) = 0;

    virtual void removeSubData(const char *data_spec) = 0;

    virtual void copyContentsFrom(HOM_DopData &hom_data) = 0;

    virtual void *_asVoidPointer() = 0;
};

#endif

