/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ContainerAlt_h__
#define __SIM_ContainerAlt_h__

#include "SIM_API.h"
#include "SIM_Container.h"

/// This SIM_Data subclass is a simple container for other data. This is
/// the default data type used when creating subdata more than one level
/// deep inside another data.
class SIM_API SIM_ContainerAlt : public SIM_Container
{
protected:
    /// The constructor for when this data is created by our SIM_DataFactory.
    explicit		 SIM_ContainerAlt(const SIM_DataFactory *factory);
    virtual		~SIM_ContainerAlt();

    virtual bool	 getIsAlternateRepresentationSubclass() const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ContainerAlt,
			SIM_Container,
			"Data Container Alt",
			getEmptyDopDescription());
};

#endif

