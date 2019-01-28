/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	AP_Interface.h ( SI Library, C++)
 *
 * COMMENTS:
 */

#ifndef __AP_Interface__
#define __AP_Interface__

#include "SI_API.h"
#include <stdio.h>
#include <UI/UI_Object.h>
#include <UT/UT_Signal.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_WorkBuffer.h>

class AP_Interface;
class UI_Value;
class UI_Manager;
template<typename T> class UT_SymbolMap;
class UI_Feel;
class UI_Window;
template <typename, typename> class UT_Pair;
typedef UT_SymbolMap<UI_Value *> UI_NamedValueMap;
typedef UT_SymbolMap<UI_Object *> UI_NamedObjectMap;

/// Base class for interfaces.
/// This class' main responsibility is to maintain hash tables of named
/// UI_Feel "object symbol" and UI_Value "value symbol" items. 
class SI_API AP_Interface : public UI_Object {
public:
    AP_Interface();

    /// Construct with given name and predefined value names with corresponding
    /// interests. The actual values are not created until wireInteface() is
    /// called. Typically, initApplication() is first called to create the
    /// the values and then wireInterface() is called afterwards to attach the
    /// methods to the corresponding values.
    AP_Interface(const char *myname,
		 const char *const *names,
		 UI_EventMethod const *methods);

    virtual ~AP_Interface();

    /// Return the name passed in the constructor
    const UT_String	&getName() const { return myName; }

    /// Attach values names to the methods passed in the constructor.
    /// The actual UI_Value's are typically created by calling
    /// initApplication() first. If the values do not exist yet, then they are
    /// automatically created into uims (if not nullptr), else they will
    /// created in this object.
    void	  	 wireInterface(UI_Manager *uims);

    // Removes interests from all the value names passed in the constructor.
    void		 unwireInterface(UI_Manager *uims);

    /// Turns a .ui file into UI gadgets. If quiet is true, no errors or
    /// warnings are printed. If defines is specified, this allows you to
    /// pass CPP style symbols to the parser, in the form "SYMBOL=VALUE".
    /// =VALUE may be omitted.
    bool		 readUIFile(const char *ui_filename,
       			            bool quiet = false);
    
    /// Create new preference file given its base name using standard rules
    /// (eg. in $HOME/houdiniX.Y). This may return nullptr if a .nosave file is
    /// found.
    static FILE		*createPreferenceFile(const char *filename);

    /// Read the given preference file given its base name using standard
    /// rules. The values will be initialized according to the parsed file into
    /// this object.
    bool		 readPreferenceFile(const char *pref_filename,
        		                    bool quiet = false);
    
    /// Set the named value symbol to the corresponding pointer. If warn is
    /// true, then a warning will be issued if the name already exists as an
    /// object symbol, or if the value symbol name already previously existed.
    void		 setValueSymbol(const char *symbol, UI_Value *value,
				bool warn = true);

    /// Find the UI_Value for the given named value symbol
    UI_Value		*findValueSymbol(const char *symbol) const;

    /// Get the UI_Value for the given named value symbol, creating it if it
    /// doesn't exist yet. If 'create' is false, then getValueSymbol() is the
    /// same as findValueSymbol().
    UI_Value		*getValueSymbol(const char *symbol, int create = 1);

    /// Returns all the Symbol/UI_Value's pairs from the ValueSymbol table
    /// return array will contain nullptr values if its symbol doesn't exist.
    int			 getValueSymbols(UT_StringArray &symbols,
       			                 UT_Array<UI_Value *> &values) const;

    /// Set the named object symbol to the corresponding pointer. If warn is
    /// true, then a warning will be issued if the name already exists as an
    /// value symbol, or if the object symbol name already previously existed.
    void	   	 setObjectSymbol(const char *symbol, UI_Object  *who,
					 int warn = 1);

    /// Find the UI_Object for the given object symbol name. Returns nullptr if
    /// it doesn't exist.
    UI_Object	   	*getObjectSymbol(const char *symbol) const;

    /// Find the UI_Feel for the given object symbol name. Returns nullptr if
    /// it doesn't exist. This is equivalent to findObject<UI_Feel*>(symbol).
    UI_Feel		*getFeelSymbol(const char *symbol) const;

    /// Find the UI_Feel for the given value pointer. Returns nullptr if
    /// it doesn't exist.
    UI_Feel		*findValueFeel( UI_Value * value) const;

    /// Delete the entry for the given object symbol name
    void		 removeObjectSymbol(const char *symbol);
    
    /// Find the pointer for the given object symbol name, dynamically casted
    /// to the given template type.
    template<typename T>
    T *findObject(const char *name) const
    {
	return dynamic_cast<T *>(getObjectSymbol(name));
    }

    /// Find the pointer for the given value symbol name, dynamically casted
    /// to the given template type.
    template<typename T>
    T *findValue(const char *name) const
    {
	return dynamic_cast<T *>(findValueSymbol(name));
    }

    /// Initialize this object. This method is intended to be overriden by
    /// subclasses since the base class implementation is empty. The subclass
    /// typically calls readUIFile() here to create the interface which
    /// populates the object and value symbols.
    virtual void initApplication(UI_Manager *uims,
				 int argc,
				 const char **argv);

