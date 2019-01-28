/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UI_HotkeyHelper.h (UI Library, C++)
 *
 * COMMENTS:	Helper class for hotkey handling
 */

#ifndef __UI_HOTKEYHELPER_H__
#define __UI_HOTKEYHELPER_H__

#include "UI_API.h"
#include <UT/UT_PtrProxy.h>
#include <UT/UT_StringMap.h>
#include "UI_Object.h"

class UI_Event;

/// Hotkey handler method. Returns true if successfully consumed.
typedef bool (UI_Object::*UI_HotkeyMethod)(UI_Event *);


/// The UI_HotkeyHelper class allows one to maintain a table of hotkeys and
/// their associated handler functions (of type UI_HotkeyMethod) so that we
/// can easily perform functions like dispatching hotkey handlers as well as
/// testing to see if a hotkey is going to be handled without invoking the
/// handler.
class UI_API UI_HotkeyHelper
{
public:
    struct Entry
    {
	const char *	mySymbol;
	UI_HotkeyMethod	myMethod;
	bool		myEchoFlag;
    };

    /// Default constructor
	    UI_HotkeyHelper(UI_Object *client);
    /// Construct using an array of hotkey entries terminated by NULL mySymbol.
	    UI_HotkeyHelper(UI_Object *client, const Entry *hotkey_entries);
    /// Destructor
    virtual ~UI_HotkeyHelper();
    
    /// Adds an array of hotkey entries terminated by NULL mySymbol.
    void    addHotkeys(const Entry *hotkey_entries);
    /// Add a single hotkey entry
    void    addHotkey(const Entry &entry);
    /// Remove a hotkey entry
    void    removeHotkey(const char *symbol);

    /// Invoke the handler for the given UI hotkey event.
    /// Returns true if consumed, false otherwise.
    bool    processHotkey(UI_Event *event);

    /// Will we handle the given UI hotkey symbol?
    bool    willProcessHotkey(UI_Event *event);

private: // methods

    UI_HotkeyHelper(const UI_HotkeyHelper &copy);		// not yet done
    UI_HotkeyHelper & operator=(const UI_HotkeyHelper &copy);	// not yet done

    struct TableEntry
    {
	TableEntry(UI_HotkeyMethod method, bool echo)
	    : myMethod(method), myEchoFlag(echo) {}

	UI_HotkeyMethod		 myMethod;
	bool			 myEchoFlag;
    };

private: // data

    UT_RefProxy<UI_Object>  myClient;
    UT_StringMap<TableEntry> myTable;

};

#endif // __UI_HOTKEYHELPER_H__
