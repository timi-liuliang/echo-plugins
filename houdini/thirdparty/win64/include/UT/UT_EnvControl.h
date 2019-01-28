/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_EnvControl.h ( UT Library, C++)
 *
 * COMMENTS:	In Houdini applications, there are many environment variables
 *		which control the behaviour of certain features/capabilities.
 *		This class provides a centralized place to efficiently query
 *		these variables.  The reason for the centralization is so that
 *		we can easily get a list of these control variables.  As well,
 *		this mechanism provides ways of changing the control settings
 *		from environment variables to disk file to registry entry.
 *		Currently only environment variable control is supported.
 *
 *
 *   When adding a new environment variable, please document the variable in
 *   $SHD/ref/env.txt
 */

#ifndef __UT_EnvControl__
#define __UT_EnvControl__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_StringArray.h"

enum UT_IntControl
{
    // UT library: Public interface
    ENV_HOUDINI_WINDOW_CONSOLE,
    ENV_HOUDINI_CONSOLE_LINES,
    ENV_HOUDINI_DISABLE_CONSOLE,
    ENV_HOUDINI_TEXT_CONSOLE,
    ENV_HOUDINI_VERBOSE_ERROR,
    ENV_HOUDINI_DISABLE_FILE_LOAD_WARNINGS,
    ENV_HOUDINI_INTERRUPT_THRESH,
    ENV_HOUDINI_OLD_SPINNER,
    ENV_HOUDINI_HSERVER_PORT,
    ENV_HOUDINI_DISABLE_CPUID,
    ENV_HOUDINI_DISABLE_MMX,
    ENV_HOUDINI_DISABLE_XMMX,
    ENV_HOUDINI_DISABLE_SSE,
    ENV_HOUDINI_DISABLE_3DNOW,
    ENV_HOUDINI_DISABLE_AVX,
    ENV_HOUDINI_OSX_SWAP_DEL_AND_BACKSPACE,
    
    // UT: Internal controls
    ENV_HOUDINI_COREDUMP,
    ENV_HOUDINI_UI_COREDUMP,
    ENV_HOUDINI_SYSTRACE,
    ENV_HOUDINI_ENABLE_UNDO_LOG,
    ENV_HOUDINI_INTERRUPT_DEBUG,
    ENV_HOUDINI_NO_ASSERTS,
    ENV_HOUDINI_INTERACTIVE_ASSERTS,
    ENV_HOUDINI_INTERACTIVE_ASSERTS_TRACE,
    ENV_HOUDINI_ACCESS_METHOD,
    ENV_HOUDINI_CONNECTIONS_DEBUG,

    // Houdini Environment File Control
    ENV_HOUDINI_NO_ENV_FILE_OVERRIDES,
    ENV_HOUDINI_NO_ENV_FILE,
    
    // FS: Public interface
    ENV_HOUDINI_DSO_ERROR,
    ENV_HOUDINI_DSO_DEEPBIND,
    ENV_HOUDINI_CPP_EXPAND_PRAGMA,
    ENV_HOUDINI_ANONYMOUS_STATISTICS,
    ENV_HOUDINI_ANONYMOUS_STATISTICS_UPLOAD_ERRORS,

    // GA: Public interface
    ENV_HOUDINI_GEOMETRY_CACHESIZE,

    // IMG3D: Public interface
    ENV_HOUDINI_TEX3D_USAGE,

    // IMG: Public interface
    ENV_HOUDINI_CINEON_WHITE_POINT,
    ENV_HOUDINI_CINEON_BLACK_POINT,
    ENV_HOUDINI_CINEON_FULL_RANGE,
    ENV_HOUDINI_TIFF_SAMPLEFORMAT,
    ENV_HOUDINI_TIFF_BOTTOMLEFT,
    ENV_HOUDINI_DISABLE_IMAGE_DSO,
    ENV_HOUDINI_RAT_USAGE,
    ENV_HOUDINI_RAT_FILES,
    ENV_HOUDINI_RAT_TILESIZE,
    ENV_HOUDINI_RAT_LOCALCACHE,
    ENV_HOUDINI_RAT_OPTIMIZE_BANDWIDTH,
    ENV_HOUDINI_PTEX_USAGE,
    ENV_HOUDINI_PTEX_FILES,
    ENV_HOUDINI_PTEX_WINDING,
    ENV_HOUDINI_UVENLARGER_DIFFUSE_WRAP,
    ENV_HOUDINI_THREAD_IMAGE,
    ENV_HOUDINI_MDISPLAY_WAIT,
    ENV_HOUDINI_IMAGE_DEBUG,
    ENV_HOUDINI_TARGA_LINEAR,
    ENV_HOUDINI_ENABLE_EXR_TEXTURE,
    // IMG: Private interface
    ENV_HOUDINI_MIP_READ,
    ENV_HOUDINI_FB_FILTER_ORIGINAL,
    ENV_HOUDINI_SOCKET_PORT,
    ENV_HOUDINI_AUDIT_IO,
    ENV_HOUDINI_OLD_RATS,
    ENV_HOUDINI_OCIO_FILENAME_COLORSPACE,

