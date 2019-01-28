/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Command library (C++)
 *
 * COMMENTS:	Handle command parsing for expansion.
 *	The command manager class also contains the interface to the
 *	long operation interrupt mechanism.
 *
 */

#ifndef __CMD_Manager_H__
#define __CMD_Manager_H__

#include "CMD_API.h"
#include "CMD_Command.h"
#include "CMD_Source.h"
#include "CMD_Variable.h"

#include <CH/CH_Manager.h>
#include <FS/FS_ServerSocketListener.h>

#include <UT/UT_Assert.h>
#include <UT/UT_Spawn.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_TaskLock.h>
#include <UT/UT_ValArray.h>

#include <iosfwd>


class UT_WorkArgs;
class CH_AutoEventHandler;
class CMD_Args;
class CMD_Loop;
class CMD_AliasTable;
class CMD_Manager;
class CMD_Command;
class CMD_VariableChangeNotifier;
class cmd_FalseCallback;

extern "C" {
   SYS_VISIBILITY_EXPORT extern void CMDextendLibrary(CMD_Manager *theManager);
}

typedef void (*CMD_PostCmdCallback)(void *userdata);
typedef void (*CMD_CommandEchoCallback)(const char *echo_line,void *userdata);

typedef UT_ValArray<CMD_Command *> CMD_CommandList;
typedef UT_Array<CMD_Source *>	CMD_SourceStack;
typedef UT_Array<char *>	CMD_PromptStack;

#define	CMD_STATUS_VARIABLE "status"

/// This class to keeps track of the hscript context, installed commands,
/// variables, aliases, command buffer, and the prompt.
class CMD_API CMD_Manager : public CH_Manager {
public:
    CMD_Manager(const char *appname);
    virtual ~CMD_Manager();

    /// @private
    /// Called just before the CMD_Manger will be deleted
    virtual void	aboutToDelete() override final;

    virtual void	 setContext() override;
    // this command won't assert
    static bool		 getContextExists() { return theManager != NULL; }

    /// retrieves the global CMD_Manager
    static CMD_Manager	*getContext()
			 {
			    UT_ASSERT( theManager );
			    return theManager;
			 }

    /// Send input to the command manager.  If the command is complete,
    /// the command will be executed, otherwise there will be input pending
    /// with the remainder of the input sitting in a buffer until there's
    /// a full command.
    void		 sendInput(const char *input, bool create_block = true);

    /// This version splits the input into lines and sends them to the single
    /// line sendInput. We need a non-const char * so we can avoid copying the
    /// data. The components of the string are modified, and then restored, so
    /// you don't actually have to worry about the string being modified.
    void		 sendMultiLineInput(const char *input,
					    bool create_block = true);

    /// Force execution of a command
    void		 execute(const char *cmd, bool add_to_history = false,
				 std::ostream *out=0, std::ostream *err=0,
				 bool do_alias_expansion = true,
				 bool create_block = true);

    /// Non-threadsafe versions of sendInput()/sendMultiLineInput()/execute().
    /// These methods cannot be called on the same CMD_Manager object from
    /// multiple threads.
    /// @{
    void		 sendInputNoLock(
				const char *input,
				bool create_block = true);
    void		 sendMultiLineInputNoLock(
				const char *input,
				bool create_block = true);
    void		 executeNoLock(
				const char *cmd,
				bool add_to_history = false,
				std::ostream *out = 0,
				std::ostream *err = 0,
				bool do_alias_expansion = true,
				bool create_block = true);
    /// @}

    /// Try to complete the given command prefix to the longest common string.
    /// If an exact match is found, append a " ".
    void		 completeCommand(const char *prefix, 
					 UT_String &command) const;

    /// Return a list of all commands that start with the given prefix.
    void		 matchCommands(const char *prefix,
				       UT_StringArray &matches) const;

    /// Install a command into the command list.
    void		 installCommand(const char *name, const char *options,
				CMD_Callback cb, bool is_safe = true);

    /// Uninstall a command from the command list - this replaces it with the
    /// previous version of the command.  The method returns @c false if there
    /// was no such command.
    bool		 uninstallCommand(const char *name);

    /// Mark a command as having a special callback when inside a "false" if
    /// block.  That is, usually commands are skipped when they are inside an
    /// if block which has evaluated as false.  If this method is called, the
    /// command will have the given callback invoked in false if statements.
    bool		 markCommandForFalseIfStatements(const char *name,
					CMD_Callback cb);

