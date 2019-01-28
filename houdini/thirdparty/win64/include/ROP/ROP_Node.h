/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP Library (C++)
 *
 * COMMENTS:	The base class for all Render Operators
 *
 */

#ifndef __ROP_Node_h__
#define __ROP_Node_h__

#include "ROP_API.h"
#include <OP/OP_Network.h>
#include <CH/CH_Manager.h>
#include "ROP_Verbose.h"
#include "ROP_RenderItem.h"

class OP_OperatorTable;
class OP_TemplatePair;
class IMG_Raster;
class UT_Interrupt;
class UT_IStream;

class ROP_Verbose;
class ROP_Node;
class ROP_ExecuteContext;
class ROP_IFD;
class ROP_IFDBase;
class ROP_SohoOp;
class ROP_RenderCmdParms;
class ROP_RenderItem;
class ROP_RenderList;
class ROP_RenderDepParms;
class GU_DetailHandle;
class TAKE_Take;
class TIL_Sequence;
class IMG_Format;
class IMG_TileOptions;
class IMG_FileParms;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newDriverOperator(OP_OperatorTable *table);
};

enum  ROP_RENDER_CODE
{
    ROP_ABORT_RENDER = 0,	// Stop rendering
    ROP_CONTINUE_RENDER = 1,	// Go on to the next frame
    ROP_RETRY_RENDER = 2	// Retry this frame (primarily for network)
};

enum ROP_RenderMode
{
    ROP_RM_RENDER_CONTROLS,
    ROP_RM_RENDER,
    ROP_RM_PREVIEW,
    ROP_RM_VIEWPORT,
    ROP_RM_BG_RENDER,
    ROP_RM_START,
    ROP_RM_END,
    RENDER_RM_PRM,
    RENDER_RM_GETRASTER
};

enum
{
    ROP_NODE_RENDER,
    ROP_NODE_PREVIEW,
    ROP_NODE_RENDER_CTRL,
    ROP_NODE_TRANGE,
    ROP_NODE_FRANGE,
    ROP_NODE_TAKE,
    ROP_NODE_MAXPARMS
};

typedef void	(*ROP_ExecuteOverride)(ROP_Node *me, fpreal time, void *data);
typedef void 	(*ROP_RenderButtonCB)(ROP_Node *, ROP_RenderMode);

#define FLT_PARM(name, vi, t)	\
		{ return evalFloat(name, vi, t); }
#define FLT_SET(name, vi, t, val)	\
		{ setFloat(name, vi, t, val); }
#define INT_SET(name, vi, t, val)	\
		{ setInt(name, vi, t, val); }

#define INT_PARM(name, vi, t) \
		{ return evalInt(name, vi, t); }

#define INT_SET(name, vi, t, val) \
		{ setInt(name, vi, t, val); }

#define STR_PARM(name, vi, t) \
		{ evalString(str, name, vi, t); }

//___________________________________________________________________________

class ROP_SohoOutput;
class SOHO_IPRList;
class ROP_OpenGL;

class ROP_API ROP_Node : public OP_Network
{
public:
    // Standard OP_Network stuff:
    static const char	*theChildTableName;
    virtual const char	*getChildType() const;
    virtual const char	*getOpType() const;

    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;
	
    //  Nobody should have to override this, but it's public for other people

    virtual bool	 updateParmsFlags();
    virtual OP_DataType	 getCookedDataType() const;
    virtual void	*getCookedData(OP_Context &context);

    // The castToROPIFD() method returns a pointer to an IFD node or NULL
    // if the ROP is not an IFD ROP.  If a pointer is returned it may be
    // a representative node, and care must be taken to not confuse the
    // returned node with the node on which the method was called.
    // TODO: check places that store the return of this function.
    virtual ROP_IFD	*castToROPIFD();
    virtual ROP_IFDBase	*castToROPIFDBase();
    virtual ROP_SohoOp	*castToROPSoho();
    virtual ROP_OpenGL	*castToROPOpenGL();

    virtual bool	 hasImageOutput();

