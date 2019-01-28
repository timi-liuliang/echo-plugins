/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:	The base class for all SOP operators
 *		Every SOP is responsible for filling in the following methods:
 *			cookMe(float t);
 *
 */

#ifndef __SOP_Node_h__
#define __SOP_Node_h__

#include "SOP_API.h"

#include "SOP_Error.h"
#include "SOP_Guide.h"
#include "SOP_NodeFlags.h"
#include "SOP_ObjectAppearanceFwd.h"

#include <GOP/GOP_Manager.h>
#include <GU/GU_Detail.h>
#include <GU/GU_DetailHandle.h>
#include <GU/GU_DetailGroupPair.h>
#include <GU/GU_SelectType.h>
#include <GU/GU_SelectionSet.h>
#include <GEO/GEO_Detail.h>
#include <GA/GA_AttributeRef.h>
#include <GA/GA_Types.h>
#include <CH/CH_LocalVariable.h>
#include <OP/OP_InputChangeHelper.h>
#include <OP/OP_Network.h>
#include <PRM/PRM_Shared.h>
#include <UT/UT_Lock.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <SYS/SYS_Deprecated.h>

class UT_InfoTree;
class UT_Undo;
class GU_Selection;
class OP_OperatorTable;
class SOP_Node;
class SOP_SoftLockData;
class DD_Source;

class sop_AttributeEvaluator;
class SOP_NodeVerb;
class SOP_NodeParms;
class SOP_NodeCache;

enum SOP_UIChangeType
{
    SOP_UICHANGE_CACHE_UNLOAD_STATE = OP_UICHANGE_OPTYPE_SOP
};

extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newSopOperator(OP_OperatorTable *table);
};

class sopVarInfo;

class SOP_API sop_attribRef
{
public:
    UT_String	myName;
    int		mySize;
    GA_Storage	myStorage;
};

// A call back operator for local variables:
typedef fpreal (SOP_Node::*SOP_LocalVarFunc)
		(sopVarInfo *varinfo);

class SOP_API sopVarInfo
{
public:
    CH_LocalVariable	myVar; // Actual variable name.
    int			myTable; // Which table to use
    UT_String		myAttribName; // Name of the attribute
    GA_ROAttributeRef	myOffset; // Offset in attrib
    sopVarInfo		*mySecondaryVariable;	// For LIFE that needs two.
    int			myTupleIndex; // Component in tuple
    fpreal		myDefault; // Default value if not present
    GA_StorageClass	myStorageClass; // Float, Int, or String
    GA_AttributeOwner	myDictionary; // Vertex, Point, Primitive, or Detail
    int			myVaridx; // Call back parameter.
    SOP_LocalVarFunc	myCallback; // Call back function
};

// Handy function to build a meta kernel menu. Used in several sops.
SOP_API extern void	SOPfillMetaKernelMenu(
				void *, PRM_Name *names, int size,
				const PRM_SpareData *, const PRM_Parm *);

#define SOP_LOCALVAR_START	1000000	// Start of local variable indices

//___________________________________________________________________________

typedef void sopVisualizeCallback(void * /*data*/, SOP_Node *, OP_Context &, GU_Detail *);

class SOP_API SOP_Node : public OP_Network
{
public:
    virtual const char          *getChildType() const override;
    virtual const char		*getOpType() const override;

    static const char	*theChildTableName;
    virtual OP_OpTypeId  getChildTypeID() const override;
    virtual OP_OpTypeId  getOpTypeID() const override;

    const SOP_NodeFlags		&getFlags() const { return mySopFlags; }

    // Whether a SOP is cooking render is whether its object creator is.
    virtual int			 isCookingRender() const override
				 { return getCreator()->isCookingRender(); }

    // This allows us to tap into flag changes and perform unloads
    // if necessary.
    virtual void		 opChanged(OP_EventType reason, 
                		           void *data=0) override;

    // SOP networks need some logic to switch between using
    // output nodes, display nodes, or render nodes. This function can
    // also be used to just return the Output SOPs by passing false to
    // "fallback_to_display_render".
    SOP_Node			*getOutputSop(int outputidx,
					bool fallback_to_display_render = true);

    //
    // Local variable functions:
    //
    
    // Used to accumulate local variables:
    virtual const CH_LocalVariable *resolveVariable(
	    const char *name) override;
    virtual const CH_LocalVariable *resolveExtraVariableForSyntaxHighlight(
	    const char *name) override;
    
    // Each one gets a precedence from 0-3.  Resolution within
    // a precedence is undefined.
    // 0 is highest precedence.
    // This is used for generic variables like Cd 
    void			 setVariableOrder(int detail, int prim, 
						  int pt, int vtx);
    // Gets the index of specified level.  Index is same as the parameter
    // order in setVariableOrder
    int				 getVariableOrder(int level) const;
    // Given a variable idx, gives the precedence.
    int				 getVariablePrecedence(int var) const;

    // Wipe the table of local variables
    // This is automatically called by the setup function so
    // shouldn't be called outside it.

    virtual bool		 usesFootprint() const override 
				 { return true; }

    void			 setVisualizeCallback(sopVisualizeCallback *callback, void *data);

    /// Moves visualizer into our output list
    virtual bool		 addOrMoveVisualizerToOutput(int outputidx) override;

private:
    void			 clearLocalVars();
    void			 updateLocalVarOffsets();

    // invalidate any cached data
    virtual void		 clearCache() override;

    // This resets the attribute caches for @ style local variables.
    // It is implied by setupLocalVars, resetLocalVarRefs,
    // and clearLocalVars.
    void			 clearAttributeEvalCaches();
public:

    // the flag indicating the capture override state (on/off)
    // This override is mirroring the kinematic override at the OBJ level.
    // It is used to determine whether capture related SOPs use the
    // capture parameter or regular animation parameters for cooking
    static void			 setCaptureOverrideFlag( bool flag )
					{
					    theGlobalCaptOverride = flag;
					}
    static bool			 getCaptureOverrideFlag() 
					{
					    return theGlobalCaptOverride; 
					}

    
    // Build the local variable table
    // This should be done AFTER you have set the local variable
    // order & AFTER you have initialized your myGdp[*].
    bool			 setupLocalVars();

    // This resets all the myCur*s to zero so we can be sure they
    // are all cleared out.  It does NOT clear the bounding box
    // build states.
    void			 resetLocalVarRefs();

    void			 setCurGdh(int index,
					   const GU_DetailHandle &gdh);
    void			 clearCurGdh(int index);
    const GU_DetailHandle	&getCurGdh(int index);

    void			 setCurPoint(int index,
					   GA_Offset ptoff);
    void			 clearCurPoint(int index);
    void			 setCurVertex(int index,
					   GA_Offset ptoff);
    void			 clearCurVertex(int index);
    void			 setCurPrim(int index,
					   GA_Offset primoff);
    void			 clearCurPrim(int index);
    void			 setCurVertexNum(int index,
					   exint vtxnum);
    void			 clearCurVertexNum(int index);

    // Called by evalVariableValue() to resolve local variables.
    // Note that this will not resolve global variables like $F!
    bool			 getVariableValueFP(fpreal &val, int index);

    // This gets the variable value as an fpreal value.
    // Note that you should call the base class if you don't handle the given
    // index.
    virtual bool		 evalVariableValue(fpreal &val, int index,
						  int thread) override;

    // This gets the variable value as a string.  Not the string
    // corresponding to the specified index.
    // Note that you should call the base class if you don't handle the given
    // index.
    virtual bool		 evalVariableValue(UT_String &value, int index,
						  int thread) override;

    /// A simple wrapper class to help in the semantic transition of specific
    /// addGenericVariable() and addSpecificVariable() overloads that now use
    /// higher level tuple index arguments instead of lower level byte offset
    /// arguments.
    class SOP_API AttribTupleIndexArg
    {
	public:
	    explicit AttribTupleIndexArg(int index) : myIndex(index) {}
	    operator int() const { return myIndex; }
	private:
	    int		 myIndex;

    };

    // Tries to add the variable to each of our gdps, using the appropriate
    // "" or "2" suffix.
    void			 addGenericVariable(const char *varname,
					GEO_Standard_Attributes attr,
					const AttribTupleIndexArg &tuple_idx,
					GA_StorageClass storage = GA_STORECLASS_REAL,
					GA_Defaults const& defaults = GA_Defaults(0.0f));
    void			 addGenericVariable(const char *varname, 
					const char *attribname,
					const AttribTupleIndexArg &tuple_idx,
					GA_StorageClass storage = GA_STORECLASS_REAL,
					GA_Defaults const& defaults = GA_Defaults(0.0f));
    void			 addGenericVariable(const char *varname,
					int varidx,
					SOP_LocalVarFunc callback,
					GA_Defaults const& defaults = GA_Defaults(0.0f));
    // Add variables to our look up table, provided they exist in the GDP
    // The addGeneric adds all the prefixed versions, then finds the first
    // one which is valid and sets itself to that.
    void			 addGenericVariable(const GU_Detail *gdp, 
					int gdpidx,
					const char *varname, 
					const char *attribname,
					const AttribTupleIndexArg &tuple_idx,
					GA_StorageClass storage = GA_STORECLASS_REAL,
					GA_Defaults const& defaults = GA_Defaults(0.0f));
    void			 addGenericVariable(int gpdidx,
					const char *varname,
					int varidx,
					SOP_LocalVarFunc callback,
					GA_Defaults const& defaults = GA_Defaults(0.0f));
    void			 addSpecificVariable(const GU_Detail *gdp, 
					int gdpidx,
					const char *varname,
					const char *attribname,
					const AttribTupleIndexArg &tuple_idx,
					GA_StorageClass storage,
					GA_AttributeOwner dictionary,
					GA_Defaults const& defaults = GA_Defaults(0.0f));
    void			 addSpecificVariable(int gpidx,
					const char *varname,
					int varidx,
					GA_AttributeOwner dictionary,
					SOP_LocalVarFunc callback,
					GA_Defaults const& defaults = GA_Defaults(0.0f));

