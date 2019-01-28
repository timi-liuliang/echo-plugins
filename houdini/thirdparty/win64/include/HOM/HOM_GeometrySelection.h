/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_GeometrySelection_h__
#define __HOM_GeometrySelection_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include <string>
#include <vector>

class HOM_Node;

SWIGOUT(%rename(GeometrySelection) HOM_GeometrySelection;)
SWIGOUT(%feature("notabstract") HOM_GeometrySelection;)

class HOM_API HOM_GeometrySelection
{
public:
#ifdef SWIG
%extend
{
    HOM_GeometrySelection()
    { 
	return HOM().newGeometrySelection(); 
    }

    HOM_GeometrySelection(int64 val)
    { 
	return HOM().newGeometrySelection(val); 
    }
}
#else
    HOM_GeometrySelection()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_GeometrySelection(const HOM_GeometrySelection &)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_GeometrySelection()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;
    virtual std::string __str__() = 0;

    virtual bool ordered() = 0;

    virtual HOM_EnumValue &geometryType() = 0;
    virtual void setGeometryType(HOM_EnumValue &type) = 0;

    virtual HOM_EnumValue &connectivity() = 0;
    virtual void setConnectivity(HOM_EnumValue &connectivity) = 0;

    virtual std::vector<HOM_EnumValue *> primitiveTypes() = 0;
    virtual void setPrimitiveTypes(std::vector<HOM_EnumValue *> primitive_types) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Node> > nodes() = 0;
    SWIGOUT(%kwargs selectionStrings;)
    virtual std::vector<std::string> selectionStrings(
	    bool empty_string_selects_all = true,
	    bool force_numeric = false) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Selection> > selections() = 0;

    virtual bool needsMergedNode(HOM_Node &parent) = 0;
    virtual HOM_Node *mergedNode(HOM_Node &parent, const char *creator_name,
				 bool force_keep_original_objects=false,
				 bool display_original_objects=false) = 0;
    SWIGOUT(%kwargs mergedSelectionString;)
    virtual std::string mergedSelectionString(
	    bool empty_string_selects_all = true,
	    bool force_numeric = false) = 0;

    SWIGOUT(%kwargs shrinkSelection;)
    virtual void shrinkSelection(bool check_uv=true) = 0;

    SWIGOUT(%kwargs growSelection;)
    virtual void growSelection() = 0;
};

#endif