    // Returns true if rendering will overwrite any existing files.
    virtual bool	 detectIfExistingOutputFiles();
    // Runs through the output sequence deleting any matching files.
    virtual void	 deleteExistingOutputFiles();

    static void		 buildOperatorTable(OP_OperatorTable &table);
    static OP_Operator  *getManagementOperator();
    static OP_Operator	*getCHOPManagementOperator();
    static OP_Operator	*getCOP2ManagementOperator();
    static OP_Operator	*getDOPManagementOperator();
    static OP_Operator	*getSOPManagementOperator();
    static OP_Operator	*getLOPManagementOperator();
    static void		 buildGameOperatorTable(OP_OperatorTable &table);
    static void		 initSimulationOPs();
    //
    // The following method will return 1 if the raster was successfully
    //	generated.  Otherwise, it will return 0.
    //  If the OP is incapable of generating a raster, an error will be
    //	generated.
    int			 renderFile(OP_Context &ctx, float aspect,
				    const char *file);

    /// Reference one parameter. However, for ROPs, we don't want to
    /// reference the render button.
    virtual bool	 referenceSingleParameter(OP_Parameters *from,
				int parm_idx,
				const char *relative_path = NULL);

    OP_ERROR		 execute(fpreal now, ROP_Verbose *verbose=NULL);
    OP_ERROR		 executeSingle(fpreal time,
				       ROP_Verbose *verbose=NULL,
				       SOHO_IPRList *ipr=0,
				       ROP_SohoOutput *out=0,
				       bool generate=false);

    // Execute full will perform a render for the viewport or render menu.
    //	1) If not a sequence render, force background rendering
    //	2) Call the render manager to execute the render network
    //	3) Restore things
    OP_ERROR		 executeFull(fpreal now, bool sequence = false);

    // These calling mechanisms allow us to start an execute, then call
    // nextExecute() on our own time, finishing with endExecute().  This allows
    // callers to do things between renders...
    ROP_ExecuteContext	*getExecuteContext() { return myEContext; }
    void		 setExecuteContext(ROP_ExecuteContext *c)
			 { myEContext = c; }

    void		 setExecuteOverride(ROP_ExecuteOverride f, void *d)
			 { myExecuteOverride = f;
			   myExecuteOverrideData = d;
			 }
    void		 setPreExecuteOverride(ROP_ExecuteOverride f, void *d)
			 { myPreExecuteOverride = f;
			   myPreExecuteOverrideData = d;
			 }
    void		 setPostExecuteOverride(ROP_ExecuteOverride f, void *d)
			 { myPostExecuteOverride = f;
			   myPostExecuteOverrideData = d;
			 }
    OP_ERROR		 startExecute(fpreal now, ROP_Verbose *verbose = NULL);
    OP_ERROR		 nextExecute(int &again);
    OP_ERROR		 endExecute(int &again);

    void			 setRenderMode(ROP_RenderMode mode)
                                 {
                                     myRenderMode = mode;
                                 }

    ROP_RenderMode		 getRenderMode()
                                 {
                                     return myRenderMode;
                                 }

    void			 setRenderOutput(UT_String &);
    UT_String			&getRenderOutput();
    UT_String			&getRenderDevice();
    UT_String			&getRenderSuffix();
    void			 setRenderResX(int x);
    void			 setRenderResY(int y);
    int				 getRenderResX();
    int				 getRenderResY();
    void			 setRenderAspect(float aspect);
    float			 getRenderAspect();

    virtual void	 getRenderedImageInfo(TIL_Sequence &seq);
    
    // The following methods have to be public for Remote
    virtual void		 SETDORANGE(int val)
				 { INT_SET("trange", 0, 0, val) }
    virtual int			 DORANGE()
				 {
				     if(myOverrideFrameRangeFlag)
					 return 1;
				     initRenderDependencyVars(CHgetEvalTime());
				     return myCachedDoRange;
				 }

