/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_ScriptOperator.h ( OP Library, C++)
 *
 * COMMENTS:	Extends script operators to deal with PI/Selector bindings.
 */

#ifndef __PI_ScriptOperator__
#define __PI_ScriptOperator__

#include "PI_API.h"
#include <OP/OP_OTLDefinition.h>
#include <OP/OP_ScriptOperator.h>
#include <iosfwd>

class PRM_ScriptPage;

class PI_API PI_ScriptOperator : public OP_ScriptOperator
{
public:
    // If specified, the base parameters will be the first parameters in the
    // template list.
    static PI_ScriptOperator	*createScriptOp(const OP_OTLDefinition &def,
						OP_Constructor construct,
						const char	   *child_table_name,
						unsigned flags = 0,
						PRM_Template *base = 0,
						VEX_ContextType vextype =
						    VEX_INVALID_CONTEXT,
						CH_LocalVariable *vars = 0,
						bool is_written_in_python =
						    false);
    static PRM_ScriptPage	*allocateScriptPage(OP_Operator *op);

    virtual ~PI_ScriptOperator();

    // When our parm template changes, refresh all persistent handles to
    // make sure they aren't attached to a non-existant parameter.
    virtual void		 getRefreshPICommands(OP_Node *node,
                		                      std::ostream &commands);

    // This method is used to specify where to add the "Controls" page
    // in myBase during the execution of loadParmTemplates().  The
    // insert_index is used to specify where the script parm templates
    // occur with respect to myBase.  The default argument of -1 means
    // to append the templates after those in myBase.
    void			 setControlsSwitcher(int switcher_index,
						     int page_num,
						     int insert_index = -1);

    void			 setAllCookParms(bool all_cook)
				    { myAllCookParms = all_cook; }
    bool			 isAllCookParms() const
				    { return myAllCookParms; }

    void			 setAllNoResimParms(bool all_sim)
				    { myAllNoResimParms = all_sim; }
    bool			 isAllNoSimParms() const
				    { return myAllNoResimParms; }

    /// Returns a callback that will run the script defined by the 
    /// "script_callback" spare data tag string of a node parameter.
    static PRM_Callback		 getDefaultCallback();
     
protected:
    PI_ScriptOperator(	const char	 *name,
			const char	 *english,
			OP_Constructor    construct,
			PRM_Template	 *templates,
			const char	   *child_table_name,
			unsigned	  min_sources,
			unsigned	  max_sources = 9999,
			CH_LocalVariable *variables = 0,
			unsigned	  flags = 0,
			const char 	**inputlabels = 0,
			int	 	  maxoutputs = 1);

    virtual void		 removeAllBindings();
    virtual void		 refreshAllBindings();
    virtual PRM_Template	*loadParmTemplates(UT_IStream &is,
					    const char *filename,
					    UT_String &scriptName,
					    PRM_ScriptImports *&imports);

    virtual const char	*getAddedDSIncludePath();

private:
    PRM_Template	*myScriptObsoleteTemplates;
    bool		 myAllCookParms;
    bool		 myAllNoResimParms;
};

#endif
