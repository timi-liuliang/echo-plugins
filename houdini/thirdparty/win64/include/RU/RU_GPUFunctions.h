/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_GPUFunctions.h ( UT Library, C++)
 *
 * COMMENTS:
 *    This class contains a list of functions that return GPU code in
 *    in a UT_String for doing various things.
 *    
 *    The parameters and local variables for each are listed. Parameters
 *    must have valid values assigned, locals just need to be passed to the
 *    function (GLSL doesn't allow locals declared in functions - you have to
 *    pass them in).
 *
 *    If you wish to modify the code string, harden it first.
 *
 */
#ifndef RU_GPUFunctions_h
#define RU_GPUFunctions_h

#include "RU_API.h"
#include <PXL/PXL_LumFuncs.h>

class UT_String;
class TIL_TileList;
class UT_ComputeGPU;

// This adds the various tiles as single outputs to the shader, as
// OUT_red(colorname.r); OUT_green(colorname.g); etc. It also registers them
// as outputs with the UT_ComputeGPU engine.
extern RU_API void RUsetOutput(UT_ComputeGPU &engine,
			       TIL_TileList *tiles,
			       const char *outname,
			       UT_String &shader);

// vec4 texture_sample(...)
//    Samples a texture in the area (x1,y1) - (x2,y2) from 'texture' and
//    returns the filtered color.
// Parameters:
//   float x0  - sampling area
//   float y0
//   float x1
//   float y1
extern RU_API void RUgetTextureSampleGPU(const char *texture,
					 int	     vsize,
					 UT_String &code);
extern RU_API void RUgetTextureClamp(UT_String &code);
extern RU_API void RUgetTextureBilinear(const char *texture,
					int vsize,
					UT_String &code);

// vec3 toHSV(...)
//    converts an RGB color to HSV
// Parameters
//   vec3   rgbcol  - RGB color to be converted
extern RU_API void RUgetToHSVConvertGPU(UT_String &code);

// vec3 toHSV(...)
//    converts an HSV color to RGB
// Parameters
//   vec3   hsvcol  - HSV color to be converted
extern RU_API void RUgetFromHSVConvertGPU(UT_String &code);


// vec3 lambert_diffuse(vec3 N, vec3 L)
// Parameters
//   N - surface normal
//   L - Light dir
extern RU_API void RUgetDiffuseLambert(UT_String &code);

// float oren_diffuse(vec3 N, vec3 L, vec3 I, float rough, ...)
// Parameters
//   N - surface normal
//   L - Light dir
//   I - Incident ray
//   rough - roughness factor, from 0 (none) to 1
extern RU_API void RUgetDiffuseOrenNayar(UT_String &code);

// float phong_specular(vec3 N, vec3 L, vec3 I, float power)
// Parameters:
//   N - surface normal
//   L - Light dir
//   I - Incident ray
//   power - phong exponent (higher = sharper highlight)
extern RU_API void RUgetSpecularPhong(UT_String &code);

// float blinn_specular(vec3 N, vec3 L, vec3 I, float rough, ...)
// Parameters:
//   N - surface normal
//   L - Light dir
//   I - Incident ray
//   rough - roughness factor, from 0 (none) to 1
extern RU_API void RUgetSpecularBlinn(UT_String &code);

// Parm : int dropoff type. 0 = linear, 1 = ease in, 2 = ease out,
// 3 = ease in/out, 4 = gaussian
extern RU_API void RUgetDropoffFunc(UT_String &code);
// returns a branchless version for the specific function.
extern RU_API void RUgetSoftDropoff(int function, UT_String &code);

// Parm: int lumfunc : 0 = standard lum, 1 = NTSC lum, 2 = max, 3 = min,
//                     4 = average, 5 = red, 6 = green, 7 = blue.
// Returns a float.
extern RU_API void RUgetLuminanceFunc(UT_String &code);
extern RU_API void RUgetLuminanceFunc(PXL_LumFunction f, UT_String &code);

extern RU_API void RUgetFlatLightFrontend(UT_String &code);
extern RU_API void RUgetBumpLightFrontend(UT_String &code,
					  const char *bump_name);
