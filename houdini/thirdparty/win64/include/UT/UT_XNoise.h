#ifndef __UT_XNoise__
#define __UT_XNoise__

#include "UT_API.h"
#include <VM/VM_SIMD.h>
#include <SYS/SYS_Types.h>

class UT_API UT_XNoise
{
public:
    ///
    /// Initialises the noise function. Must be called before invoking any
    /// other functions.
    ///
    static void initNoise();

    ///
    /// The internal permutation and random tables and size. We can pass this
    /// buffer to OpenCL functions in $SHS/ocl/include/xnoise.h.
    static void *data();
    ///
    /// size of data() in bytes.
    ///
    static size_t dataSize();

    ///
    /// Noise function naming schemes:
    ///     noise(Output dim)D(Input dim)D[Periodic]
    ///
    static fpreal32
    noise1D1D(fpreal32 x);
    static fpreal32
    noise1D1DP(fpreal32 x,
               uint32 const xp);
    static fpreal32
    noise1D1D(fpreal32& dx,
              fpreal32 x);
    static fpreal32
    noise1D1DP(fpreal32& dx,
               fpreal32 x,
               uint32 const xp);
    static fpreal32
    noise1D2D(fpreal32 x, fpreal32 y);
    static fpreal32
    noise1D2DP(fpreal32 x, fpreal32 y,
               uint32 const xp, uint32 const yp);
    static fpreal32
    noise1D2D(fpreal32& dx, fpreal32& dy,
              fpreal32 x, fpreal32 y);
    static fpreal32
    noise1D2DP(fpreal32& dx, fpreal32& dy,
               fpreal32 x, fpreal32 y,
               uint32 const xp, uint32 const yp);
    static fpreal32
    noise1D3D(fpreal32 x, fpreal32 y, fpreal32 z);
    static fpreal32
    noise1D3DP(fpreal32 x, fpreal32 y, fpreal32 z,
               uint32 const xp, uint32 const yp, uint32 const zp);
    static fpreal32
    noise1D3D(fpreal32& dx, fpreal32& dy, fpreal32& dz,
              fpreal32 x, fpreal32 y, fpreal32 z);
    static fpreal32
    noise1D3DP(fpreal32& dx, fpreal32& dy, fpreal32& dz,
               fpreal32 x, fpreal32 y, fpreal32 z,
               uint32 const xp, uint32 const yp, uint32 const zp);
    static fpreal32
    noise1D4D(fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w);
    static fpreal32
    noise1D4DP(fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w,
               uint32 const xp, uint32 const yp, uint32 const zp, uint32 const wp);
    static fpreal32
    noise1D4D(fpreal32& dx, fpreal32& dy, fpreal32& dz, fpreal32& dw,
              fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w);
    static fpreal32
    noise1D4DP(fpreal32& dx, fpreal32& dy, fpreal32& dz, fpreal32& dw,
               fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w,
               uint32 const xp, uint32 const yp, uint32 const zp, uint32 const wp);
    static void
    noise3D1D(fpreal32 f[3],
              fpreal32 x);
    static void
    noise3D1DP(fpreal32 f[3],
               fpreal32 x,
               uint32 const xp);
    static void
    noise3D1D(fpreal32 f[3],
              fpreal32 dx[3],
              fpreal32 x);
    static void
    noise3D1DP(fpreal32 f[3],
               fpreal32 dx[3],
               fpreal32 x,
               uint32 const xp);
    static void
    noise3D2D(fpreal32 f[3],
              fpreal32 x, fpreal32 y);
    static void
    noise3D2DP(fpreal32 f[3],
               fpreal32 x, fpreal32 y,
               uint32 const xp, uint32 const yp);
    static void
    noise3D2D(fpreal32 f[3],
              fpreal32 dx[3], fpreal32 dy[3],
              fpreal32 x, fpreal32 y);
    static void
    noise3D2DP(fpreal32 f[3],
               fpreal32 dx[3], fpreal32 dy[3],
               fpreal32 x, fpreal32 y,
               uint32 const xp, uint32 const yp);
    static void
    noise3D3D(fpreal32 f[3],
              fpreal32 x, fpreal32 y, fpreal32 z);
    static void
    noise3D3DP(fpreal32 f[3],
               fpreal32 x, fpreal32 y, fpreal32 z,
               uint32 const xp, uint32 const yp, uint32 const zp);
    static void
    noise3D3D(fpreal32 f[3],
              fpreal32 dx[3], fpreal32 dy[3], fpreal32 dz[3],
              fpreal32 x, fpreal32 y, fpreal32 z);
    static void
    noise3D3DP(fpreal32 f[3],
               fpreal32 dx[3], fpreal32 dy[3], fpreal32 dz[3],
               fpreal32 x, fpreal32 y, fpreal32 z,
               uint32 const xp, uint32 const yp, uint32 const zp);
    static void
    noise3D4D(fpreal32 f[3],
              fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w);
    static void
    noise3D4DP(fpreal32 f[3],
               fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w,
               uint32 const xp, uint32 const yp, uint32 const zp, uint32 const wp);
    static void
    noise3D4D(fpreal32 f[3],
              fpreal32 dx[3], fpreal32 dy[3], fpreal32 dz[3], fpreal32 dw[3],
              fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w);
    static void
    noise3D4DP(fpreal32 f[3],
               fpreal32 dx[3], fpreal32 dy[3], fpreal32 dz[3], fpreal32 dw[3],
               fpreal32 x, fpreal32 y, fpreal32 z, fpreal32 w,
               uint32 const xp, uint32 const yp, uint32 const zp, uint32 const wp);
    static fpreal64
    noise1D1D(fpreal64 x);
    static fpreal64
    noise1D1DP(fpreal64 x,
               uint64 const xp);
    static fpreal64
    noise1D1D(fpreal64& dx,
              fpreal64 x);
    static fpreal64
    noise1D1DP(fpreal64& dx,
               fpreal64 x,
               uint64 const xp);
    static fpreal64
    noise1D2D(fpreal64 x, fpreal64 y);
    static fpreal64
    noise1D2DP(fpreal64 x, fpreal64 y,
               uint64 const xp, uint64 const yp);
    static fpreal64
    noise1D2D(fpreal64& dx, fpreal64& dy,
              fpreal64 x, fpreal64 y);
    static fpreal64
    noise1D2DP(fpreal64& dx, fpreal64& dy,
               fpreal64 x, fpreal64 y,
               uint64 const xp, uint64 const yp);
    static fpreal64
    noise1D3D(fpreal64 x, fpreal64 y, fpreal64 z);
    static fpreal64
    noise1D3DP(fpreal64 x, fpreal64 y, fpreal64 z,
               uint64 const xp, uint64 const yp, uint64 const zp);
    static fpreal64
    noise1D3D(fpreal64& dx, fpreal64& dy, fpreal64& dz,
              fpreal64 x, fpreal64 y, fpreal64 z);
    static fpreal64
    noise1D3DP(fpreal64& dx, fpreal64& dy, fpreal64& dz,
               fpreal64 x, fpreal64 y, fpreal64 z,
               uint64 const xp, uint64 const yp, uint64 const zp);
    static fpreal64
    noise1D4D(fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w);
    static fpreal64
    noise1D4DP(fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w,
               uint64 const xp, uint64 const yp, uint64 const zp, uint64 const wp);
    static fpreal64
    noise1D4D(fpreal64& dx, fpreal64& dy, fpreal64& dz, fpreal64& dw,
              fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w);
    static fpreal64
    noise1D4DP(fpreal64& dx, fpreal64& dy, fpreal64& dz, fpreal64& dw,
               fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w,
               uint64 const xp, uint64 const yp, uint64 const zp, uint64 const wp);
    static void
    noise3D1D(fpreal64 f[3],
              fpreal64 x);
    static void
    noise3D1DP(fpreal64 f[3],
               fpreal64 x,
               uint64 const xp);
    static void
    noise3D1D(fpreal64 f[3],
              fpreal64 dx[3],
              fpreal64 x);
    static void
    noise3D1DP(fpreal64 f[3],
               fpreal64 dx[3],
               fpreal64 x,
               uint64 const xp);
    static void
    noise3D2D(fpreal64 f[3],
              fpreal64 x, fpreal64 y);
    static void
    noise3D2DP(fpreal64 f[3],
               fpreal64 x, fpreal64 y,
               uint64 const xp, uint64 const yp);
    static void
    noise3D2D(fpreal64 f[3],
              fpreal64 dx[3], fpreal64 dy[3],
              fpreal64 x, fpreal64 y);
    static void
    noise3D2DP(fpreal64 f[3],
               fpreal64 dx[3], fpreal64 dy[3],
               fpreal64 x, fpreal64 y,
               uint64 const xp, uint64 const yp);
    static void
    noise3D3D(fpreal64 f[3],
              fpreal64 x, fpreal64 y, fpreal64 z);
    static void
    noise3D3DP(fpreal64 f[3],
               fpreal64 x, fpreal64 y, fpreal64 z,
               uint64 const xp, uint64 const yp, uint64 const zp);
    static void
    noise3D3D(fpreal64 f[3],
              fpreal64 dx[3], fpreal64 dy[3], fpreal64 dz[3],
              fpreal64 x, fpreal64 y, fpreal64 z);
    static void
    noise3D3DP(fpreal64 f[3],
               fpreal64 dx[3], fpreal64 dy[3], fpreal64 dz[3],
               fpreal64 x, fpreal64 y, fpreal64 z,
               uint64 const xp, uint64 const yp, uint64 const zp);
    static void
    noise3D4D(fpreal64 f[3],
              fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w);
    static void
    noise3D4DP(fpreal64 f[3],
               fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w,
               uint64 const xp, uint64 const yp, uint64 const zp, uint64 const wp);
    static void
    noise3D4D(fpreal64 f[3],
              fpreal64 dx[3], fpreal64 dy[3], fpreal64 dz[3], fpreal64 dw[3],
              fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w);
    static void
    noise3D4DP(fpreal64 f[3],
               fpreal64 dx[3], fpreal64 dy[3], fpreal64 dz[3], fpreal64 dw[3],
               fpreal64 x, fpreal64 y, fpreal64 z, fpreal64 w,
               uint64 const xp, uint64 const yp, uint64 const zp, uint64 const wp);

