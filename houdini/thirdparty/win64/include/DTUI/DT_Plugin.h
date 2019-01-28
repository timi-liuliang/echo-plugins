/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DT_Plugin.h ( DT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DT_Plugin__
#define __DT_Plugin__

#include "DTUI_API.h"
#include "DT_ViewportProvider.h"
#include <OP/OP_Output.h>
#include <OP/OP_Value.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Error.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_Set.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_WorkBuffer.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Visibility.h>

#include <float.h>
#include <hboost/any.hpp>
#include <iosfwd>

#define DT_LODCHOOSER_PLUGIN_NAME "Object Appearance"
#define DT_LIGHTBANK_PLUGIN_NAME "Light Bank"
#define DT_STYLER_PLUGIN_NAME "Material Style Sheets"

class PRM_Parm;
class PRM_Template;
class DD_ChoiceList;
class DD_Source;
class UT_WorkBuffer;

// ======================================================================
// Menu items

/// A single menu item object. A menu item consists of a label, which is used
/// for display, and a token, which is used for setting a value, and is not
/// user-visible. The token should be globally unique for the top menu and
/// all its sub-menus.
/// A menu item can also be marked as having sub-menu. This, however, only
/// applies to node menus, not parameter menus. For parameter menus, the
/// flag is ignored.
class DTUI_API DT_MenuItem
{
public:
    DT_MenuItem(const UT_StringHolder &token,
		const UT_StringHolder &label);
    
    const UT_StringHolder	&token() const;
    const UT_StringHolder	&label() const;
    
    bool operator==(const DT_MenuItem &other) const;

    static int compare(const DT_MenuItem *m1, const DT_MenuItem *m2);

private:
    UT_StringHolder		 myToken;
    UT_StringHolder		 myLabel;
};

typedef UT_Array<DT_MenuItem> DT_MenuItemList;

class DTUI_API DT_NodeMenuItem :
    public DT_MenuItem
{
public:
    enum Flags
    {
	MI_None			= 0x00,
	MI_Disabled		= 0x01,
	MI_ToggleUnchecked	= 0x02,
	MI_ToggleChecked	= 0x06,
	MI_HasSubMenu		= 0x08
    };

    DT_NodeMenuItem(const UT_StringHolder &token,
		    const UT_StringHolder &label,
		    const UT_StringHolder &hotkey_base = UT_StringHolder(),
		    Flags flags = MI_None);

    const UT_StringHolder	&hotkey() const;
    bool			 enabled() const;
    bool			 isToggle() const;
    bool			 toggledOn() const;
    bool			 hasSubmenu() const;

private:
    Flags			 myFlags;
    UT_StringHolder		 myHotkey;
};

static inline DT_NodeMenuItem::Flags
operator|(DT_NodeMenuItem::Flags a, DT_NodeMenuItem::Flags b)
    { return DT_NodeMenuItem::Flags((int)a | (int)b); }

typedef UT_Array<DT_NodeMenuItem> DT_NodeMenuItemList;

// ======================================================================
// Type definitions

/// Opaque data type to pass values of any type across boundaries.
class DT_Value : private hboost::any
{
public:
    /// Create a new, empty DT_Value object. A value can be assigned to it
    /// later using the assignment operator.
    DT_Value() {}

    /// Create a new DT_Value from the given value. A copy of the incoming
    /// value will be stored, rather than a reference to it.
    template<typename T>
    DT_Value(const T &value) : hboost::any(value) {} 

    /// Assign a new value to this DT_Value. Any previous value will be 
    /// destroyed, and a copy made of the incoming value.
    template<typename T>
    DT_Value 	&operator=(const T &value)
		{
		    hboost::any::operator=(value);
		    return *this;
		}
    
    /// An explicit copy function to copy in a new DT_Value. The call is 
    /// explicit, rather than using the implicit assignment operator, to avoid
    /// arbitrary copying, which can get expensive for large objects.
    void 	 copy(const DT_Value &v) 
		 { hboost::any::operator=(static_cast<const hboost::any &>(v)); }
    
    /// Reset the DT_Value object to be empty.
    void 	 clear() 
		 { hboost::any::operator=(hboost::any()); }
    
    /// Return the std::type_info for the stored value. This can be used
    /// to verify the type contained within. E.g.:
    /// \code
    /// DT_Value foo(2);
    /// if (foo.type() == typeid(int)) print("Yep. That's an int alright.");
    /// \endcode
    using hboost::any::type;
    
    /// Return a reference to the value contained within. It is important
    /// that the type matches, otherwise an exception is thrown. 
    template<typename T>
    const T 	&get() const { return hboost::any_cast<const T &>(*this); }

private:
    DT_Value(const DT_Value &);
    DT_Value &operator=(const DT_Value &);
};

// ======================================================================
// Parameter definitions

/// The base class for a parameter definition. Parameter definitions are used
/// to construct parameters for either tree values or plugin options.
///
/// \note This class cannot be instantiated directly, use the derived types to 
/// create specific parameter types.
///
/// \sa DT_ParmDefToggle, DT_ParmDefInteger, DT_ParmDefFloat, DT_ParmDefColor,
///	DT_ParmDefString, DT_ParmDefMenu, DT_ParmDefPath, DT_ParmDefButton,
///	DT_ParmDefGrouped
class DTUI_API DT_ParmDef : public UT_NonCopyable
{
public:
    /// Create a new DT_ParmDef object from a PRM_Template object. 
    /// Note that not all types of operator parameters can be converted, or
    /// represented, by DT_ParmDef. In that case it returns NULL.
    static DT_ParmDef 	*createFromTemplate(int id, const PRM_Template &tpl);
    
    /// Returns a list of menu items from a PRM_Template object. This can be
    /// used in case the menu is dynamically generated based on the parameter
    /// it is being evaluated for.
    static bool 	 getMenuItemsFromTemplate(const PRM_Template &tpl, 
                	                          PRM_Parm &parm,
                	                          DT_MenuItemList &items);
     
    virtual ~DT_ParmDef() {}
    
    enum Type
    {
        T_TOGGLE,		/// Toggle type. \c DT_ParmDefToggle
        T_INTEGER,		/// Integer type. \c DT_ParmDefInteger
        T_FLOAT,		/// Floating point type. \c DT_ParmDefFloat
        T_COLOR,		/// Color type. \c DT_ParmDefColor
        T_STRING,		/// String type. \c DT_ParmDefString
        T_MENU,			/// Menu type. \c DT_ParmDefMenu
        T_PATH,			/// Path type. \c DT_ParmDefPath
        T_BUTTON,		/// Button type. \c DT_ParmDefButton
        T_GROUPED		/// Grouped parms type. \c DT_ParmDefGrouped
    };

    enum OptionType
    {
	OT_OPTION_ONLY,		/// Only show plugin option in dialog (default)
	OT_OPTION_AND_TOOLBAR,	/// Show option in both toolbar and dialog.
	OT_TOOLBAR_ONLY,	/// Only show option in toolbar.
    };

    /// Return the unique id used for this parameter definition. This id is
    /// passed to DT_Plugin::getParameterValue, DT_Plugin::setParameterValue,
    /// DT_Plugin::getOptionValue and DT_Plugin::setOptionValue.
    int			 id() const { return myId; }
    
    /// Returns the type id of the parameter definition. See the derived
    /// definition classes for details on each type.
    Type	 	 type() const { return myType; }
    
    /// Returns a string version of the type. Used mostly for debugging
    /// purposes.
    const char		*typeName() const;
    
    /// Set a name for this parameter definition.
    DT_ParmDef		*setName(const UT_StringHolder &name);
    
    /// Returns the name of the parameter definition.
    const UT_StringHolder &name() const { return myName; }
    
    /// Show an icon, instead of a headet text / label.
    DT_ParmDef		*setIcon(const UT_StringHolder &icon);
    const UT_StringHolder &icon() const { return myIcon; }
    
    /// Set the tooltip to show on the header column, for parameters, or on
    /// the label/input for options.
    DT_ParmDef		*setTooltip(const UT_StringHolder &str);
    const UT_StringHolder &tooltip() const { return myTooltip; }
    
    /// This parameter is read-only and cannot be edited. The plugin does not
    /// have to handle it in DT_Plugin::setParameterValue or 
    /// DT_Plugin::setOptionValue.
    DT_ParmDef		*setReadOnly();
    bool		 readOnly() const { return myReadOnly; }
    
    DT_ParmDef		*setOptionType(OptionType opt_type);
    OptionType		 optionType() const { return myOptionType; }
    
    /// Set an alternative name to use on this parameter's label in the toolbar,
    /// if this parameter definition is used for both the option dialog and
    /// the toolbar. This should ideally be a more compact version of the more 
    /// descriptive name used in the dialog.
    DT_ParmDef		*setToolbarName(const UT_StringHolder &toolbar_name);
    const UT_StringHolder &toolbarName() const;
    
    /// The parameter can be in an indeterminate state. Ignored for options.
    DT_ParmDef		*setCanBeIndeterminate();
    bool		 canBeIndeterminate() const
			 { return myCanBeIndeterminate; }

    /// Returns true if multiple selected rows can set this column value at
    /// the same time.
    virtual bool	 allowsMultiValueSet() const
			 { return !readOnly(); }

    virtual bool operator==(const DT_ParmDef &other) const = 0;

protected:
    DT_ParmDef(int id, const UT_StringHolder &name, Type parm_type);

    int			myId;
    UT_StringHolder	myName;
    UT_StringHolder	myIcon;
    Type		myType;
    UT_StringHolder	myTooltip;
    OptionType		myOptionType;
    UT_StringHolder	myToolbarName;
    bool		myReadOnly;
    bool		myCanBeIndeterminate;
};

typedef UT_SharedPtr<DT_ParmDef> DT_ParmDefPtr;
typedef UT_SharedPtr<const DT_ParmDef> DT_ParmDefHandle;
typedef UT_Array<DT_ParmDefHandle> DT_ParmDefList;


/// Defines a boolean parameter, represented by a toggle.
///
/// When used with DT_Plugin::setParameterValue, DT_Plugin::getParameterValue,
/// DT_Plugin::setOptionValue, and DT_Plugin::getOptionValue, the \c value
/// argument should be of type \c bool.
class DTUI_API DT_ParmDefToggle : public DT_ParmDef
{
    DT_ParmDefToggle(int id, const UT_StringHolder &name);
public:
    /// Create a new DT_ParmDefToggle type with a given id and name. 
    static DT_ParmDefToggle *create(int id, const UT_StringHolder &name);
    
    /// Automatically cast the base class definition to this type.
    static const DT_ParmDefToggle &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_TOGGLE); 
		          return static_cast<const DT_ParmDefToggle &>(def); 
		        }

    /// This toggle acts like a radio button. In that case, multi-selection
    /// parameter change does not apply and the toggle is drawn like a radio
    /// button.
    DT_ParmDefToggle 	*setRadio();
    bool		 isRadio() const;
    
    /// Sets the icon to use for the toggle button itself instead of the 
    /// standard UI toggle. For indeterminate states, the icon will be drawn
    /// slightly grayed out.
    /// \note Ignored for radio buttons.
    DT_ParmDefToggle	*setToggleIcon(const UT_StringHolder &icon);
    
    /// Return the value of the toggle icon. 
    const char		*toggleIcon() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    void		 copyValue(bool src, DT_Value &dst) const;
    
    /// Copy a value from DT_Value opaque type to a concrete type.
    void		 copyValue(const DT_Value &src, bool &dst) const;

    /// Dont allow triggering multiple radio buttons with one click.
    virtual bool	 allowsMultiValueSet() const
			 {
			     if (isRadio())
				 return false;
			     return DT_ParmDef::allowsMultiValueSet();
			 }

    virtual bool operator==(const DT_ParmDef &other) const;

