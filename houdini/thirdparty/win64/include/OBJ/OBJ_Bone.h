/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Bone.h ( Object Library, C++)
 *
 * COMMENTS:
 *		Object that implements bones
 */

#ifndef __OBJ_Bone__
#define __OBJ_Bone__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_XformOrder.h>
#include <SYS/SYS_Inline.h>

class UT_String;
class SOP_CaptureRegion;

#define BONESTATE_CHOPNET "KIN_Chops"

// NOTE: IF YOU ADD ANYTHING HERE, YOU MUST UPDATE
//	 THE switcher[] INSIDE OBJ_Bone.C
enum OBJ_BoneIndex
{
    // Bone
    I_LINK = I_N_GEO_INDICES,
    I_REST_ANGLE,
    I_LENGTH,
    I_SOLVER,
    I_DAMPEN,
    I_XRANGE,
    I_XDAMP,
    I_XROLLOFF,
    I_YRANGE,
    I_YDAMP,
    I_YROLLOFF,
    I_ZRANGE,
    I_ZDAMP,
    I_ZROLLOFF,

    // Capture
    I_CAPTURE,

    // Capture-Deform SOPs
    I_CRSWITCHER,
    I_CCRCENTER,
    I_CCRROTATE,
    I_CCRSCALE,
    I_CCRTOPHEIGHT,
    I_CCRTOPCAP,
    I_CCRBOTHEIGHT,
    I_CCRBOTCAP,
    I_CRCENTER,
    I_CRROTATE,
    I_CRSCALE,
    I_CRTOPHEIGHT,
    I_CRTOPCAP,
    I_CRBOTHEIGHT,
    I_CRBOTCAP,
    // Capture Bone Pose Xform (translate, rotate, scale)
    I_LABEL_1,
    I_CAPTPOSE_LENGTH,
    I_CAPTPOSE_T,
    I_CAPTPOSE_R,
    I_CAPTPOSE_S,
    
    I_N_BONE_INDICES		// should always be last in the list
};

#define EVAL_FLOAT(name, idx, vi, t)	\
		return evalFloat(name, &getIndirect()[idx], vi, t);
#define EVAL_FLOAT_T(name, idx, vi, t, thread)	\
		return evalFloatT(name, &getIndirect()[idx], vi, t, thread);
#define EVAL_FLOATS(name, idx, v, t)	\
		evalFloats(name, &getIndirect()[idx], v, t);
#define EVAL_FLOATS_T(name, idx, v, t, stid)	\
		evalFloatsT(name, &getIndirect()[idx], v, t, stid);
#define EVAL_INT(name, idx, vi, t)	\
		return evalInt(name, &getIndirect()[idx], vi, t);
#define EVAL_STR(name, idx, vi, t)	\
		evalString(str, name, &getIndirect()[idx], vi, t);

#define SET_FLOAT(name, parm_index, vector_index, t, val) \
	    setFloat(name, getIndirect()[parm_index], vector_index, t, val)
#define SET_FLOATREF(name, vector_index, t, val) \
	    setChRefFloat(getParmList()->getParmIndex(name), vector_index, t, val)
#define SET_INT(name, parm_index, vector_index, t, val) \
	    setInt(name, getIndirect()[parm_index], vector_index, t, val)
#define SET_STRING(val, meaning, name, parm_index, vector_index, t) \
	    setString(val, meaning, name, getIndirect()[parm_index], \
		      vector_index, t)

class SOP_Node;
class CHOP_Node;

class OBJ_API OBJ_Bone : public OBJ_Geometry
{
public:
    static UT_XformOrder	 XFORM_ORDER;
    enum KinOverride {
	KIN_OVERRIDE_NONE,
	KIN_OVERRIDE_REST,
	KIN_OVERRIDE_CAPTURE,
	NUM_KIN_OVERRIDES
    };

	    			 OBJ_Bone(OP_Network *, const char *,
					  OP_Operator *);
    virtual			 ~OBJ_Bone();