    virtual fpreal	FSTART()
	        {
		    if(myOverrideFrameRangeFlag)
			return myOverrideStartFrame;
		    initRenderDependencyVars(CHgetEvalTime());
		    return myCachedStart;
		}
    virtual fpreal	FEND()
	        {
		    if(myOverrideFrameRangeFlag)
			return myOverrideEndFrame;
		    initRenderDependencyVars(CHgetEvalTime());
		    return myCachedEnd;
		}

    virtual fpreal	FINC()
		{
		    if(myOverrideFrameRangeFlag)
			return myOverrideFrameInc;
		    initRenderDependencyVars(CHgetEvalTime());
		    return myCachedInc;
		}

    
    static int			 doRenderCback(void *data, int index,
					       fpreal time,
					       const PRM_Template *);

    static int			 doRenderBackgroundCback(void *data, int index,
					       fpreal time,
					       const PRM_Template *);
    static int			 doRenderDialog(void *data, int index,
						fpreal time,
						const PRM_Template *);

    static PRM_Template		*getROPbaseTemplate();
    static PRM_Template		*getROPscriptTemplate();
    static OP_TemplatePair	*getROPcop2Template();
    static CH_LocalVariable	 myVariableList[];
   
    bool                         executePreRenderScript(fpreal ttime);
    bool                         executePreFrameScript(fpreal ttime);
    bool                         executePostFrameScript(fpreal ttime);
    bool                         executePostWriteScript(fpreal ttime);
    bool                         executePostRenderScript(fpreal ttime);

    static void		 buildRibVersionMenu(PRM_Name *menu, int max,
					     bool for_archive=false);

    //Absolute Width
    virtual fpreal       getW() const;
    //Absolute Height
    virtual fpreal       getH() const;

    // This adds render items to a ordered list. The list is traversed from
    // 1 -> entries to render. 
    virtual void	 buildRenderDependencies(const ROP_RenderDepParms &p);

    virtual void	 postProcessList( ROP_RenderList &list,
					  const ROP_RenderDepParms &p);

    virtual void	 initRenderDependencies();
    virtual void	 cleanupRenderDependencies();
    
	    void	 addJobFrames(const char *job_name,
				      const char *frame_string);

    static void		 setDependencyRenderFlag(bool f);
    static bool	         getDependencyRenderFlag();

    static void		 setFrameByFrameFlag(bool f);
    static bool		 getFrameByFrameFlag();

    static void		 setRenderScriptFlag(bool enable);
    static bool		 getRenderScriptFlag();

    static void		 setOutputResolutionScale(fpreal xscale,fpreal yscale);
    static void		 getOutputResolutionScale(float &xs, float &ys);

    virtual bool	 getOutputResolution(int &x, int &y)
			    { x = y = 0; return false; }
    virtual void	 getOutputFile(UT_String & /*name*/) { }

    // three levels - 0 = low/fastest, 1 = med/fast, 2 = normal
    static void		 setRenderQuality(int quality);
    int			 getRenderQuality();

    // Override the frame range used by this node
    void		 overrideFrameRange(bool enable,
					    fpreal s=1.0, fpreal e=1.0,
					    fpreal i=1.0);
    

    // Return true if the output is currently overridden
    bool		 isOutputOverridden() const
			    { return myOverrideOutputFlag; }

    // Override the output filename for this node. Used by the hscript render
    // -o command.
    virtual void	 overrideOutput(bool enable, const char *fname=0);

    // Return the overriden filename. 'fname' is NOT modified if this function
    // returns false. Deprecated, use getOutputOverrideEx() instead.
    bool		 getOutputOverride(
			    UT_String &fname,
			    fpreal t,
			    bool expand = true) const;

    // getOutputOverrideEx() differs from getOutputOverride() in that it will
    // perform the necessary parm evaluation as well so that we string result
    // will always returned. Since this requires expansion, it will always call
    // getOutputOverride(...,expand=true).
    //
    // For compatibility, it returns true if the result was overridden, false
    // otherwise. Method is non-const since it may add warnings upon failure to
    // create the output directory.
    bool		 getOutputOverrideEx(
			    UT_String &result,
			    fpreal t,
			    const char *output_parm_name,
			    const char *mkdir_parm_name);

