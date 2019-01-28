/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_ScriptOperator.h ( OP Library, C++)
 *
 * COMMENTS:	Reads an OP definition from a script to generate the parm
 *		template, etc.
 */

#ifndef __OP_ScriptOperator__
#define __OP_ScriptOperator__

#include "OP_API.h"
#include "OP_Operator.h"
#include <UT/UT_Lock.h>
#include <UT/UT_SymbolTable.h>
#include <SYS/SYS_Types.h>
#include <sys/types.h>

class UT_InfoTree;
class PRM_ScriptParm;
class PRM_ScriptPage;
class PRM_ScriptImports;
class PRM_Name;
class DS_Stream;
class OP_Node;

// Function to call for each parameter.  Return 1 if successful, 0 if fail.
typedef int (*OP_ScriptParmFunc)(OP_Node *node, PRM_Template *temp,
				 int idx, fpreal t, void *data);

class OP_API OP_ScriptOperator : public OP_Operator
{
public:
    virtual ~OP_ScriptOperator();

    static OP_ScriptOperator	*createMantraScriptOp(const char *name,
						      const char *english);

    virtual bool		 hasLoadedParmTemplates() const;
    virtual int			 updateParmTemplates();

    // To force our parm templates to be reloaded, we just reset our parm
    // dialog time stamp, and call updateParmTemplates.
    virtual void		 forceUpdateParmTemplates();

    /// Obtains the full name of the script provided by this operator.
    /// It idendifies the script using an operator name, "opdef:"or "op:' path, 
    /// or using the script name (refering to a file on disk).
    void	 getScriptName(UT_String &name,
			       bool forvex, bool for_python = false,
			       const char * section_name = NULL) const;

    /// Obtains the function (shader) name used for script.
    /// It provieds the shader name that should be used in code.
    void	getScriptFunctionName(UT_String &name) const;

    /// Obtains the secondary function (shader) names (as keys) provided by this
    /// operator, along with the section names (as mapped values) where
    /// the function definition is stored.
    void	getScriptSecondaryFunctionsNames( 
			UT_StringMap<UT_StringHolder> &functions ) const;

    /// Tests if the script operator has a script section corresponding
    /// to the given type. Returns true if operator is defined by an HDA
    /// that a script section for the type. If the operator is not defined
    /// by a library or the library definition has no such section, this method
    /// returns false.
    bool	 hasExplicitScriptSection(VEX_ContextType context_type);

    /// Returns true if the operator has cached code.
    bool	 hasCachedCode();


    void	 resetTemplate(OP_TemplatePair *pair);
    // Build a command line for the script to be called with.  This does all
    // the channel evaluation and string expansion required.  If there is an
    // error, 0 is returned (otherwise 1). This function essentially calls
    // OP_Parameters::buildVexCommand, using the right template list, and
    // prefixing the script name.
    int		 buildVexCommand(UT_String &result, OP_Node *node, fpreal now);

    // Build a index entry for our script operator:
    void	 buildIndexEntry(UT_String &result, const UT_String &dsloc);

    // This will find all matching files in the correct Houdini path location
    // and build a list of their contents.
    // *NOTE* The caller will be responsible for cleaning up the
    // 	      definition array elements.
    static int	 findScriptOperators(const char *path,
				     const char *filename,
				     OP_OTLDefinitionArray &defs);
    // Get the table that lists all index files read by parseIndex.
    static UT_StringSet		&getAllIndexFiles();

    // This will traverse the parameters for the script and call traverseFunc
    // for each.  If there is an error, 0 is returned (otherwise 1).
    int          traverseCommand(OP_Node *node, 
	                         OP_ScriptParmFunc traverseFunc, void *data,
	   			 fpreal now);

    // Currently, only VEX is supported
    enum {
	OP_SYNTAX_VEX,		// Vex syntax for building command
	OP_SYNTAX_SHELL,	// Shell syntax for building command
	OP_SYNTAX_RMAN		// Renderman syntax for building command
    };

    // If the grammar of the dialog script needs to be changed, then the
    // following routine can be used to handle tokens.
    // It should return 1 if the token is handled, otherwise 0.
    virtual int		 handleUnknownToken(DS_Stream &is, UT_String &token);

