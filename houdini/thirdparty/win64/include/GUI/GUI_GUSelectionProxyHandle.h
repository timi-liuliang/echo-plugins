/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_GUSelectionProxyHandle.h ( GUI Library, C++)
 *
 * COMMENTS:
 * 	Convenience typedef for a shared ptr for GUI_GUSelectionProxy.
 */

#ifndef GUI_GUSelectionProxyHandle_h
#define GUI_GUSelectionProxyHandle_h

class GUI_GUSelectionProxy;
#include <UT/UT_SharedPtr.h>

typedef UT_SharedPtr<GUI_GUSelectionProxy> GUI_GUSelectionProxyHandle;

#endif
