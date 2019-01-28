/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Generator.h
 *
 * COMMENTS:
 *	Base class for generator COPs. 
 */
#ifndef _COP2_GENERATOR_H_
#define _COP2_GENERATOR_H_

#include "COP2_API.h"
#include <TIL/TIL_Defines.h>
#include "COP2_MaskOp.h"


// generated planes
#define CREATE_PLANE_RGBA	0
#define CREATE_PLANE_RGBA3	1
#define CREATE_PLANE_RGB	2
#define CREATE_PLANE_A		3
#define CREATE_PLANE_A3		4
#define CREATE_PLANE_M		5
#define CREATE_PLANE_M3		6
#define CREATE_PLANE_Z		7
#define CREATE_PLANE_L		8
#define CREATE_PLANE_B		9
#define CREATE_PLANE_P	       10
#define CREATE_PLANE_N	       11
#define CREATE_PLANE_V	       12
#define CREATE_PLANE_TERRAIN_H 13
#define CREATE_PLANE_NONE      14
    
#define ADD_REPLACE	0
#define ADD_RENAME	1
#define ADD_ADD		2
#define ADD_SCREEN	3
#define ADD_SUB		4
#define ADD_MULT	5
#define ADD_MIN		6
#define ADD_MAX		7
#define ADD_AVERAGE	8

class COP2_API COP2_Generator : public COP2_MaskOp
{
public:
    virtual TIL_Sequence *cookSequenceInfo(OP_ERROR &error);

    static int		 setResCallback(void *,int,float, const PRM_Template *);
    static int		 setRateCallback(void *,int,float,const PRM_Template *);
    static int		 setDepthCallback(void *,int,float,const PRM_Template*);
    
    virtual bool	 isGenerator() const { return true; }
    
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];
    static PRM_Template		 myObsoleteParms[];
    static const char *		 myInputLabels[];

    virtual bool	    isTimeModifier() const { return true; }
    
    virtual void	SET_ADD_PLANES(int planes)
			{
			    if( canAccess(PRM_WRITE_OK) )
				setInt("addplanes",0,0, planes);
			}
    
    virtual void	SET_PLANES(int planes)
			{
			    if( canAccess(PRM_WRITE_OK) )
				setInt("planes", 0, 0, planes);
			}
    
    virtual void	SET_FORMAT(TIL_DataFormat format)
			{
			    if( canAccess(PRM_WRITE_OK) )
				setInt("depth", 0, 0, (int) format);
			}
    
    virtual void	SET_ADD_OP(int op)
			{
			    if( canAccess(PRM_WRITE_OK) )
				setInt("addplaneop", 0, 0, op);
			}
    
    virtual void	SET_CUSTOM_PLANES(const UT_String &st)
			{
			    if( canAccess(PRM_WRITE_OK) )
				setString(st, CH_STRING_LITERAL,
					  "customplanes",0,0);
			}

    static int	depthChanged(void *, int, float, const PRM_Template *);
    static int	overrideSizeChanged(void *, int, float, const PRM_Template *);
    static int	overrideAspectChanged(void *,int, float, const PRM_Template *);
    static int	overrideRangeChanged(void *,int, float, const PRM_Template *);
    static int	overrideRateChanged(void *, int, float, const PRM_Template *);

    // For the output area (an area of a plane belonging to this node)
    // and a set of input areas, determine which input areas and which
    // parts of these areas are needed to cook the output area.
    virtual void	 getInputDependenciesForOutputArea(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);
    
    virtual int getOpInput() const	{ return 0; }
    virtual int getMaskInput() const	{ return 1; }
    virtual bool isFrameEffectHandled() { return false; }
    
    virtual int		 scope(UT_String &) { return 0; }
    virtual int		 frameScope(UT_String &) { return 0; }
    virtual bool	 isPixelBased() const { return false; }

    virtual void	 inputConnectChanged(int which);
   