    static int			 addCustomVariableCallback(const char *attr,
					const char *varname, void *data);
    void			 addCustomVariable(const char *attr,
					const char *varname);

    void			 cacheVarPtBox(int gpdidx);
    void			 cacheVarPrimBox(int gpdidx);

    //
    // These are the call back functions to evaluate various variable
    // types.
    //
    fpreal		 varCallbackT(sopVarInfo *varinfo);

    fpreal		 varCallbackPT(sopVarInfo *varinfo);
    fpreal		 varCallbackPR(sopVarInfo *varinfo);
    fpreal		 varCallbackVTX(sopVarInfo *varinfo);

    fpreal		 varCallbackNPT(sopVarInfo *varinfo);
    fpreal		 varCallbackNPR(sopVarInfo *varinfo);
    fpreal		 varCallbackNVTX(sopVarInfo *varinfo);

    fpreal		 varCallbackCE(sopVarInfo *varinfo);
    fpreal		 varCallbackMIN(sopVarInfo *varinfo);
    fpreal		 varCallbackMAX(sopVarInfo *varinfo);
    fpreal		 varCallbackSIZE(sopVarInfo *varinfo);
    fpreal		 varCallbackLIFE(sopVarInfo *varinfo);
    fpreal		 varCallbackAGE(sopVarInfo *varinfo);
    fpreal		 varCallbackBB(sopVarInfo *varinfo);
    fpreal		 varCallbackWEIGHT(sopVarInfo *varinfo);

    // These methods return the variables being iterated over.  The index
    // must be 0 or 1.
    const GU_DetailHandle &curGdh(int index) const
    { UT_ASSERT(index == 0 || index == 1); return myCurGdh[index]; }

    // Note that the current vertex num will be 0 even if there is no
    // current vertex.  Use curVertex() to check if there is a current
    // vertex.
    int			 curVertexNum(int index) const
    { UT_ASSERT(index == 0 || index == 1); return myCurVtxNum[index]; }

    const GU_Detail	*curGdp(int index) const
    { UT_ASSERT(index == 0 || index == 1); return myCurGdp[index]; }

    GA_Offset		 curVertex(int local_var_gdp_index) const
    { UT_ASSERT(local_var_gdp_index == 0 || local_var_gdp_index == 1);
      if (!myCurGdp[local_var_gdp_index]) return GA_INVALID_OFFSET;
      return myCurVtxOff[local_var_gdp_index]; }

    GA_Offset curPoint(int index) const
    { UT_ASSERT(index == 0 || index == 1); if (!myCurGdp[index]) return GA_INVALID_OFFSET; return myCurPtOff[index]; }

    const GEO_Primitive *curPrim(int index) const
    { UT_ASSERT(index == 0 || index == 1); if (!myCurGdp[index]) return 0; return myCurGdp[index]->getGEOPrimitive(myCurPrimOff[index]); }

    // The point bounding box is used from $BBX, $BBY, and $BBZ.
    UT_BoundingBox	 curPointBBox(int index)
    {
        UT_ASSERT(index == 0 || index == 1);
        cacheVarPtBox(index);
        return myCurPtBox[index];
    }

    // The primitive bounding box is used from $MINX, $MINY, and $MINZ.
    UT_BoundingBox	 curPrimBBox(int index)
    {
        UT_ASSERT(index == 0 || index == 1);
        cacheVarPrimBox(index);
        return myCurPrimBox[index];
    }


    // Function to set the pivot parm to the centroid of the input group
    void		 setPivotParmToGroupCentroid(OP_Context &context);

private:
    // Helper function for building the drop down group menus.
    void		getGroupMenuHeader(int sourcenum, const char * title,
					    UT_String & header);

    static int		buildSopGroups(const GU_Detail *src,
				const char *prim_header,
				const char *pt_header,
				const char *edge_header,
                                const char *vertex_header,
				PRM_Name *menuEntries, int themenusize,
				unsigned type, int start,
				bool includeselection, const PRM_Parm *parm);

    static int		buildSopEdgeGroups(const GU_Detail *src,
				const char * edge_header,
				const char *prim_header, const char *pt_header,
                                const char *vertex_header,
				PRM_Name *menuEntries, int themenusize, 
				bool addprim, int start,
				const PRM_Parm *parm);

    static int		buildSopBreakpointGroups(const GU_Detail *src,
				const char * brkpt_header,
				const char * prim_header, const char *pt_header,
				const char * edge_header,
                                const char *vertex_header,
				PRM_Name *choicenames, int menusize, 
				bool addprim, int start,
				const PRM_Parm *parm);

public:
    //  The following method has to be public for some static functions in
    //	the code
    static void		 buildGroupsFromSop( SOP_Node * src_sop, 
				    const PRM_Parm *parm,
				    const PRM_SpareData *spare, 
				    unsigned type, int start,
				    PRM_Name *menuEntries, int thelistsize );

    static int		 buildEdgeGroupsFromSop( SOP_Node * src_sop, 
				    const PRM_Parm *parm,
				    const PRM_SpareData *spare, 
				    bool addprim, int start,
				    PRM_Name *menuEntries, int thelistsize );

    static int		 buildBreakpointGroupsFromSop( SOP_Node * src_sop, 
				    const PRM_Parm *parm,
				    const PRM_SpareData *spare, 
				    bool addprim, int start,
				    PRM_Name *menuEntries, int thelistsize );

    void		 buildNamedPrims(int thesourcenum,
					PRM_Name *thechoicenames,
					int themenusize,
					const PRM_Parm *parm,
					bool globvectors,
					bool numberedvolumes);

    void		 buildInputGroups(int thesourcenum,
					PRM_Name *thechoicenames,
					int themenusize,
					unsigned type, int start,
					bool includeselection,
					const PRM_Parm *parm);

    int 		 buildInputEdgeGroups(int sourcenum,
	                                PRM_Name *choicenames, int menusize,
					bool addprim, int start,
					const PRM_Parm *parm);

    int 		 buildInputBreakpointGroups(int sourcenum, 
					PRM_Name *choicenames, int menusize,
					bool addprim, int start,
					const PRM_Parm *parm);

    virtual int		 setModelLock(int lock_type, 
				      void *modeler = 0,
				      int allow_softlock_promotion=0,
				      UT_Undo *undo = 0) override;

    //  Nobody should have to override this, but it's public for other people
    virtual OP_DataType	 getCookedDataType() const override;

    virtual void	*getCookedData(OP_Context &ctx) override;

    virtual bool	 getBoundingBox(UT_BoundingBox &box, 
                	                OP_Context &ctx) override;

    // Get cooked geometry. If "forced" is true and cooking fails, we create
    // an empty gdp and clear the cooking flags.
    // Note that the resulting handle may be reused in future cooks!
    // To prevent the handle from being reused, call addPreserveRequest()
    // on the handle.
    const GU_Detail	*getCookedGeo(OP_Context &, int forced = 0);
    GU_DetailHandle	 getCookedGeoHandle(OP_Context &, int forced = 0);
    GU_DetailHandle 	 cookOutput(OP_Context &context, int outputidx, SOP_Node *interests);
    const SOP_Guide	*getCookedGuide1() const;
    const SOP_Guide	*getCookedGuide2() const;

    // This is similar to the getCookedGeo except it merely returns the
    // cached GEO, if any.  If none, it returns NULL without trying to cook.
    const GU_Detail	*getLastGeo();

    // Returns the equivalent of getLastGeo of the given input.  This lets
    // you get an input geometry without locking the input and without
    // risking cooking it.  It can, and will, return NULL without trying
    // to cook in many cases.
    const GU_Detail	*getInputLastGeo(int input, fpreal time);

    OP_ERROR	 	 cookGuide1(OP_Context &context);
    OP_ERROR	 	 cookGuide2(OP_Context &context);

    OP_ERROR		 cookGuideList(OP_Context &context,
				UT_Array<GU_DetailHandle> &list);

    // The guide list can consist of either GOP_Guide derived objects or simple
    // GU_Detail objects (but not a combination).  This method is used to query
    // which of the two, if any, the node generates.
    virtual GOP_GuideListType	getGuideListType() const
				    { return OP_GUIDELIST_GUDETAIL; }

    virtual OP_ERROR	 lockInput(unsigned idx, 
                    	           OP_Context &context) override;
    void		 unlockInput(unsigned idx) override;
    virtual OP_ERROR	 lockInputs(OP_Context &context) override;
    void		 unlockInputs() override;

