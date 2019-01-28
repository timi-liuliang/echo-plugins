/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Impacts_h__
#define __SIM_Impacts_h__

#include "SIM_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_Vector3Array.h>
#include "SIM_DataUtils.h"

class UT_IStream;
class SIM_ObjectArray;

/// This flag is set if the given impact is intended to represent friction
/// between two objects, rather than the direct collision response.
/// If set, the "normal" represents the tangential friction direction, and
/// the impulse is the friction impulse.
#define SIM_IMPACTS_FRICTION		0x00000001

/// This flag is set for SIM_Impacts entries that are repulsion forces
/// rather than inelastic collisions. Used by SIM_ColliderRepulsion.
#define SIM_IMPACTS_REPULSION		0x00000002

/// This flag is set for SIM_Impact entries that represent situations where
/// a point is already inside an SDF. Used by SIM_ColliderPoint.
#define SIM_IMPACTS_POINTINSIDE		0x00000004

/// This flag is set for cloth-volume collision.
#define SIM_IMPACTS_CLOTHVOLUME		0x00000008

/// This flag is set for cloth-cloth collision.
#define SIM_IMPACTS_CLOTHCLOTH		0x00000010

/// For particle collisions, particle should stop.
#define SIM_IMPACTS_PARTSTOP		0x00000020

/// For particle collisions, particle should die.
#define SIM_IMPACTS_PARTKILL		0x00000040

/// For particle collisions, particle should continue on course.
#define SIM_IMPACTS_PARTCONTINUE	0x00000080

/// For particle collisions, particle should stick.
#define SIM_IMPACTS_PARTSTICK		0x00000100

/// For particle collisions, particle should bounce.
#define SIM_IMPACTS_PARTBOUNCE		0x00000200

/// Combination of all the SIM_IMPACTS_PART* flags.
#define SIM_IMPACTS_PARTANY		(SIM_IMPACTS_PARTSTOP | \
					 SIM_IMPACTS_PARTKILL | \
					 SIM_IMPACTS_PARTCONTINUE | \
					 SIM_IMPACTS_PARTSTICK | \
					 SIM_IMPACTS_PARTBOUNCE)

/// Used by the cloth/volume collider to indicate that an impact will require
/// feedback to be resolved.
#define SIM_IMPACTS_FEEDBACK		0x00000400

/// Marks that this impact won't (and didn't) compute the primitive/uv
/// of the collision.  This can accelerate collisions with SDF based
/// geometry by points as no ray tests are then needed against the geometry.
#define SIM_IMPACTS_NOPRIMUV		0x00000800

/// Marks that this is a resting impact, ie, if the objects were ballistic,
/// this impact would not have occurred.  These sort of impacts tend to
/// be generated by gravity, thus the resting nomenclature.
#define SIM_IMPACTS_RESTING		0x00001000


/// This class defines a SIM_Data subclass for holding collision information.
/// Each contact is represented by a position in simulation space, a normal
/// to the collision surface, the ids of the objects involved in
/// the contact, and the strength of the impact impulse.
/// Optionally, a friction impulse can also be included with the impact.
///
/// Remember your basic physics and the definition of "impulse":
///   I = F * dt = m * dv
class SIM_API SIM_Impacts : public SIM_Data
{
public:
    /// Get the number of contacts.
    int			 getNumImpacts() const;
    /// Get the position for a particular contact.
    /// The position is stored in simulation space.
    const UT_Vector3	&getPosition(int index) const;
    /// Get the normal for a particular contact, pointing away from the first
    /// object. This value generally represents the normal of the surface at
    /// the point of contact. This value is used to determine the
    /// direction of the impact force at this contact point.
    const UT_Vector3	&getNormal(int index) const;
    /// Get the collision impulse.
    fpreal		 getImpulse(int index) const;
    /// Set the collision impulse.
    void		 setImpulse(int index, fpreal impulse);
    /// Remove the last impact
    void		 removeLastImpact();
    /// Get flags associated with the impact
    int			 getFlags(int index) const;
    /// Get the point number involved in the collision. This function may
    /// return -1 if the point number is unknown.
    GA_Index		 getPointNumber(int index) const;
    /// Get the primitive number involved in the collision. This function
    /// may return -1 if the primitive number is unknown.
    int			 getPrimitiveNumber(int index) const;
    /// Get the u coordinate of the primitive where the impact occurred.
    fpreal		 getPrimitiveU(int index) const;
    /// Get the v coordinate of the primitive where the impact occurred.
    fpreal		 getPrimitiveV(int index) const;
    /// Get the w coordinate of the primitive where the impact occurred.
    fpreal		 getPrimitiveW(int index) const;
    /// Get the unique id of the other object involved in a contact.
    int			 getOtherObjId(int index) const;
    /// Get the point number on the other object involved in the collision.
    /// This function may return -1 if the point number is unknown.
    int			 getOtherObjPointNumber(int index) const;
    /// Get the primitive number on the other object involved in the collision.
    /// This function may return -1 if the primitive number is unknown.
    int			 getOtherObjPrimitiveNumber(int index) const;
    /// Get the u coordinate of the primitive where the impact occurred
    /// on the other object.
    fpreal		 getOtherObjPrimitiveU(int index) const;
    /// Get the v coordinate of the primitive where the impact occurred
    /// on the other object.
    fpreal		 getOtherObjPrimitiveV(int index) const;
    /// Get the w coordinate of the primitive where the impact occurred
    /// on the other object.
    fpreal		 getOtherObjPrimitiveW(int index) const;
    /// Get the estimated time of the collision.
    SIM_Time		 getTime(int index) const;