    /// Install a callback that gets executed after every command. 
    /// 'userdata' is passed on each call to the callback.
    /// Returns the previous installed post command callback.
    CMD_PostCmdCallback	 setPostCmdCallback(
				CMD_PostCmdCallback callback, void *userdata);
    /// Return the userdata set in setPostCmdCallback().
    void *		 getPostCmdCallbackData() const
			    { return myPostCmdCallbackData; }

    /// Install a callback that gets run after every command is echoed,
    /// if echoing is turned on.  Your callback may never keep a pointer to
    /// the data that is passed in.
    void		 setCommandEchoCallback(
				CMD_CommandEchoCallback callback,
				void *userdata);
    /// Return the userdata set in setCommandEchoCallback().
    void *		 getCommandEchoCallbackData() const
			    { return myCommandEchoCallbackData; }

    /// Displays a list of all commands.
    void		 commandDump(std::ostream &os, const char *pattern=0,
				    const char *prefix=0) const;
    /// Displays help documentation for the given command. Returns \c true
    /// if a single command matched, \c false if multiple commands matched
    /// and a help message or a dump is printed out.
    void		 commandHelp(std::ostream &os, const char *cmd,
				     bool full_match_only = false) const;
    /// Searches help files for the given pattern.  HOUDINI_PATH is used to
    /// locate filename.
    void		 searchHelp(CMD_CommandList &list,
				    const char *filename,
				    const char *pattern) const;
    /// Note this only prints exact matches to the help.
    void		 printHelp(std::ostream &os, const char *filename,
				   const char *pattern = 0) const;
    void		 saveAliases(std::ostream &os);
    void		 saveVariables(std::ostream &os, bool level0_locals);

    /// Toggle to enable/disable recording of commands in the history buffer.
    /// The previous value of the toggle is returned.
    int			 setHistoryCapture(int onOff)
			 {
			     int	prev = myHistoryCapture;
			     myHistoryCapture = onOff;
			     return prev;
			 }
    CMD_Source		*pushSource(const char *filename, int verbose=0);
    CMD_Source		*popSource(int checkLoop = 1);
    CMD_Source		*getSource()
			 {
			    UT_ASSERT(mySourceStack.entries());
			    return mySourceStack(mySourceStack.entries()-1);
			 }
    int			 getSourceLevel() { return mySourceStack.entries()-1; }

    void		 pushOutputStreams(std::ostream &out, std::ostream &err);
    void		 popOutputStreams();

    /// Removes the named variable
    void		 unsetVariable(const char *name);
    /// Resets all variables to their defaults.
    void		 resetVariables();
    /// Sets a value for the named variable, creating it if necessary.  Set
    /// 'local' to 0 if the value should be exported.
    void		 setVariable(const char *name, const char *value,
					int local, int uplevel=0);

    /// Returns true if the named variable exists.
    bool		 hasVariable(const char *name) const;
    /// Returns the value of the named variable.
    bool		 getVariable(const char *name, UT_String &value)
			    { return getVariableValueByName(name, value); }
    /// Returns the value of the named variable as a fpreal64.
    bool		 getVariable(const char *name, fpreal64 &value)
			    { return getVariableValueByName(name, value); }
    /// Returns the value of the named variable as a float.
    bool		 getVariable(const char *name, fpreal32 &value)
			    { return getVariableValueByName(name, value); }
    /// Returns the value of the named variable as an int.
    bool		 getVariable(const char *name, int32 &value)
			    { return getVariableValueByName(name, value); }
    /// Returns the value of the named variable as an int.
    bool		 getVariable(const char *name, int64 &value)
			    { return getVariableValueByName(name, value); }

    bool		 hasEnv(const char *name) const;
    bool		 getEnv(const char *name, UT_String &value);
    /// Returns a list of all defined variables.
    void		 getVariableNames(UT_StringArray &names, int dirty=0);
    int			 clearDirtyVariables();
    int			 clearDirtyVariables(const UT_StringArray &names);
    /// Returns true if the name is legal and no variable with the name is
    /// currently defined.
    int			 isVariableNameOk(const char *name);

    void		 setMotName(const char *path);
    /// Convenience method to set the JOB variable.
    void		 setJob(const char *path);

    /// Return the number of commands run.  It's possible for this to overflow
    unsigned		 getCommandsRun() const	{ return myCommandCount; }

    std::ostream	&getError(int printMessage = 1);

    /// These two return pointers to their previous streams so that
    /// you can restore them if you need to:
    std::ostream	*setStandardOut(std::ostream &os);
    std::ostream	*setStandardErr(std::ostream &os);
    /// Call this if you are deleting a stream that was previously used
    /// in a call to setStandard[Out|Err].
    void		 streamDeleted(std::ostream &os);