    static void		 buildOperatorTable(OP_OperatorTable &table);
    static void		 initializeExpressions();

    // Change the string into a valid group name.  A return value of true
    // indicates that the string needed changing, in which case severity
    // specifies the level of error this method generated (UT_ERROR_NONE,
    // UT_ERROR_WARNING, or UT_ERROR_ABORT).
    // Note that this does not force the name to be non-zero in length.
    bool		 forceValidGroupName(UT_String &name,
					     UT_ErrorSeverity severity);
    static bool		 forceValidGroupName(UT_String &name,
					     UT_ErrorManager *error,
					     UT_ErrorSeverity severity);
    // Change the string into a valid group prefix.  A return value of true
    // indicates that the string needed changing, in which case severity
    // specifies the level of error this method generated (UT_ERROR_NONE,
    // UT_ERROR_WARNING, or UT_ERROR_ABORT).
    // Note that this does not force the prefix to be non-zero in length.
    bool		 forceValidGroupPrefix(UT_String &prefix,
					       UT_ErrorSeverity severity);

    void 		 expandGroupMask(const char *pattern,
					 UT_String &outNames,
					 GA_GroupType type,
					 const GU_Detail *pgdp);
    void 		 expandGroupMask(const char *pattern,
					 UT_String &outNames,
					 GA_GroupType type)
			 { expandGroupMask(pattern, outNames, type, gdp); }

    /// Typedef to help make use of GroupCreator less verbose.
    typedef GOP_Manager::GroupCreator GroupCreator;

    const GA_PrimitiveGroup	*parsePrimitiveGroups(const char *pattern,
						      const GroupCreator &creator,
						      bool allow_numeric = true,
						      bool ordered = false,
						      bool strict = false,
						      GA_Index offset = GA_Index(0));
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo)
    const GA_PrimitiveGroup	*parsePrimitiveGroups(const char *pattern,
						      GU_Detail *pgdp,
						      int allow_numeric=1,
						      int ordered = 1,
						      bool strict = false,
						      int offset = 0);
    const GA_PrimitiveGroup	*parsePrimitiveGroups(const char *pattern)
				 { return parsePrimitiveGroups(pattern, GroupCreator(gdp, false), true, true); }

    GA_PrimitiveGroup		*parsePrimitiveGroupsCopy(const char *pattern,
							  const GroupCreator &creator,
							  bool allow_numeric = true,
							  bool ordered = false,
							  bool strict = false);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo)
    GA_PrimitiveGroup		*parsePrimitiveGroupsCopy(const char *pattern,
							  GU_Detail *pgdp,
							  int allow_numeric=1,
							  int ordered = 1,
							  bool strict = false);
    GA_PrimitiveGroup		*parsePrimitiveGroupsCopy(const char *pattern)
				 { return parsePrimitiveGroupsCopy(pattern, GroupCreator(gdp, false), true, true); }

    const GA_PointGroup		*parsePointGroups(const char *pattern,
						  const GroupCreator &creator,
						  bool allow_numeric = true,
						  bool ordered = false,
						  bool strict = false,
						  GA_Index offset = GA_Index(0));
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo)
    const GA_PointGroup		*parsePointGroups(const char *pattern,
						  GU_Detail *pgdp,
						  int allow_numeric=1,
						  int ordered = 1,
						  bool strict = false,
						  int offset = 0);
    const GA_PointGroup		*parsePointGroups(const char *pattern)
				 { return parsePointGroups(pattern, GroupCreator(gdp, false), true, true); }

    GA_PointGroup		*parsePointGroupsCopy(const char *pattern,
						  const GroupCreator &creator,
						  bool allow_numeric = true,
						  bool ordered = false,
						  bool strict = false);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo)
    GA_PointGroup		*parsePointGroupsCopy(const char *pattern,
						  GU_Detail *pgdp,
						  int allow_numeric=1,
						  int ordered = 1,
						  bool strict = false);
    GA_PointGroup		*parsePointGroupsCopy(const char *pattern)
				 { return parsePointGroupsCopy(pattern, GroupCreator(gdp, false), true, true); }

    const GA_EdgeGroup		*parseEdgeGroups(const char *pattern,
						 const GU_Detail *pgdp,
						 bool strict = true,
						 GA_Index prim_offset = GA_Index(0),
						 GA_Index point_offset = GA_Index(0));
    const GA_EdgeGroup		*parseEdgeGroups(const char *pattern)
				 { return parseEdgeGroups(pattern, (const GU_Detail *)gdp); }

    GA_EdgeGroup		*parseEdgeGroupsCopy(const char *pattern,
						     const GU_Detail *pgdp);
    GA_EdgeGroup		*parseEdgeGroupsCopy(const char *pattern)
				 { return parseEdgeGroupsCopy(pattern, (const GU_Detail *)gdp); }

    const GA_BreakpointGroup	*parseBreakpointGroups(const char *pattern,
						       const GU_Detail *pgdp,
						       bool strict = true,
						       GA_Index offset = GA_Index(0));
    const GA_BreakpointGroup	*parseBreakpointGroups(const char *pattern)
				 { return parseBreakpointGroups(pattern, (const GU_Detail *)gdp); }

    GA_BreakpointGroup		*parseBreakpointGroupsCopy(const char *pattern,
							   const GU_Detail *pgdp);
    GA_BreakpointGroup		*parseBreakpointGroupsCopy(const char *pattern)
				{return parseBreakpointGroupsCopy(pattern,(const GU_Detail *)gdp);}

    const GA_VertexGroup	*parseVertexGroups(const char *pattern,
						       const GU_Detail *pgdp,
						       bool strict = true,
						       GA_Index offset = GA_Index(0));
    const GA_VertexGroup	*parseVertexGroups(const char *pattern)
				 { return parseVertexGroups(pattern, (const GU_Detail *)gdp); }

    GA_VertexGroup		*parseVertexGroupsCopy(const char *pattern,
							   const GU_Detail *pgdp);
    GA_VertexGroup		*parseVertexGroupsCopy(const char *pattern)
				 { return parseVertexGroupsCopy(pattern, (const GU_Detail *)gdp); }

    const GA_Group		*parseAllGroups(const char *pattern,
				    const GroupCreator &creator,
				    bool allow_numeric = true,
				    bool ordered = false,
				    GA_GroupMaskType mask=GA_GMASK_FULL_MASK,
                                    GA_GroupType bestguess=GA_GROUP_PRIMITIVE);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo)
    const GA_Group		*parseAllGroups(const char *pattern,
				    GU_Detail *pgdp,
				    int allow_numeric=1,
				    int ordered=1,
				    GA_GroupMaskType mask=GA_GMASK_FULL_MASK);
    const GA_Group		*parseAllGroups(const char *pattern)
				 { return parseAllGroups(pattern, GroupCreator(gdp, false), true, true); }

    GA_Group		*parseAllGroupsCopy(const char *pattern,
				    const GroupCreator &creator,
				    bool allow_numeric = true,
				    bool ordered = false,
				    GA_GroupMaskType mask=GA_GMASK_FULL_MASK,
                                    GA_GroupType bestguess=GA_GROUP_PRIMITIVE);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo)
    GA_Group		*parseAllGroupsCopy(const char *pattern,
				    GU_Detail *pgdp,
				    int allow_numeric=1,
				    int ordered=1,
				    GA_GroupMaskType mask=GA_GMASK_FULL_MASK);
    GA_Group		*parseAllGroupsCopy(const char *pattern)
				 { return parseAllGroupsCopy(pattern, GroupCreator(gdp, false), true, true); }

    // Explicitly destroy an adhoc group. Usually this is done on your behalf
    // internally, after the SOP cooks, but this method allows you to delete
    // an adhoc group within the cooking methods. Return is 0 if not found.
    // An adhoc group is created when the SOP groups are parsed, or when you
    // call createAdhocPrimGroup() or createAdhocPointGroup().

    int 		 destroyAdhocGroup (const GA_Group *group)
			 { return myGroupParse.destroyAdhocGroup(group); }
    void		 destroyAdhocGroups()
			 { myGroupParse.destroyAdhocGroups(); }
    void		 destroyAdhocGroups(const GU_Detail *g)
			 { myGroupParse.destroyAdhocGroups(g); }

    // Set the selection based on the given geometry:
    
    /// Update the selection object on the detail to match the one for this
    /// SOP node.
    SYS_DEPRECATED(16.0)
    void updateUserSelectionOnDetail();
    
    /// Set a specific override selection for this node only. Automatically 
    /// updates the selection object on the detail.
    SYS_DEPRECATED(16.0)
    void setNodeSelectionSet(GU_SelectionSetHandle selection_set);
    
    SYS_DEPRECATED(16.0)
    GU_SelectionSetHandle nodeSelectionSet() const;
    
    /// Clears the override selection. Automatically updates the selection 
    /// object on the detail.
    SYS_DEPRECATED(16.0)
    void clearNodeSelectionSet();
    
    /// Copies the cooked selection generated by the SOP to the detail-external
    /// user selection. If \c context is NULL, then the last successful cook,
    /// if any, is used.
    SYS_DEPRECATED(16.0)
    void copyCookedSelectionToUserSelection(OP_Context *context = 0);
    
