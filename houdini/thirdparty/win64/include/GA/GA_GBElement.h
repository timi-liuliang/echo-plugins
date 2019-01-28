/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GBElement.h ( GA Library, C++)
 *
 * COMMENTS:
 * 	Class to maintain old GB/GD/GEO/GU/ interfaces to elements.  For
 * 	points and vertices, instances are mere interface stubs wrapping
 * 	an index map and an offset.  GA_Primitive, however, derives from
 * 	GA_GBElement to permit maximum legacy compatibility.
 *	
 *	This class provides methods from GB_Element, GB_AttributeElem, as
 *	well as utility methods to aid in implementing the interfaces of
 *	GB_Vertex (and its subclasses) and [GD|GEO]_Point.
 */

#ifndef __GA_GBElement__
#define __GA_GBElement__

#include "GA_API.h"

#include "GA_AIFSharedStringTuple.h"
#include "GA_AIFStringTuple.h"
#include "GA_Attribute.h"
#include "GA_AttributeDict.h"
#include "GA_AttributeRef.h"
#include "GA_AttributeSet.h"
#include "GA_Detail.h"
#include "GA_IndexMap.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>
#include <string.h>

class GA_AttributeRefMap;
class GA_Defragment;

SYS_DEPRECATED_PUSH_DISABLE()

/// @note This class has no virtual methods nor is the destructor virtual.
/// Instances of derived classes are either passed by value (vertices),
/// managed by an object derived from GA_GBElementPage (points), or managed
/// through pointers to the subclass GA_Primitive (primitives).
class GA_API GA_GBElement 
{
protected:
    /// This class is only for supporting legacy code and is slated for future
    /// removal.  We protect the constructors to discourage extraneous usage.
    inline GA_GBElement(const GA_IndexMap &index_map, GA_Offset offset)
	: myIndexMap(&index_map)
	, myOffset(offset)
    {
    }
    /// Default c-tor
    GA_GBElement()
	: myIndexMap(NULL)
	, myOffset(-1)		// Start with an invalid offset
    { }
    /// Copy c-tor
    GA_GBElement(const GA_GBElement &src)
	: myIndexMap(src.myIndexMap)
	, myOffset(src.myOffset)
    { }

     /// We protect the destructor as nobody has any business using a reference
     /// to this class to delete an instance of a class derived from us.
    ~GA_GBElement() {}

    /// @{
    /// Copy/Compare the @b handle (not the contents)
    GA_GBElement	&operator=(const GA_GBElement &src)
			 {
			     myIndexMap = src.myIndexMap;
			     myOffset = src.myOffset;
			     return *this;
			 }
    bool		 operator==(const GA_GBElement &src) const
			 {
			     return myIndexMap == src.myIndexMap &&
				    myOffset == src.myOffset;
			 }
    /// @}

public:

    /// Provide access to the GA_Detail containing this element
    GA_Detail		&getDetail() const	{ return myIndexMap->getDetail(); }
    /// Provide access to the GA_IndexMap containing this element
    const GA_IndexMap	&getIndexMap() const	{ return *myIndexMap; }

    /// Provide the GA_Offset of this element in its GA_IndexMap.  This should
    /// be used to access attribute data (not the ordered index).
    GA_Offset		 getMapOffset() const	{ return myOffset; }
    /// Provide the ordered GA_Index of this element in its GA_IndexMap
    GA_Index		 getMapIndex() const
				{ return myIndexMap->indexFromOffset(myOffset); }

    /// Test to see whether the GA_GBElement is bound to a valid offset
    SYS_DEPRECATED_HDK(13.0)
    bool		 isGAValid() const	{ return GAisValid(myOffset); }

    /// Copy a single attribute over.
    SYS_DEPRECATED_HDK(13.0)
    void		copyAttributeData(const GA_RWAttributeRef &dstref,
					  const GA_GBElement &src,
					  const GA_ROAttributeRef &srcref);

    /// For defragmentation, we need to update the offset
    void		swapOffsetValue(const GA_Defragment &defrag);

