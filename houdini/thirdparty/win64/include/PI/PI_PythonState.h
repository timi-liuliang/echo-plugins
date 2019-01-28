/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:
 *
 * COMMENTS:
 *
 *      Various enums and types, etc... related to Viewer states.
 */

#ifndef PI_PythonState_H
#define PI_PythonState_H

#include "PI_API.h"
#include "PI_ResourceTemplate.h"
#include <PY/PY_Callback.h>
#include <PY/PY_Python.h>
#include <OP/OP_OpTypeId.h>
#include <UT/UT_EnumMacros.h>
#include <UT/UT_WeakPtr.h>
#include <UT/UT_Set.h>
#include <UT/UT_StringMap.h>

#include <vector>

#define PI_TO_STRING(enum_module,enum_elem,ienum_elem,ienum_elem_name,enum_value) \
    case ienum_elem: return ienum_elem_name;

class OP3D_ScriptSelectStateInfoPtr;

namespace PI_PythonState
{
// state 'glue' interface 
const UT_StringHolder kStateIfaceMod	= "stateinterface";
const UT_StringHolder kStateCreateFunc	= "createViewerStateTemplate";
const UT_StringHolder kStateError	= "Viewer state error";
const UT_StringHolder kStateWarning	= "Viewer state warning";

// Callback event enums
enum class Event : short 
{
    Create
    ,Enter
    ,Exit
    ,Generate
    ,HandleToState
    ,Interrupt
    ,MenuAction
    ,Mouse
    ,MouseWheel
    ,Resume
    ,Selection
    ,StartSelection
    ,StopSelection
    ,StateToHandle
};

// Builtin menu item type enums
enum class MenuItemType : short 
{
    Action = 0,
    Invalid,
    Menu,
    RadioStrip,
    RadioStripItem,
    Separator,
    Toggle
};

// Registration status
enum class RegisterStatus : short 
{
    Invalid = 0,
    Registered,
    Waiting
};

struct CallbackInfo
{
    // Stores the callback binding info for all states
    using Binding = std::pair<UT_StringHolder /*func_name*/, Event>;
    using Bindings = UT_Array<Binding>;

    CallbackInfo() = default;
    ~CallbackInfo() = default;
    
    static Bindings const & bindings()
    {
	static Bindings theBindings = []()
	{
	    Bindings barray;
	    barray.append(Binding("onEnter",		Event::Enter));
	    barray.append(Binding("onExit",		Event::Exit));
	    barray.append(Binding("onGenerate",		Event::Generate));
	    barray.append(Binding("onHandleToState",	Event::HandleToState));
	    barray.append(Binding("onInterrupt",	Event::Interrupt));
	    barray.append(Binding("onMenuAction",	Event::MenuAction));
	    barray.append(Binding("onMouseEvent",	Event::Mouse));
	    barray.append(Binding("onMouseWheelEvent",	Event::MouseWheel));
	    barray.append(Binding("onResume",		Event::Resume));
	    barray.append(Binding("onSelection",	Event::Selection));
	    barray.append(Binding("onStartSelection",	Event::StartSelection));
	    barray.append(Binding("onStateToHandle",	Event::StateToHandle));
	    barray.append(Binding("onStopSelection",	Event::StopSelection));
	    return barray;
	}();
	return theBindings;
    }
};

// Holds python callbacks for custom ciewer state implementations
class CallbackHandler
{
    public:
    class Callback 
    {
	public:
	explicit Callback(PY_Callback *callback)
	    : myCallback(callback)
	{}

	Callback() = default;
	~Callback() = default;

	PY_Callback* operator->() const
	{
	    return myCallback.get();
	}

	private:
	UT_SharedPtr<PY_Callback> myCallback;
    };

    using Map = UT_Map<short, Callback>;

    CallbackHandler() = default;
    ~CallbackHandler() = default;