private:
    bool		myIsRadio;
    UT_StringHolder	myToggleIcon;
};


/// Defines an integer vector parameter, represented by an input field. The
/// integer vector can range from a single entry, to four.
///
/// When used with DT_Plugin::setParameterValue, DT_Plugin::getParameterValue,
/// DT_Plugin::setOptionValue, and DT_Plugin::getOptionValue, the \c value
/// argument type is according to the following table:
class DTUI_API DT_ParmDefInteger : public DT_ParmDef
{
    DT_ParmDefInteger(int id, const UT_StringHolder &name);

public:
    /// Create a new integer vector parameter definition with a given id and
    /// a name.
    /// 
    static DT_ParmDefInteger *create(int id, const UT_StringHolder &name);
    
    /// Automatically cast the base class definition to this type.
    static const DT_ParmDefInteger &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_INTEGER); 
		          return static_cast<const DT_ParmDefInteger &>(def); 
		        }

    /// @{
    /// Set/get the vector size of the integer. Valid values are 1-4.
    /// Invalid values are clamped.
    DT_ParmDefInteger 	*setVectorSize(int32 size);
    int32		 vectorSize() const;
    /// @}

    /// @{
    /// Set get the allowed range for all entries of the integer vector.
    /// The min/max values are inclusive.
    DT_ParmDefInteger 	*setRange(int32 min, int32 max);
    void		 getRange(int32 &min, int32 &max) const;
    /// @}
    
    /// @{
    /// Specify whether the range should be locked at either, or both, ends.
    /// A locked range will clamp on the locked side.
    DT_ParmDefInteger 	*setRangeLock(bool min, bool max);
    void		 getRangeLock(bool &min, bool &max) const;
    /// @}
    
    /// Set display label names for each vector component (e.g. "X"/"Y"/"Z" or
    /// "U"/"V"). The size of the label array should equal to the size of the
    /// vector size of the parameter. Therefore, it's important to call
    /// setVectorSize prior to this call.
    DT_ParmDefInteger	*setLabelNames(const UT_StringArray &labels);

    /// Return the list of label names for the vector components. If no labels
    /// were added, returns the empty array.
    const UT_StringArray &labelNames() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    /// The number of entries passed should match the return value of 
    /// vectorSize(). 
    void		 copyValue(const int *src, DT_Value &dst) const;
    
    /// Copy a value from DT_Value opaque type to a concrete type.
    /// The number of entries in the dst storage should match the return value 
    /// of vectorSize().
    void		 copyValue(const DT_Value &src, int *dst) const;
    

    virtual bool 	 operator==(const DT_ParmDef &other) const;

