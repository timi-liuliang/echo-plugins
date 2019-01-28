/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Lattice.h (C++)
 *
 * COMMENTS:
 *
 */


#ifndef __SOP_Lattice_h__
#define __SOP_Lattice_h__

#include "SOP_API.h"
#include "SOP_Node.h"

#include <GEO/GEO_PointTree.h>

#include <math.h>


class SOP_API sop_capturedata
{
public:
    GA_Offset	offset;
    float	weight;
};

class TS_MetaKernel;

class SOP_API SOP_Lattice : public SOP_Node
{
public:
	     SOP_Lattice(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Lattice();

    virtual OP_ERROR             cookInputGroups(OP_Context &context,
						 int alone = 0);
    virtual int			 isRefInput(unsigned int i) const;
       
    virtual bool		 updateParmsFlags();

    virtual bool		 unloadData();

    static OP_Node		*myConstructor(OP_Network *net,
						const char *name,
						OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    enum {
	SOP_LATTICE_LATTICE = 0,
	SOP_LATTICE_POINTCLOUD = 1
    };

protected:
    virtual OP_ERROR		 cookMySop(OP_Context &context);
    virtual OP_ERROR		 cookMyGuide1(OP_Context &context);
    virtual OP_ERROR		 cookMyGuide2(OP_Context &context);
    virtual const char		*inputLabel(unsigned idx) const;

private:
    void        getGroups(UT_String &str, fpreal t)
				{ evalString(str, 0, 0, t); }
    
    
    /// Correct any points we deformed during our last lattice pass
    /// that are no longer within our lattice's area of deformation. See
    /// cookMySop() for more details. This is for latticeDeform() only at the
    /// moment, since pointDeform() currently always duplicates our source and
    /// hence doesn't need to do this
    void	correctOldPoints(const GU_Detail *src, UT_BitArray &curr_pass);
    
    int		DEFORMTYPE()	{ return evalInt(1, 0, 0); }

    int		DIVSX(fpreal t)	{ return evalInt(2, 0, t); }
    int		DIVSY(fpreal t)	{ return evalInt(2, 1, t); }
    int		DIVSZ(fpreal t)	{ return evalInt(2, 2, t); }

    int		INTERPTYPE()	{ return evalInt(3, 0, 0); }

    void	RESTGROUP(UT_String &str, fpreal t)
				{ evalString(str, 4, 0, t); } 

    void	KERNEL(UT_String &str, fpreal t) { evalString(str, 5, 0, t); }
    float	RADIUS(fpreal t) { return evalFloat(6, 0, t); }
    float	NORMTHRESHOLD(void)		 { return evalFloat(7, 0, 0); }
    int		VISUALIZE(void) { return evalInt(8, 0, 0); }

    /// If we want the Lattice SOP to do a Lattice deformation, we call this
    /// helper method. The caller of this method is expected to lock and 
    OP_ERROR	latticeDeform	(OP_Context &context);

    /// If we want the Lattice SOP to deform the points individually, we call
    /// this helper method. The caller of this method is expected to lock and 
    /// unlock the inputs.
    OP_ERROR	pointDeform	(OP_Context &context);

    /// Helper method to build our point tree. Will only build if myPointTree
    /// is NULL. WARNING: Make sure this is called only within cookMySop(), 
    /// because it depends on myGroup, which will be invalid once cookMySop()
    /// exits if the group was adhoc
    void 	buildPointTree();

    /// Helper method to build our capture regions for our gdp's points. Will
    /// only build if myRegions is NULL. WARNING: Make sure this is called only 
    /// within cookMySop(), because it depends on myRestGroup, which will be 
    /// invalid once cookMySop() exits if the group was adhoc
    void	buildCaptureRegions(const GU_Detail *rest,
				    const TS_MetaKernel *kernel,
				    const UT_FloatArray &curr_radii);

    /// Calculates the radius for each point in our 'rest' input and stores it
    /// in curr_radii in the index corresponding to the point's number. Also 
    /// compares these new radii with the old ones in myOldRadii and returns
    /// true if any of one of them are different, false otherwise. This method
    /// will resize 'curr_radii' and 'myOldRadii' as necessary. WARNING: Make 
    /// sure this is called only within cookMySop(), because it depends on 
    /// myRestGroup, which will be invalid once cookMySop() exits if the group 
    /// was adhoc
    bool	calculateRadii(UT_FloatArray &curr_radii,
			       const GU_DetailHandle &rest_handle,
			       fpreal time);
    
    /// Small helper method to clean up the code. Deforms all points in source
    /// affected by point in the rest geo specified by 'index'. Note that
    /// 'index' is really the index for that point in 'diffarray' in
    /// pointDeform(), which is not necessarily the true index/number of the
    /// point (point->getNum()) when we have a rest group.
    inline void	deformSourcePointsForMe(int index, UT_Vector3 *diffarray);

    /// Small helper method to clean up the code. Creates the capture region for
    /// the point position in the rest geo specified by 'pos'.
    inline void createCaptureRegionForMe(int index, const TS_MetaKernel *kernel,
					 const UT_FloatArray &curr_radii,
					 UT_FloatArray &totals,
					 const UT_Vector3 &pos);
    
    int				 myPastedFlag;

    GEO_PointTreeGAOffset	*myPointTree;
    int			 	 myNumRegions;

    /// One per point of rest geo.  First int is number of captured points, then
    /// are the capture data.
    sop_capturedata	       **myRegions;
    
    int			 	 mySourceCookCount;
    int			 	 myRestCookCount;
    
    /// When we cook our guide geometry and want to draw spheres around the
    /// the guide's points to visualize the radius, we need to have the rest 
    /// group to iterate through (if one exists). But myRestGroup might be
    /// adhoc, in which case it will be destroyed after cookMySop() completes.
    /// So either we parse/create the point groups in the rest geo again in 
    /// cookMyGuide() (expensive), or we keep a simple array to remember the
    /// points' indices.
    GA_IndexArray		 myRestOffsetArray;
    
    UT_FloatArray	 	 myOldRadii;
    UT_String		 	 myOldMethod;
    float			 myOldThreshold;    // normalization threshold

    /// Keeps track of which points in our src we change
    UT_BitArray		 	 myLatticeDeformHistory;

    const GA_PointGroup		*myGroup;
    const GA_PointGroup		*myRestGroup;
};

#endif

