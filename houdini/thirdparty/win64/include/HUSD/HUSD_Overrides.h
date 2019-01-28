/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Overrides_h__
#define __HUSD_Overrides_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_UniquePtr.h>
#include <iosfwd>

#define HUSD_OVERRIDE_LOPNET_LAYER 0

class HUSD_TimeCode;

class HUSD_API HUSD_Overrides : public UT_IntrusiveRefCounter<HUSD_Overrides>,
				public UT_NonCopyable
{
public:
				 HUSD_Overrides();
				~HUSD_Overrides();

    void			 save(std::string &str) const;
    void			 saveToFile(const UT_StringHolder &path) const;
    void			 loadFromFile(const UT_StringHolder &path);
    bool			 isEmpty() const;

    void			 clear(int layer_idx);
    bool			 setDrawMode(int layer_idx,
					HUSD_AutoAnyLock &lock,
					const HUSD_FindPrims &prims,
					const UT_StringRef &drawmode);
    bool			 setActive(int layer_idx,
					HUSD_AutoAnyLock &lock,
					const HUSD_FindPrims &prims,
					bool active);
    bool			 setVisible(int layer_idx,
					HUSD_AutoAnyLock &lock,
					const HUSD_FindPrims &prims,
					const HUSD_TimeCode &timecode,
					bool visible);

    int				 layerPtrCount() const;
    const PXR_NS::XUSD_LayerPtr	&layerPtr(int idx) const;

private:
    class husd_OverridesPrivate;

    UT_UniquePtr<husd_OverridesPrivate>	 myPrivate;
};

#endif