public:
    
    SYS_DEPRECATED_PUSH_DISABLE()
    SYS_DEPRECATED_REPLACE(14.0, "SOP_Node::select(GA_GroupType gtype)")
    void		 select(GU_SelectionType stype);
    SYS_DEPRECATED_POP_DISABLE()
    
    void		 select(GA_GroupType gtype = GA_GROUP_PRIMITIVE);
    
    void		 select(GU_SelectionHandle selection, 
        		        bool add_to_sel = false);
    
			 // Select the group. opt use its type as sel type
    void		 select(const GA_Group &group, bool use_gtype = true,
				bool add_to_sel = false);
    
    void		 select(const GEO_Primitive &prim, bool sel_prim = true,
				bool add_to_sel = false);
    void                 selectPrimitive(GA_Offset primoff,
                                bool prim_sel = true,
                                bool add_to_sel = false);
    void		 selectPoint(GA_Offset ptoff, bool point_sel = true,
				bool add_to_sel = false);
    void		 selectFrom(const GEO_Primitive &prim, bool sel_prim = true,
                                    bool add_to_sel = false);
    void		 selectPointsFrom(GA_Offset ptoff, bool point_sel = true,
                                    bool add_to_sel = false);
    void		 select(const GA_Range &range, bool use_rtype = true,
				bool add_to_sel = false);

    // Selects input based on the group supplied and the group type.  If
    // group is not null, then it's type is used, otherwise the group type
    // is used.
    void		 selectInputGroup(const GA_Group *group,
					  GA_GroupType grouptype);
    // FIXME: Delete this function once it's swept out, because
    //        the int corresponds with PRM_GroupType instead of
    //        GA_GroupType.
    void		 selectInputGroup(const GA_Group *group,
					  int grouptype);

    // If no selection is present, then create a new empty primitive cook   
    // selection group. Otherwise, clear the selection.
    void 		 clearSelection();

    SYS_DEPRECATED_PUSH_DISABLE()
    SYS_DEPRECATED_REPLACE(14.0, "SOP_Node::clearSelection(GA_GroupType gtype)")
    void 		 clearSelection(GU_SelectionType stype);
    SYS_DEPRECATED_POP_DISABLE()

    /// If the cook selection group is not of the given type, create an empty
    /// cook selection group of that type. Otherwise, clear the existing cook
    /// selection group.
    void		 clearSelection(GA_GroupType gtype);
    
    // Blow away the selection and reinitialize it to 0. Much more radical
    // than clearSelection(), which leaves behind a clean, empty selection.
    // This method returns 1 if it had anything to destroy, else 0.
    bool		 destroySelection();

    // Return 1 if selection is enabled, 0 if false.
    bool		 selectionEnabled() const
			 {
			     return getHighlight();
			 }

    bool		 hasCookedSelection() const
			 {
			     return myHasCookedSelection;
			 }

    // Copy all the temp selections from the given SOP into this one. All
    // existing selections on this SOP will be destroyed first. If there are
    // no temp selections, then the cook selection is copied into a temp
    // selection.
    bool		 copyTempSelections(OP_Context &context, SOP_Node *src);

    /// Functions to convert between different group type representations.
    /// When no group type can be guessed, GA_GROUP_INVALID,
    /// PRM_GROUPTYPE_GUESS, PRM_UVGROUPTYPE_GUESS, or GA_ATTRIB_INVALID
    /// Will be returned.
    /// @{
    static GA_GroupType getGAGroupType(PRM_GroupType prmgrouptype, GA_GroupType guess = GA_GROUP_INVALID);
    static GA_GroupType getGAGroupType(PRM_UVGroupType prmuvgrouptype, GA_GroupType guess = GA_GROUP_INVALID);
    SYS_DEPRECATED_PUSH_DISABLE()
    static GA_GroupType getGAGroupType(GU_SelectionType guseltype);
    SYS_DEPRECATED_POP_DISABLE()
    static GA_GroupType getGAGroupType(GA_AttributeOwner owner);
    static GA_GroupType getGAGroupType(const char *possiblename, GA_GroupType guess = GA_GROUP_INVALID);
    static PRM_GroupType getPRMGroupType(GA_GroupType gagrouptype);
    static PRM_UVGroupType getPRMUVGroupType(GA_GroupType gagrouptype);
    SYS_DEPRECATED_PUSH_DISABLE()
    static GU_SelectionType getGUSelType(GA_GroupType gagrouptype, GU_SelectionType guess = GU_SPrimitive);
    SYS_DEPRECATED_POP_DISABLE()
    static GA_AttributeOwner getGroupAttribOwner(GA_GroupType gagrouptype, GA_AttributeOwner guess = GA_ATTRIB_INVALID);
    static const char *getGroupTypeName(GA_GroupType gagrouptype, const char *guess = "guess");
    /// @}


    virtual void	 resetSimulation () {}

    virtual int		 getNetOverviewColor( UT_Color &color ) override;

    void		 addInstanceReference(SOP_Node *node);
    void		 clearInstanceReference(SOP_Node *node);

    /// This makes *this an instance of node.  It must be called within
    /// a cookMySop.  myGdpHandle is assumed to have an active write lock.
    /// this->gdp is updated to point to the new gdp.  It is assumed
    /// that clearInstance() has already been called.
    /// The node that you pass in must be cooked prior to instancing!
    /// This has occured for free if it was lockInputed.  Otherwise,
    /// one must use getCookedGeoHandle to cook the SOP.
    void		 makeInstanceOf(SOP_Node *node, OP_Context &context, int inputidx);

    /// This makes us shallow copy the given gu_detailhandle.
    /// We are marked as a preserve request so with our next
    /// cook we will not re-use this gdp.  This should be
    /// called while cooking - ie, gdp is locked.
    /// While you can alter gdp afterwards, it is a bad idea as
    /// you will also alter the source.
    void		 makeInstanceOf(GU_ConstDetailHandle gdh);

    /// A hint to this node that it is about to be cooked at an
    /// unexpected time so shouldn't reuse the existing GDP. 
    /// Should not be used without understanding the consequences.
    void		 makePreserveRequest();

    virtual void	 setVisualization(int) {}
    virtual void	 setForceCapture(int) {}

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms) override;

    /// Fill in tree with this SOP's details
    virtual void 	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms) override;
    /// Helper class that puts our geometry info into a branch.
    void		 fillInfoTreeForGeometry(UT_InfoTree &branch,
				const OP_NodeInfoTreeParms &parms);

    /// Return an interface to apply appearance operations.  The default
    /// behaviour is to return a NULL pointer.
    virtual SOP_ObjectAppearancePtr	getObjectAppearance();

    /// This method has to do with 'fillInfoTreeNodeSpecific()' above. When the 
    /// call to OP_Node::fillInfoTree() completes (which is what called 
    /// fillInfoTreeNodeSpecific()), the UT_InfoTree 'tree' will be parsed and 
    /// displayed in a dialog box by OPUI_OpInfoDialog, at which point, that 
    /// class will want to ask us - which branch of the tree would you like to
    /// be selected by default when the dialog is displayed for the first time?
    virtual const char	*getDefaultSelectedInfoTreeBranchName() const override;
    
    /// Spare data used to determine which input the group menu is associated
    /// with.
    /// Because these rely on static initialization, do NOT use,
    /// use the sparedata functions instead.
    static PRM_SpareData	theFirstInput;
    static PRM_SpareData	theSecondInput;
    static PRM_SpareData	theThirdInput;
    static PRM_SpareData	theFourthInput;
    /// Uses the second input if present, else the first input.
    /// FIXME: Support for this has only been added to allAttribsMenu.
    static PRM_SpareData        theSecondElseFirstInput;

    /// Spare data to determine whcih input the group menu is associated
    /// with.
    static PRM_SpareData	*sparedataFirstInput();
    static PRM_SpareData	*sparedataSecondInput();
    static PRM_SpareData	*sparedataThirdInput();
    static PRM_SpareData	*sparedataFourthInput();
    static PRM_SpareData	*sparedataSecondElseFirstInput();

    ///Absolute Width
    virtual fpreal       getW() const override;
    ///Absolute Height
    virtual fpreal       getH() const override;


    /// Provide a verb interface to this SOP.  Verb interfaces are designed
    /// to ensure thread safety and prevent any persistent storage between
    /// cooks.
    virtual const SOP_NodeVerb	*cookVerb() const { return 0; }

