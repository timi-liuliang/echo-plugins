/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_SopNode_h__
#define __HOM_SopNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
class HOM_Geometry;
class HOM_GeometryDelta;
class HOM_Point;
class HOM_Prim;
class HOM_Vertex;
class HOM_SopVerb;

SWIGOUT(%rename(SopNode) HOM_SopNode;)

class HOM_API HOM_SopNode : virtual public HOM_Node
{
public:
    HOM_SopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_SopNode(const HOM_SopNode &sop_node)
    : HOM_Node(sop_node),
      HOM_NetworkMovableItem(sop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_SopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject geometry;)
    SWIGOUT(%kwargs geometry;)
    virtual HOM_Geometry *geometry(int output_index=0) = 0;
    SWIGOUT(%newobject geometryAtFrame;)
    SWIGOUT(%kwargs geometryAtFrame;)
    virtual HOM_Geometry *geometryAtFrame(double frame, int output_index=0) = 0;
    SWIGOUT(%newobject inputGeometry;)
    virtual HOM_Geometry *inputGeometry(int index) = 0;
    SWIGOUT(%newobject geometryAtFrame;)
    virtual HOM_Geometry *inputGeometryAtFrame(double frame, int index) = 0;

    SWIGOUT(%newobject geometryDelta;)
    virtual HOM_GeometryDelta *geometryDelta() = 0;

    virtual bool hasVerb() = 0;
    SWIGOUT(%newobject verb;)
    virtual HOM_SopVerb *verb() = 0;

    // This method is deprecated.
    SWIGOUT(%newobject selection;)
    virtual HOM_Selection *selection(HOM_EnumValue &selection_type) = 0;
    // This method is deprecated.
    virtual void setSelection(HOM_Selection &selection) = 0;

    SWIGOUT(%newobject curPoint;)
    virtual HOM_Point *curPoint() = 0;
    virtual void setCurPoint(HOM_Point *point_or_none) = 0;

    SWIGOUT(%newobject curPrim;)
    virtual HOM_Prim *curPrim() = 0;
    virtual void setCurPrim(HOM_Prim *prim_or_none) = 0;

    SWIGOUT(%newobject curVertex;)
    virtual HOM_Vertex *curVertex() = 0;
    virtual void setCurVertex(HOM_Vertex *vertex_or_none) = 0;


    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isRenderFlagSet() = 0;
    virtual void setRenderFlag(bool on) = 0;
    virtual bool isTemplateFlagSet() = 0;
    virtual void setTemplateFlag(bool on) = 0;
    virtual bool isSelectableTemplateFlagSet() = 0;
    virtual void setSelectableTemplateFlag(bool on) = 0;
    virtual bool isHighlightFlagSet() = 0;
    virtual void setHighlightFlag(bool on) = 0;
    virtual bool isSoftLocked() = 0;
    virtual void setSoftLocked(bool on) = 0;
    virtual bool isHardLocked() = 0;
    virtual void setHardLocked(bool on) = 0;
    virtual bool isUnloadFlagSet() = 0;
    virtual void setUnloadFlag(bool on) = 0;
    
    // This method is deprecated.
    virtual void copyNodeSelectionToUserSelection(bool force_cook = false) = 0; 

    SWIGOUT(%newobject displayNode;)
    virtual HOM_Node *displayNode() = 0;
    SWIGOUT(%newobject renderNode;)
    virtual HOM_Node *renderNode() = 0;
};

#endif
