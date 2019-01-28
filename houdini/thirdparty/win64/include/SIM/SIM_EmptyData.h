/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_EmptyData_h__
#define __SIM_EmptyData_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

/// This class holds a SIM_Options object, and provides direct access to
/// it. Most SIM_Data classes which use SIM_Options to hold basic data
/// types don't expose the contained data directly. This prevents
/// extraneous data from being attached where it doesn't belong. This
/// class has no such restrictions and so can be used as a repository
/// for arbitrary simple data types.
class SIM_API SIM_EmptyData : public SIM_Data,
			      public SIM_OptionsUser
{
public:
    /// Provides read access to our SIM_Options internal data.
    const SIM_Options	&getData() const;
    /// Provides read and write access to our SIM_Options internal data.
    SIM_Options		&getData();

    /// Signals to the data that it has been altered externally.
    void		 pubHandleModification()
			 {
			     handleModification();
			 }

protected:
    explicit		 SIM_EmptyData(const SIM_DataFactory *factory);
    virtual		~SIM_EmptyData();

private:
    static const SIM_DopDescription	*getEmptyDataDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_EmptyData,
			SIM_Data,
			"Empty Data",
			getEmptyDataDopDescription());
};

#endif

