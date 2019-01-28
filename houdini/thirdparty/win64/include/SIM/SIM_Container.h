/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Container_h__
#define __SIM_Container_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"

/// This SIM_Data subclass is a simple container for other data. This is
/// the default data type used when creating subdata more than one level
/// deep inside another data.
class SIM_API SIM_Container : public SIM_Data
{
protected:
    /// The constructor for when this data is created by our SIM_DataFactory.
    explicit		 SIM_Container(const SIM_DataFactory *factory);
    virtual		~SIM_Container();

private:
    static const SIM_DopDescription	*getContainerDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_Container,
			SIM_Data,
			"Data Container",
			getContainerDopDescription());
};

#endif

