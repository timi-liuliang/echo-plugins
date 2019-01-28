/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentRig.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentRig__
#define __GU_AgentRig__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StackBuffer.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_ValArray.h>


class UT_JSONParser;
class UT_JSONWriter;

class GU_AgentRig;
typedef UT_IntrusivePtr<GU_AgentRig>	GU_AgentRigPtr;
typedef UT_IntrusivePtr<const GU_AgentRig> GU_AgentRigConstPtr;


/// A rig for the agent primitive
class GU_API GU_AgentRig : public UT_IntrusiveRefCounter<GU_AgentRig>
{
public:
    typedef UT_StringMap<exint>	        NameMapType;
    typedef NameMapType::const_iterator	NameMapConstIter;
    typedef NameMapType::iterator	NameMapIter;

    static GU_AgentRigPtr   addRig(const char *name);
    static GU_AgentRigPtr   addRigFromFile(
				const char *filename,
				UT_StringArray &errors);

private:
    // Use the static addRig() method to create new rigs
		    GU_AgentRig(const char *name, bool is_file)
			: myName(name), myIsFile(is_file)
		    {
			UT_ASSERT(checkValid());
		    }

public:
                    ~GU_AgentRig();

    /// Construct using flat arrays.
    /// - @c names:  The array of names for each transform (one per transform)
    /// - @c child_counts:  An array of child counts for each transform
    ///		Should have the same number of entries as the @c names array
    /// - @c children:  An array of children.  This is the concatenation of all
    ///		the child indices for each transform.  There should be
    ///		@c sum(child_counts) entries in this array.
    bool	    construct(
			    const UT_StringArray& names,
			    const UT_IntArray& child_counts,
			    const UT_IntArray& children);

    int64           getMemoryUsage(bool inclusive) const;

    /// Return the name of the rig.
    const UT_StringHolder &name() const
                        { return myName; }
    /// Set the name of the rig.
    void            setName(const UT_StringHolder &name)
                        { myName = name; }

    /// Return whether the rig was loaded from disk.
    bool            isFile() const
                        { return myIsFile; }
    /// Clear the flag marking that the rig references a file on disk.
    void            clearIsFile();

    /// Clear the rig
    void	    clear();

    /// Check validity of rig
    bool	    checkValid() const;

    /// Return number of transforms
    exint	    transformCount() const
			{ return myTransforms.entries(); }
    /// Return the name of the given transform
    const UT_StringHolder &transformName(exint i) const
			{ return myTransforms(i); }
    /// Return the parent of the given transform (or -1 for the root)
    exint	    parentIndex(exint i) const
			{ return myParents(i); }
    /// Return the number of children for the given transform
    exint	    childCount(exint i) const
		    {
			return myChildStarts(i+1) - myChildStarts(i);
		    }
    /// Return the Nth child for the given transform
    exint	    childIndex(exint transform, exint child) const
		    {
                        UT_ASSERT_P(child < childCount(transform));
                        return myChildren(myChildStarts(transform) + child);
		    }
    /// Return the root transform(s) of the rig.
    const UT_IntArray &roots() const
                        { return myRoots; }

    /// Find a transform by name.  Returns -1 for an invalid name
    exint	    findTransform(const UT_StringRef &name) const;

    /// Save a rig
    bool	    save(UT_JSONWriter& w) const;
    /// Load a rig
    bool	    load(UT_JSONParser& p, UT_StringArray& errors);

    /// Dump to stdout for debugging
    void	    dump() const;

    /// Iterator for top down traversal of the rig hierarchy. Each visited
    /// transform's parent is guaranteed to be visited before its children.
    class const_iterator
    {
    public:
        const_iterator(const GU_AgentRig &rig, int start = -1)
            : myRig(rig), myIndex(0), myEnd(0), myQueue(rig.transformCount())
        {
            rewind(start);
        }

        void rewind(int start = -1)
        {
            myIndex = 0;
            myEnd = 0;

            if (start < 0)
            {
                for(int i : myRig.roots())
                    myQueue[myEnd++] = i;
            }
            else
                myQueue[myEnd++] = start;
        }

        inline void advance()
        {
            const int i = myQueue[myIndex];

            // Queue the child transforms for processing.
            for (int child = 0, n = myRig.childCount(i); child < n; ++child)
            {
                myQueue[myEnd++] = myRig.childIndex(i, child);
                UT_ASSERT_P(myEnd <= myRig.transformCount());
            }

            ++myIndex;
        }

        inline bool atEnd() const
        {
            return myIndex == myEnd;
        }

	/// Returns the transform index
        inline int operator*() const { return myQueue[myIndex]; }

	/// Pre-increment operator
	inline const_iterator& operator++() { advance(); return *this; }

    private:
        const GU_AgentRig &myRig;
        int myIndex;
        int myEnd;
        // We know our total number of xforms is bounded, so we
        // can allocate potentially on the stack the full queue.
        UT_StackBuffer<int, 1024> myQueue;
    };

private:
    UT_StringHolder myName;
    bool            myIsFile;

    // We build a symbol table sharing the string in the myTransforms array
    UT_StringArray  myTransforms;	// One per transform
    NameMapType	    myTransformsMap;	// Map from names to indices

    UT_IntArray	    myParents;		// Indices to parent transform
    UT_IntArray	    myChildStarts;	// One per transform, indices into
					// myChildren for where the children
					// for the i'th transform starts.
    UT_IntArray	    myChildren;		// Child pointers
    UT_IntArray     myRoots;            // Root transform(s) of the rig.
};

///////////////////////////////////////////////////////////////////////////////
//
// Implementation
//

inline exint
GU_AgentRig::findTransform(const UT_StringRef &name) const
{
    NameMapConstIter it = myTransformsMap.find(name);
    if (it == myTransformsMap.end())
	return -1;
    return it->second;
}

#endif