    /// Resets this object by calling unwireInterface() and then destroying all
    /// objects and values.
	    void resetApplication();
    
    virtual void handleEvent(UI_Event *event);

    // These are methods which are called only if the APP is "the"
    //	registered main application in the program.  By default, they
    //	do nothing.
    virtual int		 saveAppData(const char *filename); // ret 1 on success
    virtual int		 loadAppState();	// Allows app to load context
    virtual int		 saveAppState();	// Allows app to save context
    virtual void	 saveOnCoreDump();	// To do stuff when...yikes!
    virtual bool	 getCoreDumpFileName(UT_WorkBuffer &name);

    // Utility methods for managing the common ui.pref file
    // Load the UI pref file; should be used by Main.C only.
    bool		 loadUIInitPrefs();
    // Save the current UI state to the pref file. The layout level is 
    // the desired level to be saved.
    void		 saveUIInitPrefs(const char * layout_level, int playbarui);
    // Restore UI prefs from file. Similar to load but does not explicitly
    // set the layout level, which can only be done by Main.C; so this method
    // is used everywhere else to load the prefs from file.
    bool		 restoreUIInitPrefs();

    /// Menu items and their mapping for file chooser style preference.
    static const UT_Pair<const char *, int>	theFileChooserStyleMap[];

    // Helper methods to convert enums to labels, and vice versa.
    static int		 mapStrToInt( const char * label, 
				      const UT_Pair<const char *, int> * map );
    static const char *	 mapIntToStr( int index, 
				      const UT_Pair<const char *, int> * map );

    // If a function key is pressed and a script is attached to it,
    // this will return the script.
    //
    bool		 getFunctionKeyScript(UI_Event *event,
					     UT_String &script);

    // These static methods will get/set the exit code that the UI application
    // will exit with.  The default exit code is 0.  Note that these methods
    // are implemented in AP_Main.C.
    static void		 setAppExitCode(int exit_code);
    static int		 getAppExitCode();

    /// Returns @c true if the application is exiting.
    static bool		 isAppExiting();

    // Returns the desktop specified by the -desktop argument.
    // Returns nullptr if no such argument was specified.
    static const char	*getStartupDesktop();
    
protected:
    //
    //  Set the application to run in the fore or background.  This MUST
    //	be called from a static constructor (i.e. before main is run).
    //
    static void		 setAppForeground();

    static void		  setTheMainApplication(AP_Interface *app);
    static AP_Interface	 *getMainApplication() { return theMainApplication; }

    UT_String			 myName;
    const char * const		*myValueNames;
    UI_EventMethod const	*myMethods;

    UI_NamedValueMap	*getValueTable (void) const { return myValueTable;  }
    UI_NamedObjectMap	*getObjectTable(void) const { return myObjectTable; }

    // Updates the given UI_Window object using the -geometry or pref file
    static void		 loadWindowGeometry(
			    UI_Window *window,
			    bool default_maximized,
			    bool use_pref);

    // Returns true if the -geometry arg was parsed out and false otherwise.
    // NOTE: The values are returned in pixels!
    static bool		 getWindowGeometry(int &width, int &height, 
					   int &left, int &bottom,
					   bool &maximized,
					   bool use_pref);
    static bool          getSaveWindowGeoPref();
    static void		 saveWindowGeoPref();
    static bool		 loadWindowGeoPref(
			    int &w, int &h, int &left, int &bottom,
			    bool &maximized);

    virtual void	 getSignalList(int *signals, int max_signals);

private:
    class si_CrashHandler;
    friend class	si_CrashHandler;

    void		  installSignalHandlers();
    void		  assignUIInitPrefs();

    static const char	 *mainApplicationName();

    static void		  createCrashLog(UTsignalHandlerArg sig_arg, 
    				const char *appname, const char *filename);
    static void		  terminateSignalHandler(UTsignalHandlerArg sig_arg);
    static bool		  coredumpHandler(UTsignalHandlerArg sig_arg);
    static void		  coreDumpChaser(UTsignalHandlerArg sig_arg);
    static void		  powerFailHandlerSignalHandler(UTsignalHandlerArg);
    static void		  powerFailHandler(UT_String &saved_file_name);
    static void		  terminationWarning(uint nseconds);
    static void		  betaExpiryWarning(const char* date_info);

    static AP_Interface	 *theMainApplication;

private:
    UI_NamedValueMap	 *myValueTable;
    UI_NamedObjectMap	 *myObjectTable;
};

extern void	APregister  (AP_Interface *app);
extern void	APderegister(AP_Interface *app);

// These template functions allow you to get & cast to the appropriate
// object/value type in one step, ie:
//
// UI_Button   *button = UIgetObject<UI_Button>(this, "button.gad");
// UI_StringList *menu = UIgetValue<UI_StringList>(this, "rmb.menu");

template<class ObjectClass> inline
ObjectClass *SIgetObject(const AP_Interface *app, const char *name)
{
    return dynamic_cast<ObjectClass *>(app->getObjectSymbol(name));
}
							  
template<class ValueClass> inline
ValueClass *SIgetValue(const AP_Interface *app, const char *name)
{
    return dynamic_cast<ValueClass *>(app->findValueSymbol(name));
}
							  
#endif