    // MV: Public interface
    ENV_HOUDINI_QTML_USE_GDI,

    // VEX: Public interface
    ENV_HOUDINI_VEX_DISABLE_AUTO_FOLDING,
    ENV_HOUDINI_VEX_DISABLE_INSTANCE_CACHE,
    ENV_HOUDINI_VEX_DISABLE_JIT,
    ENV_HOUDINI_VEX_PROFINFO,
    ENV_HOUDINI_VEX_PROFINFO_HTML,
    ENV_HOUDINI_VEX_ASSERT,
    ENV_HOUDINI_VEX_MODULECACHESIZE,
    ENV_HOUDINI_VEX_RETURNBYREFERENCE,
    ENV_HOUDINI_VEX_MEMORYCACHESIZE,
    
    // VEX: Private interface
    ENV_HOUDINI_VEX_URANDOM,

    // RE: Public interface
    ENV_HOUDINI_OVERRIDE_XRES,
    ENV_HOUDINI_OVERRIDE_YRES,
    ENV_HOUDINI_UISCALE,
    ENV_HOUDINI_OGL_ERROR_CHECK,
    ENV_HOUDINI_OGL_DUMP_SHADER_ERRORS,
    ENV_HOUDINI_OGL_DEBUG,
    ENV_HOUDINI_OGL_ENABLE_TEXTURE_SEQUENCES,
    ENV_HOUDINI_OGL_CORE_PROFILE,
    ENV_HOUDINI_ENABLE_RETINA,

    // CMD
    ENV_HOUDINI_HIP_FROM_PATH,

    // OP
    ENV_HOUDINI_NO_EXPORT_RENAME,
    ENV_HOUDINI_SCRIPT_DEBUG,
    ENV_HOUDINI_COOK_MODE_THRESHOLD,

    // GEO
    ENV_HOUDINI_RMAN_CURVE_STEP,
    ENV_HOUDINI_DISABLE_ALEMBIC_INDEXED_ARRAYS,
    ENV_HOUDINI_ALEMBIC_OGAWA_STREAMS,

    // GU
    ENV_HOUDINI_GU_DEBUG,

    // RAY
    ENV_MANTRA_FORCE_RAY,
    ENV_MANTRA_NOFOGBOX,
    ENV_MANTRA_BIAS_NORMAL,
    ENV_MANTRA_DEBUGMEMORY,
    ENV_MANTRA_NONRAT_ERROR,
    ENV_MANTRA_LINEAR_CREASES,
    ENV_MANTRA_INTERNAL_RUNLIMIT,
    ENV_MANTRA_THREADED_REFINE,
    ENV_MANTRA_RENDERSTATE_STRIP_SPLITS,

    // COP
    ENV_HOUDINI_COP_ENSURECOLORPLANES,

    // IFD
    ENV_HOUDINI_RMAN_INCLUDE_FIX,
    ENV_HOUDINI_MI_DONT_FLIP_RMAP,
    ENV_HOUDINI_DISABLE_SOHO_SPOOLING,

    // MAT
    ENV_HOUDINI_DISABLE_SAVE_THUMB,

