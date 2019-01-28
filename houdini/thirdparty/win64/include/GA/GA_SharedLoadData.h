/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_SharedLoadData.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_SharedLoadData__
#define __GA_SharedLoadData__

#include "GA_API.h"
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>

/// Class to hold shared data during the loading process.
///
/// Some primitive types store shared data between them.  This class is used as
/// an interface to retain data during the loading/saving process. It also
/// Facilitates the delayed loading of this shared data.
///
/// <b>Saving:</b>@n
/// When saving, primitives will have a "pre-save" pass.  During this pass,
/// they can add "shared" data to the GA_SaveMap.  The primitive can check to
/// see if the data is already stored in the map, if not, it can add a
/// GA_SharedLoadData to the map (given a unique key).  A single JSON will be
/// be saved.  Subsequent primitives, should detect that the data is stored so
/// that only a single copy of the shared data is saved in the map.
///
/// When the primitive saves its local data, it can make reference to the named
/// shared data.
///
/// <b>Loading:</b>@n
/// Dring loading time GA_SharedDataHandles are created to represent
/// all the shared data. Primitives then ask the GA_LoadMap for a copy
/// of these handles.
/// Whenever the primitive needs the data it is requested from the shared data
/// handle and returned as a GA_SharedLoadData
class GA_API GA_SharedLoadData
{
public:
    GA_SharedLoadData();
    virtual ~GA_SharedLoadData();

    /// Returns the key that is used to refer to this shared data
    virtual UT_StringHolder getSharedDataKey() const = 0;
private:
};

#endif