    void		 setPrompt(const char *prompt);
    const char		*getPrompt(int level = -1) const
			 {
			     if (level < 0 || level >= myPromptStack.entries())
				 level = myPromptStack.entries()-1;
			    return myPromptStack(level);
			 }
    void		 pushPrompt(const char *prompt);
    void		 popPrompt();

    int			 isLoopCommand(const char *str);

    int			 evaluateCondition(const char *expr);
    int			 evaluateCondition(int argc, const char *argv[]);

    /// These two functions are inverses of each other. If the string is
    /// written out using saveProtectedString(), then you MUST use
    /// expandControlSequences() when loading the string back in.
    static void		 saveProtectedString(std::ostream &os, const char *s);
    static void		 expandControlSequences(UT_String &str);

    // buildQuotedCommandLineStringForBinaryData creates a single-quoted string
    // that can be used when building an hscript command string.  The data
    // encoded into the string may be arbitrary binary data that may contain
    // nulls.  The hscript command that receives this argument (after the
    // single-quoted string has been parsed by the hscript command-line engine)
    // can retrieve the original data with retrieveBinaryDataFromInsideCommand.
    static void buildQuotedCommandLineStringForBinaryData(
	const UT_StringRef &data, UT_WorkBuffer &result);
    static void retrieveBinaryDataFromInsideCommand(
	const char *encoded_data, UT_WorkBuffer &result);

    /// Displays the contents of the history buffer.
    void		 dumpHistory(std::ostream &os) const;
    /// Clears the history buffer.
    void		 clearHistory();

    /// Returns an entry from the history buffer.  Index 0 being the most
    /// recent command.
    const char		*getHistoryCommand(int n) const;
    /// Returns the number of entries in the history buffer.
    unsigned int	 getHistoryCount() const;

    CMD_VariableTable	*getGlobalVariables() { return myGlobals; }
    CMD_AliasTable	*getAliases() { return myAliases; }
    /// Returns the replacement for the named alias.  Returns false if the
    /// alias is not found.
    bool		 getAlias(const char *name, UT_String &value);
    /// Create a new alias or override an existing alias.
    void		 setAlias(const char *name, const char *value);
    /// Removes the named alias.
    void		 destroyAlias(const char *name);
    /// Removes all aliases.
    void		 destroyAliases();


    /// Specify whether a global hscript variable may be overwritten by an
    /// environment variable's value when loading the hip file, instead of
    /// just using the value saved in the hip file.
    void		 allowEnvironmentToOverwriteVariable(
			    const char *name, bool onoff);
    /// Should we use the environment variable value when loading this variable
    /// from the hip file?
    bool		 isEnvironmentAllowedToOverwriteVariable(
			    const char *name);
    /// Say that we're loading the global variables from a hip file or that
    /// we're done loading them.
    void		 setIsLoadingGlobalVariables(bool onoff)
			 { myIsLoadingGlobalVariables = onoff; }
    /// Are we loading global variables from the hip file?
    bool		 isLoadingGlobalVariables()
			 { return myIsLoadingGlobalVariables; }


    /// Register a variable change notification center object.
    void		 setVariableChangeNotifier(
					CMD_VariableChangeNotifier * notifier )
					{ myVariableChangeNotifier = notifier; }
    /// Returns a variable change notification center object.
    CMD_VariableChangeNotifier *    getVariableChangeNotifier() const
					{ return myVariableChangeNotifier; }

    void		 saveCommandString(std::ostream &os, const UT_String &str);

    /// Now, here, we provide some nice functions to handle getting arguments
    void		 getFrameRange(CMD_Args &args,
					int &fstart, int &fend, int &finc,
					char oframe = 'f', char ofinc = 'i');

    void		 setContinueLevel(int level) { myContinueLevel=level; }
    int			 getContinueLevel() const { return myContinueLevel; }
    void		 bumpBreakLevel(int dir) { myBreakLevel += dir; }
    int			 getBreakLevel() const { return myBreakLevel; }
    void		 bumpLoopNestLevel(int dir)
			 {
			     myLoopNestLevel += dir;
			     if (!myLoopNestLevel)
			     {
				myBreakLevel = 0;
			     }
			 }
    int			 getLoopNestLevel() const { return myLoopNestLevel; }
    CMD_Loop		*buildLoop(const char *text);
    void		 doPrompt();
    void		 getPrompt(UT_String &str);

    void		 setVerbose(int on_off);
    int			 getVerbose() const;

    void		 setCommandEcho(bool on_off)
    						{ myCommandEchoFlag = on_off; }
    bool		 getCommandEcho() const { return myCommandEchoFlag; }

