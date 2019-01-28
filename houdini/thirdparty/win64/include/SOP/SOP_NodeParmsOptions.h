/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_NodeParmsOptions.h ( SOP Library, C++)
 *
 * COMMENTS:
 *	SOP_NodeParms packet that can stores its structure in a UT_Options.
 *	This allows for automatic definition at the expense of performance.
 */

#ifndef __SOP_NodeParmsOptions_h__
#define __SOP_NodeParmsOptions_h__

#include "SOP_API.h"
#include "SOP_Node.h"
#include "SOP_NodeVerb.h"

#include <OP/OP_Options.h>

class SOP_API SOP_NodeParmsOptions : public SOP_NodeParms
{
public:
	     SOP_NodeParmsOptions(const PRM_Template *tmpl);
    virtual ~SOP_NodeParmsOptions();

    virtual void loadFromOpSubclass(const LoadParms &loadparms);

    virtual void	copyFrom(const SOP_NodeParms *src);

    /// Marshalling interface to rest of world
    /// These all use index based notation.
    virtual exint	getNumParms() const;
    virtual const char *getParmName(exint idx) const;
    virtual ParmType	getParmType(exint idx) const;

    virtual void	getParmValue(exint idx, exint &value) const;
    virtual void	getParmValue(exint idx, fpreal &value) const;
    virtual void	getParmValue(exint idx, UT_Vector2D &value) const;
    virtual void	getParmValue(exint idx, UT_Vector3D &value) const;
    virtual void	getParmValue(exint idx, UT_Vector4D &value) const;
    virtual void	getParmValue(exint idx, UT_Matrix2D &value) const;
    virtual void	getParmValue(exint idx, UT_Matrix3D &value) const;
    virtual void	getParmValue(exint idx, UT_Matrix4D &value) const;
    virtual void	getParmValue(exint idx, UT_StringHolder &value) const;
    virtual void	getParmValue(exint idx, UT_SharedPtr<UT_Ramp> &value) const;
    virtual void	getParmValue(exint idx, PRM_DataItemHandle &value) const;

    virtual void	setParmValue(exint idx, const exint &value);
    virtual void	setParmValue(exint idx, const fpreal &value);
    virtual void	setParmValue(exint idx, const UT_Vector2D &value);
    virtual void	setParmValue(exint idx, const UT_Vector3D &value);
    virtual void	setParmValue(exint idx, const UT_Vector4D &value);
    virtual void	setParmValue(exint idx, const UT_Matrix2D &value);
    virtual void	setParmValue(exint idx, const UT_Matrix3D &value);
    virtual void	setParmValue(exint idx, const UT_Matrix4D &value);
    virtual void	setParmValue(exint idx, const UT_StringHolder &value);
    virtual void	setParmValue(exint idx, const UT_SharedPtr<UT_Ramp> &value);
    virtual void	setParmValue(exint idx, const PRM_DataItemHandle &value);

    const UT_OptionEntry *getOptionEntry(const UT_StringRef &name) const
    { return myOptions.getOptionEntry(name); }

    const PRM_Template *getTemplates() const
    { return myTemplates; }

protected:
    OP_Options		myOptions;
    UT_StringArray	myParmNames;
    UT_Array<ParmType>	myParmTypes;
    const PRM_Template	*myTemplates;
    fpreal		myTime;
    friend class SOP_NodeCacheOptions;
};

///
/// Provides a mostly drop-in replacement for SOP_Node, along with a
/// local GU_Detail * to write to.
///
class SOP_API SOP_NodeCacheOptions : public SOP_NodeCache
{
public:
	     SOP_NodeCacheOptions();
    virtual ~SOP_NodeCacheOptions();

    void	doCook(const SOP_NodeVerb *verb, const SOP_NodeVerb::CookParms &cookparms);

    // Wrappers for OP_Parameter() functions.
    // Note time is unused as we've already hardened time in loadFromOp!
    fpreal		evalFloat(const char *parm, int vi, fpreal time) const;
    exint		evalInt(const char *parm, int vi, fpreal time) const;
    void		evalString(UT_String &value, const char *parm, int vi, fpreal time) const;

    fpreal		evalFloatInst(const char *parm, const int *instance, int vi, fpreal time, int nestlevel = 1) const;
    exint		evalIntInst(const char *parm, const int *instance, int vi, fpreal time, int nestlevel = 1) const;
    void		evalStringInst(const char *parm, const int *instance, UT_String &val, int vi, fpreal time, int nestlevel = 1) const;

    exint		nInputs() const
    { return myCookParms->nInputs(); }
    bool		hasInput(exint idx) const
    { return myCookParms->hasInput(idx); }
    const GU_Detail	*inputGeo(exint idx, const OP_Context &context) const
    { return inputGeo(idx); }
    const GU_Detail	*inputGeo(exint idx) const
    { return myCookParms->inputGeo(idx); }

