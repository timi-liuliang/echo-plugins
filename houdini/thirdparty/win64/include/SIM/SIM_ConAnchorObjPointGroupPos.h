/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ConAnchorObjPointGroupPos.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_ConAnchorObjPointGroupPos__
#define __SIM_ConAnchorObjPointGroupPos__

#include "SIM_API.h"
#include "SIM_ConAnchorObjPointPos.h"

class SIM_API SIM_ConAnchorObjPointGroupPos : public SIM_ConAnchorObjPointPos
{
public:
    /// Store the group of points we are anchored to.
    GETSET_DATA_FUNCS_S(SIM_NAME_POINTGROUP, PointGroup);

    GETSET_DATA_FUNCS_S(SIM_NAME_OFFSETATTRIBNAME, OffsetAttribName);
    GETSET_DATA_FUNCS_S(SIM_NAME_CONDOFATTRIBNAME, DOFAttribName);
    GETSET_DATA_FUNCS_S(SIM_NAME_CONDIRATTRIBNAME, DOFVecAttribName);

    /// Access the list of object points directly,
    /// (for example, when SIM_ConstraintIterator is not used)
    const UT_IntArray&  points() const;

    /// reset(), nextPoint() and atEnd() are used by SIM_ConstraintIterator
    /// SIM_ConstraintIterator makes the base class SIM_ConAnchorObjPointPos
    /// behave as a proxy for the individual points in the group

    /// Reset the current point to the first one in the group.
    void		reset() const;

    /// Move to the next point in the group.
    /// This will not move beyond the last point in the group.
    void		nextPoint() const;

    /// Whether we have already reached final point in the group.
    bool		atEnd() const;

protected:
    explicit		SIM_ConAnchorObjPointGroupPos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjPointGroupPos();

    virtual int		getPointSubclass() const;
    virtual void	setPointSubclass(int ptnum);

    virtual bool	getNeedsInitSubclass() const;
    virtual void	initAnchorSubclass(const SIM_Object *object);

    virtual void	buildAnchorGuideGeometrySubclass(
					const SIM_Options &options,
					const GU_DetailHandle &gdh,
					const SIM_Relationship &rel,
					const SIM_Time &t) const;

    virtual SIM_DOFType		getDOFTypeSubclass() const;
    virtual const UT_Vector3	getDOFVectorSubclass() const;
    virtual const UT_Vector3	getOffsetSubclass() const;

private:
    UT_IntArray		 myPoints;
    mutable int		 myPointsIdx;
    mutable bool	 myAtEnd;

    void 		cachePoints();

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjPointGroupPos,
			SIM_ConAnchorObjPointPos,
			"Anchor: Object Point Group Position",
			getDopDescription());
};

#endif

