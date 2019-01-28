/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_NodeVerb.h ( SOP Library, C++)
 *
 * COMMENTS:
 *	Verb-based API for SOP cooking mechanisms.  SOPs can return
 *	one of these objects to describe how they cook, allowing the
 *	cook engine to perform additional optimizations.
 */

#ifndef __SOP_NodeVerb_h__
#define __SOP_NodeVerb_h__

#include "SOP_API.h"
#include "SOP_Node.h"
#include <OP/OP_Director.h>
#include <PRM/PRM_Parm.h>
#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <SYS/SYS_Types.h>

class CVEX_Context;
class CVEX_Function;
class DEP_MicroNode;
class SOP_API SOP_CVEXDataBacking
{
public:
    int64 getMemoryUsage(bool inclusive) const
    {
	int64 mem = inclusive ? sizeof(*this) : 0;
	mem += UTarrayDeepMemoryUsage(stringdata, false);
	return mem;
    }

public:
    fpreal32		floatdata[16];
    int32		intdata;
    UT_Array<UT_StringHolder>	stringdata;
};

enum SOP_CookEngine
{
    SOP_COOK_TRADITIONAL,
    SOP_COOK_COMPILED,
};


class SOP_API SOP_NodeCache
{
public:
	     SOP_NodeCache() 
	     {
		 myVexContext = 0;
		 myVexFunction = 0;
	     }
    virtual ~SOP_NodeCache();

    virtual int64	 getMemoryUsage(bool inclusive) const;

    CVEX_Context	*getVexContext(const char *vexpression);
    CVEX_Context	*buildVexContext(const char *vexpression);
    void		 setVexFunction(const CVEX_Function &function);
    void		 clearVexContext();

protected:
    CVEX_Context	*myVexContext;
    CVEX_Function	*myVexFunction;
    UT_StringHolder	 myVEXpression;
    UT_Array<SOP_CVEXDataBacking>	myReadBacking;
    UT_Array<SOP_CVEXDataBacking>	myWriteBacking;
    friend class SOP_NodeParms;
};

class SOP_API SOP_NodeParms
{
public:
	     SOP_NodeParms() {}
    virtual ~SOP_NodeParms() {}

    class LoadParms
    {
    public:
	LoadParms(SOP_NodeParms *parms, 
		    SOP_CookEngine cookengine,
		    const OP_Node *node, 
		    const OP_Context &context,
		    SOP_NodeCache *cache, 
		    UT_ErrorManager *error,
		    DEP_MicroNode *depnode)
	: myParms(parms)
	, myCookEngine(cookengine)
	, myNode(node)
	, myContext(context)
	, myCache(cache)
	, myError(error)
	, myDepNode(depnode)
	{
	}
	virtual ~LoadParms() {}

	const OP_Node		*node() const { return myNode; }
	template <typename T>
	const T			&parms() const
				{ return *UTverify_cast<const T*>(myParms); }
	SOP_NodeCache		*cache() const { return myCache; }
	const OP_Context	&context() const { return myContext; }
	UT_ErrorManager		*error() const { return myError; }
	DEP_MicroNode		*depnode() const { return myDepNode; }

	/// Methods to wire directly to the optional depnode.
	void			 addExplicitInput(DEP_MicroNode &inp, bool check_dup)
	{ if (myDepNode) myDepNode->addExplicitInput(inp, check_dup); }
	void			 addExplicitInput(DEP_MicroNode &inp)
	{ addExplicitInput(inp, /*check_dup*/true); }

	/// Methods to add directly to any present error manager.
	UT_ErrorSeverity addMessage (const char *type, int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ if (myError) return myError->addMessage(type, code, formatError(msg), loc); 
	  return UT_ERROR_MESSAGE; }
	UT_ErrorSeverity addWarning (const char *type, int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ if (myError) return myError->addWarning(type, code, formatError(msg), loc); 
	  return UT_ERROR_WARNING; }
	UT_ErrorSeverity addError   (const char *type, int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ if (myError) return myError->addError(type, code, formatError(msg), loc); 
	  return UT_ERROR_ABORT; }

