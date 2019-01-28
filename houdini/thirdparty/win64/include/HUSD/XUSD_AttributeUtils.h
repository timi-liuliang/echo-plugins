
/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __XUSD_AttributeUtils_h__
#define __XUSD_AttributeUtils_h__

#include "HUSD_API.h"
#include <pxr/pxr.h>
#include <pxr/usd/sdf/attributeSpec.h>

PXR_NAMESPACE_OPEN_SCOPE
class UsdAttribute;
class UsdTimeCode;

/// Returns the SdfValueTypeName string best corresponding to the UT type.
template<typename UT_VALUE_TYPE>
HUSD_API const char *
HUSDgetSdfTypeName();

/// Sets the given @p attribute to the given @p value.
template<typename UT_VALUE_TYPE>
HUSD_API bool
HUSDsetAttribute(const UsdAttribute &attribute, const UT_VALUE_TYPE &value,
	const UsdTimeCode &timecode);

/// Gets the @p value of the given @p attribute at specified @p timecode.
template<typename UT_VALUE_TYPE>
HUSD_API bool
HUSDgetAttribute(const UsdAttribute &attribute, UT_VALUE_TYPE &value,
	const UsdTimeCode &timecode);

template<typename UT_VALUE_TYPE>
HUSD_API bool
HUSDgetAttributeSpecDefault(const SdfAttributeSpec &spec,
	UT_VALUE_TYPE &value);

/// Conversion function between VtValue and UT_* value objects.
template<typename UT_VALUE_TYPE>
HUSD_API bool
HUSDgetValue( const VtValue &vt_value, UT_VALUE_TYPE &ut_value );

/// Conversion function between from UT_* value objects and a VtValue with
/// a matching GfValue inside.
template<typename UT_VALUE_TYPE>
HUSD_API VtValue
HUSDgetVtValue( const UT_VALUE_TYPE &ut_value );

PXR_NAMESPACE_CLOSE_SCOPE

#endif
