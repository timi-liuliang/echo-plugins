/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeType.h ( GA Library, C++)
 *
 * COMMENTS:	Class-factory for GA_Attribute objects
 *
 *	Each attribute type should have a unique name.
 *
 *	The constructor will register the attribute type and assign it a unique
 *	integer identifier.  The unique integer identifier may change from run
 *	to run depending on construction order, so it should only be used in a
 *	single session (and you should *not* assume the value will be the same
 *	on the next session).
 *		
 */

#pragma once

#ifndef __GA_AttributeType__
#define __GA_AttributeType__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>

class GA_IndexMap;
class GA_Attribute;
class GA_AIFBase;
class GA_AIFEditDelta;
class GA_AttributeOptions;
class GA_ReuseStrategy;
class UT_Options;
class UT_JSONParser;

template <typename AIF_TYPE> class GA_AIFDefinition;

class GA_API GA_AttributeType
{
public:
    GA_AttributeType(const UT_StringHolder &name);
    virtual ~GA_AttributeType();

    static const GA_AttributeType *findType(const UT_StringRef &name);

    /// Method to create and initialize a new attribute.  The pure virtual
    /// alloc() method is used to allocate the attribute.
    GA_Attribute	*create(const GA_IndexMap &index_map,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				const UT_Options *creation_options,
				const GA_AttributeOptions *attribute_options) const;

    /// There may already be an attribute with the same name when we attempt
    /// to add a new attribute.  Whenever possible we want to try to recycle
    /// the existing attribute, updating it for our current needs, instead of
    /// allocating a new attribute and copying any compatible values.
    ///
    /// This method returns false whenever it was not possible to update the
    /// supplied attribute to meet the new requirements.  The attribute may
    /// be partially modified even on failure.
    /// @note Always fails if findType(requested_type) != this, regardless of
    /// strategy.
    bool		 recycleAttribute(GA_Attribute &attrib,
				 const GA_ReuseStrategy &strategy,
				 const GA_AttributeType &requested_type,
				 const UT_Options *creation_options,
				 const GA_AttributeOptions *attribute_options) const;

    /// Return information about the type
    SYS_FORCE_INLINE
    const UT_StringHolder &getTypeName() const  { return myTypeName; }
    SYS_FORCE_INLINE
    int			 getTypeId() const	{ return myTypeId; }

    bool		 isFactoryType() const { return myFactoryFlag; }
    bool		 isStandardType() const { return myStandardFlag; }
    bool		 isTopologyType() const
				    { return myTypeId == ourTopologyTypeId; }

    /// This method is used to parse the GA_AIFEditDelta type arguments saved
    /// to a JSON stream in order to allocate the appropriate subclass.  The
    /// type arguments should be stored as the next object.
    /// TODO: Maybe we should parse a UT_Options object externally and pass
    ///       that in?
    virtual bool	 jsonLoadDeltaDefinition(UT_JSONParser &p,
						 GA_AIFEditDelta *&delta) const;

    /// This method is used to register a new AIF type.  The returned object
    /// can be used to bind custom implementations to specific types and get
    /// the implementation bound to a given type.  A default implementation
    /// can be specified during the registration that will be used for those
    /// types without an explicitly bound custom implementation.
    template <typename AIF_TYPE> static
	GA_AIFDefinition<AIF_TYPE>	registerAIF(AIF_TYPE *default_impl)
					{
					    return GA_AIFDefinition<AIF_TYPE>(
						    allocAIF(default_impl));
					}

protected:
    /// Allocate the attribute
    virtual GA_Attribute	*alloc(const GA_IndexMap &index_map,
				       GA_AttributeScope scope,
				       const UT_StringHolder &name,
				       const UT_Options *creation_options)
								    const = 0;

    /// This method is the type-specific implementation of recycleAttribute().
    ///
    /// @pre &attrib.getType() == this
    virtual bool		 recycle(GA_Attribute &attrib,
					 const GA_ReuseStrategy &strategy,
					 const UT_Options *creation_options)
								    const = 0;

private:
    // Register built-in attribute types during static initialization of GA.
    // We don't want it to happen when code tries to create details in parallel.
    static void createFactoryTypes();
    class Creator;
    static Creator theCreator;

    static int		 allocAIF(GA_AIFBase *default_aif);
    static void		 bindAIF(const UT_StringHolder &type, int id, GA_AIFBase *aif);
    const GA_AIFBase	*getAIF(int id) const;

    static int		 ourTopologyTypeId;


    UT_StringHolder myTypeName;
    int             myTypeId;
    bool            myFactoryFlag;
    bool            myStandardFlag;

    UT_Array<GA_AIFBase *> myAIFs;

    // Ideally we'd only declare specific methods of GA_AIFDefinition friends,
    // but the GA_AIFDefinition template must be declared after us.
    template <typename AIF_TYPE> friend class GA_AIFDefinition;
#if 0
    template <typename AIF_TYPE> friend
	void GA_AIFDefinition<AIF_TYPE>::bind(const UT_StringHolder &, AIF_TYPE *) const;
    template <typename AIF_TYPE> friend
	const AIF_TYPE *GA_AIFDefinition<AIF_TYPE>::get(
						const GA_AttributeType &) const;
#endif
};

/// A common base class for all client-registered AIF classes.
class GA_API GA_AIFBase
{
public:
    virtual ~GA_AIFBase() {}
};

/// @brief Class to allow custom AIF interfaces to be built
///
/// GA_AIFDefinition is used by client-code to manage client-registered
/// AIF_TYPE implementations.  AIF_TYPE must be derived from GA_AIFBase.
template <typename AIF_TYPE>
class GA_API GA_AIFDefinition
{
public:
    /// Bind an implementation to the specified attribute type.  This method
    /// can be called even before the specified attribute type is registered.
    void		 bind(const UT_StringHolder &attrib_type, AIF_TYPE *aif) const
			 {
			     GA_AttributeType::bindAIF(attrib_type, myId, aif);
			 }
    /// Query the implementation for the specified attribute type.  Default
    /// implementation specified during registration, if any, is returned if
    /// no implementation has been explicitly bound to this type.
    const AIF_TYPE	*get(const GA_AttributeType &type) const
			 {
			     return static_cast<const AIF_TYPE *>(
							type.getAIF(myId));
			 }
private:
    explicit GA_AIFDefinition(int id) : myId(id) {}

    int myId;

#if defined(WIN32) || defined(__clang__)
    template <typename T>
    friend GA_AIFDefinition<T> GA_AttributeType::registerAIF(T *);
#else
    friend GA_AIFDefinition<AIF_TYPE> GA_AttributeType::registerAIF<AIF_TYPE>(AIF_TYPE *);
#endif
};


#endif