    virtual int			 isObjectRenderable(fpreal t) const;
    void			 setIsObjectRenderable(bool val);

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);
    virtual void		 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);

    CHOP_Node *			 getKinChop();
    void			 cleanBoneTransform( OP_Context &context );

    SOP_Node *			 getBoneSOP() const;

    virtual bool	getParmTransform(OP_Context &context,
					  UT_DMatrix4 &xform);
    virtual bool	getParmTransformRotates(OP_Context &context,
					    UT_DMatrix4 &xform );
    virtual bool	getParmTransformTranslates(OP_Context &context,
					    UT_DMatrix4 &xform );
    virtual bool	getParmTransformScales(OP_Context &context,
					    UT_DMatrix4 &xform );
    
    // capture transform relative to "to" node (float and double versions)
    virtual int         getRelativeCaptureTransform( OP_Node &to,
						     UT_Matrix4 &xform,
						     OP_Context &context );
    virtual int         getRelativeCaptureTransform( OP_Node &to,
						     UT_DMatrix4 &xform,
						     OP_Context &context );

    // get the transform such that bone is transformed to capture position
    // NB: this is a local to world transform (but for the capture pose)
    virtual void	getCaptureParmTransform( OP_Context &context,
						 UT_DMatrix4 &mat );
    // sets the capture trs parameters so that they represent the given xform
    // NB: this is a local to world transform (but for the capture pose)
    virtual void	setCaptureParmTransform( OP_Context &context, 
						 const UT_DMatrix4& xform );
    // next two methods obtain local to world transform of the animation
    // bone pose (independently of the current override: that is
    // they return what getLocalToWorldTransform would in the OVERRIDE_NONE)
    void		getAnimLocalToWorldTransform( OP_Context &context,
						      UT_DMatrix4 &mat );
    void		getAnimInverseLocalToWorldTransform(OP_Context &context,
						            UT_DMatrix4 &mat );


    // set the given world position by only modifying the pre-transform
    virtual int		keepWorldTransform( 
				    OP_Context &context, 
				    const UT_DMatrix4 &world_xform );

    // obtains the transformation from the reference frame associated with
    // the rest position, to the world
    int			getChildToWorldRestTransform(const char *frame, 
				    UT_DMatrix4 &xform, fpreal t)
			{
			    return getCustomChildToWorldTransform( frame, t,
								    xform);
			}   
    bool		getObjectToChainParentTransform( 
						OBJ_Bone *start_bone,
						UT_DMatrix4 &xform, fpreal t );
	    
    // methods to set the bone-specific parm values

    void		 setUseSolver( int onoff )
			     { myUseSolverFlag = onoff ? 1 : 0; }
    int			 getUseSolver() const
			     { return myUseSolverFlag; }
    int			 isUsingSolver() const
			     { return myIsUsingSolverFlag; }

    // sets the capture parameters for the bone.
    // Capture parameters "freeze" the current parameters by storing them
    // (or accumulation of them) in the capture parameters. The capture
    // parameters are used for capturing geometry by bone's capture regions.
    // They are also used for displaying the capture pose when
    // kinematic override is set to "CAPTURE"
    void		 setCaptureParameters(OP_Context &context,
					      UT_String &errors);

    // sets the capture parameters so that bone capture position
    // is the same as the animation position. Also aligns the
    // capture and deform pills (ie capture and animate regions) of each
    // capture region contained by the bone.
    // NB: both bone and capture region capture parameters are changed
    //     to aligh capture region to deform region.
    void		 alignRegionCaptureWithDeform(OP_Context &context,
						      UT_String &errors);

    // adjust the cregions within the bone so that their animation
    // region is aligned with capture region in the world space (through
    // the use of the SOP rig adjustment xform)
    // NB: If keep_bone_pos is true, only the rig adjustment parameters are
    //     changed to aligh animation (deform) region with capture region. Bone
    //     parameters do not change at all.
    void		 alignRegionDeformWithCapture(OP_Context &context,
						      bool keep_bone_pos,
						      UT_String &errors);
    void		 alignRegionDeformWithCapture(OP_Context &context,
						      UT_String &errors);

    // set the parameters
    void  		 setLinkDisplay(int onOff)
			     { SET_INT("displaylink", I_LINK, 0, 0, onOff); }
    void		 setLength(fpreal t, fpreal v)
			     { SET_FLOAT("length", I_LENGTH, 0, t, v); }
    void		 setRestX(fpreal t, fpreal v)
			     { SET_FLOAT("R", I_REST_ANGLE, 0, t, v); }
    void		 setRestY(fpreal t, fpreal v)
			     { SET_FLOAT("R", I_REST_ANGLE, 1, t, v); }
    void		 setRestZ(fpreal t, fpreal v)
			     { SET_FLOAT("R", I_REST_ANGLE, 2, t, v); }

    void		 setSOLVER( const char *solver,
				    CH_StringMeaning meaning )
			     {SET_STRING(solver, meaning, "solver",
					 I_SOLVER, 0, 0.0f);}

    void		 setXMIN(fpreal t, fpreal v)
			     { SET_FLOAT("xrange", I_XRANGE, 0, t, v); }
    void		 setXMAX(fpreal t, fpreal v)
			     { SET_FLOAT("xrange", I_XRANGE, 1, t, v); }
    void		 setYMIN(fpreal t, fpreal v)
			     { SET_FLOAT("yrange", I_YRANGE, 0, t, v); }
    void		 setYMAX(fpreal t, fpreal v)
			     { SET_FLOAT("yrange", I_YRANGE, 1, t, v); }
    void		 setZMIN(fpreal t, fpreal v)
			     { SET_FLOAT("zrange", I_ZRANGE, 0, t, v); }
    void		 setZMAX(fpreal t, fpreal v)
			     { SET_FLOAT("zrange", I_ZRANGE, 1, t, v); }

    // 0 is skeleton SOP type, 1 is Capture/Deform type
    void  		 setCaptureDisplay(int onOff)
			 { SET_INT("displaycapture", I_CAPTURE, 0, 0, onOff); }

    void		 setCRcenter(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOAT("crcenter",    I_CRCENTER,  0, t, x);
			       SET_FLOAT("crcenter",    I_CRCENTER,  1, t, y); 
			       SET_FLOAT("crcenter",    I_CRCENTER,  2, t, z); 
			     }
    void		 setCRotate(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOAT("crrotate",    I_CRROTATE,  0, t, x);
			       SET_FLOAT("crrotate",    I_CRROTATE,  1, t, y); 
			       SET_FLOAT("crrotate",    I_CRROTATE,  2, t, z); 
			     }
    void		 setCRscale(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOAT("crscale",	I_CRSCALE,  0, t, x);
			       SET_FLOAT("crscale",	I_CRSCALE,  1, t, y);
			       SET_FLOAT("crscale",	I_CRSCALE,  2, t, z);
			     }
    void		 setCRtopHeight(fpreal t, fpreal v)
			     { SET_FLOAT("crtopheight", I_CRTOPHEIGHT,0,t, v); }
    void		 setCRtopCap(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOAT("crtopcap",    I_CRTOPCAP,  0, t, x);
			       SET_FLOAT("crtopcap",    I_CRTOPCAP,  1, t, y); 
			       SET_FLOAT("crtopcap",    I_CRTOPCAP,  2, t, z); 
			     }
    void		 setCRbotHeight(fpreal t, fpreal v)
			     { SET_FLOAT("crbotheight", I_CRBOTHEIGHT,0,t, v); }
    void		 setCRbotCap(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOAT("crbotcap",    I_CRBOTCAP,  0, t, x);
			       SET_FLOAT("crbotcap",    I_CRBOTCAP,  1, t, y); 
			       SET_FLOAT("crbotcap",    I_CRBOTCAP,  2, t, z); 
			     }

    // the three following methods change the capture length capture pose
    // of the bone, and propagate the change to the descentent bones,
    // chaning their capture pose (as if they were "parented" to this bone)
    void		 setCaptLengthAndTranslateDescendents( fpreal t, 
							       fpreal length );
    void		 setCaptTransformAndDescendents( fpreal t,
						    const UT_DMatrix4 &xform );
    virtual void	 applyToCaptTransformAndDescendents( 
						    OP_Context &context,
						    const UT_DMatrix4 &xform);
	    
    // set the capture parameters
    void		 setCaptPoseBoneLength( fpreal t, fpreal l )
			     { SET_FLOATREF("captposelen", 0, t, l);
			     }
    void		 setCaptPoseT(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOATREF("captposet", 0, t, x);
			       SET_FLOATREF("captposet", 1, t, y);
			       SET_FLOATREF("captposet", 2, t, z);
			     }
    void		 setCaptPoseR(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOATREF("captposer", 0, t, x);
			       SET_FLOATREF("captposer", 1, t, y);
			       SET_FLOATREF("captposer", 2, t, z);
			     }
    void		 setCaptPoseS(fpreal t, fpreal x, fpreal y, fpreal z)
			     { SET_FLOATREF("captposes", 0, t, x);
			       SET_FLOATREF("captposes", 1, t, y);
			       SET_FLOATREF("captposes", 2, t, z);
			     }

    // depending on the kinematic override mode get/set either the current
    // or capture bone length
    fpreal		 getOverrideDependentBoneLength( fpreal t );
    void		 setOverrideDependentBoneLength( fpreal t, fpreal l );

    // methods to retrieve the bone-specific parm values

    int   DISPLAYLINK()		{ EVAL_INT("displaylink",    I_LINK, 0, 0) }
    fpreal BONELENGTH(fpreal t) 	{ EVAL_FLOAT("length",	   I_LENGTH, 0, t) }
    fpreal BONELENGTH(fpreal t, int thread)
	{ EVAL_FLOAT_T("length", I_LENGTH, 0, t, thread) }
    void  SOLVER(UT_String &str){ EVAL_STR("solver", I_SOLVER, 0, 0) }
    CH_StringMeaning SOLVERmeaning()
				{ return getStringMeaning("solver", 0, 0.0f); }
    void  REST(fpreal *v, fpreal t) { EVAL_FLOATS("R",      I_REST_ANGLE, v, t) }
    SYS_FORCE_INLINE
    void  REST(fpreal *v, fpreal t, int stid)
		{ EVAL_FLOATS_T("R", I_REST_ANGLE, v, t, stid) }
    fpreal IKDAMPENING(fpreal t)  { EVAL_FLOAT("ikdamp",         I_DAMPEN, 0, t) }
    fpreal XMIN(fpreal t)	{ EVAL_FLOAT("xrange",  I_XRANGE, 0, t) }
    fpreal XMAX(fpreal t)	{ EVAL_FLOAT("xrange",  I_XRANGE, 1, t) }
    fpreal XDAMP(fpreal t)	{ EVAL_FLOAT("xdamp",	I_XDAMP, 0, t) }
    fpreal XROLLOFF(fpreal t)	{ EVAL_FLOAT("xrolloff",I_XROLLOFF, 0, t) }
    fpreal YMIN(fpreal t)	{ EVAL_FLOAT("yrange",  I_YRANGE, 0, t) }
    fpreal YMAX(fpreal t)	{ EVAL_FLOAT("yrange",  I_YRANGE, 1, t) }
    fpreal YDAMP(fpreal t)	{ EVAL_FLOAT("ydamp",	I_YDAMP, 0, t) }
    fpreal YROLLOFF(fpreal t)	{ EVAL_FLOAT("yrolloff",I_YROLLOFF, 0, t) }
    fpreal ZMIN(fpreal t)	{ EVAL_FLOAT("zrange",  I_ZRANGE, 0, t) }
    fpreal ZMAX(fpreal t)	{ EVAL_FLOAT("zrange",  I_ZRANGE, 1, t) }
    fpreal ZDAMP(fpreal t)	{ EVAL_FLOAT("zdamp",	I_ZDAMP, 0, t) }
    fpreal ZROLLOFF(fpreal t)	{ EVAL_FLOAT("zrolloff",I_ZROLLOFF, 0, t) }
    int	  CAPTURE()		{ EVAL_INT  ("displaycapture",I_CAPTURE, 0, 0) }

    fpreal CRCEN(fpreal t, int i)    { EVAL_FLOAT("crcenter",I_CRCENTER, i,t); }
    fpreal CRROTATE(fpreal t, int i){ EVAL_FLOAT("crrotate",I_CRROTATE, i,t);}
    fpreal CRSCALE(fpreal t, int i){ EVAL_FLOAT("crscale",I_CRSCALE, i,t);}
    fpreal CRTOPH(fpreal t)	{ EVAL_FLOAT("crtopheight", I_CRTOPHEIGHT,0,t);}
    fpreal CRTOPC(fpreal t, int i){ EVAL_FLOAT("crtopcap",I_CRTOPCAP, i,t);}
    fpreal CRBOTH(fpreal t)	{ EVAL_FLOAT("crbotheight", I_CRBOTHEIGHT,0,t);}
    fpreal CRBOTC(fpreal t, int i){ EVAL_FLOAT("crbotcap",I_CRBOTCAP, i,t);}
    

    fpreal getCaptPoseBoneLength(fpreal t)
			    { EVAL_FLOAT("captposelen",I_CAPTPOSE_LENGTH,0,t); }
    fpreal getCaptPoseTx(fpreal t){ EVAL_FLOAT("captposet",I_CAPTPOSE_T, 0, t);}
    fpreal getCaptPoseTy(fpreal t){ EVAL_FLOAT("captposet",I_CAPTPOSE_T, 1, t);}
    fpreal getCaptPoseTz(fpreal t){ EVAL_FLOAT("captposet",I_CAPTPOSE_T, 2, t);}
    fpreal getCaptPoseRx(fpreal t){ EVAL_FLOAT("captposer",I_CAPTPOSE_R, 0, t);}
    fpreal getCaptPoseRy(fpreal t){ EVAL_FLOAT("captposer",I_CAPTPOSE_R, 1, t);}
    fpreal getCaptPoseRz(fpreal t){ EVAL_FLOAT("captposer",I_CAPTPOSE_R, 2, t);}
    fpreal getCaptPoseSx(fpreal t){ EVAL_FLOAT("captposes",I_CAPTPOSE_S, 0, t);}
    fpreal getCaptPoseSy(fpreal t){ EVAL_FLOAT("captposes",I_CAPTPOSE_S, 1, t);}
    fpreal getCaptPoseSz(fpreal t){ EVAL_FLOAT("captposes",I_CAPTPOSE_S, 2, t);}
	
    static OP_Node	*myConstructor(OP_Network *net,
					const char *name, OP_Operator *entry);

    // sets the kinematic override and if the mode is changed and recook_bones
    // is true, then the all bones are forced to recook
    // NB: if bones are not forced to cook, they may return invalid
    //     world transform since it is cached and only recomputed at cook time
    static void		 setGlobalKinOverride( KinOverride val, 
					       bool recook_bones = true );
    static KinOverride	 getGlobalKinOverride() { return theGlobalKinOverride; }

    // traverses all existing bones and invoke function on each of them
    // If function returns nonzero the traversal continues, otherwise it stops.
    // The void pointer is passed to the call of the function.
    typedef int (*OBJ_BoneCallbackFunction)( const OBJ_Bone *, void *);

    static void		 traverseAllBones( OBJ_BoneCallbackFunction fnct,
					   void *data );
    
    static const char	       **getReferenceFrames()
				 { return theReferenceFrames; }
    static const char	       **getReferenceFrameNames()
				 { return theReferenceFrameNames; }
    int				 getRestPosition(const char *frame,
						 UT_Vector3R &pos,
						 fpreal t);
    int				 setRestPosition(const char *frame,
						 UT_Vector3R &pos,
						 fpreal t);

    enum FrameType { FRAME_INVALID = -1, FRAME_WORLD = 0, FRAME_PARENT };
    int				 getPosition( FrameType frame_type,
						 UT_Vector3R &pos,
						 fpreal t);
    int				 getPosition(const char *frame,
						 UT_Vector3R &pos,
						 fpreal t);
    int				 setPosition(const char *frame,
						 UT_Vector3R &pos,
						 fpreal t);

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    // get actual rotates (in degrees) that were used for cooking
    // (this may replace local xform with the chop rotates solution)
    void			 getCookedLocalRotates( OP_Context &context,
							UT_Vector3R &rot );

    // two specialized methods for calculating translation/scale transform
    // and rotation angles of the bone with respect to the parent bone. 
    // If parent is not a bone, the xforms are with respect to world. 
    void			 getBoneToParentCaptureTranslate( 
							OP_Context &context,
							UT_Vector3R &transl );
    void			 getBoneToParentCaptureRotate(
							OP_Context &context,
							UT_DMatrix4 &xform );
    // return roatation angles rot (in degrees to keep it consistent with
    // getCookedLocalRotates). Angles are orienting bone w.r.t. parent space
    void			 getBoneToParentCaptureRotate(
							OP_Context &context,
							UT_Vector3R &rot );

    // sets capture rotate parameter based on the given rotation angle vector
    // rot - contains angles (in degrees) in parent bone space
    void			 setCaptureRotateInParentSpace(
							OP_Context &context,
							const UT_Vector3R &rot );
	
    // this method obtains a parent to world transform in the capture position.
    // If parent is a bone, the transformation positions the origin
    // at the parent's tip (not base!) and aligns axis with parent's
    // reference frame.
    // If parent is not a bone, the transform is identity.
    void			 getBoneParentCaptureTransform(
							OP_Context &context,
							UT_DMatrix4 &xform );


    // Only affect bone length
    virtual void		 setScaleFromHandle(fpreal t,
					const UT_Vector3R &s,
					bool create_keys = false,
					int key_idx = -1,
					bool check_modified = true);

    // Affect rotation and rest angles
    virtual void		 setRotateFromHandle(fpreal t,
		                 	const UT_Vector3R &r,
					bool create_keys = false,
					int key_idx = -1,
					bool check_modified = true);

    virtual const char *	 getSetRotateParmName() const;

    virtual void		 transferLocalToPreTransform(fpreal gtime);
    virtual void		 transferPreToLocalTransform(
				    fpreal gtime, bool follow_chan_refs = false);
    virtual void		 transferLocalToPreTransformRotates(fpreal t);
    
    void                         getCaptureRegions(
                                       UT_ValArray<SOP_CaptureRegion *> &list );

    
    static PRM_Template		*getTemplateList();
    static PRM_Template		*getObsolete();


    virtual OBJ_Bone		*castToOBJBone()	{ return this; }