protected:
	     COP2_Generator(OP_Network *parent, const char *name,
			    OP_Operator *entry);
    virtual ~COP2_Generator();

    virtual bool		 updateParmsFlags();
    
    virtual void		computeImageBounds(COP2_Context &context);
	
    virtual COP2_ThreadPref getThreadPreference() const
			    { return COP2_THREAD_MULTI; }
    
    virtual OP_ERROR		doCookMyTile(COP2_Context &context,
					   TIL_TileList *tilelist);
    virtual OP_ERROR		generateTile(COP2_Context &context,
					     TIL_TileList *tilelist) = 0;
    
    const char *		uniqueName(const char *name);

    virtual void	getGeneratedBounds(COP2_Context &context,
					   int &bx1, int &by1,
					   int &bx2, int &by2);

    void		applyMaskBounds(const TIL_Plane *plane, int array,
					float t, int xres,int yres, int thread,
					int &bx1,int &by1, int &bx2,int &by2);
    
    void		getBounds(const TIL_Plane *plane, int aindex,
				  float t, int xres, int yres, int thread,
				  int x1,int y1, int x2, int y2,
				  int &bx1, int &by1, int &bx2, int &by2);

	    int		OVERRIDE_SIZE()
		{ return evalInt("overridesize",0,0); }
    
    virtual void	IMAGE_SIZE(int &x, int &y)
		{ x = evalInt("size",0,0.0f);
		  y = evalInt("size",1,0.0f); }

    virtual void	SET_XRES(int x)
		{
		    if( canAccess(PRM_WRITE_OK) )
			setInt("size", 0, 0.0f, x);
		}

    virtual void	SET_YRES(int x)
		{
		    if( canAccess(PRM_WRITE_OK) )
			setInt("size", 1, 0.0f, x);
		}

	    int		OVERRIDE_ASPECT()
		{ return evalInt("overrideaspect",0,0); }
	    
    virtual float	ASPECT()
		{ return evalFloat("aspect",0,0.0f); }

    virtual int		USEPLANES()
		{ return evalInt("planes", 0, 0.0f); }
    
    virtual int		ADD_PLANES()
		{ return evalInt("addplanes",0,0.0f); }
    
    virtual int		ADD_OP()
		{ return evalInt("addplaneop", 0, 0.0f); }

		
    virtual void	CUSTOM_PLANES(UT_String &st)
		{ evalString(st, "customplanes",0,0.0f); }

    virtual TIL_DataFormat	DEPTH();

    virtual int		USE_POINTS()
		{ return evalInt("usebwpoints",0,0.0f); }

    virtual void	BW_POINTS(unsigned int &b, unsigned int &w);

    virtual int		INTERLACE()
		{ return evalInt("interlace", 0,0.0f); }

    virtual int		IDOMINANCE()
		{ return evalInt("idominance", 0,0.0f); }

    virtual int		SINGLE_IMAGE()
		{ return evalInt("singleimage",0,0.0f); }

    virtual int		START()
		{ return evalInt("start", 0, 0.0f); }

    virtual int		LENGTH()
		{ return evalInt("length",0,0.0f); }

    virtual void	SET_START(int start)
		{
		    if( canAccess(PRM_WRITE_OK) )
		    {
			deAnimate("start");
			setInt("start",0,0.0f,start);
		    }
		}

    virtual void	SET_LENGTH(int len)
		{
		    if( canAccess(PRM_WRITE_OK) )
		    {
			deAnimate("length");
			setInt("length", 0, 0.0f,len);
		    }
		}

    virtual float	RATE();

    virtual TIL_Extend	PRE_EXTEND()
		{ return (TIL_Extend)evalInt("preextend", 0, 0.0f); }

    virtual int		PRE_HOLD()
		{ return evalInt("prehold", 0, 0.0f); }

    virtual TIL_Extend	POST_EXTEND()
		{ return (TIL_Extend)evalInt("postextend",0,0.0f); }

    virtual int		POST_HOLD()
		{ return evalInt("posthold",0,0.0f); }

    virtual float AMOUNT(float t) { return evalFloat("effectamount", 0, t); }
    virtual int	 MASK_INPUT()	{ return 1; }
    
    virtual bool preferredViewPlane(UT_WorkBuffer &);
    
    int		myAddOp;
    TIL_Sequence myConnectedSequence;
};


#endif
