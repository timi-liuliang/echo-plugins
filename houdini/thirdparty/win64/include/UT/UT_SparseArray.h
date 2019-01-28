/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	Mantains a sparse array of values.
 *
 */

#ifndef __UT_SparseArray_h__
#define __UT_SparseArray_h__

#include "UT_ValArray.h"
#include "UT_SmallObject.h"

template <typename T>
class UT_SparseArray {
public:
     UT_SparseArray();
    ~UT_SparseArray();

    void	 append(int index, T data);
    void	 removeIndex(int index);

    void	 clear();

    // These operators find the index specified.  If the index doesn't exist
    // (i.e. hasn't been defined yet), a null value is returned.
    T		 operator()(unsigned int i);
    const T	 operator()(unsigned int i) const;

    // Find an index in the array, returns -1 if the entry doesn't exist.
    int		 find(int index) const;

    // Returns the number of indices stored in the sparse array.
    int		 entries() const { return (int)myArray.entries(); }

    // This method will not look up based on the array index, but rather on
    // the actual array which stores the sparse entries.
    T		 getRawEntry(int raw_index, int &index);

private:
    class SparseEntry : public UT_SmallObject<SparseEntry>
    {
    public:
	SparseEntry(int i, T d) { myIndex = i; myData = d; }

	int	 myIndex;
	T	 myData;
    };


    UT_ValArray<SparseEntry *>	myArray;
};

#include "UT_SparseArrayImpl.h"

#endif