    virtual void	 overrideDevice(bool enable, bool interactive=false,
					const char *devicestr=0);
    bool		 getDeviceOverride(UT_String &str, fpreal t, 
					   bool expand = true) const;

    static void		 setRenderButtonCB(ROP_RenderButtonCB cb);

    virtual void	 inputConnectChanged(int which);
    
    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    virtual void	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms);

    virtual bool	 getGeometryHandle(const char *path, fpreal t,
					   GU_DetailHandle &gdh,
					   UT_DMatrix4 &world_xform)
			     { return(false); }

    // forces a BG render in executeSingle().
    void		 forceBackgroundRender(bool f = true)
			     { myForceBackgroundRender = f; }
    bool		 getForceBackgroundRender() const
			     { return myForceBackgroundRender; }

    // Add some additional inputs to the ROP (used by Fetch and Subnet).
    void		 setTemporaryInputs(const UT_ValArray<ROP_Node *> &in)
			     { myTemporaryInputs = in; }
    void		 clearTemporaryInputs() { myTemporaryInputs.entries(0);}
    
    /// Render this node using the parameters set in render_parms.
    bool		 renderCommand(ROP_RenderCmdParms &render_parms);

    // Copy the errors from this node into an error manager.  This method
    // is used to save off errors before they're cleared.
    void		 copyErrorsInto(UT_ErrorManager &error_manager)
			{ error_manager.stealErrors(*getLockedErrorManager()); }

    // Append the errors from an error manager into this node.
    void		 copyErrorsFrom(UT_ErrorManager &error_manager)
			{ getLockedErrorManager()->stealErrors(error_manager); }

    // Get the last render dependency item that cooked in this node in the
    // current cook.
    ROP_RenderItemHandle getLastRenderDepItem() const
			    { return myLastRenderItem; }

    /// Helper that appends the inputs and other rop nodes that
    /// this node depends on to the give list.
    virtual bool	isRenderDependentOnNode(ROP_Node *node,
				  const ROP_RenderDepParms &parms);
   
    static void		setColorCorrectionIPGamma(bool enable, fpreal gamma);
    static fpreal	getColorCorrectionIPGamma();
    static void		setColorCorrectionIPLUT(bool enable, const char *lut);
    static const char  *getColorCorrectionIPLUT();

    static void		setColorCorrectionEnvVariables(bool set);
    static void		setFPSVariable(bool do_set = 0, fpreal frame_inc = 1);

    virtual ROP_RENDER_CODE rerenderFrame();