extern RU_API void RUget3DLightFrontend(UT_String &code,
					const char *point_name,
					const char *normal_name);

extern RU_API void RUgetLightingBackend(UT_String &code,
					const char *color_name);
extern RU_API void RUgetVolumetricLight(UT_String &code);

extern RU_API void RUgetLightingMain(UT_String &code);

extern RU_API void RUgetTransform(int vsize, const char *texture,
				  UT_String &code);

extern RU_API void RUgetTransformBlur(int vsize, const char *image,
				      UT_String &code);
extern RU_API void RUgetTransformDeformBlur(int vsize,
					    const char *image,
					    const char *prev,
					    const char *next,
					    UT_String &code);

extern RU_API void RUgetFogMain(UT_String &code);
extern RU_API void RUgetFogDistanceZ(const char *depth_name, UT_String &code);
extern RU_API void RUgetFogDistanceP(const char *point_name, UT_String &code);
extern RU_API void RUgetFogDistanceNone(UT_String &code);
extern RU_API void RUgetFogColorConst(UT_String &code);
extern RU_API void RUgetFogColorImage(const char *color_name,UT_String &code);
extern RU_API void RUgetFogDensityConst(UT_String &code);
extern RU_API void RUgetFogDensityImage(const char *alpha_name,UT_String &code);
extern RU_API void RUgetFogConst(UT_String &code);
extern RU_API void RUgetFogNoise(UT_String &code);

extern RU_API void RUgetFogTypeFog(const char *color_name, UT_String &code);
extern RU_API void RUgetFogTypeAdd(const char *color_name, UT_String &code);
extern RU_API void RUgetFogTypeHeat(const char *color_name, UT_String &code);
extern RU_API void RUgetFogTypeHaze(const char *color_name, UT_String &code);

extern RU_API void RUgetZCompositeSimple(int vsize, UT_String &code);
extern RU_API void RUgetZCompositeSuperSample(int vsize, UT_String &code);
extern RU_API void RUgetZCompositeGeo(int vsize, UT_String &code);
extern RU_API void RUgetZCompositeSuperSampleGeo(int vsize, UT_String &code);

extern RU_API void RUgetEnvironmentMain(int vsize, UT_String &code);
extern RU_API void RUgetEnvironmentBump(UT_String &code);
extern RU_API void RUgetEnvironmentNormal(UT_String &code);
extern RU_API void RUgetEnvironmentCubeDeep(UT_String &code);
extern RU_API void RUgetEnvironmentCubeStrip(UT_String &code);
extern RU_API void RUgetEnvironmentCubeCrossHorz(UT_String &code);
extern RU_API void RUgetEnvironmentCubeCrossVert(UT_String &code);
extern RU_API void RUgetEnvironmentLatLong(UT_String &code);

extern RU_API void RUgetKeyMain(int vsize, const char *key, UT_String &code);
extern RU_API void RUgetKeyMainSuperSample(int vsize, UT_String &code);
extern RU_API void RUgetKeyMainSuperSampleSelf(int vsize, UT_String &code);
extern RU_API void RUgetKeyLuma(int vsize, UT_String &code);
extern RU_API void RUgetKeyChroma(int vsize, UT_String &code);

extern RU_API void RUgetNoiseCop(UT_String &code);
extern RU_API void RUgetPerlinNoise(UT_String &code);
extern RU_API void RUgetSparseNoise(UT_String &code);
extern RU_API void RUgetAlligatorNoise(UT_String &code);

extern RU_API void RUgetDeform(const char *pname, int vsize, UT_String &code);
extern RU_API void RUgetDeformMask(const char *pname, int vsize,
				   UT_String &code);

extern RU_API void RUgetConvolve(int vsize, UT_String &code);

extern RU_API void RUgetExpand(int vsize, UT_String &code);
extern RU_API void RUgetExpandLuma(int vsize, UT_String &code);
extern RU_API void RUgetExpandPerPixel(int vsize, UT_String &code);
extern RU_API void RUgetExpandLumaPerPixel(int vsize, UT_String &code);

extern RU_API void RUgetBlurPerPixel(int vsize, UT_String &code);
#endif