protected:
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return boneIndirect; }

    // This method considers the effect of the LOOKAT fields and produces
    // a matrix that should be premultiplied to the orientation to get the
    // lookat.  It takes the local->world transform (myWorldXform) as this
    // is used to figure out where this is outside cookpaths. 
    // If 'interest' is supplied, then interest will be onto this other node
    // instead.
    // This returns 0 if no lookat is needed, in which case the matrix
    // provided will not be altered!
    // NOTE: this function is virtual, because OBJ_Bone has a "capture"
    //       mode in which the lookAt parameter (or input node's transform 
    //       for that matter) is not considered. For that reason, bones override
    //       this method to provide the mode-dependent transorm
    virtual int		 buildLookAt(OP_Context &context,
				     const UT_DMatrix4 &world, 
				     UT_DMatrix4 &lookat,
				     OP_Node *interest = 0);

    // Override these to pre-multiply a transform for the given matrix.
    // These return 1 if the matrix was modified, 0 otherwise
    virtual int	    applyInputIndependentTransform(
				OP_Context &context, UT_DMatrix4 &mat);
    virtual int	    applyPreTransform(OP_Context &context, UT_DMatrix4 &mat);

private:
    static int			getReferenceFrame(const char *frame);

    static int			*boneIndirect;
    static KinOverride		 theGlobalKinOverride;
    static const char		*theReferenceFrames[];
    static const char		*theReferenceFrameNames[];

    
    bool		    getParmTransformHelper( OP_Context &context,
					 UT_DMatrix4 &mat, 
					 OBJ_TransformComponent type );

    void		    transferLocalToPreTransformHelper(fpreal gtime, 
					 OBJ_TransformComponent type );

    bool		    getParmTransformFromBaseClass( 
				    OP_Context &context, UT_DMatrix4 &mat, 
				    OBJ_TransformComponent type );

    bool		    getParmTransformFromKin( OP_Context &context,
				  UT_DMatrix4 &mat, OBJ_TransformComponent type );

private:
// Callback for building custom world transforms from objects.
// The callback is responsible for setting the given matrix by
// the local transform of the object.
// Return 0 to stop processing, 1 to continue.
    static int	applyCustomParmTransform( 
	    void *data, OBJ_Node *node, OP_Context &context, UT_DMatrix4 &mat);

    int		getCustomChildToWorldTransform(
		    const char *frame, fpreal t, UT_DMatrix4 &mat);
    int		getCustomObjectToWorldTransform(
		    const char *frame, fpreal t, UT_DMatrix4 &mat);

    void	adjustCaptureAndRestAngles(
		    fpreal gtime, UT_DMatrix4 &rest_xform);

    int		myCurrentReferenceFrame;
    int		myLastKinSolverOpId;
    unsigned	myUseSolverFlag:1,
		myIsUsingSolverFlag:1;
};


#undef EVAL_FLOAT
#undef EVAL_FLOATS
#undef EVAL_INT
#undef EVAL_STR

#undef SET_FLOAT
#undef SET_FLOATREF
#undef SET_INT
#undef SET_STRING

#endif
