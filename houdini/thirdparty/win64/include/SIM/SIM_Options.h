/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_Options.h (C++, Utility Library)
 *
 * COMMENTS:	Subclass of UT_Options to provide some special functionality
 * 		used by the SIM library.
 *
 */

#ifndef _SIM_OPTIONS_H_
#define _SIM_OPTIONS_H_

#include "SIM_API.h"
#include <GA/GA_Types.h>
#include <OP/OP_Options.h>

class GA_Attribute;
class SIM_OptionsUser;

/// This class stores a set of name/value pairs. Each value can be one of
/// several different data types. No automatic conversion between data types
/// is provided, so accessing existing data requires knowledge of both the
/// data name and the data type.
class SIM_API SIM_Options : public OP_Options
{
public:
    explicit		 SIM_Options();
    explicit		 SIM_Options(SIM_OptionsUser *owner);
    virtual		~SIM_Options();

    /// Sets an option based on a geometric point and an attribute.
    /// This utility function relieves the programmer from worrying
    /// about the data type of the attribute and mapping it to the
    /// matching SIM_Options data type.
    void		 setOptionFromPointAttribute(GA_Offset ptoff,
					const GA_Attribute *attrib);

    const SIM_Options	&operator=(const SIM_Options &src);

protected:
    /// This function is called whenever data is added, removed, or changed.
    /// If the name parameter is null, then multiple pieces of data were
    /// changed simultaneously (probably through a load or assignment
    /// operation). This function just calls SIM_OptionsUser::optionChanged()
    /// on each user of this data.
    virtual void	 optionChanged(const char *name);

private:
    explicit		 SIM_Options(const SIM_Options &src);

    SIM_OptionsUser	*myOwner;
};

#endif