    static v4uf
    noise1D1D(v4uf x);
    static v4uf
    noise1D1DP(v4uf x,
               v4uu const xp);
    static v4uf
    noise1D1D(v4uf& dx,
              v4uf x);
    static v4uf
    noise1D1DP(v4uf& dx,
               v4uf x,
               v4uu const xp);
    static void
    noise3D1D(v4uf f[3],
              v4uf x);
    static void
    noise3D1DP(v4uf f[3],
               v4uf x,
               v4uu const xp);
    static void
    noise3D1D(v4uf f[3],
              v4uf dx[3],
              v4uf x);
    static void
    noise3D1DP(v4uf f[3],
               v4uf dx[3],
               v4uf x,
               v4uu const xp);

    // Curlnoise functions
    // 2D curlnoise from scalar noise
    static void		curlnoise2D2D(fpreal32 f[3], fpreal32 x, fpreal32 y);
    static void		curlnoise2D2D(fpreal64 f[3], fpreal64 x, fpreal64 y);
    //static void		curlnoise2D2D(v4uf n[3], const v4uf& x, const v4uf& y);
    static void		curlnoise2D3D(fpreal32 f[3], const fpreal32 x[3]);
    static void		curlnoise2D3D(fpreal64 f[3], const fpreal64 x[3]);
    // 3D curlnoise from vector noise
    static void		curlnoise3D3D(fpreal32 f[3], const fpreal32 x[3]);
    static void		curlnoise3D3D(fpreal64 f[3], const fpreal64 x[3]);
    static void		curlnoise3D4D(fpreal32 f[3], const fpreal32 x[4]);
    static void		curlnoise3D4D(fpreal64 f[3], const fpreal64 x[4]);
};

#endif // !__UT_XNoise__
