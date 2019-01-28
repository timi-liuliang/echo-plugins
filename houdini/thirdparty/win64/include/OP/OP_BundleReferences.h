/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_BundleReferences.h ( OP Library, C++)
 *
 * COMMENTS:	For parameters which reference bundles, we have to keep track
 *		of the bundle so that we get notifications to rebuild 
 *		data (e.g., the shader string) properly.
 *
 *		This class manages such bundle references by the operator's
 *		parameters. It attempts to keep track of the bundle names
 *		for the referencing/dereferencing purposes
 *		and to register and unregister the interest with the bundles.
 */

#ifndef __OP_BundleReferences__
#define __OP_BundleReferences__

#include "OP_API.h"
#include <UT/UT_Defines.h>
#include <UT/UT_SmallObject.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>

class OP_BundleRefList;
class OP_Bundle;
class OP_Network;
class OP_Node;
template<typename> class UT_Array;


/// Implementation of a reference to a single bundle by a single parameter.
/// The parameter is identified by its name (token) and the vector
/// index into the parameter (for vector parms)
class OP_API OP_BundleRef : public UT_SmallObject<OP_BundleRef>
{
public:
    OP_BundleRef( const char *name, int v_index )
	: myParmName( name ), myVecIndex( v_index ), myUsedFlag( true ) {}

    // some accessors
    const UT_String&	getParmName() const	{ return myParmName; }
    int			getVectorIndex() const	{ return myVecIndex; }
    bool		isUsed() const		{ return myUsedFlag; }

private:
    UT_String	myParmName;	// parameter name (token)
    int		myVecIndex;	// sub-parm index into a vector parm
    bool	myUsedFlag;	// flags if a parm actually references
				//   a bundle: used to "mark and clear"

    // only OP_BundleRefList knows how to modify this class properly
    friend class OP_BundleRefList;
};


/// Implementation of a manager that overlooks all the references to
/// the bundles by a all parameters of an operator.
class OP_API OP_BundleReferences
{
public:
     OP_BundleReferences( OP_Node &owner);
    ~OP_BundleReferences();

    /// Obtains a bundle defined by an operator pattern and filter using
    /// the creator and myOwner as relative_to nodes (much like OP_BundleList),
    /// and then associates this bundle with a parameter defined by a name
    /// and index.
    ///
    /// The bundle references by this parameter are updated (ie, bundle
    /// previously referenced by this parm is dereferenced, and a reference
    /// count of the new bundle (which is returned) is bumped up.
    ///
    /// The also and interest is registered with the bundle for myOwner 
    /// node (much like OP_BundleList::getPattern() does.
    ///
    /// The bundle reference used flag is set to true for that parm/index pair.
    OP_Bundle *		 getParmBundle(const char* parm_name, int vector_index,
				UT_String &pattern, OP_Network *creator,
				const char *filter);

    /// Marks all the references as unused. Usually followed by cleanUnused()
    /// to delete the references that no longer exist.
    void		 markAllUnused();

    /// Deletes all references whose used flag is false. Usually preceeded
    /// by markAllUnused, and a series of getParmBundle to identify and delete
    /// obsolete references.
    void		 cleanUnused();

    /// Deletes all references.
    void		 cleanAll();

    /// Obtains a list of bundle references. Given a bundle name,
    /// this method returns all parameters that reference that bundle.
    const UT_Array<OP_BundleRef *> 
			*getListOfRefs( const char* bundle_name ) const;

    /// Estimates memory usage of the this object
    int64		 getMemoryUsage(bool inclusive) const;

    /// Obtains a bundle defined by an operator pattern with or without
    /// subnet_inclusion
    OP_Bundle *		 getParmBundle(const char* parm_name, int vector_index,
				UT_String &pattern, OP_Network *creator,
				const char *filter, bool subnet_inclusion);

private:
    /// map: bundle name -> list of referencing parameters 
    // Given a bundle name, finds all parameters that refer to this bundle.
    // On a bundle changed event, this map is used to find all referencing
    // parameters (and possibly invoke parm dirty on them).
    UT_StringMap<OP_BundleRefList *> myBundleToParmsMap;

    /// map: parameter -> name of a bundle 
    /// NB: this is a "one-to-one" mapping, where tag = name + vector index
    // Given a parameter tag, finds a (single) bundle this parameter references.
    UT_StringMap<UT_StringHolder> myTagToBundleMap;

    // the owner of this bundle reference manager
    OP_Node			&myOwner;
};
#endif

