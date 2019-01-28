/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Geometry_h__
#define __HOM_Geometry_h__

#include "HOM_API.h"
#include "HOM_BinaryString.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_IterableList.h"
#include "HOM_BoundingBox.h"
#include "HOM_Attrib.h"
#include <UT/UT_Tuple.h>
#include <vector>
#include <string>

class HOM_Point;
class HOM_Vertex;
class HOM_Prim;
class HOM_Edge;
class HOM_Face;
class HOM_Polygon;
class HOM_Surface;
class HOM_PackedPrim;
class HOM_Volume;
class HOM_VDB;
class HOM_PointGroup;
class HOM_PrimGroup;
class HOM_EdgeGroup;
class HOM_SopNode;
class HOM_Vector2;
class HOM_Vector3;
class HOM_Matrix4;
class HOM_GUDetailHandle;
class HOM_Selection;

SWIGOUT(%rename(Geometry) HOM_Geometry;)
// Force SWIG to output an __init__ method even though it determines this
// class is abstract.
SWIGOUT(%feature("notabstract") HOM_Geometry;)

class HOM_API HOM_Geometry
{
public:
#ifdef SWIG
%extend
{
    HOM_Geometry()
    { return HOM().newGeometry(); }
    HOM_Geometry(HOM_Geometry &geo)
    { return HOM().newGeometry(geo); }
}
#else
    HOM_Geometry()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Geometry(const HOM_Geometry &)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif
    virtual ~HOM_Geometry()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject sopNode;)
    virtual HOM_SopNode *sopNode() = 0;
    virtual int sopNodeOutputIndex() = 0;

    SWIGOUT(%newobject freeze;)
    virtual HOM_Geometry *freeze(bool read_only=false) = 0;

    SWIGOUT(%newobject iterPoints;)
    virtual HOM_IterableList<HOM_Point> *iterPoints() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Point> > points() = 0;

    SWIGOUT(%newobject point;)
    virtual HOM_Point *point(int index) = 0;

    SWIGOUT(%newobject iterPrims;)
    virtual HOM_IterableList<HOM_Prim> *iterPrims() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Prim> > prims() = 0;

    SWIGOUT(%newobject prim;)
    virtual HOM_Prim *prim(int index) = 0;

    virtual bool containsPrimType(const char *type_name) = 0;
    virtual bool containsPrimType(HOM_EnumValue &prim_type) = 0;
    virtual int64 countPrimType(const char *type_name) = 0;
    virtual int64 countPrimType(HOM_EnumValue &prim_type) = 0;

    virtual double averageMinDistance(
	const HOM_Matrix4& local_transform,
	HOM_Geometry& geometry,
	const HOM_Matrix4& geometry_transform) = 0;

    virtual double averageEdgeLength() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Point> > globPoints(const char *pattern) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Prim> > globPrims(const char *pattern) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Vertex> > globVertices(
	    const char *pattern) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Edge> > globEdges(const char *pattern) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Attrib> > pointAttribs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Attrib> > primAttribs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Attrib> > vertexAttribs() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Attrib> > globalAttribs() = 0;


    SWIGOUT(%newobject findPointAttrib;)
    virtual HOM_Attrib *findPointAttrib(const char *name) = 0;

    SWIGOUT(%newobject findPrimAttrib;)
    virtual HOM_Attrib *findPrimAttrib(const char *name) = 0;

    SWIGOUT(%newobject findVertexAttrib;)
    virtual HOM_Attrib *findVertexAttrib(const char *name) = 0;

    SWIGOUT(%newobject findGlobalAttrib;)
    virtual HOM_Attrib *findGlobalAttrib(const char *name) = 0;

    virtual HOM_EnumValue& attribType() = 0;


    // These methods return global (detail) attribute values.
    virtual double floatAttribValue(const char *name) = 0;

    virtual double floatAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::vector<double> floatListAttribValue(const char *name) = 0;

    virtual std::vector<double> floatListAttribValue(HOM_Attrib &attrib) = 0;

    virtual int64 intAttribValue(const char *name) = 0;

    virtual int64 intAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::vector<int64> intListAttribValue(const char *name) = 0;

    virtual std::vector<int64> intListAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::string stringAttribValue(const char *name) = 0;

    virtual std::string stringAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::vector<std::string> stringListAttribValue(const char *name) = 0;

    virtual std::vector<std::string> stringListAttribValue(HOM_Attrib &attrib) = 0;


    // These methods return/set arrays of point/primitive attribute values.
    virtual std::vector<double> pointFloatAttribValues(const char *name) = 0;
    SWIGOUT(%kwargs pointFloatAttribValuesAsString;)
    virtual HOM_BinaryString pointFloatAttribValuesAsString(const char *name,
	    HOM_EnumValue &float_type = HOM_numericData::Float32) = 0;
    virtual std::vector<double> primFloatAttribValues(const char *name) = 0;
    SWIGOUT(%kwargs primFloatAttribValuesAsString;)
    virtual HOM_BinaryString primFloatAttribValuesAsString(const char *name,
	    HOM_EnumValue &float_type = HOM_numericData::Float32) = 0;

    virtual std::vector<int64> pointIntAttribValues(const char *name) = 0;
    SWIGOUT(%kwargs pointIntAttribValuesAsString;)
    virtual HOM_BinaryString pointIntAttribValuesAsString(const char *name,
	    HOM_EnumValue &int_type = HOM_numericData::Int32) = 0;
    virtual std::vector<int64> primIntAttribValues(const char *name) = 0;
    SWIGOUT(%kwargs primIntAttribValuesAsString;)
    virtual HOM_BinaryString primIntAttribValuesAsString(const char *name,
	    HOM_EnumValue &int_type = HOM_numericData::Int32) = 0;

    virtual std::vector<std::string> pointStringAttribValues(const char *name) = 0;
    virtual std::vector<std::string> primStringAttribValues(const char *name) = 0;

    virtual void setPointFloatAttribValues(
	    const char *name, const std::vector<double> &values) = 0;
    virtual void setPrimFloatAttribValues(
	    const char *name, const std::vector<double> &values) = 0;

    SWIGOUT(%ignore opaqueSetPointFloatAttribValuesFromString);
    virtual void opaqueSetPointFloatAttribValuesFromString(
	    const char *name, const char *values, int64 length,
	    HOM_EnumValue &float_type) = 0;
    SWIGOUT(%ignore opaqueSetPrimFloatAttribValuesFromString);
    virtual void opaqueSetPrimFloatAttribValuesFromString(
	    const char *name, const char *values, int64 length,
	    HOM_EnumValue &float_type) = 0;