    void		 setIORedirection(int on_off);
    int			 getIORedirection() const;

    /// Returns the getopts-style options for the given command.
    const char		*getOptions(const char *cmdName); 

    /// Returns true if the command is defined.
    int			 isCommandDefined(const char *name)
			 {
			     return findCommand(name, 1) >= 0;
			 }
    void		 getDSOCommands(CMD_CommandList &list);

    bool		 isLoading() const { return (myInLoadCount > 0); }
    void		 beginLoading() { myInLoadCount++; }
    void		 endLoading()	{ myInLoadCount--; }

    void		 setSafeMode(int safe_mode);
    int			 isInSafeMode() const;
    void		 setBrowserMode(int browser_mode);
    int			 isInBrowserMode() const;

    /// Sets the status code.  Use a non-zero value to indicate the last
    /// command failed.
    void		 setStatusCode( int status_code )
			 { myStatusCode = status_code; }
    /// Returns non-zero if the last command failed.
    int			 getStatusCode() const
			 { return myStatusCode; }

    /// The following versions of openPort vary slightly from one another.
    /// Some versions take CMD_Args for error output while some take
    /// an ostream.  Some versions take a const char * for the command to
    /// execute, while others take a "const char *const*" array of arguments
    /// for the command.
    ///
    /// Note that if we're told to execute a program, the port will be closed
    /// when the program exits.
    /// @{
    bool		 openPort(int port, bool safe, bool quiet,
				  bool wait, CMD_Args &args,
				  const char *execute, pid_t *pid,
				  bool separateerrors, bool suppress = true,
				  const char *ip_mask = 0,
				  FS_ServerSocketListener::ChildExitedCallback
						child_exited_callback = 0,
				  bool detach_console_for_execute=false,
				  bool drain_queue_when_waiting_for_exec=true);
    bool		 openPort(int port, bool safe, bool quiet,
				  bool wait, std::ostream &error_output,
				  const char *execute, pid_t *pid,
				  bool separateerrors, bool suppress = true,
				  const char *ip_mask = 0,
				  FS_ServerSocketListener::ChildExitedCallback
						child_exited_callback = 0,
				  bool detach_console_for_execute=false,
				  bool drain_queue_when_waiting_for_exec=true);
    bool		 openPort(int port, bool safe, bool quiet,
				  bool wait, CMD_Args &args,
				  const char *const*execute_argv, pid_t *pid,
				  bool separateerrors, bool suppress = true,
				  const char *ip_mask = 0,
				  FS_ServerSocketListener::ChildExitedCallback
						child_exited_callback = 0,
				  bool detach_console_for_execute=false,
				  bool drain_queue_when_waiting_for_exec=true);
    bool		 openPort(int port, bool safe, bool quiet,
				  bool wait, std::ostream &error_output,
				  const char *const*execute_argv, pid_t *pid,
				  bool separateerrors, bool suppress = true,
				  const char *ip_mask = 0,
				  FS_ServerSocketListener::ChildExitedCallback
						child_exited_callback = 0,
				  bool detach_console_for_execute=false,
				  bool drain_queue_when_waiting_for_exec=true);
    /// @}

    /// Closes a port opened with openPort().
    void		 closePort(int port);
    void		 closeAutoPort();

    bool		 beginChannelBlock();
    bool		 endChannelBlock();

    void		 echoOff();
    void		 echoOn();

    // Resolve channel labels and node path without exposing OP_Node*
    virtual bool getChRefLabelForUI(const CH_ChannelRef &r, UT_String &ret) const override;
    virtual bool getChRefAlias(const CH_ChannelRef &r, UT_String &ret) const override;
    virtual bool getChRefNodePath(const CH_ChannelRef &r, UT_String &ret) const override;

    /// Retrieve an automatically-generated port via openport -a
    /// Will be the same from call to call unless the port is closed
    static int		 getHScriptPort();

    /// Command for handling python command. This is a static public function
    /// so that the overridden version can call this default
    /// implementation depending on the command format.
    static void		 cmd_python(CMD_Args &args);

protected:
    bool	getVariableString(const char *name, UT_String &value,
				  int &timeDepend, int thread) override;

private:
    void	destroyContents();

    template <typename T> bool
			 getVariableValueByName(const char *name, T &value);

    friend class	 cmd_SendInput;
    friend class	 cmd_SendMultiLineInput;
    friend class	 cmd_Execute;

    void		 internalSendInput(const char *input,
					   bool run_pushed_source=false);
    // Commands for handling if statements
    static void		 cmd_if	  (CMD_Args &args);
    static void		 cmd_else (CMD_Args &args);
    static void		 cmd_endif(CMD_Args &args);
    static void		 cmd_read (CMD_Args &args);

