/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentClip.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentClip__
#define __GU_AgentClip__

#include "GU_API.h"

#include "GU_AgentRig.h"

#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_PackedArrayOfArrays.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_SymMatrix3.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

class CL_Clip;
class GU_AgentTransformGroup;
class UT_BitArray;
class UT_StringArray;

/// A factored transform geared for animation blending.
template <typename T>
class GU_AgentXformT
{
public:
    typedef GU_AgentXformT<T> type;

    SYS_FORCE_INLINE
    GU_AgentXformT() = default;

    explicit GU_AgentXformT(T scale)
	: myStretch(scale)
	, myRotate(0, 0, 0, 0)
	, myTranslate(0, 0, 0)
    {
    }

    const UT_SymMatrix3T<T>&	stretch() const	    { return myStretch; }
    const UT_QuaternionT<T>&	rotate() const	    { return myRotate; }
    const UT_Vector3T<T>&	translate() const   { return myTranslate; }

    void zero()
    {
	myStretch.zero();
	myRotate.assign(0, 0, 0, 0);
	myTranslate.assign(0, 0, 0);
    }
    void identity()
    {
	myStretch.identity();
	myRotate.identity();
	myTranslate.assign(0, 0, 0);
    }

    /// Set the transform, in SRT/XYZ transform order.
    /// The rotations are given in radians.
    void setTransform(
	    T tx, T ty, T tz,
	    T rx, T ry, T rz,
	    T sx, T sy, T sz)
    {
	const UT_XformOrder xord(UT_XformOrder::SRT, UT_XformOrder::XYZ);
	myStretch.setScale(sx, sy, sz);
	myRotate.updateFromEuler(UT_Vector3T<T>(rx, ry, rz), xord);
	myTranslate.assign(tx, ty, tz);
    }

    /// Set the transform, in SRT transform order.
    /// The rotations are specified by a quaternion.
    void setTransform(
        const UT_Vector3T<T> &t, const UT_QuaternionT<T> &r,
        const UT_Vector3T<T> &s)
    {
	myStretch.setScale(s);
	myRotate = r;
	myTranslate = t;
    }

    template <typename S>
    void getTransform(
	    S& tx, S& ty, S& tz,
	    S& rx, S& ry, S& rz,
	    S& sx, S& sy, S& sz) const
    {
        tx = myTranslate[0];
        ty = myTranslate[1];
        tz = myTranslate[2];

	const UT_XformOrder xord(UT_XformOrder::SRT, UT_XformOrder::XYZ);
        UT_Vector3D r = myRotate.computeRotations(xord);
        rx = r[0];
        ry = r[1];
        rz = r[2];

        sx = myStretch(0, 0);
        sy = myStretch(1, 1);
        sz = myStretch(2, 2);
    }

    /// Set this to a linear interpolation of the two given transforms:
    /// *this = a + t*(b - a)
    void setLerp(const type& a, const type& b, T t)
    {
	myStretch.lerp(a.myStretch, b.myStretch, t);

	if (dot(a.myRotate, b.myRotate) >= 0)
	    myRotate.lerp(a.myRotate, b.myRotate, t);
	else
	    myRotate.lerp(a.myRotate, -b.myRotate, t);
	// NOTE: We don't normalize myRotate since this is already handled when
	// we call UT_QuaternionT::getRotationMatrix().

	myTranslate = SYSlerp(a.myTranslate, b.myTranslate, t);
    }

    void addScaled(T s, const type& xform)
    {
	myStretch += xform.myStretch * s;
	if (dot(myRotate, xform.myRotate) >= 0)
	    myRotate += xform.myRotate * s;
	else
	    myRotate += xform.myRotate * -s;
	myTranslate += xform.myTranslate * s;
    }

