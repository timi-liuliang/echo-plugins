/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IntrinsicManager.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_IntrinsicManager__
#define __GA_IntrinsicManager__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_Options.h>
#include <UT/UT_Array.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ArrayStringMap.h>

#include <stddef.h>


class GA_IntrinsicDef;
class UT_StringArray;


/// @brief Manager to keep track of global handle to name mappings
///
/// Each GA_PrimitiveFactory maintains a list of intrinsic attributes.
/// These attribute provide a mechanism to query information about primitives
/// without having knowledge of the actual structure of the primitive.
///
/// There is a unique handle (or id) for each attribute name.  This class
/// maintains the mapping between name and handle.
///
class GA_API GA_IntrinsicManager 
{
public:
    /// Get the global id associated with a given token
    static GA_GlobalIntrinsic getGlobalHandle(const UT_StringRef &token);

    /// Register a global token (returns the existing handle if it's found)
    static GA_GlobalIntrinsic registerGlobalHandle(const UT_StringHolder &token);

    /// Find the token associated with the given global handle
    static const UT_StringHolder &getGlobalName(GA_GlobalIntrinsic handle);

    /// Return the number of local handles defined in the manager
    int			getLocalEntries() const
			    { return myAttributes.entries(); }

    /// Extract all attribute names
    void		extractNames(UT_StringArray &names) const;

    /// @{
    /// Mapping between global and local attributes
    GA_LocalIntrinsic   getLocalHandle(const UT_StringRef &token) const;
    GA_LocalIntrinsic   getLocalHandle(GA_GlobalIntrinsic ghandle) const;
    GA_GlobalIntrinsic  getGlobalHandle(GA_LocalIntrinsic lhandle) const
			{
			    return isValidHandle(lhandle)
				? myAttributes(lhandle)->getGlobalId()
				: GA_INVALID_INTRINSIC_HANDLE;
			}
    /// @}

    /// Look up an attribute name from the given handle
    const UT_StringHolder &getName(GA_LocalIntrinsic handle) const
			 {
			     return isValidHandle(handle)
				? myAttributes(handle)->getName()
				: UT_StringHolder::theEmptyString;
			 }

    /// @{
    /// Get the attribute storage (passed in during registration)
    GA_StorageClass	getStorage(const UT_StringRef &name) const
                            { return getStorage(getLocalHandle(name)); }
    GA_StorageClass	getStorage(GA_LocalIntrinsic handle) const
			{
			    return isValidHandle(handle)
				 ? myAttributes(handle)->getStorage()
				 : GA_STORECLASS_INVALID;
			}
    /// @}

    /// @{
    /// Get the attribute storage (passed in during registration)
    bool		getReadOnly(const UT_StringRef &name) const
			    { return getStorage(getLocalHandle(name)); }
    bool		getReadOnly(GA_LocalIntrinsic handle) const
			{
			    return isValidHandle(handle)
				 ? myAttributes(handle)->getReadOnly()
				 : true;	// invalid handles are read only
			}
    /// @}

    /// @{
    /// Get the attribute's user id (passed in during registration)
    int			getUserId(const UT_StringRef &name) const
			    { return getUserId(getLocalHandle(name)); }
    int			getUserId(GA_LocalIntrinsic handle) const
			{
			    return isValidHandle(handle)
				 ? myAttributes(handle)->getUserId()
				 : -1;
			}
    /// @}

    /// @{
    /// Get the attribute options (passed in during registration)
    const UT_Options	*getOptions(const UT_StringRef &name) const
			    { return getOptions(getLocalHandle(name)); }
    const UT_Options	*getOptions(GA_LocalIntrinsic handle) const
			 {
			    return isValidHandle(handle)
				 ? &myAttributes(handle)->getOptions()
				 : NULL;
			 }
    /// @}