protected:
	     ROP_Node(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~ROP_Node();

    // These OPs don't actually cook.  No data, nothing.  The renderFile()
    //	method is the closest thing to a cook.
    virtual OP_ERROR	 cookMe(OP_Context &context);
    virtual OP_ERROR	 bypassMe(OP_Context &context, int &copied_input);
    OP_ERROR		 initFrameRange(fpreal now, ROP_ExecuteContext &ctx);

    virtual void	 onCreated();

    
    void		 addSystemError(const char *msg = 0)
			 { getLockedErrorManager()->systemError(msg);}
    void		 addError(int code, const char *msg = 0)
			 { getLockedErrorManager()->addError(ROP_OPTYPE_NAME, code, msg);}
    void		 addWarning(int code, const char *msg = 0)
			 { getLockedErrorManager()->addWarning(ROP_OPTYPE_NAME, code, msg); }
    void		 addMessage(int code, const char *msg = 0)
			 { getLockedErrorManager()->addMessage(ROP_OPTYPE_NAME, code, msg); }

    virtual bool	 evalVariableValue(UT_String &v, int index, int thread)
			 { return OP_Network::evalVariableValue(v, index, thread); }
    virtual bool	 evalVariableValue(fpreal &v, int index, int thread);

    // The endRender() method will always be called if startRender succeeds.
    virtual int			startRender(int nframes, fpreal tstart,
					    fpreal tend);
    virtual ROP_RENDER_CODE	renderFrame(fpreal time,
					    UT_Interrupt *boss = 0);
    virtual ROP_RENDER_CODE	endRender();


    TAKE_Take			*applyRenderTake(const char *take);
    void			 restorePreviousTake(TAKE_Take *take);

    void			 initRenderDependencyVars(fpreal t);
    virtual void		 doInitRenderDependencyVars(fpreal t);

    // Internal helper method that simply traverses our inputs and calls
    // buildRenderDependencies on them.
    virtual void	 buildInputRenderDependencies(
					       const ROP_RenderDepParms &parms);

    bool		 matchingLastRenderParms(
					       const ROP_RenderDepParms &parms);
    void		 setLastRenderDepItem(ROP_RenderItemHandle item)
			    { myLastRenderItem = item; }
    void		 clearLastRenderDep();
     
    // This method returns the image device required to render it to an
    // interactive window (an mplay window, for example).
    virtual const char	*getInteractiveImageDevice() const { return ""; }
    
    virtual void	FSTART_SET(fpreal val)
		{ FLT_SET("f", 0, 0, val); }
    virtual void	FEND_SET(fpreal val)
		{ FLT_SET("f", 1, 0, val); }
    virtual void	FINC_SET(fpreal val)
		{ FLT_SET("f", 2, 0, val); }
    virtual void	DORANGE_SET(int val)
		{ INT_SET("trange", 0, 0, val); }

    virtual void	RENDER_TAKE(UT_String &take)
		{ evalString(take, "take", 0, 0.0f); }

    UT_String			 myRenderOutput;
    UT_String			 myRenderDevice;
    UT_String			 myRenderSuffix;
    float			 myRenderAspect;
    int				 myRenderX;
    int				 myRenderY;
    ROP_RenderMode		 myRenderMode;
    int				 myCurrentFrame;
    int				 myTotalFrames;
    bool			 myExecuteSingle;
    UT_String			 myJobTag;
    UT_String			 myJobFrames;

    ROP_Verbose	&getRopVerbose()
		    { return myVerbose; }
    std::ostream *getVerbose() const
		    { return myVerbose.getStream(); }
    bool	 getAlfred() const
		    { return myVerbose.getAlfred(); }

    int			 myCachedDoRange;
    fpreal		 myCachedStart;
    fpreal		 myCachedEnd;
    fpreal		 myCachedInc;
    OP_VERSION		 myCachedVersion;
    fpreal		 myCachedTime;

protected:
    //
    //  These virtuals are methods that nobody should override...
    //	Except IPR buffers which are now saved as cooked data
    //
    //  I/O methods
    //
    virtual void	 deleteCookedData();
    virtual int		 saveCookedData(const char *, OP_Context &);
    virtual int		 saveCookedData(std::ostream &os, OP_Context &,
					int binary = 0);

    // If there is a problem executing the script, then an error will be set
    // here.
    // Note that this has to remain protected (and not be made private) because
    // there are customers who inherit from ROP_Node and use this function.
    OP_ERROR	 executeScript(
	UT_String &str, CH_ScriptLanguage language, fpreal time);

    int			getNumTemporaryInputs() const
			    { return myTemporaryInputs.entries(); } 

    ROP_Node *		getTemporaryInputs(int i) const
			    { return myTemporaryInputs(i); } 

    // Create all intermediate directories for the given file path. Returns
    // false IFF it attempted to create the directories but failed to do so. In
    // that case, a warning will be added.
    bool		makeFilePathDirs(const UT_String &path);
    
    /// Override if preview is supported by rendering to 'ip'. 
    virtual bool	isPreviewAllowed() { return false; }

    
    void		applyImageFormatOptions(IMG_TileOptions &opts,
						const IMG_Format *fmt,
						fpreal t);
    void		applyImageFormatOptions(IMG_FileParms &parms,
						const IMG_Format *fmt,
						fpreal t);
	
private:
    virtual const char	*getFileExtension(int binary) const;

    static void	 resetSimulation(OP_Node* node);

    ROP_ExecuteContext	*myEContext;
    ROP_Verbose		 myVerbose;
    ROP_ExecuteOverride	 myExecuteOverride;
    void		*myExecuteOverrideData;
    ROP_ExecuteOverride	 myPreExecuteOverride;
    void		*myPreExecuteOverrideData;
    ROP_ExecuteOverride	 myPostExecuteOverride;
    void		*myPostExecuteOverrideData;

    bool		 myForceBackgroundRender;
    
    bool		 myOverrideFrameRangeFlag;
    fpreal		 myOverrideStartFrame;
    fpreal		 myOverrideEndFrame;
    fpreal		 myOverrideFrameInc;

    bool		 myOverrideOutputFlag;
    UT_String		 myOverrideOutputName;

    bool		 myOverrideDeviceFlag;
    UT_String		 myOverrideDeviceName;

    bool		 myExecuteFull;		// Recursion check
    UT_ValArray<ROP_Node *> myTemporaryInputs;
    ROP_RenderDepParms	*myLastRenderParms;
    ROP_RenderItemHandle myLastRenderItem;

    bool                 executeParmScript(const char *parm_name,
					   fpreal ttime);
    bool                 executeParmScripts(const char *parm_name,
					    fpreal ttime);

    static int		 doRenderCommand(CMD_Args &args, OP_Node *net,
					 bool showerrors);

    static void		 installCommands();
    static void		 cmdRender(CMD_Args &args);
    static void		 cmdRenderDeps(CMD_Args &args);
};

