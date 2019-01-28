/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_AttribMap_h__
#define __POP_AttribMap_h__

#include "POP_API.h"
#include <UT/UT_LinkList.h>
#include <GA/GA_AttributeRef.h>
#include <GA/GA_AttributeRefMap.h>
#include <GEO/GEO_Point.h>
#include <GEO/GEO_Vertex.h>
#include <GU/GU_Detail.h>

class UT_String;
class POP_AttribMap;
class POP_ContextData;
class GA_GBElement;
class GA_AttributeDict;
class GEO_WorkVertexBuffer;

/// The POP_AttribMap is a class used to manage attributes during particle
/// birthing.  Its primary role is tracking the attributes which are to be
/// inherited from the birth source, but it also manages the temporary
/// storage needed to evaluate attribute values from the source geometry.

class POP_API POP_AttribMapping : public UT_LinkNode
{
public:
    POP_AttribMapping()
	: myName(0)
    {
    }

    ~POP_AttribMapping()
    {
	if (myName)
	    free(myName);
    }

    GA_StorageClass	getType() const	{ return myType; }

    const GA_ROAttributeRef &getSourceOffset() const	{ return sourceOffset; }
    const GA_ROAttributeRef &getEvalOffset() const	{ return evalOffset; }
    const GA_RWAttributeRef &getDestOffset() const	{ return destOffset; }
    int		 getTupleSize() const		{ return myTupleSize; }

    /// @{
    /// Use POP_AttribMap::isEvalPoint() to determine which copy method to
    /// call.
    void	copySourceData(GA_GBElement &dest,
			    const GA_GBElement &src) const;
    void	copyEvalData(GA_GBElement &dest,
			    const GA_GBElement &eval) const;
    /// @}

private:
    void	setName(const char *name)	{ myName = strdup(name); }
    void	refreshSource(const GA_AttributeDict &point_dict);

    GA_ROAttributeRef	 sourceOffset;
    GA_ROAttributeRef	 evalOffset;
    GA_RWAttributeRef	 destOffset;
    int			 myTupleSize;
    GA_StorageClass	 myType;
    char		*myName;

    friend class POP_AttribMap;
};

class POP_API POP_AttribMap : public UT_LinkList
{
public:
             POP_AttribMap (void);
    virtual ~POP_AttribMap (void);

    /// Build the mapping between the source and destination details.  Note
    /// that no evaluation storage is allocated here.  We allocate this temp
    /// storage when needed to avoid paying the overhead when we don't need
    /// it and to give the caller an opportunity to request to specify any
    /// additional attributes that should be included (for local variable
    /// evaluation).
    void	build		(POP_ContextData* map, const GU_Detail* source,
				 GU_Detail* dest, const UT_String &pattern,
				 int addLocalVar = 0, bool refreshable = false);

    /// refreshSource() should only be called if the map was built to be
    /// refreshable by build().  Any extra source attributes registered
    /// for evaluation via registerExtraSrcEvalAttributeForEval() will
    /// have to be registered again.
    void	refreshSource(const GU_Detail *source);

    /// prepareForPossibleSourceDeletion() should be called before any action
    /// that may result in deleting our source detail.  A re-build or refresh
    /// is not a replacement for this call as the original source detail will
    /// often already be deleted by that point.  This method should typically
    /// be followed by a build(), refreshSource(), or
    /// recoverAfterSourceNotDeleted() call.
    void	prepareForPossibleSourceDeletion();

    /// This method should be called after prepareForPossibleSourceDeletion()
    /// if the source was not in fact deleted.
    void	recoverAfterSourceNotDeleted();

    /// @{
    /// AttributeRefMaps mapping attributes in the evaluation detail to the
    /// corresponding attributes in the source detail.  This is a subset of
    /// the source attributes consisting of those to be inherited by newly
    /// birthed particles and also those needed by the generator to evaluate
    /// attribute-based local variables.
    ///
    /// These methods will allocate the eval storage (detail) if it is not
    /// already available.
    GA_AttributeRefMap		&getEvalMap()
				{
				    ensureEvalStorageIsAvailable();
				    return myHandles;
				}
    GA_AttributeRefMap		&getEvalVectorMap()
				{
				    ensureEvalStorageIsAvailable();
				    return myVectorHandles;
				}
    /// @}

