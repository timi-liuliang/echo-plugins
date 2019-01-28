/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: BM_PythonStateSupport.h
 *
 * COMMENTS:
 */

#ifndef BM_PythonStateSupport_H
#define BM_PythonStateSupport_H

#include "BM_API.h"
#include <PY/PY_AutoObject.h>
#include <PI/PI_OpHandleLink.h>
#include <PI/PI_PythonState.h>
#include <UI/UI_Object.h>
#include <UI/UI_Value.h>

class AP_Interface;
struct bmHandleParmCache;
struct bmHandleParmValues;
struct bmMenuHandler;
class BM_View;
class OP_Node;
class UI_Event;

// Viewer state handler for implementing viewer state hosts such as MSS_PythonState
class BM_API BM_PythonStateSupport : public UI_Object
{
public:
    using MenuHandlers	    = UT_StringMap</*handle*/UT_SharedPtr<bmMenuHandler>>;    
    using PyObjectArgs	    = UT_StringMap</*key*/ PY_PyObject*>;
    using HandleParmCache   = UT_StringMap</*handle*/ UT_SharedPtr<bmHandleParmCache>>;
    using HandleParmValues  = UT_StringMap</*handle*/ UT_SharedPtr<bmHandleParmValues>>;

    struct Result {
	Result() 
	    : myConsumed(true)
	    , myArgs(nullptr)
	    , myKwargs(nullptr)
	{
	}
	
	~Result()
	{
	    if (myArgs)
	    {
		PY_Py_DECREF(myArgs);
	    }

	    if (myKwargs)
	    {
		PY_Py_DECREF(myKwargs);
	    }
	}

	PY_PyObject*		myArgs;
	PY_PyObject*		myKwargs;
	PY_Result		myResult;
        PY_InterpreterAutoLock	myPylock;

	bool myConsumed;
    };

    // popupmenu builder
    struct BM_API MenuSupport
    {
	using Hotkeys = UT_Array<UT_StringRef>;

	MenuSupport();
	~MenuSupport();

	void init(BM_PythonStateSupport* state_handler);
	void clear();
	void create();

	const char* uiFilename() const;
	Hotkeys const& hotkeys() const;
	MenuHandlers& handlers();

	private:
	using SubMenuDefs = UT_Array<UT_StringRef>;

    	void buildMenu(
	    PI_PythonState::Info::Menu::Ptr const& menu, 
	    UT_StringRef & menu_content );

	BM_PythonStateSupport*	myStateHandler;
	SubMenuDefs		mySubMenus;
	MenuHandlers		myMenuHandlers;
	Hotkeys			myHotkeys;
	UT_StringHolder		myUIFilename;
    };

    // Keyworded argument builder
    struct BM_API Kwargs
    {
	struct Expr
	{
	    Expr() = default;
	    Expr(Expr const& other) = default;
	    ~Expr() = default;

	    Expr(UT_StringRef const& val) : myStr(val)
	    {	    
	    }

	    Expr(Expr && other) : myStr(std::move(other.myStr))
	    {
	    }

	    const char* c_str() const
	    {
		return myStr.c_str();
	    }

	    UT_StringHolder myStr;
	};

	Kwargs( UT_StringRef const & fmt = "{'args':{%s}}");
	~Kwargs() = default;

	Kwargs(Kwargs && other);
	Kwargs(Kwargs const& other) = default;

	void add(UT_StringRef const & key, Kwargs const & value);
	void add(UT_StringRef const & key, UT_StringRef const & value);
	void add(UT_StringRef const & key, OP_Node const * value);
	void add(UT_StringRef const & key, UI_Event const * value, BM_View const & viewer);
	void add(UT_StringRef const & key, Expr const & value);
	void add(UT_StringRef const & key, UT_Vector3 const & value);
	void add(UT_StringRef const & key, int value);
	void add(UT_StringRef const & key, float value);
	void addBool(UT_StringRef const & key, int value);
	// adds a hou.GeometrySelection entry. sinfo_address is a pointer to
	// a OP3D_ScriptSelectStateInfoPtr instance
	void addGeometrySelection(UT_StringRef const & key, int64 sinfo_address);