private:
    int32		 myVectorSize;
    int32		 myRangeMin, myRangeMax;
    bool		 myRangeLockMin, myRangeLockMax;
    UT_StringArray 	 myLabelNames;
};


class DTUI_API DT_ParmDefFloat : public DT_ParmDef
{
    DT_ParmDefFloat(int id, const UT_StringHolder &name);

public:
    static DT_ParmDefFloat *create(int id, const UT_StringHolder &name);
    
    /// Automatically cast the base class definition to this type.
    static const DT_ParmDefFloat &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_FLOAT); 
		          return static_cast<const DT_ParmDefFloat &>(def); 
		        }

    /// Set the vector size of the float. Valid values are 1-4.
    /// Invalid values are clamped.
    DT_ParmDefFloat	*setVectorSize(int32 size);
    int32		 vectorSize() const;

    DT_ParmDefFloat 	*setRange(fpreal min, fpreal max);
    void		 getRange(fpreal &min, fpreal &max) const;
    
    DT_ParmDefFloat 	*setRangeLock(bool min, bool max);
    void		 getRangeLock(bool &min, bool &max) const;
    
    /// Set display label names for each vector component (e.g. "X"/"Y"/"Z" or
    /// "U"/"V"). The size of the label array should equal to the size of the
    /// vector size of the parameter. Therefore, it's important to call
    /// setVectorSize prior to this call.
    DT_ParmDefFloat	*setLabelNames(const UT_StringArray &labels);

    /// Return the list of label names for the vector components. If no labels
    /// were added, returns the empty array.
    const UT_StringArray &labelNames() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    /// The number of entries passed should match the return value of 
    /// vectorSize().
    void		 copyValue(const fpreal *src, DT_Value &dst) const;
    
    /// Copy a value from DT_Value opaque type to a concrete type.
    /// The number of entries in the dst storage should match the return value 
    /// of vectorSize().
    void		 copyValue(const DT_Value &src, fpreal *dst) const;

    virtual bool 	 operator==(const DT_ParmDef &other) const;