    /// This function adds a new impact at a position in space.
    void		 addPositionImpact(const UT_Vector3 &pos,
					const UT_Vector3 &normal,
					fpreal impulse,
					int otherobjid,
					int otherobjptnum,
					int otherobjprimnum,
					fpreal otherobjprimu,
					fpreal otherobjprimv,
					fpreal otherobjprimw,
					const SIM_Time &time,
					int flags);
    /// This function adds a new impact at a position in space and associates
    /// it with a particular point.
    void		 addPointImpact(const UT_Vector3 &pos,
					const UT_Vector3 &normal,
					fpreal impulse,
					int ptnum,
					int otherobjid,
					int otherobjptnum,
					int otherobjprimnum,
					fpreal otherobjprimu,
					fpreal otherobjprimv,
					fpreal otherobjprimw,
					const SIM_Time &time,
					int flags);
    /// This function adds a new impact at a position in space and associates
    /// it with a particular primitive and UV coordinate.
    void		 addPrimitiveImpact(const UT_Vector3 &pos,
					const UT_Vector3 &normal,
					fpreal impulse,
					int primnum,
					fpreal primu,
					fpreal primv,
					fpreal primw,
					int otherobjid,
					int otherobjptnum,
					int otherobjprimnum,
					fpreal otherobjprimu,
					fpreal otherobjprimv,
					fpreal otherobjprimw,
					const SIM_Time &time,
					int flags);
    /// Merge the impacts from another SIM_Impacts data into this one.
    void		 mergeImpacts(const SIM_Impacts &src);

    /// Merge only the impacts in the given time range. This is useful for
    /// feedback and substepping: it allows merging of feedback impacts
    /// from the fullstep end object to a substep object.
    void                 mergeImpactsInTimeRange(const SIM_Impacts &src,
                                      const SIM_Time &startTime,
                                      const SIM_Time &endTime);

    /// This is a very special-purpose function used by SIM_Engine.
    /// It removes any impacts that are self-impacts or that involve
    /// other objects that are not in the supplied object array. It
    /// starts the search for removal at the supplied index. This is
    /// used to eliminate impact data during feedback iteration so that
    /// we don't end up multiply applying impacts on non-feedback objects.
    void		 keepOnlyImpactsForAffectors(
					const SIM_ObjectArray &affectors,
					int thisobjectid,
					int startindex);