    // This version of openPort() is called by the public versions.
    bool		 openPort(int port, bool safe, bool quiet,
				  bool wait, CMD_Args *args,
				  std::ostream *error_output,
				  const char *const*execute_argv, pid_t *pid,
				  bool separateerrors, bool suppress,
				  const char *ip_mask,
				  FS_ServerSocketListener::ChildExitedCallback
						child_exited_callback,
				  bool detach_console_for_execute,
				  bool drain_queue_when_waiting_for_exec=true);
    // Find a cmd for me, if exact is specified, the command has to match
    // exactly.
    void		 processInput(CMD_Source *top, const char *line);
    void		 internalExecute(const char *str);
    void		 runCommand(char *str);
    void		 initLibraryGlue(void);
    void		 setGlueContext();
    void		 setPortContext();
    int			 findCommand(const char *name, int exact = 0);
    void		 sortCommands(void);
    void		 installBaseCommands(void);
    void		 addPortExtension();
    CMD_CommandList	*getCommandList(const char *list_name);
    void		 installBuiltInVariables();
    void		 installObsoleteCommands();
    int			 aliasExpand(UT_String &str);
    void		 setReadVariables(const char *str);
    void		 breakAllSources();
    void		 updateNoAliasVariable();
    void		 pushStandardOut(std::ostream *os);
    void		 popStandardOut();
    void		 pushStandardErr(std::ostream *os);
    void		 popStandardErr();
    void		 updateStatusVariable();

    // Process the options for the command, stripping them out and returning
    // the new argc.
    void	 stripOptions(const CMD_Command *cmd, CMD_Args &args);

private: // data

    CMD_CommandList		*myCmdList;
    CMD_PromptStack		 myPromptStack;
    UT_String			 myPendingCommands;
    UT_String			 myReadVariables;
    int				 myReadVariablesLocal;
    UT_SymbolMap<CMD_Command *>	 myExactList;
    UT_SymbolMap<cmd_FalseCallback *> *myFalseMarkedCommands;
    CMD_SourceStack		 mySourceStack;
    CMD_VariableTable		*myGlobals;
    UT_StringSet		 myEnvironmentOverridableGlobals;
    bool			 myIsLoadingGlobalVariables;
    CMD_VariableChangeNotifier	*myVariableChangeNotifier;
    CMD_AliasTable		*myAliases;
    UT_ValArray<std::ostream *>	 myStandardOuts;
    UT_ValArray<std::ostream *>	 myStandardErrs;
    unsigned			 myCommandCount;
    int				 myAliasProtect;	// No alias expansion
    int				 myFlags;
    int				 myBreakLevel;
    int				 myContinueLevel;
    int				 myLoopNestLevel;
    int				 myLocalSetLevel;
    int				 myExecuteCommandStack;
    int				 myStatusCode;
    unsigned			 myAliasExpand:1,
				 myHistoryExpand:1,
                                 myHistoryCapture:1,
				 myIsInSafeMode:1,
				 myIsInBrowserMode:1;
    UT_String			 myAppName;
    UT_String			 myUnfinishedInput;

    int				 myInLoadCount;
    int				 myEchoOff;

    CMD_PostCmdCallback		 myPostCmdCallback;
    void			*myPostCmdCallbackData;

    // Support for command echoing
    CMD_CommandEchoCallback	 myCommandEchoCallback;
    void			*myCommandEchoCallbackData;

    // The command echo flag tells us whether or not to echo every command
    // back out to the stdout.  The myOldCommandEchoFlag is used to remember
    // our state so that we can turn off command echoing during a source.
    bool			 myCommandEchoFlag;

    // A set of commands that no longer do anything, but we keep them around
    // to prevent scripts and hip files from erroring out.
    UT_StringSet		 myObsoleteCommands;

    static CH_AutoEventHandler	*theCHEventHandler;
    static int			 theChannelBlockCount;
    static CMD_Manager		*theManager;
};

inline
CMD_Manager *CMDgetManager()
{
    return CMD_Manager::getContext();
}

//////////////////////////////////////////////////////////////////////////////
//
// Inline definitions
//

template <typename T> bool
CMD_Manager::getVariableValueByName(const char *name, T &value)
{
    CMD_Source		*src;
    CMD_VariableTable	*table;
    int			 i;

    for (i = mySourceStack.entries()-1; i >= 0; i--)
    {
	src = mySourceStack(i);
	table = src->getLocalVariables();
	if (table->getVariable(name, value))
	    return true;
    }
    return myGlobals->getVariable(name, value);
}

#endif