private:
    int32		 myVectorSize;
    fpreal		 myRangeMin, myRangeMax;
    bool		 myRangeLockMin, myRangeLockMax;
    UT_StringArray 	 myLabelNames;
};


class DTUI_API DT_ParmDefColor : public DT_ParmDef
{
    DT_ParmDefColor(int id, const UT_StringHolder &name);
public:
    static DT_ParmDefColor *create(int id, const UT_StringHolder &name);
    
    /// Automatically cast the base class definition to this type.
    static const DT_ParmDefColor &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_COLOR); 
		          return static_cast<const DT_ParmDefColor &>(def); 
		        }

    DT_ParmDefColor 	*setIncludeAlpha(bool include_alpha);
    bool		 includeAlpha() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    /// The number of entries in src should be 3 for colors that don't 
    /// include alpha, and 4 for colors that do.
    void		 copyValue(const fpreal *src, DT_Value &dst) const;
    
    /// Copy a value from DT_Value opaque type to a concrete type.
    /// The number of entries in dst should be 3 for colors that don't 
    /// include alpha, and 4 for colors that do.
    void		 copyValue(const DT_Value &src, fpreal *dst) const;
    
    virtual bool operator==(const DT_ParmDef &other) const;
private:
    bool	myIncludeAlpha;
};


class DTUI_API DT_ParmDefString : public DT_ParmDef
{
    DT_ParmDefString(int id, const UT_StringHolder &name);
public:
    enum StringType
    {
	ST_ANY,
	ST_IDENTIFIER,
	ST_PATH
    };

    static DT_ParmDefString *create(int id, const UT_StringHolder &name);
    
    /// Automatically cast the base class definition to this type.
    static const DT_ParmDefString &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_STRING); 
		          return static_cast<const DT_ParmDefString &>(def); 
		        }

    DT_ParmDefString 	*setStringType(StringType type);
    StringType		 stringType() const;

    DT_ParmDefString 	*setHasMenu();
    bool		 hasMenu() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    void		 copyValue(const char *src,
				DT_Value &dst) const;
    /// Copy a value from DT_Value opaque type to a concrete type.
    void		 copyValue(const DT_Value &src,
				UT_StringHolder &dst) const;

    virtual bool operator==(const DT_ParmDef &other) const;

private:
   StringType	 myStringType;
   bool		 myHasMenu;
};


class DTUI_API DT_ParmDefMenu : public DT_ParmDef
{
    DT_ParmDefMenu(int id, const UT_StringHolder &name);
public:
    /// Create a new menu parameter definition. By default the menu is marked
    /// as dynamic, requiring the implementation of either 
    /// DT_Plugin::getOptionMenuItems, DT_Plugin::getParameterMenuItems, or
    /// both.
    static DT_ParmDefMenu *create(int id, const UT_StringHolder &name);
    
    /// Automatically cast the base class definition to this type.
    static const DT_ParmDefMenu &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_MENU); 
		          return static_cast<const DT_ParmDefMenu &>(def); 
		        }

    /// Set this menu to be a static menu with a fixed set of menu entries.
    DT_ParmDefMenu 	*setStaticMenu(const DT_MenuItemList &items);
    
    /// Return the list of static menu items for this parameter definition.
    /// If the menu is marked as dynamic, this list will be empty.
    const DT_MenuItemList &staticMenu() const;
    
    /// Mark this menu as being dynamically generated. Any existing list of
    /// static menu items will be deleted. The \c pref_width_in_em parameter
    /// is used as a guidance to set the approximate display width of the menu
    /// in units of \c em. The width in pixels is computed based on the
    /// font used.
    DT_ParmDefMenu 	*setDynamicMenu(fpreal pref_width_in_em = 12.0);

    /// Returns \c true if this menu is dynamically generated.
    bool		 isDynamicMenu() const;

    /// Mark this as an action menu, where selecting a menu entry triggers
    /// an action rather than updating the value shown in the menu. For this
    /// type of menu, the parameter value is always shown as the menu text.
    /// That menu text is not expected to show up in the list of menu items.
    DT_ParmDefMenu 	*setActionMenu();

    /// Returns \c true if this is an action menu.
    bool		 isActionMenu() const;

    // Returns the preferred width of dynamic menus in units of \c em.
    fpreal		 dynamicMenuWidth() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    void		 copyValue(const char *src,
				DT_Value &dst) const;
    /// Copy a value from DT_Value opaque type to a concrete type.
    void		 copyValue(const DT_Value &src,
				UT_StringHolder &dst) const;

    /// Don't allow triggering multiple action menus at once.
    virtual bool	 allowsMultiValueSet() const
			 {
			    if (isActionMenu())
				return false;
			    return DT_ParmDef::allowsMultiValueSet();
			 }

    virtual bool operator==(const DT_ParmDef &other) const;

private:
    DT_MenuItemList	 myStaticItems;
    fpreal		 myPrefWidthInEm;
    bool		 myDynamicMenu;
    bool		 myActionMenu;
};


class DTUI_API DT_ParmDefPath : public DT_ParmDef
{
    DT_ParmDefPath(int id, const UT_StringHolder &name);
public:
    enum PathType
    {
	PT_OPERATOR,		//!< Any scene operator
	PT_OBJECT_PATH,		//!< Objects + virtual SOP paths
	PT_PARM,		//!< Node parameter
	PT_FILE,		//!< File browser
	PT_DIRECTORY,		//!< Directory browser
    };

    static DT_ParmDefPath *create(int id, const UT_StringHolder &name);
    
