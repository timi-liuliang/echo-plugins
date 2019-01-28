/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Classifier.h (UT Library, C++)
 *
 * COMMENTS: This is a generic implementation of disjoint-set (aka union/find)
 * data structure.
 *
 * We have two versions of this data structure, the unreferenced and the
 * referenced one, respectively realized by classes UT_Classifier and
 * UT_PtrClassifier.
 *
 * The referenced version essentially has the same functionality as the
 * unreferenced one but in addition also keeps track of the elements out of
 * which it makes the initial singleton classes and can offer reverse look up
 * on them.
 *
 * Elements are turned into singleton classes using makeClass() method which
 * returns an integer "ticket number". Each ticket number represents an element
 * in the structure. Ticket numbers are generated consecutively starting from
 * 0, therefore, if the objects we wish to classify happen to be indexed
 * consecutively (as is the case with points, primitives, etc), there will be
 * a natural mapping between ticket numbers and object indices which makes
 * moving between ticket numbers and and actual objects trivial. Otherwise,
 * there will be a need for mapping between ticket numbers and the objects.
 * In this case the class UT_PtrClassifier can help by keeping back-references
 * to the objects for which tickets are issued. Therefore, the makeClass()
 * method takes a reference to the object for which the ticket is being issued
 * and this reference can be later retrieved from the ticket number using the
 * getRef() method.
 *
 * In contrast, UT_Classifier::makeClass does not take a reference to the
 * object and instead allows multiple tickets to be generated at once.
 *
 * The unionParts operation combines the classes to which its two operands
 * belong into a single class. Each class is identified by its "root"
 * (sometimes called "leader" in the literature) which is one of the elements
 * in that class. The ticket number for the root of a class can be looked up
 * using the partRoot() method. Therefore to check to see if two elements are
 * contained in the same class, one can compare their corresponding roots,
 * or equivalently use the areInSameClass() shortcut. In UT_PtrClassifier one
 * can directly lookup a reference to the root of a class using getRootRef().
 *
 * It is often useful to have consecutive class indices. The classIndex()
 * method returns such a classIndex for each ticket number. Note that these
 * indices can change every time a new class is created or two classes are
 * merged into one.
 *
 * It is sometimes useful to have access to the size of each class. The
 * optional parameter "track_class_sizes" for the constructor of UT_Classifier
 * and UT_PtrClassifier enables tracking of class sizes. At any time, the
 * size of the class to which an element belongs can be queried using
 * classSize() method.
 *
 * Note 1: in general this data structure does not support deletion of elements.
 *
 * Note 2: Neither reads nor writes are thread-safe.
 *
 */


#ifndef __UT_Classifier__
#define __UT_Classifier__

#include <SYS/SYS_Types.h>
#include "UT_Array.h"
#include "UT_ValArray.h"

#define PARENT_MASK	 0x00ffffffffffffff
#define RANK_MASK 	 0xff00000000000000
#define RANK_BUMP 	 0x0100000000000000

class UT_API UT_Classifier
{
public:
    			 UT_Classifier(bool track_class_sizes = false) :
				myTrackSizes(track_class_sizes) { }

    			~UT_Classifier() = default;

    /// create one or more singleton classes which get consecutive ticket
    /// numbers returns the ticket number of that last class created
    SYS_FORCE_INLINE
    exint 		 makeClass();

    SYS_FORCE_INLINE
    exint 		 makeClass(exint num_classes);


    /// merge the classes containing elements with tickets a and b into a
    /// single part
    SYS_FORCE_INLINE
    void 		 unionClasses(exint a, exint b);

    /// Look up the ticket number for the root element of the part of the
    /// parameter element
    SYS_FORCE_INLINE
    exint		 classRoot(exint elem) const;

    SYS_FORCE_INLINE
    bool		 areInSameClass(exint a, exint b) const
			    { return classRoot(a) == classRoot(b); }

    /// Return number of distinct classes
    exint 		 numClasses() const { return myNumClasses; }

    /// Return the total number of elements in all classes
    exint       	 size() const  { return myElems.size(); }
    exint 		 entries() const  { return size(); }

    /// Returns an ordinal number between 0 and number of classes - 1
    /// creation of new classes or unification of distinct classes invalidate
    /// class indices. Each time the class indices are invalidated, the first
    /// call to classIndex takes linear time. All subsequent calls take
    /// constant time so long as the class indices remain valid.
    /// The root_independent parameter is passed to buildIndex() if the index
    /// has not been previously built needs to be rebuilt. See the comments
    /// above that method for explanation.

    SYS_FORCE_INLINE
    exint 		 classIndex(exint elem, bool root_independent = false)
    {
	if (myIndex.size() == 0)
	    buildIndex(root_independent);

	return myIndex(elem);
    }


    SYS_FORCE_INLINE
    exint 		 classSize(exint elem) const
    {
	UT_ASSERT(myTrackSizes);
	return mySize.size() > 0 ? mySize(classRoot(elem)) : -1;
    }

    /// Copy the whole index array.
    void		 getAllClassIndices(UT_IntArray &out_array);

    /// Free up all unnecessary information except for class roots.
    /// makeClass and unionClasses operation are ignored on a consolidated
    /// classifier. class size and index information are also expunged (unless
    /// keep_size or keep_index are respectively set to true) and methods
    /// classIndex() and classSize() return -1 on all input.
    /// Of course classRoot() very much works as usual after consolidation!
    /// Calling reset() revives the object from the state of consolidation
    /// but naturally resets all the classes into singletons.
    void		 consolidate(bool keep_size = false,
				bool keep_index = false);

