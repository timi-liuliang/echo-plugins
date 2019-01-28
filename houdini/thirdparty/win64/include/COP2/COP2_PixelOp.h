/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_PixelOp.h
 *
 * COMMENTS:
 *	Base class for all simple pixel operations
 */
#ifndef _COP2_PIXELOP_H_
#define _COP2_PIXELOP_H_

#include "COP2_API.h"
#include <TIL/TIL_Defines.h>
#include <RU/RU_PixelEngine.h>

class COP2_PixelFunction;
class RU_GPUPixelEngine;

#include "COP2_MaskOp.h"

#define ARG_PIXELOP_CC_IN_UNPREMULT	"dounpremult"
#define ARG_PIXELOP_QUANTIZE_HERE	"quantize"

class COP2_API COP2_PixelOp : public COP2_MaskOp
{
public:
    virtual bool		 isSlowOperation() const { return false; }


    // override COP2_MaskOp version.
    virtual COP2_Node * getPassThroughInput(COP2_Context &);
    
    static OP_TemplatePair	 myTemplatePair;
    static PRM_Template		 myTemplateList[];

    // For the output area (an area of a plane belonging to this node)
    // and a set of input areas, determine which input areas and which
    // parts of these areas are needed to cook the output area.
    virtual void	 getInputDependenciesForOutputArea(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);

    virtual TIL_ViewerType  getCurveEditorType()
				{ return TIL_VIEW_GRAPH_CURVES; }

protected:
    virtual bool		 updateParmsFlags();
    
    virtual OP_ERROR		doCookMyTile(COP2_Context &context,
					     TIL_TileList *tilelist);

    virtual COP2_ContextData	*newContextData(const TIL_Plane *, int,
						float t, int xres, int yres,
						int thread,
						int max_threads);
    
    // All pixel operations should override addPixelFunction.
    virtual RU_PixelFunction	*addPixelFunction(const TIL_Plane *plane,
						  int array_index, float t,
						  int xres, int yres,
						  int thread)=0;
    
    // Don't override this method! 
    virtual void		 getPixelFunction(const TIL_Plane *plane,
						  int array_index, float t,
						  int xres, int yres,
						  int thread,
						  COP2_PixelFunction *&func,
						  bool concatenate,
						  bool dependent_too);
    
    virtual bool		checkAlignment(int x1, int y1, int x2, int y2,
					       int ix1, int iy1,
					       int ix2, int iy2,
					       int tilex, int tiley);
    
		COP2_PixelOp(OP_Network *parent, const char *name,
			   OP_Operator *entry);
    virtual	~COP2_PixelOp();

    bool	computeAlphaValues(COP2_Context &context, TIL_TileList *tiles,
				   til_AlignedBlock *&before,
				   til_AlignedBlock *&after,
				   bool &isalphaconst);
    void	multiplyAlpha(TIL_TileList *tiles, float *alpha,
			      bool docooked, bool isalphaconst);

    static int			 editCurves(void *, int, float,
					    const PRM_Template *);
    
    virtual void	getMaxNumThreadsInCook(COP2_Context &,
					       int &plane,
					       int &node,
					       int &op) const;
    
    virtual bool	 isGPUAssisted(const TIL_Plane &plane,
				       int array_index, float t,
				       int xres, int yres,
				       int thread_index);
private:
    int		QUANTIZE_HERE()
		{ return evalInt(ARG_PIXELOP_QUANTIZE_HERE, 0, 0); }
    
    int		DO_UNPREMULT()
		{ return evalInt(ARG_PIXELOP_CC_IN_UNPREMULT, 0, 0); }

    int		myPixelOpParmBase;
};

class COP2_API cop2_PixelOpContext : public COP2_ContextData
{
public:
		 cop2_PixelOpContext()
		     : myEngine(0), myGPUEngine(0), myFunction(0) {}
    
    virtual	~cop2_PixelOpContext();

    virtual bool createPerPlane() const { return true; }
    virtual bool createPerRes() const   { return false; }
    virtual bool createPerThread() const;
   
    RU_PixelEngine	*myEngine;
    RU_GPUPixelEngine   *myGPUEngine;
    COP2_PixelFunction	*myFunction;
};

#endif