    /// Automatically cast the base class definition to this type.
    static const DT_ParmDefPath &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_PATH); 
		          return static_cast<const DT_ParmDefPath &>(def); 
		        }

    DT_ParmDefPath 	*setPathType(PathType path_type);
    PathType		 pathType() const;
    
    DT_ParmDefPath 	*setOpFilter(const UT_StringHolder &op_filter);
    const char		*opFilter() const;
    
    DT_ParmDefPath 	*setMultiSelection();
    bool		 multiSelection() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    void		 copyValue(const char *src,
				DT_Value &dst) const;
    /// Copy a value from DT_Value opaque type to a concrete type.
    void		 copyValue(const DT_Value &src,
				UT_StringHolder &dst) const;

    virtual bool operator==(const DT_ParmDef &other) const;

private:
    PathType		 myPathType;
    UT_StringHolder	 myOpFilter;
    bool		 myMultiSelection;
};


class DTUI_API DT_ParmDefButton : public DT_ParmDef
{
    DT_ParmDefButton(int id, const UT_StringHolder &name);

public:
    static DT_ParmDefButton *create(int id, const UT_StringHolder &name);
    static const DT_ParmDefButton &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_BUTTON); 
		          return static_cast<const DT_ParmDefButton &>(def); 
		        }

    DT_ParmDefButton 	*setLabelText(const UT_StringHolder &text);
    const char		*labelText() const;
    
    DT_ParmDefButton 	*setLabelIcon(const UT_StringHolder &icon);
    const char		*labelIcon() const;

    /// Copy a value from a concrete type to an opaque DT_Value object.
    void		 copyValue(bool src,DT_Value &dst) const;
    /// Copy a value from DT_Value opaque type to a concrete type.
    void		 copyValue(const DT_Value &src, bool &dst) const;

    /// Don't allow triggering multiple buttons at once.
    virtual bool	 allowsMultiValueSet() const
			 { return false; }

    virtual bool operator==(const DT_ParmDef &other) const;

private:
    UT_StringHolder	 myLabelText;
    UT_StringHolder	 myLabelIcon;
};


class DTUI_API DT_ParmDefGrouped : public DT_ParmDef
{
    DT_ParmDefGrouped(int id, const UT_StringHolder &name);

public:
    static DT_ParmDefGrouped *create(int id, const UT_StringHolder &name);
    static const DT_ParmDefGrouped &get(const DT_ParmDef &def) 
		        { 
			  UT_ASSERT(def.type() == T_GROUPED); 
		          return static_cast<const DT_ParmDefGrouped &>(def); 
		        }

    DT_ParmDefGrouped	*addGroupedDef(const DT_ParmDefHandle &parm_def);

    const DT_ParmDefList &groupedDefs() const
			{ return myGroupedParmDefs; }

    virtual bool operator==(const DT_ParmDef &other) const;

private:
    DT_ParmDefList	 myGroupedParmDefs;
};


// ======================================================================
// Node definition

/// A unique node identifier.
typedef exint DT_NodeId;

/// A flat list of node identifiers.
typedef UT_Array<DT_NodeId> DT_NodeIdList;

/// An unordered set of unique node identifiers.
typedef UT_Set<DT_NodeId>  DT_NodeIdSet;

class DTUI_API DT_Node : public UT_NonCopyable
{
public:
    // Initialization.
    DT_Node(const DT_Node &parent, const char *name,
	    bool sort_children = true);
    ~DT_Node();

    // Used when initializing the UI element for this DT_Node. These values
    // should not be modified after creation because they are not kept in sync
    // with the UI.
    void			 setName(const char *name);
    const UT_StringHolder	&name() const
				 { return myName; }
    void		 	 setIcon(const char *icon);
    const UT_StringHolder	&icon() const
				 { return myIcon; }
    void			 setOpId(int op_id);
    int				 opId() const
				 { return myOpId; }
    void			 setSortChildren(bool sort_children);
    bool			 sortChildren() const
				 { return mySortChildren; }

    // Unique identifier for this node. Used for defining parent->child
    // relationships. Returns 0 if this is the root node.
    DT_NodeId			 id() const		{ return myId; }
    DT_NodeId			 parentId() const 	{ return myParentId; }

    bool 			 operator==(const DT_Node &other) const
				 { return myId == other.myId; }
    bool 			 operator!=(const DT_Node &other) const
				 { return myId != other.myId; }

    /// A value that can be used to denote an invalid node id.
    static DT_NodeId		invalidId() { return -1; }

private:
    // For creating a root node.
    friend class DT_Plugin;
    DT_Node();

    void			 init(DT_NodeId id, DT_NodeId parent_id,
        			      const char *name, bool sort_children);

    DT_NodeId			 myId;
    DT_NodeId			 myParentId;

    UT_StringHolder		 myName;
    UT_StringHolder		 myIcon;

    int				 myOpId;
    bool			 mySortChildren;
};

typedef UT_SharedPtr<const DT_Node> DT_NodeHandle;

typedef UT_Array<DT_NodeHandle> DT_NodeList;

// ======================================================================
// The interface on the plugin host that the plugin can use to communicate
// through.

class DTUI_API DT_Host
{
public:
    /// The list of node change types when calling DT_Host::nodesChanged.
    enum NodeChangeType
    {
	NCT_VALUE_CHANGE,	///< Use id of node(s) whose value changed
	NCT_NAME_CHANGE,	///< Use id of node(s) renamed
	NCT_SELECTION_CHANGE,	///< Use id of node(s) whose selection changed
	NCT_CHILD_ADDED,	///< Use parent id(s) of node(s) added
	NCT_CHILD_MOVED,	///< Use parent id(s) of node(s) moved
	NCT_REMOVED,		///< Use id of node(s) removed
	NCT_EXPANDED,		///< Use id of node(s) that should be expanded
	NCT_COLLAPSED,		///< Use id of node(s) that should be collapsed
    };

