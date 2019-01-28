/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_PythonConverter_h__
#define __HUSD_PythonConverter_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_Overrides.h"

class HUSD_API HUSD_PythonConverter
{
public:
			 HUSD_PythonConverter(HUSD_AutoAnyLock &lock);
			~HUSD_PythonConverter();

    void		*getEditableLayer() const;
    void		*getEditableOverridesLayer() const;
    void		*getActiveLayer() const;
    void		*getEditableStage() const;
    void		*getEditableOverridesStage() const;
    void		*getStage() const;
    void		*getSourceLayer(int layerindex) const;
    int			 getSourceLayerCount() const;

private:
    HUSD_AutoAnyLock	&myAnyLock;
};

#endif

