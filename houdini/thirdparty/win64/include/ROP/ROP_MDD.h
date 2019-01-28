/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_MDD.h (ROP library, C++)
 *
 * COMMENTS:
 */

#ifndef __ROP_MDD__
#define __ROP_MDD__

#include "ROP_API.h"

#include <FS/FS_Writer.h>

#include "ROP_Node.h"

enum ropMDDParmIndex
{
    MDD_SOPPATH = 0,
    MDD_OUTPUT,
    MDD_RESTFRAME,
    MDD_XFORMTYPE,
    MDD_SCALE,
    MDD_COORDSYS,
    MDD_RELTIME,
    MDD_MKPATH,
    MDD_INITSIM,

    MDD_NUM_PARMS
};

#define STR_PARM(name, idx, t) \
    do { evalString(str, name, &mddIndirect[idx], 0, t); } while (false)
#define INT_PARM(name, idx, vi, t) \
    do { return evalInt(name, &mddIndirect[idx], vi, t); } while (false)
#define FLOAT_PARM(name, idx, vi, t) \
    do { return evalFloat(name, &mddIndirect[idx], vi, t); } while (false)

class SOP_Node;

class ROP_API ROP_MDD : public ROP_Node
{
public:
    static OP_Node		*myConstructor(OP_Network *net, const char*name, OP_Operator *op);

    OP_Context		&getContext() 	{ return myContext; }

    static OP_Operator		*buildOperator();
    static PRM_Template		 myTemplateList[];

protected:
	     ROP_MDD(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~ROP_MDD();

    virtual int			 startRender(int nframes, fpreal s, fpreal e);
    virtual ROP_RENDER_CODE	 renderFrame(fpreal time, UT_Interrupt *boss);
    virtual ROP_RENDER_CODE	 endRender();

    virtual void		 buildRenderDependencies(
				    const ROP_RenderDepParms &p);

    // Parameter fetching
    /// File name for output
    void	OUTPUT(UT_String &str, fpreal t)
		    {	getOutputOverrideEx(str, t, "file", "mkpath"); }

    /// Path to the SOP whose points we want to write to the file
    void	SOPPATH(UT_String &str, fpreal t) 
		    {	STR_PARM("soppath", MDD_SOPPATH, t);	}

    /// Should we initialize simulation SOPs?
    int		INITSIM()
		    {	INT_PARM("initsim", MDD_INITSIM, 0, 0); }

    enum CoordSysType {
	COORDSYS_LEFT,	// left-handed coordinate system (Houdini)
	COORDSYS_RIGHT	// right-handed coordinate system (Lightwave)
    };
    CoordSysType COORDSYS()
	{ return (CoordSysType)
		    evalInt("coordsys", &mddIndirect[MDD_COORDSYS], 0, 0); }

    int		 XFORMTYPE()
		    {	INT_PARM("xformtype", MDD_XFORMTYPE, 0, 0); }

    fpreal	 SX()
		    {	FLOAT_PARM("s", MDD_SCALE, 0, 0); }
    fpreal	 SY()
		    {	FLOAT_PARM("s", MDD_SCALE, 1, 0); }
    fpreal	 SZ()
		    {	FLOAT_PARM("s", MDD_SCALE, 2, 0); }

    bool	 RELTIME()
		    {	INT_PARM("reltime", MDD_RELTIME, 0, 0); }

    fpreal	 RESTFRAME()
		    {	FLOAT_PARM("restframe", MDD_RESTFRAME, 0, 0); }

private:
    /// This is used to look up the "real" index of ROP parameters because
    /// there are several constant parameters always in a ROP (frame range, 
    /// etc). It is allocated once by the first ROP_MDD constructor to run, 
    /// and filled in by OP_Parameters on eval* calls using the parameter
    /// names.
    static int		*mddIndirect;

    OP_Context		 myContext;
    SOP_Node		*myRenderSop;
    FS_Writer		*myFile;
    uint32		 myNumPoints;
};

#undef STR_PARM
#undef INT_PARM
#undef FLOAT_PARM

#endif
