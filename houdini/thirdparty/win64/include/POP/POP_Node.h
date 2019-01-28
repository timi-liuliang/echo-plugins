/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_Node_h__
#define __POP_Node_h__

//#define POP_USE_GA_VECTORIZE

#include "POP_API.h"
#include "POP_ContextData.h"
#include "POP_Defines.h"
#include "POP_Error.h"
#include "POP_Guide.h"
#include "POP_ParticleIterator.h"
#include "POP_ParticleList.h"
#include "POP_Shared.h"

#include "POP_GBElement.h"

#include <GOP/GOP_Manager.h>
#include <GU/GU_DetailHandle.h>
#include <GA/GA_PageHandle.h>

#include <OP/OP_Network.h>

#include <UT/UT_Interrupt.h>
#include <UT/UT_NTStreamUtil.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Thread.h>
#include <UT/UT_Vector3.h>


class EV_EXPRESSION;
class GA_Detail;
class GA_PointGroup;
class GEO_Point;
class GU_RayIntersect;
class OP_OperatorTable;
class OP_OutputCodeParms;
class POP_AttribMap;
struct POP_SlideAttribs;

extern "C"
{
    SYS_VISIBILITY_EXPORT extern void newPopOperator (OP_OperatorTable* table);
};

/// @name Macros to aid in implementing individual parameter evaluation methods
/// Convenience macros typically used while implementing parameter evaluation
/// methods on subclasses of POP_Node.
/// @{
// TODO: These functions get called per point in POPs so we should really be
//       calling the eval.*T() versions instead and pass down the thread id.
#define FLOAT_PARM(name, idx, vi, t)    \
                return evalFloat(name, &myIndirect[idx], vi, t);
#define INT_PARM(name, idx, vi, t)      \
                return evalInt(name, &myIndirect[idx], vi, t);
#define STR_PARM(name, idx, vi, t)      \
                evalString(s, name, &myIndirect[idx], vi, t);
#define STR_PARM_NE(name, idx, vi, t)      \
                evalStringRaw(s, name, &myIndirect[idx], vi, t);
/// @}

SYS_DEPRECATED_PUSH_DISABLE()

#include <GA/GA_GBElement.h>

class GA_GBPoint;
class GA_PointGroup;

class POP_API GA_GBPointIterator
{
public:
    /// Iterate over all points
    SYS_DEPRECATED_HDK(13.0)
    explicit GA_GBPointIterator(const GA_Detail &gdp);
    SYS_DEPRECATED_HDK(13.0)
             GA_GBPointIterator(const GA_Detail &gdp, GA_Range::safedeletions);

    /// Iterate over all points from the given point till the last in the list
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointIterator(
	    const GA_Detail &gdp, const GA_GBPoint *first,
	    const GA_GBPoint *last);

    /// Iterate over all points in group (group is optional)
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointIterator(const GA_Detail &gdp, const GA_PointGroup *group);
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointIterator(const GA_Detail &gdp, const GA_PointGroup *group,
		       GA_Range::safedeletions);

    /// Iterate over pairs of points - groups are optional
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointIterator(
	    const GA_Detail &gdp1, const GA_PointGroup *group1,
	    const GA_Detail &gdp2, const GA_PointGroup *group2);

    /// Destructor
    SYS_DEPRECATED_HDK(13.0)
    ~GA_GBPointIterator();

    /// Return the current point (NULL if complete)
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP		&getPoint() const	{ return SYSconst_cast(myP1); }
    /// Return the next point (NULL if no valid point)
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP		&getNextPoint() const	{ return SYSconst_cast(myN1); }

    /// Return the current point from the 2nd detail (NULL if complete)
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP		&getPoint2() const	{ return SYSconst_cast(myP2); }
    /// Return the next point from the 2nd detail
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointP		&getNextPoint2() const	{ return SYSconst_cast(myN2); }

    /// Advance to next iteration
    SYS_DEPRECATED_HDK(13.0)
    void		advance();

    /// Increment operator calls advance() to move to the next element
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointIterator	&operator++()		{ advance(); return *this; }

    // No post inc as it is harmful.
private:
    void		 setNext();
    bool		 atEnd() const
			 {
			    return myI1.atEnd() ||
					(myI2.isValid() && myI2.atEnd());
			 }
    GA_Detail		*myG1, *myG2;
    GA_Iterator		 myI1, myI2;
    GA_GBPointP		myP1, myP2;	// Current points
    GA_GBPointP		myN1, myN2;	// Next points
};

// The no-cache variant of the iterator returns not pointers to
// real GEO_Point *, but always the same GEO_Point * that it just reuses
// This allows old-style code to iterate over points without having
// to construct a GEO_Point * table.
// Note the GEO_Point * will be invalid outside this iterator, so don't
// cache it!
template <typename GA_POINT_TYPE>
class GA_GBPointIteratorNoCacheConst
{
public:
    typedef typename GA_POINT_TYPE::DetailType	DetailType;
    typedef GA_POINT_TYPE			PointType;

