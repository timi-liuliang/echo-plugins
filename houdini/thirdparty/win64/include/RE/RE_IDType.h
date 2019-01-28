/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_IDType.h ( RE Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RE_IDType__
#define __RE_IDType__

// Platform-specific implemetation typedefs go here.
// At the moment we only have the Qt implementation.

// Qt typedefs.
// QWidget * => window id
// QCursor * => cursor id
typedef void *RE_IDType;

#define RE_INVALID_ID	nullptr
inline bool	REisValid(const RE_IDType id) { return id != RE_INVALID_ID; }


#endif // __RE_IDType__
