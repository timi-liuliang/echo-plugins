/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_Primitive.h (GEO Library, C++)
 *
 * COMMENTS:    Wrapper for GEO_Primitive to emulate a pointer,
 *              avoiding hardening the primitive list in a few cases.
 */

#pragma once

#include "GEO_Primitive.h"
#include "GEO_Detail.h"
#include <GA/GA_PrimitiveP.h>

using GEO_ConstPrimitiveP = GA_PrimitiveP_T<const GEO_Primitive, const GEO_Detail>;
using GEO_PrimitiveP = GA_PrimitiveP_T<GEO_Primitive, GEO_Detail>;
