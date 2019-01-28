/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_CaptureRegion.h ( SOP Library, C++)
 *
 * COMMENTS: The Capture Region SOP
 */

#ifndef __SOP_CaptureRegion__
#define __SOP_CaptureRegion__

#include "SOP_API.h"
#include "SOP_Node.h"

class DEP_MicroNode;

class SOP_API SOP_CaptureRegion : public SOP_Node
{
public:
     // effective transform order of the capture region with respect to
     // the bone space
     static UT_XformOrder         CR_XFORM_ORDER;
	
	      SOP_CaptureRegion(OP_Network *net, const char *name, 
			        OP_Operator *entry);
     virtual ~SOP_CaptureRegion();

     static   OP_Node	*myConstructor(OP_Network *net, const char *name,
				       OP_Operator *entry);
     static   PRM_Template	myTemplateList[];
     static   PRM_Template	myObsoleteList[];
     
     // converts unused parameters
     virtual void	 resolveObsoleteParms(
				PRM_ParmList *obsolete_parms) override;
     
     // sets the capture parameters for the bone.
     // Capture parameters "freeze" the current parameters by storing them
     // (or accumulation of them) in the capture parameters. The capture
     // parameters are used for capturing geometry by bone's capture regions.
     // They are also used for displaying the capture pose when
     // kinematic override is set to "CAPTURE"
     void		 setCaptureParameters(OP_Context &context,
					      UT_String &errors);

     // transfers regular parameters to capture parameters
     void		 copyRegularToCaptureParameters( OP_Context &context );

     // Clear the rig adjustment transform
     void clearRigAdjustment(OP_Context &context);

     // sets the rig adjustment xform so that capture and animation volumes
     // align together.
     // We want to satisfy the equation which encodes the alignment
     // of animation and capture regions in world space:
     //    rig_adjust * Ma * MA = Mc * MC 
     //    rig_adjust = Mc * MC * MA^ * Ma^
     // where:
     //   MA - is a world position of animation bone, and 
     //   MC - is a world position of the capture bone, 
     //   Mc - is a acapture region in capture bone space
     //   Ma - is a animation region in animation bone space
     //   ^  - denotes an inverse
     // The arguments to this method are:
     //   bone_inv_anim  = MA^
     //   bone_capture   = MC
     void setRigAdjustment(OP_Context &context,
			   const UT_DMatrix4 &bone_inv_anim,
			   const UT_DMatrix4 &bone_capture,
			   UT_String &errors);

     // Creates data for capturing
     SYS_DEPRECATED(11.0) void getCaptureData(fpreal t, UT_Matrix4 &xform,
				float *atr_data );
     bool		 getCaptureData(fpreal t, UT_Matrix4 &xform,
					float &bcap, float &tcap,
					float &taperx, float &taperz,
					float &iweight, float &oweight,
					bool capture,
					DEP_MicroNode *node = NULL);

     // sets the capture parameters so that the region 
     // capture aligns with animation
     void synchronizeCaptureToDeform(
	    OP_Context &context,
	    const UT_DMatrix4 &bone_anim,
	    const UT_DMatrix4 &bone_inv_capture,
	    UT_String &errors);

     // sets the animation parameters so that the region
     // animation (deform) aligns with capture
     void synchronizeDeformToCapture(
	    OP_Context &context,
	    const UT_DMatrix4 &bone_inv_anim,
	    const UT_DMatrix4 &bone_capture,
	    UT_String &errors);

     // get and set the matrix (adjusted for the zfactor)
     void	getCaptureXform( OP_Context &context, UT_DMatrix4 &xform );
     void	setCaptureXform( OP_Context &context, const UT_DMatrix4 &xform);

     // get and set the matrix (adjusted for the zfactor)
     void	getDeformXform( OP_Context &context, UT_DMatrix4 &xform );
     void	setDeformXform( OP_Context &context, const UT_DMatrix4 &xform );

     // obtains a rig adjustmen transform. This transform is calculated when
     // animation bone position has been moved away from capture bone position
     // without the intention of animating. This happend often in rigging
     // process, for example adjusting joints of a limb after skin has been
     // captured. Rig adjutment transform alligns the animation region
     // with capture region so that no deformation occures on the skin.
     void	getRigAdjustXform( OP_Context & context, UT_DMatrix4 &xform );
      
     // indices to the parameters: must correspond to myTemplateList entries
     enum SOP_Indices
     {
	 I_SWITCHER = 0,