    /// The Registrar class is used to register intrinsic attributes
    /// Sample usage for a GA_Primitive sub-class might be something like:
    /// @code
    /// static GA_IntrinsicDef  theClassIntrinsics;
    /// 
    /// GA_IntrinsicManager::Registrar
    /// MyClass::registerIntrinsics(GA_PrimitiveDefinition &def)
    /// {
    ///     GA_IntrinsicManager::Registrar      r =
    ///                     MyBaseClass:registerIntrinsics(def);
    ///     if (r.start(theClassIntrinsics))
    ///     {
    ///         r.addAttribute(GA_STORECLASS_STRING, "intrinsic1", 0, true);
    ///         r.addAttribute(GA_STORECLASS_FLOAT, "intrinsic2", 1, true);
    ///     }
    ///     UT_ASSERT(r.getOk());
    ///     return r;
    /// }
    /// @endcode
    class Registrar
    {
	public:
	    /// Copy constructor
	    Registrar(const Registrar &r)
		: myManager(r.myManager)
		, myDef(r.myDef)
		, myOk(r.myOk)
	    {}
	    /// Destructor
	    ~Registrar()
	    {
		end();	// Close off last definition
	    }
	    /// Assignment operator
	    Registrar	&operator=(const Registrar &r)
			 {
			     myManager = r.myManager;
			     return *this;
			 }

	    /// Start defining attributes for the given intrinsic definition
	    bool	start(GA_IntrinsicDef &def)
			{
			    end();
			    if (myOk)
			    {
				myOk = myManager->startDefinition(def);
				myDef = &def;
			    }
			    return myOk;
			}
	    /// Method to add an intrinsic attribute.
	    ///	- The name must be unique through the inheritance hierarchy
	    /// - The @c user_id is used for evaluation (see GA_IntrinsicEval)
	    GA_GlobalIntrinsic	addAttribute(GA_StorageClass store,
					const UT_StringRef &name,
					int user_id,
					bool read_only,
					const UT_Options *options=NULL)
				{
				    GA_GlobalIntrinsic	h;
				    if (myOk)
				    {
					h = myManager->addAttribute(store,
						name, user_id, read_only,
						options);
					if (!GAisValidGlobalIntrinsic(h))
					    myOk = false;
				    }
				    else
					h = GA_INVALID_INTRINSIC_HANDLE;
				    return h;
				}
	    /// Return status of the operation
	    bool	getOk() const	{ return myOk; }

	private:
	    void	end()
			{
			    if (myOk && myDef)
				myOk = myManager->endDefinition(*myDef);
			}
	    Registrar(GA_IntrinsicManager &man)
		: myManager(&man)
		, myDef(NULL)
		, myOk(true)
	    {
	    }
	    friend class GA_Primitive;
	    friend class GA_Detail;
	    GA_IntrinsicManager	*myManager;
	    GA_IntrinsicDef	*myDef;
	    bool		 myOk;
    };

    /// Iterator class to iterate 
    /// Usage: @code
    ///    GA_IntrinsicManager::iterator    it;
    ///    for (it = manager.begin(); !it.atEnd(); ++it) ...
    /// @endcode
    class GA_API iterator
    {
	public:
	    /// Simple default constructor
	    iterator()
		: myManager(NULL),
		  myCurrent(0),
		  myEntries(0) { }
	    /// Copy constructor
	    iterator(const iterator &src)
	    {
		*this = src;
	    }
	    /// Destructor
	    ~iterator() {}
	    /// Assignment operator
	    const iterator	&operator=(const iterator &src)
				 {
				     myManager = src.myManager;
				     myCurrent = src.myCurrent;
				     myEntries = src.myEntries;
				     return *this;
				 }
	    /// Increment operator
	    iterator	&operator++()	{ advance(); return *this; }

	    // No post increment as it is harmful.

	    /// Test to see if the iteration is complete
	    bool	atEnd() const
			{
			    return !myManager || myCurrent >= myEntries;
			}
	    /// Advance to the next iteration
	    void	advance()	{ myCurrent++; }

