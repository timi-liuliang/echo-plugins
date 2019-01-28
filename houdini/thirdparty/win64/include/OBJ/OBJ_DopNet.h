/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_DopNet.h (Custom Library, C++)
 *
 * COMMENTS:    An object to describe a motion capture handle
 *
 */

#ifndef __OBJ_DopNet__
#define __OBJ_DopNet__

#include "OBJ_API.h"
#include "OBJ_Node.h"
#include <DOP/DOP_Engine.h>
#include <DOP/DOP_Parent.h>
#include <SYS/SYS_Types.h>


#define FLOAT_OBJ_PARM(name, idx, vi, t)    \
	return evalFloat(name, &getIndirect()[idx], vi, t);
#define INT_OBJ_PARM(name, idx, vi, t)      \
	return evalInt(name, &getIndirect()[idx], vi, t);
#define STR_OBJ_PARM(name, idx, vi, t)      \
	evalString(str, name, &getIndirect()[idx], vi, t);


class SIM_RenderParms;
class UT_String;


enum OBJ_DopNetIndex
{
    I_DOPNETDISPLAYFILTER = I_N_BASE_INDICES,
    I_DOPNETISPLAYER,
    I_DOPNETPLAYFILESNAME,
    I_DOPNETINITIALSTATE,
    I_DOPNETTIMESTEP,
    I_DOPNETSUBSTEP,
    I_DOPNETTIMEOFFSET,
    I_DOPNETFRAMEOFFSET,
    I_DOPNETTIMESCALE,
    I_DOPNETMAXFEEDBACK,
    I_DOPNETRESIMULATE,
    I_DOPNETAUTORESIM,
    I_DOPNETDATAHINTS,
    I_DOPNETINTERPOLATE,
    I_DOPNETCACHEENABLED,
    I_DOPNETCACHETODISK,
    I_DOPNETCACHETODISKNONINTERACTIVE,
    I_DOPNETCACHESUBSTEPS,
    I_DOPNETCACHEMAXSIZE,
    I_DOPNETCOMPRESSSIMS,
    I_DOPNETTIMELESS,
    I_DOPNETEXPLICITCACHE,
    I_DOPNETEXPLICITCACHENAME,
    I_DOPNETEXPLICITCACHENSTEPS,
    I_DOPNETEXPLICITCACHECHECKPOINTSPACING,

    I_N_DOPNET_INDICES
};

enum 
{
    OBJDOPNET_VAR_SIMFRAME = OBJ_MAX_VARIABLES,
    OBJDOPNET_VAR_SIMFRAME1,
    OBJDOPNET_VAR_SIMFRAME2,
    OBJDOPNET_VAR_SIMFRAME3,
    OBJDOPNET_VAR_SIMFRAME4,
    OBJDOPNET_VAR_SIMFRAME5,
    OBJDOPNET_VAR_SIMFRAME6,
    OBJDOPNET_VAR_SIMFRAME7,
    OBJDOPNET_VAR_SIMFRAME8,
    OBJDOPNET_VAR_SIMFRAME9,
    OBJDOPNET_NUM_VARS
};

