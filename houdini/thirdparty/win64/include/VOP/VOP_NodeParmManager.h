/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_NodeParmManager.h ( VOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VOP_NodeParmManager__
#define __VOP_NodeParmManager__

#include "VOP_API.h"
#include "VOP_Types.h"

class VOP_Language;
class VOP_TypeInfo;
class PRM_ChoiceList;
class PRM_Template;
class PRM_Type;
class UT_String;


/// Class for providing information about node parameters that correspond
/// to VOP data types. This manager maps VOP_Type to an appropriate PRM
/// parameter on a node, essentially describing the parameter UI.
class VOP_API VOP_NodeParmManager
{
public:
    /// Constructor.
    VOP_NodeParmManager( VOP_Language *language );

    /// Returns the number of parameters and parameter types known to manager.
    int		getNumParmTypes() const;

    /// @{ Returns the basic info about the i-th parameter.
    VOP_Type	getParmType(int idx) const;
    VOP_Type	getUnconditionedParmType(int idx) const;
    const char *getParmName(int idx) const;
    /// @}

    /// @{ Find parameter index based on VOP data type.
    int		getParmIndex(VOP_Type type) const;
    int		findUnconditionedParmIndex(VOP_Type type)const;
    int		guessParmIndex(VOP_Type voptype, PRM_Type parmtype, 
			int size) const;
    /// @}

    /// Returns the approximate number of fields in a type. For example, 
    /// for float returns 1, for color 3. For arrays and strings returns 1.
    int		getTypeSize(VOP_Type voptype) const;

    /// Returns true if parameter allows a drop down menu.
    bool	allowsMenu(int idx) const;

    /// @{ Methods for obtaining PRM information for a i-th parameter.
    void	fillSwitcherTemplate(PRM_Template *tplate, 
			bool with_ranges) const;
    void	fillParmTemplate(int idx, PRM_Template *tplate) const;
    int		fillDialogScript(int idx, 
			UT_String &ds_type, int &ds_size) const;
    int		fillVexPragma(int idx, UT_String &vex_pragma) const;
    bool	fillParmOpFilter(int idx, 
			UT_String &op_filter, UT_String &op_relative) const;
    /// @}

    /// @{ Checks the data type of i-th parameter
    bool	isSingleFloatParmIndex(int idx) const;
    bool	isSingleIntParmIndex(int idx) const;
    bool	isPossibleOpListIndex(int idx) const;
    /// @}

    /// @{ Obtains PRM menu with parameter types.
    PRM_ChoiceList	*getParmTypeMenu() const;
    PRM_ChoiceList	*getParmTypeNameMenu() const;
    /// @}


    /// @{ Tests uniform-varying type.
    bool	supportsUniformVarying() const;
    bool	isParameterUniformVarying(int idx) const;
    /// @}


    /// @{ Returns the node operator name for parameters and constants.
    const char	*getParameterOpName(const VOP_TypeInfo &t)const;
    const char	*getConstantOpName() const;
    /// @}

private:
    /// Needed for type conditioning.
    VOP_Language *	myLanguage;
};

#endif