    /// Reset all the elements to singleton classes. If new_size is
    /// non-negative the number of classes are changed to new_size with
    /// ticket numbers 0 to new_size - 1. Otherwise, all classes are made
    /// singletons again.
    void 		 reset(exint new_size = -1);

    /// Explicit request to build the index, instead of implicitly triggering
    /// it by calling classIndex(). If root_indepent is true, the created
    /// index of eac class will be the relative rank of its smallest element
    /// compared to other classes. Otherwise, it will be the relative rank
    /// of the class root (default) which is the case if the index is build
    /// implicitly.
    /// If min_class_size is set to a number larger than 1, only classes
    /// with min_class_size elements or larger are assigned a usual index
    /// and other classes are assigned -1.
    void		 buildIndex(bool root_independent = false,
    			    	exint min_class_size = 1);

    exint		 numIndexedClasses() const
    				{ return myNumIndexedClasses; }

private:

    SYS_FORCE_INLINE
    exint		 parent(exint i) const
    			    { return exint(myElems(i) & PARENT_MASK); }

    SYS_FORCE_INLINE
    exint		 rank(exint i) const
    			    { return exint(myElems(i) & RANK_MASK); }


    SYS_FORCE_INLINE
    void		 parent(exint i, exint p) const
    			    { myElems(i) = ((myElems(i)  & RANK_MASK) | p); }

    SYS_FORCE_INLINE
    void		 rank(exint i, exint r) const
    			    { myElems(i) = ((myElems(i)  & PARENT_MASK) | r); }

    mutable
    UT_ExintArray	 myElems;

    mutable
    UT_ExintArray	 mySize;
    UT_ExintArray 	 myIndex;

    bool 		 myTrackSizes;
    exint 		 myNumClasses			= 0;
    exint		 myNumIndexedClasses		= 0;
};



template <typename T>
class UT_PtrClassifier
{
    			 UT_PtrClassifier(bool track_class_sizes) :
				 myClasses(track_class_sizes) { }

    			~UT_PtrClassifier() { }

    /// create a singleton part using parameter element

    SYS_FORCE_INLINE
    exint 		 makeClass(const T &ref)
    {
        exint res = myClasses.makeClass();
        myRef.append(ref);
        return res;
    }

    SYS_FORCE_INLINE
    void 		 unionClasses(exint a, exint b)
    			    { myClasses.unionClasses(a, b); }

    SYS_FORCE_INLINE
    exint 		 classRoot(exint elem) const
    			    { return myClasses.classRoot(elem); }

    SYS_FORCE_INLINE
    bool 		 areInSameClass(exint a, exint b) const
    			    { return myClasses.areInSameClass(a, b); }

    SYS_FORCE_INLINE
    exint 		 numClasses() const
    			    { return myClasses.numClasses(); }

    SYS_FORCE_INLINE
    exint 		 size() const
    			    { return myClasses.size(); }
    
    void 		 reset()
    			    { myClasses.reset(); }

    SYS_FORCE_INLINE
    exint 		 classIndex(exint elem)
    			    { return myClasses.classIndex(elem); }

    SYS_FORCE_INLINE
    exint 		 classSize(exint elem)
    			    { return myClasses.classSize(elem); }

    /// reverse lookup the element from the ticket number
    SYS_FORCE_INLINE
    T 			&getRef(exint elem)
    			    { return myRef(elem); }

    /// reverse lookup the part root element from the ticket number of any
    /// part element
    T 			&getClassRootRef(exint elem)
    {
        exint root = myClasses.classRoot(elem);
        return getRef(root);
    }

    void 		consolidate(bool keep_size = false,
			    bool keep_index = false)
    {
	myClasses.consolidate(keep_size, keep_index);
    }

private:
    UT_Array<T&> 	 myRef;
    UT_Classifier 	 myClasses;
};


exint
UT_Classifier::makeClass()
{
    auto s = myElems.size();
    myElems.append(0);
    parent(s, s);
    return s;
}


exint
UT_Classifier::makeClass(exint num_classes)
{
    auto s = myElems.size();
    myElems.appendMultiple(0, num_classes);

    for (exint i = s, ie = myElems.size(); i < ie; i++)
	parent(i, i);

    if (myTrackSizes)
	mySize.appendMultiple(1, num_classes);

    myNumClasses += num_classes;
    return myElems.size() - 1;
}


void
UT_Classifier::unionClasses(exint a, exint b)
{
    exint aroot = classRoot(a);
    exint broot = classRoot(b);

    if (aroot == broot)
	return;

    auto arootrank = rank(aroot);
    auto brootrank = rank(broot);

    if (arootrank < brootrank)
    {
	parent(aroot, broot);
	if (myTrackSizes)
	    mySize(broot) += mySize(aroot);
    }
    else if (brootrank < arootrank)
    {
	parent(broot, aroot);
	if (myTrackSizes)
	    mySize(aroot) += mySize(broot);
    }
    else
    {
	parent(broot, aroot);
	rank(aroot, arootrank + RANK_BUMP);
	if (myTrackSizes)
	    mySize(aroot) += mySize(broot);
    }
    myNumClasses--;
}


exint
UT_Classifier::classRoot(exint i) const
{
    exint e = i;

    while (true)
    {
	auto p = parent(e);
	if (p == e)
	    break;
	e = p;
    }


    while (i != e)
    {
	auto p = parent(i);
	parent(i, e);
	i = p;
    }

    return e;
}

#endif