	 I_ANIM_SWITCHER,
	 I_AXIS,
	 I_CENTER,
	 I_ORIENT,
	 I_SQUASH,
	 I_TOP_H,
	 I_TOP_CAP,
	 I_BOT_H,
	 I_BOT_CAP,
	 I_ZFACTOR,

	 I_RIG_ADJUST_T,
	 I_RIG_ADJUST_R,
	 I_RIG_ADJUST_S,
	 I_RIG_ADJUST_ZFACTOR,

	 I_C_AXIS,
	 I_C_CENTER,
	 I_C_ORIENT,
	 I_C_SQUASH,
	 I_C_TOP_H,
	 I_C_TOP_CAP,
	 I_C_BOT_H,
	 I_C_BOT_CAP,
	 I_C_ZFACTOR,
	 I_C_RANGE_W,

	 I_COLOR,
	 I_N_SOPCAPTURE_INDICES	// should remain the last in this list
     };

     float	COLORR(fpreal t)    { return evalFloat( I_COLOR,    0, t); }
     float	COLORG(fpreal t)    { return evalFloat(	I_COLOR,    1, t); }
     float	COLORB(fpreal t)    { return evalFloat(	I_COLOR,    2, t); }

protected:
     virtual OP_ERROR	 cookMySop( OP_Context &context ) override;
     
private:
     // gets (capture override dependent) values for the capture region
     void getParameters( fpreal t,
		    float &tx, float &ty, float &tz,
		    float &rx, float &ry, float &rz,
		    float &squash_x, float &squash_y, float &squash_z,
		    float &top_tube_sy, float &bot_tube_sy,
		    float &top_x, float &top_y, float &top_z, 
		    float &bot_x, float &bot_y, float &bot_z,
		    int &axis, bool capture, DEP_MicroNode *node );

     // creates channel and inserts a reference in it
     bool	createChannelWithReference(const char* ch_name,
					   const char* ref,
					   CH_ExprLanguage language);
     bool	updateObsoleteParm(PRM_ParmList *obsolete_parms,
				   const char *old_parm_name, int vi,
				   const char *new_parm_name,
				   const char *new_chan_name,
				   const char *find_expr,
				   const char *replace_expr,
				   float scale);

     // sets the height and caps of the tube to match
     void	setDeformToCaptureTube( OP_Context &context );
     void	setCaptureToDeformTube( OP_Context &context );

     // NOTE: xform should be crackable with the CR_XFORM_ORDER (i.e., RTS)
     void	setRigAdjustXform(OP_Context &context, 
				  const UT_DMatrix4 &xform,
				  UT_String &errors);

     // recalculates the value of the zfactor adjustment
     void updateRigAdjusmentZFactor( OP_Context &context );


     int	AXIS()		    { return evalInt  ( I_AXIS,	    0, 0); }
     float	CENTERX(fpreal t)   { return evalFloat( I_CENTER,   0, t); }
     float	CENTERY(fpreal t)   { return evalFloat( I_CENTER,   1, t); }
     float	CENTERZ(fpreal t)   { return evalFloat( I_CENTER,   2, t); }
     float	ORIENTX(fpreal t)   { return evalFloat( I_ORIENT,   0, t); }
     float	ORIENTY(fpreal t)   { return evalFloat( I_ORIENT,   1, t); }
     float	ORIENTZ(fpreal t)   { return evalFloat( I_ORIENT,   2, t); }
     float	SQUASHX(fpreal t)   { return evalFloat( I_SQUASH,   0, t); }
     float	SQUASHY(fpreal t)   { return evalFloat( I_SQUASH,   1, t); }
     float	SQUASHZ(fpreal t)   { return evalFloat( I_SQUASH,   2, t); }
     float	TOPHEIGHT(fpreal t) { return evalFloat( I_TOP_H,    0, t); }
     float	TOPX(fpreal t)	    { return evalFloat( I_TOP_CAP,  0, t); }
     float	TOPY(fpreal t)	    { return evalFloat( I_TOP_CAP,  1, t); }
     float	TOPZ(fpreal t)	    { return evalFloat( I_TOP_CAP,  2, t); }
     float	BOTHEIGHT(fpreal t) { return evalFloat( I_BOT_H,    0, t); }
     float	BOTX(fpreal t)	    { return evalFloat( I_BOT_CAP,  0, t); }
     float	BOTY(fpreal t)	    { return evalFloat( I_BOT_CAP,  1, t); }
     float	BOTZ(fpreal t)	    { return evalFloat( I_BOT_CAP,  2, t); }
     float	ZFACTOR(fpreal t)   { return evalFloat( I_ZFACTOR,  0, t); }
     