protected:
	     SOP_Node(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~SOP_Node();

    // We listen to these as our Cache relies on unique ids, so must
    // know when they are no longer valid.
    virtual void	 setUniqueId(int id) override;
    virtual void	 clearUniqueId() override;

    // Remove the adhoc group(s) from the list of adhocs to be deleted when
    // cooking ends. This way, deletion of the group becomes your
    // responsibility. You should rarely and cautiously use these methods.
    // Most of the time you need destroyAdhocGroup[s] instead.
    // You *MUST* use the parse.*Copy methods to get the adhoc group.
    // Otherwise, you may delete a group which you don't have ownership of.
    int 		 removeAdhocGroup (const GA_Group *group)
			 { return myGroupParse.removeAdhocGroup(group); }
    void		 removeAdhocGroups()
			 { myGroupParse.removeAdhocGroups(); }

    //  Protected virtuals:
    //
    //  "cookMe" is where the actual work is done.
    //  The error is returned (OP_ERR_NONE on success).
    //
    virtual OP_ERROR	 cookMe   (OP_Context &context) override;
    virtual OP_ERROR	 bypassMe (OP_Context &context, 
                    	           int &copied_input) override;
    
    virtual OP_ERROR	 cookMySop(OP_Context &context) = 0;

    /// This is meant to be called within cookMySop() if all you
    /// want to do is invoke the verb returned by cookVerb().
    OP_ERROR		 cookMyselfAsVerb(OP_Context &context);

    // The interests node is the node to get an extra input on whatever
    // actual real data node is at the source of this chain.
    virtual GU_DetailHandle cookMySopOutput(OP_Context &context, int outputidx, SOP_Node *interests);
    virtual OP_ERROR	 cookMyGuide1(OP_Context &context);
    virtual OP_ERROR	 cookMyGuide2(OP_Context &context);
    virtual OP_ERROR	 cookMyGuideList(OP_Context &context,
			    UT_Array<GU_DetailHandle> &list);

    virtual OP_ERROR	 pubCookInputGroups(OP_Context &context, 
                    	                    int alone = 0) override;

    void		 notifyGroupParmListeners(
                            int parm_index, int group_type_index,
                            const GU_Detail *pgdp, const GA_Group *group);

    /// The cookInput.+Groups functions are helper functions that look at
    /// your Group Name and Group Type parameters (the offsets for which
    /// are passed in parm_index, for Group Name, and group_type_index,
    /// for group_type), evaluate them, and then call parse.+Group on that
    /// group name. For every method other than cookInputAllGroups,
    /// group_type_index is used only to determine whether the parm is dirty.
    /// The pointer group is set as a return value.
    /// If alone is true, we are cooking for a selector, and can't assume that
    /// our gdp is valid (because it might not have been constructed by now).
    /// If allow_reference is true, the group returned may not be modified.
    /// If fetchgdp is true and alone is true, these new groups will be
    /// created on the input_index-th input (because the gdp isn't constructed
    /// yet).
    /// If fetchgdp is true and alone is false, false, groups will be
    /// constructed on the current gdp.
    /// If fetchgdp is false, you must supply a gdp to create groups on in
    /// the paramerer pgdp.
    OP_ERROR		 cookInputPrimitiveGroups(OP_Context &context,
			    const GA_PrimitiveGroup *&group,
			    bool alone = false, bool do_selection = true,
			    int parm_index = 0,
			    int group_type_index = -1,
			    bool allow_reference = true,
			    bool ordered = false,
                            bool detached = true,
			    int input_index = 0);
    OP_ERROR		 cookInputPrimitiveGroups(OP_Context &context,
			    const GA_PrimitiveGroup *&group,
			    bool alone, bool do_selection,
			    int parm_index,
			    int group_type_index,
			    bool allow_reference,
			    bool ordered,
                            const GroupCreator &creator);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator or other version of function and dont cast away const on input geo and be careful since defaults have changed)
    virtual OP_ERROR	 cookInputPrimitiveGroups(OP_Context &context,
			    const GA_PrimitiveGroup *&group,
			    GU_DetailGroupPair &detail_group_pair,
			    int alone = 0, bool do_selection = true,
			    int input_index = 0, int parm_index = 0,
			    int group_type_index = -1,
			    bool allow_reference = true,
			    bool fetchgdp = true,
			    GU_Detail *pgdp = NULL); 

    /// See cookInputPrimitiveGroups.
    OP_ERROR		 cookInputPointGroups(OP_Context &context,
			    const GA_PointGroup *&group,
			    bool alone = false, bool do_selection = true,
			    int parm_index = 0,
			    int group_type_index = -1,
			    bool allow_reference = true,
			    bool ordered = false,
                            bool detached = true,
			    int input_index = 0);
    OP_ERROR		 cookInputPointGroups(OP_Context &context,
			    const GA_PointGroup *&group,
			    bool alone, bool do_selection,
			    int parm_index,
			    int group_type_index,
			    bool allow_reference,
			    bool ordered,
                            const GroupCreator &creator);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator or other version of function and dont cast away const on input geo and be careful since defaults have changed)
    virtual OP_ERROR	 cookInputPointGroups(OP_Context &context,
			    const GA_PointGroup *&group,
			    GU_DetailGroupPair &detail_group_pair,
			    int alone = 0, bool do_selection = true,
			    int input_index = 0, int parm_index = 0,
			    int group_type_index = -1,
			    bool allow_reference = true,
			    bool fetchgdp = true,
			    GU_Detail *pgdp = NULL);

    /// See cookInputPrimitiveGroups.
    OP_ERROR		 cookInputEdgeGroups(OP_Context &context,
			    const GA_EdgeGroup *&group,
			    bool alone = false, bool do_selection = true,
			    int parm_index = 0,
			    int group_type_index = -1,
			    bool allow_reference = true,
			    int input_index = 0);
    OP_ERROR		 cookInputEdgeGroups(OP_Context &context,
			    const GA_EdgeGroup *&group,
			    bool alone, bool do_selection,
			    int parm_index,
			    int group_type_index,
			    bool allow_reference,
                            const GU_Detail *pgdp);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator or other version of function and dont cast away const on input geo and be careful since defaults have changed)
    virtual OP_ERROR	 cookInputEdgeGroups(OP_Context &context,
			    const GA_EdgeGroup *&group,
			    GU_DetailGroupPair &detail_group_pair,
			    int alone = 0, bool do_selection = true,
			    int input_index = 0, int parm_index = 0,
			    int group_type_index = -1,
			    // JML: I do not know why Edge Groups
			    // used Copy instead of the reference version.
			    // They are left like this so as to not cause
			    // problems unforeseen.
			    bool allow_reference = false,
			    bool fetchgdp = true,
			    GU_Detail *pgdp = NULL);

    /// See cookInputPrimitiveGroups. Also, is_default_prim only affects
    /// cases when we're not able to guess the type of groups; in those
    /// cases, we try again with primitive groups if is_default_prim is
    /// true, and point groups otherwise.
     OP_ERROR		 cookInputAllGroups(OP_Context &context,
			    const GA_Group *&group,
			    bool alone = false, bool do_selection = true,
			    int parm_index = 0,
			    int group_type_index = -1,
                            GA_GroupType grouptype = GA_GROUP_INVALID,
			    bool allow_reference = true,
                            bool is_default_prim = true,
			    bool ordered = false,
                            bool detached = true,
			    int input_index = 0);
    OP_ERROR		 cookInputAllGroups(OP_Context &context,
			    const GA_Group *&group,
			    bool alone, bool do_selection,
			    int parm_index,
			    int group_type_index,
                            GA_GroupType grouptype,
			    bool allow_reference,
                            bool is_default_prim,
			    bool ordered,
                            const GroupCreator &creator);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator or other version of function and dont cast away const on input geo and be careful since defaults have changed)
    virtual OP_ERROR	 cookInputAllGroups(OP_Context &context,
			    const GA_Group *&group,
			    GU_DetailGroupPair  &detail_group_pair,
			    int alone = 0, bool do_selection = true,
			    int input_index = 0, int parm_index = 0,
			    int group_type_index = -1,
			    bool allow_reference = false,
			    bool is_default_prim = true,
			    bool fetchgdp = true,
			    GU_Detail *pgdp = NULL);

    /// Parse the input group specified by groupname, of type grouptype,
    /// in detail gdp. If allow_reference is true, this group cannot be
    /// later modified. If is_default_prim is true, if the group's type cannot
    /// be guessed, we'll try with a primitive group; otherwise, we'll try
    /// with a point group.
    const GA_Group *parseInputGroup(const UT_String &groupname,
			    GA_GroupType grouptype, const GroupCreator &creator,
			    bool allow_reference, bool is_default_prim,
                            bool ordered=false);
    const GA_Group *parseInputGroup(const UT_String &groupname,
			    int grouptype, GU_Detail *gdp,
			    bool allow_reference, bool is_default_prim);

    /// Retrieve a list of the local variables visible to this node.
    /// This list is unsorted and may contain duplicates.
    virtual void	buildLocalVarNames(UT_StringArray &out_vars) override;

private:
    /// Helper function for cookInput.+Groups. You shouldn't need to use it.
    OP_ERROR cookInputGroupsPrologue(OP_Context &context,
				     bool alone, int input_index, bool detached,
				     GroupCreator &creator);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function)
    OP_ERROR cookInputGroupsPrologue(OP_Context &context, bool fetchgdp,
				     bool alone, int input_index,
				     GU_Detail *&pgdp);

    /// Helper function for cookInput.+Groups. You shouldn't need to use it.
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function)
    void cookInputGroupsEpilogue(int parm_index, int group_type_index,
				 const GU_Detail *pgdp, const GA_Group *group,
				 int input_index, bool fetchgdp, bool alone);

protected:
    virtual void	 inputConnectChanged(int which_input) override;

//_________________________________________________________________________
//
//  Some very useful methods used in cooking SOPs, convenience if you will
//_________________________________________________________________________
//

    // Note that the context parameter is unused!
    const GU_Detail	*inputGeo(int index, OP_Context &)
			 { return inputGeo(index); }
    const GU_Detail	*inputGeo(int index) const;
    GU_DetailHandle	 inputGeoHandle(int index) const;

