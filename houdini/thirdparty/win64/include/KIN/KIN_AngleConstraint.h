/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	KIN_AngleConstraint.h ( Kinematics Library, C++)
 *
 * COMMENTS:
 * 		Constraint to restrict angle for a given bone.
 */

#ifndef __KIN_AngleConstraint__
#define __KIN_AngleConstraint__

#include "KIN_API.h"
#include <SYS/SYS_Types.h>

class KIN_API KIN_AngleConstraint
{
public:
			 KIN_AngleConstraint();
    virtual		~KIN_AngleConstraint();

    void		 setConstraint( fpreal rest[3], fpreal xrange[2],
					fpreal yrange[2], fpreal zrange[2],
					fpreal damp[3], fpreal roll[3] );
    fpreal		 constrain( int axis, fpreal angle,
				    fpreal step ) const;

private:
    fpreal		 myRestAngles[3];
    fpreal		 myMinAngles[3], myMaxAngles[3];
    fpreal		 myMinDamp[3], myMaxDamp[3];
    fpreal		 myRolloff[3];
};

#endif
