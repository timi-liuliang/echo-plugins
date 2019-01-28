/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RootData_h__
#define __SIM_RootData_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"

class SIM_RootDataId;

class SIM_API SIM_RootData : public SIM_Data
{
public:
    /// Returns the name of the root data. The method for setting the name
    /// is provided in the subclasses because the rules for when and how
    /// this name can be set depends on the subclass type.
    const UT_String	&getName() const;

    /// Determines if this root data is a match for the pattern string
    /// passed in. The string must be a single token, not a series of
    /// tokens, but wildcards are allowed. This function just calls
    /// getMatchesStringSubclass().
    bool		 getMatchesString(const char *pattern) const;

    /// Fills out the root data identifier by calling getRootDataIdSubclass().
    void		 getRootDataId(SIM_RootDataId &id) const;

    /// Returns the full path (including the path to this root data)
    /// for the specified subdata.
    void		 appendFullDataPath(const SIM_Data *subdata,
					    bool useobjectid,
					    UT_String &datapath) const;

    /// Forces the value of the passed in string to contain a value that
    /// represents a valid root data name. Currently valid root data names
    /// are the same as valid variable names.
    static void		 forceValidRootDataName(UT_String &rootdataname);

protected:
			 SIM_RootData(const SIM_DataFactory *factory);
    virtual		~SIM_RootData();

    /// This function returns whether this data matches the string pattern.
    /// This function must be overridden by subclasses.
    virtual bool	 getMatchesStringSubclass(const char *pattern) const=0;
    /// This function fills out the SIM_RootDataId structure with the
    /// unique identifier information for this root data.
    virtual void	 getRootDataIdSubclass(SIM_RootDataId &id) const=0;

    UT_String		 myName;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_RootData, SIM_Data);
};

#endif

