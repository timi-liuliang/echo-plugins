/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Callbacks.h ( PXL Library, C++)
 *
 * COMMENTS:
 */
#ifndef PXL_CALLBACKS_H
#define PXL_CALLBACKS_H

#include "PXL_API.h"

PXL_API extern bool (*PXL_IsTexture_cb)(void *);
PXL_API extern void (*PXL_DeleteTextures_cb)(void *);

#endif