    /// @{
    /// Methods to query the maximum number of entries in the various
    /// evaluation maps without forcing an allocation.  Because these
    /// methods don't account for the overlap between the attributes
    /// we evaluate for inheritence and those we evaluate for local
    /// variables, the returned value may be greater than the actual
    /// entry counts of the evaluation maps would be after allocation.
    int				peekEvalMapMaxEntries() const
				{
				    if (myEvalDetail)
					return myHandles.entries();
				    else
				    {
					// myHandles will contain all the mapped
					// attributes, all the extra evaluation
					// attributes, and P.
					return length()
					    + mySrcExtraEvalAttributes.entries()
					    + 1;
				    }
				}
    int				peekEvalVectorMapMaxEntries() const
				{
				    if (myEvalDetail)
					return myVectorHandles.entries();
				    else
				    {
					return myMappedVectorEntries +
							myExtraVectorEntries;
				    }
				}
    /// @}

    const GA_RWAttributeRef	&getEvalNormalOffset()
				    { return (myEvalNormalOffset); }
    bool			hasSrcVelocity() const
				    { return(mySrcVelocityOffset.isValid()); }

    void		getSrcOrEvalNormal(UT_Vector3 &N,
					   GEO_PointP src);
    void		setSrcNormal(const UT_Vector3& N) { mySourceNml = N; }

    /// Will allocate the eval storage (detail) if it is not already available.
    GEO_Vertex		*getEvalVertex()
			    {
				ensureEvalStorageIsAvailable();
				return myEvalVertex;
			    }
    /// Will allocate the eval storage (detail) if it is not already available.
    GEO_PointP		getEvalPoint()
			    {
				ensureEvalStorageIsAvailable();
				return myEvalPoint;
			    }
    bool		 isEvalPoint(GEO_PointP elem) const
			    {
                                if (!elem)
                                    return !myEvalPoint;
                                if (!myEvalPoint)
                                    return false;
				return &elem->getIndexMap() == &myEvalPoint->getIndexMap() &&
                                    elem->getMapOffset() == myEvalPoint->getMapOffset();
			    }
    const GU_Detail	*getSrcDetail() const
			    { return mySrcDetail; }
    const GU_Detail	*peekEvalDetail() const
			    { return myEvalDetail; }

    /// Any extra attributes should be registered before the first call
    /// needing evaluation storage.  Any such attributes will need to be
    /// registered again after any build() or refreshSource() call.
    void		 registerExtraSrcAttributeForEval(
						    const GA_Attribute *attrib);

    inline void		 ensureEvalStorageIsAvailable()
			 {
			     if (!myEvalDetail)	// already initialized
				 initializeEvalStorage();
			 }

private:
    void		 initializeEvalStorage();

    const GU_Detail		*mySrcDetail;
    GU_Detail			*myEvalDetail;
    GA_AttributeRefMap		 myHandles;
    GA_AttributeRefMap		 myVectorHandles;
    // Special instance used to manage a vertex pool bound to myEvalDetail.
    // Necessary to preserve temporary vertex/point across refreshSource().
    GA_AttributeRefMap		 myVertexPoolWrapper;

    int				 myMappedVectorEntries;
    int				 myExtraVectorEntries;

    GA_ROAttributeRef		 mySrcNormalOffset;
    GA_ROAttributeRef		 mySrcVelocityOffset;

    GA_RWAttributeRef		 myEvalNormalOffset;

    /// A list of additional attributes from the source detail requiring
    /// evaluation during birthing.
    UT_ValArray<const GA_Attribute *>	 mySrcExtraEvalAttributes;

    GEO_WorkVertexBuffer	*myVertexBuffer;
    GEO_Vertex			*myEvalVertex;
    GEO_PointP			myEvalPoint;
    UT_Vector3			 mySourceNml;
};

#endif