//_________________________________________________________________________
//
//  Convenience Error methods...
//_________________________________________________________________________
//
public:
    void		 addError(int code, const char *msg = 0)
			 { getLockedErrorManager()->addError(SOP_OPTYPE_NAME,
							     code, msg); }
    void		 addMessage(SOP_ErrorCodes code, const char *msg = 0)
			 { getLockedErrorManager()->addMessage(SOP_OPTYPE_NAME,
							       code, msg); }
    void		 addWarning(SOP_ErrorCodes code, const char *msg = 0)
			 { getLockedErrorManager()->addWarning(SOP_OPTYPE_NAME,
							       code, msg); }
    void		 addFatal(SOP_ErrorCodes code, const char *msg = 0)
			 { getLockedErrorManager()->addFatal(SOP_OPTYPE_NAME,
							     code, msg); }
    void		 addSystemError(const char *msg = 0)
			 { getLockedErrorManager()->systemError(msg);}
    void		 addCommonError(UT_CommonErrorCode what,
					const char *msg = 0)
			 { getLockedErrorManager()->commonError(what, msg);}
    void		 addCommonWarning(UT_CommonErrorCode what,
					const char *msg = 0)
			 { getLockedErrorManager()->commonWarning(what, msg);}

protected:
    //
    //  Methods for dealing with groups in SOPs

    /// We want these public as we it is perfectly fine to use them in
    /// static parm templates.  This occurs, for example, when instancing
    /// multiparms.
public:
    static PRM_ChoiceList	 allGroupMenu;	      // All the groups
    static PRM_ChoiceList	 groupMenu;	      // Prim and point groups
    static PRM_ChoiceList	 primGroupMenu;
    static PRM_ChoiceList	 pointGroupMenu;
    static PRM_ChoiceList	 edgeGroupMenu;
    static PRM_ChoiceList	 edgePointGroupMenu;  // Edge and point groups
    static PRM_ChoiceList	 breakpointGroupMenu;
    static PRM_ChoiceList	 primNamedGroupMenu;  // Named prim grps only
    static PRM_ChoiceList	 pointNamedGroupMenu; // Named pt groups only
    static PRM_ChoiceList	 edgeNamedGroupMenu;  // Named edge groups only
    static PRM_ChoiceList	 vertexNamedGroupMenu;// Named vertex groups only
    static PRM_ChoiceList	 namedPrimsMenu;
    static PRM_ChoiceList	 namedPrimsGlobMenu;
    static PRM_ChoiceList	 namedVolumesMenu;
    static PRM_ChoiceList	 namedVolumesGlobMenu;

    static PRM_ChoiceList	 allAttribMenu;
    static PRM_ChoiceList	 detailAttribMenu;
    static PRM_ChoiceList	 primAttribMenu;
    static PRM_ChoiceList	 pointAttribMenu;
    static PRM_ChoiceList	 vertexAttribMenu;

    static PRM_ChoiceList        allTextureCoordMenu;
    static PRM_ChoiceList        vertexTextureCoordMenu;

    /// Returns spare data that adds a "group select" button which runs a
    /// python script to invoke soputils.selectGroupParm. The group_type
    /// should be used if only one group type is supported by the group parm.
    /// The group_type_parm value should be used (with group_type set to
    /// GA_GROUP_INVALID) if there is a separate menu parameter to specify
    /// the group type. The input_index indicates which input of the SOP
    /// the group refers to. The merge_spare_data can contain any other
    /// PRM_SpareData that you want on the parameter (the most common being
    /// "&SOP_Node::theSecondInput" to control the input used by the
    /// SOP_Node::primGroupMenu, SOP_Node::pointGroupMenu, or other SOP_Node
    /// standard parameter group menus.
    /// If the SOP requires or supports multi-stage selection, then it's 
    /// possible to show previous selections during this picking sessions to
    /// assist the user. To do this, a comma-separated list of group types /  
    /// group parm names can be given with assoc_groups 
    /// (e.g. "point my_point_group_parm, edge my_edge_group_parm"). 
    enum class GroupSelectAsOrdered
    {
	/// AUTO: Use any bound selectors to determine whether the selection
	///       should be ordered or not.
	AUTO,
	/// FORCE_UNORDERED: Force an unordered selection.
	FORCE_UNORDERED,
	/// FORCE_ORDERED: Force an ordered selection.
	FORCE_ORDERED
    };
    static PRM_SpareData	*getGroupSelectButton(
					GA_GroupType group_type,
					const char *group_type_parm = NULL,
					int input_index = 0,
					PRM_SpareData *merge_spare_data = NULL,
					const char *assoc_groups = NULL,
					GroupSelectAsOrdered ordered =
					    GroupSelectAsOrdered::AUTO);
    static PRM_SpareData	*getGroupSelectButton(
					GA_GroupMaskType group_mask,
					const char *group_type_parm = NULL,
					int input_index = 0,
					PRM_SpareData *merge_spare_data = NULL,
					const char *assoc_groups = NULL,
					GroupSelectAsOrdered ordered =
					    GroupSelectAsOrdered::AUTO);

protected:
    /// Open a tree chooser populated by the values from the @c path_attrib
    /// string values so the user can select primitives.  The @c stringparm
    /// parameter is evaluated to choose the selected primitives, and the
    /// result is written back to the @c stringparm when the user makes their
    /// selection.  If the @c path_attrib doesn't exist, then optionally,
    /// packed primitive intrinsic names can be used to populate the tree.
    void	pickPrimitivesUsingPathAttribute(
		    const GU_Detail &gdp,
		    const GA_Range &srcrange,
		    const char *stringparm,
		    fpreal t,
		    const char *path_attrib="path",
		    bool use_packed_names=true);
    /// Pick assuming that the range is a full range of primitives.  This
    /// method gets the primitive range from the specified @c input_number.
    void	pickPrimitivesUsingPathAttribute(
		    const char *stringparm,
		    fpreal t,
		    int input_number=0,
		    const char *path_attrib="path",
		    bool use_packed_names=true);
    /// Create a GA_Range containing the primitives specified by the values in
    /// the @c pickedstring.  The primitives are selected from the @c
    /// srcrange in the given @c gdp.
    static GA_Range	getPickedRangeUsingPathAttribute(
			    const GU_Detail &gdp,
			    const GA_Range &srcrange,
			    const char *pickedstring,
			    const char *path_attrib="path",
			    bool use_packed_names=true);

    /// WARNING:  The groups returned are const and should not be modified
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator or other version of function and dont cast away const on input geo)
    int				 parseGroups(const char *pattern,
					     GU_Detail *gdp,
					     const GA_PrimitiveGroup *&prim,
					     const GA_PointGroup *&points,
					     bool strict = false);

    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator or other version of function and dont cast away const on input geo)
    int				 parseGroupsCopy(const char *pattern,
					     GU_Detail *gdp,
					     GA_PrimitiveGroup *&prim,
					     GA_PointGroup *&points,
					     GA_EdgeGroup *&edges,
					     int unify,
					     bool strict = false,
					     GA_GroupType numeric_type
							= GA_GROUP_PRIMITIVE);

public:
    /// Method to help fill in the menu entries representing attributes
    /// It will iterate though the attributes of the 'dictionary' supplied
    /// from the input 'input_index'. The 'approve' filter callback is applied 
    /// (if present) and if it returns ture for an attribute, this attribute
    /// is appended to the 'menu_entries'. If not present, all attributes
    /// are appended. The number of entries appended does not exceed
    /// 'max_menu_size' (including the terminating sentinel). 'menu_entries' are
    /// sorted alphabetically and the function returns the number of entries
    /// appended (excluding the terminating sentinel).
    int			 fillAttribNameMenu(PRM_Name *menu_entries,
				int max_menu_size,
				GA_AttributeOwner dictionary,
				int input_index,
				bool (*approve)(const GA_Attribute*,void*)=NULL,
				void *approve_data = NULL);

    /// utility method for pruning the duplicate entries in a sorted menu
    static void		 removeMenuDuplicates(PRM_Name *menuEntries, int size);

    /// Obtains pointers to the vertex, point, and primitive vector attributes.
    /// Only vector attributes are considered.
    /// NB: it needs access to the gdp, so use it only in the cook methods
    void		 getVectorAttribs( const char * attrib_names,
				UT_Array<GA_Attribute *> &vertex_attribs,
				UT_Array<GA_Attribute *> &point_attribs,
				UT_Array<GA_Attribute *> &prim_attribs);
    static void		 getVectorAttribs( GU_Detail *gdp,
				const char * attrib_names,
				UT_Array<GA_Attribute *> &vertex_attribs,
				UT_Array<GA_Attribute *> &point_attribs,
				UT_Array<GA_Attribute *> &prim_attribs);

    /// parses the vector attrib parameter and fills attribs array
    static void		 getVectorAttribs(const GA_AttributeDict &dict,
				const char * attrib_string,
				UT_Array<GA_Attribute *> &attribs,
				UT_String &bad_attribs);

