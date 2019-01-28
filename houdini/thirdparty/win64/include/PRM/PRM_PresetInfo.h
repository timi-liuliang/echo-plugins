/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_PresetInfo.h ( OP Library, C++)
 *
 * COMMENTS:	PresetInfo for an Operator type.
 */

#ifndef __PRM_PresetInfo__
#define __PRM_PresetInfo__

#include "PRM_API.h"
#include <iosfwd>
#include <UT/UT_Pair.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>

class PRM_ParmList;
class UT_StringArray;

typedef UT_Pair<UT_String, UT_String> UT_StringPair;
typedef UT_ValArray<UT_StringPair *> UT_StringPairList;

#define PRM_DEFAULT_PRESET	"Permanent Defaults"

class PRM_API PRM_PresetInfo {
public:
    /// Subdir is the name of the sub directory where the preset file can be
    /// found. Since presets can be also loaded from HDAs, subdir is synonymous
    /// with the operator table name (eg "Object"), except it can contain 
    /// version which the constructor strips off (eg, "Object12.5.100", 
    /// used for operator state parm dialogs.
    /// File is the preset file name, which is the same as the operator
    /// type it applies to (eg "myhda" or "ns::hda::1.0), the file name
    /// will be groomed to ensure it is a valid file name (eg, "myhda" or
    /// "ns-hda-1.0". By providing the unaltered optype name, it is possible
    /// to locate presets inside the HDA sections, which otherwise would be
    /// more difficult for namespaced optypes.
			 PRM_PresetInfo(const char *subdir, const char *file);
    virtual		~PRM_PresetInfo();

    void		 setIndexFile(const char *subdir);
    void		 setNeedsRefresh();

    int			 getNumPresets();
    const UT_String	&getPresetName(int i);
    const UT_String	&getPresetPath(int i);
    const UT_String	&getPresetFile();
    const UT_String	&getPresetSection();
    int			 getDefaultPreset();
    int			 getPresetIndex(const char *presetname);

    bool		 deletePresetFromDir(const char *name,
					     const char *dir = 0);
    bool		 savePresetToDir(PRM_ParmList *parmlist,
					 const char *name,
					 const char *dir);
    bool		 loadPresetFile(PRM_ParmList *parmlist,
					const char *filename);
    bool		 savePresetFile(PRM_ParmList *parmlist,
					const char *filename);

    // Should probably wrap these in an UT_ErrorManager::Scope so that they
    // will not fail because some other non-related error still exists
    // in the current UT_ErrorManager. We've had interesting problems
    // like that before.
    bool		 savePreset(PRM_ParmList *parmlist, std::ostream &os);
    bool		 loadPreset(PRM_ParmList *parmlist, UT_IStream &is);

    // This function makes sure the file name ends with ".preset".
    static void		 makeValidPresetFileName(UT_String &filename);

    static const UT_StringArray	&getValidSaveDirs();

    // Handle loading of basic contents (channels, values)
    static bool		 loadPresetContents(PRM_ParmList *parmlist,
				    const char *token, UT_IStream &is);

private:
    void		 clear();
    void		 findAllPresets();
    void		 refreshIfRequired();    

    static int		 comparePresets(UT_StringPair * const *p1,
					UT_StringPair * const *p2);

    UT_String		 mySubdir;
    UT_String		 myIndexFile;
    UT_String		 myPresetFile;
    UT_String		 myPresetSection;
    UT_StringPairList	 myPresets;
    bool		 myNeedsRefresh;
};

#endif
