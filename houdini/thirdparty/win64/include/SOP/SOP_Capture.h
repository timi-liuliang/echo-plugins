/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Capture.h ( SOP Library, C++)
 *
 * COMMENTS:    The Capture SOP
 * 		Assigns capture weighting between surface points and
 * 		deformation spheres
 */

#ifndef __SOP_Capture__
#define __SOP_Capture__

#include "SOP_API.h"
#include <UT/UT_IntArray.h>
#include <OP/OP_ParmMacros.h>
#include <PRM/PRM_Include.h>

#include "SOP_CaptureBase.h"

enum SOP_CaptureIndices
{
    I_PNTGRPS = 0,
    I_REGIONSOP,
    I_RBONE,
    I_CAPTFRAME,
    I_WEIGHTFRM,
    I_COLOR,
    I_ZEROWEIGHTCOLOR,
    I_ORFILE,
    I_SVFILE,
    I_AUTOINCR,
    I_NORMALIZEWEIGHT,
    I_DESTROYWEIGHTS,
    I_BLENDFACTOR,
    I_DOSUBNETS,
    I_RELSKELROOT,
    I_USECAPTPOSE,
    I_N_SOPCAPTURE_INDICES	// should remain the last in this list
};

class OP_Node;
class GEO_PrimTube;

class sop_CRegionIterator;
class sop_CRegionTree;

//________________________________________________________________________
//
// The SOP_Capture class
//
//________________________________________________________________________

class SOP_API SOP_Capture : public SOP_CaptureBase
{
public:
		 SOP_Capture(OP_Network*, const char *, OP_Operator*);
    virtual	~SOP_Capture() {};

    virtual int		 isRefInput(unsigned int i) const;
    static OP_Node	*myConstructor(OP_Network*, const char *, OP_Operator*);
    static PRM_Template	 myTemplateList[];
    static PRM_Template	 myObsoleteList[];

    virtual bool	 updateParmsFlags();

    void setUseAllRegions(bool value);

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
    virtual OP_ERROR	 cookMyGuide1(OP_Context &context);
    virtual const char	*inputLabel(unsigned idx) const;

    virtual void	 resetCapture();

    virtual OP_Node *	getRootObjNode();
    virtual int		getCaptureSopMethod() { return CAPTURE_REGION_SOP(); }
    virtual int		getDoSubnets()
				    { EVAL_INT("dosubnets",I_DOSUBNETS,0,0); }
    virtual float	getCaptureFrame()
			    { EVAL_FLT("captframe",     I_CAPTFRAME,0,0);}
    virtual bool	useCapturePose()
			    { EVAL_INT("usecaptpose", I_USECAPTPOSE,0,0);}

private:

//
// UI callback methods
//
    static int   saveAllOverrideCB( void *data, int index, fpreal t,
				    const PRM_Template *);
    static int   saveSelectedOverrideCB( void *data, int index, fpreal t,
				    const PRM_Template *);
    void	 saveOverrideFile  (int use_selection);

//
// Parameter access callbacks
//
    void POINT_GROUPS( UT_String &str )   
	   { EVAL_STR((UT_String)PRMgroupName.getToken(),     I_PNTGRPS,0,0);}
    int  CAPTURE_REGION_SOP()	  { EVAL_INT("captregionsop", I_REGIONSOP,0,0);}
    void ROOT_PATH(UT_String &str){ EVAL_STR("rootpath",      I_RBONE,0,0); }

    int RELATIVE_SKEL_ROOT() { EVAL_INT("relativeskel", I_RELSKELROOT,0,0); }

    int WEIGHTFROM()		  { EVAL_INT("weightFrom",    I_WEIGHTFRM,0,0);}
    int NORMALIZEWEIGHT()	  { EVAL_INT("normweights",   I_NORMALIZEWEIGHT,0,0);}
    int DESTROYWEIGHTS()	  { EVAL_INT("destroyweights", I_DESTROYWEIGHTS, 0, 0); }
    float BLENDFACTOR(fpreal t)	  { EVAL_FLT("blendfactor", I_BLENDFACTOR, 0, t); }
    int COLORTYPE()		  { EVAL_INT("color",         I_COLOR,0,0);}

    float ZEROWEIGHTCOLOR_R()	{ EVAL_INT("zeroweightcolor", I_ZEROWEIGHTCOLOR,
					    0, 0); }
    float ZEROWEIGHTCOLOR_G()	{ EVAL_INT("zeroweightcolor", I_ZEROWEIGHTCOLOR,
					    1, 0); }
    float ZEROWEIGHTCOLOR_B()	{ EVAL_INT("zeroweightcolor", I_ZEROWEIGHTCOLOR,
					    2, 0); }

    int AUTOINCR()		  { EVAL_INT("autoincr",      I_AUTOINCR,0,0);}

    void OVERRIDEFILE(UT_String &str){ EVAL_STR("captfile",    I_ORFILE,0,0);}
    void SAVEFILE(UT_String &str)    { EVAL_STR("savefile",    I_SVFILE,0,0);}
    CH_StringMeaning SAVEFILEmeaning()
				{ return getStringMeaning("savefile",0,0.0f);}

    void setSaveFile(UT_String &str, CH_StringMeaning meaning)
				{SET_STRING(str, meaning, "savefile",
					    I_SVFILE,0,0);}

//
// Private internal methods
//
    // this hardens the filename string in the process.
    void		 bumpFileName(UT_String &filename) const;

    // obtain the nodes refered to in the parameter (root and extra regions)
    void		getAllCregionNodes( OP_NodeList &nodes );

    // add all regions to internal data structure. 
    // returns 0 on error, 1 on success
    bool		createRegions(OP_Context &context,
				      const OP_NodeList &nodes,
				      sop_CRegionIterator &cregion_iter);
    bool		addRegions(OP_Node *region_node, OP_Context &context,
				   sop_CRegionIterator &cregion_iter);
    bool		createExtraRegions(OP_Context &context,
					   sop_CRegionIterator &cregion_iter);

    //
    // for point weighting computations.  Returns 1 on success, 0 otherwise
    //
    void 		 weightPoint(OP_Context &context, GA_Index pt_num,
				     const UT_Vector3 &pt_pos,
				     const UT_IntArray &regions);
    bool		 weightPoints(OP_Context &context,
				      const GA_PointGroup *pt_group,
				      const sop_CRegionTree &region_tree);

    void 		 colorPoints(OP_Context &context);

// returns 1 on success, 0 otherwise
    int	 	 	 buildGuide(OP_Context &context );

    void		 setAllCaptureRegions(UT_IntArray& regions);

//
// Private data
//


// indirect array is for optimizing UI parameter access
    static int			*parmIndirect;

    // If this is true, then every cregion specified will have
    // attributes created for it, even if some points lie outside
    // of its bbox. False by default.
    bool			myCaptureAllRegions;
};

#endif

