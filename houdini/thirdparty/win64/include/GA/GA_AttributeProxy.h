/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeProxy.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_AttributeProxy__
#define __GA_AttributeProxy__

#include "GA_API.h"
#include <UT/UT_IntrusivePtr.h>
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>

#include <stddef.h>


class GA_Attribute;
class UT_MemoryCounter;


/// @brief This class holds a reference to an attribute. 
/// Such an indirection level allows an easy way to invalidate attribute
/// references when an attribute gets deleted (or when the detail itself is
/// deleted).
class GA_API GA_AttributeProxy
{
public:
    /// Constructor.
    explicit		 GA_AttributeProxy(GA_Attribute * atr);
			~GA_AttributeProxy();

    /// @{
    /// Obtains an actual attribute. May be NULL, if the attribute got deleted
    /// or was never actually found (or sussessfully created).
    GA_Attribute	*getAttribute()
			    { return myAttribute; }
    const GA_Attribute	*getAttribute() const
			    { return myAttribute; }
    /// @}
    
    /// Returns true if the proxy is still pointing to a valid attribute.
    bool		 isValid() const
			    { return myAttribute != NULL; }

    /// Checks if there is only one reference to this proxy (held by the caller)
    bool		 isLastReference() const;

    /// Returns the memory footprint of the proxy and the attribute it
    /// represents.
    int64		 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    ///
    /// NOTE: This treats GA_AttributeProxy as unshared, despite
    /// myReferenceCount, because the count should always be 1.
    void		countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// @private
    /// This interface should only be used by the intrusive pointer handles
    inline void	 intrusive_ptr_add_ref() const	{ incref(); }
    inline void	 intrusive_ptr_release() const	{ decref(); }
    /// @}

private:
    /// Makes the attribute reference inalid. This is necessary to do when the
    /// detail or the attribute in the detail gets deleted to prevent the
    /// proxy from referencing deleted objects.
    void		 invalidate();	// only GA_AttributeSet calls it
    /// Ask the proxy to register a new reference, which essentially
    /// increments the reference counter.
    void		 incref() const;
    /// Ask the proxy to unregister a reference, which essentially
    /// decrements the reference counter. When the reference decreases to
    /// zero the proxy object gets deleted.
    void		 decref() const;

    friend class	 GA_AttributeSet;

private:
    /// A reference to the actual attribute.
    GA_Attribute		*myAttribute;

    /// A reference counter.
    mutable SYS_AtomicInt32	 myReferenceCount;
};

/// @{
/// Boost function callbacks for referencing and unreferencing GA_AttributeProxy
static inline void
intrusive_ptr_add_ref(const GA_AttributeProxy *proxy) 
{ 
    proxy->intrusive_ptr_add_ref();
}

static inline void 
intrusive_ptr_release(const GA_AttributeProxy *proxy)
{ 
    proxy->intrusive_ptr_release();
}
/// @}

/// @{
/// Define an intrusive type handle type for GA_AttributeProxy
typedef UT_IntrusivePtr<GA_AttributeProxy>	 GA_AttributeProxyHandle;
typedef UT_IntrusivePtr<const GA_AttributeProxy> GA_ConstAttributeProxyHandle;
/// @}

#endif