    DT_Host(DT_ViewportProvider *viewport_provider);
    virtual ~DT_Host();

    /// Sets a filter pattern on the tree, showing only the nodes in the tree
    /// that match using UT_String::multiMatch with the given pattern. The
    /// match is made using the full path to the tree node. If filter_cells is
    /// set to true, the filter is additionally applied to the individual cell
    /// values for that row, again using UT_String::multiMatch.
    virtual void setNodeFilter(const char *pattern, bool filter_cells) = 0;

    /// This method should be called to notify the display tree that a node, or 
    /// a set of nodes, has changed in some way.
    virtual void nodesChanged(NodeChangeType type,
                              const DT_NodeIdSet &node_ids) = 0;

    /// Some plugin options have changed and the UI needs to update.
    virtual void sharedOptionsChanged() = 0;

    /// Adds an interest on a node that's not a part of the display tree. This 
    /// is useful for when there's not a direct 1-to-1 mapping available 
    /// between tree nodes and OP nodes we're interested in.
    /// Returns \c true if successful.
    virtual bool addInterestOnNode(DT_NodeHandle node) = 0;

    /// Called by plugins to remove an interest from a node that's not a part
    /// of the display tree.
    /// Returns \c true if successful.
    virtual bool removeInterestOnNode(DT_NodeHandle node) = 0;

    /// Starts an in-place editing operation on a node.
    virtual void startRename(DT_NodeHandle node) = 0;

    /// The definition, order and/or count of the parameter definitions has
    /// changed. This call will cause the data tree to update the corresponding 
    /// display columns.
    virtual void parameterDefsChanged() = 0;

    /// Saves the node tree expansion state. This expansion state is
    /// automatically restored when the next update occurs.
    virtual void saveTreeExpansionState(bool match_nodes_by_name) = 0;

    /// Request running a delayed action. This causes the host to call
    /// DT_Plugin::runDelayedAction with the same parameters as given to this
    /// call, once the current operation is complete and control is given back
    /// to the data tree host. This can be used, for example, to perform scene  
    /// operations outside of the DT_Plugin::opEvent callback, when those 
    /// operations may cause events to be generated (e.g. changing a parameter  
    /// value as a reaction to an OP event).
    /// The action_id is any arbitrary integer, and the data is an opaque
    /// shared pointer whose interpretation is known only to the plugin.
    /// \c UT_SharedPtr<void> allows converting back to a known pointer value.
    /// E.g. if the value given is \c UT_SharedPtr<Foo> is given, then the 
    /// \c UT_SharedPtr<void> value given in DT_Plugin::runDelayedAction can be
    /// converted back into \c UT_SharedPtr<Foo>.
    virtual void addDelayedAction(int action_id, UT_SharedPtr<void> data) = 0;
    
    /// Report a message of the given severity. The message will be reported
    /// to Houdini's status line.
    virtual void addMessage(UT_ErrorSeverity sev, const char *fmt, ...) = 0;

    /// For binding the data tree to the viewport. Gets a selection from an
    /// available Scene Viewer pane. Can either take the current selection,
    /// or force a new selection. Can also choose to get back an object
    /// selection, component selection, or accept either.
    virtual bool getViewportSelection(
		    const DT_ViewportSelectionConfig &selection_config,
		    DT_ViewportSelection &selection);

    /// Interrupt any running viewport selection.
    virtual void interruptViewportSelection();

private:
    DT_ViewportProvider			*myViewportProvider;
    const DT_ViewportSelectionConfig	*myViewportSelectionConfig;
};

// ======================================================================
// Plugin definition

class DTUI_API DT_Plugin
{
public:
			 DT_Plugin(DT_Host &host);
    virtual 		~DT_Plugin();

    /// @{
    /// Methods provided by the plugin architecture.
    const DT_NodeHandle &rootNode() const { return myRootNode; }

    /// Returns the DT_DT_HostInterface object which can be used to
    /// communicate with the host to notify of dirty nodes, parameter changes,
    /// global option changes, etc.
    DT_Host &host() const { return myHost; }
    /// @}

    /// @{
    /// Methods to be implemented by the plugin.

    /// Returns the display name of the data tree plugin. This will be shown in
    /// the list of available plugins in the data tree pane.
    virtual const UT_StringHolder &name() const = 0;
    
    /// Returns the help ID to be used when the user clicks on the help button
    /// in the Data Tree pane. By default it just returns the base help for
    /// the Data Tree.
    virtual const UT_StringHolder &helpId() const = 0;
    
    /// @{
    /// Tree nodes

    enum RenameStyle {
	RS_INVALID,	// In-place name editing not allowed
	RS_ALPHANUM,	// Allow alphanumeric characters only
	RS_OPNAME,	// Allow only characters valid for an op name
	RS_ANY		// Allow any characters
    };

    /// Get the children of the given node.
    virtual bool 	 getChildren(const DT_Node &parent,
                 	             DT_NodeList &list) = 0;

    /// Returns true if the parent node potentially has children. False
    /// positives are much better than false negatives.
    virtual bool	 hasChildren(const DT_Node &parent)
			 { return true; }

    /// Get the info text to show in the MMB popup window.
    virtual bool	 getNodeInfoText(const DT_Node &node,
                	                 UT_WorkBuffer &info_text)
			 { return false; }

