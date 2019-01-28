/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedFactory.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PackedFactory__
#define __GU_PackedFactory__

#include "GU_API.h"
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <GA/GA_PrimitiveDefinition.h>
#include <GA/GA_IntrinsicDef.h>
#include "GU_PackedImpl.h"

namespace GU_PackedFactoryPrivate
{
    class IntrinsicDef;
}

class GU_API GU_PackedFactory
{
public:
    /// @{
    /// Typedefs
    typedef GA_PrimitiveDefinition::SharedDataLoader	SharedDataLoader;
    typedef GU_PackedFactoryPrivate::IntrinsicDef	IntrinsicDef;
    /// @}

    /// <b> Intrinsics in Packed Primitives </b>
    ///
    /// Convenience methods for casting a subclass of GU_PackedImpl accessor
    /// methods to the methods used by registerIntrinsic or
    /// registerTupleIntrinsic.  for example, the factory constructor might
    /// look like: @code
    ///    class MyImpl : public GU_PackedImpl {
    ///        const std::string &filename() const     { return myFilename; }
    ///        void setFilename(const std::string &f)  { myFilename = f; }
    ///        fpreal frame() const                    { return myFrame; }
    ///        void setFrame(fpreal f)                 { myFrame = f; }
    ///        bool isAnimated() const                 { return myAnimated; }
    ///    };
    ///    class MyFactory : public GU_PackedFactory {
    ///        MyFactory()
    ///        {
    ///            registerIntrinsic("filename"_sh,
    ///                StdStringGetterCast(&MyImpl::filename),
    ///                StdStringSetterCast(&MyImpl::setFilename));
    ///            registerIntrinsic("frame"_sh,
    ///                FloatGetterCast(&MyImpl::frame),
    ///                FloatSetterCast(&MyImpl::setFrame));
    ///            registerIntrinsic("animated"_sh,
    ///                BoolGetterCast(&MyImpl::isAnimated));
    ///        }
    ///    };
    /// @endcode

#define DECL_GET_CAST(TYPE, METHOD, METHODCAST) \
    template <typename CLASS> inline GU_PackedImpl::METHOD \
		METHODCAST(TYPE (CLASS::*method)(const GU_PrimPacked *)const) \
		    { return (GU_PackedImpl::METHOD)(method); }
#define DECL_SET_CAST(TYPE, METHOD, METHODCAST) \
    template <typename CLASS> inline GU_PackedImpl::METHOD \
		METHODCAST(void (CLASS::*method)(GU_PrimPacked *, TYPE)) \
		    { return (GU_PackedImpl::METHOD)(method); }

#define DECL_TGET_CAST(TYPE, METHOD, METHODCAST) \
    template <typename CLASS> inline GU_PackedImpl::METHOD \
		METHODCAST(TYPE (CLASS::*method)(const GU_PrimPacked *,exint)const) \
		    { return (GU_PackedImpl::METHOD)(method); }
#define DECL_TSET_CAST(TYPE, METHOD, METHODCAST) \
    template <typename CLASS> inline GU_PackedImpl::METHOD \
		METHODCAST(void (CLASS::*method)(GU_PrimPacked *, exint, TYPE)) \
		    { return (GU_PackedImpl::METHOD)(method); }

#define DECL_VGET_CAST(TYPE, METHOD, METHODCAST) \
    template <typename CLASS> inline GU_PackedImpl::METHOD \
		METHODCAST(void (CLASS::*method)(const GU_PrimPacked *, TYPE *, exint) const) \
		    { return (GU_PackedImpl::METHOD)(method); }
#define DECL_VSET_CAST(TYPE, METHOD, METHODCAST) \
    template <typename CLASS> inline GU_PackedImpl::METHOD \
		METHODCAST(void (CLASS::*method)(GU_PrimPacked *, const TYPE *, exint)) \
		    { return (GU_PackedImpl::METHOD)(method); }


    DECL_GET_CAST(fpreal, FloatGetter, FloatGetterCast)
    DECL_SET_CAST(fpreal, FloatSetter, FloatSetterCast)
    DECL_GET_CAST(exint, IntGetter, IntGetterCast)
    DECL_SET_CAST(exint, IntSetter, IntSetterCast)
    DECL_GET_CAST(bool, BoolGetter, BoolGetterCast)
    DECL_SET_CAST(bool, BoolSetter, BoolSetterCast)
    DECL_GET_CAST(const char *, StringGetter, StringGetterCast)
    DECL_SET_CAST(const char *, StringSetter, StringSetterCast)
    DECL_GET_CAST(std::string, StdStringGetter, StdStringGetterCast)
    DECL_SET_CAST(const std::string &, StdStringSetter, StdStringSetterCast)
    DECL_GET_CAST(UT_StringHolder, StringHolderGetter, StringHolderGetterCast)
    DECL_SET_CAST(const UT_StringHolder &, StringHolderSetter, StringHolderSetterCast)

