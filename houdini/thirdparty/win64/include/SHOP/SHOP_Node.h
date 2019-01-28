/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Node.h ( SHOP Library, C++)
 *
 * COMMENTS:	Shader OPs.  These are nodes which collect all shading
 *		information for nodes into a nice compact encapsulation.
 */

#ifndef __SHOP_Node__
#define __SHOP_Node__

#include "SHOP_API.h"

#include "SHOP_ClerkCache.h"
#include "SHOP_Error.h"

#include <OP/OP_Network.h>
#include <IMG/IMG_Raster.h>
#include <UT/UT_Map.h>

class UT_Options;
class OP_OperatorTable;
class VOP_Language;
class SHOP_ReData;
class SHOP_Node;
class SHOP_Clerk;
class SHOP_ClerkCacheEntry;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newShopOperator(OP_OperatorTable *);
}

// Sub-class variables should start their local variables at SHOP_VAR_MAX_BASE
enum {
    SHOP_VAR_OBJ = 0,
    SHOP_VAR_SOP,
    SHOP_VAR_MAX_BASE
};

#include "SHOP_NodeTypes.h"

#define SHOP_RENDERTYPE_OPTION	    "rendertype"
#define SHOP_FPRECISION_OPTION	    "soho_precision"
#define SHOP_DEFAULT_CLERK	    "VMantra"

class SHOP_API SHOP_Node : public OP_Network
{
public:
	     SHOP_Node(OP_Network *dad, const char *name,
		       OP_Operator *entry, SHOP_TYPE shader_type=SHOP_SURFACE);
    virtual ~SHOP_Node();

    static OP_VariablePair	myVariablePair;
    static CH_LocalVariable	myVariableList[];

    /// Returns the list of obsolete parameters.
    static PRM_Template	*getObsolete(SHOP_TYPE type);

    const UT_String	&getRenderMask() const	{ return myRenderMask; }

    virtual int		 getFirstParameter();
    virtual int		 getLastParameter();

    virtual bool	 runCreateScript();
    virtual void	 onCreated();

    // The options may contain:
    //	rendertype:  The the shader for a specific renderer
    virtual SHOP_Node	*findShader(SHOP_TYPE shop_type, fpreal now,
				    const UT_Options *options);

    /// Returns true if the shop node provides a shader for a given shop_type.
    virtual bool	 matchesShaderType(SHOP_TYPE shop_type);

    // Collect all shaders of the specific type which are hooked up.
    // The return code is the number of shaders in the list.
    virtual int		 collectShaders(UT_ValArray<SHOP_Node *>&list,
				    SHOP_TYPE shop_type, 
				    fpreal now, const UT_Options *options);

    /// Returns true if co-shader parameters may have corresponding node inputs.
    bool		 canHaveInputsForCoshaderParms() const;

    /// Returns the langauge used by this shop.
    const VOP_Language	*getLanguage() const;

    /// Collect all coshader parameter names on this node.
    /// If is_coshader_array is not null, the bits are set for parms 
    /// that are coshader arrays (rather than just a co-shader).
    void		 gatherCoShaderParmNames(UT_StringArray& coshaderparms,
				UT_BitArray *is_coshader_array);

    /// Gets the input node corresponding to the given co-shader parameter.
    SHOP_Node *		 getCoShaderInputNode(const char* parm_name);

    /// Obtain the coshader node(s) specified by the given parameter. 
    /// If there is an input node wired to the connector corresponding to 
    /// this parm, then it is used for constructing the list, otherwise, 
    /// parameter is evaluated and the op path is used for the list.
    /// For parameters representing a single co-shader, the list will
    /// contain zero or one entries (ie, the input node or node in parm path),
    /// but for parameters representing co-shader arrays, the list may contain 
    /// more than one entry (eg, merge shop input would build a larger array). 
    void		 getCoShaderNodes(UT_ValArray<SHOP_Node *>&list,
				    const char* parm_name,
				    fpreal now, const UT_Options *options);

    /// Represents this shop as a coshader list. Usually just a single entry,
    /// but some shops add several entries (eg, merge co-shaders shop).
    virtual void	 representAsCoShader(UT_ValArray<SHOP_Node *>&list);

    /// Collect coshader nodes for this shop's shader of a given type.
    virtual void	 gatherCoShaderNodes(UT_ValArray<SHOP_Node *>&list,
				    SHOP_TYPE main_shader_type,
				    fpreal now, const UT_Options *options);

