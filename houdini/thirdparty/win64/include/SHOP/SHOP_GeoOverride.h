/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_GeoOverride.h (SHOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SHOP_GeoOverride__
#define __SHOP_GeoOverride__

#include "SHOP_API.h"
#include <UT/UT_Options.h>

class UT_WorkBuffer;
class UT_StringArray;

/// Class to help with managing material overrides for geometry attributes
class SHOP_API SHOP_GeoOverride
{
public:
     SHOP_GeoOverride();
    ~SHOP_GeoOverride();

    /// Number of entries
    exint	entries() const	{ return myOptions.getNumOptions(); }

    /// Clear object
    void	clear()		{ myOptions.clear(); }

    ///  Merge with another SHOP_GeoOverride object
    void	merge(const SHOP_GeoOverride &other)
		    { myOptions.merge(other.myOptions); }

    /// Load from the string (i.e. "{'diff_clrb':0.32,'map':'foo.pic'}")
    bool	load(const char *string);
    /// Save to a string (note that the order of the keys may be different.
    bool	save(UT_WorkBuffer &result) const;

    /// @{
    /// Add override options
    void	addKey(const char *key, exint value)
		    { myOptions.setOptionI(key, value); }
    void	addKey(const char *key, fpreal value)
		    { myOptions.setOptionF(key, value); }
    void	addKey(const char *key, const UT_StringHolder &value)
		    { myOptions.setOptionS(key, value); }
    /// @}

    /// Extract the keys
    void	getKeys(UT_StringArray &keys) const;

    /// Check to see whether the value associated with the key is a string or a
    /// number.
    bool	isString(const char *key) const;
    /// @{
    /// Extract options
    bool	import(const char *key, fpreal &value) const;
    bool	import(const char *key, exint &value) const;
    bool	import(const char *key, UT_WorkBuffer &buf) const;
    /// @}

private:
    // For now, we store the map in a UT_Options.  In the material_override
    // attribute, we have a single scalar (int, real or string) value for each
    // key.
    UT_Options	myOptions;
};

#endif
