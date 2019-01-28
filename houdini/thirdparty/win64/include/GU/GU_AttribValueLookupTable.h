/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GU_AttribValueLookupTable.h (GU Library, C++)
 *
 * COMMENTS:    File accompanying GU_Detail.h, so that it doesn't need to include UT_StringMap.h or UT_Map.h
 *
 */

#pragma once

#ifndef __GU_AttribValueLookupTable_h__
#define __GU_AttribValueLookupTable_h__

#include "GU_Detail.h"

#include <UT/UT_IntArray.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>

class GU_Detail::AttribValueLookupTable
{
public:
    AttribValueLookupTable(const GA_Attribute *attrib);

    ~AttribValueLookupTable();

    exint entries() const { return SYSmax(myIntTable.size(), myStringTable.size()); }

    int getIntValue(int idx) const
    {
	if (idx < 0 || idx >= myIntValues.size())
	    return -1;
	return myIntValues(idx);
    }
    const UT_StringHolder &getStringHolder(int idx) const
    {
	if (idx < 0 || idx >= myStringValues.size())
	    return UT_StringHolder::theEmptyString;
	return myStringValues(idx);
    }
    const GA_OffsetList *getIntOffsets(int v) const
    {
	auto it = myIntTable.find(v);
	if (it == myIntTable.end())
	    return 0;
	return it->second;
    }
    const GA_OffsetList *getStringOffsets(const UT_StringRef &v) const 
    {
        auto it = myStringTable.find(v);
        if (it == myStringTable.end())
            return nullptr;
        return it->second;
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;

        mem += myStringValues.getMemoryUsage(false);
        mem += myIntValues.getMemoryUsage(false);

        mem += myStringTable.getMemoryUsage(false);
        for (UT_StringMap<GA_OffsetList *>::const_iterator it = myStringTable.begin(); it != myStringTable.end(); ++it)
            mem += it->second->getMemoryUsage(true);

        mem += UTgetMemoryUsage(myIntTable, false);
        for (UT_Map<int, GA_OffsetList *>::const_iterator it = myIntTable.begin(); it != myIntTable.end(); ++it)
            mem += it->second->getMemoryUsage(true);

        return mem;
    }

private:

    UT_StringMap<GA_OffsetList *> myStringTable;
    UT_Map<int, GA_OffsetList *> myIntTable;
    UT_StringArray myStringValues;
    UT_IntArray myIntValues;
};

class GU_Detail::AttribSingleValueLookupTable
{
public:
    AttribSingleValueLookupTable(const GA_Attribute *attrib);

    ~AttribSingleValueLookupTable();

    exint entries() const { return SYSmax(myIntTable.size(), myStringTable.size()); }

    GA_Offset getIntOffset(int v) const
    {
	auto it = myIntTable.find(v);
	if (it == myIntTable.end())
	    return GA_INVALID_OFFSET;
	return it->second;
    }

    GA_Offset getStringOffset(const UT_StringRef &v) const
    {
        auto it = myStringTable.find(v);
        if (it == myStringTable.end())
            return GA_INVALID_OFFSET;
        return it->second;
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;

        mem += myStringTable.getMemoryUsage(false);
        mem += UTgetMemoryUsage(myIntTable, false);

        return mem;
    }

private:
    UT_StringMap<GA_Offset> myStringTable;
    UT_Map<int, GA_Offset> myIntTable;
};

#endif
