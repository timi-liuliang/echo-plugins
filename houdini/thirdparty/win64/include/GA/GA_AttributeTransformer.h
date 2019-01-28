/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeTransformer.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_Transform__
#define __GA_Transform__

#include "GA_API.h"

#include "GA_Handle.h"
#include "GA_PageHandle.h"
#include "GA_Range.h"                   // for GA_Range
#include "GA_Types.h"                   // for GA_Offset, etc

#include <UT/UT_Array.h>                // for UT_Array
#include <UT/UT_Matrix3.h>              // for UT_Matrix3T
#include <UT/UT_Matrix4.h>
#include <UT/UT_Quaternion.h>

#include <SYS/SYS_Types.h>              // for fpreal32, fpreal64, exint


class GA_AIFMath;
class GA_AIFTuple;
class GA_Attribute;
class GA_AttributeFilter;
class GA_Detail;


/// @brief Class to efficiently transform multiple attributes
///
/// This class will transform attributes as efficiently as possible, depending
/// on the attribute's properties.  The transformer works on a single class of
/// attributes (i.e. GA_ATTRIB_POINT).
class GA_API GA_AttributeTransformer
{
public:
    /// @brief Class to pass transforms to GA_AttributeTransformer
    ///
    /// Computing inverse/quaternion transforms can be expensive.  If possible,
    /// you should reuse unchanging Transform objects rather than re-building
    /// them when performing multiple transforms.
    template <typename FLOAT_T>
    class Transform
    {
    public:
	typedef		FLOAT_T ValueType;
	Transform()
	    : myM()
	    , myHasI(false)
	    , myHasQ(false)
	{
	}
        template<typename FLOAT_T2>
	Transform(const UT_Matrix4T<FLOAT_T2> &m)
	    : myM(m)
	    , myHasI(false)
	    , myHasQ(false)
	{
	}
	/// Set the matrix
        template<typename FLOAT_T2>
	void	set(const UT_Matrix4T<FLOAT_T2> &m)
		{
		    myM = m;
		    myHasI = false;
		    myHasQ = false;
		}
	/// Compute the inverse (if required)
	void	computeInverse()
		{
		    if (!myHasI)
		    {
			UT_Matrix3T<FLOAT_T>	m3(myM);
			myM.invert(myIM);
			if (m3.determinant() < 0)
			    myIM.scale(-1, -1, -1);
			myHasI = true;
		    }
		}
	/// Compute the quaternion transform (if required)
	void	computeQuaternion()
		{
		    if (!myHasQ)
		    {
			UT_Matrix3T<FLOAT_T> m3(myM);
			myQ.updateFromArbitraryMatrix(m3);
			myHasQ = true;
		    }
		}

	/// @{
	/// Accessor
	const UT_Matrix4T<FLOAT_T>	&xform() const	{ return myM; }
	const UT_Matrix4T<FLOAT_T>	&ixform() const	{ return myIM; }
	const UT_QuaternionT<FLOAT_T>	&q() const	{ return myQ; }
	/// @}

    private:
	UT_Matrix4T<FLOAT_T>	myM;
	UT_Matrix4T<FLOAT_T>	myIM;
	UT_QuaternionT<FLOAT_T>	myQ;
	bool			myHasI;
	bool			myHasQ;
    };

    /// @brief Handle to a transformable attribute
    class GA_API Handle
    {
    public:
	/// @{
	/// Handle callback functions.  These perform different styles of
	/// transformation depending on the attribute type properties.
	typedef void (*GAtransformPageF)(const Handle &handle,
			    const Transform<fpreal32> &context,
			    GA_Offset start, GA_Offset end);
	typedef void (*GAtransformPageD)(const Handle &handle,
			    const Transform<fpreal64> &context,
			    GA_Offset start, GA_Offset end);
	typedef void (*GAtransformF)(const Handle &handle,
			    const Transform<fpreal32> &context,
			    GA_Offset offset);
	typedef void (*GAtransformD)(const Handle &handle,
			    const Transform<fpreal64> &context,
			    GA_Offset offset);
	/// @}

	/// Construct a handle for a given attribute
	Handle(GA_Attribute *attrib, bool keep_length = false);
	~Handle() { }

	/// @{
	/// Transform the attribute using the handle callback
	void	transform(const Transform<fpreal32> &ctx,
			GA_Offset start, GA_Offset end) const
		    { myTransformPageF(*this, ctx, start, end); }
	void	transform(const Transform<fpreal64> &ctx,
			GA_Offset start, GA_Offset end) const
		    { myTransformPageD(*this, ctx, start, end); }
	void	transform(const Transform<fpreal32> &ctx,
			GA_Offset offset) const
		    { myTransformF(*this, ctx, offset); }
	void	transform(const Transform<fpreal64> &ctx,
			GA_Offset offset) const
		    { myTransformD(*this, ctx, offset); }
	/// @}

	/// Check to see whether the attribute is valid
	bool		isValid() const { return myType != GA_TYPE_VOID; }
	/// Return the GA_TypeInfo for the attribute
	GA_TypeInfo	getType() const { return myType; }