     float	RIGADJUSTTX(fpreal t) {return evalFloat( I_RIG_ADJUST_T, 0, t);}
     float	RIGADJUSTTY(fpreal t) {return evalFloat( I_RIG_ADJUST_T, 1, t);}
     float	RIGADJUSTTZ(fpreal t) {return evalFloat( I_RIG_ADJUST_T, 2, t);}
     float	RIGADJUSTRX(fpreal t) {return evalFloat( I_RIG_ADJUST_R, 0, t);}
     float	RIGADJUSTRY(fpreal t) {return evalFloat( I_RIG_ADJUST_R, 1, t);}
     float	RIGADJUSTRZ(fpreal t) {return evalFloat( I_RIG_ADJUST_R, 2, t);}
     float	RIGADJUSTSX(fpreal t) {return evalFloat( I_RIG_ADJUST_S, 0, t);}
     float	RIGADJUSTSY(fpreal t) {return evalFloat( I_RIG_ADJUST_S, 1, t);}
     float	RIGADJUSTSZ(fpreal t) {return evalFloat( I_RIG_ADJUST_S, 2, t);}
     float	RIGADJUSTZFACTOR(fpreal t)  
				{return evalFloat( I_RIG_ADJUST_ZFACTOR, 0, t);}

     int	C_AXIS()	    { return evalInt  ( I_C_AXIS,	0, 0); }
     float	C_CENTERX(fpreal t)  { return evalFloat( I_C_CENTER,	0, t); }
     float	C_CENTERY(fpreal t)  { return evalFloat( I_C_CENTER,	1, t); }
     float	C_CENTERZ(fpreal t)  { return evalFloat( I_C_CENTER,	2, t); }
     float	C_ORIENTX(fpreal t)  { return evalFloat( I_C_ORIENT,	0, t); }
     float	C_ORIENTY(fpreal t)  { return evalFloat( I_C_ORIENT,	1, t); }
     float	C_ORIENTZ(fpreal t)  { return evalFloat( I_C_ORIENT,	2, t); }
     float	C_SQUASHX(fpreal t)  { return evalFloat( I_C_SQUASH,	0, t); }
     float	C_SQUASHY(fpreal t)  { return evalFloat( I_C_SQUASH,	1, t); }
     float	C_SQUASHZ(fpreal t)  { return evalFloat( I_C_SQUASH,	2, t); }
     float	C_TOPHEIGHT(fpreal t){ return evalFloat( I_C_TOP_H,	0, t); }
     float	C_TOPX(fpreal t)    { return evalFloat( I_C_TOP_CAP,	0, t); }
     float	C_TOPY(fpreal t)    { return evalFloat( I_C_TOP_CAP,	1, t); }
     float	C_TOPZ(fpreal t)    { return evalFloat( I_C_TOP_CAP,	2, t); }
     float	C_BOTHEIGHT(fpreal t){ return evalFloat( I_C_BOT_H,	0, t); }
     float	C_BOTX(fpreal t)    { return evalFloat( I_C_BOT_CAP,	0, t); }
     float	C_BOTY(fpreal t)    { return evalFloat( I_C_BOT_CAP,	1, t); }
     float	C_BOTZ(fpreal t)    { return evalFloat( I_C_BOT_CAP,	2, t); }
     float	C_ZFACTOR(fpreal t){ return evalFloat(I_C_ZFACTOR,	0, t); }
     float	C_INWEIGHT(fpreal t) { return evalFloat( I_C_RANGE_W,	0, t); }
     float	C_OUTWEIGHT(fpreal t){ return evalFloat( I_C_RANGE_W,	1, t); }