    // UI
    ENV_HOUDINI_UI_DEBUG,
    ENV_HOUDINI_QT_DEBUG,
    ENV_HOUDINI_ENABLE_FPS_SCALE,
    ENV_HOUDINI_UI_QUEUE_SIZE,
    ENV_HOUDINI_UI_QUEUE_DEBUG,
    ENV_HOUDINI_X11_SYNCHRONOUS,
    ENV_HOUDINI_WRITE_BINARY_ICONS,
    ENV_HOUDINI_NO_SPLASH,
    ENV_HOUDINI_NO_START_PAGE_SPLASH,
    ENV_HOUDINI_ENABLE_TABLET,
    ENV_HOUDINI_USE_NATIVE_FILE_CHOOSER,
    ENV_HOUDINI_MAX_FILE_HISTORY,
    ENV_HOUDINI_FONT_RENDERING,
    ENV_HOUDINI_RUN_IN_FOREGROUND,

    // FP
    ENV_HOUDINI_FPS,

    // OPUI
    ENV_HOUDINI_PERFORMANCE_SIZE,
    ENV_HOUDINI_BUFFEREDSAVE,
    ENV_HOUDINI_HIPEXT,
    ENV_HOUDINI_ENABLE_OLD_PRESETS,
    ENV_HOUDINI_USE_OTL_AS_DEFAULT_HDA_EXT,

    // DM
    ENV_HOUDINI_SKIP_NAME,
    ENV_HOUDINI_OGL_FILECOUNT,

    // MATUI
    ENV_HOUDINI_TOP_POS_SINGLE_PASS,

    // Custom
    ENV_HOUDINI_COP_TESTFRACTION,
    ENV_HOUDINI_ABEKAS_PAL_XRES,
    ENV_HOUDINI_ABEKAS_PAL_YRES,
    ENV_HOUDINI_IPLAY_IP,

    // LM
    ENV_HOUDINI_NOHKEY,
    ENV_HOUDINI_NOHKEY_APPRENTICE,

    // CHOP
    ENV_HOUDINI_CHOP_OLDNOISE,

    // SHOP
    ENV_HOUDINI_FULL_SHADERSTRING,
    ENV_HOUDINI_TYPED_SHADERSTRING,

    // VOP
    ENV_HOUDINI_RSL_HASHING,

    // SHLF
    ENV_HOUDINI_NO_DEFAULT_SHELF_TOOLS,
    ENV_HOUDINI_SHELF_EXCLUDE_DIRS_PATTERN,

    // Other
    ENV_HOUDINI_CONDENSE_HIP,
    ENV_HOUDINI_UNBUFFERED_STDINOUT,
    ENV_HOUDINI_VERBOSE_ASSET_LICENSING,
    ENV_HOUDINI_NO_ASSET_INSTANTIATION,   

    // Backwards Compatability
    ENV_HOUDINI4_COMPATIBILITY,
    ENV_HOUDINI55_COMPATIBILITY,
    ENV_HOUDINI6_COMPATIBILITY,
    ENV_HOUDINI7_COMPATIBILITY,
    ENV_HOUDINI8_COMPATIBILITY,
    ENV_HOUDINI95_COMPATIBILITY,
    ENV_HOUDINI10_COMPATIBILITY,
    ENV_HOUDINI11_COMPATIBILITY,

    ENV_HOUDINI11_PERFMON_COMPATIBILITY,
    ENV_HOUDINI10_VOLUME_COMPATIBILITY,
    ENV_HOUDINI6_FILTER_COMPATIBILITY,
    ENV_HOUDINI13_FILTER_COMPATIBILITY,
    ENV_HOUDINI_MUV_ATTRIB_COMPATIBILITY,
    ENV_HOUDINI_SIMPLIFIED_NODE_NAMES,
    ENV_HOUDINI13_VOLUME_COMPATIBILITY,
    ENV_HOUDINI13_GEO_COMPATIBILITY,
    ENV_HOUDINI14_GEO_COMPATIBILITY,
    ENV_HOUDINI15_PARMDATA_COMPATIBILITY,
    ENV_HOUDINI15_5_GEO_COMPATIBILITY,

    ENV_HOUDINI_LMINFO_VERBOSE,

    // hscript
    ENV_HSCRIPT_NO_HISTORY,
    ENV_HSCRIPT_SAFEQUIT,
    ENV_HSCRIPT_ALLOW_BACKGROUND_RENDER,

    ENV_HOUDINI_OLD_CONST_BEHAVIOUR,

    // This is for "internal use" only.  It turns on features that are not
    // ready for prime-time.
    ENV_HOUDINI_EXPERIMENTAL,

    // Maximum number of files to keep in the backup directory
    ENV_HOUDINI_MAX_BACKUP_FILES,

