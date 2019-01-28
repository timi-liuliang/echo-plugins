/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ObjNode_h__
#define __HOM_ObjNode_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
class HOM_Geometry;
class HOM_Point;
class HOM_Matrix4;
class HOM_Vector3;

SWIGOUT(%rename(ObjNode) HOM_ObjNode;)

class HOM_API HOM_ObjNode : virtual public HOM_Node
{
public:
    HOM_ObjNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ObjNode(const HOM_ObjNode &obj_node)
    : HOM_Node(obj_node),
      HOM_NetworkMovableItem(obj_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ObjNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)


    SWIGOUT(%newobject origin;)
    virtual HOM_Vector3 *origin() = 0;

    SWIGOUT(%newobject localTransform;)
    virtual HOM_Matrix4 *localTransform() = 0;
    SWIGOUT(%newobject localTransformAtTime;)
    virtual HOM_Matrix4 *localTransformAtTime(double time) = 0;

    SWIGOUT(%newobject worldTransform;)
    virtual HOM_Matrix4 *worldTransform() = 0;

    SWIGOUT(%newobject worldTransformAtTime;)
    virtual HOM_Matrix4 *worldTransformAtTime(double time) = 0;

    SWIGOUT(%kwargs setWorldTransform;)
    virtual void setWorldTransform(
	    HOM_Matrix4 &matrix, bool fail_on_locked_parms = false) = 0;

    virtual void setCookTransform(HOM_Matrix4 &matrix) = 0;

    SWIGOUT(%newobject parmTransform;)
    virtual HOM_Matrix4 *parmTransform() = 0;

    SWIGOUT(%kwargs setParmTransform;)
    virtual void setParmTransform(
	    HOM_Matrix4 &matrix, bool fail_on_locked_parms = false) = 0;

    SWIGOUT(%newobject parmPivotTransform;)
    virtual HOM_Matrix4 *parmPivotTransform() = 0;

    SWIGOUT(%kwargs setParmPivotTransform;)
    virtual void setParmPivotTransform(
	    HOM_Matrix4 &matrix, bool fail_on_locked_parms = false) = 0;

    SWIGOUT(%newobject preTransform;)
    virtual HOM_Matrix4 *preTransform() = 0;

    virtual void setPreTransform(HOM_Matrix4 &matrix) = 0;

    virtual void movePreTransformIntoParmTransform() = 0;

    virtual void moveParmTransformIntoPreTransform() = 0;
    virtual void moveParmTranslateIntoPreTransform() = 0;
    virtual void moveParmRotateIntoPreTransform() = 0;
    virtual void moveParmScaleIntoPreTransform() = 0;

    virtual HOM_Matrix4 *parentAndSubnetTransform() = 0;

    SWIGOUT(%newobject getTransformToNode;)
    virtual HOM_Matrix4 *getTransformToNode(HOM_ObjNode &obj_node) = 0;

    SWIGOUT(%newobject getTransformFromPointToPoint;)
    virtual HOM_Matrix4 *getTransformFromPointToPoint(
	    HOM_Vector3 &pos3, HOM_ObjNode &other_node,
	    HOM_Vector3 &other_pos3) = 0;

    SWIGOUT(%newobject buildLookatRotation;)
    virtual HOM_Matrix4 *buildLookatRotation(HOM_ObjNode &to_node,
	    HOM_Vector3 *up_vector = NULL) = 0;

    virtual bool isObjectDisplayed() = 0;

    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isSelectableInViewport() = 0;
    virtual void setSelectableInViewport(bool on) = 0;
    virtual bool isShowingOrigin() = 0;
    virtual void showOrigin(bool on) = 0;
    virtual bool isUsingXray() = 0;
    virtual void useXray(bool on) = 0;

    SWIGOUT(%newobject displayNode;)
    virtual HOM_Node *displayNode() = 0;

    SWIGOUT(%newobject renderNode;)
    virtual HOM_Node *renderNode() = 0;

    virtual void combine(const std::vector<HOM_Node *> &nodes) = 0;

    SWIGOUT(%kwargs material;)
    virtual void material(const char *operation,
	    const std::vector<std::string> &parms=std::vector<std::string>()) = 0;

    // Override HOM_Node's _parmClipData() and _saveParmClip() methods
    // to handle animation data when constraints are enabled
    virtual HOM_BinaryString _parmClipData(double start,
					   double end,
					   bool binary = true,
					   bool use_blosc_compression = true,
					   double sample_rate = 0,
					   bool scoped_only = false) = 0;

    virtual void _saveParmClip(std::string file_name,
			       double start,
			       double end,
			       double sample_rate = 0,
			       bool scoped_only = false) = 0;

};

#endif