class OBJ_API OBJ_DopNet : public OBJ_Node, public DOP_Parent
{
public:
				 OBJ_DopNet(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_DopNet();

    static CH_LocalVariable	 theVariables[];
    /// Handle the evaluation of standards string local variables.
    virtual bool		 evalVariableValue(
				    UT_String &value, int index, int thread);
    virtual bool		 evalVariableValue(fpreal &v, int i, int thr)
				 {
				     return OBJ_Node::
						evalVariableValue(v, i, thr);
				 }

    virtual const char		*getChildType() const;
    virtual OBJ_OBJECT_TYPE	 getObjectType() const;
    virtual OP_OpTypeId		 getChildTypeID() const;

    static const char	*theChildTableName;

    virtual OP_Node		*getRenderNodePtr();
    virtual int			 isObjectRenderable(fpreal t) const;

    // Functions for going through all our simulation objects and returning
    // the geometry representing each one.
    int				 getNumDOPGeometries() const;
    GU_ConstDetailHandle	 getDOPGeometry(int index,
						UT_DMatrix4 &xform,
						bool fordisplay,
						bool acceptguide) const;
    const SIM_Data		*getDOPData(int index) const;
    const SIM_RootData		*getDOPRootData(int index) const;
    const SIM_RenderParms	*getDOPRenderParms(int index) const;
    bool			 getDOPIsGuide(int index) const;
    virtual bool		 getIsTimeless() const { return TIMELESS(); }

    virtual DOP_Parent		*castToDOPParent();
    virtual const DOP_Parent	*castToDOPParent() const;

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		*getObsolete();
    static PRM_Template		*getTemplateList();

    virtual int		isObjectLitBy(OBJ_Node *, fpreal) { return 1; }
    virtual bool	isLightInLightMask(const OBJ_Node *, fpreal)
			    { return true; }

    virtual int		 	 doDrawLit() const { return 1; }

    void	 DISPLAYFILTER(UT_String &str)
		 { STR_OBJ_PARM("displayfilter", I_DOPNETDISPLAYFILTER, 0, 0.0); }

    bool 	 ISPLAYER() const
    		 { INT_OBJ_PARM("isplayer", I_DOPNETISPLAYER, 0, 0.0); }
    void 	 PLAYFILESNAME(UT_String &str) const
    		 { STR_OBJ_PARM("playfilesname", I_DOPNETPLAYFILESNAME, 0, 0.0); }

    void	 INITIALSTATE(UT_String &str)
		 { STR_OBJ_PARM("initialstate", I_DOPNETINITIALSTATE, 0, 0.0); }
    fpreal	 TIMESTEP()
		 { FLOAT_OBJ_PARM("timestep", I_DOPNETTIMESTEP, 0, 0.0); }
    int		 SUBSTEP()
		 { INT_OBJ_PARM("substep", I_DOPNETSUBSTEP, 0, 0.0); }
    fpreal	 TIMEOFFSET()
		 { FLOAT_OBJ_PARM("timeoffset", I_DOPNETTIMEOFFSET, 0, 0.0); }
    fpreal	 STARTFRAME()
		 { INT_OBJ_PARM("startframe", I_DOPNETFRAMEOFFSET, 0, 0.0); }
    fpreal	 TIMESCALE()
		 { FLOAT_OBJ_PARM("timescale", I_DOPNETTIMESCALE, 0, 0.0); }
    int		 MAXFEEDBACK()
		 { INT_OBJ_PARM("maxfeedback", I_DOPNETMAXFEEDBACK, 0, 0.0); }
    int		 INTERPOLATE()
		 { INT_OBJ_PARM("interpolate", I_DOPNETINTERPOLATE, 0, 0.0); }
    int		 AUTORESIM()
		 { INT_OBJ_PARM("autoresim", I_DOPNETAUTORESIM, 0, 0.0); }
    int		 DATAHINTS()
		 { INT_OBJ_PARM("datahints", I_DOPNETDATAHINTS, 0, 0.0); }
    int		 CACHEENABLED()
		 { INT_OBJ_PARM("cacheenabled", I_DOPNETCACHEENABLED, 0, 0.0); }
    int		 CACHETODISK()
		 { INT_OBJ_PARM("cachetodisk", I_DOPNETCACHETODISK, 0, 0.0); }
    int		 CACHETODISKNONINTERACTIVE()
		 { INT_OBJ_PARM("cachetodisknoninteractive", I_DOPNETCACHETODISKNONINTERACTIVE, 0, 0.0); }
    int		 CACHESUBSTEPS()
		 { INT_OBJ_PARM("cachesubsteps", I_DOPNETCACHESUBSTEPS, 0, 0.0); }
    int		 CACHEMAXSIZE()
		 { INT_OBJ_PARM("cachemaxsize", I_DOPNETCACHEMAXSIZE, 0, 0.0); }

    void	 COMPRESSSIMS(UT_String &str)
		 { STR_OBJ_PARM("compresssims", I_DOPNETCOMPRESSSIMS, 0, 0.0); }

    bool 	 EXPLICITCACHE()
		 { INT_OBJ_PARM("explicitcache", I_DOPNETEXPLICITCACHE, 0, 0.0); }
    void 	 EXPLICITCACHENAME(UT_String &str)
		 { STR_OBJ_PARM("explicitcachename", I_DOPNETEXPLICITCACHENAME, 0, 0.0); }
    int  	 EXPLICITCACHENSTEPS()
		 { INT_OBJ_PARM("explicitcachensteps", I_DOPNETEXPLICITCACHENSTEPS, 0, 0.0); }
    int  	 EXPLICITCACHECHECKPOINTSPACING()
    		 { INT_OBJ_PARM("explicitcachecheckpointspacing", I_DOPNETEXPLICITCACHECHECKPOINTSPACING, 0, 0.0); }
    static int	 resimulateStatic(void *, int, fpreal, const PRM_Template *);

    // This must be const so we thus don't use the indirect.
    int		 TIMELESS() const
		 { return evalInt("timeless", 0, 0.0); }

    // The display and render ops of a DOP Network are always equal.
    virtual int			 getDandROpsEqual();
    virtual int			 updateDandROpsEqual(int check_inputs = 1);

    // Override clearInterrupted to avoid invalidating the whole cache when
    // the user interrupts a cook.
    virtual void		 clearInterrupted();

    virtual OBJ_DopNet		*castToOBJDopNet()	{ return this; }

    // invalidate any cached data
    virtual void	 clearCache();

    virtual bool opIsPlayer() const { return ISPLAYER(); }

protected:
    virtual bool	 	 updateParmsFlags();
    virtual void		 getDescriptiveName(UT_String &name) const;
    virtual void		 getDescriptiveParmName(UT_String &name) const;

    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return myDopNetIndirect; }

