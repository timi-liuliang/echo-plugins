/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_GUSelectionProxy.h ( GUI Library, C++)
 *
 * COMMENTS:
 * 	A helper class for allowing a GUI_DetailLook to indirectly reference a
 * 	GU_SelectionHandle.  The indirection lets us change the GU_Selection
 * 	assigned to a GUI_DetailLook without having to reference a different
 * 	proxy.  Finally, this class also tracks the GUI_DetailLook referring
 * 	to this proxy.
 */

#ifndef GUI_GUSelectionProxy_h
#define GUI_GUSelectionProxy_h

#include <GU/GU_SelectType.h>
#include "GUI_GUSelectionProxyHandle.h"
#include "GUI_API.h"

class GUI_DetailLook;

class GUI_API GUI_GUSelectionProxy
{
public:
     GUI_GUSelectionProxy();
    ~GUI_GUSelectionProxy();

     void setSelection(GU_SelectionHandle selection)
					{ mySelection = selection; }
     GU_SelectionHandle	getSelection() const
					{ return mySelection; }

     void setLook(GUI_DetailLook *look)
					{ myLook = look; }
     GUI_DetailLook *getLook() const
					{ return myLook; }
private:
     GU_SelectionHandle		 mySelection;
     GUI_DetailLook		*myLook;
};


#endif