    UT_String		&getScriptHelp() 	{ return myHelp; }
    virtual const char	*getOperatorShortHelpString();
    virtual void	 getOperatorSpecificInfoText(int verbose,
						     UT_WorkBuffer &text);

    /// Fill in this info tree with details about this operator (similar to
    /// getOperatorSpecificInfoText(..)
    virtual void         fillInfoTreeOperatorSpecific(UT_InfoTree &tree,
				const OP_NodeInfoTreeParms &parms);          
    // Edit the script file.  We check the script to see if it has any of the
    // common object code file extensions.  If it does, then we replace it with
    // the extension given.  The common object code extensions are:
    //		.slo, .so, .vex
    // The object code file extensions array must be null terminated.  If the
    // file is not found, the context is used in the funciton declarator.
    void		 editScriptFile(const char *pathprefix,
					const char *context = "surface",
					const char *extension = ".vfl",
					const char *title = "VEX Function",
					const char **obj_extensions = 0,
					int localdir = 1);

    // If this node type uses Python to cook, this method will return the
    // compiled version of that code.  This method caches its results, so
    // subsequent calls to it will avoid reloading and recompiling the
    // code.  This way, the sop can recook without reading from disk.
    // When the node type is changed, the cache is cleared.
    PY_CompiledCode	*getCachedCompiledPythonCookCode();
    bool		 isCompiledPythonCookCodeCached();

    virtual void	 clearOTLIndexFile();

    // Mark the parameter templates as dirty and send out notifications
    // to interested parties that an update is required.
    void		 dirtyParmTemplates();

    fpreal	 getUnitLength() const { return myUnitLength; }
    fpreal	 getUnitMass() const { return myUnitMass; }

protected:
    OP_ScriptOperator(	const char	 *name,
			const char	 *english,
			OP_Constructor    construct,
			PRM_Template	 *templates,
			const char	 *child_table_name,
			unsigned	  min_sources,
			unsigned	  max_sources = 9999,
			CH_LocalVariable *variables = 0,
			unsigned	  flags = 0,
			const char 	**inputlabels = 0,
			int		  maxoutputs = 1);

    /// Obtains a string that specifies the definition source of this operator.
    void		getDefinitionSourceText( UT_String &defsource );

    virtual bool	loadParmTemplatesSubclass();

    // Dummy virtuals that aren't implemented at this level.
    virtual void		 removeAllBindings()
				 { }
    virtual void		 refreshAllBindings()
				 { }
    virtual PRM_Template	*loadParmTemplates(UT_IStream & /*is*/,
					    const char * /* filename */,
					    UT_String & /* scriptName */,
					    PRM_ScriptImports *& /* imports */)
				 { return 0; }

    // Use this method to free parm templates.
    static void			 freeParmTemplates(PRM_Template *templates);

    // Use this method to free switcher defaults.
    static void			 freeSwitcherDefaults(PRM_Default *defs);

    UT_String			 myScriptName;
    UT_String			 myHelp;
    fpreal			 myUnitLength;
    fpreal			 myUnitMass;
    PRM_ScriptPage		*myPage;
    time_t			 myDialogTimeStamp;
    int				 myStartParm, myEndParm;

    // The following information (if set) is used to insert a folder into the
    // specified switcher from myBase.  myBaseSwitcherDefaults is allocated
    // using malloc and must not be destroyed before myParmTemplates.
    int				 myBaseControlsSwitcherIndex;
    int				 myBaseControlsSwitcherPageNum;
    int				 myBaseControlsInsertIndex;
    PRM_Default			*myBaseSwitcherDefaults;

    // Format for generating cmd
    unsigned			 mySyntax;

    // The info text that contains a short help string.
    UT_String			 myInfoText;

    PY_CompiledCode		*myCachedCompiledPythonCookCode;
    UT_Lock			 myCachedCompiledPythonCookLock;

    /// State var indicating if cached code is saved with HDA definition.
    int				 myHasCachedCode;

    // A table containing all the index files that get read by parseIndex.
    static UT_StringSet	 	 theAllIndexFiles;
};

// ============================================================================
/// This is a helper class that parses the extra info from the HDA definition.
/// It can also write te extra info to the HDA definition.
class OP_API OP_ExtraInfoBuffer
{
public:
    /// Constructor and destructor.
		 OP_ExtraInfoBuffer();
		 OP_ExtraInfoBuffer(const char *extra_info);
		~OP_ExtraInfoBuffer();