    virtual void		 childFlagChange(OP_Node *);

    // Returns true so we evaluate all parms when cooking. This ensures that
    // any parm dependencies in our simulation parms get set up.
    virtual bool	         cookedDataUsesAllParameters() const
				 { return true; }

    // Override DOP_Parent virtual functions.
    virtual const DOP_Engine	&getEngineSubclass() const;
    virtual void		 setDOPTimeSubclass(const SIM_Time &t);
    virtual DOP_Engine		&getNonConstEngineSubclass();
    virtual void		 filterDataSubclass(const SIM_Time &t);

    virtual void		 handleRefilterParms(const SIM_Time &t);
    virtual void		 handleResimParms(const SIM_Time &t);

    virtual bool	createSpareParametersFromChannels(
					UT_BitArray &selection,
					const CH_ChannelList &channels);
    virtual bool	createSpareParametersForLoad(UT_StringArray &names,
					PRM_ParmNameMap &nmap,
					UT_String &errors);

    virtual void	getOwnedMicroNodes(
			    DEP_MicroNodeList &micronodes);

    virtual void	dumpMicroNodes(
			    std::ostream &os,
			    bool as_DOT,
			    int indent_level) const;

private:
    DOP_Engine				 myEngine;
    UT_ValArray<const SIM_RootData *>	 myRootData;
    SIM_ConstDataArray			 myData;
    UT_Array<GU_ConstDetailHandle>	 myGeometries;
    UT_Array<UT_DMatrix4>		 myPositionData;
    UT_ValArray<const SIM_RenderParms *> myRenderParms;
    UT_BitArray				 myGeoIsGuide;

    static int				*myDopNetIndirect;
    static int				 myLastInternalParmIndex;
};

#undef FLOAT_OBJ_PARM
#undef INT_OBJ_PARM
#undef STR_OBJ_PARM

#endif
