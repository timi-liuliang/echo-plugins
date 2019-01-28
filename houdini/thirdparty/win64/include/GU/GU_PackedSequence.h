/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedSequence.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PackedSequence__
#define __GU_PackedSequence__

#include "GU_PackedImpl.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringArray.h>

class GA_PrimitiveFactory;
class UT_MemoryCounter;

/// The base class for a sequence of packed geometry.
class GU_API GU_PackedSequence : public GU_PackedImpl
{
public:
    enum WrapMode
    {
	WRAP_CYCLE,	// Cycle through disks
	WRAP_CLAMP,	// Clamp to range
	WRAP_STRICT,	// Strict range (no geometry outside range)
	WRAP_MIRROR,	// Bounce back and forth
    };

    class TimeSample
    {
    public:
	exint	conditionalIndex() const { return myT < .5 ? myI0 : myI1; }
	void	singleIndex(exint i)
		{
		    myI0 = myI1 = i;
		    myT = 0;
		}

	// Quantize time into @c nstep individual slices
	static fpreal	quantize(fpreal time, exint nsteps);

	exint	i0() const { return myI0; }
	exint	i1() const { return myI1; }
	// Fractional time
	fpreal	t() const { return myT; }
	// Quantized time sample
	fpreal	qt(exint nsteps) const { return myI0 + quantize(myT, nsteps); }


	exint	myI0;	// First sample index
	exint	myI1;	// Index of second sample
	fpreal	myT;	// Blending factor
    };

    // When generating shared instance keys for blended geometry, an
    // SingleItemKey is used to uniquely identify key geometry.  For example,
    // for a disk sequence, the string value could be the filename for the
    // file.
    // Currently, this stores a string, but may be extended in the future.
    class SingleItemKey
    {
    public:
	SingleItemKey()
	{
	}
	SingleItemKey(const UT_StringHolder &k)
	    : myString(k)
	{
	}
	bool			 operator==(const SingleItemKey &k) const
					{ return myString== k.myString; }
	SYS_HashType		 hash() const { return myString.hash(); }
	const UT_StringHolder	&string() const { return myString; }
	void			 setString(const UT_StringHolder &s)
					{ myString= s; }
    private:
	UT_StringHolder	myString;
    };
    /// Class to store keys for a blend key.  If two separate primitives
    /// generate identical keys, it's assumed they are equivalent.
    class InstanceKey
    {
    public:
	InstanceKey() {}
	InstanceKey(const SingleItemKey &k0, const SingleItemKey &k1, fpreal t)
	{
	    set(k0, k1, t);
	}
	const SingleItemKey	&key0() const	{ return myKey[0]; }
	const SingleItemKey	&key1() const	{ return myKey[1]; }
	void	 set(const SingleItemKey &k0, const SingleItemKey &k1, fpreal t)
	{
	    myKey[0] = k0;
	    myKey[1] = k1;
	    myT = t;
	}
	void		 setKey0(const SingleItemKey &k) { myKey[0] = k; }
	void		 setKey1(const SingleItemKey &k) { myKey[1] = k; }
	fpreal		 t() const	{ return myT; }
	void		 setT(fpreal t) { myT = t; }

	SYS_HashType	 hash() const
	{
	    SYS_HashType	h = myKey[0].hash();
	    SYShashCombine(h, myKey[1].hash());
	    SYShashCombine(h, myT);
	    return h;
	}
	bool		operator==(const InstanceKey &k) const
	{
	    return myT == k.myT
		&& myKey[0] == k.myKey[0]
		&& myKey[1] == k.myKey[1];
	}
    private:
	SingleItemKey	myKey[2];
	fpreal		myT;
    };

    GU_PackedSequence();
    GU_PackedSequence(const GU_PackedSequence &src)
	: GU_PackedImpl(src)
	, myIndex(src.myIndex)
	, myWrap(src.myWrap)
    {
    }
    virtual ~GU_PackedSequence();

    /// Subclass returns the number of pieces of geometry in the sequence
    virtual exint	sequenceSize() const = 0;

    exint intrinsicSequenceSize(const GU_PrimPacked *prim) const
    { return sequenceSize(); }

    /// Get a detail handle representing the interpolated sub-frame geometry.
    GU_ConstDetailHandle	getBlendedDetail(fpreal t) const;

    /// Method to get an identifying key for the item.  This is used to create
    /// shared instances where possible.  By default, the key will be a unique
    /// string.
    virtual InstanceKey	getInstanceKey(exint quantize_steps) const;

    /// Make a unique instance key for this primitive.  This can be used to
    /// prevent any possible sharing of 
    InstanceKey		getUniqueInstanceKey(exint quantize_steps) const;

    /// @{
    /// Implementation of API on GU_PackedImpl
    virtual bool	isValid() const;
    virtual bool	getBounds(UT_BoundingBox &box) const;
    virtual bool	getRenderingBounds(UT_BoundingBox &box) const;
    virtual void	getVelocityRange(UT_Vector3 &min, UT_Vector3 &max)const;
    virtual void	getWidthRange(fpreal &min, fpreal &max) const;
    virtual bool        unpack(GU_Detail &destgdp, const UT_Matrix4D *transform) const;
    virtual GU_ConstDetailHandle	getPackedDetail(
						GU_PackedContext *c = nullptr
					) const;
    /// @}

    /// @{
    /// Member data accessors for intrinsics
    fpreal		index() const { return myIndex; }
    exint		wrappedIndex() const
			{
			    TimeSample s;
			    bool valid = getTimeSample(s,myIndex);
			    return valid ? s.i0() : -1;
			}
    fpreal		intrinsicIndex(const GU_PrimPacked *prim) const { return index(); }
    UT_StringHolder	wrapModeLabel() const;
    UT_StringHolder	intrinsicWrapModeLabel(const GU_PrimPacked *prim) const
    { return wrapModeLabel(); }
    WrapMode		wrapMode() const { return myWrap; }
    void		setIndex(GU_PrimPacked *prim, fpreal f);
    void		setWrapMode(GU_PrimPacked *prim, WrapMode w);
    void		setWrapMode(GU_PrimPacked *prim, const UT_StringHolder &mode);
    /// @}

protected:
    /// Get time sample.  Returns false if the index is invalid.
    /// The time sample will be initialized with the two indices for blending
    /// as well as the blending amount.
    bool	getTimeSample(TimeSample &sample, fpreal t) const;
    bool	getTimeSample(TimeSample &sample) const
		    { return getTimeSample(sample, myIndex); }

    /// @{
    /// Methods to get information about individual items in the sequence
    /// Subclass method to query the bounding box for the given item
    virtual bool	sequenceBounds(exint index,
				UT_BoundingBox &box) const = 0;
    virtual void	sequenceVelocityRange(exint index,
				UT_Vector3 &min, UT_Vector3 &max) const = 0;
    virtual void	sequenceWidthRange(exint index,
				fpreal &min, fpreal &max) const = 0;
    /// @}

    /// Subclass method to get point geometry for the n'th item
    virtual GU_ConstDetailHandle	sequencePointCloud(exint index) const=0;

    /// Subclass to return the geometry for the n'th item
    virtual GU_ConstDetailHandle	sequenceGeometry(exint index) const = 0;

    // Store the index and wrap mode in the options
    void	storeData(UT_Options &options) const;

    /// @{
    /// update index/wrap from the options (returns true if data changed)
    bool	updateData(GU_PrimPacked *prim, const UT_JSONValueMap &options);
    bool	updateData(GU_PrimPacked *prim, const UT_Options &options);
    /// @}

private:
    fpreal		myIndex;
    WrapMode		myWrap;
};

#endif
