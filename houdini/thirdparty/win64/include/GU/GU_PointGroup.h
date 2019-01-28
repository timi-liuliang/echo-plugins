/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GU_PointGroup_h__
#define __GU_PointGroup_h__

#include "GU_API.h"
#include "GU_ElementGroup.h"
#include "GU_Detail.h"
#include <UT/UT_BitArray.h>
#include <UT/UT_ThreadedAlgorithm.h>

////////////////////////////////////////////////////////////////////////////
//
//  GU_PointGroup - Creates a point group and selects specific points to
//		    be created given a set of parameters 
//
////////////////////////////////////////////////////////////////////////////

class GU_API GU_PointGroup : public GU_ElementGroup
{
public:
    /// Constructor and Destructor allocate and free a ptRefArray
    /// if mask isn't GEOPRIMALL
    /// NOTE: The constructor without a name allocates a detached group
    ///       that is destroyed by the destructor.
    GU_PointGroup(const GU_Detail *gdp, const GA_PrimitiveTypeId &id);
    GU_PointGroup(const char *n, GU_Detail *gdp, const GA_PrimitiveTypeId &id);
    GU_PointGroup(GU_Detail *gdp, GA_PointGroup *ptgroup);
    virtual ~GU_PointGroup();    

    virtual GA_ElementGroup	*newGroup(const char *name);
    virtual GA_ElementGroup	*find(const char *name) const;
    virtual void	 destroy(void);

    // actual method that allows you to generate a group given the 
    // set of parameters
    void                 generateGroup(const GU_GroupParms &parms); 

    // methods that allow you to select points to be in the group 
    void		 range(int, int, int, int, int, int) const; 
    void		 pattern(const char *pattern, int nelements,
				 int order) const;
    void		 patternGroup(const char *pattern, bool order) const;
    int			 boundingBox(float tx, float ty, float tz,
				     float halfx, float halfy, float halfz,
				     float rx, float ry, float rz,
				     bool includeNotWhollyContained);
    void		 boundingSphere(float tx, float ty, float tz, 
					float radx, float rady, float radz,
					bool includeNotWhollyContained) 
					const;
    void		 boundingObject(const GU_RayIntersect *rayTree) const;
    void		 boundingVolume(const GU_Detail *vgdp, fpreal iso, bool invert) const;

    void		 normal(UT_Vector3 &nml, float angle) const;
    void		 backface(const UT_Vector3 &eye) const;
    void		 degenerate(bool degenerate, bool zaf, bool doOpen,
				    float tol) const;

    void		 edgeDist(const GA_PointGroup *ptgroup, int depth) const;

protected:
    THREADED_METHOD_CONST(GU_PointGroup, gdp()->getNumPoints() > 5000,
		    unorderedBoundingBox
		    )
    void unorderedBoundingBoxPartial(
		    const UT_JobInfo &info) const;

    THREADED_METHOD6_CONST(GU_PointGroup, gdp()->getNumPoints() > 5000,
		    unorderedBoundingSphere,
		    float, a,
		    float, b,
		    float, c,
		    float, tx,
		    float, ty,
		    float, tz)
    void unorderedBoundingSpherePartial(
		    float a,
		    float b,
		    float c,
		    float tx,
		    float ty,
		    float tz,
		    const UT_JobInfo &info) const;

    THREADED_METHOD1_CONST(GU_PointGroup, gdp()->getNumPoints() > 5000,
		    unorderedBoundingObject,
		    const GU_RayIntersect *, rayTree)
    void unorderedBoundingObjectPartial(
		    const GU_RayIntersect *rayTree,
		    const UT_JobInfo &info) const;

    THREADED_METHOD3_CONST(GU_PointGroup, gdp()->getNumPoints() > 5000,
		    unorderedBoundVolume,
		    const UT_ValArray<const GEO_Primitive *> &, vlist,
		    float, iso,
		    bool, invert)
    void unorderedBoundVolumePartial(
		    const UT_ValArray<const GEO_Primitive *> &vlist, 
		    float iso, 
		    bool invert, 
		    const UT_JobInfo &info) const;

private:
    UT_BitArray			*myPtArray; // Point array for prim masks
    const char			*myName; // name of the group
    GA_PrimitiveTypeId		 myId;
    bool			 myAllPoints;
};

#endif
