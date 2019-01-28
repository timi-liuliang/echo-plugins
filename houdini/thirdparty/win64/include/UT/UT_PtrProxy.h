/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PtrProxy.h ( UT Library, C++)
 *
 * COMMENTS:	Template interface to the UT_ProxyPointer methods
 */

#ifndef __UT_PtrProxy__
#define __UT_PtrProxy__

#include "UT_ProxyPointer.h"


//
// First half of the proxy pointer interface.  The UT_PtrProxy maintains a
// pointer to the source object.
//
template <typename utObj>
class UT_PtrProxy {
public:
    explicit UT_PtrProxy(utObj *me)
    {
	myId = UT_ProxyPointer::allocProxy(me);
    }
    ~UT_PtrProxy()
    {
	UT_ProxyPointer::freeProxy(myId);
    }

    int			getId() const	{ return myId; }

    operator		const utObj *() const
			{
			    const void *v = UT_ProxyPointer::lookupProxy(myId);
			    return (const utObj *)v;
			}
    operator		utObj *()
			{
			    void *v = UT_ProxyPointer::lookupProxy(myId);
			    return (utObj *)v;
			}
    operator		int() const
			{
			    return myId;
			}
    void		swizzle(utObj *me)
			{
			    UT_ProxyPointer::swizzlePointer(me);
			}
    unsigned int	getReferences() const
			{
			    return UT_ProxyPointer::getReferenceCount(myId)-1;
			}

private:
    int	 	myId;
};

template <typename utObj>
class UT_RefProxy {
public:
    explicit UT_RefProxy(int id)
    {
	init(id);
    }
    explicit UT_RefProxy(const UT_PtrProxy<utObj>&ptr)
    {
	init(ptr.getId());
    }
    ~UT_RefProxy()	{ UT_ProxyPointer::deReferenceProxy(myId); }

    operator		const utObj *() const
			{
			    const void *v = UT_ProxyPointer::lookupProxy(myId);
			    return (const utObj *)v;
			}
    operator		utObj *()
			{
			    void *v = UT_ProxyPointer::lookupProxy(myId);
			    return (utObj *)v;
			}
    operator		int() const
			{
			    return myId;
			}

private:
    void	init(int id)
		{
		    myId = id;
		    UT_ProxyPointer::referenceProxy(myId);
		}
    int		myId;
};


#endif