protected:

    /// Drag-n-drop receiver functions

    virtual int			acceptDragDrop(DD_Source &src, 
               			               const char *l) override;
    virtual int			testDragDrop(DD_Source &src) override;
    virtual void		getDragDropChoice(DD_Source &src,
						DD_ChoiceList &choices) override;

    typedef void (SOP_Node::*GroupOperation)(const GA_Group *, void *);

    /// Perform an operation on each group matching the specified mask.
    int forEachGroupMatchingMask(
        const char *pattern,
        GroupOperation operation,
        void *data,
        GA_GroupType grpType,
        const GEO_Detail *pgdp=nullptr,
        bool maintainorder=false);
public:
    static int invokeForEachGroupMatchingMask(
	const GU_Detail *gdp,
        const char *pattern,
        GA_GroupType grpType,
        const std::function<void(const GA_Group *)> &operation,
        bool maintainorder);
protected:

    /// Take an optional group and subdivide it into smaller groups as
    /// input to operation

public:
    static void invokeForSubdivisionOfPrimitiveGroup(
		const std::function<void(const GA_Group *)> &operation,
		const GA_PrimitiveGroup *group, 
		int optype, 
		int opstep, const char *oplabel,
		const GU_Detail *gdp);

protected:
    void			subdividePrimitiveGroup(
					GroupOperation operation,
					const GA_PrimitiveGroup *group, 
					void *data, int optype, 
					int opstep, const char *oplabel,
					const GU_Detail *gdp=0);
    void			subdivideMixPrimitiveGroup(
					GroupOperation operation,
					const GA_PrimitiveGroup *group, 
					void *data, int optype, 
					int opstep, const char *oplabel,
					GU_Detail *gdp=0);
    void			subdividePointGroup(
					GroupOperation operation,
					const GA_PointGroup *group, 
					void *data, int optype, 
					int opstep, const char *oplabel,
					GU_Detail *gdp=0);


    /// Create an adhoc (internal) group and add it to the list of adhoc groups.
    /// The group will be deleted for you after cooking.
    GA_PrimitiveGroup	*createAdhocPrimGroup (GU_Detail &geo,
					       const char *p="adhoc")
			 { return myGroupParse.createPrimitiveGroup(geo, p); }
    GA_PointGroup	*createAdhocPointGroup(GU_Detail &geo,
					       const char *p="adhoc")
			 { return myGroupParse.createPointGroup(geo, p); }
    GA_EdgeGroup	*createAdhocEdgeGroup (GU_Detail &geo,
					       const char *p="adhoc")
			 { return myGroupParse.createEdgeGroup(geo, p); }
    GA_VertexGroup	*createAdhocVertexGroup (GU_Detail &geo,
						 const char *p="adhoc")
			 { return myGroupParse.createVertexGroup(geo, p); }
    GA_PrimitiveGroup	*createAdhocPrimGroup(const GEO_Detail &geo)
    {
        GA_PrimitiveGroup *group = geo.newDetachedPrimitiveGroup();
        myGroupParse.appendAdhocGroup(group, true);
        return group;
    }
    GA_PointGroup	*createAdhocPointGroup(const GEO_Detail &geo)
    {
        GA_PointGroup *group = geo.newDetachedPointGroup();
        myGroupParse.appendAdhocGroup(group, true);
        return group;
    }
    GA_EdgeGroup	*createAdhocEdgeGroup(const GEO_Detail &geo)
    {
        GA_EdgeGroup *group = geo.newDetachedEdgeGroup();
        myGroupParse.appendAdhocGroup(group, true);
        return group;
    }
    GA_VertexGroup	*createAdhocVertexGroup(const GEO_Detail &geo)
    {
        GA_VertexGroup *group = geo.newDetachedVertexGroup();
        myGroupParse.appendAdhocGroup(group, true);
        return group;
    }
    GA_BreakpointGroup  *createAdhocBreakpointGroup (const GEO_Detail &geo)
    {
        return myGroupParse.createBreakpointGroup(geo);
    }

    /// This is only called when the instance flag is set.  
    ///	The gdp will be modified shortly after this function is called.  After
    ///	the return, the gdp member data should contain an empty (or full)
    ///	GU_Detail.  Setting the duplicate flag will duplicate the current gdp
    ///	in the new one.
    /// This used to require one to derive from it to clear references
    ///  to myInstanceOp.  This is now handled at this level.
    void	 	 clearInstance(int duplicate = 0);
    /// True if another SOP has an instance of our geometry.
    bool		 hasInstances();
    /// Counts the number SOPs that are descended instances of our geometry.
    int			 countInstances();
    /// Clears SOPs with descended instances of our geometry.
    void		 clearAllInstances();

    /// In between cooks, the gdp pointer might be reset by the cooking engine
    /// (eg. if its gdp was instanced by some other SOP). If this function
    /// returns true, then it will be reset to an empty gdp. If this function
    /// returns false, then the contents of the old gdp will be duplicated into
    /// the new gdp. SOPs performing simulation which rely on the cooked
    /// contents of its gdp from the last frame should override this to return
    /// false.
    virtual bool	 shouldResetGeoToEmpty() const  { return true; }

    /// The duplicate*Source methods take a context parameter but do not
    /// use it for anything.
    /// The specified input must already be locked.  The context used to
    /// lock the input is the context used for duplication.
    OP_ERROR		 duplicateSource(unsigned index, OP_Context &context,
					 GU_Detail *gdp, bool clean=true);
    OP_ERROR		 duplicateSource(unsigned index, OP_Context &context)
    {
        return duplicateSource(index, context, gdp, true);
    }

    /// Only duplicates the source if the source has changed. Otherwise,
    /// it copies the points from the source to our gdp. It's meant to be
    /// used by most deformer SOPs, which simply transform the points and
    /// or the primitives. If wattrib is true, we check if any transforming
    /// primitive or vertex attributes are present and if any point attribs
    /// exist, and copy those too.
    ///
    /// NOTE: Do not use this if you are creating or destroying point
    /// attributes.
    ///
    /// NOTE: While this takes a gdp to operate on, if it is not the SOP's
    /// own gdp, it will just do a duplicateSource.
    OP_ERROR		 duplicatePointSource(unsigned index, OP_Context &ctx,
					 GU_Detail *gdp, bool clean = true,
					 bool wattrib = true,
					 bool *fully = 0);
    OP_ERROR		 duplicatePointSource(unsigned index, OP_Context &ctx)
			 {
			     return duplicatePointSource(index, ctx, gdp, 1,
							 true, 0);
			 }

    /// This copies all the point positions from the given input gdp into
    /// the current one.
    /// @pre input_i has already been locked via lockInput
    bool		 copyPointPosFromSource(
			    unsigned input_i, OP_Context &ctx);

    /// @brief Only duplicates the source if the source has changed since the
    /// last call to this method.
    OP_ERROR		 duplicateChangedSource(unsigned idx, OP_Context &ctx,
					int *changed = 0, bool force=false);

    /// Only duplicates primitives in primgroup from src_gdp to dest_gdp
    OP_ERROR		 duplicateSourceGroup(const GU_Detail *src_gdp,
					  const GA_PrimitiveGroup *primgroup,
					  GU_Detail *dest_gdp=0);
    
    /// Forces duplicateChangedSource() to do the duplication on its next call.
    void		 resetChangedSourceFlags();

    /// @brief Checks if a duplication will occur on the next call to
    /// duplicateChangedSource().
    ///
    /// Similar to useInputSource() except it doesn't affect the flags. It's
    /// used to simply query whether the given input has changed. Returns false
    /// if there's errors on the input. Upon return, @c changed is updated.
    OP_ERROR		 checkChangedSourceFlags(unsigned idx, OP_Context &ctx,
					int *changed);

    /// @brief Mark the given input as used for cooking by this node.
    ///
    /// This updates source flags to mark the input as now clean, returning
    /// whether the input has changed since the last time this function was
    /// called. If force is true, then we treat the input as always dirty.
    /// Returns false if the input has errors.
    /// @note duplicateChangedSource() calls this function.
    bool		 useInputSource(unsigned idx, bool &changed, bool force)
			 {
			     return myInputChangeHelper.useInput(
						*this, idx, changed, force);
			 }

    virtual void	 deleteCookedData() override;
    virtual int		 deleteNodeData(void *) override;

    /// Convenience wrappers for several common pasting operations. The last
    /// method destroys both groups:
    GA_PrimitiveGroup	*getPastedSurfaces()
			 {
			     return gdp->getPastedSurfaces("_sop_all_pasted_");
			 }
    GA_PrimitiveGroup	*getPastedSurfaces(GA_PrimitiveGroup *&used)
			 {
			     return gdp->getPastedSurfaces(used, 
						     "_sop_all_pasted_",
						     "_sop_used_pasted_");
			 }
    void		 updatePastedDisplacement(GA_Offset ptoff,
						const GA_PrimitiveGroup *all,
						GA_PrimitiveGroup *used)
			 {
			     gdp->updatePastedDisplacement(ptoff, all, used);
			 }
    int 		 updatePastedDisplacements()
			 {
			     // return 1 if pasted surfaces found
			     return gdp->updatePastedDisplacements();
			 }
    int 		 updatePastedDependents(GA_PrimitiveGroup *all,
						GA_PrimitiveGroup *used)
			 {
			     // all and used are deleted if they exist
			     return gdp->updatePastedDependents(all, used);
			 }

    /// Methods for dealing with group centroid
    void		 convertPivotAboutGroupCentroid(
				    PRM_Parm &grppivot_parm,
				    int pivot_parm_idx,
				    const UT_Vector3 *centroid = 0);
    void		 convertPivotAboutGroupCentroid(
				    PRM_Parm &grppivot_parm,
				    const char *pivot_parm_name,
				    const UT_Vector3 *centroid = 0);
    
    
    /// subclasses override this to return the group centroid if they support
    /// it.
    virtual bool	 getGroupCentroid(OP_Context & /*context*/,
					  UT_Vector3 & /*centroid*/)
				    { return false; }

    //
    //  I/O methods
    //
    virtual OP_ERROR	 save(std::ostream &os, const OP_SaveFlags &flags,
					const char *pathPrefix,
					const UT_String &name_override = UT_String()) override;
    virtual bool	 load(UT_IStream &is, const char *extension,
                                           const char *path=0) override;

    void		 refreshGdp();


    //
    // Data for the SOP class
    //

    /// The gdp handle provides a indirect reference to the current
    /// gdp of this SOP.
    GU_DetailHandle	 myGdpHandle;

    /// During cooks, myGdpHandle is writelocked and stored in the
    /// gdp variable.  This is to provide backwards compatibility.
    /// The gdp variable should not be trusted or accessed elsewhere.
    GU_Detail		*gdp;
    SOP_Guide		*myGuide1;	// primary guide geometry
    SOP_Guide		*myGuide2;	// secondary guide geometry
    SOP_NodeFlags	 mySopFlags;