	UT_ErrorSeverity sopAddMessage (int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ return addMessage("SOP", code, msg, loc); }
	UT_ErrorSeverity sopAddWarning (int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ return addWarning("SOP", code, msg, loc); }
	UT_ErrorSeverity sopAddError   (int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ return addError("SOP", code, msg, loc); }

    protected:
	/// Prefix errors so we can get sensible results.
	UT_StringHolder		formatError(const char *msg) const
	{
	    // Do not prefix with the path in traditional cook
	    // as it is properly assigned.
	    if (myCookEngine == SOP_COOK_TRADITIONAL)
		return msg;
	    UT_StringHolder	result;
	    if (!msg || !*msg)
		return result;
	    UT_WorkBuffer		buf;
	    UT_String			fullpath;
	    if (node())
		node()->getFullPath(fullpath);
	    else
		fullpath = "<internal>";
	    buf.sprintf("%s: %s", (const char *) fullpath, msg);
	    result = buf;
	    return result;
	}

	bool			 myCookEngine;
	SOP_NodeParms		*myParms;
	const OP_Node		*myNode;
	const OP_Context	&myContext;
	SOP_NodeCache		*myCache;
	UT_ErrorManager		*myError;
	DEP_MicroNode		*myDepNode;
    };

    void 	 loadFromOp(const LoadParms &loadparms);

    bool	 applyVexpression(const char *vexpression,
			    const UT_Array<GU_ConstDetailHandle> &vexinputs,
			    const LoadParms &loadparm);

    /// Detects specially named parameters, cooks required inputs using
    /// the traditional cook engine.
    bool	 applyVexpressionFromNode(
			    SOP_Node *node, OP_Context &context,
			    SOP_NodeCache *cache);

    virtual void	copyFrom(const SOP_NodeParms *src) { }

			    
    enum ParmType
    {
	PARM_INTEGER,
	PARM_FLOAT,
	PARM_VECTOR2,
	PARM_VECTOR3,
	PARM_VECTOR4,
	PARM_MATRIX2,
	PARM_MATRIX3,
	PARM_MATRIX4,
	PARM_STRING,
	PARM_RAMP,
	PARM_DATA,
	PARM_UNSUPPORTED
    };

    /// Marshalling interface to rest of world
    /// These all use index based notation.
    virtual exint	getNumParms() const { return 0; }
    virtual const char *getParmName(exint idx) const { return 0; }
    virtual ParmType	getParmType(exint idx) const { return PARM_UNSUPPORTED; }

    virtual void	getParmValue(exint idx, exint &value) const {}
    virtual void	getParmValue(exint idx, fpreal &value) const {}
    virtual void	getParmValue(exint idx, UT_Vector2D &value) const {}
    virtual void	getParmValue(exint idx, UT_Vector3D &value) const {}
    virtual void	getParmValue(exint idx, UT_Vector4D &value) const {}
    virtual void	getParmValue(exint idx, UT_Matrix2D &value) const {}
    virtual void	getParmValue(exint idx, UT_Matrix3D &value) const {}
    virtual void	getParmValue(exint idx, UT_Matrix4D &value) const {}
    virtual void	getParmValue(exint idx, UT_StringHolder &value) const {}
    virtual void	getParmValue(exint idx, UT_SharedPtr<UT_Ramp> &value) const {}
    virtual void	getParmValue(exint idx, PRM_DataItemHandle &value) const {}

    virtual void	setParmValue(exint idx, const exint &value) {}
    virtual void	setParmValue(exint idx, const fpreal &value) {}
    virtual void	setParmValue(exint idx, const UT_Vector2D &value) {}
    virtual void	setParmValue(exint idx, const UT_Vector3D &value) {}
    virtual void	setParmValue(exint idx, const UT_Vector4D &value) {}
    virtual void	setParmValue(exint idx, const UT_Matrix2D &value) {}
    virtual void	setParmValue(exint idx, const UT_Matrix3D &value) {}
    virtual void	setParmValue(exint idx, const UT_Matrix4D &value) {}
    virtual void	setParmValue(exint idx, const UT_StringHolder &value) {}
    virtual void	setParmValue(exint idx, const UT_SharedPtr<UT_Ramp> &value) {}
    virtual void	setParmValue(exint idx, const PRM_DataItemHandle &value) {}
				
protected:
    virtual void loadFromOpSubclass(const LoadParms &loadparms) = 0;

