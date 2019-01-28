/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FONT_Registry.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FONT_Registry__
#define __FONT_Registry__

#include "FONT_API.h"

#include "FONT_Enums.h"
#include "FONT_Info.h"

#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_ArrayStringSet.h>
#include <UT/UT_Set.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_ValArray.h>

class UT_JSONParser;
class UT_JSONWriter;
class UT_JSONValueMap;

struct font_CacheInfo;
typedef UT_ArrayStringMap<font_CacheInfo> font_CacheMap;

typedef UT_Array<const FONT_Info *> FONT_InfoPtrList;
typedef struct FT_FaceRec_*  FT_Face;


class FONT_API FONT_Registry
{
public:
    ~FONT_Registry();
    
    /// Returns the FONT_Registry singleton.
    static FONT_Registry   &instance();

    /// Return a alphabetic list of all available typefaces.
    UT_StringList 	    typefaces() const;

    /// Returns all fonts from the given typeface.
    const FONT_InfoPtrList &typefaceFonts(const char *typeface) const;

    /// Return a list of all font files that have been loaded.
    /// If `include_system_font_files` is true then the returned list includes
    /// font files that were loaded from system directories.  Otherwise,
    /// the returned list includes only font files found in the Houdini
    /// search path.
    UT_StringArray	    fontFilePaths(
				bool include_system_font_files=true) const;

    /// Returns the closest variant on the given font with the additional
    /// constraints.
    /// \param font The font to base the variant on.
    /// \param variant Specifies the variation upon the base font to return.
    const FONT_Info	   &getVariant(
				const FONT_Info &font_info,
				FONT_Variant variant) const;
    
    /// Return a font face given an name. The name can be any of the possible
    /// alternative names for a font. For example font names used in older
    /// version of Houdini (e.g. "DejaVuSansMono-BoldOblique" would be the
    /// "DejaVu Sans Mono" family, bold weight, oblique style), for factory
    /// fonts. For system fonts, the path name would have been given.
    /// If the font is not found, an invalid FONT_Info object is returned,
    /// containing just the font name.
    const FONT_Info 	   &get(const char *name) const;
    
    /// Returns a comma separated list of file globs that match the supported
    /// extensions.
    static const char	   *fileExtensionFilterString();
    
private:
    FONT_Registry();
    
    bool		 getInfoFromFontFile(const UT_StringHolder &font_path,
        		                     FONT_InfoPtrList &font_infos);

    void		 saveFontCache(UT_JSONWriter &j);
    bool		 loadFontCache(UT_JSONParser &p,
				       font_CacheMap &cached_fonts);
    static const FONT_Info *parseFontInfo(const UT_JSONValueMap *m, 
                                          const UT_StringHolder &font_path);
    static bool		 parseFontsAndPath(UT_JSONParser &p, 
               		                   font_CacheMap &cache);
    void		 writeFontsForPath(UT_JSONWriter &j, const char *path, 
        		                   const FONT_InfoPtrList &fl);    
    void		 init();
    bool		 createFontInfosFromPath(
				const UT_StringHolder &font_path);
    void		 initTypefacesAndFontAliases();

    void		 getCacheFilename(UT_String &filename) const;
    
    static bool	 	 parseType1Data(const FT_Face font_face, 
               		                FONT_Info &fi);
    static bool	 	 parseTrueTypeData(const FT_Face font_face, 
               		                   FONT_Info &fi);

    struct FontInfoPtrLess
    {
	bool operator()(const FONT_Info * const &a, const FONT_Info * const &b) const
	{
	    return *a < *b;
	}
    };
    
    typedef UT_SortedSet<const FONT_Info *, FontInfoPtrLess> FontSet;
    typedef UT_ArrayStringMap<const FONT_Info *> FontAliasMap;
    typedef UT_SortedStringMap<FONT_InfoPtrList> TypefaceMap;
    typedef UT_ArrayStringMap<FONT_InfoPtrList> PathFontMap;
    typedef UT_ArrayStringMap<UT_ArrayStringSet> FontOverrides;
    
    FontSet		 myFonts;
    FontAliasMap	 myFontAliases;
    TypefaceMap		 myTypefaceMap;
    FontOverrides	 myFontOverrides;
    UT_ArrayStringSet	 myBrokenFonts;
    FT_Library		 myFTLib;
};

#endif // __FONT_Registry__
