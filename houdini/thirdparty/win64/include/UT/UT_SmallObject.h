/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SmallObject.h ( UT Library, C++)
 *
 * COMMENTS:	Small object allocator.
 */

#ifndef __UT_SmallObject__
#define __UT_SmallObject__

#include "UT_SmallObjectAllocator.h"

// A small object allocator:
//	cleanPages:
//	   Pages are freed when they become empty. Requires an extra 4 bytes
//	   per object and can hurt performance. But reduces memory footprint.
//	pageSize:
//	   Number of objects per page (how many objects are allocated at once).
//	maxObjectSize:
//	   The size above which we revert to regular new and delete.
//	threadSafe:
//	   Will ensure that allocs/free's are done in a thread safe manner.
//	SuperClass:
//	   Allows you to create a UT_SmallObject with an arbitrary base class.
//	   This is useful if you want to have class A, with some subclass B
//	   which is a small object, but another subclass C which is not. In
//	   This case you would have:
//		class B : public UT_SmallObject<... , A> { };
//		class C : public A { };
//	SubClass:
//	   Set this to the class being created. The only reason for this
//	   parameter is so that we won't get linker errors on Windows.
#define UT_SMALLOBJECT_CLEANPAGES_DEFAULT	true
#define UT_SMALLOBJECT_CLEANPAGES_ON		true
#define UT_SMALLOBJECT_CLEANPAGES_OFF		false

#define UT_SMALLOBJECT_PAGESIZE_DEFAULT		1024

#define UT_SMALLOBJECT_THREADSAFE_DEFAULT	true
#define UT_SMALLOBJECT_THREADSAFE_ON		true
#define UT_SMALLOBJECT_THREADSAFE_OFF		false

#define UT_SMALLOBJECT_MAXOBJECTSIZE_DEFAULT	128

// We need a default SuperClass argument. Any empty class with a virtual
// destructor will do the trick.
class UT_SmallObjectDefaultSuperClass
{
public:
};

// The UT_SmallObject class template.
template <class SubClass,
	  bool cleanPages = UT_SMALLOBJECT_CLEANPAGES_DEFAULT,
	  int pageSize = UT_SMALLOBJECT_PAGESIZE_DEFAULT,
	  bool threadSafe = UT_SMALLOBJECT_THREADSAFE_DEFAULT,
          size_t maxObjectSize = UT_SMALLOBJECT_MAXOBJECTSIZE_DEFAULT,
	  class SuperClass = UT_SmallObjectDefaultSuperClass>
class UT_SmallObject : public SuperClass
{
public:
    /// Regular new/delete operators
    // @{
    static void	*operator new(size_t size)
		 {
		     return UT_SmallObjectAllocator::allocate(size);
		 }
    static void	 operator delete(void *p, size_t size)
		 {
		     UT_SmallObjectAllocator::deallocate(p, size);
		 }
    // @}

    // Placement new/delete operators
    // @{
    static void	*operator new(size_t /*size*/, void *p)
		 {
		     return p;
		 }
    static void	 operator delete(void *, void *)
		 {
		 }
    // @}
};

#endif
