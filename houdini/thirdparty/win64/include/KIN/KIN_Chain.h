/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	KIN_Chain.h ( Kinematics Library, C++)
 *
 * COMMENTS:
 *		This class holds the specification of a kinematic
 *		chain. The chain is specified as a string of the
 *		form: "length rx ry rz, ..., length rx ry rz;"
 */

#ifndef __KIN_Chain__
#define __KIN_Chain__

#include "KIN_API.h"
#include "KIN_Bone.h"
#include <UT/UT_Array.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector3Array.h>
#include <UT/UT_XformOrder.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>

class KIN_Bone;
class KIN_Solver;

enum KIN_ChangeType
{
    CHAIN_NUMBER_OF_BONES = 0,
    BONE_LENGTH,
    BONE_DAMPENING,
    BONE_ROTATE,
    BONE_DATA
};

// "curve" solver type for Follow Curve
class KIN_API KIN_CurveParm
{
public:
    bool				 myClosedFlag;
    bool				 myOrientToNormalsFlag;
    UT_Vector3R				 myTangent;
    UT_Vector3RArray			 myPoints;
    UT_Vector3RArray			 myNormals;
};

// NOT IMPLEMENTED! DO NOT USE!
class KIN_API KIN_SpineParm : public KIN_CurveParm
{
public:
    fpreal				 myTolerance; // tolerance on solution
};

// Common parameters for inverse kinematics related solvers
class KIN_API KIN_IKSolverParm
{
public:
    UT_Vector3R				 myEndAffectorPos;	
    UT_Vector3R				 myTwistAffectorPos;
    int					 myTwistAffectorFlag;
    fpreal				 myTwist;	
    fpreal				 myDampen;	
};

// "constraint" solver type for Inverse Kinematics (IK) with Constraints
class KIN_API KIN_ConstraintParm : public KIN_IKSolverParm
{
public:
    fpreal				 myTolerance; // tolerance on solution
};

// "inverse" solver type for Inverse Kinematics (IK)
class KIN_API KIN_InverseParm : public KIN_IKSolverParm
{
public:
    KIN_InverseParm()
    {
	myResistStraight = true;
	myTrackingThresholdFactor = 1e-03;
    }

    fpreal				 myTrackingThresholdFactor;
    int					 myResistStraight;
};

class KIN_API KIN_Chain
{
public:

			 KIN_Chain();
			 KIN_Chain(const KIN_Chain &copy_from);
			~KIN_Chain();

    static const UT_XformOrder &getXformOrder() { return myXformOrder; }

    // These functions are for getting and setting bone information.
    // All access to a chains bones must go through the chain so that we
    // can make sure our solver is up to date. The separate updateBone,
    // setBoneRotate, and setBoneRotates functions are only provided
    // for convenience. Also notice that the Chain creator no longer
    // has to actually create or remove any bones from the chain,
    // only to set the number of bones.
    int			 getNbones() const;
    void		 setNbones(int bones);
    const KIN_Bone	&getBone(int index) const
			    { return myBones(index); }
    void		 updateBone(int index, fpreal length, fpreal *rot,
				    fpreal damp, const UT_Matrix4R &xform,
				    const void *data);
    void		 setBoneRotate(int index, int axis, fpreal r);
    void		 setBoneRotates(int index, const fpreal *r);
    void		 getBoneRotates(int index, fpreal *r) const;

    // Currently we are limited to one constraint per bone, so the
    // interface to the bone constraints can be very simple. Any constraint
    // that gets passed in here will be deleted by the bone.
    void		 setConstraint( int index, fpreal rest[3],
					fpreal xrange[2], fpreal yrange[2],
					fpreal zrange[2], fpreal damp[3],
					fpreal roll[3] );
    fpreal		 constrain( int index, int axis,
				    fpreal angle, fpreal step ) const;

    // Utility functions for getting transform information for each bone
    // or for just the chain end position.
    void		 getEndPosition(UT_Vector3R &result) const;
    void		 getBoneStates(UT_Matrix4R *xforms) const;
    fpreal		 getLength() const;

    // Functions for setting solver information for the chain
    // The solver types are same tokens as found in the "Solver Type" parameter
    // of the InverseKin CHOP. See the comments for the KIN_*Parm classes above.
    int			 solve(const char *type, const void *parms,
			       KIN_Chain &solution);

    // Essentially a copy operator
    void		 copyFrom(const KIN_Chain &src);
    void		 copySubChain(const KIN_Chain &src, int from, int to);

    int64		 getMemoryUsage(bool inclusive) const
			 {
			     int64 mem = inclusive ? sizeof(*this) : 0;
			     mem += myBones.getMemoryUsage(false);
			     mem += mySolverType.getMemoryUsage(false);
			     return mem;
			 }

private:
    void		 initializeSolver();
    void		 chainChanged(KIN_ChangeType type);

    UT_Array<KIN_Bone>				 myBones;
    UT_UniquePtr<KIN_Solver>			 mySolver;
    UT_StringHolder				 mySolverType;
    const void					*mySolverParms;
    static const UT_XformOrder			 myXformOrder;
};

SYS_FORCE_INLINE
void
KIN_Chain::setBoneRotates(int index, const fpreal *r)
{
    auto &&bone = myBones(index);
    if( !SYSisEqual( r[0], bone.getRotate(0) ) ||
	!SYSisEqual( r[1], bone.getRotate(1) ) ||
	!SYSisEqual( r[2], bone.getRotate(2) ) )
	chainChanged(BONE_ROTATE);
    bone.setRotates(r);
}

SYS_FORCE_INLINE
void
KIN_Chain::getBoneRotates(int index, fpreal *r) const
{
    getBone(index).getRotates(r);
}

#endif