    /// Parses the extra info string.
    /// After this call, you can use the query methods below.
    void	 readExtraInfo(const char *extra_info);

    /// @{ Writes out the extra info state of this object into a string.
    void	 writeExtraInfo(UT_WorkBuffer &extra_info);
    void	 writeExtraInfo(UT_String &extra_info);
    /// @}

    /// @{ Getters and setters.
    bool			foundKeywords() const 
				{ return myIsKeywordFound; }

    bool			getHideDefaultParms() const
				{ return myHideDefaultParms; }
    void			setHideDefaultParms(bool hide)
				{ myHideDefaultParms = hide; }

    const UT_StringHolder &	getRepresentativeNodePath() const 
				{ return myRepresentativeNodePath; }
    void			setRepresentativeNodePath(const char *path)
				{ myRepresentativeNodePath.harden(path); }

    const UT_StringHolder &	getGuideNodePath() const
				{ return myGuideNodePath; }
    void			setGuideNodePath(const char *path)
				{ myGuideNodePath.harden(path); }

    const UT_StringHolder &	getOpSubType() const
				{ return myOpSubType; }
    void			setOpSubType(const char *type)
				{ myOpSubType.harden(type); }

    const UT_StringHolder &	getShaderType() const
				{ return myShaderType; }
    void			setShaderType(const char *type)
				{ myShaderType.harden(type); }

    const UT_StringHolder &	getRenderMask() const
				{ return myRenderMask; }
    void			setRenderMask(const char *type)
				{ myRenderMask.harden(type); }

    const UT_StringHolder &	getVopnetMask() const
				{ return myVopnetMask; }
    void			setVopnetMask(const char *type)
				{ myVopnetMask.harden(type); }

    const UT_StringHolder &	getPDGType() const
				{ return myPDGType; }
    void			setPDGType(const char *type)
				{ myPDGType.harden(type); }
    /// @}

protected:
    // Resets the data in this class, we do this before we parse a new string.
    void		resetSettings();
     
private:
    bool		myIsKeywordFound;
    bool		myHideDefaultParms;
    UT_StringHolder	myRepresentativeNodePath;
    UT_StringHolder	myGuideNodePath;
    UT_StringHolder	myOpSubType;
    UT_StringHolder	myShaderType;
    UT_StringHolder	myRenderMask;
    UT_StringHolder	myVopnetMask;
    UT_StringHolder	myPDGType;
};

// ============================================================================
/// Helper class for reading and writing FunctionName HDA section.
class OP_API OP_FunctionNameInfo
{
public:
    /// @{ Reads from and writes to a file.
    bool	readFile(const FS_IndexFile &file);
    void	writeFile(FS_IndexFile &file) const;
    /// @}

    /// @{ Reads from and writes to a string buffer.
    void	readBuffer(const char *str);
    void	writeBuffer(UT_WorkBuffer &str) const;
    /// @}

    /// @{ The name of the main (shader) function HDA represents.
    void	setMainFunctionName(const char *name) 
		{ myMainFunctionName = name; }
    const UT_StringHolder &getMainFunctionName() const
		{ return myMainFunctionName; }
    /// @}

    /// @{ The dictionary of additional functions stored in HDA.
    void	setSecondaryFunctionsMap(const UT_StringMap<UT_StringHolder> &m)
		{ mySecondaryFunctionsMap = m; }
    const UT_StringMap<UT_StringHolder> &getSecondaryFunctionsMap() const
		{ return mySecondaryFunctionsMap; }
    /// @}

    /// Utility function to obtain the function code section names referenced
    /// by a function info section.
    static void	getFunctionCodeSections(UT_StringArray &section_names,
			const FS_IndexFile &file);
private:
    void	parseStringJSON(const char *str);
    void	writeStringJSON(UT_WorkBuffer &str) const;

private:
    /// Name of the function or a script represented by the HDA (eg, shader).
    UT_StringHolder	myMainFunctionName;

    /// List of other functions stored by the HDA, and a map to the
    /// HDA section where the function implementation is stored.
    UT_StringMap<UT_StringHolder>   mySecondaryFunctionsMap;
};

#endif
