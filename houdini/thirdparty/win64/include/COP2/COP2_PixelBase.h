/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_PixelBase.h
 *
 * COMMENTS:
 *	Base class for single pixel operations (ie. gamma, bright)
 */
#ifndef __COP2_PixelBase_h__
#define __COP2_PixelBase_h__

#include "COP2_API.h"
#include <UT/UT_IntArray.h>
#include "COP2_Node.h"

#define ARG_PIXELBASE_RGBA	"scopergba"
#define ARG_PIXELBASE_SCOPE	"pscope"
#define ARG_PIXELBASE_FSCOPE	"fscope"
#define ARG_PIXELBASE_FRANGE	"frange"
#define ARG_PIXELBASE_FDROP	"fdropoff"
#define ARG_PIXELBASE_FFUNC	"fdropfunc"
#define ARG_PIXELBASE_FEFFECT	"foutside"
#define ARG_PIXELBASE_SPECIFIC	"flist"
// menu						   8
#define ARG_PIXELBASE_AUTOFRAME	"fautoadjust"
#define ARG_PIXELBASE_CURLENGTH	"currange"

class COP2_API COP2_PixelBase : public COP2_Node
{
public:

    virtual TIL_Sequence *cookSequenceInfo(OP_ERROR &err);

    static OP_TemplatePair	 myTemplatePair;
    static PRM_Template		 myTemplateList[];
    
    static int		 scopeChanged(void *data, int idx, float t,
				      const PRM_Template *);
    static int		 toggleScope(void *data, int idx, float t,
				     const PRM_Template *);
    static int		 frameChanged(void *data, int idx, float t,
				     const PRM_Template *);

    virtual bool	 isPixelBased() const { return true; }

    void	setScope(const UT_String &st);

    virtual bool	 isTimeDepOperation();
protected:
	     COP2_PixelBase(OP_Network *parent,
			    const char *name,
			    OP_Operator *entry);
    virtual ~COP2_PixelBase();
    
    virtual bool		 updateParmsFlags();
    
    virtual int		 scope(UT_String &scope);
    virtual int		 frameScope(UT_String &scope);

    virtual bool	 usesPartialFrameScope() { return true; }
    virtual float	 getFrameScopeEffect(int image_index);
    
    virtual COP2_ThreadPref getThreadPreference() const
			    { return COP2_THREAD_MULTI; }
   
    virtual void	getInputDependenciesForOutputArea(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);

    virtual bool	 isFrameScoped(int image_index);

    virtual void	 setDefaultScope(bool color, bool alpha,
					 const char *others);
 
private:
    void		 toggleScope(int comp);
    void		 updateButtons();
    void		 updateFrameScope(int op, float t);

    void		 getFrameScope(UT_IntArray &scope_frames,
				       bool &allscoped);

    void		 autoAdjustLength(int oldlen, int newlen);

    int			 myFrameScopeMethod;
    float		 myFrameScopeStart;
    float		 myFrameScopeEnd;
    float		 myFrameScopePreDrop;
    float		 myFrameScopePostDrop;
    int			 myFrameScopeFunction;
    float		 myFrameScopeEffect;
    UT_IntArray		 mySpecificFrames;
    bool		 myScopeAllFlag;
    
protected:
    int		RGBA()
		{ return evalInt(ARG_PIXELBASE_RGBA,0,0); }
    
    
    int		FSCOPE_METHOD()
		{ return evalInt(ARG_PIXELBASE_FSCOPE,0,0); }

    void	FRANGE(float &start, float &end)
		{ start = evalFloat(ARG_PIXELBASE_FRANGE,0,0);
		  end   = evalFloat(ARG_PIXELBASE_FRANGE,1,0); }
    
    void	FDROP(float &start, float &end)
		{ start = evalFloat(ARG_PIXELBASE_FDROP,0,0);
		  end   = evalFloat(ARG_PIXELBASE_FDROP,1,0); }
    
    int		FSCOPE_FUNCTION()
		{ return evalInt(ARG_PIXELBASE_FFUNC,0,0); }

    float	FSCOPE_EFFECT()
		{ return evalFloat(ARG_PIXELBASE_FEFFECT, 0, 0); }
    
    void	FRAME_SPECIFIC(UT_String &st)
		{ evalString(st, ARG_PIXELBASE_SPECIFIC,0,0); }

    int		FAUTO()
		{ return evalInt(ARG_PIXELBASE_AUTOFRAME, 0,0); }

    int		CURRENT_LENGTH()
		{ return evalInt(ARG_PIXELBASE_CURLENGTH, 0,0); }

    void	SCOPE(UT_String &st)
		{ evalString(st, ARG_PIXELBASE_SCOPE,0,0); }
    

    void	SET_SCOPE(const UT_String &st)
		{
		    setChRefString(ARG_PIXELBASE_SCOPE,0,0, st,
				   CH_STRING_LITERAL);
		}
    
    void	SET_FRANGE(float start, float end)
		{
		    if( canAccess(PRM_WRITE_OK) )
		    {
			setSilentFloat(ARG_PIXELBASE_FRANGE,0,0, start);
			setSilentFloat(ARG_PIXELBASE_FRANGE,1,0, end);
		    }
		}
    
    void	SET_FDROP(float start, float end)
		{
		    if( canAccess(PRM_WRITE_OK) )
		    {
			setSilentFloat(ARG_PIXELBASE_FDROP,0,0, start);
			setSilentFloat(ARG_PIXELBASE_FDROP,1,0, end);
		    }
		}
    
    void	SET_CURRENT_LENGTH(int length)
		{
		    if( canAccess(PRM_WRITE_OK) )
			setSilentInt(ARG_PIXELBASE_CURLENGTH, 0,0, length);
		}

};

#endif