    DECL_TGET_CAST(fpreal, FloatTupleGetter, FloatTupleGetterCast)
    DECL_TSET_CAST(fpreal, FloatTupleSetter, FloatTupleSetterCast)
    DECL_TGET_CAST(exint, IntTupleGetter, IntTupleGetterCast)
    DECL_TSET_CAST(exint, IntTupleSetter, IntTupleSetterCast)
    DECL_TGET_CAST(bool, BoolTupleGetter, BoolTupleGetterCast)
    DECL_TSET_CAST(bool, BoolTupleSetter, BoolTupleSetterCast)
    DECL_TGET_CAST(const char *, StringTupleGetter, StringTupleGetterCast)
    DECL_TSET_CAST(const char *, StringTupleSetter, StringTupleSetterCast)
    DECL_TGET_CAST(const std::string &, StdStringTupleGetter, StdStringTupleGetterCast)
    DECL_TSET_CAST(const std::string &, StdStringTupleSetter, StdStringTupleSetterCast)
    DECL_TGET_CAST(const UT_StringHolder &, StringHolderTupleGetter, StringHolderTupleGetterCast)
    DECL_TSET_CAST(const UT_StringHolder &, StringHolderTupleSetter, StringHolderTupleSetterCast)

    DECL_VGET_CAST(fpreal32, F32VectorGetter, F32VectorGetterCast)
    DECL_VSET_CAST(fpreal32, F32VectorSetter, F32VectorSetterCast)
    DECL_VGET_CAST(fpreal64, F64VectorGetter, F64VectorGetterCast)
    DECL_VSET_CAST(fpreal64, F64VectorSetter, F64VectorSetterCast)
    DECL_VGET_CAST(int32, I32VectorGetter, I32VectorGetterCast)
    DECL_VSET_CAST(int32, I32VectorSetter, I32VectorSetterCast)
    DECL_VGET_CAST(int64, I64VectorGetter, I64VectorGetterCast)
    DECL_VSET_CAST(int64, I64VectorSetter, I64VectorSetterCast)
    DECL_VGET_CAST(bool, BVectorGetter, BVectorGetterCast)
    DECL_VSET_CAST(bool, BVectorSetter, BVectorSetterCast)

    template <typename CLASS>
    inline GU_PackedImpl::StringArrayGetter
    StringArrayGetterCast(void (CLASS::*method)(const GU_PrimPacked *,UT_StringArray &) const)
    {
        return (GU_PackedImpl::StringArrayGetter)(method);
    }

    DECL_SET_CAST(const UT_StringArray &, StringArraySetter,
	    StringArraySetterCast)

#undef DECL_GET_METHOD_CAST
#undef DECL_SET_METHOD_CAST
#undef DECL_TGET_METHOD_CAST
#undef DECL_TSET_METHOD_CAST
#undef DECL_VGET_METHOD_CAST
#undef DECL_VSET_METHOD_CAST

    /// Factory constructor
    GU_PackedFactory(const UT_StringHolder &name,
		    const UT_StringHolder &label,
		    const UT_StringHolder &icon=UT_StringHolder());
    /// Factory destructor
    virtual ~GU_PackedFactory();

    bool isRegistered() const { return myTypeDef != 0; }

    /// @{
    /// Member data accessors
    const UT_StringHolder	&name() const { return myName; }
    const UT_StringHolder	&label() const { return myLabel; }
    const UT_StringHolder	&icon() const { return myIcon; }
    const GA_PrimitiveDefinition &typeDef() const { return *myTypeDef; }
    /// @}

    /// Create instance of a procedural
    virtual GU_PackedImpl	*create() const = 0;

    /// Load shared data for the specific implementation.  Shared data must be
    /// saved in an array containing: @code
    ///	  [ "string type", "string key", object ]
    /// @endcode
    /// The @c type is used to differentiate between different types of shared
    /// primitive data, while the @c key is used to uniquely identify blocks of
    /// shared data.
    ///
    /// When loading a geometry file, the type and key will be loaded and
    /// passed to this method to resolve the shared data.  The code should look
    /// something like: @code
    ///    if (!strcmp(type, "typename1"))
    ///    {
    ///         Typename1Container  data;
    ///         if (!Typename1Container::load(p, data.myObject))
    ///              return false;
    ///         loadmap.resolveSharedData(key, data);
    ///     }
    /// @endcode
    /// This assumes that Typename1Container is a sub-class of
    /// GA_SharedLoadData and where Typename1Container::myObject is the object
    /// which is shared between multiple primitives.
    ///
    /// Note that the base class should be called if the type can't be handled
    /// by the subclass.
    virtual GA_SharedLoadData* loadSharedData(UT_JSONParser &p,
					const char *type,
					const char *key,
					bool isDelayedLoad) const;

    virtual GA_SharedLoadDataStat* statSharedData(UT_JSONParser& parser) const;

