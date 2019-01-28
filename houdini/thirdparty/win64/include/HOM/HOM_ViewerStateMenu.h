/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef HOM_ViewerStateMenu_h
#define HOM_ViewerStateMenu_h

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"
#include "HOM_Defines.h"

#include <string>
#include <vector>
#include <utility>

SWIGOUT(%rename(ViewerStateMenu) HOM_ViewerStateMenu;)
SWIGOUT(%feature("notabstract") HOM_ViewerStateMenu;)

class HOM_API HOM_ViewerStateMenu
{
public:
#ifdef SWIG
%extend
{   
    HOM_ViewerStateMenu(std::string const& handle, std::string const& label)
    { 
	return HOM().newViewerStateMenu(handle, label); 
    }
}
#else
    HOM_ViewerStateMenu()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_ViewerStateMenu()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%kwargs addToggleItem;)
    virtual void addToggleItem( 
	std::string const & handle,
	std::string const & label,
	bool default_value,
	std::string const & hotkey=std::string() ) = 0;

    SWIGOUT(%kwargs addActionItem;)
    virtual void addActionItem( 
	std::string const & handle,
	std::string const & label,
	std::string const & hotkey=std::string()) = 0;

    SWIGOUT(%kwargs addSeparator;)
    virtual void addSeparator( ) = 0;

    SWIGOUT(%kwargs addRadioStrip;)
    virtual void addRadioStrip( 
	std::string const & handle,
	std::string const & label,
	std::string const & default_value ) = 0;

    SWIGOUT(%kwargs addRadioStripItem;)
    virtual void addRadioStripItem( 
	std::string const & parent, 
	std::string const & handle,
	std::string const & label,
	std::string const & hotkey=std::string() ) = 0;

    virtual void addMenu(HOM_ViewerStateMenu const& sub_menu) = 0;
 };

#endif // HOM_ViewerStateMenu_h