    bool has(Event event) const
    {
	return myEventCallbacks.count(static_cast<short>(event));
    }

    void add(Event event, void* callback)
    {
	myEventCallbacks[static_cast<short>(event)] = 
	    Callback(new PY_Callback(callback));
    }

    Callback* find(Event event)
    {
	auto it = myEventCallbacks.find(static_cast<short>(event));
	if (it != myEventCallbacks.end()) 
	{
	    return &it->second;
	}
	return nullptr;
    }

    void clear()
    {
	myEventCallbacks.clear();
    }

    private:
    Map myEventCallbacks;
};

// Info contains the information for registering a custom viewer state
struct Info 
{  
    using KeyModifiers = UT_Set<UT_StringHolder>;
    using ActiveViewers = UT_StringArray;

    struct Register
    {
	// registration type
	enum class Type : unsigned
	{	
	    Embedded,	    // registered from HOUDINI path
	    SelfInstalled,  // registered from HDA
	    Volatile	    // temporary registration
	};

	Register() 
	    : myOp_independent(0)
	    , myCategoryType(SOP_OPTYPE_ID)
	{
	    ;
	}

	Type regType() const
	{
	    if (myFolder_path.length() > 0)
	    {
		return Type::SelfInstalled;
	    }

	    if (UT_StringWrap(myType).matchFileExtension( ".pystate" ))
	    {
		return Type::Embedded;
	    }

	    return Type::Volatile;
	}

	const char* stateType() const
	{
	    return myType.c_str();
	}

	// registration info
	UT_StringHolder myFolder_path;
	UT_StringHolder myIcon;
	UT_StringHolder myLabel;
	UT_StringHolder myModule_name;
	UT_StringHolder myType;
	UT_StringHolder myCategory;
	UT_StringHolder myVersion;
	UT_StringHolder myVolatile_hotkey;
	int		myOp_independent;
	OP_OpTypeId	myCategoryType;
    };

    // Holds the handles used by a viewer state
    struct Handle 
    {
	using Map = UT_StringMap</*Handle name key*/ Handle>;
	using Binding = std::pair<std::string, std::string>;
	using StaticBindings = std::vector<Binding>;

	Handle() 
	    : myCachePrevValues(false)
	    , myLateBindingFlag(true)
	{
	}

	~Handle() = default;

	UT_StringHolder	myTypeName;
	UT_StringHolder	myName;
	UT_StringHolder	mySettings;
	StaticBindings	myBindings;
	bool		myCachePrevValues;
	bool		myLateBindingFlag;
    };
    
    // Holds the selectors used by a viewer state
    struct Selector
    {
	UT_SharedPtr<OP3D_ScriptSelectStateInfoPtr> myInfo;
	UT_StringHolder myHotkey;
	bool myAutoStart;

	Selector() : myAutoStart(true)
	{
	}

	bool valid() const
	{
	    return myInfo.get() != nullptr;
	}
    };

    // Holds the selectors used by a viewer state
    struct EntrySelector
    {
	using		Array = UT_Array<EntrySelector>;
	UT_StringHolder myName;
	UT_StringHolder myTypeName;
	UT_StringHolder myPrompt;
	UT_StringHolder myGroupParam;
	int             myInputIndex;
	bool            myInputRequired;
	UT_StringHolder myPrimMask;
	bool            myAllowDragging;
	UT_StringHolder myMenu;
	bool            mySelectAll;

	EntrySelector() 
	    : myGroupParam("group")
	    , myInputIndex(0)
	    , myInputRequired(true)
	    , myPrimMask("0xffffffff")
	    , myAllowDragging(false)
	    , myMenu("grouptype")
	    , mySelectAll(false)
	{
	}

	bool valid() const
	{
	    return !myName.isEmpty() && !myTypeName.isEmpty() && !myPrompt.isEmpty();
	}
    };

