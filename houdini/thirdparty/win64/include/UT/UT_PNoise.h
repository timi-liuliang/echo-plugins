/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PNoise.h ( UT Library, C++)
 *
 * COMMENTS:	New, nicely bounded noise functions.
 *		Allows for periodicity of noise as well.
 */

#ifndef __UT_PNoise__
#define __UT_PNoise__

#include "UT_API.h"

#include <VM/VM_SIMD.h>
#include <SYS/SYS_Types.h>

#include "UT_VectorTypes.h"

class UT_API UT_PNoise
{
public:
    static void		initNoise();

    // Duplicated verbatim for fpreal64 and v4uf
    
    // Noise
    static fpreal32 at1D(fpreal32 const x);
    static fpreal32 at2D(fpreal32 const x, fpreal32 const y);
    static fpreal32 at3D(fpreal32 const x, fpreal32 const y, fpreal32 const z);
    static fpreal32 at4D(fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w);

    // Noise, Derivatives
    static fpreal32 at1D(fpreal32& dfdx, fpreal32 const x);
    static fpreal32 at2D(fpreal32& dfdx, fpreal32& dfdy,
                         fpreal32 const x, fpreal32 const y);
    static fpreal32 at3D(fpreal32& dfdx, fpreal32& dfdy, fpreal32& dfdz,
                         fpreal32 const x, fpreal32 const y, fpreal32 const z);
    static fpreal32 at4D(fpreal32& dfdx, fpreal32& dfdy, fpreal32& dfdz, fpreal32& dfdw,
                         fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w);

