/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VRAY_Procedural.h
 *
 * COMMENTS:	Backward compatibility for previously named file
 */

#ifndef __VRAY_Procedural_h__
#define __VRAY_Procedural_h__

#include <RAY/RAY_Procedural.h>

using VRAY_Handle = RAY_Handle;
using VRAY_ObjectHandle = RAY_ObjectHandle;
using VRAY_MaterialHandle = RAY_MaterialHandle;
using VRAY_LightHandle = RAY_LightHandle;
using VRAY_ProceduralArg = RAY_ProceduralArg;
using VRAY_ProceduralGeo = RAY_ProceduralGeo;
using VRAY_ROProceduralGeo = RAY_ROProceduralGeo;
using VRAY_PrimitiveMaterialAttributes = RAY_PrimitiveMaterialAttributes;
using VRAY_ProceduralChild = RAY_ProceduralChild;
using VRAY_ProceduralChildPtr = RAY_ProceduralChildPtr;
using VRAY_Procedural = RAY_Procedural;

#endif
