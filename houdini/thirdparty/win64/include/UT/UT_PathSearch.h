/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PathSearch.h ( FS Library, C++)
 *
 * COMMENTS:
 *	This class defines Houdini path searching.  The class assumes that you
 *	will be searching for sub-paths within a given path.  Each path class
 *	will cache the list of directories to scan for.  This is done in the
 *	creation of the path searcher.
 *	Each path search is given a default path (which defaults to the
 *	houdini path), a path environment variable.
 *
 *	In the path expansion, the following rules apply:
 *		@	= Expands to the HOUDINI_PATH
 *		&	= Expands to the default path
 *		^	= Expands to the default sub-path
 *	Paths may be colon or semi-colon separated.
 *
 *	For example:
 *		UT_PathSearch	theHotkeyPath(def = "@/config/Hotkeys",
 *					env = "HOUDINI_HOTKEY_PATH");
 *		UT_PathSearch	theVexPath(
 *					def = "@/vex/^",
 *					env = "HOUDINI_VEX_PATH");
 *
 *	When searching for a file, can use one of:
 *		int findFile(const char *search, UT_String &path);
 *		int findAllFiles(const char *search,
 *			         UT_StringArray &paths);
 *
 * 	There are a number of well known paths.  Typically, you should simply
 * 	call the static access method to get the path that you're interested
 * 	in.
 */

#ifndef __UT_PathSearch__
#define __UT_PathSearch__

#include "UT_API.h"
#include "UT_WorkBuffer.h"
#include "UT_StringArray.h"


template <typename T> class UT_ValArray;
typedef UT_ValArray<const char *> UT_StringList;
class UT_StringSet;


enum UT_KnownPath
{
    UT_HOUDINI_PATH = 0,

    UT_HOUDINI_DSO_PATH,
    UT_HOUDINI_IMAGE_DSO_PATH,
    UT_HOUDINI_AUDIO_DSO_PATH,
    UT_HOUDINI_PDG_DSO_PATH,
    UT_HOUDINI_VEX_DSO_PATH,
    UT_MANTRA_DSO_PATH,

    UT_HOUDINI_OTL_PATH,
    UT_HOUDINI_OPLIBRARIES_PATH,
    UT_HOUDINI_OTLSCAN_PATH,
    UT_HOUDINI_SCRIPT_PATH,
    UT_HOUDINI_SOHO_PATH,
    UT_HOUDINI_GALLERY_PATH,
    UT_HOUDINI_PARM_DS_PATH,
    UT_HOUDINI_PDG_PATH,

    UT_HOUDINI_TEXTURE_PATH,
    UT_HOUDINI_GEOMETRY_PATH,
    UT_HOUDINI_CLIP_PATH,


    UT_VEX_DISPLACE_PATH,
    UT_VEX_FOG_PATH,
    UT_VEX_LIGHT_PATH,
    UT_VEX_SHADOW_PATH,
    UT_VEX_SURFACE_PATH,

    UT_VEX_CHOP_PATH,
    UT_VEX_COP2_PATH,
    UT_VEX_IMAGE3D_PATH,
    UT_VEX_SOP_PATH,
    UT_VEX_POP_PATH,
    UT_VEX_CVEX_PATH,

    UT_GLSL_PATH,

    UT_HOUDINI_UI_PATH,
    UT_HOUDINI_UI_APP_PATH,
    UT_HOUDINI_UI_AUDIO_PATH,
    UT_HOUDINI_UI_CURSOR_PATH,
    UT_HOUDINI_UI_DIALOG_PATH,
    UT_HOUDINI_UI_ICON_PATH,
    UT_HOUDINI_UI_TEMPLATE_PATH,

    // Now, we have the paths required for the UT_PathFile searching
    UT_PATHFILE_DESK,		// Desk macro
    UT_PATHFILE_ASSET_STORE,	// Asset store path
    UT_PATHFILE_TOOLBAR,	// Toolbar path
    UT_PATHFILE_RADIALMENU,	// Radial menu path
    UT_PATHFILE_CUSTOM,		// CPUI path
    UT_PATHFILE_PYTHON_PANEL,	// Python panel path
    UT_PATHFILE_MENU,		// XML menus path