    /// The node delete operation is valid for this plugin.
    virtual bool	 canDeleteNode(const DT_Node &node)
			 { return false; }

    /// Called when a request for delete occurs.
    virtual bool	 deleteNodes(const DT_NodeList &node_list)
			 { return false; }

    /// Check if the node copy operation is valid for this plugin.
    virtual bool	 canCopyNodes(const DT_NodeList &list,
				 DT_NodeList *filtered_list)
			 { return false; }

    /// Called when a request to copy occurs.
    virtual bool	 copyNodes(const DT_NodeList &list)
			 { return false; }

    /// Check if the reviously copied nodes can be pasted into the given parent.
    virtual bool	 canPasteNodes(const DT_Node &parent_node)
			 { return false; }

    /// Called when a request to paste into a parent node occurs.
    virtual bool	 pasteNodes(const DT_Node &parent_node)
			 { return false; }

    /// The node rename operation is valid for this plugin.
    virtual RenameStyle	 getRenameStyle(const DT_Node &node)
			 { return RS_INVALID; }

    /// Called when a request for node rename occurs.
    virtual bool	 renameNode(const DT_Node &node, const char *new_name)
			 { return false; }

    /// Called when the right mouse-button (RMB) is clicked on a node. The 
    /// entire node selection is passed in, allowing to create a combined menu
    /// for all selected nodes. 
    /// When retrieving the top-level menu, \c sub_menu_token will be \c NULL.
    virtual bool	 getNodeMenu(const DT_NodeList &node_list,
                	             const char *sub_menu_token,
                	             DT_NodeMenuItemList &items)
			 { return false; }
    
    /// Perform the menu action specified by the user. The \c menu_token 
    /// argument will be the token of the menu item that the user selected.
    /// The node list will be the same as was passed into \c getNodeMenu.
    virtual void	 runMenuAction(const DT_NodeList &node_list,
                	               const char *menu_token)
			 { }
    /// @}

    /// @{
    /// Tree parameters

    enum ParmState
    {
	PS_INVALID,		///< No value can be retrieved.
	PS_VALID,		///< Value is valid
	PS_INDETERMINATE,	///< The value state is indeterminate
	PS_ANIMATED,		///< The value is animated and should have a
				///<  green background.
	PS_LOCKED		///< The value is locked and cannot be edited.
    };

    /// Returns the list of parameter definitions for the tree columns.
    virtual const DT_ParmDefList &getParameterDefs() = 0;

    /// For dealing with DT_ParmDefGrouped parameters, this method controls
    /// which grouped DT_ParmDef should be displayed for a particular DT_Node.
    virtual int		 getGroupedParameterId(const DT_Node &node, int parm_id)
			 { return parm_id; }

    /// Retrieve a value of a specific parameter for the given node. The
    /// parm_id is the value returned from DT_ParmDef::id(). The for_feel
    /// flag indicates if we are fetching this value to set up our UI_Feel
    /// when the mouse moves over this cell.
    ///
    /// Use the copyValue function for each function definition to retrieve
    /// a concrete values from the opaque DT_Value type.
    /// The caller expects DT_Value to be able to cast to specific types
    /// to retrieve the display value. Use the following table, based on the
    /// parm def type:
    ///
    /// A return value of \c PS_INDETERMINATE should only be returned if the
    /// parameter definition supports it, otherwise it is ignored.
    /// Return \c PS_INVALID if no value can be retrieved from the node.
    /// \note This function is never called for T_BUTTON types. Only
    /// setParameterValue.
    virtual ParmState	 getParameterValue(const DT_Node &node, int parm_id,
					   DT_Value &value, bool for_feel) = 0;
    
    /// Sets the value of a specific parameter for the given node. The parm_id
    /// is the value returned from DT_ParmDef::id()
    /// See the table for getValue for which C++ types should be expected for
    /// a given parameter definition type.
    virtual bool	 setParameterValue(const DT_Node &node, int parm_id,
					   const DT_Value &value) = 0;

    /// Sets the same value of a specific parameter on multiple nodes. This can
    /// be used to optimize costly operations, or support exclusivity. The first
    /// node in the list is the node for who the user set the value on.
    /// The parm_id is the value returned from DT_ParmDef::id()
    virtual bool	 setParameterValues(const DT_NodeList &node_list,
					    int parm_id,
					    const DT_Value &value);

    /// For DT_ParmDefMenu items that are defined as dynamic, this function
    /// can be used to return a list of menu items based on the specific node,
    /// or the current state of other parameters.
    virtual bool	 getParameterMenuItems(const DT_Node &node, 
					       int parm_id,
					       DT_MenuItemList &items) 
			 { return false; }
    /// @}

    /// @{
    /// Selection

    /// Called when a node queries for its selection state. If the selection
    /// state can be computed, the selection state is returned through the
    /// argument 'state' and a value of true returned. Return false if the
    /// selection can not be computed, or not supported. In that case, if
    /// DT_Node::opId returns a valid id, the op node is queried for its
    /// selection state.
    virtual bool	 getSelected(const DT_Node &node, bool &state)
			 { return false; }

    /// Called when a node is selected or de-selected in the UI. The requested
    /// selection state is given by the 'state' argument. If the selection is
    /// handled, a value of \c true should be returned. Otherwise, return a
    /// value of \c false. In that case, if DT_Node::opId returns a valid id,
    /// the op node is selected.
    virtual bool	 setSelected(const DT_Node &node, bool state)
			 { return false; }
    /// @}