    virtual int		 getInputTileLOD() const;
    virtual int		 getOutputTileLOD() const;
    virtual bool	 isInputVisible(int idx) const;
    virtual bool	 isOutputVisible(int idx) const;

    virtual SHOP_Node	*getOutputNode() const;
    virtual void	 getInputName(UT_String &name, int idx) const;
    virtual int		 getInputFromName(const UT_String &in) const;
    virtual SHOP_TYPE	 getInputType(int idx) const;
    virtual void	 getOutputName(UT_String &name, int idx) const;
    virtual int		 getOutputFromName(const UT_String &out) const;
    virtual SHOP_TYPE	 getOutputType(int idx) const;

    virtual void	 saveDialogScriptExtraInfo(std::ostream &os);
    virtual void	 createGalleryEntry(OP_GalleryEntry &entry);

    const UT_IntArray	&getVisibleInputs() const
			 { return myVisibleInputs; }
    const UT_IntArray	&getVisibleOutputs() const
			 { return myVisibleOutputs; }

    // This is only called from a Clerk to find out the shader name (when
    // building the render data). Note that the returned pointer may become
    // invalidated, so make a copy of the data if it must be kept around.
    virtual UT_StringHolder 
			 getShaderName(bool forvex, SHOP_TYPE shoptype) const;
    virtual SHOP_TYPE	 getShaderType() const;
    virtual bool	 evalVariableValue(fpreal &v, int idx, int thread)
		    { return OP_Network::evalVariableValue(v, idx, thread); }
    virtual bool	 evalVariableValue(UT_String &str, int idx, int thread);

    virtual bool	 createSpareParametersFromChannels(
				    UT_BitArray &selection,
				    const CH_ChannelList &channels);
    virtual bool	 createSpareParametersForLoad(
				    UT_StringArray &names,
				    PRM_ParmNameMap &nmap,
				    UT_String &errors);

    static void		 pushCallers(OP_Node *obj, OP_Node *sop);
    static void		 popCallers();

    /// Obtain a clerk that matches the shop type of this node and the render
    /// type. If the shop type of this node is multi-type, then the method
    /// argument is used to interpret the multi type as a specific shop type,
    /// which is then used to match the clerk.
    SHOP_Clerk		*getClerk(const char *rendertype,
				    SHOP_TYPE interpret_type = SHOP_INVALID );
    SHOP_Clerk		*getAnyClerk(bool only_string_generators);

    virtual void	 buildVexScript(UT_String &result,
				    fpreal t,
				    OP_Node *parent,
				    UT_Map<int, bool> *visitlist);
    virtual void	 buildVexScript(UT_String &result,
				    fpreal t,
				    DEP_MicroNode *depnode,
				    UT_Map<int, bool> *visitlist);

