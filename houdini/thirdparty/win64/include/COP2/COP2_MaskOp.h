/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_MaskOp.h
 *
 * COMMENTS:
 *	Base class for pixel operations that are maskable. 
 */
#ifndef __COP2_MaskOp_h__
#define __COP2_MaskOp_h__

#include "COP2_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_FloatArray.h>

#include "COP2_PixelBase.h"

#define ARG_MASKOP_AMOUNT	"effectamount"
#define ARG_MASKOP_MASKINPUT	"maskinput"
#define ARG_MASKOP_MASKPLANE	"maskplane"
#define ARG_MASKOP_RESIZEMASK   "maskresize"
#define ARG_MASKOP_MASKINVERT	"maskinvert"

class COP2_API COP2_MaskOp : public COP2_PixelBase
{
public:
    static OP_TemplatePair	 myTemplatePair;
    static PRM_Template		 myTemplateList[];

    // override if the mask and op inputs are different from 0 and 1. Op is the
    // input with the planes being operated on; Mask is the input containing
    // the mask. 
    virtual int getOpInput() const	{ return 0; }
    virtual int getMaskInput() const	{ return 1; }

    virtual const char  *inputConnectorLabel(unsigned idx);
    
    virtual int getAuxInput() const { return getMaskInput(); }
    virtual void getAuxInputName(OP_ConnectorId& name_out)
    { getUniqueInputName(name_out, getMaskInput(), true);  }

    virtual COP2_Node * getPassThroughInput(COP2_Context &) { return this; }

    // override if you don't want this class to do the mask operation (ie,
    // blur may handle the mask itself). 
    virtual bool doNormalMask(COP2_Context &)	{ return true; }

    // override if the frame scope dropoff effect is not handled by the
    // derived class.
    virtual bool isFrameEffectHandled() { return true; }

    int		getMaskFromInput();

    virtual void	 computeImageBounds(COP2_Context &context);

    bool	getMaskBounds(float t, int xres, int yres, int thread,
			      int &x1,int &y1, int &x2, int &y2);
    
    bool	getMaskBounds(COP2_Context &context, 
			      int &x1,int &y1, int &x2, int &y2)
		{ return getMaskBounds(context.myTime,
				       context.myXres, context.myYres,
				       context.myThreadIndex, x1,y1,x2,y2); }
    
    virtual void	   modifyViewMatrix(int input,
					    COP2_Context &context,
					    UT_Matrix4 &matrix,
					    float zoom);
protected:
	     COP2_MaskOp(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~COP2_MaskOp();
    
    virtual bool		 updateParmsFlags();
    
    virtual TIL_Sequence *  cookSequenceInfo(OP_ERROR &error);

    // initializes all the mask parms for the mask op (normally done in
    // COP2_MaskOp::cookSequenceInfo above; however, if your node doesn't call
    // COP2_MaskOp's version, call setupMaskParms() at the end of your
    // cookSequenceInfo() method.
	    void	 setupMaskParms();
    
    virtual float	 getFrameScopeEffect(int image_index);
    
    // Override this method rather than cookMyTile().
    virtual OP_ERROR	 doCookMyTile(COP2_Context &context,
				      TIL_TileList *tiles) = 0;
    
    // Do NOT override this method; this method implements the masking
    // operation.
    virtual OP_ERROR	 cookMyTile(COP2_Context &context,
				    TIL_TileList *tiles);

    virtual void	getInputDependenciesForOutputArea(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);

    // utility function to add the mask dependency. Called by the above method.
    // If you don't call COP2_MaskOp's getInputDep...Area(), call this
    // method in your own getInputDep...Area() method.
    void		getMaskDependency(COP2_CookAreaInfo &output_area,
					  const COP2_CookAreaList &input_areas,
					  COP2_CookAreaList &needed_areas);
	
    TIL_Region		*getMaskRegion(COP2_Context &context,
				       const TIL_Plane *plane, float t,
				       int x1, int y1, int x2, int y2);

    float *		getMaskData(COP2_Context &context, int x1,int y1,
				    int x2,int y2, int component = 0);

    virtual float AMOUNT(float t) { return evalFloat(ARG_MASKOP_AMOUNT, 0, t);}
    virtual int	 MASK_INPUT()	{ return evalInt(ARG_MASKOP_MASKINPUT, 0, 0); }
    virtual void MASK_PLANE(UT_String &s)
		{ evalString(s, ARG_MASKOP_MASKPLANE, 0, 0); }
    virtual int	MASK_INVERT() { return evalInt(ARG_MASKOP_MASKINVERT, 0,0); }
    virtual int MASK_RESIZE() { return evalInt(ARG_MASKOP_RESIZEMASK, 0,0); }

    bool	getMask(int &pindex, int &cindex);

    float	getAmount(int image_index);
    
    int		myMaskInput;
    UT_String   myMaskPlane;
    bool	myMaskInvert;
    bool	myHasMask;
    bool	myMaskResize;
    UT_Lock	myAmountLock;
    UT_IntArray myAmountIndex;
    UT_FloatArray myAmountValue;
};

#endif