    /// Calculates the impulses that should be applied on objects a and b.
    /// The resulting velocity of the specified points on the two objects
    /// after applying the impulse should be (-relvel * bounce). Either
    /// or both of obja and objb can be null in which case the mass of
    /// those objects is considered to be infinite. The ptnum values can
    /// be -1 if the world space position should be used to fetch the
    /// impulse mass matrix. The dynamicfrictionmultiplier is multiplied
    /// by the friction value to get the dynamic friction value. The
    /// returned impulse should be applied equally to both objects. Note
    /// that the returned impulse may have a normal different than nml.
    /// The normal of the returned impulse should be used, as the impact
    /// normal, not the original nml. Otherwise the impulse magnitude will
    /// be wrong. Also, the output normal properly accounts for friction.
    /// However, the returned normal direction will be in the same hemisphere
    /// as the passed in normal, so it can be used directly as the impact
    /// normal for object a (the passed in normal is the normal on objb). 
    static UT_Vector3	 getRequiredImpulse(const UT_Vector3 &worldspacepos,
				const UT_Vector3 &nml,
				const SIM_Object *obja,
				GA_Index ptnuma,
				bool objainfinitemass,
				const SIM_Object *objb,
				GA_Index ptnumb,
				bool objbinfinitemass,
				fpreal bounce,
				fpreal friction,
				fpreal dynamicfrictionmultiplier,
				fpreal bounceforward,
				bool usesdfvelocitya = false,
				bool usepointvelocitya = true,
				bool usesdfvelocityb = false,
				bool usepointvelocityb = true);
    // Same as above but the caller can explicitly supply Ka and vela.
    static UT_Vector3	 getRequiredImpulse(const UT_DMatrix3 &Ka,
				const UT_Vector3 &vela,
				const UT_Vector3 &worldspacepos,
				const UT_Vector3 &nml,
				const SIM_Object *objb,
				GA_Index ptnumb,
				bool objbinfinitemass,
				fpreal bounce,
				fpreal friction,
				fpreal dynamicfrictionmultiplier,
				fpreal bounceforward,
				bool usesdfvelocityb = false,
				bool usepointvelocityb = true);
    // Same as above, but the caller can explicitly supplict Kb and velb,
    // and supplies a known desired relative velocity (normal only).
    static UT_Vector3	 getRequiredImpulse(const UT_Vector3 &desiredrelvel,
				const UT_DMatrix3 &Ka,
				const UT_Vector3 &vela,
				const UT_DMatrix3 &Kb,
				const UT_Vector3 &velb,
				const UT_Vector3 &worldspacepos,
				const UT_Vector3 &nml,
				fpreal friction,
				fpreal dynamicfrictionmultiplier,
				fpreal bounceforward);

    /// Split an impulse into normal and tangential (friction) components.
    static void		 splitImpulse(const UT_Vector3 &impulse,
				      const UT_Vector3 &normal,
				      fpreal &normalImpulseLength,
				      UT_Vector3 &tangentImpulse);

protected:
    /// The SIM_Impacts constructor.
    explicit		 SIM_Impacts(const SIM_DataFactory *factory);
    /// The SIM_Impacts destructor.
    virtual		~SIM_Impacts();

    /// Clears out all contact information.
    virtual void	 initializeSubclass();
    /// Copies the contact information from another SIM_Impacts.
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    /// Saves the contact information to a stream.
    virtual void	 saveSubclass(std::ostream &os) const;
    /// Loads contact information from a stream.
    virtual bool	 loadSubclass(UT_IStream &is);
    /// Returns the total memory used by this object.
    virtual int64	 getMemorySizeSubclass() const;
    /// Creates a SIM_QueryArrays object to treat impact as a record
    virtual SIM_Query	*createQueryObjectSubclass() const;

    /// Helper method for the loadSubclass method to resize all of our
    /// member variable arrays.
    void		 resizeArrays(int numimpacts);

    /// Remove a impact!  O(n) operation.  This is protected  
    /// because the use of this method is discouraged.
    void		 removeImpact(int index);
private:
    UT_Vector3Array	 myPositions;
    UT_Vector3Array	 myNormals;
    UT_FloatArray	 myImpulses;
    UT_IntArray		 myFlags;
    UT_IntArray		 myPtNums;
    UT_IntArray		 myPrimNums;
    UT_FloatArray	 myPrimUs;
    UT_FloatArray	 myPrimVs;
    UT_FloatArray	 myPrimWs;
    UT_IntArray		 myOtherObjIds;
    UT_IntArray		 myOtherObjPtNums;
    UT_IntArray		 myOtherObjPrimNums;
    UT_FloatArray	 myOtherObjPrimUs;
    UT_FloatArray	 myOtherObjPrimVs;
    UT_FloatArray	 myOtherObjPrimWs;
    UT_FloatArray	 myTimes;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_Impacts,
			SIM_Data,
			"Impact Information",
			getEmptyDopDescription());
};

#endif