    /// Query how many intrinsic attributes are supported by this primitive type
    /// Default: return 0
    int		getIntrinsicCount() const
		    { return myIntrinsics.entries(); }
    /// Get information about the definition of the Nth intrinsic
    /// Default: assert
    SYS_DEPRECATED_REPLACE(16.0, UT_StringHolder)
    void	getIntrinsicDefinition(int idx,
				UT_String &name,
				GA_StorageClass &storage,
				bool &read_only) const;
    void	getIntrinsicDefinition(int idx,
				UT_StringHolder &name,
				GA_StorageClass &storage,
				bool &read_only) const;
    /// Get the intrinsic tuple size (which might be varying)
    /// Default: return 0
    GA_Size	getIntrinsicTupleSize(const GU_PrimPacked *proc,
				int idx) const;
    /// @{
    /// Get/Set intrinsic attribute values for a primitive
    /// Default: return 0
    GA_Size	getIntrinsicValue(const GU_PrimPacked *proc,
			int idx, fpreal64 *val, GA_Size vsize) const;
    GA_Size	getIntrinsicValue(const GU_PrimPacked *proc,
			int idx, int64 *val, GA_Size vsize) const;
    GA_Size	getIntrinsicValue(const GU_PrimPacked *proc,
			int idx, UT_String &str) const;
    GA_Size	getIntrinsicValue(const GU_PrimPacked *proc,
			int idx, UT_StringHolder &str) const;
    GA_Size	getIntrinsicValue(const GU_PrimPacked *proc,
			int idx, UT_StringArray &strings) const;
    GA_Size	setIntrinsicValue(GU_PrimPacked *proc,
			int idx, const fpreal64 *val, GA_Size vsize) const;
    GA_Size	setIntrinsicValue(GU_PrimPacked *proc,
			int idx, const int64 *val, GA_Size vsize) const;
    GA_Size	setIntrinsicValue(GU_PrimPacked *proc,
			int idx, const UT_StringArray &values) const;
    GA_Size	setIntrinsicValue(GU_PrimPacked *proc,
			int idx, const char **values, GA_Size size) const;
    /// @}

    /// Register a scalar floating point intrinsic
    void	registerIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::FloatGetter getter,
			GU_PackedImpl::FloatSetter setter=NULL);
    /// Register a scalar integer intrinsic
    void	registerIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter getter,
			GU_PackedImpl::IntSetter setter=NULL);
    /// Register a scalar boolean intrinsic
    void	registerIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::BoolGetter getter,
			GU_PackedImpl::BoolSetter setter=NULL);
    /// Register a scalar string (const UT_StringHolder &) intrinsic
    void	registerIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::StringGetter getter,
			GU_PackedImpl::StringSetter setter=NULL);
    /// Register a scalar string (const std::string &) intrinsic
    void	registerIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::StdStringGetter getter,
			GU_PackedImpl::StdStringSetter setter=NULL);
    /// Register a scalar string (const UT_StringHolder &) intrinsic
    void	registerIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::StringHolderGetter getter,
			GU_PackedImpl::StringHolderSetter setter=NULL);

    /// Register an intrinsic for a floating point array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::FloatTupleGetter getter,
			GU_PackedImpl::FloatTupleSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for an integer array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::IntTupleGetter getter,
			GU_PackedImpl::IntTupleSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for a boolean array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::BoolTupleGetter getter,
			GU_PackedImpl::BoolTupleSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for a string (const char *) array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::StringTupleGetter getter,
			GU_PackedImpl::StringTupleSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for a string (const std::string &) array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::StdStringTupleGetter getter,
			GU_PackedImpl::StdStringTupleSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for a string (const StringHolder &) array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::StringHolderTupleGetter getter,
			GU_PackedImpl::StringHolderTupleSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);

    /// Register an intrinsic for a floating point array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::F32VectorGetter getter,
			GU_PackedImpl::F32VectorSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for a floating point array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::F64VectorGetter getter,
			GU_PackedImpl::F64VectorSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for an integer array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::I32VectorGetter getter,
			GU_PackedImpl::I32VectorSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for an integer array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::I64VectorGetter getter,
			GU_PackedImpl::I64VectorSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register an intrinsic for a boolean array
    void	registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
			GU_PackedImpl::BVectorGetter getter,
			GU_PackedImpl::BVectorSetter setter=NULL,
			GU_PackedImpl::IntSetter set_tuple_size=NULL);
    /// Register a string array intrinsic.
    void        registerTupleIntrinsic(const UT_StringHolder &name,
			GU_PackedImpl::IntGetter get_tuple_size,
                        GU_PackedImpl::StringArrayGetter getter,
                        GU_PackedImpl::StringArraySetter setter = nullptr,
                        GU_PackedImpl::IntSetter set_tuple_size = nullptr);

    /// Clear any cached geometry.  The method should return the number of
    /// items cleared, and the default method just returns 0.
    virtual exint	clearCachedGeometry();

private:
    UT_StringHolder			 myName;
    UT_StringHolder			 myLabel;
    UT_StringHolder			 myIcon;
    const GA_PrimitiveDefinition	*myTypeDef;
    UT_Array<IntrinsicDef *>		 myIntrinsics;
    GA_IntrinsicDef			 myGAIntrinsics;

    friend class	GU_PrimPacked;	// To set the type definition.
};

#endif