    /// Iterate over all points in group (group is optional)
    SYS_DEPRECATED_HDK(13.0)
    explicit		GA_GBPointIteratorNoCacheConst(
				const DetailType &gdp,
				const GA_PointGroup *group = 0)
			    : myGdp(gdp)
			    , myIt(myGdp.getPointRange(group))
			    , myPoint(gdp.getPointMap(),
                                      myIt.atEnd() ? GA_INVALID_OFFSET
                                                   : *myIt)
			{
			}

    SYS_DEPRECATED_HDK(13.0)
			GA_GBPointIteratorNoCacheConst(
				const DetailType &gdp,
				GA_Range::safedeletions safe,
				const GA_PointGroup *group = 0)
			    : myGdp(gdp)
			    , myIt(myGdp.getPointRange(group), safe)
			    , myPoint(gdp.getPointMap(),
                                      myIt.atEnd() ? GA_INVALID_OFFSET
                                                   : *myIt)
			{
			}

    /// Return the current point (NULL if complete)
    // @{
    SYS_DEPRECATED_HDK(13.0)
    const PointType *	get() const
			{
			    if (atEnd())
				return 0;
			    return &myPoint;
			}
    SYS_DEPRECATED_HDK(13.0)
    const PointType *	operator->() const
			{
			    return get();
			}
    // @}

    SYS_DEPRECATED_HDK(13.0)
    bool		atEnd() const
			{
			    return myIt.atEnd();
			}

    /// Advance to next iteration
    // @{
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointIteratorNoCacheConst &
			operator++()
			{
			    advance();
			    return *this;
			}
    SYS_DEPRECATED_HDK(13.0)
    void		advance()
			{
			    ++myIt;
                            if (myIt.atEnd())
                                myPoint = PointType(myGdp.getPointMap(), GA_INVALID_OFFSET);
                            else
                                myPoint = PointType(myGdp.getPointMap(), *myIt);
			}
    // @}

private:

    const DetailType &	myGdp;
    GA_Iterator		myIt;
    PointType		myPoint;
};

template <typename GA_POINT_TYPE>
class GA_GBPointIteratorNoCache
			: private GA_GBPointIteratorNoCacheConst<GA_POINT_TYPE>
{
public:

    typedef GA_GBPointIteratorNoCacheConst<GA_POINT_TYPE>
						Super;

    typedef typename GA_POINT_TYPE::DetailType	DetailType;
    typedef GA_POINT_TYPE			PointType;

    /// Iterate over all points in group (group is optional)
    SYS_DEPRECATED_HDK(13.0)
    explicit		GA_GBPointIteratorNoCache(
				DetailType &gdp,
				const GA_PointGroup *group = 0)
			    : GA_GBPointIteratorNoCacheConst<GA_POINT_TYPE>(
				    gdp, group)
			{
			}

    SYS_DEPRECATED_HDK(13.0)
			GA_GBPointIteratorNoCache(
				DetailType &gdp,
				GA_Range::safedeletions safe,
				const GA_PointGroup *group = 0)
			    : GA_GBPointIteratorNoCacheConst<GA_POINT_TYPE>(
				    gdp, safe, group)
			{
			}


    /// Return the current point (NULL if complete)
    // @{
    SYS_DEPRECATED_HDK(13.0)
    PointType *		get()
			    { return const_cast<PointType *>(Super::get()); }
    SYS_DEPRECATED_HDK(13.0)
    const PointType *	get() const
			    { return Super::get(); }
    SYS_DEPRECATED_HDK(13.0)
    PointType *		operator->()
			    { return get(); }
    SYS_DEPRECATED_HDK(13.0)
    const PointType *	operator->() const
			    { return Super::get(); }
    // @}

    // Increment operator calls advance() to move to the next element
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPointIteratorNoCache &
			operator++()
			    { Super::advance(); return *this; }

    SYS_DEPRECATED_HDK(13.0)
    bool		atEnd() const
			    { return Super::atEnd(); }

};

SYS_DEPRECATED_POP_DISABLE()

/// @def GA_FOR_ALL_GPOINTS(gdp, point)
/// Iterate over all points.
#define GA_FOR_ALL_GPOINTS(gdp, point)	\
	for (GA_GBPointIterator it(*(gdp)); \
		(point = GEO_PointP(gdp, it.getPoint())); ++it)

/// @def GA_FOR_ALL_GROUP_POINTS_NC(gdp, grp, GEO_Point, point)
/// Iterate over all points in group.  @see GA_RTIElementGroup
/// The point will be reused for all the iterations, so do not cache
/// the resulting pointer!
#define GA_FOR_ALL_GROUP_POINTS_NC(gdp, grp, PtType, pt)	\
	for (GA_GBPointIteratorNoCache<PtType> pt(*(gdp),grp); !pt.atEnd(); ++pt)

/// @def GA_FOR_ALL_GROUP_POINTS(gdp, grp, point)
/// Iterate over all points in group.  @see GA_RTIElementGroup
#define GA_FOR_ALL_GROUP_POINTS(gdp, grp, point)	\
	for (GA_GBPointIterator it(*(gdp), grp); \
		(point = GEO_PointP(gdp, it.getPoint())); ++it)

#define GA_FOR_ALL_GROUP_POINTS_NC_CONST(gdp, grp, PtType, pt)	\
	for (GA_GBPointIteratorNoCacheConst<PtType> pt(*(gdp),grp); !pt.atEnd(); ++pt)

