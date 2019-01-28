/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	KIN_Bone.h ( Kinematics Library, C++)
 *
 * COMMENTS:
 *		class for storing bone specification
 */

#ifndef __KIN_Bone__
#define __KIN_Bone__

#include "KIN_API.h"
#include "KIN_AngleConstraint.h"
#include <UT/UT_Matrix4.h>
#include <SYS/SYS_Types.h>
#include <string.h>

class KIN_API KIN_Bone
{
public:
			 KIN_Bone()
			 {
			     myRot[0] = myRot[1] = myRot[2] = 0.0;
			     myLength = 0.0;
			     myDampening = 0.0;
			     myExtraXform.identity();
			     myData = 0;
			 }
			~KIN_Bone()
			 {
			 }

    fpreal		 getRotate( int axis ) const	{ return myRot[axis]; }
    void		 getRotates( fpreal *r ) const
			 { memcpy( r, myRot, sizeof(myRot) ); }
    const UT_Matrix4R	&getExtraXform() const	{ return myExtraXform; }
    
    fpreal		 getLength()  const	{ return myLength; }
    fpreal		 getDampening() const	{ return myDampening; }
    const void		*getData() const	{ return myData; }
    fpreal		 constrain(int axis, fpreal angle, fpreal step) const
			 { return myConstraint.constrain(axis, angle, step); }
    
    void		 setRotate( int axis, fpreal r )
			 { myRot[axis] = r; }
    void		 setRotates( const fpreal *r )
			 { memcpy( myRot, r, sizeof(myRot) ); }
    void		 setExtraXform(const UT_Matrix4R &xform)
			 { myExtraXform = xform; }
    
    void		 setLength( fpreal l )	{ myLength = l; }
    void		 setDampening( fpreal d ){ myDampening = d; }
    void		 setData( const void *d ){ myData = d; }
    void		 setConstraint( fpreal rest[3], fpreal xrange[2],
				fpreal yrange[2], fpreal zrange[2],
				fpreal damp[3], fpreal roll[3] )
			 {
			     myConstraint.setConstraint(rest, xrange, yrange,
							zrange, damp, roll);
			 }

    int64		 getMmemoryUsage(bool inclusive) const
			 {
			     return inclusive ? sizeof(*this) : 0;
			 }

private:
    fpreal			 myRot[3];
    fpreal			 myLength;
    fpreal			 myDampening;
    UT_Matrix4R			 myExtraXform;
    const void			*myData;
    KIN_AngleConstraint		 myConstraint;
};

#endif