    /// Perform an additive blend with the specified percentage.
    void concat(T t, const type& xform)
    {
        myRotate.normalize();

        UT_QuaternionT<T> additive_r = xform.myRotate;
        additive_r.normalize();
        UT_QuaternionT<T> new_r = additive_r * myRotate;

	if (dot(myRotate, new_r) < 0)
            new_r.negate();

        myRotate.lerp(myRotate, new_r, t);
    }

    void getMatrix4(UT_Matrix4T<T>& m) const
    {
	// Set m to the transform myStretch*myRotate*myTranslate
        if (!myStretch.isIdentity())
        {
            UT_Matrix3T<T> sr;
            myRotate.getRotationMatrix(sr);
            sr.leftMult(myStretch);
            m = sr;
        }
        else
            myRotate.getTransformMatrix(m);

	m.setTranslates(myTranslate);
    }

private:
    UT_SymMatrix3T<T>	myStretch;
    UT_QuaternionT<T>	myRotate;
    UT_Vector3T<T>	myTranslate;
};
typedef GU_AgentXformT<float>	GU_AgentXformF;
typedef GU_AgentXformT<double>	GU_AgentXformD;
typedef GU_AgentXformT<fpreal>	GU_AgentXformR;

// Declare as POD for UT_Array optimizations.
SYS_DECLARE_IS_POD(GU_AgentXformF)

class GU_AgentClip;
typedef UT_IntrusivePtr<GU_AgentClip>	GU_AgentClipPtr;
typedef UT_IntrusivePtr<const GU_AgentClip> GU_AgentClipConstPtr;
typedef UT_Array<GU_AgentClipConstPtr>	GU_AgentClipPtrArray;


/// An agent motion clip
///
/// It is represented as array of samples, where each sample has
/// transformsPerSample() number of transforms. These transforms can be "local"
/// (corresponding to the transform hierarchy of rig()), or "world" where they
/// are multiplied all the way up to a root node in the rig.
class GU_API GU_AgentClip : public UT_IntrusiveRefCounter<GU_AgentClip>
{
public:

    typedef fpreal32                                FloatType;
    typedef GU_AgentXformT<FloatType>		    Xform;
    typedef UT_Array<Xform>			    XformArray;
    typedef UT_Matrix4T<FloatType>		    Matrix4;
    typedef UT_Array<Matrix4>		            Matrix4Array;
    typedef UT_SharedPtr<Matrix4Array>		    Matrix4ArrayPtr;
    typedef UT_SharedPtr<const Matrix4Array>	    Matrix4ArrayConstPtr;

    /// Create an empty clip
    static GU_AgentClipPtr	addClip(
				    const char* name,
                                    const GU_AgentRigConstPtr &rig);

    /// Create a clip given a CHOP clip file
    static GU_AgentClipPtr	addClipFromFile(
				    const char* name,
				    const char* filename,
				    const GU_AgentRigConstPtr& rig,
				    UT_StringArray& errors);

    /// Compute the world transforms 'matrices' from the local 'xforms'
    /// corresponding to the transforms in 'rig'.
    static void			computeWorldTransforms(
				    const GU_AgentRig& rig,
				    const XformArray& xforms,
				    Matrix4Array& matrices);

    /// Compute the local transforms 'matrices' from the local 'xforms'
    /// corresponding to the transforms in 'rig'.
    static void			computeLocalTransforms(
				    const GU_AgentRig& rig,
				    const XformArray& xforms,
				    Matrix4Array& matrices);

    /// Given the local transforms 'xforms' corresponding to 'rig', convert
    /// them to world transforms in-place.
    /// The optional 'in_world_space' array can be used to specify if some
    /// transforms in 'xforms' are already in world space.
    static void			computeWorldTransforms(
				    const GU_AgentRig& rig,
                                    const UT_BitArray *in_world_space,
				    Matrix4Array& xforms);

    /// Given the world transforms 'xforms' corresponding to 'rig', convert
    /// them to local transforms in-place.
    /// The optional 'in_world_space' array can be used to specify if only some
    /// transforms in 'xforms' are currently in world space.
    static void			computeLocalTransforms(
				    const GU_AgentRig& rig,
                                    const UT_BitArray *in_world_space,
				    Matrix4Array& xforms);