    virtual bool buildShaderString(UT_String &result, fpreal now,
		    const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		    SHOP_TYPE interpret_type = SHOP_INVALID);
    virtual bool buildShaderHandle(UT_String &result, fpreal now,
		    const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		    SHOP_TYPE interpret_type = SHOP_INVALID);
    virtual bool buildShaderLanguage(UT_String &result, fpreal now,
		    const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		    SHOP_TYPE interpret_type = SHOP_INVALID);
    virtual bool buildShaderData(SHOP_ReData &data, fpreal now,
		    const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		    SHOP_TYPE interpret_type = SHOP_INVALID);
    virtual bool buildShaderBounds(UT_BoundingBox &box, fpreal now,
		    const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		    SHOP_TYPE interpret_type = SHOP_INVALID);
    virtual bool buildCoShaderStrings(UT_StringArray &result,OP_NodeList &shops,
		    fpreal now, const UT_Options *options,
		    OP_Node *obj=0, OP_Node *sop=0, 
		    SHOP_TYPE interpret_type = SHOP_INVALID);
    virtual bool buildCoShaderHandles(UT_StringArray &result, fpreal now,
		    const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		    SHOP_TYPE interpret_type = SHOP_INVALID);
    virtual bool buildCoShaderLanguages(UT_StringArray &result, fpreal now,
		    const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		    SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Build a shader string for a VOP node.  If the VOP isn't a code creator,
    /// the code creator parent of the VOP will have its string generated.
    virtual bool	buildVopShaderString(VOP_Node *vop,
				    UT_String &result, fpreal now,
				    const UT_Options *options,
				    OP_Node *obj, OP_Node *sop,
				    SHOP_TYPE interpret_type = SHOP_INVALID);

    void		  destroyCache();
    SHOP_ClerkCacheEntry *getClerkData(const SHOP_Clerk *clerk);
    void		  destroyClerkData(const SHOP_Clerk *clerk);
    void		  addClerkData(const SHOP_Clerk *clerk,
					SHOP_ClerkCacheEntry *entry);

    /// Returns the shader types this shop outputs.
    virtual void	 getOutputShaderTypes(UT_Array<SHOP_TYPE> & types);

    static void		 buildShaderTable(OP_OperatorTable &table);
    static void		 initializeExpressions();
    static OP_Operator  *getManagementOperator();
    static OP_Operator	*getMaterialManagementOperator();

    // Convenience error methods
    void	 addError(int code, const char *msg = 0)
		 { UTaddError("SHOP", code, msg);}
    void	 addMessage(SHOP_ErrorCode code, const char *msg = 0)
		 { UTaddMessage("SHOP", code, msg);}
    void	 addWarning(SHOP_ErrorCode code, const char *msg = 0)
		 { UTaddWarning("SHOP", code, msg);}
    void	 addFatal(SHOP_ErrorCode code, const char *msg = 0)
		 { UTaddFatal("SHOP", code, msg);}
    void	 addSystemError(const char *msg = 0)
		 { UTaddSystemError(msg);}
    void	 addCommonError(UT_CommonErrorCode what, const char *msg = 0)
		 { UTaddCommonError(what, msg);}

    bool	 containsShopType(SHOP_TYPE type) const;
    void	 bumpContainedType(SHOP_TYPE type, int dir);

    int		 getOGLScriptCacheState() const { return myOGLScriptCache; }
    void	 setOGLScriptCache(int val, 
				   const char *vtxsrc, const char *geomsrc,
				   const char *fragsrc) 
				{ myOGLScriptCache = val;
				  myOGLVtxSource.harden(vtxsrc);
				  myOGLGeomSource.harden(geomsrc);
				  myOGLFragSource.harden(fragsrc);
				  myOGLProgram.clear();
				}
    void	 setOGLShaderProgram(bool enable, 
				   const char *program_file) 
			        { myOGLScriptCache = enable ? 1 : -1;
				    myOGLVtxSource.clear();
				    myOGLGeomSource.clear();
				    myOGLFragSource.clear();
				    myOGLProgram.harden(program_file);
				}
    const char  *getOGLVtxSource() const 
				{ return (const char *) myOGLVtxSource; }
    const char  *getOGLGeomSource() const 
				{ return (const char *) myOGLGeomSource; }
    const char  *getOGLFragSource() const 
				{ return (const char *) myOGLFragSource; }

    const char  *getOGLShaderProgram() const 
				{ return (const char *) myOGLProgram; }

    // The following evaluator will evaluate the string parameter.  However, it
    // will also perform any bundle pattern expansion, or matrix evaluation
    // based on the PRM_SpareData.
    // The quoted bool returns whether the result should be considered to be a
    // string (or whether it's another data type).
    void		 evalStringParm(UT_String &result,
					const char *parm_name,
					int vector_index, fpreal now,
					const PRM_Template *tplate,
					bool &quoted,
					const char *rtype, int fprecision);

    // Standard OP Network methods
    static const char	*theChildTableName;
    virtual const char	*getChildType() const;
    virtual const char	*getOpType() const;

    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;

    // Absolute Width
    virtual fpreal       getW() const;
    // Absolute Height
    virtual fpreal       getH() const;  

    // SHOP Parameter Evaluation with options.
    bool		 evalShopParameter(const char *name, int vec_idx,
					    fpreal now, int &value,
					    const UT_Options *options,
					    bool add_cook_dependency=false);
    bool		 evalShopParameter(const char *name, int vec_idx,
					    fpreal now, fpreal &value,
					    const UT_Options *options,
					    bool add_cook_dependency=false);
    bool		 evalShopParameter(const char *name, int vec_idx,
					    fpreal now, UT_String &value,
					    const UT_Options *options,
					    bool add_cook_dependency=false);

    // Overrides of OP_Node methods specific to materials 
    virtual UT_String	*getMaterialIconFilename()
			 { return &myMaterialIconFilename; }
    virtual void	 setMaterialIconFilename(const char *icon_file)
			 { myMaterialIconFilename.harden(icon_file); }

    virtual IMG_Raster	*getMaterialIconImage()
			 { return &myMaterialIconImage; }

    virtual bool	 getMaterialIconAllowRegenerateFlag() 
			 { return myAllowIconRegeneration; }
    virtual void	 setMaterialIconAllowRegenerateFlag(bool allow) 
			 { myAllowIconRegeneration = allow; }

    virtual SHOP_TYPE	 getNamedInputType(
				const OP_ConnectorId& input_name) const;
    virtual SHOP_TYPE	 getNamedOutputType(
				const OP_ConnectorId& output_name) const;

    /// Returns a procedural shader node of a given type, if the shop
    /// indeed has such a node. Returns NULL otherwise.
    virtual VOP_Node *	getProcedural(VOP_Type type);

protected:
    virtual void	 changeParmTemplate(PRM_Template *new_template);
    virtual const PRM_Template	*getShaderParmTemplates();

    // Standard OP Node methods
    virtual int		 getDataClass() const;
    virtual OP_ERROR	 cookMe(OP_Context &);
    virtual OP_ERROR	 bypassMe(OP_Context &, int &);
    virtual const char	*getFileExtension(int binary) const;

    virtual OP_DataType	 getCookedDataType() const;
    virtual void	 deleteCookedData();
    virtual int		 saveCookedData(const char *, OP_Context &);
    virtual int		 saveCookedData(std::ostream &, OP_Context &, int);

    virtual bool	 cookedDataUsesAllParameters() const
			 { return true; }
    virtual bool	 cookedDataNeedsErrors() const
			 { return true; }

    /// Disable parms whose corresponding input connectors are wired in.
    virtual bool	 updateParmsFlags();

    // Regenerates the visible inputs and outputs arrays.
    virtual void	 updateVisibleInputsAndOutputs();

    // This method will expand the pattern specified, possibly generating a
    // bundle for expansion.
    void		 resolveParmBundle(const char *parm_name,
					   int vector_index, 
					   UT_String &pattern,
					   OP_Network *creator,
					   const char *filter,
					   bool expand_fullpath = false);

    // Instead of simply getting the node wired into the input specified, we
    // also get the output number which it's wired from.
    SHOP_Node		*getFullInput(int input_idx, int &output_idx) const;

    void		 setShaderType(SHOP_TYPE t);

    /// Checks if the options specify a render mask that limit
    /// the types of nodes that can be returned by findShader().
    /// Returns this object if no clerk name is specified or if
    /// the node maches the specified clerk. Otherwise, returns NULL.
    SHOP_Node *		 checkRenderMask( const UT_Options *options);

    bool		 renderMatch(const char *rendertype) const;
    bool		 renderMatch(const SHOP_Clerk &clerk) const;

    int				 myContainedShops[SHOP_MAX_TYPES];
    UT_String			 myRenderMask;
    UT_Map<SHOP_ClerkCacheKey, SHOP_ClerkCacheEntry *> myClerkCache;
    int				 myOGLScriptCache;
    UT_String			 myOGLVtxSource;
    UT_String			 myOGLGeomSource;
    UT_String			 myOGLFragSource;
    UT_String			 myOGLProgram;

    UT_IntArray			 myVisibleInputs;
    UT_IntArray			 myVisibleOutputs;

    UT_String			 myMaterialIconFilename;
    IMG_Raster			 myMaterialIconImage;
    bool			 myAllowIconRegeneration; 

private:
    // This is overriden to selectively save nodes for compiled assets.
    virtual bool		 getAllowSavingChild(OP_Node *node,
					const OP_SaveFlags &flags);

    static void			 installCommands();

    SHOP_TYPE			 myShaderType;
};

// ============================================================================
class SHOP_API SHOP_AutoPushCallers
{
public:
    SHOP_AutoPushCallers(OP_Node *obj, OP_Node *sop)
	: myObject(obj)
	, mySop(sop)
    {
	if (myObject || mySop)
	    SHOP_Node::pushCallers(obj, sop);
    }
    ~SHOP_AutoPushCallers()
    {
	if (myObject || mySop)
	    SHOP_Node::popCallers();
    }

private:
    OP_Node	*myObject;
    OP_Node	*mySop;
};

// ============================================================================
/// Filter used by material vop containers.
class SHOP_API SHOP_MaterialFilter : public OP_OperatorFilter
{
public:
    void		setContextName(const UT_StringHolder &context);
    virtual bool	allowOperatorAsChild(OP_Operator *op);

private:
    UT_StringHolder	myContextName;
};


#endif
