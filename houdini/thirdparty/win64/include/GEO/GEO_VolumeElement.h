/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_VolumeElement.h (GEO Library, C++)
 *
 * COMMENTS:	This is the base class for all volume element types.
 */

#pragma once

#ifndef __GEO_VolumeElement__
#define __GEO_VolumeElement__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include <GA/GA_Defines.h>
#include <SYS/SYS_Inline.h>
#include <UT/UT_Array.h>

class GA_Detail;

class GEO_API GEO_VolumeElement : public GEO_Primitive
{
public:
    SYS_FORCE_INLINE
    GEO_VolumeElement(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Primitive(d, offset)
    {}

    /// The number of faces that make up this volume
    virtual GA_Size	getFaceCount() const = 0;
    /// Returns the indices to the vertices, not the vertex offsets!
    virtual int		getFaceIndices(GA_Size faceno, UT_IntArray &vtxlist) const = 0;

    virtual GA_Offset findSharedFace(GA_Size faceno) const = 0;

    /// The number of edges in this volume.
    virtual GA_Size	getEdgeCount() const = 0;
    /// Returns the indices of the edge.  Note each edge shows up once
    /// and due to the nature of volumes, is undirected.
    virtual void	getEdgeIndices(GA_Size edgeno, int &e0, int &e1) const = 0;

    virtual int		getBBox(UT_BoundingBox *bbox) const;
    virtual void	addToBSphere(UT_BoundingSphere *bsphere) const;
    virtual UT_Vector3	baryCenter() const;
    virtual UT_Vector3	 computeNormal() const;

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	addPointRefToGroup(GA_PointGroup &grp) const;

    virtual bool	 vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const;
#endif

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GA_FAMILY_VOLUMEELEMENT; }

    /// All subclasses should call this method to register the intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Primitive::registerIntrinsics(defn); }
    
    virtual bool	evaluatePointRefMap(GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				fpreal u, fpreal v, uint du, uint dv) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const
			 {
			    return GEO_Primitive::evaluatePointV4(pos, u, v,
					du, dv);
			 }
    virtual bool	evaluateBaryCenterRefMap(GA_Offset result_vertex,
				GA_AttributeRefMap &hlist) const;

SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif

