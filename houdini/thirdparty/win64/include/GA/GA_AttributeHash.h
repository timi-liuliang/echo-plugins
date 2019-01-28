/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_AttributeHash.h (GA Library, C++)
 *
 * COMMENTS:    Class to act as a hash key for hashing on attribute values
 *              and group memberships of an element.
 */

#ifndef __GA_AttributeHash_h__
#define __GA_AttributeHash_h__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_AIFCompare.h"
#include "GA_IndexMap.h"
#include <UT/UT_Map.h>

class GA_AttributeHash
{
public:
    GA_AttributeHash(const GA_IndexMap &indexmap,
                     const GA_Offset offset,
                     const GA_Attribute *const *attribs,
                     const GA_Size nattribs,
                     const GA_ElementGroup *const *groups = NULL,
                     const GA_Size ngroups = 0)
        : myIndexMap(indexmap)
        , myOffset(offset)
        , myAttribs(attribs)
        , myNAttribs(nattribs)
        , myGroups(groups)
        , myNGroups(ngroups)
        , myHash(computeHash())
    {}
    GA_AttributeHash(const GA_AttributeHash &that)
        : myIndexMap(that.myIndexMap)
        , myOffset(that.myOffset)
        , myAttribs(that.myAttribs)
        , myNAttribs(that.myNAttribs)
        , myGroups(that.myGroups)
        , myNGroups(that.myNGroups)
        , myHash(that.myHash)
    {}

    bool operator==(const GA_AttributeHash &that) const
    {
        if (!myNGroups && !myNAttribs)
            return true;

        // The group handling doesn't work properly for
        // elements in different details.
        UT_ASSERT_P(&myIndexMap.getDetail() == &that.myIndexMap.getDetail() ||
            (myNGroups == 0 && that.myNGroups == 0));

        if (&myIndexMap.getDetail() == &that.myIndexMap.getDetail())
        {
            // Check group membership
            if (myNGroups != that.myNGroups)
                return false;

            for (GA_Size i = 0; i < myNGroups; ++i)
            {
                if (myGroups[i] != that.myGroups[i])
                    return false;
            }
        }

        UT_ASSERT_P(myNAttribs == that.myNAttribs);

        // Check attributes
        const GA_Offset off0 = myOffset;
        const GA_Offset off1 = that.myOffset;
        for (GA_Size i = 0; i < myNAttribs; ++i)
        {
            const GA_Attribute *attribute = myAttribs[i];
            const GA_AIFCompare *compare = attribute->getAIFCompare();
            bool result;
            if (compare && (!compare->isEqual(
                    result, *attribute, off0, *that.myAttribs[i], off1) || !result))
                return false;
        }
        return true;
    }

    const size_t &hash() const { return myHash; }

private:
    size_t computeHash() const {
        if (!myNGroups && !myNAttribs)
            return 0;

        size_t h = 0;

        // Apply group membership
        for (GA_Size i = 0; i < myNGroups; ++i)
        {
            h ^= SYSwang_inthash(SYSpointerHash(myGroups[i]));
        }

        // Apply attributes
        const GA_Offset off = myOffset;
        for (GA_Size i = 0; i < myNAttribs; ++i)
        {
            const GA_Attribute *attribute = myAttribs[i];
            const GA_AIFCompare *compare = attribute->getAIFCompare();
            if (compare)
                h ^= SYSwang_inthash(compare->hash(*attribute, off));
        }
        return h;
    }
private:
    const GA_Offset               myOffset;
    const GA_IndexMap            &myIndexMap;
    const GA_Attribute *const    *myAttribs;
    const GA_Size                 myNAttribs;
    const GA_ElementGroup *const *myGroups;
    const GA_Size                 myNGroups;
    const size_t                  myHash;
};

struct GA_AttributeHashFunctor
{
    inline size_t operator()(const GA_AttributeHash &key) const
    {
        return key.hash();
    }
};

// These class declarations act just like templated typedefs,
// which unfortunately, C++ lacks.
template <typename ToType>
class GA_AttributeHashMap  : public UT_Map<GA_AttributeHash,  ToType, GA_AttributeHashFunctor >
{ };

#endif
