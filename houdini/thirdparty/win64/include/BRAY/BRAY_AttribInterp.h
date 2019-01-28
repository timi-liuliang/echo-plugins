/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_AttribInterp.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_AttribInterp__
#define __RAY_AttribInterp__

#include "BRAY_API.h"

/// Class to help interpolation of attributes for quads & triangles
class BRAY_API BRAY_AttribInterp
{
public:
    /// Given a quadrilateral with values at: @code
    ///   s0t1    s1t1
    ///      +----+
    ///      |    |
    ///      +----+
    ///   s0t0    s1t0
    /// @endcode
    /// Perform bilinear interpolation to get the resulting value.  For @c int
    /// or @c UT_StringHolder, the value is conditionally copied.
    /// - @c tsize is the tuple size
    /// - @c s will interpolate between the left (0.0) and right (1.0) edges
    /// - @c t will interpolate between the bottom (0.0) and top (1.0) edges.
    ///
    /// @c T valid for { bool, int32/64, fpreal32/64, UT_StringHolder }
    template <typename T>
    static void	quad(T *dest, const T *s0t0, const T *s1t0,
			const T *s1t1, const T *s0t1,
			int tsize, float s, float t);

    /// Given a triangle with values at the corners, perform barycentric
    /// interpolation to get the resulting value.  For @c int or @c
    /// UT_StringHolder, the value is conditionally copied.
    ///
    /// @c s and @c t should be the barycentric coordinates for the triangle
    /// formed by @c p0, @c p1, @c p2 and thus <tt>s+t <= 1</tt>
    ///
    /// @c T valid for { bool, int32/64, fpreal32/64, UT_StringHolder }
    template <typename T>
    static void	tri(T *dest, const T *p0, const T *p1, const T *p2,
			int tsize, float s, float t);

    /// @{
    /// Return the conditional index for the given s,t coordinates.
    /// The vertex ordering is: @code
    ///
    ///     Tri        Quad
    ///      3        3    2
    ///      +         +--+
    ///     / \        |  |
    ///    +---+       +--+
    ///   0     1     0    1
    ///
    /// @endcode
    ///
    static int	conditionalQuad(float s, float t);
    static int	conditionalTri(float s, float t);
    /// @}
};

#endif
