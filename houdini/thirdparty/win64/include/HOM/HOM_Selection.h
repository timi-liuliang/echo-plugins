/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Selection_h__
#define __HOM_Selection_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Geometry.h"
#include "HOM_PtrOrNull.h"
#include <string>

SWIGOUT(%rename(Selection) HOM_Selection;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_Selection;)

class HOM_API HOM_Selection
{
public:
#ifdef SWIG
%extend
{
    HOM_Selection(HOM_EnumValue &selection_type)
    { return HOM().newSelection(selection_type, NULL, NULL); }
    HOM_Selection(HOM_Geometry &geo,
		  HOM_EnumValue &selection_type,
		  const char *selection_string)
    { return HOM().newSelection(selection_type, &geo, selection_string); }
    HOM_Selection(HOM_Geometry &geo,
		  const std::vector<HOM_EnumValue *> &selection_types,
		  const char *selection_string)
    { return HOM().newSelection(selection_types, &geo, selection_string); }
    HOM_Selection(const std::vector<HOM_Prim *> &prims)
    { return HOM().newSelection(prims); }
    HOM_Selection(const std::vector<HOM_Point *> &points)
    { return HOM().newSelection(points); }
    HOM_Selection(const std::vector<HOM_Vertex *> &vertices)
    { return HOM().newSelection(vertices); }
    HOM_Selection(const std::vector<HOM_Edge *> &edges)
    { return HOM().newSelection(edges); }
}
#else
    HOM_Selection()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_Selection()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject freeze;)
    virtual HOM_Selection *freeze() = 0;
    SWIGOUT(%kwargs invert;)
    virtual void invert(HOM_Geometry &geo) = 0;
    SWIGOUT(%kwargs convert;)
    virtual void convert(HOM_Geometry &geo,
	    HOM_EnumValue &selection_type) = 0;
    SWIGOUT(%kwargs boundary;)
    virtual void boundary(HOM_Geometry &geo,
	    bool uv_connectivity = false) = 0;
    SWIGOUT(%kwargs grow;)
    virtual void grow(HOM_Geometry &geo,
	    bool uv_connectivity = false) = 0;
    SWIGOUT(%kwargs shrink;)
    virtual void shrink(HOM_Geometry &geo,
	    bool uv_connectivity = false) = 0;
    SWIGOUT(%kwargs combine;)
    virtual void combine(HOM_Geometry &geo,
	    HOM_Selection &selection,
	    HOM_EnumValue &modifier) = 0;
    virtual void clear() = 0;

    virtual HOM_EnumValue &selectionType() = 0;
    virtual int numSelected() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Prim> > prims(HOM_Geometry &geo) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Point> > points(HOM_Geometry &geo) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Vertex> > vertices(HOM_Geometry &geo) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Edge> > edges(HOM_Geometry &geo) = 0;

    SWIGOUT(%kwargs selectionString;)
    virtual std::string selectionString(HOM_Geometry &geo,
	    bool force_numeric = false,
	    bool collapse_where_possible = true,
	    bool asterisk_to_select_all = false) = 0;
};

#endif