#ifdef SWIG
%extend
{
    %kwargs setPointFloatAttribValuesFromString;
    void setPointFloatAttribValuesFromString(
	    const char *name, InterpreterObject values,
	    HOM_EnumValue &float_type = HOM_numericData::Float32)
    {
	HOM_PyBuffer buffer(values);
	self->opaqueSetPointFloatAttribValuesFromString(
	    name, (const char *)buffer.myData, buffer.myLength, float_type);
    }

    %kwargs setPrimFloatAttribValuesFromString;
    void setPrimFloatAttribValuesFromString(
	    const char *name, InterpreterObject values,
	    HOM_EnumValue &float_type = HOM_numericData::Float32)
    {
	HOM_PyBuffer buffer(values);
	self->opaqueSetPrimFloatAttribValuesFromString(
	    name, (const char *)buffer.myData, buffer.myLength, float_type);
    }
}
#endif

    virtual void setPointIntAttribValues(
	    const char *name, const std::vector<int64> &values) = 0;
    virtual void setPrimIntAttribValues(
	    const char *name, const std::vector<int64> &values) = 0;

    SWIGOUT(%ignore opaqueSetPointIntAttribValuesFromString);
    virtual void opaqueSetPointIntAttribValuesFromString(
	    const char *name, const char *values, int64 length,
	    HOM_EnumValue &int_type) = 0;
    SWIGOUT(%ignore opaqueSetPrimIntAttribValuesFromString);
    virtual void opaqueSetPrimIntAttribValuesFromString(
	    const char *name, const char *values, int64 length,
	    HOM_EnumValue &int_type) = 0;

