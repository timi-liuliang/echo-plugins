/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_LinkListT_h__
#define __UT_LinkListT_h__

#ifdef WIN32
    #pragma warning(disable:4251)
    #pragma warning(disable:4275)
#endif
#include "UT_LinkList.h"

// These template classes are used to hide some of the casting
// and node management associated with UT_LinkList.


template <class T>
class UT_LinkNodeT : public UT_LinkNode
{
public:
    explicit UT_LinkNodeT(const T &src_data) : UT_LinkNode(), data(src_data) {}

    UT_LinkNodeT<T>	       *&prev()
	{ return reinterpret_cast<UT_LinkNodeT<T> *&>(UT_LinkNode::prev()); }
    UT_LinkNodeT<T>	       *&next()
	{ return reinterpret_cast<UT_LinkNodeT<T> *&>(UT_LinkNode::next()); }
    const UT_LinkNodeT<T>	*prev() const
	{ return static_cast<const UT_LinkNodeT<T> *>(UT_LinkNode::prev()); }
    const UT_LinkNodeT<T>	*next() const
	{ return static_cast<const UT_LinkNodeT<T> *>(UT_LinkNode::next()); }

    T		data;
};

template <class T>
class UT_LinkListT : public UT_LinkList
{
public:
    UT_LinkListT() {}
    ~UT_LinkListT() {}

    UT_LinkNodeT<T>     *next(UT_LinkNodeT<T> *node) const
			{
			    return (UT_LinkNodeT<T> *) UT_LinkList::next(node);
			}

    UT_LinkNodeT<T>     *prev(UT_LinkNodeT<T> *node) const
			{
			    return (UT_LinkNodeT<T> *) UT_LinkList::prev(node);
			}
    
    UT_LinkNodeT<T>	*head() const
			{
			    return (UT_LinkNodeT<T> *) UT_LinkList::head();
			}
    
    UT_LinkNodeT<T>	*tail() const
			{
			    return (UT_LinkNodeT<T> *) UT_LinkList::tail();
			}
    
    void 		append( const T &data )
			{
			    UT_LinkNodeT<T>*	temp;

			    temp = new UT_LinkNodeT<T>(data);

			    UT_LinkList::append( temp );
			}
    
    void		append(UT_LinkNodeT<T> *node)
			{
			    UT_LinkList::append(node);
			}
};


#endif // __UT_LinkListT_h__
