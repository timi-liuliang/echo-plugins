/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        BM_InputSelector.h ( Direct Manipulation Library, C++)
 *
 * COMMENTS:
 * 	The base class for objects that select the inputs of an op.
 */

#ifndef __BM_InputSelector_h__
#define __BM_InputSelector_h__

#include "BM_API.h"
#include <PI/PI_ResourceTemplate.h>
#include <SI/AP_Interface.h>

class BM_View;
class OP_Node;

class BM_API BM_InputSelector : public AP_Interface
{
public:
	     BM_InputSelector(BM_View &viewer,
			      PI_SelectorTemplate &templ);
    virtual ~BM_InputSelector();

    virtual const char	*className() const;

    // The selector handles mouse events and modifies the selection
    // accordingly.  Return 1 if the event was consumed and 0 otherwise.
    virtual int		 handleMouseEvent(UI_Event *event);

    // Return 1 if the event was consumed and 0 otherwise.
    virtual int		 handleMouseWheelEvent(UI_Event *event);

    // Return true if handled.
    virtual bool	 handleDoubleClickEvent(UI_Event *event);

    virtual void	 handleNodeDeleted(OP_Node &node);

	  PI_SelectorTemplate	&getTemplate()       { return myTemplate; }
    const PI_SelectorTemplate	&getTemplate() const { return myTemplate; }

    // This method allows the resource manager can set the viewer (and
    // associated scene manager) to something different if a selector is
    // reused.  It should not be used otherwise.  A nil pointer is allowed so
    // the select can remove any of its interests.  However, methods of the
    // selector should never be called when its viewer is nil.  The method is
    // virtual so descendants can also have a chance to clean up.
    virtual void	 setViewer(BM_View *viewer);

    // Do any special rendering required by this selector.
    virtual void	 doRender(RE_Render *r, int x, int y, int ghost);

    virtual void	 overlayRender(RE_Render *r, int x, int y);
    
    // This method is called by the resource manager after the selector is
    // created.
    void		 initializeUI();

    // Some states use this virtual to query whether or not this selector
    // needs locate mouse messages (mouse moves with no buttons down).
    virtual int		 hasLocates() const { return 0; }

    // The selector needs to be notified when it starts/stops selecting
    // so it can add/remove interests (the selection type, etc.).  
    virtual void	 startSelecting();
    virtual void	 stopSelecting();

    virtual const char	*cursor() const;


    virtual bool getStateParmNames(UT_StringArray &ret, const char* prefix=nullptr ) {return false;}
    virtual bool evalStateParm(const char *name, UT_StringHolder &ret) {return false;}
    virtual bool setStateParm(const char *name, const UT_StringHolder &val) {return false;}
    virtual bool pressStateButton(const char *name) {return false;}
protected:
	  BM_View	&baseViewer()	    { return *myBaseViewer; }
    const BM_View	&baseViewer() const { return *myBaseViewer; }

    // Do the AP_Interface initialization.  This is called by initializeUI().
    // By default this method does nothing, but subclasses have the option
    // of parsing a UI file here.
    virtual void	 initApplication(UI_Manager *, int c, const char **v);

private:
    PI_SelectorTemplate	&myTemplate;

    BM_View		*myBaseViewer;
    const char		*myBumpedCursor; // cursor I'm replacing when active
};

#endif
