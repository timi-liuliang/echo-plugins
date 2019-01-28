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

#ifndef __GU_PrimGroup_h__
#define __GU_PrimGroup_h__

#include "GU_API.h"
#include <GEO/GEO_Primitive.h>
#include "GU_ElementGroup.h"
#include "GU_Detail.h"

////////////////////////////////////////////////////////////////////////////
//
//  GU_PrimGroup - Creates a primitive group and selects specific 
//		   primitives to be members given a set of parameters
//
////////////////////////////////////////////////////////////////////////////


class GU_API GU_PrimGroup : public GU_ElementGroup
{
public:
    /// Create a detached group that we own.
    GU_PrimGroup(const GU_Detail *gdp, const GA_PrimitiveTypeId &id)
	: GU_ElementGroup(SYSconst_cast(gdp))
	, myId(id)
    { newGroup(NULL); }
    GU_PrimGroup(const char *n, GU_Detail *gdp, const GA_PrimitiveTypeId &id)
	: GU_ElementGroup(gdp)
	, myId(id)
    { newGroup(n); }
    GU_PrimGroup(GU_Detail *gdp, GA_PrimitiveGroup *primGroup)
	: GU_ElementGroup(gdp)
    { 
	myName = primGroup->getName();
	setGroup(primGroup);
    }
    virtual ~GU_PrimGroup();

    GA_PrimitiveGroup *primGroup() const;

    virtual GA_ElementGroup	*newGroup(const char *name);
    virtual GA_ElementGroup	*find(const char *name) const;
    virtual void		 destroy(void);

    // Goes through the methods below with the given parameters and creates
    // a group.
    void                 generateGroup(const GU_GroupParms &parms);

    // methods that allow you to select objects to be in the group 
    void		 range(int num, int, int, int, int, int) const;
    void		 pattern(const char *pattern, int nelements, int) const;
    void		 patternGroup(const char *pattern, bool order) const;
    int			 boundingBox(float, float, float, float, float, float,
				     float, float, float, bool);
    void		 boundingSphere(float tx, float ty, float tz,
					float radx, float rady, float radz,
					bool includeNotWhollyContained)
					const;
    void		 normal(UT_Vector3 &nml, float angle) const;
    void		 backface(const UT_Vector3 &eye) const;
    void		 degenerate(bool degenerate, bool zaf, bool doOpen,
    				    float tol) const;

    void		 nonplanar(float tol) const;

private:
    /// The name of the group, or NULL if a detached group
    const char		*myName;
    GA_PrimitiveTypeId	 myId;

    int			 primInsideSphere(GEO_Primitive *prim, float, float, 
					  float, float, float, float, bool) const;

    bool		 primPartiallyInBBox(GEO_Primitive * prim) const;

    bool		 matchTypeId(const GEO_Primitive *prim) const;
};

#endif