	/// @{
	/// Check whether length of vectors/normals should be preserved after
	/// transforming.
	bool		keepLength() const { return myKeepLength; }
	void		setKeepLength(bool v) { myKeepLength = v; }
	/// @}

	/// @{
	/// Accessors to provide information in the callback handle functions
        /// NOTE: Always check isDouble() before calling getV3!
	GA_Attribute	*getAttribute() const	{ return myAttribute; }
        template<typename FLOAT_T>
	const GA_RWHandleT<UT_Vector3T<FLOAT_T> > &getV3() const
        {
            UT_ASSERT_COMPILETIME(sizeof(GA_RWHandleV3) == sizeof(GA_RWHandleV3D));
            UT_ASSERT_P(myIsDouble == (sizeof(FLOAT_T) == 8));
            UT_ASSERT_P(!myIsDouble == (sizeof(FLOAT_T) == 4));
            return (const GA_RWHandleT<UT_Vector3T<FLOAT_T> > &)myV3;
        }
	const GA_AIFMath	*getMath() const	{ return myMath; }
	const GA_AIFTuple	*getTuple() const	{ return myTuple; }
	/// @}

	/// @{
	/// Check whether the inverse matrix or quaternion transform are
	/// required
	bool	needInverse() const	{ return myNeedI; }
	bool	needQuaternion() const	{ return myNeedQ; }
	/// @}

        /// Check if the attribute is double-precision, if can choose
        /// between single and double-precision transformation.
        bool isDouble() const { return myIsDouble; }

    private:
	/// Set up the transform callback functions
	void	setupFunctions();

	GAtransformF		 myTransformF;
	GAtransformD		 myTransformD;
	GAtransformPageF	 myTransformPageF;
	GAtransformPageD	 myTransformPageD;
	GA_Attribute		*myAttribute;
	const GA_AIFMath	*myMath;
	const GA_AIFTuple	*myTuple;
        GA_RWHandleV3		 myV3;
	GA_TypeInfo		 myType;
	bool			 myKeepLength;
	bool			 myNeedI;
	bool			 myNeedQ;
        bool                     myIsDouble;
    };

public:
     GA_AttributeTransformer(GA_Detail &gdp, GA_AttributeOwner owner);
    ~GA_AttributeTransformer();

    /// Add all transformable attributes to the handle list.  When @c
    /// keep_length is set, normals and vectors will maintain their lengths
    /// under scaling.
    void	addAttributes(bool keep_lengths=false);
    /// Add a filtered selection of attributes to the handle list.  The filter
    /// should filter out non-transforming attributes.  When @c keep_length is
    /// set, normals and vectors will maintain their lengths under scaling.
    void	addAttributes(const GA_AttributeFilter &filter,
			    bool keep_lenghts=false);
    /// Add a single transformable attributes to the handle list.  When @c
    /// keep_length is set, normals and vectors will maintain their lengths
    /// under scaling.
    void	addAttribute(GA_Attribute *attrib,
			    bool keep_lengths=false);
    /// Clear the list of transformable attributes.
    void        clearAttributes();

    /// Return the number of attributes which will be transformed
    exint		 entries() const { return myHandles.entries(); }
    /// Return the handle to the individual attribute
    const Handle	&getHandle(exint i) const { return myHandles(i); }
    /// Return the bound detail
    GA_Detail		&getDetail() const { return myDetail; }

    /// @{
    /// Perform the transformation on all attributes.
    ///
    /// @note The Transform reference is non-const.  The methods are mostly
    /// thread safe (provided threads access unique pages of data).  However,
    /// since the Transform object is non-const, the Transform should @b not be
    /// shared across threads.
    void	transform(const GA_Range &range,
			Transform<fpreal32> &m) const;
    void	transform(const GA_Range &range,
			Transform<fpreal64> &m) const;
    void	transform(const GA_Range &range,
			Transform<fpreal32> &m32,
			Transform<fpreal64> &m64) const;
    void	transform(GA_Offset offset, Transform<fpreal32> &m) const;
    void	transform(GA_Offset offset, Transform<fpreal64> &m) const;
    void	transform(GA_Offset offset,
			Transform<fpreal32> &m32,
			Transform<fpreal64> &m64) const;
    /// @}

    /// @{
    /// Check whether any attributes require the inverse/quaternion transforms
    bool	needInverse() const	{ return myNeedI; }
    bool	needQuaternion() const	{ return myNeedQ; }
    /// @}

    /// @{
    /// Check whether any attributes are single- or double-precision
    bool        hasSingle() const { return myHasSingle; }
    bool        hasDouble() const { return myHasDouble; }
    /// @}

private:
    /// Ensure the transform has its inverse/quaternion computed (if needed)
    template <typename FLOAT_T>
    void	updateTransform(Transform<FLOAT_T> &xform) const
		{
		    if (myNeedI)
			xform.computeInverse();
		    if (myNeedQ)
			xform.computeQuaternion();
		}
    GA_Detail		&myDetail;
    UT_Array<Handle>	 myHandles;
    GA_AttributeOwner	 myOwner;
    bool		 myNeedI;
    bool		 myNeedQ;
    bool                 myHasSingle;
    bool                 myHasDouble;
};

#endif
