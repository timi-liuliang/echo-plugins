/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_PopGeometry_h__
#define __SIM_PopGeometry_h__

#include "SIMZ_API.h"
#include <POP/POP_ContextData.h>
#include <SIM/SIM_GeometryCopy.h>
#include "SIMZ_Utils.h"

class UT_IStream;
class SIM_Object;

// This SIM_Geometry implementation gets geometry from a POP.
class SIMZ_API SIM_PopGeometry : public SIM_GeometryCopy
{
public:
    GETSET_DATA_FUNCS_B(SIM_NAME_NEEDSRESET, NeedsReset);

    POP_ContextData		&getContextData();
    const POP_ContextData	&getContextData() const;

    void			 cacheGuides(const SIM_Data *owner);
    void			 cacheGuides(const SIM_Data *owner, const UT_Array<GU_DetailHandle> &guidelist);
    const UT_Array<GU_DetailHandle> *getGuideCache(const SIM_Data *owner) const;

    void			 resetGuides(SIM_Time time);

    virtual void		 setOwnGeometrySubclass(const GU_DetailHandle &gdh);

protected:
    explicit		 SIM_PopGeometry(const SIM_DataFactory *factory);
    virtual		~SIM_PopGeometry();

    virtual void	 initializeSubclass();
    virtual bool	 loadSubclass(UT_IStream &is);
    virtual int64	 getMemorySizeSubclass() const;
    virtual void	 makeEqualSubclass(const SIM_Data *source);

private:
    POP_ContextData	 myContextData;

    UT_Array< UT_Array<GU_DetailHandle> >	myGuideCache;
    UT_ValArray< const SIM_Data * > myGuideOwners;
    SIM_Time		 myGuideTime;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_PopGeometry,
			SIM_GeometryCopy,
			"POP Geometry",
			getEmptyDopDescription());
};

#endif

