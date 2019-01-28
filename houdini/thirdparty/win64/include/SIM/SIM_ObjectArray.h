/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ObjectArray_h__
#define __SIM_ObjectArray_h__

#include "SIM_API.h"
#include "SIM_PtrArraySorted.h"

class SIM_Object;
class SIM_ObjectAtTime;
class SIM_ConstObjectArray;
class SIM_DataFilter;

/// Holds pointers to a number of SIM_Object objects.
class SIM_API SIM_ObjectArray :
    public SIM_PtrArraySorted<SIM_Object *>
{
public:
			 SIM_ObjectArray() : 
			     SIM_PtrArraySorted<SIM_Object *>(compareObjectId)
			 {
			 }
    virtual		~SIM_ObjectArray() {}

    /// This function returns a pointer the the SIM_Object with the
    /// specified object id. If the object id is not found, it returns
    /// null.
    SIM_Object		*findObjectById(int objectid) const;

    /// This function returns the index within the array of the object
    /// with the specified id. Returns -1 if objectid doesn't occur in
    /// the array.
    int			 findPositionById(int objectid) const;

    /// Fills an array with all the objects in this array that match the
    /// supplied filter.
    void		 filter(const SIM_DataFilter &filter,
				SIM_ObjectArray &filtered) const;
    /// Fills an array with all the objects in this array that match the
    /// supplied filter.
    void		 filterConst(const SIM_DataFilter &filter,
				SIM_ConstObjectArray &filtered) const;

    /// This function is used for sorting the objects in the array.
    /// It compares objects using the object id values.
    static int		 compareObjectId(SIM_Object * const *,
					 SIM_Object * const *);
};

/// Holds pointers to a number of const SIM_Object objects.
class SIM_API SIM_ConstObjectArray :
    public SIM_PtrArraySorted<const SIM_Object *>
{
public:
			 SIM_ConstObjectArray() :
			    SIM_PtrArraySorted<const SIM_Object *>(compareConstObjectId)
			 {}
    virtual		~SIM_ConstObjectArray() {}

    /// An assignment operator to copy a SIM_ObjectArray into a
    /// SIM_ConstObjectArray.
    const SIM_ConstObjectArray	&operator=(const SIM_ObjectArray &src);

    /// This function returns a pointer the the SIM_Object with the
    /// specified object id. If the object id is not found, it returns
    /// null.
    const SIM_Object	*findObjectById(int objectid) const;

    /// This function returns the index within the array of the object
    /// with the specified id.
    int			 findPositionById(int objectid) const;

    /// Fills an array with all the objects in this array that match the
    /// supplied filter.
    void		 filterConst(const SIM_DataFilter &filter,
				SIM_ConstObjectArray &filtered) const;

    /// This function is used for sorting the objects in the array.
    /// It compares objects using the object id values.
    static int		 compareConstObjectId(const SIM_Object * const *,
					      const SIM_Object * const *);
};

class SIM_API SIM_ObjectAtTimeArray :
    public SIM_PtrArraySorted<SIM_ObjectAtTime *>
{
public:
			 SIM_ObjectAtTimeArray(int objectid);
    virtual		~SIM_ObjectAtTimeArray();

    int			 getObjectId() const;

    /// This function is used for sorting the objects in the array.
    /// It compares objects using the object id values.
    static int		 compareObjectTimes(SIM_ObjectAtTime * const *,
					    SIM_ObjectAtTime * const *);

private:
    int			 myObjectId;
};

class SIM_API SIM_ObjectAtTimeMatrix :
    public SIM_PtrArraySorted<SIM_ObjectAtTimeArray *>
{
public:
			 SIM_ObjectAtTimeMatrix() :
			     SIM_PtrArraySorted<SIM_ObjectAtTimeArray *>(compareObjectId)
			 {}
    virtual		~SIM_ObjectAtTimeMatrix()
			 { deleteAllEntries(); }

    void		 deleteAllEntries();

    /// Returns the total number of SIM_ObjectAtTime entries in all the
    /// arrays stored in this matrix.
    int			 fullCount() const;

    /// Finds or creates a SIM_ObjectAtTimeArray for the object, and adds
    /// the specified entry to the array.
    void		 addObjectAtTime(SIM_ObjectAtTime *objectattime);

    /// This function is used for sorting the objects in the array.
    /// It compares objects using the object id values.
    static int		 compareObjectId(SIM_ObjectAtTimeArray * const *,
					 SIM_ObjectAtTimeArray * const *);

};


#endif