// These are the parms to the buildRenderDependencies methods.
class ROP_RenderDepParms
{
public:
		     ROP_RenderDepParms() :
			 myOrder(NULL),
			 myFullStart(0.0f),
			 myFullEnd(0.0f),
			 myFullFrameInc(0.0f),
			 myStart(0.0f),
			 myEnd(0.0f),
			 myFrameInc(0.0f),
			 myDoInputs(false),
			 myIgnoreBypass(true),
			 myIgnoreLocks(false),
			 myFrameMerge(true),
			 myRenderSerial(-1),
			 myDepParent(NULL),
			 myTopLevelRop(NULL),
			 myFirstFrameFlag(true) { }

    // copy constructor; some members are initilized to NULL rather than copied.
		     ROP_RenderDepParms(const ROP_RenderDepParms &copy)
			 : myRenderSerial(-1)
			{
			    *this = copy;
			}
		    ~ROP_RenderDepParms()
		     {
			 myDepParent = NULL;
		     }
    
    // Set/Get RenderList
    void	     setOrder(ROP_RenderList &order) { myOrder = &order; }
    ROP_RenderList  &getOrder() const { UT_ASSERT(myOrder); return *myOrder; }

    // Set/Get Frame Range
    void	     setFullFrameRange(fpreal start, fpreal end, fpreal finc)
		     { myFullStart = start; myFullEnd = end; myFullFrameInc = finc; }
    void	     setFrameRange(fpreal start, fpreal end, fpreal finc)
		     { myStart = start; myEnd = end; myFrameInc = finc; }
    fpreal	     getFullStart()	    const { return myFullStart; }
    fpreal	     getFullEnd()	    const { return myFullEnd; }
    fpreal	     getFullFrameInc()  const { return myFullFrameInc; }
    fpreal	     getStart()	    const { return myStart; }
    fpreal	     getEnd()	    const { return myEnd; }
    fpreal	     getFrameInc()  const { return myFrameInc; }

    void	    setFullAscendingFrameRange(ROP_Node &rop)
		    {
			fpreal start = myFullStart;
			fpreal end = myFullEnd;
			fpreal finc = myFullFrameInc;
			if(rop.DORANGE()==2)
			{
			    start = rop.FSTART();
			    end = rop.FEND();
			    finc = rop.FINC();
			}
			if (start > end)
			    UTswap(start, end);
			if (finc <= 0)
			    finc = 1.0;
			setFrameRange(start, end, finc);
		    }

