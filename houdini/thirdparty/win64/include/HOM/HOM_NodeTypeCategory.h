/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NodeTypeCategory_h__
#define __HOM_NodeTypeCategory_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_EnumValue.h"
#include "HOM_Color.h"
#include "HOM_ElemPtr.h"
#include "HOM_PtrOrNull.h"
#include <map>

class HOM_NodeType;
class HOM_SopVerb;
class HOM_ViewerState;

SWIGOUT(%rename(NodeTypeCategory) HOM_NodeTypeCategory;)

class HOM_API HOM_NodeTypeCategory
{
public:
    virtual ~HOM_NodeTypeCategory()
    {}

    // Because the only instances of NodeTypeCategory objects are the ones
    // created by the HOM_Module::*NodeTypeCategory() methods, the comparison
    // operators can just compare pointers.
    bool operator==(HOM_PtrOrNull<HOM_NodeTypeCategory> category)
    { return this == category.myPointer; }

    bool operator!=(HOM_PtrOrNull<HOM_NodeTypeCategory> category)
    { return !operator==(category); }


    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual std::string name() const = 0;

    virtual std::string typeName() const = 0;
    
    virtual std::string label() const = 0;

    virtual std::map<std::string, HOM_ElemPtr<HOM_NodeType> > nodeTypes() = 0;
    SWIGOUT(%newobject nodeType;)
    virtual HOM_NodeType *nodeType(const char *type_name) = 0;

    virtual std::map<std::string, HOM_ElemPtr<HOM_SopVerb> > nodeVerbs() = 0;
    SWIGOUT(%newobject nodeVerb;)
    virtual HOM_SopVerb *nodeVerb(const char *verb_name) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ViewerState> > viewerStates(
	    HOM_EnumValue &viewer_type) = 0;

    virtual void loadDSO(const char *dso_path) = 0;

    virtual bool hasSubNetworkType() = 0;

    virtual std::string defaultShape() = 0;
    virtual void clearDefaultShapes() = 0;
    virtual void setDefaultShape(const char *shape) = 0;
    virtual HOM_Color defaultColor() = 0;
    virtual void clearDefaultColors() = 0;
    virtual void setDefaultColor(HOM_Color *color) = 0;
    virtual std::string defaultWireStyle() = 0;
    virtual void setDefaultWireStyle(const char *wirestyle) = 0;

    virtual void *_asVoidPointer() = 0;
};

#endif
