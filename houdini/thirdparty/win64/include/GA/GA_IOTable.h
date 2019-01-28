/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IOTable.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_IOTable__
#define __GA_IOTable__

#include "GA_API.h"

#include <UT/UT_Array.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>

class GA_IO;
class GA_SaveOptions;
class GA_LoadOptions;
class UT_StringArray;


/// Container class to store GA_IO objects for a detail specialization
class GA_API GA_IOTable
{
public:
     GA_IOTable();
    ~GA_IOTable();

    /// Return the token for JSON
    static const char	*getJSONToken()
			    { return "hjson"; }
    /// Return the token for "classic" .bgeo/.geo formats
    static const char	*getClassicToken()
			    { return "hclassic"; }

    /// Append or replace an IO entry.
    /// IO entries are scanned in the order they are added.
    bool	 add(GA_IO *io);

    /// Find an IO entry by name.  With no name, the "default" IO class will be
    /// returned.
    const GA_IO	*find(const char *name=NULL) const;

    /// @{
    /// Find an IO entry by looking for the "geo:format" property
    const GA_IO	*find(const GA_SaveOptions *options) const;
    const GA_IO	*find(const GA_LoadOptions *options) const;
    /// @}

    /// @{
    /// Find the IO format which is the best match for the given filename
    const GA_IO	*checkFilename(const char *filename,
			const GA_SaveOptions *options) const;
    const GA_IO	*checkFilename(const char *filename,
			const GA_LoadOptions *options) const;
    /// @}

    /// Get a list of IO tokens supported
    void	 getNames(UT_StringArray &names) const;

    /// Return the number of GA_IO entries
    exint	 entries() const;

    /// Return the i'th GA_IO item
    const GA_IO	*get(exint i) const;

    /// Clear the entries
    void	 clear();

    /// Convenience operator
    const GA_IO	*operator()(exint i) const
		{
		    return get(i);
		}

    const UT_Array<GA_IO *>	&getList() const { return myList; }

private:
    UT_Array<GA_IO *> myList;
};

#endif