    ENV_HOUDINI_JITTERY_CAMERA,

    ENV_HOUDINI_ERRORLOG_LEVEL,

    ENV_HOUDINI_MAXTHREADS,
    ENV_HOUDINI_MAXSPUS,

    ENV_HOUDINI_MMB_PAN,
    ENV_HOUDINI_INTERNAL_IGNORE_SIGNALS,

    ENV_HOUDINI_EXTERNAL_HELP_BROWSER,

    ENV_HOUDINI_CASE_SENSITIVE_FS,

    // Controls the number of complete FBX scenes to
    // keep cached in memory after import. Defaults to 1.
    ENV_HOUDINI_MAX_CACHED_FBX_SCENES,

    ENV_HOUDINI_NETEXCHANGE_HEARTBEAT,
    ENV_HOUDINI_DOPVOLUME_NANTEST,

    ENV_HOUDINI_MULTITHREADED_COOKING,

    ENV_HOUDINI_SHOW_OLD_RENDER_PROPERTIES,	// Show all properties
    ENV_HOUDINI_SHOW_OLD_SHOPS,

    ENV_HOUDINI_OGL_FIXEDVOXELSIZE,
	
    // OpenCL
    ENV_HOUDINI_OCL_DEVICENUMBER,
    ENV_HOUDINI_OCL_REPORT_MEMORY_USE,

    ENV_HOUDINI_SAS_ENABLE,
    ENV_HOUDINI_SAS_COMPILE_ENABLE,
    ENV_HOUDINI_SAS_DISABLE_ASSET_DOWNLOAD,

    ENV_HOUDINI_NO_LEGACY_MENUS,

    // Maximum megabytes in write buffer.
    ENV_HOUDINI_IOBUFFER_SIZE,

    ENV_HOUDINI_INTERNAL_CRASH_ON_NANS,

    ENV_HOUDINI_AUTOCONVERT_IMAGE_FILES,
    ENV_HOUDINI_IMAGE_DISPLAY_OVERRIDE,
    ENV_HOUDINI_ANIMATE_NETWORK_CHANGES,

    ENV_HOUDINI_SOP_DISABLE_CACHENODE,
    ENV_HOUDINI_DOP_MAXCACHE,

    ENV_HOUDINI_OGL_FAST_BONES,
    ENV_HOUDINI_FAST_SKIN_BLENDSHAPES,

    ENV_HOUDINI_DISTRIBUTEDPROJECTION_DIAGNOSTICS,

    ENV_HOUDINI_USE_HFS_OCL,

    ENV_HOUDINI_DISABLE_AUDIO_SERVER,

    ENV_HOUDINI_CHOP_MOTION_SAMPLES,

    ENV_HOUDINI_SOP_DISABLE_COMPILENODE,
    ENV_HOUDINI_DISABLE_JEMALLOCTEST,

    ENV_HOUDINI_ENABLE_3DMOUSE,

    ENV_HOUDINI16_5_DEFORM_COMPATIBILITY,

    // Used to force regular LM checks on Steam builds
    ENV_HOUDINI_IGNORE_STEAM_LICENSING,

    // OptiX
    ENV_HOUDINI_NVIDIA_OPTIX_DEVICENUMBER,

    ENV_MAX_INT_CONTROLS
};

enum UT_FltControl
{
    // UT: Public interface
    ENV_HOUDINI_GAUSS_EXP,
    ENV_HOUDINI_MITCHELL_B,
    ENV_HOUDINI_MITCHELL_C,

    // UT: Internal variables
    ENV_HOUDINI_SYSTRACE_THRESH,

    // IMG: Public interface
    ENV_HOUDINI_CINEON_FILM_GAMMA,
    ENV_HOUDINI_IMAGE_DISPLAY_GAMMA,
    
    // RE
    ENV_HOUDINI_OGL_MAX_GL_VERSION,
    
    // UI
    ENV_HOUDINI_CHOP_LINE_WIDTH,
    ENV_HOUDINI_COLOR_PICKER_GAMMA,
    ENV_HOUDINI_LINUX_DOUBLE_CLICK_TIME,

    // GR
    ENV_HOUDINI_LOD,

    // Custom
    ENV_HOUDINI_WF_GAMMA,

    // ROP: Internal variables
    ENV_HOUDINI_CHECK_XFORM,

