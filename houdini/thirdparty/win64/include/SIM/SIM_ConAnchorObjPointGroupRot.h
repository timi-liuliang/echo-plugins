/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ConAnchorObjPointGroupRot.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_ConAnchorObjPointGroupRot__
#define __SIM_ConAnchorObjPointGroupRot__

#include "SIM_ConAnchorObjPointRot.h"
#include "SIM_DopDescription.h"
#include "SIM_PRMShared.h"

class SIM_API SIM_ConAnchorObjPointGroupRot : public SIM_ConAnchorObjPointRot
{
public:
    /// The point group that we are anchored to.
    GETSET_DATA_FUNCS_S(SIM_NAME_POINTGROUP, PointGroup);

    GETSET_DATA_FUNCS_S(SIM_NAME_ROTATIONATTRIBNAME, RotationAttribName);
    GETSET_DATA_FUNCS_S(SIM_NAME_CONDOFATTRIBNAME, DOFAttribName);
    GETSET_DATA_FUNCS_S(SIM_NAME_CONDIRATTRIBNAME, DOFVecAttribName);

    /// Resets the current point to the first in the group.
    void		reset() const;

    /// Moves the current point to the next one.
    /// This will not move beyond the last point in the group.
    void		nextPoint() const;

    /// Whether we have already reached the last point in the group.
    bool		atEnd() const;

protected:
    explicit		SIM_ConAnchorObjPointGroupRot(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointGroupRot();

    virtual int		getPointSubclass() const;
    virtual void	setPointSubclass(int ptnum);

    virtual bool	getNeedsInitSubclass() const;
    virtual void	initAnchorSubclass(const SIM_Object *object);

    virtual void	buildAnchorGuideGeometrySubclass(
						const SIM_Options &options,
						const GU_DetailHandle &gdh,
						const SIM_Relationship &rel,
						const SIM_Time &time) const;
    

    virtual const UT_Vector3	getRotationSubclass() const;
    virtual SIM_DOFType		getDOFTypeSubclass() const;
    virtual const UT_Vector3	getDOFVectorSubclass() const;

private:
    UT_IntArray		myPoints;
    mutable int		myPointsIdx;
    mutable bool	myAtEnd;

    void		cachePoints();

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjPointGroupRot,
			SIM_ConAnchorObjPointRot,
			"Anchor: Object Point Group Rotation",
			getDopDescription());
};

#endif