    // Popupmenu structure
    struct MenuItem : public std::enable_shared_from_this<MenuItem>
    {
	using Ptr = UT_SharedPtr<MenuItem>;
	using Weak = UT_WeakPtr<MenuItem>;

	MenuItem() = default;
	virtual ~MenuItem() = default;

	virtual UT_StringHolder label() const 
	{
	    return myLabel;
	}

	virtual UT_StringHolder handle() const
	{
	    return myHandle;
	}

	virtual UT_StringHolder hotkey() const 
	{
	    return myHotkey;
	}

	virtual MenuItemType type() const
	{
	    return MenuItemType::Invalid;
	}

	virtual bool canHaveSymbol() const
	{
	    return type() != MenuItemType::Invalid && !myHandle.isEmpty();
	}

	operator bool () const
	{
	    return type() != MenuItemType::Invalid;
	}

	UT_StringHolder myLabel;
	UT_StringHolder myHandle;
	UT_StringHolder myHotkey;

    };

    struct MenuItemContainer : public MenuItem
    {
	using MenuItems = UT_Array<MenuItem::Ptr>;
	using Ptr = UT_SharedPtr<MenuItemContainer>;
	using Weak = UT_WeakPtr<MenuItemContainer>;

	MenuItemContainer() = default;
	~MenuItemContainer() override = default;

	template <typename T>
	typename T::Ptr addItem()
	{
	    typename T::Ptr p = UTmakeShared<T>();
	    myItems.append(p);
	    return p;
	}

	template <typename T>
	void addItem( typename T::Ptr const & item)
	{
	    myItems.append(item);
	}

	// depth-first search 
	template< typename FUNC >
	MenuItem::Ptr findItem(FUNC const & in_test)
	{
	    for (auto && item : myItems)
	    {	
		if (in_test(item))
		{
		    return item;
		}

		auto cont = std::dynamic_pointer_cast<MenuItemContainer>(item);
		if (cont)
		{
		    auto p = cont->findItem(in_test);
		    if (p)
		    {
			return p;
		    }
		}
	    }
	    return nullptr;
	}

	template< typename FUNC >
	MenuItem::Ptr const findItem( FUNC const & in_test ) const
	{
	    return const_cast<MenuItemContainer*>(this)->findItem(in_test);
	}

	MenuItems myItems;
    };

    using MenuItemContainers = UT_StringMap<MenuItem::Ptr>;

    struct Menu : public MenuItemContainer
    {
	using Ptr = UT_SharedPtr<Menu>;
	using Weak = UT_WeakPtr<Menu>;
	using HotkeyCache = UT_StringMap<MenuItem::Weak>;
	using HandleCache = UT_StringMap<MenuItem::Weak>;	

	struct Separator : public MenuItem
	{	
	    using Ptr = UT_SharedPtr<Separator>;
	    using Weak = UT_WeakPtr<Separator>;

	    Separator() = default;
	    ~Separator() override = default;

	    MenuItemType type() const override
	    {
		return MenuItemType::Separator;
	    }
	};

	struct ActionItem : public MenuItem
	{
	    using Ptr = UT_SharedPtr<ActionItem>;
	    using Weak = UT_WeakPtr<ActionItem>;

	    ActionItem() = default;
	    ~ActionItem() override = default;

	    MenuItemType type() const override
	    {
		return MenuItemType::Action;
	    }

	    bool canHaveSymbol() const override
	    {
		return false;
	    }
	};

	struct ToggleItem : public MenuItem
	{
	    using Ptr = UT_SharedPtr<ToggleItem>;
	    using Weak = UT_WeakPtr<ToggleItem>;

	    ToggleItem() : myDefaultValue(false)
	    {
	    }
	    ~ToggleItem() override = default;

	    MenuItemType type() const override
	    {
		return MenuItemType::Toggle;
	    }

	    bool myDefaultValue;
	};

	struct RadioStrip : public MenuItemContainer
	{
	    using Ptr = UT_SharedPtr<RadioStrip>;
	    using Weak = UT_WeakPtr<RadioStrip>;

