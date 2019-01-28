/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ProxyPointer.h ( UT Library, C++)
 *
 * COMMENTS:	A proxy pointer is used when you have other objects referencing
 *	a source object.  In many cases, it's possible for the source object to
 *	be destroyed leaving dangling references to the now deleted source
 *	object.
 *	This class allows the source object to obtain a "proxy" pointer that
 *	the other objects can reference.  When the source object is deleted, it
 *	should clear the proxy pointer so that all referencing objects will get
 *	a null pointer (instead of a pointer to garbage memory).
 *
 *      Example:
 *	class A {
 *	    A() { myProxy = UT_ProxyPointer::allocProxy(this); }
 *	   ~A() { UT_ProxyPointer::freeProxy(myProxy); }
 *	};
 *
 *	class B {
 *	   B(int proxy)	{ myPointer=UT_ProxyPointer::reference(proxy); }
 *	  ~B()		{ UT_ProxyPointer::deReference(myPointer); }
 *	   A	*getA() { return (A *)UT_ProxyPointer::lookup(myPointer); }
 *	};
 *	main()
 *	{
 *	    A	*a;
 *	    B	*b0, *b1;
 *	    a = new A();
 *	    b0 = new B(a->myProxy);
 *	    b1 = new B(a->myProxy);
 *	    fprintf(stderr, "0x%08x\n", b0->getA()); delete b0;
 *	    delete a;
 *	    fprintf(stderr, "0x%08x\n", b1->getA()); delete b1;
 *	}
 *
 *  NOTE:  This class mainly provides a defense against poor programming.
 *  However, it can provide memory efficiency since this allows links to be
 *  built between objects without maintining double links.  The cost here is
 *  that there are multiple de-references instead of a single de-reference of
 *  the pointer, meaning that cache performance is impacted.  As well, more
 *  null pointer checking needs to be performed.
 */

#ifndef __UT_ProxyPointer__
#define __UT_ProxyPointer__

#include "UT_API.h"

class UT_API UT_ProxyPointer {
public:
    // The source object can use this class to allocate a proxy pointer
    static int		allocProxy(void *ptr);
    static void		freeProxy(int &id);

    //  This ID is guaranteed (well pretty much) to be an invalid id.
    static inline int	nullId()	{ return -1; }
    static inline int	isValid(int id)	{ return id >= 0; }

    // When a class needs to reference the proxy, they should make sure to
    // reference/de-reference the proxy properly.
    static void		referenceProxy(int id);
    static void		deReferenceProxy(int id);

    //
    // Swizzling the pointer allows you to change the object the proxy points
    // to so that all references to the pointer will be updated to the new
    // pointer.
    static void		swizzlePointer(int id, void *ptr);

    // This method can be used to get the pointer out of the proxy.  If the
    // source object has been free'd a null pointer (i.e. 0) will be returned.
    static void		*lookupProxy(int id);

    // For some odd reason, you may want to know if there are other people
    // referencing the proxy.  This method will return the number of references
    // to the proxy (including yourself).
    static unsigned	 getReferenceCount(int id);
};

#endif