    UT_OUTLINEFONT_PATH,	// Outline fonts

    UT_OCL_PATH,		// OpenCL kernels
    UT_VOP_TYPES_PATH,		// VOP connector and language types definitions

    UT_HOUDINI_VIEWERSTATE_PATH, // Viewer state path (viewer_states)

    UT_MAX_KNOWN_PATHS
};

class UT_API UT_PathSearch
{
public:
     UT_PathSearch(const char  *defpath,
		   int		var_id,		// From UT_EnvControl
		   const char  *carat_expand = "",
		   int		at_expand = UT_HOUDINI_PATH);
     UT_PathSearch(const UT_PathSearch &copy) = default;

    static bool	pathMap(UT_String &path);
    static void	pathMapSave(UT_WorkBuffer &wbuf, bool pretty);
    static void	pathMapLoad(const char *dict);
    static void	pathMapAdd(const char *src, const char *dest,
			    bool case_sensitive);
    static bool	pathMapRemove(const char *src);

    /// Given a variable name (e.g. "HOUDINI_PATH") return the known path enum
    /// associated with it.  If the name doesn't match a known path, the
    /// function will return @c UT_MAX_KNOWN_PATHS.
    static UT_KnownPath	 lookupPath(const char *name);

    /// Given a known path, return the variable name associated with the path
    /// Returns @c nullptr on error.
    static const char	*lookupPath(UT_KnownPath path);

    static const UT_PathSearch	*getInstance(UT_KnownPath path_type);

    // Make your own copy of a UT_PathSearch class.  You are responsible for
    // deleting this.  However, using this, you can add to the path dynamically
    // (i.e. CPP)
    static UT_PathSearch	*getHardPath(UT_KnownPath path_type);

    // Cause all known paths to be re-built.
    static void		rehashAll();
    // Rehashes a specific known path - note that this will not rehash
    // copies of known paths.
    static void		rehashSpecific(UT_KnownPath pathid);
    void		rehash();

    // A useful little convenience function which will return the expansion of
    // $HOME/houdini$HOUDINI_VER
    static void		getHomeHoudini(UT_String &str);

    // extending the path allows multiple paths to be created in a single
    // search path.  For example, when scanning for Surface shaders, the
    // carat_expand can be set to "Surface" and then extended with "include"
    void		extendPath(const char *defpath = 0,
				  int var_id = -1,
				  const char *carat_expand = 0,
				  int at_expand = -1);

    // Add a single path to the search path
    void		appendPath(const char *path);

    // Prepend single path to the search path
    void		prependPath(const char *path);

    // Find the first file that occurs in the path
    int		findFile(UT_String &result, const char *search) const;
    int		findFile(UT_WorkBuffer &result, const char *search) const;
    int		findDirectory(UT_String &result, const char *search) const;

    // Find the first of the given list of files in the path. If they exist
    // in the same directory, then the order is determined from the search_list
    bool	findFileFromList(UT_String &result,
				 const UT_StringArray &search_list) const;
    bool	findDirectoryFromList(UT_String &result,
				 const UT_StringArray &search_list) const;

    // Find all files in the path.  
    // The contents of the list is APPENDED to,
    // it isn't automatically cleared.
    int		findAllFiles(const char *search,
			     UT_StringArray &list) const;

    int		findAllDirectories(const char *search,
			     UT_StringArray &list) const;

    // This function will match all files with the given extension in the
    // search path and fill the given list with the filenames only. Returns the
    // # of entries in the filled list. (sample extension = ".vex")
    int		matchAllFiles(const char *extension,
			      bool returnfullpaths,
			      UT_StringArray &list,
			      bool recurse=false,
			      bool skip_dup_paths=false) const;

    // This function will match all files with one of the given extensions
    // in the search path and fill the given list with the filenames only.
    // Returns the # of entries in the filled list. The extensions should begin
    // with a period.
    int		matchAllFiles(const UT_StringList &exts,
			      bool returnfullpaths,
			      UT_StringArray &list,
			      bool recurse=false,
			      bool skip_dup_paths=false) const;

    int		matchAllFilesInSubdirectory(const char *extension,
					    const char *subdirectory,
					    bool returnfullpaths,
					    UT_StringArray &list,
					    bool recurse=false,
					    bool skip_dup_paths=false) const;

    // Old style find-file.  If a result buffer is passed in, the search
    // result will be stored in it.  This is a little dangerous since it's a
    // fixed length buffer.
    int		houdiniFindFile(const char *find, UT_String &result) const;
    int		houdiniFindFile(const char *find, UT_WorkBuffer &result) const;

    int		houdiniFindDirectory(const char *find, UT_String &result) const;
    int		houdiniFindDirectory(const char *find, UT_WorkBuffer &r) const;

    int		 getEntries() const		{ return myPaths.entries(); }
    const char	*getPathComponent(int i) const	{ return myPaths(i); }

    // Base paths are all paths tested, whether they are valid directories or
    // not.
    int		 getBaseEntries() const	   { return myBasePaths.entries(); }
    const char	*getBaseComponent(int i) const	{ return myBasePaths(i); }
    const char	*getCondensedComponent(int i) const
			{ return myCondensedPaths(i); }

    const char	*getDefaultPath() const		{ return myDefaultPath; }
    const char	*getCaratExpand() const		{ return myCaratExpand; }

    int		 getVariableId() const		{ return myVariableId; }
    int		 getAtExpand() const		{ return myAtExpand; }

    // Take a fully qualified path and collapse just the $HIP part of the path
    static int	 condenseHip(UT_String &path);
    static int	 condenseHip(UT_WorkBuffer &path);

    // Take a fully qualified path and collapse just the $JOB part of the path
    static int	 condenseJob(UT_String &path);

    // Helper struct which constructs with myPaths/myCondensedPaths suitable
    // for calling into condensePath().
    struct UT_API CommonPaths
    {
	CommonPaths();

	enum { NUM_PATHS = 7 };

	int size() const { return NUM_PATHS; }
	const char *hip() const { return myPaths[2]; }

	const char *myPaths[NUM_PATHS];
	const char *myCondensedPaths[NUM_PATHS];
    };

    // Take a fully qualified path and collapse it to a "common" variable path.
    // For example, $HFS/... instead of /usr/hfs5.3.391/...
    // @{
    // @note This is slow if you use it in a loop. Instead, use the alternate
    // form of condenseCommon() with a CommonPaths object constructed outside
    // the loop.
    static inline bool
    condenseCommon(UT_String &path)
    {
	CommonPaths common;
	return condenseCommon(path, path, common);
    }

    // @note This is slow if you use it in a loop. Instead, use the alternate
    // form of condenseCommon() with a CommonPaths object constructed outside
    // the loop.
    static inline bool
    condenseCommon(UT_WorkBuffer &path)
    {
	CommonPaths common;
	return condenseCommon(path, path.buffer(), common);
    }

    // @note This is slow if you use it in a loop. Instead, use the alternate
    // form of condenseCommon() with a CommonPaths object constructed outside
    // the loop.
    static inline bool
    condenseCommon(UT_String &condensed, const char *path)
    {
	CommonPaths common;
	return condenseCommon(condensed, path, common);
    }

    // @note This is slow if you use it in a loop. Instead, use the alternate
    // form of condenseCommon() with a CommonPaths object constructed outside
    // the loop.
    static inline bool
    condenseCommon(UT_WorkBuffer &condensed, const char *path)
    {
	CommonPaths common;
	return condenseCommon(condensed, path, common);
    }

    static inline bool
    condenseCommon(UT_String &path, const CommonPaths &common)
    {
	return condenseCommon(path, path, common);
    }

    static inline bool
    condenseCommon(UT_WorkBuffer &path, const CommonPaths &common)
    {
	return condenseCommon(path, path.buffer(), common);
    }

    static inline bool
    condenseCommon(UT_String &path, const char *source_path,
		   const CommonPaths &common)
    {
	return condensePath(path, source_path, common.myPaths,
			    common.myCondensedPaths, common.size());
    }

    static inline bool
    condenseCommon(UT_WorkBuffer &path, const char *source_path,
		   const CommonPaths &common)
    {
	return condensePath(path, source_path, common.myPaths,
			    common.myCondensedPaths, common.size());
    }
    // @}