    // OpenCL: Memory pool size, as percentage of GPU memory.
    ENV_HOUDINI_OCL_MEMORY_POOL_SIZE,

    ENV_MAX_FLT_CONTROLS
};

enum UT_StrControl
{
    // UT: Public interface
    ENV_HOME,
    ENV_USER,
    ENV_EDITOR,
    ENV_VISUAL,
    ENV_SHELL,
    ENV_HFS,
    ENV_HOUDINI_OS,
    ENV_HOUDINI_TEMP_DIR,
    ENV_HOUDINI_DESKTOP_DIR,
    ENV_HOUDINI_UNDO_DIR,
    ENV_HOUDINI_MESSAGE_CMD,
    ENV_HOUDINI_CONFIRM_CMD,
    ENV_HOUDINI_ASSERT_CMD,
    ENV_HOUDINI_WEB_BROWSER_CMD,
    ENV_HOUDINI_TERMINAL_CMD,

    // UT: Internal variables
    ENV_HOUDINI_SYSTRACE_FILENAME,
    ENV_HOUDINI_PATHMAP,

    // external tools
    ENV_HOUDINI_COMPARE,

    // IMG: Public interface
    ENV_HOUDINI_DEF_TEXTURE_COLOR,

    // VEX(VMAT)
    ENV_HOUDINI_DEFAULT_SURFACE,
    ENV_HOUDINI_DEFAULT_VOLUME,
    ENV_HOUDINI_DEFAULT_LIGHT,
    ENV_HOUDINI_DEFAULT_AMBIENT,
    ENV_HOUDINI_DEFAULT_MATTE,
    ENV_HOUDINI_DEFAULT_SHADOW,
    ENV_HOUDINI_VEX_JIT_OPTIMIZE,

    // HOM
    ENV_HOUDINI_SCRIPT_LICENSE,

    // OP
    ENV_HOUDINI_HIP_DEFAULT_NAME,	// "untitled.hip"
    ENV_HOUDINI_COPY_SUFFIX,
    ENV_HOUDINI_OPNAMESPACE_HIERARCHY,

    // VOP
    ENV_HOUDINI_VOP_DEFINITIONS_PATH,

    // GEO
    ENV_HOUDINI_RMAN_CURVE_BASIS,

    // COP
    ENV_HOUDINI_CINEON_RENDER_LUT,
    ENV_HOUDINI_CINEON_PREVIEW_LUT,
    ENV_HOUDINI_COP_DISKDEFAULT,
    ENV_HOUDINI_IMAGE_DISPLAY_LUT,
    ENV_HOUDINI_IMAGE_INSPECT_LUT,
    ENV_HOUDINI_OCIO_SRGB_FILE_COLORSPACE,

    // CHOP
    ENV_HOUDINI_UNITS,

    // IFD
    ENV_HOUDINI_MANTRA_SHADOW,
    ENV_HOUDINI_MANTRA_REFLECT,
    ENV_HOUDINI_RI_SHADERPATH,
    ENV_HOUDINI_RI_RIXPLUGINPATH,
    ENV_HOUDINI_RI_TEXTUREPATH,
    ENV_HOUDINI_RI_EYESPLITS,
    ENV_MANTRA_ENGINE_PROCEDURAL,

    // UI
    ENV_HOUDINI_FILE_VIEWER,
    ENV_HOUDINI_UI_PATH,
    ENV_HOUDINI_UI_RESOURCES,
    ENV_HOUDINI_UI_APP_PATH,
    ENV_HOUDINI_UI_AUDIO_PATH,
    ENV_HOUDINI_UI_CURSOR_PATH,
    ENV_HOUDINI_UI_DIALOG_PATH,
    ENV_HOUDINI_UI_ICON_PATH,
    ENV_HOUDINI_UI_TEMPLATE_PATH,
    ENV_HOUDINI_ICON_CACHE_DIR,

    // FUI
    ENV_HOUDINI_STPATH,
    ENV_HOUDINI_STEDIT,

    // CHUI
    ENV_HOUDINI_CHOP_IMAGE,

    // OPUI
    ENV_HOUDINI_DOUBLEBUFFER,
    ENV_HOUDINI_WORKSHEET_BOXPICK,
    ENV_HOUDINI_DEFOPTYPEPREFIX,

