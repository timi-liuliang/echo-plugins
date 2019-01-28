/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Input.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _OP_ConnectorId_h_
#define _OP_ConnectorId_h_

#include "OP_API.h"

#include <string>

class OP_API OP_ConnectorId
{
public:

    OP_ConnectorId(int id)
    {
	set(id);
    }

    OP_ConnectorId()
    {
	reset();
    }

    int toBaseType() const
    {
	return myId;
    }

    void set(int id)
    {
	myId = id;
    }

    void reset()
    {
	myId = -1;
    }

    bool isValid() const
    {
	return myId >= 0;
    }

    bool operator==(const OP_ConnectorId& other_id) const
    {
	return myId == other_id.myId;
    }

    bool operator!=(const OP_ConnectorId& other_id) const
    {
	return !(*this == other_id);
    }

    OP_ConnectorId& operator=(const OP_ConnectorId& other_id)
    {
	myId = other_id.myId;
	return *this;
    }

    bool operator<(const OP_ConnectorId& other) const
    {
	return myId < other.myId;
    }

private:

    int myId;
};

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <>
struct DefaultClearer<OP_ConnectorId>
{
    static void clear(OP_ConnectorId &v) { v.reset(); }
    static bool isClear(const OP_ConnectorId &v) { return !v.isValid(); }
    static void clearConstruct(OP_ConnectorId *p)
    {
        new ((void *)p) OP_ConnectorId();
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif
