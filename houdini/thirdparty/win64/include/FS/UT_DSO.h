/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	Dynamic shared object management
 *
 */

#ifndef __UT_DSO_H__
#define __UT_DSO_H__

#include "FS_API.h"

#include <UT/UT_PathSearch.h>
#include <SYS/SYS_Types.h>

#if defined( WIN32 )
    #define FS_DSO_EXTENSION	".dll"
#elif defined( MBSD )
    #define FS_DSO_EXTENSION	".dylib"
#else
    #define FS_DSO_EXTENSION	".so"
#endif

class dso_file;
class UT_String;
class UT_StringRef;

// This abstract class is only used as a return value from UT_DSO::loadDSO()
// and a parameter to UT_DSO::findSymbol().
class UT_DSOHandle;

class FS_API UT_DSO {
public:
    // These static functions provide a much simplified and more generic
    // interface over UT_DSO objects.  They also correspond more directly to
    // the underlying system's dlopen/LoadLibrary, dlsym/GetProcAddress, and
    // dlclose/FreeLibrary interface than do UT_DSO objects.

    // loadDSO() will load one particular shared object, using the system's
    // standard search path.  A pointer to an opaque UT_DSOHandle object is
    // returned on success, and you can use this handle to look up symbols.
    // on failure, this function returns null.
    //
    // Unlike UT_DSO objects, it will not load all the shared objects it can
    // find in Houdini's search paths.  Also, a parameter controls whether or
    // not the symbols are available to shared objects that are loaded later.
    // UT_DSO objects instead look for a special function in the shared object
    // to determine if its symbols should be made available.
    static UT_DSOHandle *loadDSO(
	    const char *file_name, bool available_to_later_loaded_libraries,
	    UT_String *error = NULL);

    // Find a symbol (function or global variable) in a loaded shared object.
    // Unlike the findProcedure() method on UT_DSO objects, you pass in
    // a dso object that's already been loaded, instead of a file name.  This
    // way you can quickly look up multiple symbols in the same shared library.
    // This function also doesn't for HoudiniDSOVersion symbols like UT_DSO
    // objects do.
    static void *findSymbol(UT_DSOHandle &handle, const char *symbol_name);

    // Decrement the reference cound on the loaded library, unloading it if
    // it's no longer used.
    static int closeDSO(UT_DSOHandle &handle);

    // Return the UT_DSOHandle for the main program.  This lets you look up
    // symbols in the main program from a shared library that was loaded.
    static UT_DSOHandle *getExecutable();

    // Return the UT_DSOHandle for a given loaded library.
    // Return NULL if the library is not loaded.
    // Use this function to check whether a particular library has been loaded
    // or to look up symbols in a library.
    static UT_DSOHandle *getLoadedLibrary(const char *lib_name);

    //------------------------------------------------------------------------

    // Given the name of a shared object, search for the file on disk using the
    // system's library path resolution mechanism and try to load it.  Returns
    // null if the file could not be found or couldn't be loaded for some
    // reason.
    static UT_DSOHandle *loadLibraryInPath(
	const char *shared_object_name,
	bool available_to_later_loaded_libraries);

    //------------------------------------------------------------------------

    // Load a dynamic object and run the function specified.
    bool	 run(const char *function_name, void *data = NULL,
		     bool validate_version = true);
    bool	 run(const char *filename, const char *function_name,
		     void *data = NULL, bool validate_version = true,
		     UT_KnownPath path = UT_HOUDINI_DSO_PATH);

    // This will go through the available DSOs in reverse order.
    // Normally, we run them from $HOME -> $HFS.  However, if the
    // called procedure will override earlier on successive calls (rather
    // than ignore) the correct order should be $HFS -> $HOME.  This is
    // the case with CMDextendLibrary.
    bool	 runReverse(const char *function_name, void *data = NULL,
        	            bool validate_version = true);

    // Rather than running the function, simply load the dynamic object and
    // return a pointer to the function.  This returns a pointer to the
    // function (or NULL if no function was found).
    // The actual name of the dynamic object is returned in the fullpath string.
    void	*findProcedure(const char *filename, const char *function_name,
			       UT_String &fullpath, 
			       bool validate_version = true);

    // This will return the file which is currently being loaded (i.e. in the
    // run() command.  If there is no file, then it will return a null ptr.
    static const char	*getRunningFile();

private:
    bool		 dsoListRun(exint data_i,
				    const UT_StringRef& func_name,
				    void* func_data,
				    bool validate_version,
				    exint func_i);
    
    static bool		 checkDSOVersion(UT_DSOHandle *handle, 
               		                 const char *filename);
    
    typedef void	(*DSOCallback)(void *);
    static DSOCallback	 loadDSOAndFindFunction(UT_DSOHandle *&handle, 
               	 	                        const char *file_name,
               	 	                        const char *func_name,
               	 	                        bool validate_version,
               	 	                        bool err_on_missing_func);
    static UT_DSOHandle *openLibrary(const char *filename);
private:
    static const char	*theRunningFile;
};

#endif