    // VEX(VCC)
    ENV_HOUDINI_VEX_COMPILER,

    // Custom
    ENV_HOUDINI_ABEKAS_LOGIN_NAME,
    ENV_HOUDINI_ABEKAS_LOGIN_PASS,
    ENV_HOUDINI_MPLAY_WRITE_CMD,
    ENV_HOUDINI_MPLAY_FLIP_CMD,
    ENV_HOUDINI_MPLAY_LOCKPATH,
    ENV_HOUDINI_PLAY_WRITE_CMD,
    ENV_HOUDINI_PLAY_FLIP_CMD,
    ENV_HOUDINI_VIEW_METACAM,	//OTLs that lock to view in a special way

    // Path variables
    ENV_HOUDINI_PATH,
    ENV_HOUDINI_DSO_PATH,
    ENV_HOUDINI_IMAGE_DSO_PATH,
    ENV_HOUDINI_AUDIO_DSO_PATH,
    ENV_HOUDINI_PDG_DSO_PATH,
    ENV_HOUDINI_VEX_DSO_PATH,
    ENV_MANTRA_DSO_PATH,
    ENV_HOUDINI_OTL_PATH,
    ENV_HOUDINI_OPLIBRARIES_PATH,
    ENV_HOUDINI_OTLSCAN_PATH,
    ENV_HOUDINI_SCRIPT_PATH,
    ENV_HOUDINI_SOHO_PATH,
    ENV_HOUDINI_GALLERY_PATH,
    ENV_HOUDINI_PARM_DS_PATH,
    ENV_HOUDINI_DESKTOP_PATH,
    ENV_HOUDINI_ASSET_STORE_PATH,
    ENV_HOUDINI_TOOLBAR_PATH,
    ENV_HOUDINI_RADIALMENU_PATH,
    ENV_HOUDINI_CUSTOM_PATH,
    ENV_HOUDINI_PYTHON_PANEL_PATH,
    ENV_HOUDINI_MENU_PATH,
    ENV_HOUDINI_PDG_PATH,

    ENV_HOUDINI_TEXTURE_PATH,
    ENV_HOUDINI_GEOMETRY_PATH,
    ENV_HOUDINI_CLIP_PATH,

    ENV_HOUDINI_VEX_PATH,

    ENV_HOUDINI_GLSL_PATH,

    ENV_HOUDINI_OUTLINEFONT_PATH,

    ENV_HOUDINI_BACKUP_DIR,

    ENV_HOUDINI_BACKUP_FILENAME,
    ENV_HOUDINI_AUTOSAVE_FILENAME,

    ENV_HOUDINI_VIEW_MANTRA,
    ENV_HOUDINI_VIEW_RMAN,
    ENV_HOUDINI_DEFAULT_RENDERERS,
    ENV_HOUDINI_DEFAULT_RIB_RENDERER,
    ENV_HOUDINI_RIBSHADER_DIR,
    ENV_HOUDINI_RIBTEMP_DIR,

    ENV_HOUDINI_THUMB_RENDER,

    // These are here so we can have them show up in hconfig and provide
    // help.  They should only be used implicitly.
    ENV_HSITE,		// Between HFS and HOME.
    ENV_JOB,		// We should not use JOB!

    ENV_HOUDINI_ERRORLOG_FILENAME,

    ENV_HOUDINI_SPLASH_MESSAGE,
    ENV_HOUDINI_SPLASH_FILE,

    ENV_HOUDINI_GEOMETRY_FORMAT,

    // OPENGL
    ENV_HOUDINI_OGL_DEBUG_EXCLUDE,
    ENV_HOUDINI_OGL_PICKBUFFER_PATH,
    ENV_HOUDINI_OGL_SNAPBUFFER_PATH,
    ENV_HOUDINI_OGL_BEAUTY_PATH,
    ENV_HOUDINI_OGL_VIEWPORT_PATH,

    // OPENCL
    ENV_HOUDINI_OCL_DEVICETYPE,
    ENV_HOUDINI_OCL_VENDOR,
    ENV_HOUDINI_OCL_PATH,
    
    // Orbolt
    ENV_HOUDINI_SAS_SERVER,

    // Anonymous Usage Statistics
    ENV_HOUDINI_STATS_API_URL,

    ENV_HOUDINI_DSO_EXCLUDE_PATTERN,

    ENV_HOUDINI_USER_PREF_DIR,

