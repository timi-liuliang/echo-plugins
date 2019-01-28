/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_CaptureBase.h ( SOP Library, C++)
 *
 * COMMENTS:	Rudimentary base class for the Capture and ProximityCapture
 *		classes to share some functions. Since they started out as
 *		separate sources, only minimal commonalities have been 
 *		abstracted.
 */

#ifndef __SOP_CaptureBase__
#define __SOP_CaptureBase__

#include "SOP_API.h"
#include <GEO/GEO_CaptureData.h>
#include "SOP_Node.h"

class PRM_RefId;
class SOP_CaptureRegion;

class SOP_API SOP_CaptureBase : public SOP_Node
{
    friend class SOP_UndoCaptureOpDepend;

public:
    SOP_CaptureBase( OP_Network *parent, const char *name, OP_Operator *entry );
    virtual ~SOP_CaptureBase();

    virtual void	setVisualization( int onoff ) override
			{
			    // only trigger recooks if different!
			    if( myVisualization != onoff )
			    {
				myVisualization = onoff;
				forceRecook();
			    }
			}

    virtual void	setForceCapture( int onoff ) override
			{
			    // only trigger recooks if different!
			    if( myForceCapture != onoff )
			    {
				myForceCapture = onoff;
				if( onoff )
				    forceRecook();
			    }
			}

    static void	 buildObjectMenu(void *, PRM_Name *, int, void *, PRM_Parm *);

    virtual int		changeString(const char *from, const char *to,
				bool fullword) override;

    virtual void	resolveObsoleteParms(
				PRM_ParmList *obsolete_parms) override;

protected:  // methods

    // cregion methods
    enum {
	SOP_REGION_DISPLAY = 0,
	SOP_REGION_RENDER,
	SOP_REGION_CREGION
    };

    enum {
	SOP_INPUT_GEOMETRY = 0,
	SOP_INPUT_CREGION,
	SOP_NUM_INPUTS
    };

	// override this to do special cases, make sure you call the base class
	// first before adding your own dependencies!
    virtual void	buildParmDependency( int parm_index ) override;
    virtual void	handleOpDependency( int referenced_op_id,
				const OP_Dependency &op_dep,
				OP_InterestType interest_type,
				bool &need_cook,
				const char *old_fullpath,
				const char *old_cwd,
				const char *old_chan_name ) override;

    // This will be called if the timeInterest flag is set.
    // This will return true if the node needs to be dirtied for cooking
    virtual bool	handleTimeChange(fpreal t) override;

    virtual void	resetCapture();

    virtual OP_Node *	getRootObjNode() = 0;
    virtual int		getCaptureSopMethod() = 0;
    virtual int		getDoSubnets() = 0;
    virtual float	getCaptureFrame() = 0;
    virtual bool	useCapturePose() = 0;

    static int		forceRecapture( void *data, int index, fpreal t,
					const PRM_Template * );

    void		getCregionNodes( OP_Node *root, OP_NodeList &nodes );
    void		getCregionNodesFromString( const UT_String &str,
						   OP_NodeList &nodes );
    SOP_Node *		getRegionNode( OP_Node *bone,
				       bool add_extra_input = false );

    SYS_DEPRECATED(12.0) bool	getRegionData( fpreal t,
				       const OP_Node *region_sop,
				       const GEO_Detail *region_gdp,
				       const GA_ROAttributeRef &capt_atr_offset,
				       const GEO_Primitive *tube_prim,
				       float *atr_data,
				       UT_Matrix4 &xform );

    bool		getRegionData( fpreal t,
					const OP_Node *region_sop,
					const GA_ROAttributeRef &capt_atr_offset,
					const GEO_Primitive *tube_prim,
					UT_Matrix4 &xform,
					float &bcap, float &tcap,
					float &taperx, float &taperz,
					float &iweight, float &oweight);

    GU_DetailHandle	getRegionGdh( OP_Context &context, int region_idx,
				      int add_error = 1 );
    bool		validateCaptureRegions( OP_Context &context );

protected:  // data

    GEO_CaptureData	myCaptureData;	// transferred to gdp when cooked
    int			myNumPtsAtCaptureFrame;

    unsigned		myVisualization : 1,
			myForceCapture : 1;

private:    // methods
    void		buildCRegionDependencies(const PRM_RefId &ref,
						 OP_Node *node);
    // traverses the network up from each of the "nodes" and finds 
    // the instances of SOP_CaptureRegion and returns them in the "cregions"
    void		getCaptureRegionInstances( const OP_NodeList &nodes,
				   UT_ValArray< SOP_CaptureRegion *> &cregions);
    
private:    //data
    bool		myOldCaptureOverride; //used for restoring old override

};

#endif // __SOP_CaptureBase__
