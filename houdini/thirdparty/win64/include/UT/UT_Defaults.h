/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Defaults.h (UT Library, C++)
 *
 * COMMENTS:    A class for representing default values for tuples of arbitrary length.
 */

#pragma once

#ifndef __UT_Defaults__
#define __UT_Defaults__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Storage.h"
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>
#include <string.h>

class UT_API UT_Defaults
{
public:
    SYS_FORCE_INLINE
    UT_Defaults()
    {
        initZero();
    }
    template<typename T>
    explicit SYS_FORCE_INLINE
    UT_Defaults(T value)
    {
        initValue(value);
    }
    template<typename T>
    UT_Defaults(const T *values, exint tuplesize)
    {
        UT_ASSERT_COMPILETIME(UT_StorageNum<T>::theStorage != UT_Storage::INVALID);
        if (tuplesize <= 0)
        {
            initZero();
            return;
        }

        // Check for constant value
        T constvalue = values[0];
        bool equal = true;
        for (exint i = 1; i < tuplesize; ++i)
        {
            if (values[i] != constvalue)
            {
                equal = false;
                break;
            }
        }
        if (equal)
            initValue(constvalue);
        else if (UTisIntStorage(UT_StorageNum<T>::theStorage))
        {
            myITuple = new int64[tuplesize];
            for (exint i = 0; i < tuplesize; ++i)
                myITuple[i] = int64(values[i]);
            myType.setTupleSize(tuplesize);
            myType.setStorage(UT_Storage::INT64);
        }
        else
        {
            myFTuple = new fpreal64[tuplesize];
            for (exint i = 0; i < tuplesize; ++i)
                myFTuple[i] = fpreal64(values[i]);
            myType.setTupleSize(tuplesize);
            myType.setStorage(UT_Storage::REAL64);
        }
    }
protected:
    /// NOTE: This assumes that the UT_Defaults hasn't been initialized to
    /// something that owns memory.
    SYS_FORCE_INLINE
    void initZero()
    {
        UT_ASSERT_COMPILETIME(sizeof(myI) >= sizeof(myF));
        UT_ASSERT_COMPILETIME(sizeof(myI) >= sizeof(myITuple));
        UT_ASSERT_COMPILETIME(sizeof(myI) >= sizeof(myFTuple));

        myI = 0;
        myType.setTupleSize(1);
        myType.setStorage(UT_Storage::REAL64);
    }
    /// NOTE: This assumes that the UT_Defaults hasn't been initialized to
    /// something that owns memory.
    template<typename T>
    SYS_FORCE_INLINE
    void initValue(T value)
    {
        UT_ASSERT_COMPILETIME(UT_StorageNum<T>::theStorage != UT_Storage::INVALID);
        if (UTisIntStorage(UT_StorageNum<T>::theStorage))
        {
            myI = int64(value);
            myType.setTupleSize(1);
            myType.setStorage(UT_Storage::INT64);
        }
        else
        {
            myF = fpreal64(value);
            myType.setTupleSize(1);
            myType.setStorage(UT_Storage::REAL64);
        }
    }
public:
    UT_Defaults &operator=(const UT_Defaults &that)
    {
        if (this == &that)
            return *this;

        if (getTupleSize() != 1)
            clear();

        if (that.getTupleSize() == 1)
        {
	    copy(that);
        }
        else
        {
            if (that.getStorage() == UT_Storage::REAL64)
            {
                myFTuple = new fpreal64[that.getTupleSize()];
                memcpy(myFTuple,that.myFTuple,sizeof(fpreal64)*that.getTupleSize());
            }
            else
            {
                myITuple = new int64[that.getTupleSize()];
                memcpy(myITuple,that.myITuple,sizeof(int64)*that.getTupleSize());
            }
            myType = that.myType;
        }
        return *this;
    }
    SYS_FORCE_INLINE
    UT_Defaults &operator=(UT_Defaults &&that)
    {
	copy(that);
        // Indicate that that doesn't have any memory to free.
        that.myType.setTupleSize(1);
        return *this;
    }
    SYS_FORCE_INLINE
    UT_Defaults(const UT_Defaults &that)
    {
        // Indicate that *this* doesn't have any memory to free.
        myType.setTupleSize(1);
        *this = that;
    }
    SYS_FORCE_INLINE
    UT_Defaults(UT_Defaults &&that)
    {
	copy(that);
        // Indicate that that doesn't have any memory to free.
        that.myType.setTupleSize(1);
    }
    bool operator==(const UT_Defaults &that) const
    {
        if (getTupleSize() != that.getTupleSize())
            return false;
        if (getStorage() != that.getStorage())
            return false;
        if (getTupleSize() == 1)
        {
            if (getStorage() == UT_Storage::REAL64)
                return myF == that.myF;
            return myI == that.myI;
        }
        exint tuplesize = getTupleSize();
        if (getStorage() == UT_Storage::REAL64)
        {
            for (exint i = 0; i < tuplesize; ++i)
            {
                if (myFTuple[i] != that.myFTuple[i])
                    return false;
            }
        }
        else
        {
            for (exint i = 0; i < tuplesize; ++i)
            {
                if (myITuple[i] != that.myITuple[i])
                    return false;
            }
        }
        return true;
    }
    SYS_FORCE_INLINE
    bool operator!=(const UT_Defaults &that) const
    {
        return !(*this == that);
    }
    SYS_FORCE_INLINE
    ~UT_Defaults()
    {
        clear();
    }
    SYS_FORCE_INLINE
    void clear()
    {
        if (myType.getTupleSize() > 1)
        {
            if (myType.getStorage()==UT_Storage::REAL64)
                delete [] myFTuple;
            else
                delete [] myITuple;
        }
        initZero();
    }
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (myType.getTupleSize() > 1)
        {
            if (myType.getStorage()==UT_Storage::REAL64)
                mem += sizeof(fpreal64)*myType.getTupleSize();
            else
                mem += sizeof(int64)*myType.getTupleSize();
        }
        return mem;
    }

    SYS_FORCE_INLINE
    bool isZero() const
    {
        return myI == 0;
    }
    SYS_FORCE_INLINE
    UT_Storage getStorage() const
    {
        return myType.getStorage();
    }
    SYS_FORCE_INLINE
    exint getTupleSize() const
    {
        return myType.getTupleSize();
    }

    fpreal64 getF(exint i = 0) const
    {
        // Zero is such a common default that it deserves a special case
        if (isZero())
            return 0;

        // It's also extremely common that all components have
        // the same default, so we only need one entry.
        if (myType.getTupleSize()==1)
        {
            if (myType.getStorage()==UT_Storage::REAL64)
                return myF;
            return fpreal64(myI);
        }

        if (i >= myType.getTupleSize())
            i = myType.getTupleSize()-1;
        else if (i < 0)
            i = 0;
        if (myType.getStorage()==UT_Storage::REAL64)
            return myFTuple[i];
        return fpreal64(myITuple[i]);
    }

    int64 getI(exint i = 0) const
    {
        if (isZero())
            return 0;
        if (myType.getTupleSize()==1)
        {
            if (myType.getStorage()==UT_Storage::REAL64)
                return int64(myF);
            return myI;
        }
        if (i >= myType.getTupleSize())
            i = myType.getTupleSize()-1;
        else if (i < 0)
            i = 0;
        if (myType.getStorage()==UT_Storage::REAL64)
            return int64(myFTuple[i]);
        return myITuple[i];
    }

private:
    void copy(const UT_Defaults &that)
    {
        UT_ASSERT_COMPILETIME(sizeof(myI) >= sizeof(myF));
        UT_ASSERT_COMPILETIME(sizeof(myI) >= sizeof(myITuple));
        UT_ASSERT_COMPILETIME(sizeof(myI) >= sizeof(myFTuple));

	myI = that.myI;
	myType = that.myType;
    }

protected:
    union {
        fpreal64 *myFTuple;
        int64 *myITuple;
        fpreal64 myF;
        int64 myI;
    };
    UT_TupleType<-1,void> myType;
};

#endif