    OP_ERROR		error();
    void		addError(int code, const char *msg = 0)
    { myCookParms->sopAddError(code, msg); }
    void		addWarning(int code, const char *msg = 0)
    { myCookParms->sopAddWarning(code, msg); }
    void		addMessage(int code, const char *msg = 0)
    { myCookParms->sopAddMessage(code, msg); }

    /// Registers an error for cooking the given node's transform. If label is
    /// supplied, then it will be used as a label of the transform error.
    void		 addTransformError(const OP_Node &node,
					   const char *label=0) const
    {
	UT_String	path;

	node.getFullPath(path);
	if (UTisstring(label))
	{
	    path += " (as ";
	    path += label;
	    path += ")";
	}

	myCookParms->addError("OP", OP_ERR_TRANSFORM, path);
    }

    void notifyGroupParmListeners(int groupparm_idx,
			    int grouptype_idx,
			    const GU_Detail *parm_gdp,
			    const GA_Group *parm_group) const
    { myVerb->notifyGroupParmListeners(myCookParms->getNode(),
		    groupparm_idx, grouptype_idx, parm_gdp, parm_group); }

    void		 select(GA_GroupType gtype = GA_GROUP_PRIMITIVE)
    { myCookParms->select(gtype); }
    
    void		 select(GU_SelectionHandle selection, 
        		        bool add_to_sel = false)
    { myCookParms->select(selection, add_to_sel); }
    
			 // Select the group. opt use its type as sel type
    void		 select(const GA_Group &group, bool use_gtype = true,
				bool add_to_sel = false)
    { myCookParms->select(group, use_gtype, add_to_sel); }
    
    void		 select(const GEO_Primitive &prim, bool sel_prim = true,
				bool add_to_sel = false)
    { myCookParms->select(prim, sel_prim, add_to_sel); }
    void		 selectPoint(GA_Offset ptoff, bool point_sel = true,
				bool add_to_sel = false)
    { myCookParms->selectPoint(ptoff, point_sel, add_to_sel); }
    void		 selectFrom(const GEO_Primitive &prim, bool sel_prim = true,
                                    bool add_to_sel = false)
    { myCookParms->selectFrom(prim, sel_prim, add_to_sel); }
    void		 selectPointsFrom(GA_Offset ptoff, bool point_sel = true,
                                    bool add_to_sel = false)
    { myCookParms->selectPointsFrom(ptoff, point_sel, add_to_sel); }
    void		 select(const GA_Range &range, bool use_rtype = true,
				bool add_to_sel = false)
    { myCookParms->select(range, use_rtype, add_to_sel); }

    // Selects input based on the group supplied and the group type.  If
    // group is not null, then it's type is used, otherwise the group type
    // is used.
    void		 selectInputGroup(const GA_Group *group,
					  GA_GroupType grouptype)
    { myCookParms->selectInputGroup(group, grouptype); }

    // If no selection is present, then create a new empty primitive cook   
    // selection group. Otherwise, clear the selection.
    void 		 clearSelection()
    { myCookParms->clearSelection(); }

    /// If the cook selection group is not of the given type, create an empty
    /// cook selection group of that type. Otherwise, clear the existing cook
    /// selection group.
    void		 clearSelection(GA_GroupType gtype)
    { myCookParms->clearSelection(gtype); }
    
    // Blow away the selection and reinitialize it to 0. Much more radical
    // than clearSelection(), which leaves behind a clean, empty selection.
    // This method returns 1 if it had anything to destroy, else 0.
    bool		 destroySelection()
    { 	return myCookParms->destroySelection(); }

    // Return 1 if selection is enabled, 0 if false.
    bool		 selectionEnabled() const
			 { return myCookParms->selectionEnabled(); }

    /// Typedef to help make use of GroupCreator less verbose.
    typedef GOP_Manager::GroupCreator GroupCreator;

    void	 	addExtraInput(OP_Node *op, OP_InterestType type)
    {
	UT_ASSERT(type == OP_INTEREST_DATA);
	if (type == OP_INTEREST_DATA && op && myCookParms->depnode())
	    myCookParms->depnode()->addExplicitInput(op->dataMicroNode());
    }

protected:
    // Note this context has no valid time!
    virtual OP_ERROR	cook(OP_Context &context) = 0;


    /// These are transitory for the duration of the cook.
    GU_Detail		*gdp;

    const SOP_NodeVerb::CookParms	*cookparms() const { return myCookParms; }

private:
    const SOP_NodeParmsOptions	*myOptions;
    const SOP_NodeVerb::CookParms	*myCookParms;
    const SOP_NodeVerb		*myVerb;
};

#endif