    // Used for coercing one type to another
    template <typename T, typename S> 
    void coerceValue(T &result, const S &src) const
    {
    }

};

template <> 
void inline SOP_NodeParms::coerceValue(exint &result, const exint &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(bool &result, const exint &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(exint &result, const bool &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(bool &result, const bool &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(fpreal &result, const fpreal &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_Vector2D &result, const UT_Vector2D &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_Vector3D &result, const UT_Vector3D &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_Vector4D &result, const UT_Vector4D &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_Matrix2D &result, const UT_Matrix2D &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_Matrix3D &result, const UT_Matrix3D &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_Matrix4D &result, const UT_Matrix4D &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_StringHolder &result, const UT_StringHolder &src) const
{ result = src; }
template <> 
void inline SOP_NodeParms::coerceValue(UT_SharedPtr<UT_Ramp> &result, const UT_SharedPtr<UT_Ramp> &src) const
{ result = src; }
template <>
void inline SOP_NodeParms::coerceValue(PRM_DataItemHandle &result, const PRM_DataItemHandle &src) const
{ result = src; }


///
/// SOP_NodeVerb
///
/// This should be a singleton class.  Build it with the Register
/// template to allow it to be discoverable by name.
///
class SOP_API SOP_NodeVerb
{
public:
	     SOP_NodeVerb() {}
    virtual ~SOP_NodeVerb() {}

    template <typename T>
    class Register
    {
    public:
	Register()
	{
	    myVerb = new T();
	    SOP_NodeVerb::registerVerb(myVerb);
	}
	~Register()
	{
	    SOP_NodeVerb::unregisterVerb(myVerb);
	    delete myVerb;
	}

	const T *get() const { return myVerb; }

    private:
	T		*myVerb;

    };

    virtual SOP_NodeCache	*allocCache() const
				    { return new SOP_NodeCache; }

    virtual SOP_NodeParms	*allocParms() const = 0;

    virtual UT_StringHolder	 name() const = 0;

    /// Finds a verb which matches the given name
    static const SOP_NodeVerb	*lookupVerb(const UT_StringRef &name);
    static void			 listVerbs(UT_StringArray &verbnames);
    static void			 registerVerb(SOP_NodeVerb *verb);
    static void			 unregisterVerb(SOP_NodeVerb *verb);

    class NodeExecuteInfo
    {
    public:
	void		clear()
	{
	    myExprInputs.clear();
	    myTimeDeps.clear();
	}
	void setExprInputs(const UT_Array<GU_ConstDetailHandle> &inputs,
			   const UT_IntArray &timedeps)
	{
	    myExprInputs = inputs;
	    myTimeDeps = timedeps;
	}
	const UT_Array<GU_ConstDetailHandle> &exprinputs() const { return myExprInputs; }
	const UT_IntArray &timedeps() const { return myTimeDeps; }

    protected:
	UT_Array<GU_ConstDetailHandle>	myExprInputs;
	UT_IntArray			myTimeDeps;
    };

    typedef UT_Map<const OP_Node *, NodeExecuteInfo> ForbiddenNodeMap;

    /// During execution of a verb chain you may wish to disable
    /// the traditional cooking of nodes.  This provides a set of
    /// nodes you do not want recursed into during the verb cook.
    /// It will be NULL if there are none or this thread isn't in
    /// a compiled execution path.
    static const ForbiddenNodeMap *forbiddenNodes();
    static const ForbiddenNodeMap *forbiddenNodes(int thread);

    /// This scope class will push the given list of nodes marking
    /// them as inelligible for cooking in the traditional path.
    /// Needless to say, the invoker needs to ensure the set's lifetime
    /// persists.
    class ScopeForbiddenNodes
    {
    public:
	ScopeForbiddenNodes(const ForbiddenNodeMap *forbiddennodes);
	~ScopeForbiddenNodes();
    protected:
	const ForbiddenNodeMap		*myPrevNodes;
    };

    enum CookMode
    {
	// With in place the destination gdp will be the first input or
	// a copy.
	// The first input will be null.
	COOK_INPLACE,
	// With duplicate, the destination gdp will be a copy of the
	// first input.  All inputs will be valid.
	COOK_DUPLICATE,
	// With generator, the destination gdp is empty, possibly
	// a stashAll().  All inputs will be valid.
	COOK_GENERATOR,
	// With instance, the destination gdp is invalid.  All inputs
	// will be valid.
	COOK_INSTANCE,
	// There will be a destination gdp, but its contents are unspecified
	// It may contain the results of the last cook. All inputs are
	// valid.
	COOK_GENERIC,
	// If cookMode is pass through, the cook method will not be
	// invoked at all.  Instead, the first input will be passed through
	// unchanged.  If there is no first input, an empty gdp is
	// created.
	COOK_PASSTHROUGH,
    };

    class NodeInputs
    {
    public:
		 NodeInputs() {}
	virtual ~NodeInputs() {}

	virtual exint	nInputs() const = 0;
	/// Returns if the input is wired.
	virtual bool	hasInput(exint idx) const = 0;

	/// Demands the input be cooked.  After this, the inputGeo()
	/// will return the contents.
	/// Returns false if the cook failed for some reason.
	virtual bool	cookInput(exint idx) = 0;

	/// Returns an invalid handle if the input isn't wired or not yet cooked.
	virtual GU_DetailHandle	inputGeo(exint idx) = 0;

	/// Unlocks/forgets the given input.  Returns an invalid handle if
	/// not wired or not cooked.
	virtual GU_DetailHandle	unloadInput(exint idx, bool flushce) = 0;
    };

    class InputParms
    {
    public:
	InputParms(NodeInputs &inputs, const SOP_NodeParms *parms,
		    UT_ErrorManager *error, DEP_MicroNode *depnode)
	: myInputs(inputs)
	, myParms(parms)
	, myError(error)
	, myDepNode(depnode)
	{
	}

	virtual ~InputParms() {}

	NodeInputs	&inputs() const { return myInputs; }

	template <typename T>
	const T			&parms() const
				{ return *UTverify_cast<const T*>(myParms); }

	UT_ErrorManager *error() const { return myError; }

	DEP_MicroNode *depnode() const { return myDepNode; }

    protected:
	NodeInputs		&myInputs;
	const SOP_NodeParms	*myParms;
	UT_ErrorManager		*myError;
	DEP_MicroNode		*myDepNode;
    };

    class CookParms
    {
    public:
	CookParms(GU_DetailHandle &destgdh,
		  const UT_Array<GU_ConstDetailHandle> &inputs,
		  SOP_CookEngine cookengine,
		  SOP_Node *node,
		  const OP_Context &context,
		  const SOP_NodeParms *parms,
		  SOP_NodeCache *cache,
		  UT_ErrorManager *error,
		  DEP_MicroNode *depnode)
	: myDestGdh(destgdh)
	, myInputs(inputs)
	, myCookEngine(cookengine)
	, myNode(node)
	, myContext(context)
	, myParms(parms)
	, myCache(cache)
	, myError(error)
	, myDepNode(depnode)
        , myCookTime(0)
	{}
	virtual ~CookParms() {}

	/// The initial state of gdh depends on the cookMode()
	GU_DetailHandle		&gdh() const { return myDestGdh; }

	/// The currently cooking SOP.  This is only non-null if in
	/// the old cook path.  When executing compiled nodes it will
	/// be null as the verb is supposed to operate independently
	/// of the original SOP.
	SOP_Node		*getNode() const { return (myCookEngine == SOP_COOK_TRADITIONAL) ? myNode : 0; }

	template <typename T>
	T			*getCookNode() const
				{ if (myCookEngine != SOP_COOK_TRADITIONAL) 
				    return 0;
				  return UTverify_cast<T *>(myNode); }
        fpreal                   getCookTime() const
                                { if (myCookEngine != SOP_COOK_TRADITIONAL)
                                    return 0;
                                  return myCookTime; }
        void                     setCookTime(fpreal time) { myCookTime = time; }

	/// The node that generated this verb, if any...  
	SOP_Node		*getSrcNode() const { return myNode; }
	/// The getCwd() should be used to evaluate relative paths.
	OP_Node			*getCwd() const { if (myNode) return myNode; return OPgetDirector(); }
	const OP_Context	&getContext() const
				 { return myContext; }

	/// Whether the underlying verb manages data ids.  All new nodes
	/// should manage data ids, this is purely for the
	/// SOP_NodeParmsOptions wrapper.
	bool			 getManagesDataIDs() const
	{
	    if (!myNode) return true;
	    return myNode->getFlags().getManagesDataIDs();
	}

	/// Accessors designed to simplify porting from old cook code.
	/// Just becaues you have nInputs does not mean all are wired or
	/// cooked.  But, the number demanded by your OP_Operator shall
	/// be present.
	/// Inputs have been locked & preserve request set 
	exint			 nInputs() const { return myInputs.size(); }
	bool			 hasInput(exint idx) const
	{
	    if (idx < 0 || idx >= myInputs.size())
		return false;
	    return myInputs(idx).isValid();
	}
	const GU_Detail		*inputGeo(exint idx) const
	{
	    if (!hasInput(idx)) return 0;
	    return myInputs(idx).gdp();
	}
	GU_ConstDetailHandle	 inputGeoHandle(exint idx) const
	{
	    if (!hasInput(idx)) return GU_ConstDetailHandle();
	    return myInputs(idx);
	}

	template <typename T>
	const T			&parms() const
				{ return *UTverify_cast<const T*>(myParms); }

	/// Cache may, or may not, persist between cooks.  But it is
	/// guaranteed this is the only thread using the cache.
	SOP_NodeCache		*cache() const { return myCache; }

	UT_ErrorManager		*error() const { return myError; }
	DEP_MicroNode		*depnode() const { return myDepNode; }

	/// Methods to wire directly to the optional depnode.
	void			 addExplicitInput(DEP_MicroNode &inp, bool check_dup)
	{ if (myDepNode) myDepNode->addExplicitInput(inp, check_dup); }
	void			 addExplicitInput(DEP_MicroNode &inp)
	{ addExplicitInput(inp, /*check_dup*/true); }

	/// Methods to add directly to any present error manager.
	UT_ErrorSeverity addMessage (const char *type, int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ if (myError) return myError->addMessage(type, code, formatError(msg), loc); 
	  return UT_ERROR_MESSAGE; }
	UT_ErrorSeverity addWarning (const char *type, int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ if (myError) return myError->addWarning(type, code, formatError(msg), loc); 
	  return UT_ERROR_WARNING; }
	UT_ErrorSeverity addError   (const char *type, int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ if (myError) return myError->addError(type, code, formatError(msg), loc); 
	  return UT_ERROR_ABORT; }

	UT_ErrorSeverity sopAddMessage (int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ return addMessage("SOP", code, msg, loc); }
	UT_ErrorSeverity sopAddWarning (int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ return addWarning("SOP", code, msg, loc); }
	UT_ErrorSeverity sopAddError   (int code, const char *msg=0,
				     const UT_SourceLocation *loc=0) const
	{ return addError("SOP", code, msg, loc); }

	void		 select(GA_GroupType gtype = GA_GROUP_PRIMITIVE) const
	{ if (getNode())
	    getNode()->select(gtype); }
	
	void		 select(GU_SelectionHandle selection, 
				    bool add_to_sel = false) const
	{ if (getNode())
	    getNode()->select(selection, add_to_sel); }
	
			     // Select the group. opt use its type as sel type
	void		 select(const GA_Group &group, bool use_gtype = true,
				    bool add_to_sel = false) const
	{ if (getNode())
	    getNode()->select(group, use_gtype, add_to_sel); }
	
	void		 select(const GEO_Primitive &prim, bool sel_prim = true,
				    bool add_to_sel = false) const
	{ if (getNode())
	    getNode()->select(prim, sel_prim, add_to_sel); }
        void             selectPrimitive(GA_Offset primoff, bool prim_sel = true,
                                    bool add_to_sel = false) const
        { if (getNode())
            getNode()->selectPrimitive(primoff, prim_sel, add_to_sel); }
        void		 selectPoint(GA_Offset ptoff, bool point_sel = true,
				    bool add_to_sel = false) const
	{ if (getNode())
	    getNode()->selectPoint(ptoff, point_sel, add_to_sel); }
	void		 selectFrom(const GEO_Primitive &prim, bool sel_prim = true,
					bool add_to_sel = false) const
	{ if (getNode())
	    getNode()->selectFrom(prim, sel_prim, add_to_sel); }
	void		 selectPointsFrom(GA_Offset ptoff, bool point_sel = true,
					bool add_to_sel = false) const
	{ if (getNode())
	    getNode()->selectPointsFrom(ptoff, point_sel, add_to_sel); }
	void		 select(const GA_Range &range, bool use_rtype = true,
				    bool add_to_sel = false) const
	{ if (getNode())
	    getNode()->select(range, use_rtype, add_to_sel); }

	// Selects input based on the group supplied and the group type.  If
	// group is not null, then it's type is used, otherwise the group type
	// is used.
	void		 selectInputGroup(const GA_Group *group,
					      GA_GroupType grouptype) const
	{ if (getNode())
	    getNode()->selectInputGroup(group, grouptype); }

	// If no selection is present, then create a new empty primitive cook   
	// selection group. Otherwise, clear the selection.
	void 		 clearSelection() const
	{ if (getNode())
	    getNode()->clearSelection(); }

	/// If the cook selection group is not of the given type, create an empty
	/// cook selection group of that type. Otherwise, clear the existing cook
	/// selection group.
	void		 clearSelection(GA_GroupType gtype) const
	{ if (getNode())
	    getNode()->clearSelection(gtype); }
	
	// Blow away the selection and reinitialize it to 0. Much more radical
	// than clearSelection(), which leaves behind a clean, empty selection.
	// This method returns 1 if it had anything to destroy, else 0.
	bool		 destroySelection() const
	{ 	if (getNode())
		return getNode()->destroySelection(); 
	    return false;
	}

	// Return 1 if selection is enabled, 0 if false.
	bool		 selectionEnabled() const
			     {
				 if (!getNode())
				     return false;
				 return getNode()->getHighlight();
			     }

	/// Shims for local variables
	bool		setupLocalVars() const
	{ if (getNode()) return getNode()->setupLocalVars(); return false; }
	void		resetLocalVarRefs() const
	{ if (getNode()) getNode()->resetLocalVarRefs(); }
	void		setCurGdh(int index, const GU_DetailHandle &gdh) const
	{ if (getNode()) getNode()->setCurGdh(index, gdh); }
	void		clearCurGdh(int index) const
	{ if (getNode()) getNode()->clearCurGdh(index); }
	void		setCurPoint(int index, GA_Offset off) const
	{ if (getNode()) getNode()->setCurPoint(index, off); }
	void		clearCurPoint(int index) const
	{ if (getNode()) getNode()->clearCurPoint(index); }
	void		setCurVertex(int index, GA_Offset off) const
	{ if (getNode()) getNode()->setCurVertex(index, off); }
	void		clearCurVertex(int index) const
	{ if (getNode()) getNode()->clearCurVertex(index); }
	void		setCurPrim(int index, GA_Offset off) const
	{ if (getNode()) getNode()->setCurPrim(index, off); }
	void		clearCurPrim(int index) const
	{ if (getNode()) getNode()->clearCurPrim(index); }
	void		setCurVertexNum(int index, exint num) const
	{ if (getNode()) getNode()->setCurVertexNum(index, num); }
	void		clearCurVertexNum(int index) const
	{ if (getNode()) getNode()->clearCurVertexNum(index); }
	void		 setVariableOrder(int detail, int prim, 
					      int pt, int vtx) const
	{ if (getNode()) getNode()->setVariableOrder(detail, prim, pt, vtx); }

	/// Tracks if any local variables were accessed by op functions.
	void		 resetLocalVariableAccessed() const
	{ if (getNode()) getNode()->myUsesSOPLocalVar = false; }
	bool		 wasLocalVariableAccessed() const
	{ if (!getNode()) return false;
	  return getNode()->myUsesSOPLocalVar; }


    protected:
	/// Prefix errors so we can get sensible results.
	UT_StringHolder		formatError(const char *msg) const
	{
	    // Do not prefix with the path in traditional cook
	    // as it is properly assigned.
	    if (myCookEngine == SOP_COOK_TRADITIONAL)
		return msg;
	    UT_StringHolder	result;
	    if (!msg || !*msg)
		return result;
	    UT_WorkBuffer		buf;
	    UT_String			fullpath;
	    if (myNode)
		myNode->getFullPath(fullpath);
	    else
		fullpath = "<internal>";
	    buf.sprintf("%s: %s", (const char *) fullpath, msg);
	    result = buf;
	    return result;
	}

	bool					 myCookEngine;
	GU_DetailHandle				&myDestGdh;
	const UT_Array<GU_ConstDetailHandle>	&myInputs;
	SOP_Node				*myNode;
	const OP_Context			&myContext;
	const SOP_NodeParms			*myParms;
	SOP_NodeCache				*myCache;
	UT_ErrorManager				*myError;
	DEP_MicroNode				*myDepNode;
        // Only valid in traditional cook paths:
        fpreal					 myCookTime;
    };

    /// If doPartialInputCook is false, we do assume cookInputs() will
    /// apply to all inputs regardless of node parameters.  If local
    /// variables require the inputs, it is required for this to be false
    /// for the inputs to be available when parameters are evaluated.
    virtual bool		 doPartialInputCook() const { return false; }
    virtual bool		 cookInputs(const InputParms &parms) const
    {
	for (exint i = 0; i < parms.inputs().nInputs(); i++)
	    if (parms.inputs().hasInput(i))
	    {
		if (!parms.inputs().cookInput(i))
		    return false;
	    }
	return true;
    }

    /// These are for the old-cook path to allow nodes to store relevant
    /// local variable information inside the SOP for callbacks.  The
    /// evalVariableValue() should be guarded to ensure it doesn't
    /// run from a forbidden node.
    /// These can be used if the local variables depend only on the inputs
    /// and are not change for every parameter.  If they change for
    /// every parameter, traditional cook paths will need to setup &
    /// evaluate explicitly in the cook method.
    virtual void		setupLocalVariables(SOP_Node *sop, const UT_Array<GU_ConstDetailHandle> &inputs) const {}
    virtual void		resetLocalVariables(SOP_Node *sop) const {}

    /// Traditional nodes may evaluate parameters while cooking, this is
    /// required for dynamic local variables.  If this is true, no pre-cooking
    /// of the parameter struture will be done in the traditional cook
    /// path avoiding double cooking parameters.  Remember other cook paths
    /// will not have access to the sop so should use the parameter structure!
    virtual bool                evaluatesParametersDuringCook() const { return false; }

    virtual CookMode		 cookMode(const SOP_NodeParms *parms) const { return COOK_DUPLICATE; }

    /// Attributes can end up cached on the GPU.  Some SOPs are able
    /// to handle this. Some are ambivalent (such as switch or null)
    /// And some expect all GPU buffers to be flushed prior to
    /// operating.  If a node requires a CE flush, all inputs
    /// will be clean.
    virtual bool		 requiresCEFlush() const { return true; }
    virtual bool		 usesCE() const { return false; }
    
    /// Compute the output geometry.
    virtual void	cook(const CookParms &cookparms) const = 0;

    /// This is used to update handles, it only has an effect
    /// if oldsop is non-null, ie, we are in the old cook path.
    /// Neither the group nor the gdp have to persist beyond this call.
    /// Pass -1 for the grouptype if there is no grouptype parameter.
    void notifyGroupParmListeners(SOP_Node *oldsop,
			    int groupparm_idx,
			    int grouptype_idx,
			    const GU_Detail *gdp,
			    const GA_Group *group) const;
    
protected:
};

#endif