    // Set/Get Input traversal flags
    void	     setDoInputsFlag(bool do_inputs) { myDoInputs = do_inputs; }
    bool	     getDoInputs()	   const { return myDoInputs; }

    // Set/Get TopLevel ROP -- this is the top most ROP that is traversed.  This
    // is used to prevent subnet traversals to go outside this ROP.  Only
    // useful when DoInputs is off.
    void	     setTopLevelRop(ROP_Node *rop) { myTopLevelRop = rop; }
    ROP_Node	    *getTopLevelRop() const        { return myTopLevelRop; }

    // Set/Get Ignore flags
    void	     setIgnoreFlags(bool ignore_bypass, bool ignore_locks)
		     {
			 myIgnoreBypass = ignore_bypass;
			 myIgnoreLocks  = ignore_locks;
		     }
    bool	     getIgnoreBypass() const { return myIgnoreBypass; }
    bool	     getIgnoreLocks()  const { return myIgnoreLocks; }

    // Set/Get Render Serial number
    void	     setRenderSerial(int render_serial) 
		     { myRenderSerial = render_serial; }
    int		     getRenderSerial() const { return myRenderSerial; }

    // Set/Get Dependency parent
    void	     setDepParent(const ROP_RenderItemHandle &dep_parent)
		     { myDepParent = dep_parent; }
    const ROP_RenderItemHandle &getDepParent() const { return myDepParent; }

    void	     setFirstFrame(bool first) { myFirstFrameFlag = first; }
    bool	     isFirstFrame() const { return myFirstFrameFlag; }

    void	     setFrameMerge(bool merge)  { myFrameMerge = merge; }
    bool	     getFrameMerge() const	{ return myFrameMerge; }

    const ROP_RenderDepParms &operator=(const ROP_RenderDepParms &copy)
	{
	    myOrder = copy.myOrder;
	    myDepParent = copy.myDepParent;
	    myTopLevelRop = copy.myTopLevelRop;
	    myFullStart = copy.myFullStart;
	    myFullEnd = copy.myFullEnd;
	    myFullFrameInc = copy.myFullFrameInc;
	    myStart = copy.myStart;
	    myEnd = copy.myEnd;
	    myFrameInc = copy.myFrameInc;
	    myRenderSerial = copy.myRenderSerial;
	    myDoInputs = copy.myDoInputs;
	    myIgnoreBypass = copy.myIgnoreBypass;
	    myIgnoreLocks = copy.myIgnoreLocks;
	    myFrameMerge = copy.myFrameMerge;
	    myFirstFrameFlag = copy.myFirstFrameFlag;
	    return *this;
	}
    bool operator==(const ROP_RenderDepParms &copy)
	{
	    return (myFullStart == copy.myFullStart &&
		    myFullEnd == copy.myFullEnd &&
		    myFullFrameInc == copy.myFullFrameInc &&
		    myStart == copy.myStart &&
		    myEnd == copy.myEnd &&
		    myFrameInc == copy.myFrameInc &&
		    myDoInputs == copy.myDoInputs &&
		    myIgnoreBypass == copy.myIgnoreBypass &&
		    myIgnoreLocks == copy.myIgnoreLocks &&
		    myFrameMerge == copy.myFrameMerge &&
		    myRenderSerial == copy.myRenderSerial);
	}
	
private:
    ROP_RenderList	*myOrder;
    ROP_RenderItemHandle myDepParent;
    ROP_Node		*myTopLevelRop;
    fpreal		 myFullStart;
    fpreal		 myFullEnd;
    fpreal		 myFullFrameInc;
    fpreal		 myStart;
    fpreal		 myEnd;
    fpreal		 myFrameInc;
    int			 myRenderSerial;
    bool		 myDoInputs;
    bool		 myIgnoreBypass;
    bool		 myIgnoreLocks;
    bool		 myFrameMerge;
    bool		 myFirstFrameFlag;
};

#undef FLT_PARM
#undef INT_PARM
#undef STR_PARM
#undef FLT_SET
#undef INT_SET

#endif