    // Noise, Vector
    static void at1D(fpreal32 f[3], fpreal32 const x);
    static void at2D(fpreal32 f[3],
                        fpreal32 const x, fpreal32 const y);
    static void at3D(fpreal32 f[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z);
    static void at4D(fpreal32 f[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w);

    // Noise, Vector, with derivatives
    static void at1D(fpreal32 f[3],
                        fpreal32 dfdx[3],
                        fpreal32 const x);
    static void at2D(fpreal32 f[3],
                        fpreal32 dfdx[3], fpreal32 dfdy[3],
                        fpreal32 const x, fpreal32 const y);
    static void at3D(fpreal32 f[3],
                        fpreal32 dfdx[3], fpreal32 dfdy[3], fpreal32 dfdz[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z);
    static void at4D(fpreal32 f[3],
                        fpreal32 dfdx[3], fpreal32 dfdy[3], fpreal32 dfdz[3], fpreal32 dfdw[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w);

    // Periodic Noise
    static fpreal32 at1DP(fpreal32 const x,
                         uint32 const px);
    static fpreal32 at2DP(fpreal32 const x, fpreal32 const y,
                         uint32 const px, uint32 const py);
    static fpreal32 at3DP(fpreal32 const x, fpreal32 const y, fpreal32 const z,
                         uint32 const px, uint32 const py, uint32 const pz);
    static fpreal32 at4DP(fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w,
                         uint32 const px, uint32 const py, uint32 const pz, uint32 const pw);

    // Periodic Noise, Derivatives
    static fpreal32 at1DP(fpreal32& dfdx, fpreal32 const x,
                         uint32 const px);
    static fpreal32 at2DP(fpreal32& dfdx, fpreal32& dfdy,
                         fpreal32 const x, fpreal32 const y,
                         uint32 const px, uint32 const py);
    static fpreal32 at3DP(fpreal32& dfdx, fpreal32& dfdy, fpreal32& dfdz,
                         fpreal32 const x, fpreal32 const y, fpreal32 const z,
                         uint32 const px, uint32 const py, uint32 const pz);
    static fpreal32 at4DP(fpreal32& dfdx, fpreal32& dfdy, fpreal32& dfdz, fpreal32& dfdw,
                         fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w,
                         uint32 const px, uint32 const py, uint32 const pz, uint32 const pw);

    // Periodic Noise, Vector
    static void at1DP(fpreal32 f[3], fpreal32 const x,
                        uint32 const px);
    static void at2DP(fpreal32 f[3],
                        fpreal32 const x, fpreal32 const y,
                        uint32 const px, uint32 const py);
    static void at3DP(fpreal32 f[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z,
                        uint32 const px, uint32 const py, uint32 const pz);
    static void at4DP(fpreal32 f[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w,
                        uint32 const px, uint32 const py, uint32 const pz, uint32 const pw);

    // Periodic Noise, Vector, with derivatives
    static void at1DP(fpreal32 f[3],
                        fpreal32 dfdx[3],
                        fpreal32 const x,
                        uint32 const px);
    static void at2DP(fpreal32 f[3],
                        fpreal32 dfdx[3], fpreal32 dfdy[3],
                        fpreal32 const x, fpreal32 const y,
                        uint32 const px, uint32 const py);
    static void at3DP(fpreal32 f[3],
                        fpreal32 dfdx[3], fpreal32 dfdy[3], fpreal32 dfdz[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z,
                        uint32 const px, uint32 const py, uint32 const pz);
    static void at4DP(fpreal32 f[3],
                        fpreal32 dfdx[3], fpreal32 dfdy[3], fpreal32 dfdz[3], fpreal32 dfdw[3],
                        fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w,
                        uint32 const px, uint32 const py, uint32 const pz, uint32 const pw);

    // Flowat
    
    // There is no 1D flow at.
    static fpreal32 flowat2D(fpreal32 const x, fpreal32 const y, fpreal32 const flow);
    static fpreal32 flowat3D(fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const flow);
    static fpreal32 flowat4D(fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w, fpreal32 const flow);

    // Flowat, Vector
    static void flowat2D(fpreal32 f[3],
                            fpreal32 const x, fpreal32 const y,
                            fpreal32 const flow);
    static void flowat3D(fpreal32 f[3],
                            fpreal32 const x, fpreal32 const y, fpreal32 const z,
                            fpreal32 const flow);
    static void flowat4D(fpreal32 f[3],
                            fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w,
                            fpreal32 const flow);

    // Periodic Flowat
    static fpreal32 flowat2DP(fpreal32 const x, fpreal32 const y,
                             uint32 const px, uint32 const py,
                             fpreal32 const flow);
    static fpreal32 flowat3DP(fpreal32 const x, fpreal32 const y, fpreal32 const z,
                             uint32 const px, uint32 const py, uint32 const pz,
                             fpreal32 const flow);
    static fpreal32 flowat4DP(fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w,
                             uint32 const px, uint32 const py, uint32 const pz, uint32 const pw,
                             fpreal32 const flow);

    // Periodic Flowat, Vectorial
    static void flowat2DP(fpreal32 f[3],
                            fpreal32 const x, fpreal32 const y,
                            uint32 const px, uint32 const py,
                            fpreal32 const flow);
    static void flowat3DP(fpreal32 f[3],
                            fpreal32 const x, fpreal32 const y, fpreal32 const z,
                            uint32 const px, uint32 const py, uint32 const pz,
                            fpreal32 const flow);
    static void flowat4DP(fpreal32 f[3],
                            fpreal32 const x, fpreal32 const y, fpreal32 const z, fpreal32 const w,
                            uint32 const px, uint32 const py, uint32 const pz, uint32 const pw,
                            fpreal32 const flow);

    // Curlat functions
    
    static void curlnoise2D2D(fpreal32 f[3],
                              fpreal32 const x,
                              fpreal32 const y);
    static void curlnoise2D3D(fpreal32 f[3],
                              fpreal32 const x,
                              fpreal32 const y,
                              fpreal32 const z);
    static void curlnoise3D3D(fpreal32 f[3],
                              fpreal32 const x,
                              fpreal32 const y,
                              fpreal32 const z);
    static void curlnoise3D4D(fpreal32 f[3],
                              fpreal32 const x,
                              fpreal32 const y,
                              fpreal32 const z,
                              fpreal32 const w);


    // Legacy compatibility
    
    // Noise
    static inline fpreal32 noise1D(fpreal32 const x)
    {
    	return at1D(x);
    }
    static inline fpreal32 noise2D(fpreal32 const x, fpreal32 const y)
    {
    	return at2D(x, y);
    }
    static inline fpreal32 noise2D(fpreal32 const x[2])
    {
	return at2D(x[0], x[1]);
    }
    static inline fpreal32 noise3D(fpreal32 const x[3])
    {
	return at3D(x[0], x[1], x[2]);
    }
    static inline fpreal32 noise4D(fpreal32 const x[4])
    {
	return at4D(x[0], x[1], x[2], x[3]);
    }
    // Noise, Derivatives
    static inline void noise1D(fpreal32 &n, fpreal32 &dndx, fpreal32 const x)
    {
	n = at1D(dndx, x);
    }
    static inline void noise2D(fpreal32 &n, fpreal32 &dndx, fpreal32 &dndy, const fpreal32 x[2])
    {
	n = at2D(dndx, dndy, x[0], x[1]);
    }
    static inline void noise2D(fpreal32 &n, fpreal32 dndx[2], const fpreal32 x[2])
    {
	n = at2D(dndx[0], dndx[1], x[0], x[1]);
    }
    static inline void noise2D(fpreal32 &n, fpreal32 &dndx, fpreal32 &dndy,
                               fpreal32 const x, fpreal32 const y)
    {
	n = at2D(dndx, dndy, x, y);
    }
    static inline void noise3D(fpreal32 &n, fpreal32 dndx[3], const fpreal32 x[3])
    {
	n = at3D(dndx[0], dndx[1], dndx[2],
	         x[0], x[1], x[2]);
    }
    static inline void noise3D(fpreal32 &n, fpreal32 &dndx,
                               fpreal32 &dndy, fpreal32 &dndz,
                               const fpreal32 x[3])
    {
	n = at3D( dndx, dndy, dndz, x[0], x[1], x[2]);
    }
    static inline void noise4D(fpreal32 &n, fpreal32 dndx[4],
                               const fpreal32 x[4])
    {
	n = at4D(dndx[0], dndx[1], dndx[2], dndx[3],
	         x[0], x[1], x[2], x[3]);
    }
    static inline void noise4D(fpreal32 &n, fpreal32 &dndx,
                               fpreal32 &dndy, fpreal32 &dndz, fpreal32 &dndw,
                               const fpreal32 x[4])
    {
	n = at4D(dndx, dndy, dndz, dndw, x[0], x[1], x[2], x[3]);
    }
    // Periodic Noise
    static inline fpreal32 noise1D(fpreal32 const x, uint32 const px)
    {
    	return at1DP(x, px);
    }
    static inline fpreal32 noise2D(fpreal32 const x[2], uint32 const per[2])
    {
	return at2DP(x[0], x[1], per[0], per[1]);
    }
    static inline fpreal32 noise2D(fpreal32 const x, fpreal32 const y,
                                   uint32 const px, uint32 const py)
    {
	return at2DP(x, y, px, py);
    }
    static inline fpreal32 noise3D(fpreal32 const x[3], uint32 const per[3])
    {
	return at3DP(x[0], x[1], x[2],
	               per[0], per[1], per[2]);
    }
    static inline fpreal32 noise4D(fpreal32 const x[4], uint32 const per[4])
    {
	return at4DP(x[0], x[1], x[2], x[3],
	               per[0], per[1], per[2], per[3]);
    }
    // Noise, Vector
    static inline void noise1D(fpreal32 f[3], fpreal32 const x)
    {
    	at1D(f, x);
    }
    static inline void noise2D(fpreal32 f[3],
                               fpreal32 const x, fpreal32 const y)
    {
    	at2D(f, x, y);
    }
    static inline void noise2D(fpreal32 f[3], fpreal32 const x[2])
    {
	at2D(f, x[0], x[1]);
    }
    static inline void noise3D(fpreal32 f[3], fpreal32 const x[3])
    {
	at3D(f, x[0], x[1], x[2]);
    }
    static inline void noise4D(fpreal32 f[3], fpreal32 const x[4])
    {
	at4D(f, x[0], x[1], x[2], x[3]);
    }
    // Noise, Vector, Derivative
    static inline void noise1D(fpreal32 f[3], fpreal32 dfdx[3], fpreal32 const x)
    {
	at1D(f, dfdx, x);
    }
    static inline void noise2D(fpreal32 f[3], fpreal32 dfdx[3], fpreal32 dfdy[3], fpreal32 const x[2])
    {
	at2D(f, dfdx, dfdy,
	        x[0], x[1]);
    }
    static inline void noise2D(fpreal32 f[3],
                               fpreal32 dfdx[3], fpreal32 dfdy[3],
                               fpreal32 const x, fpreal32 y)
    {
	at2D(f, dfdx, dfdy, x, y);
    }
    static inline void noise3D(fpreal32 f[3], fpreal32 dfdx[3], fpreal32 dfdy[3], fpreal32 dfdz[3], fpreal32 const x[3])
    {
	at3D(f, dfdx, dfdy, dfdz,
	        x[0], x[1], x[2]);
    }
    static inline void noise4D(fpreal32 f[3], fpreal32 dfdx[3], fpreal32 dfdy[3], fpreal32 dfdz[3], fpreal32 dfdw[3], fpreal32 const x[4])
    {
	at4D(f, dfdx, dfdy, dfdz, dfdw,
	        x[0], x[1], x[2], x[3]);
    }
    // Periodic Noise, Vector
    static inline void noise1D(fpreal32 f[3], fpreal32 const x, uint32 const px)
    {
    	at1DP(f, x, px);
    }
    static inline void noise2D(fpreal32 f[3],
                               fpreal32 const x, fpreal32 const y,
                               uint32 const px, uint32 const py)
    {
    	at2DP(f, x, y, px, py);
    }
    static inline void noise2D(fpreal32 f[3], fpreal32 const x[2], uint32 const per[2])
    {
	at2DP(f, x[0], x[1],
	        per[0], per[1]);
    }
    static inline void noise3D(fpreal32 f[3], fpreal32 const x[3], uint32 const per[3])
    {
	at3DP(f, x[0], x[1], x[2],
	        per[0], per[1], per[2]);
    }
    static inline void noise4D(fpreal32 f[3], fpreal32 const x[4], uint32 const per[4])
    {
	at4DP(f, x[0], x[1], x[2], x[3],
	        per[0], per[1], per[2], per[3]);
    }

    // Flow noise

    static inline fpreal32 flownoise2D(fpreal32 const x, fpreal32 const y,
                                       fpreal32 const flow)
    {
    	return flowat2D(x, y, flow);
    }
    static inline fpreal32 flownoise2D(fpreal32 const x[2], fpreal32 const flow)
    {
    	return flowat2D(x[0], x[1], flow);
    }
    static inline fpreal32 flownoise3D(fpreal32 const x[3], fpreal32 const flow)
    {
    	return flowat3D(x[0], x[1], x[2], flow);
    }
    static inline fpreal32 flownoise4D(fpreal32 const x[4], fpreal32 const flow)
    {
    	return flowat4D(x[0], x[1], x[2], x[3], flow);
    }

    // Flow noise, Periodic
    static inline fpreal32 flownoise2D(fpreal32 const x[2], uint32 const per[2],
                                    fpreal32 const flow)
    {
    	return flowat2DP(x[0], x[1], per[0], per[1], flow);
    }
    static inline fpreal32 flownoise2D(fpreal32 const x, fpreal32 y,
                                       uint32 const px, uint32 const py,
                                       fpreal32 const flow)
    {
    	return flowat2DP(x, y, px, py, flow);
    }
    static inline fpreal32 flownoise3D(fpreal32 const x[3], uint32 const per[3],
                                    fpreal32 const flow)
    {
    	return flowat3DP(x[0], x[1], x[2],
    	                   per[0], per[1], per[2], flow);
    }
    static inline fpreal32 flownoise4D(fpreal32 const x[4], uint32 const per[4],
                                    fpreal32 const flow)
    {
    	return flowat4DP(x[0], x[1], x[2], x[3],
    	                   per[0], per[1], per[2], per[3],
    	                   flow);
    }
    // Flow noise, Vector
    static inline void flownoise2D(fpreal32 f[3],
                                   fpreal32 const x, fpreal32 const y,
                                   fpreal32 const flow)
    {
    	flowat2D(f, x, y, flow);
    }
    static inline void flownoise2D(fpreal32 f[3],
                                   fpreal32 const x[2], fpreal32 const flow)
    {
    	flowat2D(f, x[0], x[1], flow);
    }
    static inline void flownoise3D(fpreal32 f[3],
                                   fpreal32 const x[3], fpreal32 const flow)
    {
    	flowat3D(f, x[0], x[1], x[2], flow);
    }
    static inline void flownoise4D(fpreal32 f[3],
                                   fpreal32 const x[4], fpreal32 const flow)
    {
    	flowat4D(f, x[0], x[1], x[2], x[3], flow);
    }
    // Flow noise, Vector, Periodic
    static inline void flownoise2D(fpreal32 f[3],
                                   fpreal32 const x, fpreal32 const y,
                                   uint32 const px, uint32 const py,
                                   fpreal32 const flow)
    {
    	flowat2DP(f, x, y, px, py, flow);
    }
    static inline void flownoise2D(fpreal32 f[3],
                                   fpreal32 const x[2], uint32 const period[2],
                                   fpreal32 const flow)
    {
    	flowat2DP(f, x[0], x[1], period[0], period[1], flow);
    }
    static inline void flownoise3D(fpreal32 f[3],
                                   fpreal32 const x[3], uint32 const period[3],
                                   fpreal32 const flow)
    {
    	flowat3DP(f, x[0], x[1], x[2],
    	            period[0], period[1], period[2], flow);
    }
    static inline void flownoise4D(fpreal32 f[3],
                                   fpreal32 const x[4], uint32 const period[4],
                                   fpreal32 const flow)
    {
    	flowat4DP(f, x[0], x[1], x[2], x[3],
    	            period[0], period[1], period[2], period[3], flow);
    }

    static inline void curlnoise2D3D(fpreal32 f[3], const fpreal32 x[3])
    {
    	curlnoise2D3D(f, x[0], x[1], x[2]);
    }
    static inline void curlnoise3D3D(fpreal32 f[3], const fpreal32 x[3])
    {
    	curlnoise3D3D(f, x[0], x[1], x[2]);
    }
    static inline void curlnoise3D4D(fpreal32 f[3], const fpreal32 x[4])
    {
    	curlnoise3D4D(f, x[0], x[1], x[2], x[3]);
    }

    // Noise
    static fpreal64 at1D(fpreal64 const x);
    static fpreal64 at2D(fpreal64 const x, fpreal64 const y);
    static fpreal64 at3D(fpreal64 const x, fpreal64 const y, fpreal64 const z);
    static fpreal64 at4D(fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w);

    // Noise, Derivatives
    static fpreal64 at1D(fpreal64& dfdx, fpreal64 const x);
    static fpreal64 at2D(fpreal64& dfdx, fpreal64& dfdy,
                         fpreal64 const x, fpreal64 const y);
    static fpreal64 at3D(fpreal64& dfdx, fpreal64& dfdy, fpreal64& dfdz,
                         fpreal64 const x, fpreal64 const y, fpreal64 const z);
    static fpreal64 at4D(fpreal64& dfdx, fpreal64& dfdy, fpreal64& dfdz, fpreal64& dfdw,
                         fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w);

    // Noise, Vector
    static void at1D(fpreal64 f[3], fpreal64 const x);
    static void at2D(fpreal64 f[3],
                        fpreal64 const x, fpreal64 const y);
    static void at3D(fpreal64 f[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z);
    static void at4D(fpreal64 f[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w);

    // Noise, Vector, with derivatives
    static void at1D(fpreal64 f[3],
                        fpreal64 dfdx[3],
                        fpreal64 const x);
    static void at2D(fpreal64 f[3],
                        fpreal64 dfdx[3], fpreal64 dfdy[3],
                        fpreal64 const x, fpreal64 const y);
    static void at3D(fpreal64 f[3],
                        fpreal64 dfdx[3], fpreal64 dfdy[3], fpreal64 dfdz[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z);
    static void at4D(fpreal64 f[3],
                        fpreal64 dfdx[3], fpreal64 dfdy[3], fpreal64 dfdz[3], fpreal64 dfdw[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w);

    // Periodic Noise
    static fpreal64 at1DP(fpreal64 const x,
                         uint64 const px);
    static fpreal64 at2DP(fpreal64 const x, fpreal64 const y,
                         uint64 const px, uint64 const py);
    static fpreal64 at3DP(fpreal64 const x, fpreal64 const y, fpreal64 const z,
                         uint64 const px, uint64 const py, uint64 const pz);
    static fpreal64 at4DP(fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w,
                         uint64 const px, uint64 const py, uint64 const pz, uint64 const pw);

    // Periodic Noise, Derivatives
    static fpreal64 at1DP(fpreal64& dfdx, fpreal64 const x,
                         uint64 const px);
    static fpreal64 at2DP(fpreal64& dfdx, fpreal64& dfdy,
                         fpreal64 const x, fpreal64 const y,
                         uint64 const px, uint64 const py);
    static fpreal64 at3DP(fpreal64& dfdx, fpreal64& dfdy, fpreal64& dfdz,
                         fpreal64 const x, fpreal64 const y, fpreal64 const z,
                         uint64 const px, uint64 const py, uint64 const pz);
    static fpreal64 at4DP(fpreal64& dfdx, fpreal64& dfdy, fpreal64& dfdz, fpreal64& dfdw,
                         fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w,
                         uint64 const px, uint64 const py, uint64 const pz, uint64 const pw);

    // Periodic Noise, Vector
    static void at1DP(fpreal64 f[3], fpreal64 const x,
                        uint64 const px);
    static void at2DP(fpreal64 f[3],
                        fpreal64 const x, fpreal64 const y,
                        uint64 const px, uint64 const py);
    static void at3DP(fpreal64 f[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z,
                        uint64 const px, uint64 const py, uint64 const pz);
    static void at4DP(fpreal64 f[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w,
                        uint64 const px, uint64 const py, uint64 const pz, uint64 const pw);

    // Periodic Noise, Vector, with derivatives
    static void at1DP(fpreal64 f[3],
                        fpreal64 dfdx[3],
                        fpreal64 const x,
                        uint64 const px);
    static void at2DP(fpreal64 f[3],
                        fpreal64 dfdx[3], fpreal64 dfdy[3],
                        fpreal64 const x, fpreal64 const y,
                        uint64 const px, uint64 const py);
    static void at3DP(fpreal64 f[3],
                        fpreal64 dfdx[3], fpreal64 dfdy[3], fpreal64 dfdz[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z,
                        uint64 const px, uint64 const py, uint64 const pz);
    static void at4DP(fpreal64 f[3],
                        fpreal64 dfdx[3], fpreal64 dfdy[3], fpreal64 dfdz[3], fpreal64 dfdw[3],
                        fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w,
                        uint64 const px, uint64 const py, uint64 const pz, uint64 const pw);

    // Flowat
    
    // There is no 1D flow at.
    static fpreal64 flowat2D(fpreal64 const x, fpreal64 const y, fpreal64 const flow);
    static fpreal64 flowat3D(fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const flow);
    static fpreal64 flowat4D(fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w, fpreal64 const flow);

    // Flowat, Vector
    static void flowat2D(fpreal64 f[3],
                            fpreal64 const x, fpreal64 const y,
                            fpreal64 const flow);
    static void flowat3D(fpreal64 f[3],
                            fpreal64 const x, fpreal64 const y, fpreal64 const z,
                            fpreal64 const flow);
    static void flowat4D(fpreal64 f[3],
                            fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w,
                            fpreal64 const flow);

    // Periodic Flowat
    static fpreal64 flowat2DP(fpreal64 const x, fpreal64 const y,
                             uint64 const px, uint64 const py,
                             fpreal64 const flow);
    static fpreal64 flowat3DP(fpreal64 const x, fpreal64 const y, fpreal64 const z,
                             uint64 const px, uint64 const py, uint64 const pz,
                             fpreal64 const flow);
    static fpreal64 flowat4DP(fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w,
                             uint64 const px, uint64 const py, uint64 const pz, uint64 const pw,
                             fpreal64 const flow);

    // Periodic Flowat, Vectorial
    static void flowat2DP(fpreal64 f[3],
                            fpreal64 const x, fpreal64 const y,
                            uint64 const px, uint64 const py,
                            fpreal64 const flow);
    static void flowat3DP(fpreal64 f[3],
                            fpreal64 const x, fpreal64 const y, fpreal64 const z,
                            uint64 const px, uint64 const py, uint64 const pz,
                            fpreal64 const flow);
    static void flowat4DP(fpreal64 f[3],
                            fpreal64 const x, fpreal64 const y, fpreal64 const z, fpreal64 const w,
                            uint64 const px, uint64 const py, uint64 const pz, uint64 const pw,
                            fpreal64 const flow);

    // Curlat functions
    
    static void curlnoise2D2D(fpreal64 f[3],
                              fpreal64 const x,
                              fpreal64 const y);
    static void curlnoise2D3D(fpreal64 f[3],
                              fpreal64 const x,
                              fpreal64 const y,
                              fpreal64 const z);
    static void curlnoise3D3D(fpreal64 f[3],
                              fpreal64 const x,
                              fpreal64 const y,
                              fpreal64 const z);
    static void curlnoise3D4D(fpreal64 f[3],
                              fpreal64 const x,
                              fpreal64 const y,
                              fpreal64 const z,
                              fpreal64 const w);


    // Legacy compatibility
    
    // Noise
    static inline fpreal64 noise1D(fpreal64 const x)
    {
    	return at1D(x);
    }
    static inline fpreal64 noise2D(fpreal64 const x, fpreal64 const y)
    {
    	return at2D(x, y);
    }
    static inline fpreal64 noise2D(fpreal64 const x[2])
    {
	return at2D(x[0], x[1]);
    }
    static inline fpreal64 noise3D(fpreal64 const x[3])
    {
	return at3D(x[0], x[1], x[2]);
    }
    static inline fpreal64 noise4D(fpreal64 const x[4])
    {
	return at4D(x[0], x[1], x[2], x[3]);
    }
    // Noise, Derivatives
    static inline void noise1D(fpreal64 &n, fpreal64 &dndx, fpreal64 const x)
    {
	n = at1D(dndx, x);
    }
    static inline void noise2D(fpreal64 &n, fpreal64 &dndx, fpreal64 &dndy, const fpreal64 x[2])
    {
	n = at2D(dndx, dndy, x[0], x[1]);
    }
    static inline void noise2D(fpreal64 &n, fpreal64 dndx[2], const fpreal64 x[2])
    {
	n = at2D(dndx[0], dndx[1], x[0], x[1]);
    }
    static inline void noise2D(fpreal64 &n, fpreal64 &dndx, fpreal64 &dndy,
                               fpreal64 const x, fpreal64 const y)
    {
	n = at2D(dndx, dndy, x, y);
    }
    static inline void noise3D(fpreal64 &n, fpreal64 dndx[3], const fpreal64 x[3])
    {
	n = at3D(dndx[0], dndx[1], dndx[2],
	         x[0], x[1], x[2]);
    }
    static inline void noise3D(fpreal64 &n, fpreal64 &dndx,
                               fpreal64 &dndy, fpreal64 &dndz,
                               const fpreal64 x[3])
    {
	n = at3D( dndx, dndy, dndz, x[0], x[1], x[2]);
    }
    static inline void noise4D(fpreal64 &n, fpreal64 dndx[4],
                               const fpreal64 x[4])
    {
	n = at4D(dndx[0], dndx[1], dndx[2], dndx[3],
	         x[0], x[1], x[2], x[3]);
    }
    static inline void noise4D(fpreal64 &n, fpreal64 &dndx,
                               fpreal64 &dndy, fpreal64 &dndz, fpreal64 &dndw,
                               const fpreal64 x[4])
    {
	n = at4D(dndx, dndy, dndz, dndw, x[0], x[1], x[2], x[3]);
    }
    // Periodic Noise
    static inline fpreal64 noise1D(fpreal64 const x, uint64 const px)
    {
    	return at1DP(x, px);
    }
    static inline fpreal64 noise2D(fpreal64 const x[2], uint64 const per[2])
    {
	return at2DP(x[0], x[1], per[0], per[1]);
    }
    static inline fpreal64 noise2D(fpreal64 const x, fpreal64 const y,
                                   uint64 const px, uint64 const py)
    {
	return at2DP(x, y, px, py);
    }
    static inline fpreal64 noise3D(fpreal64 const x[3], uint64 const per[3])
    {
	return at3DP(x[0], x[1], x[2],
	               per[0], per[1], per[2]);
    }
    static inline fpreal64 noise4D(fpreal64 const x[4], uint64 const per[4])
    {
	return at4DP(x[0], x[1], x[2], x[3],
	               per[0], per[1], per[2], per[3]);
    }
    // Noise, Vector
    static inline void noise1D(fpreal64 f[3], fpreal64 const x)
    {
    	at1D(f, x);
    }
    static inline void noise2D(fpreal64 f[3],
                               fpreal64 const x, fpreal64 const y)
    {
    	at2D(f, x, y);
    }
    static inline void noise2D(fpreal64 f[3], fpreal64 const x[2])
    {
	at2D(f, x[0], x[1]);
    }
    static inline void noise3D(fpreal64 f[3], fpreal64 const x[3])
    {
	at3D(f, x[0], x[1], x[2]);
    }
    static inline void noise4D(fpreal64 f[3], fpreal64 const x[4])
    {
	at4D(f, x[0], x[1], x[2], x[3]);
    }
    // Noise, Vector, Derivative
    static inline void noise1D(fpreal64 f[3], fpreal64 dfdx[3], fpreal64 const x)
    {
	at1D(f, dfdx, x);
    }
    static inline void noise2D(fpreal64 f[3], fpreal64 dfdx[3], fpreal64 dfdy[3], fpreal64 const x[2])
    {
	at2D(f, dfdx, dfdy,
	        x[0], x[1]);
    }
    static inline void noise2D(fpreal64 f[3],
                               fpreal64 dfdx[3], fpreal64 dfdy[3],
                               fpreal64 const x, fpreal64 y)
    {
	at2D(f, dfdx, dfdy, x, y);
    }
    static inline void noise3D(fpreal64 f[3], fpreal64 dfdx[3], fpreal64 dfdy[3], fpreal64 dfdz[3], fpreal64 const x[3])
    {
	at3D(f, dfdx, dfdy, dfdz,
	        x[0], x[1], x[2]);
    }
    static inline void noise4D(fpreal64 f[3], fpreal64 dfdx[3], fpreal64 dfdy[3], fpreal64 dfdz[3], fpreal64 dfdw[3], fpreal64 const x[4])
    {
	at4D(f, dfdx, dfdy, dfdz, dfdw,
	        x[0], x[1], x[2], x[3]);
    }
    // Periodic Noise, Vector
    static inline void noise1D(fpreal64 f[3], fpreal64 const x, uint64 const px)
    {
    	at1DP(f, x, px);
    }
    static inline void noise2D(fpreal64 f[3],
                               fpreal64 const x, fpreal64 const y,
                               uint64 const px, uint64 const py)
    {
    	at2DP(f, x, y, px, py);
    }
    static inline void noise2D(fpreal64 f[3], fpreal64 const x[2], uint64 const per[2])
    {
	at2DP(f, x[0], x[1],
	        per[0], per[1]);
    }
    static inline void noise3D(fpreal64 f[3], fpreal64 const x[3], uint64 const per[3])
    {
	at3DP(f, x[0], x[1], x[2],
	        per[0], per[1], per[2]);
    }
    static inline void noise4D(fpreal64 f[3], fpreal64 const x[4], uint64 const per[4])
    {
	at4DP(f, x[0], x[1], x[2], x[3],
	        per[0], per[1], per[2], per[3]);
    }

    // Flow noise

    static inline fpreal64 flownoise2D(fpreal64 const x, fpreal64 const y,
                                       fpreal64 const flow)
    {
    	return flowat2D(x, y, flow);
    }
    static inline fpreal64 flownoise2D(fpreal64 const x[2], fpreal64 const flow)
    {
    	return flowat2D(x[0], x[1], flow);
    }
    static inline fpreal64 flownoise3D(fpreal64 const x[3], fpreal64 const flow)
    {
    	return flowat3D(x[0], x[1], x[2], flow);
    }
    static inline fpreal64 flownoise4D(fpreal64 const x[4], fpreal64 const flow)
    {
    	return flowat4D(x[0], x[1], x[2], x[3], flow);
    }

    // Flow noise, Periodic
    static inline fpreal64 flownoise2D(fpreal64 const x[2], uint64 const per[2],
                                    fpreal64 const flow)
    {
    	return flowat2DP(x[0], x[1], per[0], per[1], flow);
    }
    static inline fpreal64 flownoise2D(fpreal64 const x, fpreal64 y,
                                       uint64 const px, uint64 const py,
                                       fpreal64 const flow)
    {
    	return flowat2DP(x, y, px, py, flow);
    }
    static inline fpreal64 flownoise3D(fpreal64 const x[3], uint64 const per[3],
                                    fpreal64 const flow)
    {
    	return flowat3DP(x[0], x[1], x[2],
    	                   per[0], per[1], per[2], flow);
    }
    static inline fpreal64 flownoise4D(fpreal64 const x[4], uint64 const per[4],
                                    fpreal64 const flow)
    {
    	return flowat4DP(x[0], x[1], x[2], x[3],
    	                   per[0], per[1], per[2], per[3],
    	                   flow);
    }
    // Flow noise, Vector
    static inline void flownoise2D(fpreal64 f[3],
                                   fpreal64 const x, fpreal64 const y,
                                   fpreal64 const flow)
    {
    	flowat2D(f, x, y, flow);
    }
    static inline void flownoise2D(fpreal64 f[3],
                                   fpreal64 const x[2], fpreal64 const flow)
    {
    	flowat2D(f, x[0], x[1], flow);
    }
    static inline void flownoise3D(fpreal64 f[3],
                                   fpreal64 const x[3], fpreal64 const flow)
    {
    	flowat3D(f, x[0], x[1], x[2], flow);
    }
    static inline void flownoise4D(fpreal64 f[3],
                                   fpreal64 const x[4], fpreal64 const flow)
    {
    	flowat4D(f, x[0], x[1], x[2], x[3], flow);
    }
    // Flow noise, Vector, Periodic
    static inline void flownoise2D(fpreal64 f[3],
                                   fpreal64 const x, fpreal64 const y,
                                   uint64 const px, uint64 const py,
                                   fpreal64 const flow)
    {
    	flowat2DP(f, x, y, px, py, flow);
    }
    static inline void flownoise2D(fpreal64 f[3],
                                   fpreal64 const x[2], uint64 const period[2],
                                   fpreal64 const flow)
    {
    	flowat2DP(f, x[0], x[1], period[0], period[1], flow);
    }
    static inline void flownoise3D(fpreal64 f[3],
                                   fpreal64 const x[3], uint64 const period[3],
                                   fpreal64 const flow)
    {
    	flowat3DP(f, x[0], x[1], x[2],
    	            period[0], period[1], period[2], flow);
    }
    static inline void flownoise4D(fpreal64 f[3],
                                   fpreal64 const x[4], uint64 const period[4],
                                   fpreal64 const flow)
    {
    	flowat4DP(f, x[0], x[1], x[2], x[3],
    	            period[0], period[1], period[2], period[3], flow);
    }

    static inline void curlnoise2D3D(fpreal64 f[3], const fpreal64 x[3])
    {
    	curlnoise2D3D(f, x[0], x[1], x[2]);
    }
    static inline void curlnoise3D3D(fpreal64 f[3], const fpreal64 x[3])
    {
    	curlnoise3D3D(f, x[0], x[1], x[2]);
    }
    static inline void curlnoise3D4D(fpreal64 f[3], const fpreal64 x[4])
    {
    	curlnoise3D4D(f, x[0], x[1], x[2], x[3]);
    }

    // Noise
    static v4uf at1D(v4uf const x);
    static v4uf at2D(v4uf const x, v4uf const y);
    static v4uf at3D(v4uf const x, v4uf const y, v4uf const z);
    static v4uf at4D(v4uf const x, v4uf const y, v4uf const z, v4uf const w);

    // Noise, Derivatives
    static v4uf at1D(v4uf& dfdx, v4uf const x);
    static v4uf at2D(v4uf& dfdx, v4uf& dfdy,
                         v4uf const x, v4uf const y);
    static v4uf at3D(v4uf& dfdx, v4uf& dfdy, v4uf& dfdz,
                         v4uf const x, v4uf const y, v4uf const z);
    static v4uf at4D(v4uf& dfdx, v4uf& dfdy, v4uf& dfdz, v4uf& dfdw,
                         v4uf const x, v4uf const y, v4uf const z, v4uf const w);

    // Noise, Vector
    static void at1D(v4uf f[3], v4uf const x);
    static void at2D(v4uf f[3],
                        v4uf const x, v4uf const y);
    static void at3D(v4uf f[3],
                        v4uf const x, v4uf const y, v4uf const z);
    static void at4D(v4uf f[3],
                        v4uf const x, v4uf const y, v4uf const z, v4uf const w);

    // Noise, Vector, with derivatives
    static void at1D(v4uf f[3],
                        v4uf dfdx[3],
                        v4uf const x);
    static void at2D(v4uf f[3],
                        v4uf dfdx[3], v4uf dfdy[3],
                        v4uf const x, v4uf const y);
    static void at3D(v4uf f[3],
                        v4uf dfdx[3], v4uf dfdy[3], v4uf dfdz[3],
                        v4uf const x, v4uf const y, v4uf const z);
    static void at4D(v4uf f[3],
                        v4uf dfdx[3], v4uf dfdy[3], v4uf dfdz[3], v4uf dfdw[3],
                        v4uf const x, v4uf const y, v4uf const z, v4uf const w);

    // Periodic Noise
    static v4uf at1DP(v4uf const x,
                         v4ui const px);
    static v4uf at2DP(v4uf const x, v4uf const y,
                         v4ui const px, v4ui const py);
    static v4uf at3DP(v4uf const x, v4uf const y, v4uf const z,
                         v4ui const px, v4ui const py, v4ui const pz);
    static v4uf at4DP(v4uf const x, v4uf const y, v4uf const z, v4uf const w,
                         v4ui const px, v4ui const py, v4ui const pz, v4ui const pw);

    // Periodic Noise, Derivatives
    static v4uf at1DP(v4uf& dfdx, v4uf const x,
                         v4ui const px);
    static v4uf at2DP(v4uf& dfdx, v4uf& dfdy,
                         v4uf const x, v4uf const y,
                         v4ui const px, v4ui const py);
    static v4uf at3DP(v4uf& dfdx, v4uf& dfdy, v4uf& dfdz,
                         v4uf const x, v4uf const y, v4uf const z,
                         v4ui const px, v4ui const py, v4ui const pz);
    static v4uf at4DP(v4uf& dfdx, v4uf& dfdy, v4uf& dfdz, v4uf& dfdw,
                         v4uf const x, v4uf const y, v4uf const z, v4uf const w,
                         v4ui const px, v4ui const py, v4ui const pz, v4ui const pw);

    // Periodic Noise, Vector
    static void at1DP(v4uf f[3], v4uf const x,
                        v4ui const px);
    static void at2DP(v4uf f[3],
                        v4uf const x, v4uf const y,
                        v4ui const px, v4ui const py);
    static void at3DP(v4uf f[3],
                        v4uf const x, v4uf const y, v4uf const z,
                        v4ui const px, v4ui const py, v4ui const pz);
    static void at4DP(v4uf f[3],
                        v4uf const x, v4uf const y, v4uf const z, v4uf const w,
                        v4ui const px, v4ui const py, v4ui const pz, v4ui const pw);

    // Periodic Noise, Vector, with derivatives
    static void at1DP(v4uf f[3],
                        v4uf dfdx[3],
                        v4uf const x,
                        v4ui const px);
    static void at2DP(v4uf f[3],
                        v4uf dfdx[3], v4uf dfdy[3],
                        v4uf const x, v4uf const y,
                        v4ui const px, v4ui const py);
    static void at3DP(v4uf f[3],
                        v4uf dfdx[3], v4uf dfdy[3], v4uf dfdz[3],
                        v4uf const x, v4uf const y, v4uf const z,
                        v4ui const px, v4ui const py, v4ui const pz);
    static void at4DP(v4uf f[3],
                        v4uf dfdx[3], v4uf dfdy[3], v4uf dfdz[3], v4uf dfdw[3],
                        v4uf const x, v4uf const y, v4uf const z, v4uf const w,
                        v4ui const px, v4ui const py, v4ui const pz, v4ui const pw);

    // Flowat
    
    // There is no 1D flow at.
    static v4uf flowat2D(v4uf const x, v4uf const y, v4uf const flow);
    static v4uf flowat3D(v4uf const x, v4uf const y, v4uf const z, v4uf const flow);
    static v4uf flowat4D(v4uf const x, v4uf const y, v4uf const z, v4uf const w, v4uf const flow);

    // Flowat, Vector
    static void flowat2D(v4uf f[3],
                            v4uf const x, v4uf const y,
                            v4uf const flow);
    static void flowat3D(v4uf f[3],
                            v4uf const x, v4uf const y, v4uf const z,
                            v4uf const flow);
    static void flowat4D(v4uf f[3],
                            v4uf const x, v4uf const y, v4uf const z, v4uf const w,
                            v4uf const flow);

    // Periodic Flowat
    static v4uf flowat2DP(v4uf const x, v4uf const y,
                             v4ui const px, v4ui const py,
                             v4uf const flow);
    static v4uf flowat3DP(v4uf const x, v4uf const y, v4uf const z,
                             v4ui const px, v4ui const py, v4ui const pz,
                             v4uf const flow);
    static v4uf flowat4DP(v4uf const x, v4uf const y, v4uf const z, v4uf const w,
                             v4ui const px, v4ui const py, v4ui const pz, v4ui const pw,
                             v4uf const flow);

    // Periodic Flowat, Vectorial
    static void flowat2DP(v4uf f[3],
                            v4uf const x, v4uf const y,
                            v4ui const px, v4ui const py,
                            v4uf const flow);
    static void flowat3DP(v4uf f[3],
                            v4uf const x, v4uf const y, v4uf const z,
                            v4ui const px, v4ui const py, v4ui const pz,
                            v4uf const flow);
    static void flowat4DP(v4uf f[3],
                            v4uf const x, v4uf const y, v4uf const z, v4uf const w,
                            v4ui const px, v4ui const py, v4ui const pz, v4ui const pw,
                            v4uf const flow);

    // Curlat functions
    
    static void curlnoise2D2D(v4uf f[3],
                              v4uf const x,
                              v4uf const y);
    static void curlnoise2D3D(v4uf f[3],
                              v4uf const x,
                              v4uf const y,
                              v4uf const z);
    static void curlnoise3D3D(v4uf f[3],
                              v4uf const x,
                              v4uf const y,
                              v4uf const z);
    static void curlnoise3D4D(v4uf f[3],
                              v4uf const x,
                              v4uf const y,
                              v4uf const z,
                              v4uf const w);


    // Legacy compatibility
    
    // Noise
    static inline v4uf noise1D(v4uf const x)
    {
    	return at1D(x);
    }
    static inline v4uf noise2D(v4uf const x, v4uf const y)
    {
    	return at2D(x, y);
    }
    static inline v4uf noise2D(v4uf const x[2])
    {
	return at2D(x[0], x[1]);
    }
    static inline v4uf noise3D(v4uf const x[3])
    {
	return at3D(x[0], x[1], x[2]);
    }
    static inline v4uf noise4D(v4uf const x[4])
    {
	return at4D(x[0], x[1], x[2], x[3]);
    }
    // Noise, Derivatives
    static inline void noise1D(v4uf &n, v4uf &dndx, v4uf const x)
    {
	n = at1D(dndx, x);
    }
    static inline void noise2D(v4uf &n, v4uf &dndx, v4uf &dndy, const v4uf x[2])
    {
	n = at2D(dndx, dndy, x[0], x[1]);
    }
    static inline void noise2D(v4uf &n, v4uf dndx[2], const v4uf x[2])
    {
	n = at2D(dndx[0], dndx[1], x[0], x[1]);
    }
    static inline void noise2D(v4uf &n, v4uf &dndx, v4uf &dndy,
                               v4uf const x, v4uf const y)
    {
	n = at2D(dndx, dndy, x, y);
    }
    static inline void noise3D(v4uf &n, v4uf dndx[3], const v4uf x[3])
    {
	n = at3D(dndx[0], dndx[1], dndx[2],
	         x[0], x[1], x[2]);
    }
    static inline void noise3D(v4uf &n, v4uf &dndx,
                               v4uf &dndy, v4uf &dndz,
                               const v4uf x[3])
    {
	n = at3D( dndx, dndy, dndz, x[0], x[1], x[2]);
    }
    static inline void noise4D(v4uf &n, v4uf dndx[4],
                               const v4uf x[4])
    {
	n = at4D(dndx[0], dndx[1], dndx[2], dndx[3],
	         x[0], x[1], x[2], x[3]);
    }
    static inline void noise4D(v4uf &n, v4uf &dndx,
                               v4uf &dndy, v4uf &dndz, v4uf &dndw,
                               const v4uf x[4])
    {
	n = at4D(dndx, dndy, dndz, dndw, x[0], x[1], x[2], x[3]);
    }
    // Periodic Noise
    static inline v4uf noise1D(v4uf const x, v4ui const px)
    {
    	return at1DP(x, px);
    }
    static inline v4uf noise2D(v4uf const x[2], v4ui const per[2])
    {
	return at2DP(x[0], x[1], per[0], per[1]);
    }
    static inline v4uf noise2D(v4uf const x, v4uf const y,
                                   v4ui const px, v4ui const py)
    {
	return at2DP(x, y, px, py);
    }
    static inline v4uf noise3D(v4uf const x[3], v4ui const per[3])
    {
	return at3DP(x[0], x[1], x[2],
	               per[0], per[1], per[2]);
    }
    static inline v4uf noise4D(v4uf const x[4], v4ui const per[4])
    {
	return at4DP(x[0], x[1], x[2], x[3],
	               per[0], per[1], per[2], per[3]);
    }
    // Noise, Vector
    static inline void noise1D(v4uf f[3], v4uf const x)
    {
    	at1D(f, x);
    }
    static inline void noise2D(v4uf f[3],
                               v4uf const x, v4uf const y)
    {
    	at2D(f, x, y);
    }
    static inline void noise2D(v4uf f[3], v4uf const x[2])
    {
	at2D(f, x[0], x[1]);
    }
    static inline void noise3D(v4uf f[3], v4uf const x[3])
    {
	at3D(f, x[0], x[1], x[2]);
    }
    static inline void noise4D(v4uf f[3], v4uf const x[4])
    {
	at4D(f, x[0], x[1], x[2], x[3]);
    }
    // Noise, Vector, Derivative
    static inline void noise1D(v4uf f[3], v4uf dfdx[3], v4uf const x)
    {
	at1D(f, dfdx, x);
    }
    static inline void noise2D(v4uf f[3], v4uf dfdx[3], v4uf dfdy[3], v4uf const x[2])
    {
	at2D(f, dfdx, dfdy,
	        x[0], x[1]);
    }
    static inline void noise2D(v4uf f[3],
                               v4uf dfdx[3], v4uf dfdy[3],
                               v4uf const x, v4uf y)
    {
	at2D(f, dfdx, dfdy, x, y);
    }
    static inline void noise3D(v4uf f[3], v4uf dfdx[3], v4uf dfdy[3], v4uf dfdz[3], v4uf const x[3])
    {
	at3D(f, dfdx, dfdy, dfdz,
	        x[0], x[1], x[2]);
    }
    static inline void noise4D(v4uf f[3], v4uf dfdx[3], v4uf dfdy[3], v4uf dfdz[3], v4uf dfdw[3], v4uf const x[4])
    {
	at4D(f, dfdx, dfdy, dfdz, dfdw,
	        x[0], x[1], x[2], x[3]);
    }
    // Periodic Noise, Vector
    static inline void noise1D(v4uf f[3], v4uf const x, v4ui const px)
    {
    	at1DP(f, x, px);
    }
    static inline void noise2D(v4uf f[3],
                               v4uf const x, v4uf const y,
                               v4ui const px, v4ui const py)
    {
    	at2DP(f, x, y, px, py);
    }
    static inline void noise2D(v4uf f[3], v4uf const x[2], v4ui const per[2])
    {
	at2DP(f, x[0], x[1],
	        per[0], per[1]);
    }
    static inline void noise3D(v4uf f[3], v4uf const x[3], v4ui const per[3])
    {
	at3DP(f, x[0], x[1], x[2],
	        per[0], per[1], per[2]);
    }
    static inline void noise4D(v4uf f[3], v4uf const x[4], v4ui const per[4])
    {
	at4DP(f, x[0], x[1], x[2], x[3],
	        per[0], per[1], per[2], per[3]);
    }

    // Flow noise

    static inline v4uf flownoise2D(v4uf const x, v4uf const y,
                                       v4uf const flow)
    {
    	return flowat2D(x, y, flow);
    }
    static inline v4uf flownoise2D(v4uf const x[2], v4uf const flow)
    {
    	return flowat2D(x[0], x[1], flow);
    }
    static inline v4uf flownoise3D(v4uf const x[3], v4uf const flow)
    {
    	return flowat3D(x[0], x[1], x[2], flow);
    }
    static inline v4uf flownoise4D(v4uf const x[4], v4uf const flow)
    {
    	return flowat4D(x[0], x[1], x[2], x[3], flow);
    }

    // Flow noise, Periodic
    static inline v4uf flownoise2D(v4uf const x[2], v4ui const per[2],
                                    v4uf const flow)
    {
    	return flowat2DP(x[0], x[1], per[0], per[1], flow);
    }
    static inline v4uf flownoise2D(v4uf const x, v4uf y,
                                       v4ui const px, v4ui const py,
                                       v4uf const flow)
    {
    	return flowat2DP(x, y, px, py, flow);
    }
    static inline v4uf flownoise3D(v4uf const x[3], v4ui const per[3],
                                    v4uf const flow)
    {
    	return flowat3DP(x[0], x[1], x[2],
    	                   per[0], per[1], per[2], flow);
    }
    static inline v4uf flownoise4D(v4uf const x[4], v4ui const per[4],
                                    v4uf const flow)
    {
    	return flowat4DP(x[0], x[1], x[2], x[3],
    	                   per[0], per[1], per[2], per[3],
    	                   flow);
    }
    // Flow noise, Vector
    static inline void flownoise2D(v4uf f[3],
                                   v4uf const x, v4uf const y,
                                   v4uf const flow)
    {
    	flowat2D(f, x, y, flow);
    }
    static inline void flownoise2D(v4uf f[3],
                                   v4uf const x[2], v4uf const flow)
    {
    	flowat2D(f, x[0], x[1], flow);
    }
    static inline void flownoise3D(v4uf f[3],
                                   v4uf const x[3], v4uf const flow)
    {
    	flowat3D(f, x[0], x[1], x[2], flow);
    }
    static inline void flownoise4D(v4uf f[3],
                                   v4uf const x[4], v4uf const flow)
    {
    	flowat4D(f, x[0], x[1], x[2], x[3], flow);
    }
    // Flow noise, Vector, Periodic
    static inline void flownoise2D(v4uf f[3],
                                   v4uf const x, v4uf const y,
                                   v4ui const px, v4ui const py,
                                   v4uf const flow)
    {
    	flowat2DP(f, x, y, px, py, flow);
    }
    static inline void flownoise2D(v4uf f[3],
                                   v4uf const x[2], v4ui const period[2],
                                   v4uf const flow)
    {
    	flowat2DP(f, x[0], x[1], period[0], period[1], flow);
    }
    static inline void flownoise3D(v4uf f[3],
                                   v4uf const x[3], v4ui const period[3],
                                   v4uf const flow)
    {
    	flowat3DP(f, x[0], x[1], x[2],
    	            period[0], period[1], period[2], flow);
    }
    static inline void flownoise4D(v4uf f[3],
                                   v4uf const x[4], v4ui const period[4],
                                   v4uf const flow)
    {
    	flowat4DP(f, x[0], x[1], x[2], x[3],
    	            period[0], period[1], period[2], period[3], flow);
    }

    static inline void curlnoise2D3D(v4uf f[3], const v4uf x[3])
    {
    	curlnoise2D3D(f, x[0], x[1], x[2]);
    }
    static inline void curlnoise3D3D(v4uf f[3], const v4uf x[3])
    {
    	curlnoise3D3D(f, x[0], x[1], x[2]);
    }
    static inline void curlnoise3D4D(v4uf f[3], const v4uf x[4])
    {
    	curlnoise3D4D(f, x[0], x[1], x[2], x[3]);
    }
};

#endif

