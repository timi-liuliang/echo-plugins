/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FFT.h
 *
 * COMMENTS:
 *
 *	Performs a fast discrete fourier transform (or an inverse) on
 *      array data.
 *      FORWARD: Takes real data and produces complex data.
 *      INVERSE: Takes complex data and produces real data.
 */

#ifndef __UT_FFT__
#define __UT_FFT__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

template <typename T>
class UT_API UT_FFT
{
public:
    explicit UT_FFT(exint npnts);
    ~UT_FFT();

    void toFrequency(T *source, T *destr, T *desti, exint npnts);
    void toFrequency(T *sourcer, T *sourcei, T *destr, T *desti, exint npnts);
    void toTime(T *sourcer, T *sourcei, T *dest, exint npnts);

    void toTime(T *sourcer, T *sourcei, T *destr, T *desti, exint npnts);

    // two dimensional FFT'ing
    static void toFrequency2D(T *sourcer, T *sourcei, T *destr, T *desti,
			      exint size);
    static void toTime2D(T *sourcer, T *sourcei, T *destr, T *desti, exint size);

private:

    int for_fft(T *yre, T *yim, T *zre, T *zim, exint npts);
    int inv_fft(T *yre, T *yim, T *zre, T *zim, exint npts);

    void dofft(int agamma);
    void fftcopy(T *pr_from, T *pi_from, T *p_to, exint npts, int is_plus);
    
    void ifftcopy(T *p_from, T *pr_to, T *pi_to, int agamma, T factr, T facti);
    void fftfill(T *p_co, exint part, exint all);
    
    void make_w(int neww);
    void butterfly(int fly,int agamma);

    static void init_bitsw();
    static int slow_bitsw(int k,int n);
    static exint bitswitch(exint k, exint n);
	
    T *x_co;
    T *w_co;
    
    static unsigned char tbl[256];
    int gamma;
    exint fft_size;

};
    
#endif
