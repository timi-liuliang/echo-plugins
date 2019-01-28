/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimTetMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */
#ifndef GT_PrimTetMesh_h
#define GT_PrimTetMesh_h

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_GEOPrimCollect.h"
#include "GT_AttributeList.h"

class GT_API GT_PrimTetMesh : public GT_Primitive
{
public:
		  GT_PrimTetMesh(const GT_DataArrayHandle     &vertex_list,
				 const GT_AttributeListHandle &vert_attribs,
				 const GT_AttributeListHandle &point_attribs,
				 const GT_AttributeListHandle &prim_attribs,
				 const GT_AttributeListHandle &dtl_attribs,
				 const GT_DataArrayHandle     &face_verts);

		 GT_PrimTetMesh(const GT_PrimTetMesh &pmesh,
				const GT_AttributeListHandle &vert_attribs,
				const GT_AttributeListHandle &point_attribs,
				const GT_AttributeListHandle &prim_attribs,
				const GT_AttributeListHandle &dtl_attribs);
		  
		  GT_PrimTetMesh(const GT_PrimTetMesh &tetmesh);
    virtual      ~GT_PrimTetMesh();

    virtual int		getPrimitiveType() const { return GT_PRIM_TET_MESH; }
    virtual const char *className() const	 { return "GT_PrimTetMesh"; }

    int		  getTetCount() const
	{ return myUniformAttribs ? myUniformAttribs->get(0)->entries() : 0; }
    
    virtual const GT_AttributeListHandle &getVertexAttributes() const
					  { return myVertexAttribs; }
    virtual const GT_AttributeListHandle &getPointAttributes() const
					  { return myPointAttribs; }
    virtual const GT_AttributeListHandle &getUniformAttributes() const
					  { return myUniformAttribs; }
    virtual const GT_AttributeListHandle &getDetailAttributes() const
					  { return myDetailAttribs; }

    // point connectivity. 
    virtual const GT_DataArrayHandle &getVertexList() const
					  { return myVertexList; }

    virtual void		enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const;
    virtual bool		refine(GT_Refine &refiner,
					const GT_RefineParms *parms) const;
    
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const;
private:
    GT_DataArrayHandle		 myVertexList;
    GT_DataArrayHandle		 myFaceVerts;
    
    GT_AttributeListHandle	 myVertexAttribs;
    GT_AttributeListHandle	 myPointAttribs;
    GT_AttributeListHandle	 myUniformAttribs;
    GT_AttributeListHandle	 myDetailAttribs;
};

#endif
