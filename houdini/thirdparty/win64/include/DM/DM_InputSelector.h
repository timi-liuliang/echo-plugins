/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        DM_InputSelector.h ( Direct Manipulation Library, C++)
 *
 * COMMENTS:
 * 	The base class for objects that select the inputs of a sop or obj.
 */

#ifndef __DM_InputSelector_h__
#define __DM_InputSelector_h__

#include "DM_API.h"
#include <GUI/GUI_RenderCommon.h>
#include <BM/BM_InputSelector.h>
#include <GEO/GEO_PrimTypeCompat.h>
#include <GU/GU_SelectType.h>

class DM_Viewport;
class UI_Menu;

class DM_API DM_InputSelector : public BM_InputSelector
{
public:
	     DM_InputSelector(BM_View &viewer,
			      PI_SelectorTemplate &templ);
    virtual ~DM_InputSelector();

    virtual const char	*className() const;

    // Gets a string representing the current selection.
    virtual void	 generateAllSelectionStrings(UT_String &sel_string,
				bool only_one_gdp,
				bool force_numeric,
				bool consume_selections);

    // Override this virtual to allow other classes to programatically force
    // this selector to convert any current pre-seleciton in the viewport to
    // a real selection on the geometry. Does nothing by default.
    virtual void	 commitViewportPreSelection(UI_Event *event,
				GU_SelectionRule sel_rule);

    // Override this virtual to let the selector expose a list of looks
    // that the selector can pick from.
    virtual void	 getSelectableLooks(GUI_DetailList &look_list);

    // Override to return a custom toolbar UI 
    virtual UI_Feel	*getCustomToolbox() const { return nullptr; }
    
    // Selectors respond to keyboard events to know when to finish a selection
    // stage and possibly enter the next.
    virtual int		 handleKeyEvent(UI_Event *event,
					DM_Viewport &viewport) = 0;
    
     // Return the feel containing the RMB menu. Subclasses implement this.
    virtual UI_Menu	*getSelectorMenu() const = 0;
    virtual UI_Menu	*getSelectorHotkeyMenu() const
			    { return nullptr; }
    
    virtual bool	 handleTransitoryKey(const UI_Event &event)
    			    { return false; }
    
    // Override this virtual to return the types of primitives that this
    // selector is interested in.
    virtual GEO_PrimTypeCompat::TypeMask primMask() const;
};

#endif
