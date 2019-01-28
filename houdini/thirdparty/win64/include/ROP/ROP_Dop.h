/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP library (C++)
 *
 * COMMENTS:	Base class for Dop output
 *
 */

#ifndef __ROP_Dop_h__
#define __ROP_Dop_h__

#include "ROP_API.h"
#include <DOP/DOP_Parent.h>
#include "ROP_Node.h"

#define STR_PARM(name, vi, t) \
		{ evalString(str, name, vi, t); }
#define INT_PARM(name, vi, t) \
                { return evalInt(name, vi, t); }

class DOP_Node;

enum {
    ROP_DOP_RENDER,
    ROP_DOP_RENDER_BACKGROUND,
    ROP_DOP_RENDER_CTRL,
    ROP_DOP_TRANGE,
    ROP_DOP_FRANGE,
    ROP_DOP_TAKE,
    ROP_DOP_DOPPATH,
    ROP_DOP_DOPOUTPUT,
    ROP_DOP_USESIMFRAMES,
    ROP_DOP_MKPATH,
    ROP_DOP_INITSIM,
    ROP_DOP_COMPRESSSIMS,
    ROP_DOP_ALFPROGRESS,
    // ROP_DOP_BACKGROUND,
    ROP_DOP_TPRERENDER,
    ROP_DOP_PRERENDER,
    ROP_DOP_LPRERENDER,
    ROP_DOP_TPREFRAME,
    ROP_DOP_PREFRAME,
    ROP_DOP_LPREFRAME,
    ROP_DOP_TPOSTFRAME,
    ROP_DOP_POSTFRAME,
    ROP_DOP_LPOSTFRAME,
    ROP_DOP_TPOSTRENDER,
    ROP_DOP_POSTRENDER,
    ROP_DOP_LPOSTRENDER,

    ROP_DOP_MAXPARMS
};

class ROP_API ROP_Dop : public ROP_Node
{
public:
    virtual bool		 updateParmsFlags();
    virtual bool	 evalVariableValue(UT_String &v, int index, int thread);
    virtual bool	 evalVariableValue(fpreal &v, int index, int thread);
    virtual void		 inputConnectChanged(int which);

    static CH_LocalVariable	 myVariableList[];
    static PRM_Template		*getTemplates();
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *op);

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    virtual void	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms);

    static void		 buildSaveMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    virtual bool	 detectIfExistingOutputFiles();
    virtual void	 deleteExistingOutputFiles();

    DOP_Node		*getSourceDOP(fpreal t) const;
protected:
	     ROP_Dop(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~ROP_Dop();

    virtual int			 startRender(int nframes, fpreal s, fpreal e);
    virtual ROP_RENDER_CODE	 renderFrame(fpreal time, UT_Interrupt *boss);
    virtual ROP_RENDER_CODE	 endRender();

    virtual void		 getDescriptiveParmName(UT_String &name) const
				 { name = "dopoutput"; }

private:
    void  	DOPPATH(UT_String &str, fpreal t) const
		    { STR_PARM("doppath", 0, t)}

    void	OUTPUT(UT_String &str, fpreal t) 
		    { getOutputOverrideEx(str, t, "dopoutput", "mkpath"); }

    int		INITSIM(void) const
		    { INT_PARM("initsim", 0, 0) }

    void	COMPRESSSIMS(UT_String &str, fpreal t) const
		    { STR_PARM("compresssims", 0, t) }

    bool	SAVEBACKGROUND(fpreal t) const
		    { 
		      return false;
		      //INT_PARM("savebackground", 0, t);
		    }
    bool	ALFPROGRESS() const
		    { INT_PARM("alfprogress", 0, 0) }

    bool	USESIMFRAMES() const
		    { INT_PARM("usesimframes", 0, 0) }

    DOP_Node		*myDop;
    fpreal		 myEndTime;
    fpreal		 myStartTime;
    bool 		 myModifiedPlayFlag;
    bool		 myBackgroundRender;
    int			 myLastSimFrame;
};


#undef INT_PARM
#undef STR_PARM

#endif