    // The path variables passed in should ALWAYS be UT_StrControl types.
    // The values of these variables are tested against the leading portion of
    // the full path specified and the "longest" match is used for replacement
    // of the lead.  For example the result might be $HOME/relpath.
    static bool	 condensePath(UT_WorkBuffer &result,
			      const char *source_path,
			      const char *const paths[],
			      const char *const condensed_paths[],
			      int npaths);

    static inline bool
    condensePath(UT_String &path,
		 const char *source_path,
		 const char *const paths[],
		 const char *const condensed_paths[],
		 int npaths)
    {
	UT_WorkBuffer condensed;
	bool ok = condensePath(condensed, source_path,
			       paths, condensed_paths, npaths);
	if (ok)
	    condensed.copyIntoString(path);
	else if (path.buffer() != source_path)
	    path.harden(source_path);
	return ok;
    }

    static inline bool
    condensePath(UT_String &path,
	         const char *const paths[],
		 const char *const condensed_paths[],
		 int npaths)
    {
	return condensePath(path, path, paths, condensed_paths, npaths);
    }

    // This method will take a fully qualified path and strip off the lead
    // characters of the pathname -- resulting in path which can still be found
    // by the path searching code, but which is simpler in nature...
    // For example, the path $HOME/houdini/fonts/Type1/Courier might be
    // stripped to simply "Type1/Courier"
    static int	 stripLeadPath(UT_String &path, UT_KnownPath pathvar);

    // Similar to stripLeadPath except that it will always return the shortest
    // path, irrespective of the order of the paths.
    static int	 stripMaxLeadPath(UT_String &path, UT_KnownPath pathvar);

    // This method, given a filename that is possibly fully specified, will
    // try to return the fully specified path to the file.  Returns true
    // if absolute_path is valid, false otherwise.
    static bool  resolveAbsolutePath(const char *filename,
				     UT_String &absolute_path);

private:
    UT_PathSearch & operator=( const UT_PathSearch &copy ); // not implemented

private:
    void			 initialize(const char *def,
					   int var_id,
					   const char *carat,
					   int at_expand);
    void			 clearPath();

    /// Helper function for matchAllFiles() and matchAllFilesInSubdirectory().
    void			 matchAllFilesInPath(const UT_StringList &exts,
						     const char *path,
						     bool returnfullpaths,
						     bool recurse,
						     UT_StringArray &list,
						     UT_StringSet *seen)const;

    int				 myVariableId;
    UT_String			 myDefaultPath;
    UT_String			 myCaratExpand;
    int				 myAtExpand;
    int				 myMaxPath;	// Max path length

    UT_StringArray		 myPaths;
    UT_StringArray		 myBasePaths;
    UT_StringArray		 myCondensedPaths;
};

inline int
HoudiniFindFile(const char *find, UT_String &result)
{
    if (!UT_PathSearch::getInstance(UT_HOUDINI_PATH)->
			  houdiniFindFile(find, result))
	result = 0;
    return result.isstring();
}

inline int
HoudiniFindFile(const char *find, UT_WorkBuffer &result)
{
    if (!UT_PathSearch::getInstance(UT_HOUDINI_PATH)->
			  houdiniFindFile(find, result))
	result.clear();
    return result.length() != 0;
}

inline int
HoudiniFindMulti(const char *find, UT_StringArray &result)
{
    result.clear();
    UT_PathSearch::getInstance(UT_HOUDINI_PATH)->
	findAllFiles(find, result);
    return result.entries();
}

inline int
HoudiniFindDir(const char *find, UT_String &result)
{
    if (!UT_PathSearch::getInstance(UT_HOUDINI_PATH)->
			  houdiniFindDirectory(find, result))
	result = 0;
    return result.isstring();
}

inline int
HoudiniFindDir(const char *find, UT_WorkBuffer &result)
{
    if (!UT_PathSearch::getInstance(UT_HOUDINI_PATH)->
			  houdiniFindDirectory(find, result))
	result.clear();
    return result.length() != 0;
}

inline int
HoudiniFindDirMulti(const char *find, UT_StringArray &result)
{
    result.clear();
    UT_PathSearch::getInstance(UT_HOUDINI_PATH)->
	findAllDirectories(find, result);
    return result.entries();
}

#endif
