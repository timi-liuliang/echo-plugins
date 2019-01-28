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

#ifndef __GU_EdgeGroup_h__
#define __GU_EdgeGroup_h__

#include "GU_API.h"
#include "GU_Group.h"
#include "GU_Detail.h"

////////////////////////////////////////////////////////////////////////////
//
//  GU_EdgeGroup - Creates a Edge group and selects specific points to
//		    be created given a set of parameters 
//
////////////////////////////////////////////////////////////////////////////

class GU_API GU_EdgeGroup : public GU_Group
{
public:
    /// Create a detached group that we own.
    GU_EdgeGroup(const GU_Detail *gdp, const GA_PrimitiveTypeId &id);
    GU_EdgeGroup(const char *n, GU_Detail *gdp, const GA_PrimitiveTypeId &id);
    GU_EdgeGroup(GU_Detail *gdp, GA_EdgeGroup *ptgroup);
    virtual ~GU_EdgeGroup();    

    virtual GA_Group	*newGroup(const char *name);
    virtual GA_Group	*find(const char *name) const;
    virtual void	 destroy(void);

    virtual bool	 boolean(GU_GroupBoolOp boolop, 
				 const UT_String &group1, bool negate1,
				 const UT_String &group2, bool negate2) const;

    virtual bool	 boolean(GU_GroupBoolOp boolop, 
				 GA_Group *group1, bool negate1,
				 GA_Group *group2, bool negate2) const;

    GA_EdgeGroup	*edgeGroup() const;

    // actual method that allows you to generate a group given the 
    // set of parameters
    void                 generateGroup(const GU_GroupParms &parms); 

    // methods that allow you to select points to be in the group 
    void		 range(int, int, int, int, int, int) const; 
    void		 pattern(const char *pattern, int nelements,
				 int order) const;
    void		 patternGroup(const char *pattern, bool order) const;

    void		 boundingSphere(float tx, float ty, float tz, 
					float radx, float rady, float radz,
					bool includeNotWhollyContained) 
					const;

    void		 normal(UT_Vector3 &nml, float angle) const;

    void		 degenerate(bool degenerate, bool zaf, bool doOpen,
				    float tol) const;
    
    int			 boundingBox(float tx, float ty, float tz,
				     float halfx, float halfy, float halfz,
				     float rx, float ry, float rz, bool includeNotWhollyContained);    

    void		 backface(const UT_Vector3 &eye) const;
   
    bool		 isEdgeInsideBBox(const GA_Edge & edge, 
					  bool includeNotWhollyContained) const;

    bool		 isEdgeInsideSphere(const GA_Edge & edge,
					    float rx2, float ry2, float rz2,
					    float tx, float ty, float tz,
					    bool includeNotWhollyContained) const;

private:
    const char		*myName; // name of the group
    GA_PrimitiveTypeId	 myId;

    bool		 matchTypeId(const GA_Primitive *prim) const;
};

#endif
