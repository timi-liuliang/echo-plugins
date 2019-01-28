/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_VertexSelection (C++)
 *
 * COMMENTS:
 *	Vertex selection.
 *
 */

#ifndef __GU_VertexSelection_h__
#define __GU_VertexSelection_h__

#include "GU_API.h"
#include "GU_Selection.h"
#include "GU_OrderedIndexGroup.h"

class GU_API GU_VertexSelection : public GU_Selection
{
public:
    GU_VertexSelection(const GA_VertexGroup *vtx_group = NULL);
    virtual ~GU_VertexSelection();

    virtual bool	 select(const GU_Detail &gd,
                	        uint prim_id, uint v_num, uint,
				GU_SelectionRule rule,
				GU_SelectResult &added,
				GU_SelectFinishData *&finish_data);
    virtual bool	 testSelect(const GU_Detail &gd,
                	            uint prim_id, uint v_num, uint,
                	            bool accept_existing,
                	            bool accept_new,
                	            GU_SelectResult &result) const;

    virtual bool	 selectionPoint(const GU_SelectResult &added,
					UT_Vector3 &xsect) const;
    virtual bool	 selectionPoint(const GU_SelectResult &added,
				        UT_Vector3 rayorig, 
					UT_Vector3 &raydir, 
					UT_Vector3 &xsect,
					bool       &normal,
					UT_Vector3 &vector,
					bool accurate, 
					float *u, float *v) const;

    virtual void 	 assign(const GU_Detail &gd, const GU_Selection &sel);
    
    virtual bool	 selectAll(const GU_Detail &gd); 
    virtual bool	 toggleAll(const GU_Detail &gd);
    
    virtual bool	 validate(const GU_Detail &gd);

    virtual bool selectBoundary(const GU_Detail &gd, bool checkuv, const UT_StringHolder &uvattribname);
    virtual bool growSelection(const GU_Detail &gd, bool checkuv, const UT_StringHolder &uvattribname);
    virtual bool shrinkSelection(const GU_Detail &gd, bool checkuv, const UT_StringHolder &uvattribname);
    
    virtual bool	 filterByPrimitiveMask(const GU_Detail &gd, 
					       const GA_PrimitiveTypeMask &mask);
    
    virtual bool	 modifyPoint(const GU_Detail &gd, GA_Offset ptoff, 
                	             GU_ModifyType type);
    
    virtual bool	 modifyVertex(const GU_Detail &gd, GA_Offset vtxoff, 
                	              GU_ModifyType type);
    
    virtual bool	 modifyPrimitive(const GU_Detail &gd, GA_Offset primoff, 
                	                 GU_ModifyType type);
    
    virtual bool	 modifyEdge(const GU_Detail &gd, const GA_Edge &edge, 
                	            const GEO_Primitive *prim, 
                	            GU_ModifyType type);
    
    virtual bool	 modifyBreakpoint(const GU_Detail &gd, 
                	                  const GA_Breakpoint &bkp, 
                	                  GU_ModifyType type);

    virtual const GA_PointGroup		*points(const GU_Detail &gd) const;
    virtual const GA_PrimitiveGroup	*primitives(const GU_Detail &gd) const;
    virtual const GA_VertexGroup	*vertices(const GU_Detail &gd) const;
    virtual const GA_EdgeGroup		*edges(const GU_Detail &gd) const;
    virtual const GA_BreakpointGroup 	*breakpoints(const GU_Detail &gd) const;
    virtual const GA_Group		*mainGroup(const GU_Detail &gd) const
					 { return vertices(gd); }

    virtual bool hasVertexIndex(GA_Index index) const; 

    
    virtual void	 clear();
    virtual void	 clearAndDestroy();

    virtual GA_Size	 entries()   const;

    virtual bool	 getBoundingBox(const GU_Detail &gd,
                	                UT_BoundingBox &bbox) const;
    virtual bool	 getBoundingBox(const GU_Detail &gd,
                	                UT_BoundingBox &bbox,
					const UT_Matrix4R &transform) const;

    virtual bool	 getBoundingBoxUV(const GU_Detail &gd,
                	                  UT_BoundingBox &bbox,
					  const char *name,
					  int isvertex) const;

    virtual GA_GroupType classType() const;

    virtual GU_Selection *clone();

    virtual bool	 modifyMatchingGroup(const GU_Detail &gd,
                	                     const GA_Group &group,
                	                     GU_SelectionRule rule);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += GU_Selection::getMemoryUsage(false);
        mem += myVertices.getMemoryUsage(false);
        return mem;
    }

private:
    GU_VertexSelection(const GU_VertexSelection &s);

    virtual bool	saveElements(UT_JSONWriter &w) const;
    virtual bool	loadElements(UT_JSONParser &p);

    friend struct VertexLoader;
    typedef GU_OrderedIndexGroup<GA_Index> OrderedVertexGroup;
    OrderedVertexGroup		myVertices;
};

#endif