    // --------------------------------------------------------------------
    // 
    // Methods from GB_Element
    //
    // --------------------------------------------------------------------
    /// Return the ordered position of the element.  This provides backwards
    /// compatibility with the older GB library.
    SYS_DEPRECATED_HDK(13.0)
    GA_Index		 getNum() const		{ return getMapIndex(); }

    /// This method returns true if the element (point/primitive/vertex) is
    /// part of 3D geometry.  Otherwise it returns false.
    bool		 isPrimary() const;

    // Methods which don't have any support in GA
    // const GB_BitArray	&getGroups();
    // void			 setNum();

    // --------------------------------------------------------------------
    // 
    // Methods from GB_AttributeElem
    //
    // --------------------------------------------------------------------
    SYS_DEPRECATED_HDK(13.0)
    void	copyAttribData(const GA_GBElement &src,
			    const GA_AttributeSet &src_dict,
			    const GA_AttributeSet &dest_dict,
			    bool copy_point_P=false)
		{
		    gaCopyAttribData(src, src_dict, dest_dict,
			    copy_point_P ? NULL : getDetail().getP());
		}
    SYS_DEPRECATED_HDK(13.0)
    void	copyAttribData(const GA_GBElement &src,
			    const GA_AttributeSet &dict,
			    bool copy_point_P=false)
		{
		    copyAttribData(src, dict, dict, copy_point_P);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	copyAttributeValues(const GA_GBElement &src,
				    const GA_AttributeSet &dict,
				    bool copy_point_P=false)
		{
		    copyAttribData(src, dict, copy_point_P);
		}

    SYS_DEPRECATED_HDK(13.0)
    void	scaleAttributeValues(fpreal scale,
				    const GA_AttributeSet &dest_dict,
				    bool scale_point_P=false);
    SYS_DEPRECATED_HDK(13.0)
    void	addAttributeValues(const GA_GBElement &src,
				    const GA_AttributeSet &src_dict,
				    const GA_AttributeSet &dest_dict,
				    fpreal scale=1,
				    bool add_point_P=false);
    SYS_DEPRECATED_HDK(13.0)
    void	addAttributeValues(const GA_GBElement &src,
				    const GA_AttributeSet &dict,
				    fpreal scale=1,
				    bool add_point_P=false)
		{
		    addAttributeValues(src, dict, dict, scale, add_point_P);
		}
    SYS_DEPRECATED_HDK(13.0)
    void	lerpAttributeValues(const GA_GBElement &v0,
				    const GA_GBElement &v1,
				    fpreal bias,
				    const GA_AttributeSet &src_dict,
				    const GA_AttributeSet &dest_dict,
				    bool lerp_point_P=false);
    SYS_DEPRECATED_HDK(13.0)
    void	lerpAttributeValues(const GA_GBElement &v0,
				    const GA_GBElement &v1,
				    fpreal bias,
				    const GA_AttributeSet &dict,
				    bool lerp_point_P=false)
		{
		    lerpAttributeValues(v0, v1, bias, dict, dict, lerp_point_P);
		}

private:
    /// @cond

    /// Effectively a template namespace for access to basic types supported by
    /// GA_AIFTuple.
    template <typename T> class BasicAccessors
    {
    public:
	static T	 get(const GA_ROAttributeRef &h, GA_Offset off,
			     int index)
			    {
				T	 val;
				get(h, off, val, index);
				return val;
			    }
	static void	 get(const GA_ROAttributeRef &h, GA_Offset off,
			     T &val, int index)
			    {
				if (!h.getAIFTuple() ||
				    !h.getAIFTuple()->get(h.getAttribute(), off,
							  val, index))
				{
				    val = 0;
				}
			    }
	static void	 set(const GA_WOAttributeRef &h, GA_Offset off,
			     const T &val, int index)
			    {
				if (h.getAIFTuple())
				    h.getAIFTuple()->set(h.getAttribute(), off,
							 val, index);
			    }
	static void	 get(const GA_ROAttributeRef &h, GA_Offset off,
			     T *val, int count)
			    {
				if (!h.getAIFTuple() ||
				    !h.getAIFTuple()->get(h.getAttribute(), off,
							  val, count))
				{
				    memset(val, 0, sizeof(T)*count);
				}
			    }
	static void	 set(const GA_WOAttributeRef &h, GA_Offset off,
			     const T *val, int count)
			    {
				if (h.getAIFTuple())
				    h.getAIFTuple()->set(h.getAttribute(), off,
							 val, count);
			    }
    };

    /// Effectively a template namespace for access to POD types consisting of
    /// a simple tuple of a basic type supported by GA_AIFTuple.
    template <typename T, typename BASIC_T> class PODCompoundAccessors
    {
    public:
	static T	 get(const GA_ROAttributeRef &h, GA_Offset off,
			     int index)
			    {
				T	 val;
				get(h, off, val, index);
				return val;
			    }
	static void	 get(const GA_ROAttributeRef &h, GA_Offset off,
			     T &val, int index)
			    {
				const size_t n_basic =
						sizeof(T)/sizeof(BASIC_T);
				if (!h.getAIFTuple() ||
				    !h.getAIFTuple()->get(h.getAttribute(), off,
							  (BASIC_T *)&val,
							  n_basic,
							  index*n_basic))
				    val = T();
			    }
	static void	 set(const GA_WOAttributeRef &h, GA_Offset off,
			     const T &val, int index)
			    {
				const size_t n_basic =
						sizeof(T)/sizeof(BASIC_T);
				if (h.getAIFTuple())
				    h.getAIFTuple()->set(h.getAttribute(), off,
							 (const BASIC_T *)&val,
							 n_basic,
							 index*n_basic);
			    }
	static void	 get(const GA_ROAttributeRef &h, GA_Offset off,
			     T *val, int count)
			    {
				if (!h.getAIFTuple() ||
				    !h.getAIFTuple()->get(h.getAttribute(), off,
					    (BASIC_T *)val,
					    count*sizeof(T)/sizeof(BASIC_T)))
				{
				    T	def;
				    for (int i; i < count; i++)
					val[i] = def;
				}
			    }
	static void	 set(const GA_WOAttributeRef &h, GA_Offset off,
			     const T *val, int count)
			    {
				if (h.getAIFTuple())
				    h.getAIFTuple()->set(h.getAttribute(), off,
					    (const BASIC_T *)val,
					    count*sizeof(T)/sizeof(BASIC_T));
			    }
    };
    /// @endcond
    /// @cond

    /// A private template class used to register allowable type parameters for
    /// the public access method templates that follow.
    ///
    /// Explicit specializations of this class follow, each containing a single
    /// Accessors type which behaves effectively as a namespace.
    ///
    /// The non-type FAKE parameter is to ensure the following specializations
    /// are only partial, not full.  Full specializations are not allowed at
    /// class scope.
    template <typename T, int FAKE> class TypeInfo
    {
    };
    /// @endcond

    /// @cond
    template <int FAKE> class TypeInfo<fpreal32, FAKE>
	{ public: typedef BasicAccessors<fpreal32> Accessors; };
    template <int FAKE> class TypeInfo<int32, FAKE>
	{ public: typedef BasicAccessors<int32> Accessors; };

    template <int FAKE> class TypeInfo<fpreal64, FAKE>
	{ public: typedef BasicAccessors<fpreal64> Accessors; };
    template <int FAKE> class TypeInfo<int64, FAKE>
	{ public: typedef BasicAccessors<int64> Accessors; };

    template <int FAKE> class TypeInfo<UT_Vector2, FAKE>
	{ public: typedef PODCompoundAccessors<UT_Vector2, fpreal32> Accessors; };
    template <int FAKE> class TypeInfo<UT_Vector3, FAKE>
	{ public: typedef PODCompoundAccessors<UT_Vector3, fpreal32> Accessors; };
    template <int FAKE> class TypeInfo<UT_Vector4, FAKE>
	{ public: typedef PODCompoundAccessors<UT_Vector4, fpreal32> Accessors; };
    template <int FAKE> class TypeInfo<UT_Quaternion, FAKE>
	{ public: typedef PODCompoundAccessors<UT_Quaternion, fpreal32> Accessors; };
    template <int FAKE> class TypeInfo<UT_Matrix3, FAKE>
	{ public: typedef PODCompoundAccessors<UT_Matrix3, fpreal32> Accessors; };
    template <int FAKE> class TypeInfo<UT_Matrix4, FAKE>
	{ public: typedef PODCompoundAccessors<UT_Matrix4, fpreal32> Accessors; };
    /// @endcond

public:


    // Methods which don't have any support in GA
    // template<typename T>	*castAttribData(int n);
    // void			*getAttribData(int n);
    // GB_AttributeData		&getAttrib(int n);

    /// @name Get/Set Accessors For Individual Attributes
    /// @{

    /// These template methods can only be instantiated with the following
    /// types:
    ///  fpreal32, fpreal64, int32, int64, UT_Vector[34], UT_Matrix[34]
    ///
    /// These methods use AIFs and so may fail gracefully or convert data
    /// types appropriately.

    SYS_DEPRECATED_HDK(14.0)
    const char *getString(const GA_ROAttributeRef &h, int index = 0) const
		    {
			const GA_AIFStringTuple *aif = h.getAIFStringTuple();
			if (!aif)
			    return 0;
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			return aif->getString(h.getAttribute(), getMapOffset(), 
					      index); 
		    }

    SYS_DEPRECATED_HDK(14.0)
    void 	setString(const GA_WOAttributeRef &h, const char *str,
			    int index = 0)
		    { 
			const GA_AIFStringTuple *aif = h.getAIFStringTuple();
			if (!aif) 
			    return;
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			aif->setString(h.getAttribute(), getMapOffset(), 
				       str, index);
		    }

    template <typename T>
    SYS_DEPRECATED_HDK(14.0)
	T	 getValue(const GA_ROAttributeRef &h, int index = 0) const
		    { 
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			return TypeInfo<T,0>::Accessors::get(
						h, getMapOffset(), index);
		    }
    template <typename T>
    SYS_DEPRECATED_HDK(14.0)
	void	 setValue(const GA_WOAttributeRef &h, const T &val,
			  int index = 0)
		    { 
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			TypeInfo<T,0>::Accessors::set(
					    h, getMapOffset(), val, index);
		    }
    template <typename T>
    SYS_DEPRECATED_HDK(14.0)
	void	 get(const GA_ROAttributeRef &h, T &val, int index = 0) const
		    { 
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			TypeInfo<T,0>::Accessors::get(
					    h, getMapOffset(), val, index);
		    }
    template <typename T>
    SYS_DEPRECATED_HDK(14.0)
	void	 get(const GA_ROAttributeRef &h, T *val, int count) const
		    { 
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			TypeInfo<T,0>::Accessors::get(
					    h, getMapOffset(), val, count);
		    }
    template <typename T>
    SYS_DEPRECATED_HDK(14.0)
	void	 set(const GA_WOAttributeRef &h, const T *val, int count)
		    { 
			UT_ASSERT( h.getAttribute() && 
			    &h.getAttribute()->getDetail() == &getDetail()); 
			TypeInfo<T,0>::Accessors::set(
					    h, getMapOffset(), val, count);
		    }
    /// @}


protected:
    void	gaCopyAttribData(const GA_GBElement *src,
				const GA_Attribute *skip=NULL);
    void	gaCopyAttribData(const GA_GBElement &src,
			    const GA_AttributeSet &src_dict,
			    const GA_AttributeSet &dest_dict,
			    const GA_Attribute *skip = NULL);
    void	gaCopyAttribData(const GA_GBElement &src,
			    const GA_AttributeDict &src_dict,
			    const GA_AttributeDict &dest_dict,
			    const GA_Attribute *skip = NULL);

private:
    const GA_IndexMap	*myIndexMap;
    GA_Offset		 myOffset;
};

SYS_DEPRECATED_POP_DISABLE()

#endif

