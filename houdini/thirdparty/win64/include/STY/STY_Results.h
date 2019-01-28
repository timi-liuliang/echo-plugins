/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Results.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Results__
#define __STY_Results__

#include "STY_API.h"
#include "STY_Style.h"
#include "STY_Types.h"
#include <UT/UT_StringMap.h>

/// A combination of a single property value and the style entry that
/// set the value.
class STY_Result
{
public:
			 STY_Result()
			 { }
			 STY_Result(const STY_ConstantHandle &value,
				    const STY_StyleHandle &source)
			    : myValue(value),
			      mySource(source)
			 { }
    void		 clear()
			 {
			    myValue.reset();
			    mySource.reset();
			 }
    bool		 isClear() const
			 {
			    return !myValue && !mySource;
			 }

    /// NOTE: These are only needed to avoid a compile error in
    ///       UT_ArrayMap<UT_StringHolder,STY_Result>::operator==()
    /// @{
    bool operator==(const STY_Result &that) const
    {
        return (myValue == that.myValue) && (mySource == that.mySource);
    }
    bool operator!=(const STY_Result &that) const
    {
        return !(*this == that);
    }
    /// @}

    STY_ConstantHandle	 myValue;
    STY_StyleHandle	 mySource;
};

namespace UT {
template<>
struct DefaultClearer<STY_Result>
{
    SYS_FORCE_INLINE
    static void clear(STY_Result &v)
    { v.clear(); }
    SYS_FORCE_INLINE
    static bool isClear(STY_Result &v)
    { return v.isClear(); }
    SYS_FORCE_INLINE
    static void clearConstruct(STY_Result *p)
    { new((void *)p) STY_Result(); }

    static const bool clearNeedsDestruction = false;
};
}

/// A map of property names to result values.
class STY_API STY_ResultMap :
    public UT_ArrayStringMap<STY_Result>
{
public:
    void		 merge(const STY_ResultMap &src);
    void		 merge(const STY_ConstantMap &src);
    uint		 hash() const;

    using Base = UT_ArrayStringMap<STY_Result>;
};

namespace UT {
template<>
struct DefaultClearer<STY_ResultMap> :
    public DefaultClearer<STY_ResultMap::Base>
{};
}

/// This is the data type returned when evaluating style sheet results.
/// The top level string map is the category name (one of the STY_OVERRIDE_*
/// constants). Each STY_ResultMap contains the result key/value pairs
/// within that category.
class STY_API STY_Results :
    public UT_ArrayStringMap<STY_ResultMap>
{
};

#endif

