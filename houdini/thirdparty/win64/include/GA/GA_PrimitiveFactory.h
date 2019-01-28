/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveFactory.h ( GA Library, C++)
 *
 * COMMENTS:
 * 	A GA_PrimitiveFactory manages a collection of GA_PrimitiveDefinition
 * 	objects and an associated set of intrinsic attributes.
 */

#pragma once

#ifndef __GA_PrimitiveFactory__
#define __GA_PrimitiveFactory__

#include "GA_API.h"
#include "GA_IOTable.h"
#include "GA_IntrinsicManager.h"
#include "GA_PrimitiveDefinition.h"
#include "GA_PrimitiveFamilyMask.h"
#include "GA_PrimitiveTypeMask.h"
#include "GA_PrimitiveTypeId.h"
#include "GA_PrimitiveTypes.h"
#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_FSATable.h>

#include <stddef.h>

class GA_Detail;
class GA_IO;
class GA_Primitive;

class UT_StringArray;
class UT_ArrayStringSet;

class GA_API GA_PrimitiveFactory 
{
public:
    // Define a map where the key is the primitive name and the value is the
    // DSO file defining the primitive.  Primitives not defined by DSOs are not
    // stored in this map.
    typedef UT_ArrayStringMap<UT_StringHolder> ga_DSODefinitions;

    GA_PrimitiveFactory(
        const UT_FSATable *factory_ids,
        unsigned non_factory_id_start, int nentries_hint = 0,
        bool isforprimarydetail = true);
    ~GA_PrimitiveFactory();

    /// Register a new primitive definition.
    /// In the event of a name collision with an existing definition, we return
    /// NULL.
    GA_PrimitiveDefinition *registerDefinition(
        const UT_StringHolder &name,
        GA_PrimitiveBlockConstructor construct,
        GA_PrimitiveFamilyMask mask=GA_FAMILY_NONE,
        const UT_StringHolder &label=UT_StringHolder(),
        const UT_StringHolder &icon=UT_StringHolder());

    /// Return the number of possible primitive types in this factory.
    /// Primitive type id's may not always be contiguous (i.e. there may be
    /// invalid primitive id's in the range).
    int				 getPrimTypeCount() const { return myNextId; }

    /// Lookup an existing primitive definition, by either token or primitive
    /// identifier.
    const GA_PrimitiveDefinition	*lookupDefinition(
					    const UT_StringRef &name) const;
    const GA_PrimitiveDefinition	*lookupDefinition(
					    const GA_PrimitiveTypeId &id) const;
    const GA_PrimitiveDefinition	*lookupDefinition(int id) const;

    /// Given a factory type id, return the corresponding GA_PrimitiveTypeId
    GA_PrimitiveTypeId getFactoryTypeId(int id) const
    { 
	if (id >= 0 && id < myList.size() && myList(id))
	    return GA_PrimitiveTypeId(id);
	return GA_PrimitiveTypeId(); 
    }

    /// Return a list of all primitives defined by DSOs
    static const ga_DSODefinitions	&dsoDefinitions();


    /// Get a primitive type mask representing all known types registered by
    /// this primitive factory. 
    GA_PrimitiveTypeMask	getTypeMaskAll() const;
    
    /// Query the first and last primitive type id in this factory
    void		 getDefinitionIdRange(unsigned &first,
					      unsigned &last) const;

    /// @{
    /// Fill out the list of intrinsic names for the given primitive type.  The
    /// intrinsic names are added to the set.
    void	addPrimitiveIntrinsicNames(const GA_PrimitiveTypeId &id,
				UT_ArrayStringSet &names) const;
    /// @}

    /// Get the names of all detail intrinsics.  You can avoid creating a
    /// detail to do this by calling GUgetFactory().getDetailIntrinsics(names);
    void getDetailIntrinsicNames(UT_StringArray &names) const
    {
        myDetailIntrinsics.extractNames(names);
    }

    /// Returns true if this factory is for use by GEO_Detail and false
    /// if this type is for use by GD_Detail.
    bool        isForPrimaryDetail() const { return myForPrimaryDetail; }

    /// Class to traverse all primitive definitions in the factory
    class iterator
    {
	public:
	    iterator()
		: myFactory(NULL)
		, myCurr(0)
		, myEnd(0)
	    {}
	    iterator(const iterator &src)
	    {
		*this = src;
	    }
	    ~iterator() {}
	    iterator		&operator=(const iterator &src)
				{
				    myFactory = src.myFactory;
				    myCurr = src.myCurr;
				    myEnd = src.myEnd;
				    return *this;
				}
	    bool		operator==(const iterator &src) const
				{
				    if (atEnd() && src.atEnd())
					return true;
				    return myFactory == src.myFactory &&
					    myCurr == src.myCurr &&
					    myEnd == src.myEnd;
				}
	    bool		operator!=(const iterator &cmp) const
				    { return !(*this == cmp); }
	    bool		atEnd() const
				    { return myCurr >= myEnd; }
	    iterator		&operator++()	 { advance(); return *this; }
	    // No post increment as it is harmful.
	    void		rewind()
				{
				    if (myFactory)
				    {
					myFactory->getDefinitionIdRange(
						    myCurr, myEnd
						);
					// Go one beyond last definition
					myEnd++;
				    }
				}

	    void		advance()
				{
				    for (myCurr++; myCurr < myEnd; ++myCurr)
				    {
					// Skip empty entries in the table
					if (myFactory->lookupDefinition(myCurr))
					    break;
				    }
				}
	    const GA_PrimitiveDefinition	*def() const
				{ return myFactory->lookupDefinition(myCurr); }
	private:
	    iterator(const GA_PrimitiveFactory *f)
		: myFactory(f)
	    {
		rewind();
	    }
	    const GA_PrimitiveFactory	*myFactory;
	    unsigned int		 myCurr, myEnd;
	    friend class GA_PrimitiveFactory;
    };
    iterator	begin() const	{ return iterator(this); }

    /// @{
    /// Interface to the IO table -- this may not exist in future versions
    bool		 registerIO(GA_IO *io)
			    { return myIOTable.add(io); }
    const GA_IO 	*findIO(const char *name)
			    { return myIOTable.find(name); }
    void		 getIONames(UT_StringArray &names)
			    { myIOTable.getNames(names); }
    /// @}

private:
    /// Access detail/global intrinsic attributes defined for this factory
    const GA_IntrinsicManager	&getDetailIntrinsics() const
				    { return myDetailIntrinsics; }
    GA_IntrinsicManager		&getDetailIntrinsics()
				    { return myDetailIntrinsics; }

    const UT_StringArray	&getPrimitiveIntrinsics();

    UT_Array<GA_PrimitiveDefinition *>	 myList;
    UT_ArrayStringMap<GA_PrimitiveDefinition *> myTable;
    const UT_FSATable			*myFactoryIds;
    unsigned int			 myNextId;
    GA_PrimitiveTypeMask		 myTypeMask;

    GA_IntrinsicManager				 myDetailIntrinsics;
    UT_StringArray				*myPrimitiveIntrinsics;

    /// Currently, we use the primitive factory as a place to store the IO
    /// table.  It's really a per-detail specialization variable, and this is
    /// the best place to put it.  We rely on the detail friendliness to allow
    /// access to it.
    GA_IOTable					 myIOTable;

    /// true iff this primitive factory is for use in GEO_Detail,
    /// and not GD_Detail.
    const bool myForPrimaryDetail;

    friend class	GA_Detail;
    friend class	GA_PrimitiveDefinition;
};

#endif