     void	setAXIS( int v)	    { setChRefInt  ( I_AXIS,	0, 0, v); }
     void	setCENTER(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat( I_CENTER, 0, t, v.x());
					setChRefFloat( I_CENTER, 1, t, v.y());
					setChRefFloat( I_CENTER, 2, t, v.z());
				    }
     void	setORIENT(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat( I_ORIENT, 0, t, v.x());
					setChRefFloat( I_ORIENT, 1, t, v.y());
					setChRefFloat( I_ORIENT, 2, t, v.z());
				    }
     void	setSQUASH(const UT_Vector3 &v, fpreal t)
     				    {
					setChRefFloat( I_SQUASH, 0, t, v.x());
					setChRefFloat( I_SQUASH, 1, t, v.y());
					setChRefFloat( I_SQUASH, 2, t, v.z());
				    }
      void	setTOPHEIGHT(float v, fpreal t)
				    { 
					setChRefFloat( I_TOP_H,  0, t, v);
				    }
     void	setTOP(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat( I_TOP_CAP, 0, t, v.x());
					setChRefFloat( I_TOP_CAP, 1, t, v.y());
					setChRefFloat( I_TOP_CAP, 2, t, v.z());
				    }
     void	setBOTHEIGHT(float v, fpreal t)
     				    { 
					setChRefFloat( I_BOT_H,  0, t, v);
				    }
     void	setBOT(const UT_Vector3 &v, fpreal t) 
				    {
					setChRefFloat( I_BOT_CAP, 0, t, v.x());
					setChRefFloat( I_BOT_CAP, 1, t, v.y());
					setChRefFloat( I_BOT_CAP, 2, t, v.z());
				    }  
     void	setZFACTOR(float v, fpreal t)
     				    { 
					setChRefFloat( I_ZFACTOR,  0, t, v);
				    }


     
     void	setC_AXIS( int v)   { setChRefInt  ( I_C_AXIS,	0, 0, v); }
     void	setC_CENTER(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat( I_C_CENTER, 0, t, v.x());
					setChRefFloat( I_C_CENTER, 1, t, v.y());
					setChRefFloat( I_C_CENTER, 2, t, v.z());
				    }
     void	setC_ORIENT(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat( I_C_ORIENT, 0, t, v.x());
					setChRefFloat( I_C_ORIENT, 1, t, v.y());
					setChRefFloat( I_C_ORIENT, 2, t, v.z());
				    }
     void	setC_SQUASH(const UT_Vector3 &v, fpreal t)
     				    {
					setChRefFloat( I_C_SQUASH, 0, t, v.x());
					setChRefFloat( I_C_SQUASH, 1, t, v.y());
					setChRefFloat( I_C_SQUASH, 2, t, v.z());
				    }
     void	setC_TOPHEIGHT(float v, fpreal t)
				    { 
					setChRefFloat( I_C_TOP_H,  0, t, v);
				    }
     void	setC_TOP(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat( I_C_TOP_CAP, 0, t,v.x());
					setChRefFloat( I_C_TOP_CAP, 1, t,v.y());
					setChRefFloat( I_C_TOP_CAP, 2, t,v.z());
				    }
     void	setC_BOTHEIGHT(float v, fpreal t)
     				    { 
					setChRefFloat( I_C_BOT_H,  0, t, v);
				    }
     void	setC_BOT(const UT_Vector3 &v, fpreal t) 
				    {
					setChRefFloat( I_C_BOT_CAP, 0, t,v.x());
					setChRefFloat( I_C_BOT_CAP, 1, t,v.y());
					setChRefFloat( I_C_BOT_CAP, 2, t,v.z());
				    }  
     void	setC_ZFACTOR(float v, fpreal t)
     				    { 
					setChRefFloat( I_C_ZFACTOR,  0, t, v);
				    }

     void	setRIGADJUSTT(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat(I_RIG_ADJUST_T,0,t,v.x());
					setChRefFloat(I_RIG_ADJUST_T,1,t,v.y());
					setChRefFloat(I_RIG_ADJUST_T,2,t,v.z());
				    }
     void	setRIGADJUSTR(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat(I_RIG_ADJUST_R,0,t,v.x());
					setChRefFloat(I_RIG_ADJUST_R,1,t,v.y());
					setChRefFloat(I_RIG_ADJUST_R,2,t,v.z());
				    }
     void	setRIGADJUSTS(const UT_Vector3 &v, fpreal t)
				    {
					setChRefFloat(I_RIG_ADJUST_S,0,t,v.x());
					setChRefFloat(I_RIG_ADJUST_S,1,t,v.y());
					setChRefFloat(I_RIG_ADJUST_S,2,t,v.z());
				    }
     void	setRIGADJUSTZFACTOR(float v, fpreal t)  
				    {
					setChRefFloat( I_RIG_ADJUST_ZFACTOR,
						       0, t, v );
				    }

};

#endif