    /// Replaces whitespace, special characters, etc with underscores.
    static void                 forceValidName(UT_String &clipname)
                                    { clipname.forceValidVariableName(".-"); }

private:
				GU_AgentClip(const char* name,
                                             const char *filename,
                                             const GU_AgentRigConstPtr &rig)
				    : myName(name)
                                    , myFileName(filename)
                                    , myIsFile(filename != NULL)
				    , myNumXformTracks(0)
                                    , myRig(rig)
				    , myStart(0)
				    , mySampleRate(24.0)
				 {
				 }

public:

				~GU_AgentClip();

    int64                       getMemoryUsage(bool inclusive) const;

    /// Initialize this clip to a 2D array of identity transforms given by
    /// 'num_transform_samples' columns each of length
    /// 'num_transforms_per_sample'.
    /// @pre num_transform_samples must be larger than 0.
    /// @pre num_transforms_per_sample must be larger than 0.
    void			init(exint num_transform_samples,
				     exint num_transforms_per_sample);

    /// Load from clip.
    void                        load(const CL_Clip &clip);

    /// Copy from another agent clip, and inherit the external reference if
    /// there is one. If the rigs are different, identity matrices will be used
    /// for the additional joints.
    void                        load(const GU_AgentClip &clip);

    /// Return the number of transform tracks of the clip that was loaded into
    /// this clip.
    // This is currently only used for consistency checks.
    int				numTransformTracks() const
                                    { return myNumXformTracks; }

    /// Save to clip
    void                        save(CL_Clip &clip,
                                     bool worldspace = false) const;

    const UT_StringHolder&	name() const
				    { return myName; }

    bool                        isFile() const
                                    { return myIsFile; }
    void                        clearIsFile();

    const UT_StringHolder&      fileName() const
                                    { UT_ASSERT(isFile()); return myFileName; }

    const GU_AgentRig&          rig() const
                                    { return *myRig; }

    /// Number of transforms in each sample
    exint			transformsPerSample() const
				{
				    if (myLocalTransforms.entries() < 1)
					return 0;
				    return myLocalTransforms(0).entries();
				}

    /// Number of samples in this clip
    exint			sampleCount() const
				{
				    return myLocalTransforms.entries();
				}

    /// Start time of this clip in seconds
    /// @{
    fpreal			start() const
				    { return myStart; }
    void                        setStart(fpreal t)
                                    { myStart = t; }
    /// @}

    /// Length of the clip, in seconds.
    fpreal                      length() const
				{
				    return secondsFromSample(sampleCount());
				}

    /// Sample rate of this clip
    /// @{
    fpreal			sampleRate() const { return mySampleRate; }
    void                        setSampleRate(fpreal sample_rate)
                                    { mySampleRate = sample_rate; }
    /// @}

    /// Directly set the local transforms for this clip (as an alternative to
    /// using GU_AgentClip::load()).
    ///
    /// @param samples An array of local transform samples 
    ///
    /// @pre samples (and each sample within) must be at least length 1
    /// @pre Each entry in samples must be the same length
    /// @post sampleCount() == samples.entries()
    /// @post transformsPerSample() == samples.last().entries()
    void			setLocalTransforms(
				    const UT_Array<XformArray> &samples);

    const XformArray&		localTransforms(exint sample_i) const
				    { return myLocalTransforms(sample_i); }
    Matrix4ArrayConstPtr	worldTransforms(exint sample_i) const
                                    { return myWorldTransforms(sample_i); }

    /// Samples a transform in the clip at a specific time.
    void                        sampleTransform(exint i, fpreal seconds,
                                                bool worldspace,
                                                Matrix4 &xform_sample) const;