    ENV_HOUDINI_AUTHOR,

    ENV_PDGDIR,

    ENV_HOUDINI_VIEWERSTATE_PATH,

    // Optix
    ENV_HOUDINI_NVIDIA_OPTIX_DSO_PATH,

    ENV_MAX_STR_CONTROLS
};

class UT_API UT_EnvControl
{
public:
     UT_EnvControl();
    ~UT_EnvControl();

    static int		 getInt(UT_IntControl i)
				{ return getControl().myIValues[i]; }
    static fpreal	 getFloat(UT_FltControl i)
				{ return getControl().myFValues[i]; }
    static const char	*getString(UT_StrControl i)
				{ return getControl().mySValues[i]; }

    // Look up the appropriate variable name for the token
    static const char	*getIntVar(UT_IntControl i);
    static const char	*getFloatVar(UT_FltControl i);
    static const char	*getStringVar(UT_StrControl i);

    static void		 setStringVar(UT_StrControl i, const char *value);

    // Look up the appropriate old style variable name for the token.  This can
    // be used to check for old environment variables.
    static const char	*getOldIntVar(UT_IntControl i);
    static const char	*getOldFloatVar(UT_FltControl i);
    static const char	*getOldStringVar(UT_StrControl i);

    /// Indicate that the given variable changed and update our cached values.
    /// If varname is NULL, then all variables are updated.
    static void		 varChanged(const char *varname)
			    { varChanged(varname, nullptr); }

    /// Update the cached varname value with the given value so that we can
    /// avoid an extraneous HoudiniGetenv() for the equivalent code snippet:
    /// @code
    ///      HoudiniSetenv(name, value);
    ///      UT_EnvControl::varChanged(name);
    /// @endcode
    /// @return true if name was found and value is non-null, in which case the
    /// HoudiniSetenv() was called, else false is returned.
    static bool		 varChanged(const char *varname, const char *value);

    /// Like varChanged() except that it can avoid an extraneous
    /// HoudiniGetenv() for the equivalent code snippet:
    /// @code
    ///      HoudiniSetenv(name, value);
    ///      UT_EnvControl::varChanged(name);
    /// @endcode
    /// @return true if name was found, in which case the HoudiniSetenv() will
    /// be called, else false is returned.
    static bool		 changeVar(const char *name, const char *value);

    static void		 changeExistLookupDefault(UT_IntControl i, int new_def);

    /// Set all additional .env files to be parsed for env variable values.
    /// It is best to call getAdditionalEnvironmentFiles() first and then
    /// append to the list because this function will overwrite all the
    /// additional paths that were previously set.
    /// You can still use the HOUDINI_NO_ENV_FILE_OVERRIDES to disallow
    /// overwrites, knowing that these files get applied AFTER the default
    /// houdini.env file in the user's home directory.
    /// That said, HOUDINI_NO_ENV_FILE will have no effect on this function.
    /// If additional files are set, they will be used, regardless of the
    /// value of HOUDINI_NO_ENV_FILE.
    static void          setAdditionalEnvironmentFiles(
                                        const UT_StringArray &files);

    /// Return the current list of additional environment files.
    static const UT_StringArray&
                         getAdditionalEnvironmentFiles();

private:
    void		 doInit(bool init_defaults);
    void		 setDefaults();
    bool                 loadEnvironmentFile(const char *path,
                                             bool allow_overrides);
    void                 loadDefaultEnvironmentFile();
    void                 loadEnvironmentFiles();
    void                 loadEnvironment(bool init_defaults);

    void		 updateExistLookup(int i, const char *value = nullptr);
    void		 updateIntLookup(int i, const char *value = nullptr);
    void		 updateFltLookup(int i, const char *value = nullptr);
    void		 updateStrLookup(int i, bool fix_path,
					 const char *value = nullptr);

    int				 myIValues[ENV_MAX_INT_CONTROLS];
    float			 myFValues[ENV_MAX_FLT_CONTROLS];
    char			*mySValues[ENV_MAX_STR_CONTROLS];

    static UT_EnvControl	&getControl()
				 {
				    if (!theControl)
					theControl = new UT_EnvControl();
				    return *theControl;
				 }
    static UT_EnvControl	*theControl;

    static UT_StringArray        theAdditionalEnvironmentFiles;
};

#endif
