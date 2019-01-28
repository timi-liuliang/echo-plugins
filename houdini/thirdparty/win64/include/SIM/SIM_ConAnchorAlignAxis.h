/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorAlignAxis_h__
#define __SIM_ConAnchorAlignAxis_h__

#include "SIM_API.h"
#include "SIM_ConAnchorRotational.h"

class SIM_ConAnchorSpatial;

/// A concrete constraint source: the "lookat" constraint, set to
/// track the orientation from one spatial anchor to another.
class SIM_API SIM_ConAnchorAlignAxis : public SIM_ConAnchorRotational
{
public:
    GETSET_DATA_FUNCS_V3(SIM_NAME_OBJECTAXIS, ObjectAxis);

protected:
    explicit		 SIM_ConAnchorAlignAxis(const SIM_DataFactory *factory);
    virtual		~SIM_ConAnchorAlignAxis();

    virtual bool	 getNeedsInitSubclass() const;
    virtual void	 initConstraintSubclass(const SIM_Relationship *rel);

    virtual UT_Quaternion	 getOrientationSubclass(
					const SIM_Time &time) const;
    virtual UT_Vector3		 getAngularVelocitySubclass(
					const SIM_Time &time) const;
    
private:
    void		 getPositions(
				SIM_ConAnchorSpatial *&goalfrom,
				SIM_ConAnchorSpatial *&goalto);
    void		 getConstPositions(
				const SIM_ConAnchorSpatial *&goalfrom,
				const SIM_ConAnchorSpatial *&goalto) const;

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorAlignAxis,
			SIM_ConAnchorRotational,
			"Anchor: Align Axis",
			getDopDescription());
};

#endif