private:
    //
    //  These virtuals are methods that nobody should override...
    //
    //  I/O methods
    //
    virtual OP_ERROR	 saveIntrinsic(std::ostream &os, 
                    	               const OP_SaveFlags &flags) override;

    virtual bool	 loadPacket(UT_IStream &is, 
                	            short class_id, short sig,
				    const char *path=0) override;
    virtual bool	 loadPacket(UT_IStream &is, const char *token,
				    const char *path=0) override;

    virtual void	 clearAndDestroy() override;

    virtual int		 saveCookedData(const char *, 
               		                OP_Context &) override;
    virtual int		 saveCookedData(std::ostream &os, 
               		                OP_Context &, 
               		                int binary = 0) override;
    virtual bool	 loadCookedData(UT_IStream &is, 
                	                const char *path=0) override;

    /// loadHardCookedData is just like loadCookedData, except it doesn't
    /// check if this SOP is softlocked.  Use it only if you know you are
    /// loading hardlocked data (as in SOP_UndoModel)
	    bool	 loadHardCookedData(UT_IStream &is, const char *path=0);

    virtual const char	*getFileExtension(int binary) const override;

    int		 	 softLockable(const GU_Detail &unmodelled_gdp,
				      const GU_Detail &modelled_gdp) const;
    /// take a snapshot of the current gdp and stores the soft lock data.
    /// copies the previous slock data to the given reference (so caller
    /// can use it for undo)
    /// returns 1 on success, 0 otherwise
    int		 	 storeSoftLockData(OP_Context &context, int prev_lock,
					   SOP_SoftLockData *old_slockdata=0);

    sopVisualizeCallback *myVisualizeCallback;
    void		 *myVisualizeCallbackData;
    
public:

    // compact the weight data of an index pair attribute
    static void compactIndexPair(GA_Attribute *attrib);

public:
    /// Do not use this function! Using it will likely cause crashes.
    virtual void	 unloadDataHack();

    virtual bool	 unloadData() override;

    /// Returns the amount of memory owned by this node, *NOT* including
    /// any cooked geometry.
    virtual int64	 getMemoryUsage(bool inclusive) const override;

    /// Returns the amount of memory used by this SOP, that will
    /// be unloaded by an unload call.  Baseclass defaults to
    /// returning the GDP's data.
    virtual int64	 getLoadedMemoryUsage() const;

    /// Returns whether this node currently has cooked data or not.
    bool		 isLoaded() const override;

    /// Returns if the sop cache manager will allow unloading.
    bool		 shouldUnload() const;

    /// Convenience method to prepare newly allocated GU_Detail objects to
    /// act as the output.
    /// Note that this returns the passed in gdp pointer.
    static GU_Detail		*prepNewDetail(GU_Detail *gdp);

    /// This method is automatically called at the conclusion of cooking to
    /// notify the gdp that it's data has changed.  By default, this will call:
    /// @code
    ///   if (bumpalldataid)
    ///	      gdp->bumpAllDataIds();
    ///   gdp->defragment();
    ///   gdp->edgeGroups().makeAllEdgesValid();
    ///   gdp->incrementMetaCacheCount();
    /// @endcode
    static void		markDetailDirty(GU_Detail *dirtygdp,
					bool bumpalldataid);

    /// All SOP nodes can be a selection owner.
    /// This replaces the previous inheritance from SOP_SelectionOwner.
    virtual bool isSelectionOwnerType() const final
    { return true; }

private:
    OP_InputChangeHelper myInputChangeHelper;

    /// We store all locked inputs in this list.  We can then allow
    /// the same source node to be cooked with different contexts
    /// and let copy on write semantics avoid problems.
    /// Any non-null handle here has PreserveRequest set and should
    /// have it removed before removing.
    UT_ValArray<GU_DetailHandle *>	myInputGeoHandles;

    GOP_Manager		 myGroupParse;
    
    friend class	 SOP_UndoModel; // needs access to gdp
    friend class	 SOP_CacheManager; // needs access to handle.
    friend class	 SOP_CacheData; // needs access to handle.
    friend class	 sop_AttributeEvaluator; // needs access to local var info

    //
    // Private data
    //
    SOP_SoftLockData	*mySoftLockData;

    bool		 myHasCookedSelection;

    /// the capture override flag is used to indicate wheter the SOPs should
    /// use their capture parameters or cook capture geometry. (e.g., 
    /// SOP_CaptureRegion). This flag is mirrored from OBJ_Bone, which
    /// allows to view bones and capture region in their capture configuration
    static  bool	 theGlobalCaptOverride;

    UT_Lock		 myInstancingOpsLock;
    UT_Set<int>		 myInstancingOps;
    int			 myInstanceOp;		// Which op we are instancing.

public:
    // This method needs to be public so HOM can flag an expression as needing
    // corresponding to local variables.
    void			 setUsesSOPLocalVar()
				 { myUsesSOPLocalVar = true; }

    sop_AttributeEvaluator	*createAttributeEvaluator(const char *attribname);
protected:
    /// Local variable items:
    UT_SymbolMap<sopVarInfo *>	 myLocalVarTable;
    UT_ValArray<sopVarInfo *>	 myLocalVarArray;
    int				 myLocalVarOrder;
    
    /// Tracks if any expression has triggered a local variable,
    /// like $PT, etc.  If this is false, it is safe to only
    /// evaluate the parm once for all subtypes.
    /// The higher level code is expected to clear this flag, evaluate their
    /// relevant parms, and then test this flag.
    bool			 myUsesSOPLocalVar;
private:
    // myFlagUnused used to be myLocalVarActive, which has been moved down to
    // OP_Node. It's still here to avoid breaking HDK binary compatibility.
    bool			 myFlagUnused;
    const GU_Detail		*myCurGdp[2];
    GU_DetailHandle		 myCurGdh[2];

    /// This is the selection set we use when the footprint flag is on.
    GU_SelectionSetHandle	 myFootprintSelectionSet;
    
    /// This is a complete override for all selection sets. Used temporarily
    /// by selectors.
    GU_SelectionSetHandle	 mySelectionSetOverride;
    
protected:
    GA_Offset 			 myCurVtxOff[2];
    int				 myCurVtxNum[2];
    GA_Offset			 myCurPtOff[2];
    GA_Offset			 myCurPrimOff[2];
    int				 myCurBuiltPtBox[2];
    UT_BoundingBox		 myCurPtBox[2];
    int				 myCurBuiltPrimBox[2];
    UT_BoundingBox		 myCurPrimBox[2];

    int				 myCurGdpVal;	// Which gdp is being traversed

    /// Cached values so we can tell if we have to rebuild.
    int				 myLocalVarOrderCache;
    int				 myCurrentLayerCache;
    /// This is all the old attribute information, one for each of the
    /// dictionary types...  If this (including the order) matches the
    /// new attribute data, no offsets will have changed.
    UT_Array<sop_attribRef>	 myAttribCache[2][4];
    UT_String			 myVarMapCache[2];
    int				 myAttribCacheOffsetGdpId[2];
    int				 myAttribCacheOffsetMetaCacheCount[2];
    unsigned int		 myAttribCacheTotalDictSize[2];
    /// Stores the @ cache lookups generated JIT.
    UT_StringMap<sop_AttributeEvaluator *>	myAttributeEvaluatorCache;

    /// Stores the parms & cache for old-style node cooking of verb
    /// based nodes.
    SOP_NodeCache		*myNodeVerbCache;
    SOP_NodeParms		*myNodeVerbParms;
    friend class SOP_NodeVerb;
    friend class SOP_NodeParms;
    friend class sop_NodeInputs;
};

/// Function to get a list of template arrays containing parm names that must
/// be reserved when compiling VEX SOPs.
SOP_API extern void
SOPgetVexReservedTemplateLists(UT_ValArray<PRM_Template *> &tplatelists);

#endif