/// @def GA_FOR_ALL_OPT_GROUP_POINTS_NC(gdp, grp, GEO_Point, point)
/// Iterate over all points in group, but the group can be NULL.
/// @see GA_RTIElementGroup
/// The point will be reused for all the iterations, so do not cache
/// the resulting pointer!
#define GA_FOR_ALL_OPT_GROUP_POINTS_NC_CONST(gdp, grp, PtType, point) \
	    GA_FOR_ALL_GROUP_POINTS_NC_CONST(gdp, grp, PtType, point)

/// @def GA_FOR_PAIRS_OF_GROUP_POINTS(gdp1, group1, point1, gdp2, group2, point2)
/// Iterate over points in the specified groups across both GA_Details in a
/// pairwise fashion.
#define GA_FOR_PAIRS_OF_GROUP_POINTS(gdp1, grp1, point1, gdp2, grp2, point2) \
	for (GA_GBPointIterator it(*(gdp1), grp1, *(gdp2), grp2); \
		(point1=GEO_PointP(gdp1, it.getPoint())) && \
		(point2=GEO_PointP(gdp2, it.getPoint2())); ++it)

/// Wrapper to easily mimic old GA_GBElement interface for primitives,
/// now that they're not GA_GBElement objects.
class GA_GBPrimWrap : public GA_GBElement
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPrimWrap()
        : GA_GBElement()
    {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPrimWrap(const GA_Primitive *prim)
        : GA_GBElement(prim->getIndexMap(), prim->getMapOffset())
    {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPrimWrap(const GA_Primitive &prim)
        : GA_GBElement(prim.getIndexMap(), prim.getMapOffset())
    {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBPrimWrap &operator=(const GA_GBPrimWrap &that)
    {
        GA_GBElement::operator=(that);
        return *this;
    }
    GA_GBPrimWrap *operator->()
    { return this; }
    const GA_GBPrimWrap *operator->() const
    { return this; }
};

/// Prototype for function that sets attributes after points are birthed.
///     @param data	 POP_ContextData being used
///     @param ppt	 particle's point
///     @param vel	 particle velocity
///     @param userdata  passed in data
typedef void (POP_Node::*POP_BirthAttribFunc)
		 (POP_ContextData* data, GEO_PointP ppt, UT_Vector3* vel, 
		  void* userdata);

/// Prototype for function that returns a floating point value.
/// Function returns the value.
///     @param data     POP_ContextData being used. Needed for random num
///                     generation.
typedef float (POP_Node::*POP_FloatFunc)(POP_ContextData* data);

/// Prototype for function that returns a integer value.
/// Function returns the value.
///     @param data     POP_ContextData being used. Needed for random num
///			generation.
typedef int (POP_Node::*POP_IntFunc)(POP_ContextData* data);

/// Prototype for function that can be called per node for POP_Node::traverse()
/// and POP_Node::pseudocook().
typedef void (POP_Node::*POP_TraverseFunc)(void* userdata);

/// Prototype for function that can be called per node to allow jitter
/// births. Modifies the passed in values to update gdp, source groups,
/// intersectors (if necessary), and attribute mappings (if necessary).
typedef void (POP_Node::*POP_JitterFunc)(GU_Detail **gdp,
		 GA_PointGroup **pointGroup, GA_PrimitiveGroup **primGroup,
		 GU_RayIntersect **rayIntersect, POP_AttribMap **map,
		 float timeIncFraction);

/// A PtrArray which stores pointers to POP_ParticleList.
typedef UT_ValArray<POP_ParticleList*> POP_ParticleListArray;

//-----------------------------------------------------------------------------

// Parameter evaluation 

/// @name Pointers to evaluation methods
/// @{
typedef float (POP_Node::*POP_FParam)(float t);
typedef int   (POP_Node::*POP_IParam)(float t);
typedef void  (POP_Node::*POP_SParam)(UT_String &s, float t);
typedef void  (POP_Node::*POP_VParam)(EV_EXPRESSION* expr, 
				      float &x, float& y, float &z, int thread);
/// @}

/// @name Macros to aid in efficient per-particle parameter evaluation
/// @{

/// @brief Use in class definition to declare members for caching non-variable
/// dependent values.

/// Declares a member variable @c myvar to cache a parameter value and an
/// evaluator method @c to return that value.
#define POP_FPARM(myvar, get) \
	float	myvar; \
	float	get(float) { return(myvar); }

#define POP_IPARM(myvar, get) \
	int	myvar; \
	int	get(float) { return(myvar); }

#define POP_SPARM(myvar, get) \
	UT_String	myvar; \
	void		get(UT_String& s, float) { s = myvar; }

#define POP_VPARM(myvar, get) \
	float	myvar[3]; \
	void	get(EV_EXPRESSION*, float &x, float &y, float &z, int) \
		{ x = myvar[0]; y = myvar[1]; z = myvar[2]; }

/// @name Macros to aid in efficient per-particle parameter evaluation
/// @{

/// @brief Use during cook to select between a per-particle or per-cook
/// evaluation method.

/// Calls the parameter evaluation method @c eval, and if the evaluation is
/// dependent on any per-particle variables, sets the method pointer @c var
/// to point to @c eval.  Otherwise it caches the value in the member variable
/// @c myvar and sets @c var to point to @c get, the method that simply returns
/// said cached value.
#define POP_FCACHE(var, eval, get, myvar, classname) \
        { \
	if (isVarDependent(data->getDetail(), \
			   (POP_FParam) &classname::eval, t, thread)) \
	    var = (POP_FParam) &classname::eval; \
	else \
	    { myvar = eval(t); var = (POP_FParam) &classname::get; } \
        }

#define POP_ICACHE(var, eval, get, myvar, classname) \
        { \
	if (isVarDependent(data->getDetail(), \
			   (POP_IParam) &classname::eval, t, thread)) \
	    var = (POP_IParam) &classname::eval; \
	else \
	    { myvar = eval(t); var = (POP_IParam) &classname::get; } \
        }

#define POP_SCACHE(var, eval, get, myvar, classname) \
        { \
	if (isVarDependent(data->getDetail(), \
			    (POP_SParam)  &classname::eval, t, thread)) \
	    var = (POP_SParam) &classname::eval; \
	else \
	    { eval(myvar, t); var = (POP_SParam) &classname::get; } \
        }

#define POP_VCACHE(var, expr, get, myvar, classname) \
        { \
	if (isVarDependent(data->getDetail(), expr, thread)) \
	    var = &classname::evalVector; \
	else \
	    { evalVector(expr, myvar[0], myvar[1], myvar[2], thread); \
	      var = (POP_VParam) &classname::get; } \
        }
/// @}

/// @name Macros to aid in efficient per-particle parameter evaluation
/// @{

/// @brief Use during cook to call a parameter evaluation method pointer.
#define POP_PEVAL(name) \
        (this->*name)(t)

#define POP_SEVAL(name, s) \
        (this->*name)(s, t)

#define POP_VEVAL(name, expr, x, y, z, thread) \
        (this->*name)(expr, x, y, z, thread)

/// @brief Use during cook to call a parameter evaluation method pointer during
/// a callback.
#define POP_CB_PEVAL(name) \
        (this->*name)(data->myTime)

/// @brief Use during cook to call a parameter evaluation method pointer during
/// a callback.
#define POP_CB_SEVAL(name, s) \
        (this->*name)(s, data->myTime)
/// @}

//-----------------------------------------------------------------------------

class POP_API POP_Node : public OP_Network
{
public:
    static const char	*theChildTableName;
    virtual const char  *getChildType() const;
    virtual const char  *getOpType() const;

    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;

    static void		 buildOperatorTable (OP_OperatorTable& table);

    virtual int		 getNetOverviewColor (UT_Color& color);

    /// traverse() is used to traverse all nodes and perform some method 
    /// one each node.
    void		 traverse     (POP_TraverseFunc traverse, 
				       void* userdata = 0);

    /// pseudocook() is used to traverse all nodes and perform some method 
    /// one each node in the cook chain.
    void		 pseudocook   (POP_TraverseFunc traverse, 
				       void* userdata = 0);

    /// preCook() and postCook() performs pre and post cook processing.  They 
    /// are called by POP_ContextData::cook() and POP_ContextData::cookGuide().
    void		 preCook   (OP_Context& context);
    void		 postCook  (OP_Context& context);

    /// computeNetChecksum() returns a CRC checksum which is "unique"
    /// for a particular network topology with a particular set of parameters.
    unsigned long 	 computeNetChecksum (void);

    /// addAllAttribs() traverses the nodes that will be cooked and
    /// adds the required attributes to the gdp by calling addAttrib().
    void		 addAllAttribs     (POP_ContextData* data);

    /// getParticleList() acts as an access method to get at this POP's
    /// particle list. It just returns this POP's POP_ParticleList and 
    /// assumes that this structure is already up to date. Use 
    /// buildParticleList() to build an up-to-date particle list.
    POP_ParticleList* 	 getParticleList (void) { return(&myParticleList); };

    /// timestep() is used to perform timestepping. (i.e. move particles,
    /// update state, etc). This function can be overridden to perform a
    /// custom integration step. The default implementation calls the static
    /// timestepStatic function. Note that this function is only called on
    /// the cook POP, not every POP in the network.
    virtual void	 timestep(OP_Context &context);
    /// Standard particle timestep function.
    static void		 timestepStatic(OP_Context &context);

    /// prepParticles() prepares particles for the current frame by resetting
    /// the particle's state.
    static void		 prepParticles	(OP_Context &context);

    /// applyRulesToList() is really the heart of timestep. It actually applies
    /// the appropriate rules to each particle.
    static void		 applyRulesToList( POP_ParticleList *particleList,
					   POP_ContextData *data );

    /// reapParticles() removes dead particles from the system.
    static void		 reapParticles	(OP_Context &context);

    void		 warnDisplayFlag   (void);
    void		 unwarnDisplayFlag (void);

    virtual int		 isRefInput (unsigned) const { return 0; }

    /// @{
    /// These functions are used by lots of POPs to generate obj/sop menus
    static int		filterForceObjs(OP_Node *node);
    static void         buildForceObjMenu (void *data, PRM_Name* menu,
				      int maxSize,
				      const PRM_SpareData* , const PRM_Parm *);
    static void         buildObjMenu (void *data, PRM_Name* menu,
				      int maxSize,
				      const PRM_SpareData* , const PRM_Parm *);
    static void         buildSopMenu (void *data, PRM_Name* menu,
				      int maxSize,
				      const PRM_SpareData*, const PRM_Parm *);
    /// @}

    virtual void 	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    virtual void	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms);

    // Absolute Width
    virtual fpreal       getW() const;
    // Absolute Height
    virtual fpreal       getH() const;  

    /// This function provides a whole bunch of slightly different functionality
    /// to each of a bunch of different POPs (thus the complex signature).
    /// Any POP which accesses a piece of SOP geometry should use this function
    /// to get the geometry. It supports DOP geometry paths as well.
    /// The first parameter is the path to get the gometry from. If NULL, the
    /// data and t parameters are used to evaluate parameters on the POP to
    /// get the path. addextrainput tells it whether to add an extra input
    /// to the POP for when the SOP changes. ignorexform tells this function
    /// to ignore any transform on the object containing the SOP (or DOP).
    /// copygdp provides a place to stores a transformed version of the
    /// geometry if required. This and all subsequent parameter can be NULL
    /// if the information isn't required (such as when the geometry is
    /// being accessed just to get a list of available group names). The
    /// prevsop parameter can point to the SOP that was last retrieved. It's
    /// value is updated by this function, after doing a comparison to
    /// see if the value has changed. retdoxform and retxform return whether
    /// a transform is required on the geometry, and what that transform is.
    /// Finally, retneedsreset helps POPs keep track of whether anything has
    /// changed since the last call to this function. This takes into account
    /// changes to the prevsop, whether or not the geometry has to be xformed,
    /// and the value passed in through retneedsreset.
    GU_Detail		*getGeo(const char *overridegeopath,
				const POP_ContextData *data, fpreal t,
				int addextrainput, int ignorexform,
				GU_Detail *copygdp, OP_Node **prevsop,
				int *retdoxform, UT_DMatrix4 *retxform,
				int *retneedsreset);

protected:
	     POP_Node (OP_Network* parent, const char* name, 
		       OP_Operator* entry);
    virtual ~POP_Node (void);

    virtual OP_ERROR	 cookPop  (OP_Context &context) = 0;

    // This will change a POPobjName POPsopName pair into a POPfullsopName
    // You can override what it applies to with the specified overrides.
    // If soptoken is "" it will only convert the object.
    // Note it looks for the objtoken and soptoken inside the obsolete
    // list and the desttoken inside the ParmList.
    // The relative is 1 if the new path should be relative to this, and
    // 0 if it should be absolute.
    void		 applyBackwardsPathCompatibility(
				    PRM_ParmList *obsolete_parms,
				    const char *objtoken = 0, 
				    const char *soptoken = 0,
				    const char *desttoken = 0,
				    int relative = 1);

    virtual void	 getOperationInfo(UT_String &info, OP_Context &context);

    /// Add any attributes required by this POP to the detail in the supplied
    /// POP_ContextData (pointer passed as void *).  This method is called by
    /// addAllAttribs().
    virtual void	 addAttrib (void*) {};

    /// @name Add standard error/warning
    /// @{
    void		 addError(POP_ErrorCodes code, const char* msg = 0)
			 { UTaddError("POP", code, msg); }
    void		 addMessage(POP_ErrorCodes code, const char* msg = 0)
			 { UTaddMessage("POP", code, msg); }
    void		 addWarning(POP_ErrorCodes code, const char* msg = 0)
			 { UTaddWarning("POP", code, msg); }
    void		 addFatal(POP_ErrorCodes code, const char* msg = 0)
			 { UTaddFatal("POP", code, msg); }
    /// @}

    /// getInputParticleList() returns a linked list of all of the
    /// particle system primitives used by a particular input. It will cook the
    /// input POP as necessary.
    POP_ParticleList* 	 getInputParticleList (int idx, OP_Context& context);

    // cacheForDisplay() caches the current gdp for display purposes if
    // necessary.
    void		 cacheForDisplay (OP_Context &context);

    /// checkActivation() returns 1 if the POP should be active and 0
    /// otherwise.
    int			 checkActivation (POP_ContextData* data, 
					  POP_FParam activate);

    /// doUpdateViewport() indicates whether the POP needs to perform
    /// updates for the viewport (like guide geometry for example). 
    int			 doUpdateViewport (POP_ContextData* data);

    /// @name Add standard attribute

    /// @{
    /// add___Attrib() is used to add attributes to the detail if
    /// they don't exist already. 
    void	 addAlphaAttrib       (POP_ContextData* data);
    void	 addAngVelAttrib      (POP_ContextData* data);
    void	 addAttractAttrib     (POP_ContextData* data);
    void	 addBounceAttrib      (POP_ContextData* data);
    void	 addComAttrib         (POP_ContextData* data);
    void	 addChargeAttrib      (POP_ContextData* data);
    void	 addClingAttrib       (POP_ContextData* data);
    void	 addDiffuseAttrib     (POP_ContextData* data);
    void	 addDistanceAttrib    (POP_ContextData* data);
    void	 addDragAttrib        (POP_ContextData* data);
    void	 addFollowAttrib      (POP_ContextData* data);
    void	 addFDynamicAttrib    (POP_ContextData* data);
    void	 addFStaticAttrib     (POP_ContextData* data);
    void	 addGenAttrib         (POP_ContextData* data);
    void	 addHitDiffuseAttrib  (POP_ContextData* data);
    void	 addHitIDAttrib       (POP_ContextData* data);
    void	 addHitNormalAttrib   (POP_ContextData* data);
    void	 addHitForceAttrib    (POP_ContextData* data);
    void	 addHitPosAttrib      (POP_ContextData* data);
    void	 addHitPosUVAttrib    (POP_ContextData* data);
    void	 addHitTimeAttrib     (POP_ContextData* data);
    void	 addHitUVAttrib       (POP_ContextData* data);
    void	 addIDAttrib          (POP_ContextData* data);
    void	 addInstanceAttrib    (POP_ContextData* data);
    void	 addMassAttrib        (POP_ContextData* data);
    void         addNearestAttrib     (POP_ContextData* data);
    void         addNearestDistAttrib (POP_ContextData* data);
    void	 addNumHitAttrib      (POP_ContextData* data);
    void         addNumProximityAttrib(POP_ContextData *data);
    void	 addOrbitAttrib       (POP_ContextData* data);
    void	 addOrbitAxisAttrib   (POP_ContextData* data);
    void	 addOrbitCenterAttrib (POP_ContextData* data);
    void	 addOrbitRadiusAttrib (POP_ContextData* data);
    void	 addOrbitSpeedAttrib  (POP_ContextData* data);
    void	 addOrientAttrib      (POP_ContextData* data);
    void	 addParentAttrib      (POP_ContextData* data);

    void	 addPosPathAttrib     (POP_ContextData* data);

    void	 addPosPrimAttrib     (POP_ContextData* data);
    void	 addPosUVAttrib       (POP_ContextData* data);
    void	 addPrevVelAttrib     (POP_ContextData* data);
    void	 addOriginAttrib      (POP_ContextData* data);
    void	 addRotAttrib         (POP_ContextData* data);
    void	 addScaleAttrib       (POP_ContextData* data);
    void	 addScale3Attrib      (POP_ContextData* data);
    void	 addSpeedAttrib       (POP_ContextData* data);
    void	 addSpeedLimitAttrib  (POP_ContextData* data);
    void	 addSpringKAttrib     (POP_ContextData* data);
    void	 addSpriteRotAttrib   (POP_ContextData* data);
    void	 addSpriteScaleAttrib (POP_ContextData* data);
    void	 addSpriteShopAttrib  (POP_ContextData* data);
    void	 addSpriteUVAttrib    (POP_ContextData* data);
    void	 addTensionAttrib     (POP_ContextData* data);
    void	 addTorqueAttrib      (POP_ContextData* data);
    void	 addTransAttrib       (POP_ContextData* data);
    void	 addUpAttrib          (POP_ContextData* data);
    /// @}

    /// addUserVariable() is used to add user defined attributes to
    /// the detail. It also takes care of adding local variables.
    GA_RWAttributeRef	addUserVariable(POP_ContextData *data,
				  const char* name, int tuplesize, 
				  GA_StorageClass type,
                                  GA_Defaults def, const char* local = 0)
		 {return(data->addUserVariable(name, tuplesize, type, def, local));}

    /// @{
    /// buildParticleList() is used to build the list of particle
    /// primitives that the POP should process.
    OP_ERROR		 buildParticleList (OP_Context &context,
					    int checkForDuplicates = 0);
    OP_ERROR		 buildParticleList (POP_ParticleList& buildList,
					    OP_Context &context,
					    int checkForDuplicates = 0);
    OP_ERROR		 buildParticleList (int input, OP_Context &contexti);
    OP_ERROR		 buildParticleList (POP_ParticleList& buildList,
					    int input, OP_Context &context);
    /// @}

    /// initParticleList() initializes the particle list with 
    /// the particle primitive. Used by generators.
    void		 initParticleList (OP_Context &context, 
					   GEO_PrimParticle* part = NULL);

    // a helper method that adds primitves form src_list to dest_list
    // INPUTS:
    //   src_list - list whose particles to add to tdest_list
    //   pop - pop operator that is an owner of src_list (used for error checks)
    //   input_index - if pop is an input operator, then this argument is
    //                its index. If pop is a generator or a non-input, use -1
    //   contest - context at which particles are added to dest_list
    //   checkForDuplicates - if true, particles from src_list are checked 
    //                if there is already such particle in dest_list
    // RETURNS: 
    //	the error 
    OP_ERROR		 addListToParticleList( POP_ParticleList& dest_list,
					POP_ParticleList& src_list,
					const POP_Node *pop, int input_index,
					OP_Context &context, 
					int checkForDuplicates);

    // addToParticleList() adds a primitive to the particle list.
    int 		 addToParticleList (OP_Context &context,
					    GEO_PrimParticle* part, int idx);

    // buildParticleListArray() is used to build an array of pointers
    // to particle lists containing the particle primitives that the
    // POP should process.  No inputs does NOT return an error.
    // Only valid if no error is returned.
    // Parameters:
    //   array - a reference to the POP_ParticleListArray to append to
    //   num - will be set to the number of particle lists appended
    //   context - the context
    //   checkForDuplicates - a flag indicating whether to check for
    //                        duplicate primitives.
    OP_ERROR		buildParticleListArray(POP_ParticleListArray& array,
					       int& num, OP_Context &context,
					       int checkForDuplicates = 0);

    /// birthParticle() should be used to birth particles into a particle
    /// system
    ///  @param data	 POP_ContextData being used
    ///  @param part	 particle primitive to birth into
    ///  @param pos	 position of the birthed point
    ///  @param map	 mapping for inheriting attributes from the inherit
    ///			 attribute data.
    ///  @param inherit	 inherit general attributes (required when map is used)
    ///  @param split	 inherit parental attributes (parent, origin, 
    ///			 generation)
    ///  @param originIndex   origin index value.
    ///  @param originGeo     origin from geometry number	
    ///	 @param originUse     how to set origin id. Bit mask
    ///			    - POP_ORIGIN_INDEX: add passed in index
    ///			    - POP_ORIGIN_GEO: add source geometry number
    ///			    - POP_ORIGIN_PARENT: add split origin
    ///  @param lifetime	total expected life of the particle
    ///  @param birthAttrib	function to set attributes after particle is
    ///				birthed
    ///  @param userdata	passed into birthAttrib
    GA_Offset		 birthParticle (POP_ContextData* data, 
					GEO_PrimParticle* part, 
					const UT_Vector4& pos, 
					POP_AttribMap* map,
				        GEO_PointP inherit,
				        GA_Offset splitoff,
				        POP_IntFunc originIndex, int originGeo,
					int originUse, POP_FloatFunc lifetime,
					POP_BirthAttribFunc birthAttrib = NULL,
					void* userData = NULL);

    // initParticleAttrib() handles setting some attributes when birthing a 
    // particle. These include ID, origin, parent and generation.
    void		 initParticleAttrib (POP_ContextData* data, 
					     GA_Offset ptoff,
				             GA_Offset splitoff,
					     float age, float expected,
                                             POP_IntFunc originIndex,
                                             int originGeo, int originUse);

    /// @name Standard update rules
    /// @{

    /// applyDefault___Rules() applies the default rules for that
    /// attribute to the point.
    static void		 applyDefaultPosRules (GA_Offset ptoff, 
					       POP_ContextData* data, 
					       int* state);
    static void		 applyDefaultVelRules (GA_Offset ptoff, 
					       POP_ContextData* data, 
					       int* state);
    static void		 applyDefaultRotRules (GA_Offset start, 
					      GA_Offset end,
					       POP_ContextData* data, 
					       int* state);
    static void		 applyDefaultAngVelRules (GA_Offset start, 
					      GA_Offset end,
					       POP_ContextData* data, 
					       int* state);
    static void		 applyDefaultUpRules (GA_Offset start, 
					      GA_Offset end,
					      POP_ContextData* data, 
					      int* state);
    static void		 applyDefaultSpeedRules (GA_Offset start,
						GA_Offset end, 
					      POP_ContextData* data);
    static void		 applyDefaultSlideRules (GA_Offset start,
						 GA_Offset end, 
					       POP_ContextData* data, 
					       int* state,
					       int globalsuppress);
    /// @}

    // Methods for handling sliding particles
    static void		 setupStateForSlidingParticles();
    static void		 cleanupStateForSlidingParticles();
    static void		 preProcessSlidingParticle(
					GA_Offset ptoff,
					POP_ContextData *data,
					POP_SlideAttribs &attribs,
					int *state,
					UT_Vector3 &nml,
					UT_Vector3 &surface_vel);
    static void		 postProcessSlidingParticle(
					GA_Offset ptoff, 
					POP_ContextData *data, 
					POP_SlideAttribs &attribs,
					int *state,
					UT_Vector3 &nml,
					UT_Vector3 &initial_pos,
					const UT_Vector3 &surface_vel);

    static void		 getPosFromReferencedSop(
					UT_Vector4 &pos,
					GA_Offset ptoff, 
				        POP_ContextData *data);

    static void		 getPosFromPrimUV(
					POP_ContextData *data,
					const char *sop_path,
					int primnum,
					const UT_Vector2 &uv,
					GA_Offset ptoff,
					UT_Vector4 &pos);

    /// Add guide geometry for accelerations to myGuide.
    void 		 addAccelGuide (GEO_PointP ppt, float ax, float ay, 
	                                float az, float scale = 1.0f, 
					int normalize = 0);
    void 		 addAccelGuide (const GA_Detail *gdp, GA_Offset ptoff,
					float ax, float ay, float az,
					float scale = 1.0f, 
					int normalize = 0);

    /// Change the string into a valid group name.  A return value of true
    /// indicates that the string needed changing, in which case severity
    /// specifies the level of error this method generated (UT_ERROR_NONE,
    /// UT_ERROR_WARNING, or UT_ERROR_ABORT).
    /// Note that this does not force the name to be non-zero in length.
    bool		 forceValidGroupName(UT_String &name,
					     UT_ErrorSeverity severity);
    /// Change the string into a valid group prefix.  A return value of true
    /// indicates that the string needed changing, in which case severity
    /// specifies the level of error this method generated (UT_ERROR_NONE,
    /// UT_ERROR_WARNING, or UT_ERROR_ABORT).
    /// Note that this does not force the prefix to be non-zero in length.
    bool		 forceValidGroupPrefix(UT_String &name,
					       UT_ErrorSeverity severity);

    /// @name Get group corresponding to supplied pattern
    /// @{
    const GA_PointGroup	*parsePointGroups (const char* pattern,
                                           GU_Detail* pgdp=0,
                                           int allow_numeric = 1);
    const GA_PrimitiveGroup	*parsePrimitiveGroups (const char* pattern,
                                           GU_Detail* pgdp=0,
                                           int allow_numeric = 1);
    GA_PointGroup	*parsePointGroupsCopy(const char* pattern,
                                           GU_Detail* pgdp=0,
                                           int allow_numeric = 1);
    GA_PrimitiveGroup	*parsePrimitiveGroupsCopy(const char* pattern,
                                           GU_Detail* pgdp=0,
                                           int allow_numeric = 1);
    /// @}

    /// evalVector() evaluates a vector expression and places the results
    /// in the x, y and z parameters.
    void                 evalVector (EV_EXPRESSION* expr,
                                     float &x, float &y, float &z,
				     int thread);

    /// @name Test a parameter for references to per-particle variables
    /// @{
    virtual int          isVarDependent (GU_Detail* gdp, POP_FParam param,
					 float t, int thread);
    virtual int          isVarDependent (GU_Detail* gdp, POP_IParam param,
					 float t, int thread);
    virtual int          isVarDependent (GU_Detail* gdp, POP_SParam param,
					 float t, int thread);
    virtual int          isVarDependent (GU_Detail* gdp, EV_EXPRESSION* expr,
					 int thread);
    /// @}

    /// @brief Control whether parameter evalutions are allowed to reference
    /// local variables.
    ///
    /// Derived classes that implement local variables will generally want to
    /// override this virtual.
    virtual void	 allowVar (int val);

    void	 	 traverseNode (POP_TraverseFunc traverse,
				       void* userdata = 0);

    void		 flagTraverse 	(void* userdata);
    virtual void	 doPreCook 	(void* userdata);
    void	 	 updateChecksum (void* userdata);

    // For nodes that include a geometry (SOP/DOP) path, this function
    // returns the parameter name that contains the path. This function
    // is used by getGeoPath().
    virtual const char	*getGeoPathParmName() const;
    // Fetches the geometry path to be used by this node. This function
    // either returns the appropriate context geometry path, or the value
    // of the parm specified in getGeoPathParmName(). This function is
    // used by getGeo().
    void		 getGeoPath(UT_String &geopath,
				    const POP_ContextData *data,
				    fpreal t);

    // Obtain the world transform with optional reference object.
    // If either of them is NULL, then we omit using it. This method will also
    // add transform errors if encountered.
    bool		 getXformToReference(OP_Context &context,
					     OP_Node *target,
					     OP_Node *reference,
					     UT_DMatrix4 &mat);

    /// This method returns the previous context data used to cook this
    /// node, or the one used to cook the first input if we don't have 
    /// one cached.
    const POP_ContextData	*getPrevContextData() const;

    static PRM_ChoiceList	pointGroupMenu;
   
    static void		 buildPointGroupMenu(void* data, PRM_Name* menu,
					     int maxSize, const PRM_SpareData*,
					     const PRM_Parm *);
    static void		 buildGenericGroupMenu(const GA_ElementGroupTable *table,
					       PRM_Name *menu, int maxSize,
					       int &index);

    static PRM_ChoiceList	generatorMenu;

    static void		 buildGeneratorMenu(void* data, PRM_Name* menu,
					    int maxSize, const PRM_SpareData*,
					    const PRM_Parm *);

    POP_ParticleList	 myParticleList;

    /// The guide handle owns the guide detail and permits indirect (external)
    /// references to the POP_Guide.  Within the context of this node, we can
    /// safely use myGuide as a convenient way to avoid casting.
    ///
    /// @warning Preserve requests are ignored.
    GU_DetailHandle	 myGuideHandle;
    POP_Guide		*myGuide;

    GOP_Manager	 	 myGroupParse;

private:

    virtual OP_ERROR	 cookMe       (OP_Context &context);
    virtual OP_ERROR	 bypassMe     (OP_Context &context, int &copied_input);

    virtual OP_DataType	 getCookedDataType (void) const;
    virtual void	 deleteCookedData (void);

    virtual int		 saveCookedData (const char* filename, OP_Context &);
    virtual int		 saveCookedData (std::ostream &os, OP_Context &,
					 int binary = 0);
    virtual const char*  getFileExtension (int binary) const;

    virtual const char	*getCookTimerLabel(const OP_Context &context) const;

    virtual int 	 doDeleteRewire (void) const;

    static void          initializeExpressions (void);

    GU_Detail*		 myPrevDetail;
    POP_ContextDataHandle	 myPrevContextData;
    UT_String		 myInfoText;
    int			 myWarnDisplayFlag;
    int			 myTraverse;
    int			 myPseudoCooking;
};

// Function to get a list of template arrays containing parm names that must
// be reserved when compiling VEX POPs.
POP_API extern void
POPgetVexReservedTemplateLists(UT_ValArray<PRM_Template *> &tplatelists);

#endif
