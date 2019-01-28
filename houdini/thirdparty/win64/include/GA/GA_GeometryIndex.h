/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GeometryIndex.h
 *
 * COMMENTS:
 */

#ifndef GA_GEOMETRYINDEX_H_
#define GA_GEOMETRYINDEX_H_

#include "GA_API.h"

#include <UT/UT_Map.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_String.h>
#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONWriter.h>

class GA_API GA_GeometryIndex
{
public:
    GA_GeometryIndex();
    virtual ~GA_GeometryIndex();

    void clear();

    /// Writes the index to a JSON reader
    /// Should be called from within an array context
    bool writeToJSON(UT_JSONWriter& writer);

    /// Reads the index from a JSON parser
    bool readFromJSON(UT_JSONParser& parser);

    /// Adds an entry to the index. Key can be an exint or a string.
    void addEntry(exint key, exint location);

    void addEntry(const char* key, exint location);

    /// Adds a subindex entry. Fill the returned index as you would
    /// the top level index. Memory is owned by the parent index.
    GA_GeometryIndex* addSubindexEntry(exint key);

    GA_GeometryIndex* addSubindexEntry(const char* key);

    /// Retrieves an entry.
    exint getEntryByInt(exint key);

    exint getEntryByString(const UT_StringRef& key);

    /// Retrieves subindex entry
    GA_GeometryIndex* getSubindexEntryByInt(exint key);

    GA_GeometryIndex* getSubindexEntryByString(const UT_StringRef& key);

    /// Returns true if the index has no entries
    bool isEmpty() const;

private:
    UT_Map<exint, exint> myIntEntries;
    UT_StringMap<exint> myStringEntries;

    UT_Map<exint, GA_GeometryIndex*> myIntSubindexEntries;
    UT_StringMap<GA_GeometryIndex*> myStringSubindexEntries;

    void deleteSubindexEntries();
};

#endif // GA_GEOMETRYINDEX_H_