    /// @{
    /// User-modifiable Plugin Options

    /// Returns the list of parameter definitions for the tree columns.
    virtual const DT_ParmDefList &getOptionDefs();

    /// Gets a value for a given option from the plugin.
    /// See getParameterValue
    /// \param parm_id The id of the parameter. Corresponds to the return
    ///                value of DT_ParmDef::id.
    /// \param value The value returned from the plugin.
    /// \return \c true if successful, \c false otherwise.
    virtual bool	 getOptionValue(int parm_id, DT_Value &value)
			 { return false; }

    /// Sets an option value for the plugin.
    virtual bool	 setOptionValue(int parm_id, const DT_Value &value)
			 { return false; }

    /// For DT_ParmDefMenu items that are defined as dynamic, this function
    /// returns the list of menu items based on the current state.
    virtual bool	 getOptionMenuItems(int parm_id,
                	                    DT_MenuItemList &items) 
			 { return false; }
    /// @}

    /// @{
    /// Events

    /// Called when an operator event has occurred. All nodes that have an
    /// interest in a given operator are in the array. See OP_EventType
    /// for explanation of what the \c data pointer contains.
    virtual void	 opEvent(const DT_NodeIdSet &node_ids,
                	         OP_EventType type, void *data)
			 { }


    /// Called by the host when it is time to run a delayed action that was
    /// added with DT_Host::addDelayedAction. Multiple delayed actions are
    /// coalesced into one call, hence the list of data.
    virtual void	 runDelayedAction(
			     int action_id,
                	     const UT_Array<UT_SharedPtr<void> > &data_list)
			 { }
    /// @}

    /// @{
    /// Drag and drop
    ///

    /// Drag Source
    virtual bool	 dragHasData(const DT_Node &node,
                	             const char *data_label)
			 { return false; }

    virtual void	*dragGetData(const DT_Node &node,
                	             const char *data_label)
			 { return NULL; }

    /// Drop Receiver
    virtual bool	 dropAccept(const DT_Node &node,
                	            DD_Source &drag_src,
                	            const char *data_label)
			 { return false; }

    virtual bool	 dropTest(const DT_Node &node,
                	          DD_Source &drag_src)
 			 { return false; }
    virtual void	 dropGetChoice(const DT_Node &node,
                	               DD_Source &drag_src,
                	               DD_ChoiceList &drop_choices)
			 { }

    /// When drag and drop is used to reorder child nodes, we get notified
    /// through this function.
    virtual void	 nodesReordered(const DT_Node &parent_node,
					const UT_IntArray &from_idx,
					const UT_IntArray &to_idx)
			 { }
    /// @}

    /// @{
    /// Persistence
    
    /// When the desktop is saved, this method is called to allow the plugin
    /// to save any data that would not be saved otherwise. 
    virtual bool	 writeDesktopData(std::ostream &os)
			 { return false; }
    
    virtual bool	 readDesktopData(std::istream &is)
			 { return false; }
    
    virtual bool	 writeSceneData(std::ostream &os)
			 { return writeDesktopData(os); }
    
    virtual bool	 readSceneData(std::istream &is)
			 { return readDesktopData(is); }
    /// @}

protected:
    /// Only subclasses can access a non-const version of our root DT_Node.
    DT_Node			&rootNodeRef();
    /// Under special circumstances, a subclass may want to create a new
    /// root-like DT_Node (using the default DT_Node constructor, which is
    /// private). This function makes that possible.
    static DT_NodeHandle	 createRootNode();

private:
    DT_NodeHandle		 myRootNode;
    DT_Host			&myHost;
};

typedef UT_SharedPtr<DT_Plugin> DT_PluginHandle;


/// A data tree plugin factory base class. Used to create Data Tree plugins on 
/// demand and to query names of plugins without creating them.
/// For writing a new Data Tree plugin, see \c ::newDataTreePluginFactory for
/// and example of how to sub-class this class to create a factory for your
/// plugin.
class DTUI_API DT_PluginFactory
{
public:
    virtual ~DT_PluginFactory() {}

    /// 
    virtual const UT_StringHolder &name() const = 0;

    /// Create a new instance of a data tree plugin from this factory.
    virtual DT_PluginHandle create(DT_Host &host) = 0;

protected:
    static bool registerFactory(DT_PluginFactory &factory);
};

extern "C" {
    /// All DSOs that provide a Data Tree plugin factory should implement this
    /// function. All it needs to do is to create the appropriate 
    /// DT_PluginFactory for the plugin(s) it implements.
    /// Example:
    /// \code
    /// class MyDataTreePlugin : public DT_Plugin { ... };
    ///    
    /// class MyDataTreePluginFactory : public DT_PluginFactory
    /// {
    /// public:
    ///   MyDataTreePluginFactory()
    ///   {
    ///     registerFactory(*this);
    ///   }
    ///	
    ///   virtual const char *name() const { return "My Data Tree Plugin"; }
    ///  
    ///   virtual DT_PluginHandle create(DT_Host &host)
    ///   { 
    ///     return DT_PluginHandle(new MyDataTreePlugin(host));
    ///   }
    /// };
    ///
    /// UT_UniquePtr<MyDataTreePluginFactory> theFactory;
    /// 
    /// SYS_VISIBILITY_EXPORT void newDataTreePluginFactory()
    /// {
    ///   theFactory.reset(new MyDataTreePluginFactory);
    /// }
    /// \endcode

    SYS_VISIBILITY_EXPORT void newDataTreePluginFactory();
};

#endif // __DT_Plugin__

