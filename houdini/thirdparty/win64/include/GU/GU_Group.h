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

#ifndef __GU_Group_h__
#define __GU_Group_h__

#include "GU_API.h"

#include <UT/UT_BoundingBox.h>
#include <UT/UT_VectorTypes.h>
#include <GA/GA_Types.h>

class UT_String;
class GU_Detail;
class GU_RayIntersect;
class GA_PointGroup;
class GA_Group;

//////////////////////////////////////////////////////////////////////////////
//
// The parameters used to create a specified group.
//
//////////////////////////////////////////////////////////////////////////////

class GU_API GU_GroupParms
{
public:
     GU_GroupParms()
     {
    	 useBounding = 0;
    	 useNumbering = 0;
	 useNormal = 0;
	 useBackface = 0;
	 useEdgeDist = 0;
	 useDegenerate = 0;
	 origptgroup = 0;
	 edgedepth = 0;
	 useNonPlanar = 0;
	 isoOffset = 0.0;
	 invertVolume = false;
	 includeNotWhollyContained = false;
	 boundGdp = 0;
	 pattern = pattern_group = 0;
     }
    ~GU_GroupParms() {}

    const char		*pattern;
    const char		*pattern_group;
    int			 ordered;
    int			 start;
    int			 end;
    int			 incStart;
    int			 incEnd;
    float		 radx, rady, radz;
    float		 tx, ty, tz;
    float		 rx, ry, rz;
    float		 nx, ny, nz;		// Normal
    UT_Vector3		 eye;			// Eye position for backface
    float		 spreadAngle;
    int			 boundingType;
    int			 numberingType;
    int			 useBounding;
    int			 useNumbering;
    int			 useNormal;
    int			 useBackface;
    int			 booleanType;
    int			 useEdgeDist;
    const GA_PointGroup	*origptgroup;
    int			 edgedepth;
    GU_RayIntersect	*rayTree;
    const UT_String	*group1;
    const UT_String	*group2;
    int			 useNonPlanar;
    float		 nonPlanarTol;
    int			 useDegenerate;
    bool		 useDegenPrims;
    bool		 useZAF;		// zero area faces
    bool		 doOpenZAF;		// treat open faces as closed
    float		 tolZAF;
    float		 isoOffset;
    bool		 invertVolume;
    bool		 includeNotWhollyContained;
    const GU_Detail	*boundGdp;
};

//////////////////////////////////////////////////////////////////////////////
// 
// The enumeration of the possible group boolean operations.
// 
//////////////////////////////////////////////////////////////////////////////

enum GU_GroupBoolOp
{
    GU_GROUP_BOOLOP_NONE  = 0,
    GU_GROUP_BOOLOP_OR    = 1,
    GU_GROUP_BOOLOP_AND   = 2,
    GU_GROUP_BOOLOP_XOR   = 3,
    GU_GROUP_BOOLOP_MINUS = 4
};

//////////////////////////////////////////////////////////////////////////////
//
//  This class is a group constructor class.  As most of the functionality
//  are common to both point and primitive groups it exists as a templated
//  class.  It modifies a single GA_Group which is created upon construction 
//  of the subclasses GU_PointGroup and GU_PrimGroup.  Use generateGroup to
//  create a group with the given parameters.
//
//  To construct your own operations if they are common to both points and
//  primitives place them here in the protected section.  
//
//////////////////////////////////////////////////////////////////////////////

class GU_API GU_Group
{
public:
    GU_Group(GU_Detail *gdp);
    virtual ~GU_Group();

    virtual GA_Group	*newGroup(const char *) = 0;
    virtual GA_Group	*find(const char *) const = 0;
    virtual void		 destroy(void) = 0;
    

    GA_Group	*group() const 		{ return myGroup; }

    // Select items based on a boolean operation done between one or two
    // different groups (e.g. intersect, union, subtraction, negation).
    // Returns false if any error was encountered and true otherwise.
    virtual bool	 boolean(GU_GroupBoolOp boolop, 
				 const UT_String &group1, bool negate1,
				 const UT_String &group2, bool negate2) const;

    virtual bool	 boolean(GU_GroupBoolOp boolop, 
				 GA_Group *group1, bool negate1,
				 GA_Group *group2, bool negate2) const;

protected:
    // this method actually creates the group given the current parms
    virtual void		 generateGroup(const GU_GroupParms &parms) = 0;

    //
    // methods that allow you to select objects to be in the group 
    //

    // Selects specific items given a start, and end number as well as
    // a step and an increment so you can select every 2 of 3
    virtual void		 range(int, int, int, int, int, int) const=0;
    	    void		 getRangeValues(int, int &, int &, 
						     int &, int &) const;


    // Selects specific items by a pattern i.e. 1-10:2,3 (1 to 10 every 2 of 3)
    virtual void		 pattern(const char *pattern, int nelements, 
					 int order) const=0;
    // Like pattern, but uses the GOP Group Parser to act the same
    // as the rest of our groups.
    virtual void		 patternGroup(const char *pattern, bool order) const=0;

    // Select specific items within the given bounding box 
    virtual int			 boundingBox(float, float, float,
				 	     float, float, float,
					     float, float, float,
					     bool);


    // Select specific items within the given bounding sphere 
    virtual void		 boundingSphere(float, float, float,
						float, float, float,
						bool) const = 0;

    // Select just those items with the given normal and an angle about
    // that normal.
    virtual void		 normal(UT_Vector3 &nml, float angle) const = 0;

    /// Select just those items that are degenerate, or are zero area faces.
    /// Set degenerate flag to select degenerates.  Set the zaf flag to
    /// select zero area faces.  Set the doOpen flag to treat open faces
    /// as closed.  Set tol for a tolerance used in detecting degeneracies.
    virtual void		 degenerate(bool degenerate, bool zaf,
    					    bool doOpen, float tol) const = 0;
    

    static int			 isBackface(UT_Vector3 pos,
					    UT_Vector3 nml,
					    UT_Vector3 eye);


    // Used by the subclasses for the bounding routines to determine if a
    // specified point lies within a given sphere
    bool 			 pointInsideSphere(const GU_Detail *gdp,
						   GA_Offset ptoff,
				    	 	   float a, float b, float c,
				    		   float tx, float ty, float tz)
						   const;

    // methods to access the private infomation
    GU_Detail		*gdp() const		{ return myGdp; }
    UT_BoundingBox	*getBBox() const	{ return myBBox; }

    // methods to set the private data
    void			 setGroup(GA_Group *grp) { myGroup = grp; }
    void			 deleteBBox();


private:

    GA_Group		*myGroup; // The group being constructed
    UT_BoundingBox	*myBBox; // Bounding box
    GU_Detail		*myGdp; // Detail containing the group
};

#endif