	    RadioStrip() 
	    {
	    }
	    ~RadioStrip() override = default;

	    MenuItemType type() const override
	    {
		return MenuItemType::RadioStrip;
	    }

	    // the radio item handle value
	    UT_StringHolder myDefaultValue;
	};

	struct RadioStripItem : public MenuItem
	{
	    using Ptr = UT_SharedPtr<RadioStripItem>;
	    using Weak = UT_WeakPtr<RadioStripItem>;

	    RadioStripItem() : myPosition(0)
	    {}

	    ~RadioStripItem() override = default;

	    MenuItemType type() const override
	    {
		return MenuItemType::RadioStripItem;
	    }

	    bool canHaveSymbol() const override
	    {
		return false;
	    }

	    RadioStrip::Ptr parent() const
	    {
		return myParent.lock();
	    }

	    unsigned myPosition;
	    RadioStrip::Weak myParent;
	};

	// adds a new menu item to a given container
	template< typename T >
	typename T::Ptr addItem()
	{
	    auto cont = findContainer(UT_StringRef()); 
	    if (cont)
	    {
		return cont->addItem<T>();
	    }
	    static typename T::Ptr dummy;
	    return dummy;
	}

	template< typename T >
	typename T::Ptr addItem(UT_StringRef const& handle)
	{
	    auto cont = findContainer(handle); 
	    if (cont)
	    {
		return cont->addItem<T>();
	    }
	    static typename T::Ptr dummy;
	    return dummy;
	}

	template< typename T >
	void addItem( typename T::Ptr const& item )
	{
	    auto cont = std::dynamic_pointer_cast<MenuItemContainer>(
		this->shared_from_this());
	    cont->addItem<T>(item);
	}

	// find existing menu item, cast to requested type and returns a UT_SharedPTr
	template< typename T, typename FUNC >
	typename T::Ptr queryItem(FUNC const& test)
	{
	    auto item = findItem(test);
	    if (item)
	    {
	    	return std::dynamic_pointer_cast<T>(item);
	    }
	    static typename T::Ptr dummy;
	    return dummy;
	}

	template <typename T>
	typename T::Ptr itemByHotkey(UT_StringRef const & hotkey)
	{
	    if (myHotkeyCache.contains(hotkey))
	    {
		return std::dynamic_pointer_cast<T>(myHotkeyCache[hotkey].lock());
	    }

	    auto menu_item = queryItem<MenuItem>(
		[&](MenuItem::Ptr const & item) {return hotkey == (*item).myHotkey;});
	    if (menu_item)
	    {
		myHotkeyCache[hotkey] = menu_item;
	    }
	    return std::dynamic_pointer_cast<T>(myHotkeyCache[hotkey].lock());
	}

	template <typename T>
	typename T::Ptr itemByHandle(UT_StringRef const & handle)
	{
	    if (myHandleCache.contains(handle))
	    {
		return std::dynamic_pointer_cast<T>(myHandleCache[handle].lock());
	    }

	    auto menu_item = queryItem<MenuItem>(
		[&](MenuItem::Ptr const & item) {return handle == (*item).myHandle;});
	    if (menu_item)
	    {
		myHandleCache[handle] = menu_item;
	    }
	    return std::dynamic_pointer_cast<T>(myHandleCache[handle].lock());
	}

        MenuItemType type() const override
	{
	    return MenuItemType::Menu;
	}

	bool canHaveSymbol() const override
	{
	    return false;
	}

	private:
	MenuItemContainer::Ptr findContainer(UT_StringRef const& handle)
	{
	    if (handle.isEmpty())
	    {
		// return top menu container
		return std::dynamic_pointer_cast<MenuItemContainer>(
		    this->shared_from_this());
	    }

	    return queryItem<MenuItemContainer>(
		[&](MenuItem::Ptr const & item) {return handle == (*item).myHandle;});
	}

