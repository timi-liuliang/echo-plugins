/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Handles.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_Handles__
#define __GT_Handles__

#include "GT_API.h"
#include <UT/UT_IntrusivePtr.h>

/// Forward declaration of all shared pointer handles
class GT_AttributeList;
class GT_AttributeMap;
class GT_DataArray;
class GT_Primitive;
class GT_Transform;
class GT_TransformArray;
class GT_FaceSetMap;

/// @see GT_AttributeList
typedef UT_IntrusivePtr<GT_AttributeList>	GT_AttributeListHandle;
/// @see GT_AttributeMap
typedef UT_IntrusivePtr<GT_AttributeMap>	GT_AttributeMapHandle;
/// @see GT_DataArray
typedef UT_IntrusivePtr<GT_DataArray>		GT_DataArrayHandle;
/// @see GT_Primitive
typedef UT_IntrusivePtr<GT_Primitive>		GT_PrimitiveHandle;
/// @see GT_Transform
typedef UT_IntrusivePtr<GT_Transform>		GT_TransformHandle;
/// @see GT_TransformArray
typedef UT_IntrusivePtr<GT_TransformArray>	GT_TransformArrayHandle;
/// @see GT_FaceSetMap
typedef UT_IntrusivePtr<GT_FaceSetMap>		GT_FaceSetMapPtr;

#endif

