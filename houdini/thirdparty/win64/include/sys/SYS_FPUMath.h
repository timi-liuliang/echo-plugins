/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        SYS_FPUMath.h (SYS Library, C++)
 *
 * COMMENTS:    Prototypes for FPU implementations of math functions.
 *              These are intentionally named with non-standard names,
 *              because they have different signatures.
 */

#pragma once

#include "SYS_API.h"
#include "SYS_Inline.h"

#if defined(LINUX)
// This is actually here because Mac expects the assembly functions to have
// underscore prefixes, and Linux and Mac use the same assembly file.
#define NAME_WRAP(x) _##x
#else
#define NAME_WRAP(x) x
#endif

extern "C" {

SYS_API void NAME_WRAP(cos_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(cos_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(sin_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(sin_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(cos_and_sin_fpu_f)(const float* in, float out[2]);
SYS_API void NAME_WRAP(cos_and_sin_fpu_d)(const double* in, double out[2]);
SYS_API void NAME_WRAP(tan_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(tan_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(atan_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(atan_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(atan2_fpu_f)(const float in[2], float* out);
SYS_API void NAME_WRAP(atan2_fpu_d)(const double in[2], double* out);
SYS_API void NAME_WRAP(asin_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(asin_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(acos_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(acos_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(fmod_fpu_f)(const float in[2], float* out);
SYS_API void NAME_WRAP(fmod_fpu_d)(const double in[2], double* out);
SYS_API void NAME_WRAP(log_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(log_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(log10_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(log10_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(log2_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(log2_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(log1p_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(log1p_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(expm1_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(expm1_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(exp_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(exp_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(exp2_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(exp2_fpu_d)(double* in_out);
SYS_API void NAME_WRAP(pow_fpu_f)(const float in[2], float* out);
SYS_API void NAME_WRAP(pow_fpu_d)(const double in[2], double* out);
SYS_API void NAME_WRAP(tanh_fpu_f)(float* in_out);
SYS_API void NAME_WRAP(tanh_fpu_d)(double* in_out);

}

namespace fpu {
SYS_FORCE_INLINE float  cos(float x)    { NAME_WRAP(cos_fpu_f)(&x);   return x; }
SYS_FORCE_INLINE double cos(double x)   { NAME_WRAP(cos_fpu_d)(&x);   return x; }
SYS_FORCE_INLINE float  sin(float x)    { NAME_WRAP(sin_fpu_f)(&x);   return x; }
SYS_FORCE_INLINE double sin(double x)   { NAME_WRAP(sin_fpu_d)(&x);   return x; }
SYS_FORCE_INLINE void   cos_and_sin(float x,  float out[2])  { NAME_WRAP(cos_and_sin_fpu_f)(&x,out); }
SYS_FORCE_INLINE void   cos_and_sin(double x, double out[2]) { NAME_WRAP(cos_and_sin_fpu_d)(&x,out); }
SYS_FORCE_INLINE float  tan(float x)    { NAME_WRAP(tan_fpu_f)(&x);   return x; }
SYS_FORCE_INLINE double tan(double x)   { NAME_WRAP(tan_fpu_d)(&x);   return x; }
SYS_FORCE_INLINE float  atan(float x)   { NAME_WRAP(atan_fpu_f)(&x);  return x; }
SYS_FORCE_INLINE double atan(double x)  { NAME_WRAP(atan_fpu_d)(&x);  return x; }
SYS_FORCE_INLINE float  atan2(float y,  float x)  { float  in[2] = {y,x}; float out;  NAME_WRAP(atan2_fpu_f)(in,&out); return out; }
SYS_FORCE_INLINE double atan2(double y, double x) { double in[2] = {y,x}; double out; NAME_WRAP(atan2_fpu_d)(in,&out); return out; }
SYS_FORCE_INLINE float  asin(float x)   { NAME_WRAP(asin_fpu_f)(&x);  return x; }
SYS_FORCE_INLINE double asin(double x)  { NAME_WRAP(asin_fpu_d)(&x);  return x; }
SYS_FORCE_INLINE float  acos(float x)   { NAME_WRAP(acos_fpu_f)(&x);  return x; }
SYS_FORCE_INLINE double acos(double x)  { NAME_WRAP(acos_fpu_d)(&x);  return x; }
SYS_FORCE_INLINE float  fmod(float numer,  float denom)  { float  in[2] = {numer,denom}; float out;  NAME_WRAP(fmod_fpu_f)(in,&out); return out; }
SYS_FORCE_INLINE double fmod(double numer, double denom) { double in[2] = {numer,denom}; double out; NAME_WRAP(fmod_fpu_d)(in,&out); return out; }
SYS_FORCE_INLINE float  log(float x)    { NAME_WRAP(log_fpu_f)(&x);   return x; }
SYS_FORCE_INLINE double log(double x)   { NAME_WRAP(log_fpu_d)(&x);   return x; }
SYS_FORCE_INLINE float  log10(float x)  { NAME_WRAP(log10_fpu_f)(&x); return x; }
SYS_FORCE_INLINE double log10(double x) { NAME_WRAP(log10_fpu_d)(&x); return x; }
SYS_FORCE_INLINE float  log2(float x)   { NAME_WRAP(log2_fpu_f)(&x);  return x; }
SYS_FORCE_INLINE double log2(double x)  { NAME_WRAP(log2_fpu_d)(&x);  return x; }
SYS_FORCE_INLINE float  log1p(float x)  { NAME_WRAP(log1p_fpu_f)(&x); return x; }
SYS_FORCE_INLINE double log1p(double x) { NAME_WRAP(log1p_fpu_d)(&x); return x; }
SYS_FORCE_INLINE float  expm1(float x)  { NAME_WRAP(expm1_fpu_f)(&x); return x; }
SYS_FORCE_INLINE double expm1(double x) { NAME_WRAP(expm1_fpu_d)(&x); return x; }
SYS_FORCE_INLINE float  exp(float x)    { NAME_WRAP(exp_fpu_f)(&x);   return x; }
SYS_FORCE_INLINE double exp(double x)   { NAME_WRAP(exp_fpu_d)(&x);   return x; }
SYS_FORCE_INLINE float  exp2(float x)   { NAME_WRAP(exp2_fpu_f)(&x);  return x; }
SYS_FORCE_INLINE double exp2(double x)  { NAME_WRAP(exp2_fpu_d)(&x);  return x; }
SYS_FORCE_INLINE float  pow(float base, float exponent)  { float  in[2] = {base,exponent}; float out;  NAME_WRAP(pow_fpu_f)(in,&out); return out; }
SYS_FORCE_INLINE double pow(double base,double exponent) { double in[2] = {base,exponent}; double out; NAME_WRAP(pow_fpu_d)(in,&out); return out; }
SYS_FORCE_INLINE float  tanh(float x)   { NAME_WRAP(tanh_fpu_f)(&x);  return x; }
SYS_FORCE_INLINE double tanh(double x)  { NAME_WRAP(tanh_fpu_d)(&x);  return x; }
}

#undef NAME_WRAP