	HotkeyCache myHotkeyCache;
	HandleCache myHandleCache;
    };

    // Info impl
    Info() 
    {
	myMenu = UTmakeShared<Menu>();
    }

    void addModifierKey(UT_StringRef const& modifierKey)
    {
	myModifierKeys.insert(modifierKey);
    }    

    PY_Result bindStateFactory(void* callback)
    {
	if (!myCallbacks.has(PI_PythonState::Event::Create)) {
	    // install the creation callback
	    myCallbacks.add(Event::Create, callback);

	    // add the rest of the supported callbacks
	    auto* pymodule = PI_PythonState::kStateIfaceMod.c_str();
	    for ( auto && b : CallbackInfo::bindings())
	    {
		PY_Result result = PYbindFunctionFromModule(pymodule, b.first.c_str());
		if (result.myResultType != PY_Result::PY_OBJECT) 
		{
		    myCallbacks.clear();
		    return result;
		}
		myCallbacks.add(b.second, result.myOpaquePyObject);
	    }
	}
	return PY_Result();	    
    }
    
    bool valid() const
    {
	// return true if PI_PythonState::Info is a valid object
	// note: the create callback is mandatory
	return myCallbacks.has(PI_PythonState::Event::Create);
    }

    void bindMenu( Menu::Ptr const & menu )
    {
	myMenu = menu;
    }

    Register		    myReg;
    CallbackHandler	    myCallbacks;
    Handle::Map		    myHandles;
    Menu::Ptr		    myMenu; 
    EntrySelector::Array    myEntrySelectors;
    Selector		    mySelector;
    KeyModifiers	    myModifierKeys;

    // Store the active viewer names in which state instances are running
    // Note: this runtime info is transient and only available while a python 
    // state is active.
    ActiveViewers	    myActiveViewers;
};

// Viewer state template
class Template : public PI_StateTemplate
{
    public:
    Template(
	Info const&	info,
	void*		constructor,
	PRM_Template*	templates,
	PI_ViewerType   viewer_type,
	unsigned	network_types
    ) : PI_StateTemplate(
	    info.myReg.myType.c_str()
	    ,info.myReg.myLabel.c_str()
	    ,info.myReg.myIcon.c_str()
	    ,constructor
	    ,templates
	    ,viewer_type
	    ,network_types
	    ,info.myReg.myOp_independent
	    ,info.myReg.myVolatile_hotkey.c_str())
	    ,myInfo(info)
	    ,myRefCount(0)
    {
	;
    }

    ~Template()
    {
    }

    CallbackHandler& callbacks()
    {
	return myInfo.myCallbacks;
    }

    PI_PythonState::Info& info()
    {
	return myInfo;
    }

    PI_PythonState::Info const& info() const
    {
	return myInfo;
    }

    void addRefCount()
    {
	myRefCount++;
    }

    size_t releaseRefCount()
    {
	if (myRefCount)
	{
	    --myRefCount;
	}
	return myRefCount;
    }

    size_t useCount() const
    {
	return myRefCount;
    }

    void setInfo( PI_PythonState::Info const& info )
    {
	myInfo = info;
    }

private:
    // Used by a Python state host class (e.g. MSS_PythonState)
    PI_PythonState::Info myInfo;

    // Maintain the ref count of type templates. This ref counting is used for 
    // managing the life cycle of PI_PythonState::Template ojbects stored by 
    // PI_ResourceManager.
    size_t myRefCount;
};

// This interface is implemented by Python State hosts (e.g. MSS_PythonState) and 
// gives access to the underlying state object for various operations.
// 
struct HostAccessor
{
    HostAccessor() = default;
    virtual ~HostAccessor() = default;

    // Link an operator with the state instance
    virtual void bindHost( OP_Operator* op ) = 0;    
};

}

#endif // PI_PythonState_H