	    /// Get the current attribute token
	    const UT_StringHolder &getToken() const
				    { return myManager->getName(myCurrent); }
	    GA_LocalIntrinsic	 getLocalHandle() const
				    { return myCurrent; }
	    /// Get the current attribute handle (global id)
	    int			 getIndex() const
				    { return myCurrent; }
	private:
	    /// Private c-tor:  Access this through the begin() method
	    iterator(const GA_IntrinsicManager *manager)
		: myManager(manager),
		  myCurrent(0),
		  myEntries(manager->getLocalEntries())
	    {
	    }
	    const GA_IntrinsicManager	*myManager;
	    GA_LocalIntrinsic		 myCurrent; // Iterate over the locals
	    int				 myEntries;

	    friend class GA_IntrinsicManager;
    };

    /// Start traversal of intrinsic attributes names
    iterator	begin()	const	{ return iterator(this); }

private:
    /// Check to see if handle is in bounds
    inline bool	isValidHandle(GA_LocalIntrinsic h) const
		    { return h >= 0 && h < myAttributes.entries(); }

    /// The internal definition of an attribute
    class ga_IntrinsicAttribute
    {
	public:
	    ga_IntrinsicAttribute(GA_StorageClass store,
			const UT_StringHolder &name,
			GA_GlobalIntrinsic global_id,
			GA_LocalIntrinsic local_id,
			int user_id,
			bool read_only,
			const UT_Options *options)
		: myOptions()
		, myName(name)
		, myGlobalId(global_id)
		, myLocalId(local_id)
		, myUserId(user_id)
		, myStorage(store)
		, myReadOnly(read_only)
	    {
		if (options)
		    myOptions = *options;
	    }
	    ~ga_IntrinsicAttribute() {}

	    const UT_StringHolder &getName() const { return myName; }
	    GA_StorageClass	 getStorage() const { return myStorage; }
	    const UT_Options	&getOptions() const { return myOptions; }
	    int			 getGlobalId() const { return myGlobalId; }
	    int			 getLocalId() const { return myLocalId; }
	    int			 getUserId() const { return myUserId; }
	    bool		 getReadOnly() const { return myReadOnly; }
	private:
	    UT_Options		myOptions;
            UT_StringHolder	myName;
	    GA_GlobalIntrinsic	myGlobalId;
	    GA_LocalIntrinsic	myLocalId;
	    int			myUserId;
	    GA_StorageClass	myStorage;
	    bool		myReadOnly;
    };

    /// @{
    /// When registering attributes, you *must* bracket the attribute
    /// definitions within a startDefinition/endDefinition pair.  Code usually
    /// looks like: @code
    ///	   static GA_IntrinsicDef	theIntrinsicDef;
    ///	   MyClass::registerIntrinsicAttributes(GA_IntrinsicManager &man) {
    ///		MyBaseClass::registerIntrinsicAttributes(man);
    ///		man.startDefinition(theIntrinsicDef);
    ///		man.addAttribute(storage, "foo", 0, ...);
    ///		man.addAttribute(storage, "bar", 1, ...);
    ///		man.endDefinition(theIntrinsicDef);
    ///	    }
    /// @endcode
    /// @see GA_IntrinsicEval
    bool		startDefinition(GA_IntrinsicDef &def);
    bool		endDefinition(GA_IntrinsicDef &def);
    /// @}
    /// Add an local intrinsic.  If there's a name collision within this
    /// object, this method will fail by returning an invalid handle (see
    /// GAisValidGlobalIntrinsic()) The user_id is the value returned by @c
    /// getUserId() and usually refers to an enum private to the caller.
    /// A copy is made of the options if passed in.
    GA_GlobalIntrinsic	addAttribute(GA_StorageClass store, const UT_StringRef &name,
				int user_id, bool read_only,
				const UT_Options *options);


     GA_IntrinsicManager();
    ~GA_IntrinsicManager();

    UT_ArrayStringMap<ga_IntrinsicAttribute *> myTokenToLocalTable;
    UT::ArrayMap<GA_GlobalIntrinsic, int> myGlobalToLocalTable;
    UT_Array<ga_IntrinsicAttribute *>	myAttributes;

    friend class	GA_PrimitiveFactory;
    friend class	GA_PrimitiveDefinition;
    friend class	Registrar;
};

#endif
