/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_CaptureOverride.h (SOP Library, C++)
 *
 * COMMENTS:	Implements a SOP for manipulating weight overrides
 *
 */

#ifndef __SOP_CaptureOverride_h__
#define __SOP_CaptureOverride_h__

#include "SOP_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_VectorTypes.h>
#include <PRM/PRM_Name.h>
#include <PRM/PRM_Parm.h>

#include "SOP_CaptureData.h"
#include "SOP_Node.h"


#define SOP_CAPTUREOVERRIDE_GRP_IDX	0

class OP_Context;

struct GEO_CaptureBoneStorage;
class GEO_CaptureOverrideData;
class GEO_CaptureOverrideDelta;
class SOP_UndoCaptureOverride;

class SOP_API SOP_CaptureOverride : public SOP_Node
{
friend class SOP_UndoCaptureOverride;
friend class SOP_UndoCaptureOverrideOpDepend;

public:
    SOP_CaptureOverride(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_CaptureOverride();

    const UT_String &	getUndoLabel() const { return myNewOpLabel; }

    static OP_Node *	ourConstructor(OP_Network*, const char*, OP_Operator*);
    static PRM_Template	ourTemplateList[];

    virtual bool	updateParmsFlags() override;

    // This function allows an external caller to tell us to start a new
    // operation.  This involves apply the current operaation and starting
    // a new one.
    void		beginNewOperation();

    virtual OP_ERROR	cookInputGroups( OP_Context &context,
				int alone ) override;

    // Rebuild the group and/or the selection if necessary:
    OP_ERROR		updateInputGroups( OP_Context &context,
				   bool changed_input, bool changed_group );

    // This will rebuild the selection if it is necessary to do so.  It
    // should be called in code paths where we don't do cookInputGroups.
    void		updateCookSelection(
				   bool changed_input, bool changed_group );
 
    // Checks if the sop currently overrides the capture weights for the
    // specified point.  Note that this has to take into account both the
    // current and temporary overrides.
    bool		hasWeightOverride( int point_num ) const;

    void		startNewChange( const int *points, int num_points,
					int clamp_negative, int normalize = 0 );
    void		changeWeight( fpreal t, const char *cregion,
				      float weight, int add_weight );
    void		changeWeight( fpreal t,
				      const UT_ValArray<const char *> &cregions,
				      float weight, int add_weight );
    void		normalizeWeights( fpreal t,
			    const UT_ValArray<const char *> &cregions );

    void		createCaptureWeightOverride( const int *points,
						     int num_points, fpreal t );
    void		removeCaptureWeightOverride( const int *points,
						     int num_points, fpreal t );

    int			addAverageCaptureWeight( OP_Context &context,
						 const UT_IntArray &points,
						 UT_StringMap<float *> &averages );

    virtual void	setVisualization( int onoff ) override;

protected:
    void	applyPermanentOverrides(OP_Context *ctx = 0);

    virtual OP_ERROR	cookMySop( OP_Context &context ) override;

    void	applyAndClearCurrentDelta();

    void	clearAllDeltas(fpreal t);

    virtual void	buildOpDependencies() override;
    virtual void	buildParmDependency( int parm_index ) override;
    virtual void	handleOpDependency( int referenced_op_id,
				const OP_Dependency &op_dep,
				OP_InterestType interest_type,
				bool &need_cook,
				const char *old_fullpath,
				const char *old_cwd,
				const char *old_chan_name ) override;

    //
    //  I/O methods      
    //                           
    virtual OP_ERROR    save(std::ostream &os, const OP_SaveFlags &flags,
			      const char *path_prefix,
			      const UT_String &name_override = UT_String()
			) override;
    virtual bool	load(UT_IStream &is, const char *extension,
			      const char *path=0) override;

private:    // methods
    PRM_Parm &		getParmByName( PRM_Name &name )
				{ return getParm( name.getToken() ); }
    const PRM_Parm &	getParmByName( PRM_Name &name ) const
				{ return getParm( name.getToken() ); }

    void		fixParmCRegionPathsForRootChange(
				    const UT_String &old_full_root_path,
				    const UT_String &full_root_path );

    void		setCRegionsParm( fpreal t,
				const UT_ValArray<const char *> &cregions );

    OP_ERROR		createCurrentDelta( OP_Context &context,
					    const GA_PointGroup *pt_group );

    int			addCaptureRegion( UT_String &cregion_name );

    // parm callbacks
    static void		buildCregionMenu(
				void *, PRM_Name *, int,
				const PRM_SpareData *, const PRM_Parm * );
    static int		getCaptureRegionDetailData( void *user_data,
					    char *full_cregion_path,
					    char *rel_cregion_path,
					    GEO_CaptureBoneStorage &tube_data );

    // Get the selection group string:
    void		getSelString(UT_String &str)
	    { evalString(str, SOP_CAPTUREOVERRIDE_GRP_IDX, 0, 0.0f); }

    // Apply current delta to permanent overrides
    static int		applyStatic(void *, int idx,
				    fpreal t, const PRM_Template *);
    // Respond to button press to clear all overrides
    static int		clearallStatic(void *, int idx,
				       fpreal t, const PRM_Template *);
    
    static int      	openSpreadsheetStatic(void *, int idx,
				       fpreal t, const PRM_Template *);

    void		checkCaptureRegions();
    
    bool		checkChangedSource(unsigned idx, OP_Context &context);

    void		rollbackCurrentOverridesDelta();

    void		ensureCaptureAttributesExist();

    // Undo/Redo
    void		undoOverrideDeltas(
			    GEO_CaptureOverrideDelta *delta);
    void		redoOverrideDeltas(
			    GEO_CaptureOverrideDelta *delta);
    void		undoClearOverrides(
			    SOP_UndoCaptureOverride *undo);
    void		redoClearOverrides(
			    SOP_UndoCaptureOverride *undo);

private:    // data
    UT_String myNewOpLabel; // label for undos

    GEO_CaptureOverrideData *myPermanentOverrides;
    GEO_CaptureOverrideDelta *myCurrentOverridesDelta;

    bool myCookedFlag; // have we cooked yet?
    bool myApplyPermanentFlag;

    // Selection group
    const GA_ElementGroup *myGroup;

    SOP_CaptureData myCaptureData;
    UT_String       myRootPath;
    unsigned int    myUpdatedColors : 1,
                    myForceUpdateColors : 1;

    static PRM_ChoiceList   theCregionMenu;
    static int              theCregionIndex;
};

#endif // __SOP_CaptureOverride_h__

