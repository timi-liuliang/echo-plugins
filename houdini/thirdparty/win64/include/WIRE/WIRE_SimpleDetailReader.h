/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __WIRE_SimpleDetailReader_h__
#define __WIRE_SimpleDetailReader_h__

#include "WIRE_API.h"

#include <GU/GU_Detail.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <UT/UT_Quaternion.h>
#include <SIM/SIM_Object.h>

// float
#define WIRE_SIMPLEDETAILREADER_GET_VT_F_INTERNAL(name) \
    private: fpreal get##name##Internal(GA_Index primind, GA_Size vtx) const \
	{ \
	    const GA_Attribute *a = my##name.getAttribute(); \
	    GA_AttributeOwner owner = a->getOwner(); \
	    if (owner == GA_ATTRIB_DETAIL) \
		return my##name.get(GA_DETAIL_OFFSET); \
	    if (primind >= myGdpConst->getNumPrimitives()) \
		return 0; \
	    GA_Offset offset = myGdpConst->primitiveOffset(primind); \
	    if(owner == GA_ATTRIB_PRIMITIVE) \
		return my##name.get(offset); \
	    const GEO_Primitive *prim = myGdpConst->getGEOPrimitive(offset); \
	    if (vtx >= prim->getVertexCount()) \
		return 0; \
	    if (owner == GA_ATTRIB_VERTEX) \
		return my##name.get(prim->getVertexOffset(vtx)); \
	    return my##name.get(prim->getPointOffset(vtx)); \
	}
#define WIRE_SIMPLEDETAILREADER_GET_VT_F(name) \
    WIRE_SIMPLEDETAILREADER_GET_VT_F_INTERNAL(name) \
    public: fpreal get##name(GA_Index primind, GA_Size vtx) const \
    { \
	fpreal result = my##name##Default; \
	if(my##name.isValid()) \
	    result *= get##name##Internal(primind, vtx); \
	return result; \
    }

// int
#define WIRE_SIMPLEDETAILREADER_GET_VT_I(name) \
    public: int get##name(GA_Index primind, GA_Size vtx) const \
	{ \
	    const GA_Attribute *a = my##name.getAttribute(); \
	    if (!a) \
		return 0; \
	    GA_AttributeOwner owner = a->getOwner(); \
	    if (owner == GA_ATTRIB_DETAIL) \
		return my##name.get(GA_DETAIL_OFFSET); \
	    if (primind >= myGdpConst->getNumPrimitives()) \
		return 0; \
	    GA_Offset offset = myGdpConst->primitiveOffset(primind); \
	    if(owner == GA_ATTRIB_PRIMITIVE) \
		return my##name.get(offset); \
	    const GEO_Primitive *prim = myGdpConst->getGEOPrimitive(offset); \
	    if (vtx >= prim->getVertexCount()) \
		return 0; \
	    if (owner == GA_ATTRIB_VERTEX) \
		return my##name.get(prim->getVertexOffset(vtx)); \
	    return my##name.get(prim->getPointOffset(vtx)); \
	}

#define WIRE_SIMPLEDETAILREADER_GET_PT_F(name) \
    public: fpreal get##name(GA_Index ptind) const \
	{ \
	    fpreal retval = my##name##Default; \
	    if(my##name.isValid()) \
	    { \
		GA_Offset offset = myGdpConst->pointOffset(ptind); \
		if (offset != GA_INVALID_OFFSET) \
		    retval *= my##name.get(offset); \
	    } \
	    return retval; \
	}
#define WIRE_SIMPLEDETAILREADER_GET_VT_B(name) \
    WIRE_SIMPLEDETAILREADER_GET_VT_F_INTERNAL(name##F) \
    WIRE_SIMPLEDETAILREADER_GET_VT_I(name##I) \
    public: bool get##name(GA_Index primind, GA_Size vtx) const \
	{ \
	    if(my##name##I.isValid()) \
		return get##name##I(primind, vtx) != 0; \
	    if(my##name##F.isValid()) \
		return get##name##FInternal(primind, vtx) > 0.5; \
	    return false; \
	}

class WIRE_API WIRE_SimpleDetailReader
{
public:
    WIRE_SimpleDetailReader(const SIM_Object &object);
    ~WIRE_SimpleDetailReader();

    bool isValid() const { return myGdpConst; }

    const GU_Detail *getDetailConst() const { return myGdpConst; }

    const UT_DMatrix4 &getTransform() const { return myTransform; }

    WIRE_SIMPLEDETAILREADER_GET_VT_F(Width)
    WIRE_SIMPLEDETAILREADER_GET_VT_B(NoCollide)
    WIRE_SIMPLEDETAILREADER_GET_PT_F(Friction)
    WIRE_SIMPLEDETAILREADER_GET_PT_F(DynamicFriction)

    UT_Vector3 getPosition(GA_Index i) const
	{ return myGdpConst->getPos3(myGdpConst->pointOffset(i)) * myTransform; }
    UT_Vector3 getVelocity(GA_Index i) const
	{
	    if (myVelocity.isInvalid())
		return UT_Vector3(0, 0, 0);
	    return rowVecMult3(myVelocity.get(myGdpConst->pointOffset(i)), myTransform);
	}

protected:
    GU_DetailHandleAutoReadLock *myGdpLock;

    UT_DMatrix4 myTransform;

    const GU_Detail *myGdpConst;

    GA_ROHandleF myWidth;
    fpreal myWidthDefault;
    GA_ROHandleF myNoCollideF;
    GA_ROHandleI myNoCollideI;
    GA_ROHandleF myFriction;
    fpreal myFrictionDefault;
    GA_ROHandleF myDynamicFriction;
    fpreal myDynamicFrictionDefault;

    GA_ROHandleV3 myVelocity;
};

#endif
