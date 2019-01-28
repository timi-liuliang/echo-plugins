/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_PositionComposite_h__
#define __SIM_PositionComposite_h__

#include "SIM_API.h"
#include "SIM_Position.h"
#include "SIM_OptionsUser.h"

/// This is the simplest implementation for standard positional data.
/// Positional data is not exactly the same as a transform in that it
/// does not permit scales, and the rotation is always applied before
/// the translation.
class SIM_API SIM_PositionComposite : public SIM_Position,
				      public SIM_OptionsUser
{
public:
    /// Control the operation to use when compositing our position subdata.
    GETSET_DATA_FUNCS_S(SIM_NAME_POSCOMPOSITEOP, PosCompositeOp);

protected:
    explicit		 SIM_PositionComposite(const SIM_DataFactory *factory);
    virtual		~SIM_PositionComposite();

    /// Gets a transform matrix from our position and rotation data.
    virtual void	 getTransformSubclass(UT_DMatrix4 &xform) const;

private:
    static const SIM_DopDescription	*getPositionCompositeDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_PositionComposite,
			SIM_Position,
			"Position Composite",
			getPositionCompositeDopDescription());
};

#endif