    /// Add an extra channel into the clip (as an alternative to
    /// GU_AgentClip::load()).
    ///
    /// @param name Unique name of the channel.
    /// @param samples Float array with length sampleCount().
    void                        addChannel(const UT_StringHolder &name,
                                           const FloatType *samples);

    /// Finds a channel by name. Returns -1 if the channel does not exist.
    exint                       findChannel(const UT_StringRef &name) const;
    /// Samples an arbitrary channel in the clip at a specific time.
    /// For transforms, use GU_AgentClip::sampleTransform().
    FloatType                   sampleChannel(exint channel_index,
                                              fpreal seconds) const;
    /// Returns the names of all additional channels in the clip.
    const UT_StringArray       &channelNames() const { return myTrackNames; }
    exint                       channelCount() const
                                    { return myTrackNames.entries(); }

    /// Add a scaled version of our local transforms using the given clip time.
    /// If the time in seconds exceeds the length of our clip, then it is
    /// wrapped around.
    void			addScaledLocalTransforms(
				    XformArray& dst,
				    fpreal scale,
				    fpreal seconds) const;
    /// Variant of addScaledLocalTransforms that only affects a subset of the
    /// joints.
    ///
    /// @param inv_total_weight Inverse of the total blend weight at each joint.
    void			addScaledLocalTransforms(
				    XformArray &dst,
                                    const UT_BitArray &joints,
				    fpreal scale,
                                    const UT_Array<fpreal> &inv_total_weights,
				    fpreal seconds) const;
    /// Perform an additive blend with the specified percentage.
    void			concatLocalTransforms(
				    XformArray &dst,
                                    const GU_AgentTransformGroup &xform_grp,
				    fpreal percent,
				    fpreal seconds) const;
    void			addScaledLocalTransform(
				    Xform& xform,
                                    exint i,
				    fpreal scale,
				    fpreal seconds) const;

    inline fpreal		sampleFromSeconds(fpreal seconds) const
				{
				    return (seconds - myStart) * mySampleRate;
				}
    inline fpreal		secondsFromSample(fpreal sample) const
				{
				    return myStart + (sample / mySampleRate);
				}
    inline exint		nearestSampleFromSeconds(fpreal seconds) const
				{
				    return (exint)
					SYSrint(sampleFromSeconds(seconds));
				}
    inline fpreal		wrappedSampleFromSeconds(fpreal seconds) const
				{
				    fpreal len = sampleCount();
				    fpreal s = SYSfmod(
						sampleFromSeconds(seconds),
						len);
				    if (s >= 0)
					return s;
				    else
					return s + len;
				}

private:
    /// Given a sample time in seconds, find the two samples that we need to
    /// interpolate between.
    void                        getSamples(fpreal scale, fpreal seconds,
                                           exint& sample_a, fpreal& weight_a,
                                           exint& sample_b, fpreal& weight_b) const;

    /// Build myWorldTransforms from myLocalTransforms.
    void                        computeWorldSpaceSamples();

    UT_StringHolder		myName;
    UT_StringHolder		myFileName;
    bool                        myIsFile;
    int				myNumXformTracks; // put here for the same ABI 
    GU_AgentRigConstPtr         myRig;
    fpreal			myStart;
    fpreal			mySampleRate;

    /// The local transforms for the rig, at each sample.
    UT_Array<XformArray>	myLocalTransforms;
    /// The world transforms for the rig, at each sample.
    UT_Array<Matrix4ArrayPtr>	myWorldTransforms;

    /// Additional (non-tranform) tracks.
    UT_PackedArrayOfArrays<FloatType>   myTrackData;
    UT_StringArray                      myTrackNames;
    UT_StringMap<int>                   myTrackIndex;
};

SYS_FORCE_INLINE exint
GU_AgentClip::findChannel(const UT_StringRef &name) const
{
    auto it = myTrackIndex.find(name);
    return it != myTrackIndex.end() ? it->second : -1;
}

#endif