	// special support for menu handles mapped to ui symbols
	void add(MenuHandlers const & handles);

	operator UT_StringHolder () const
	{
	    return value();
	}

	UT_StringHolder value() const
	{
	    UT_StringHolder value;
	    value.sprintf( myOutputFormat.c_str(), myStr.c_str() );

	    return value;
	}

	private:
	UT_StringHolder myStr;
	UT_StringHolder myOutputFormat;
	size_t		myCount;

    };

    explicit 
    BM_PythonStateSupport(PI_PythonState::Info&	info, UI_Value& handleMenuItemValue);
    BM_PythonStateSupport() = default;
    ~BM_PythonStateSupport();

    PY_Result createInstance(BM_View const & viewer);

    bool isValid() const;

    void processEvent(
	PI_PythonState::Event	event, 
	Kwargs&			kwargs,
	Result&			result,
	PY_Result::Type		retType=PY_Result::NONE);

    void processEvent(
	PI_PythonState::Event	event, 
	Kwargs&			kwargs,
	PyObjectArgs&		py_args,
	Result&			result,
	PY_Result::Type		retType=PY_Result::NONE);

    bool hasEvent(PI_PythonState::Event event);

    PI_PythonState::Info const& templateInfo() const;

    // Menu support
    MenuSupport& menu();
    void handleMenuActionItemEvent(UI_Event *event);
    void handleMenuItem(UT_StringRef const & menuItemHandle);
    int handleKeyTypeEvent(UI_Event *event, PI_PythonState::Info const& info);
    AP_Interface* menuSymbolContainer();

    //bool triggerEventParmCallback(const char *event_name);

    // Handle binding support
    void addHandleLink(
	PI_OpHandleLink *pi, 
	PI_OpHandleLink::LateBinding* binding);
    void removeHandleLink(
	PI_OpHandleLink *pi, 
	PI_OpHandleLink::LateBinding* binding);

    bool toStateValues(PI_OpHandleLink *pi, OP_Node* node);
    bool toHandleValues(PI_OpHandleLink *pi, OP_Node* node);

    int getParmValue(
	PI_OpHandleLink const*  pi,
	int		        pindex, 
	fpreal &		val, 
	fpreal			d) const;
    int getParmValue(
	PI_OpHandleLink const*  pi,
	int		        pindex, 
	int &			val, 
	int			d) const;
    int getParmValue(
	PI_OpHandleLink const*	pi, 
	int			pindex, 
	UT_String &		val, 
	UT_String		d) const;

    int storeParmValue(
	PI_OpHandleLink const* pi, 
	int pindex, 
	fpreal value, 
	bool setkey);
    int storeParmValue(
	PI_OpHandleLink const* pi, 
	int pindex, 
	int value, 
	bool setkey);
    int storeParmValue(
	PI_OpHandleLink const* pi, 
	int pindex, 
	UT_StringRef const& value);

private:
    void		    addHandleParmCache(UT_StringRef const& key, bool cache_prev_values);
    void		    clearHandleParmCache(UT_StringRef const& hname);
    bmHandleParmCache&	    handleParmCache( UT_StringRef const& hname );
    bmHandleParmCache const& handleParmCache( UT_StringRef const& hname ) const;
    void		    clearHandleParmValues(UT_StringRef const& hname);
    char const*		    menuActionItemSymbolName() const;
    void		    processEvent_internal(
				PI_PythonState::Event	event, 	
				char const *		args_expr, 
				char const *		kwargs_expr,
				Result&			result,
				PY_Result::Type		retType);
    void		    processResult(Result const & result);

    UT_StringHolder	    myActiveViewer;
    UI_Value		    myMenuActionItemSymbol;
    UI_Value&		    myHandleMenuItemValue;
    MenuSupport		    myMenuSupport;
    PI_PythonState::Info*   myTemplateInfo;
    PY_AutoObject	    myState;
    HandleParmCache	    myHandleParmCache;
    HandleParmValues	    myHandleParmValues;
};

#endif // BM_PythonStateSupport_H
