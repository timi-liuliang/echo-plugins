/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_hotKeys_h__
#define __HOM_hotKeys_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"
#include "HOM_EnumModules.h"
#include "HOM_Errors.h"
#include <vector>
#include <map>

SWIGOUT(%rename(hotkeys) HOM_hotkeys;)
class HOM_API HOM_hotkeys
{
public:
    virtual ~HOM_hotkeys()
    {}

    virtual std::string __repr__() = 0;

    // Hotkey information
    virtual std::vector<std::string>
				 assignments(
	const char *hotkey_symbol) = 0;
    virtual std::string		 hotkeyDescription(
	const char *hotkey_symbol) = 0;
    virtual std::string		 hotkeyLabel(
	const char *hotkey_symbol) = 0;
    virtual bool		 isKeyMatch(const char *key,
					    const char *hotkey_symbol) = 0;
    virtual std::vector<std::string>
				 findConflicts(const char *hotkey_symbol,
					       const char *key) = 0;

    virtual int			 changeIndex() = 0;


    // Functions for manager UI
    virtual std::vector< std::map<std::string, std::string> >
				 commandsInContext(const char * context) = 0;

    virtual std::vector< std::map<std::string, std::string> >
				 contextsInContext(const char *context) = 0;


    // Modify hotkeys and key assignments
    virtual bool		 addCommand(
	const char *hotkey_symbol,
	const char *label,
	const char *description,
	const std::vector<std::string> &assignments = std::vector<std::string>()) = 0;

    virtual bool		 addContext(const char *hotkey_symbol,
					    const char *label,
					    const char *description) = 0;

    virtual void		 removeHotkeySymbol(
	const char* hotkey_symbol) = 0;

    virtual std::string		 hotkeySymbol(
	const char *english_context,
	const char *english_command = nullptr) = 0;
    virtual bool		 clearAssignments(
	const char *hotkey_symbol) = 0;
    virtual bool		 addAssignment(const char *hotkey_symbol,
					       const char *key) = 0;
    virtual bool		 removeAssignment(const char *hotkey_symbol,
						  const char *key) = 0;

    virtual std::map<std::string, std::vector<std::string> >
				 _getHotkeysStatus(
	const char *hotkey_symbol,
	int modifier_mask = 0) = 0;

    virtual std::vector<int>	 availableKeycodes(
	const char *hotkey_symbol,
	int modifier_mask = 0) = 0;


    // Key utilities
    virtual std::string		 keycodeToString(int keycode,
						 short modifiers = 0) = 0;
    virtual int			 stringToKeycode(const char *key,
						 short modifiers = 0) = 0;


    // backup/restore/save
    virtual void		 _createBackupTables() = 0;
    virtual void		 _restoreBackupTables() = 0;
    virtual void		 revertToDefaults(
	const char *hotkey_symbol,
	bool one_level_only) = 0;
    virtual bool		 saveOverrides() = 0;


    // Keymaps
    virtual bool		 saveAsKeymap(
	const char *name,
	const char *path = nullptr) = 0;
    virtual bool		 loadKeymap(
	const char *name,
	const char *path = nullptr) = 0;
    virtual bool		 importKeymap(const char *name,
					      const char *path) = 0;
    virtual std::vector<std::pair<std::string, std::string> >
				 keymaps() = 0;
    virtual std::string		 currentKeymap() = 0;
};

#endif