#ifdef SWIG
%extend
{
    %kwargs setPointIntAttribValuesFromString;
    void setPointIntAttribValuesFromString(
	    const char *name, InterpreterObject values,
	    HOM_EnumValue &int_type = HOM_numericData::Int32)
    {
	HOM_PyBuffer buffer(values);
	self->opaqueSetPointIntAttribValuesFromString(
	    name, (const char *)buffer.myData, buffer.myLength, int_type);
    }

    %kwargs setPrimIntAttribValuesFromString;
    void setPrimIntAttribValuesFromString(
	    const char *name, InterpreterObject values,
	    HOM_EnumValue &int_type = HOM_numericData::Int32)
    {
	HOM_PyBuffer buffer(values);
	self->opaqueSetPrimIntAttribValuesFromString(
	    name, (const char *)buffer.myData, buffer.myLength, int_type);
    }
}
#endif

    virtual void setPointStringAttribValues(
	    const char *name, const std::vector<std::string> &values) = 0;
    virtual void setPrimStringAttribValues(
	    const char *name, const std::vector<std::string> &values) = 0;

    virtual void renamePointAttrib(const char *old_name, const char *new_name) = 0;
    virtual void renamePrimAttrib(const char *old_name, const char *new_name) = 0;
    virtual void renameVertexAttrib(const char *old_name, const char *new_name) = 0;
    virtual void renameGlobalAttrib(const char *old_name, const char *new_name) = 0;

    SWIGOUT(%newobject createPoint;)
    virtual HOM_Point *createPoint() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Point> >createPoints(
	const std::vector<std::vector<double> > &point_positions) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Point> >createPoints(
	const std::vector<HOM_Vector3 *> &point_positions) = 0;

    SWIGOUT(%newobject createPolygon;)
    virtual HOM_Polygon *createPolygon() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Polygon> >createPolygons(
	const std::vector<std::vector<HOM_Point *> > &points) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Polygon> >createPolygons(
	const std::vector<std::vector<int> > &point_numbers) = 0;

    SWIGOUT(%newobject createPacked;)
    virtual HOM_PackedPrim *createPacked(const char *packed_type) = 0;

    SWIGOUT(%newobject createTetrahedron;)
    virtual HOM_Prim *createTetrahedron() = 0;

    SWIGOUT(%newobject createTetrahedronInPlace;)
    virtual HOM_Prim *createTetrahedronInPlace(HOM_Point *p0, HOM_Point *p1,
						HOM_Point *p2, HOM_Point *p3) = 0;

    SWIGOUT(%newobject createNURBSCurve;)
    SWIGOUT(%kwargs createNURBSCurve;)
    virtual HOM_Face *createNURBSCurve(int num_points=4, bool is_closed=false,
                                       int order=4) = 0;

    SWIGOUT(%newobject createBezierCurve;)
    SWIGOUT(%kwargs createBezierCurve;)
    virtual HOM_Face *createBezierCurve(int num_points=4, bool is_closed=false,
                                        int order=4) = 0;

    SWIGOUT(%newobject createNURBSSurface;)
    SWIGOUT(%kwargs createNURBSSurface;)
    virtual HOM_Surface *createNURBSSurface(
	    int rows, int cols,
	    bool is_closed_in_u=false, bool is_closed_in_v=false,
	    int order_u=4, int order_v=4) = 0;

    SWIGOUT(%newobject createBezierSurface;)
    SWIGOUT(%kwargs createBezierSurface;)
    virtual HOM_Surface *createBezierSurface(
	    int rows, int cols,
	    bool is_closed_in_u=false, bool is_closed_in_v=false,
	    int order_u=4, int order_v=4) = 0;

    SWIGOUT(%newobject createMeshSurface;)
    SWIGOUT(%kwargs createMeshSurface;)
    virtual HOM_Surface *createMeshSurface(
	    int rows, int cols,
	    bool is_closed_in_u=false, bool is_closed_in_v=false) = 0;

    SWIGOUT(%newobject createVolume;)
    SWIGOUT(%kwargs createVolume;)
    virtual HOM_Volume *createVolume(
	    int xres, int yres, int zres, HOM_BoundingBox *bounding_box=NULL) = 0;


    virtual void transform(HOM_Matrix4 &matrix) = 0;

    virtual void transformPrims(
	    const std::vector<HOM_Prim *> &prims, HOM_Matrix4 &matrix) = 0;

    SWIGOUT(%kwargs deletePrims;)
    virtual void deletePrims(
	    const std::vector<HOM_Prim *> &prims, bool keep_points=false) = 0;

    virtual void deletePoints(const std::vector<HOM_Point *> &points) = 0;


    // Swig does not allow keyword arguments for overloaded functions, so
    // addAttrib is wrapped in houpythonportion.py to enable keyword
    // arguments.  If you want to change the signature of addAttrib, you must
    // also edit houpythonportion.py.
    SWIGOUT(%newobject addAttrib;)
    virtual HOM_Attrib *addAttrib(
	    HOM_EnumValue &type, const char *name,
	    int64 default_value,
	    bool transform_as_normal,
	    bool create_local_variable) = 0;

    virtual HOM_Attrib *addAttrib(
	    HOM_EnumValue &type, const char *name,
	    double default_value,
	    bool transform_as_normal,
	    bool create_local_variable) = 0;

    virtual HOM_Attrib *addAttrib(
	    HOM_EnumValue &type, const char *name,
	    const char *default_value,
	    bool transform_as_normal,
	    bool create_local_variable) = 0;

    virtual HOM_Attrib *addAttrib(
	    HOM_EnumValue &type,
	    const char *name,
	    const std::vector<int64> &default_value,
	    bool transform_as_normal,
	    bool create_local_variable) = 0;

    virtual HOM_Attrib *addAttrib(
	    HOM_EnumValue &type,
	    const char *name,
	    const std::vector<double> &default_value,
	    bool transform_as_normal,
	    bool create_local_variable) = 0;

    virtual HOM_Attrib *addAttrib(
	    HOM_EnumValue &type,
	    const char *name,
	    const std::vector<std::string> &default_value,
	    bool transform_as_normal,
	    bool create_local_variable) = 0;

    SWIGOUT(%kwargs addArrayAttrib;)
    SWIGOUT(%newobject addArrayAttrib;)
    virtual HOM_Attrib *addArrayAttrib(
	    HOM_EnumValue &type, const char *name,
	    HOM_EnumValue &data_type,
	    int64 tuple_size = 1) = 0;


    virtual void setGlobalAttribValue(
	    HOM_Attrib &attrib, int64 attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    HOM_Attrib &attrib, double attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    HOM_Attrib &attrib, const char *attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    HOM_Attrib &attrib, const std::vector<int64> &attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    HOM_Attrib &attrib, const std::vector<double> &attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    HOM_Attrib &attrib, const std::vector<std::string> &attrib_value) = 0;

    virtual void setGlobalAttribValue(
	    const char *attrib_name, int64 attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    const char *attrib_name, double attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    const char *attrib_name, const char *attrib_value) = 0;
    // Note that, because of strangess the with way swig works when deciding
    // which overloaded method to call, it's very important that we list the
    // vector of ints before the vector of doubles.  Otherwise, swig will call
    // the double version when you pass in a vector of ints.
    virtual void setGlobalAttribValue(
	    const char *attrib_name, const std::vector<int64> &attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    const char *attrib_name, const std::vector<double> &attrib_value) = 0;
    virtual void setGlobalAttribValue(
	    const char *attrib_name,
	    const std::vector<std::string> &attrib_value) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject attribValue(const char *name)
    { return HOMattribValue(*self, name); }

    InterpreterObject attribValue(HOM_Attrib &attrib)
    { return HOMattribValue(*self, attrib); }
}
#endif

    virtual std::vector<std::string> attributeCaptureRegions() = 0;
    virtual std::vector<std::string> attributeCaptureObjectPaths() = 0;


    virtual std::vector<std::string> intrinsicNames() = 0;

    virtual hboost::any intrinsicValue(
	    const char *intrinsic_name) = 0;
    virtual size_t intrinsicSize(const char *intrinsic_name) = 0;
    virtual bool intrinsicReadOnly(const char *intrinsic_name) = 0;

    virtual void setIntrinsicValue(const char *intrinsic_name, double value) = 0;
    virtual void setIntrinsicValue(const char *intrinsic_name, int value) = 0;
    virtual void setIntrinsicValue(const char *intrinsic_name, int64 value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const char *value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<double> &value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<int> &value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<int64> &value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<std::string> &value) = 0;

    virtual HOM_BinaryString data() = 0;

    virtual void saveToFile(const char *file_name) = 0;

    virtual void loadFromFile(const char *file_name) = 0;

    virtual void merge(HOM_Geometry &geometry) = 0;

    virtual void clear() = 0;

    SWIGOUT(%kwargs intersect;)
    virtual int intersect(
    	    const HOM_Vector3 &rayorig, const HOM_Vector3 &dir,
    	    HOM_Vector3 &p, HOM_Vector3 &n, HOM_Vector3 &uvw,
    	    const char *pattern = nullptr, float min_hit=1e-2f, float max_hit=1e18f) = 0;
    
    virtual std::vector<int> _orderedPoints(
            const char *groupname, 
            const char *corner_groupname, 
            HOM_Vector2 &dimensions) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_PointGroup> > pointGroups() = 0;

    SWIGOUT(%newobject findPointGroup;)
    virtual HOM_PointGroup *findPointGroup(const char *name) = 0;

    SWIGOUT(%newobject createPointGroup;)
    virtual HOM_PointGroup *createPointGroup(
            const char *name, bool is_ordered=false) = 0;


    virtual std::vector<HOM_ElemPtr<HOM_PrimGroup> > primGroups() = 0;

    SWIGOUT(%newobject findPrimGroup;)
    virtual HOM_PrimGroup *findPrimGroup(const char *name) = 0;

    SWIGOUT(%newobject createPrimGroup;)
    virtual HOM_PrimGroup *createPrimGroup(
            const char *name, bool is_ordered=false) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_EdgeGroup> > edgeGroups() = 0;

    SWIGOUT(%newobject findEdgeGroup;)
    virtual HOM_EdgeGroup *findEdgeGroup(const char *name) = 0;

    SWIGOUT(%newobject createEdgeGroup;)
    virtual HOM_EdgeGroup *createEdgeGroup(
            const char *name) = 0;

    SWIGOUT(%newobject findEdge;)
    virtual HOM_Edge *findEdge(HOM_Point * p0, HOM_Point * p1) = 0;

    virtual HOM_BoundingBox boundingBox() = 0;
    virtual HOM_BoundingBox pointBoundingBox(const char *ptgroup) = 0;
    virtual HOM_BoundingBox primBoundingBox(const char *primgroup) = 0;

    virtual UT_Tuple<HOM_ElemPtr<HOM_Prim>, double, double, double>
	nearestPrim(const std::vector<double> &position) = 0;

    SWIGOUT(%kwargs primLoop;)
    virtual std::vector<HOM_ElemPtr<HOM_Prim> >
	primLoop(std::vector<HOM_Prim *> prims,
		 HOM_EnumValue &loop_type = HOM_componentLoopType::Partial) = 0;
    SWIGOUT(%kwargs pointLoop;)
    virtual std::vector<HOM_ElemPtr<HOM_Point> >
	pointLoop(std::vector<HOM_Point *> points,
		 HOM_EnumValue &loop_type = HOM_componentLoopType::Partial) = 0;
    SWIGOUT(%kwargs edgeLoop;)
    virtual std::vector<HOM_ElemPtr<HOM_Edge> >
	edgeLoop(std::vector<HOM_Edge *> edges,
		 HOM_EnumValue &loop_type = HOM_componentLoopType::Partial,
		 bool full_loop_per_edge = true,
		 bool force_ring = false,
		 bool allow_ring = true) = 0;

    virtual std::vector<HOM_Vector3>
	pointNormals(const std::vector<HOM_Point *> &points) = 0;

    SWIGOUT(%newobject selection;)
    virtual HOM_Selection *selection() = 0;

    // This method is not documented in the user documentation.  See
    // the comments in HOMF_Geometry.C for what it does.
    virtual HOM_GUDetailHandle *_guDetailHandle() = 0;
};

#endif
