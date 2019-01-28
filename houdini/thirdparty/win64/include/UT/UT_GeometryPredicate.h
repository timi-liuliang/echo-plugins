/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This code comes from public domain C code for "Fast Robust Predicates
 *	for Computational Geometry" from
 *	    http://www.cs.cmu.edu/~quake/robust.html
 *	that provides precise inside/outside tests for lines, planes, circles,
 *	and spheres.
 *
 *	It is based on the papers
 *	    Adaptive Precision Floating-Point Arithmetic and Fast Robust
 *	    Geometric Predicates
 *	and
 *	    Robust Adaptive Floating-Point Geometric Predicates
 */

#ifndef __UT_GeometryPredicate_h__
#define __UT_GeometryPredicate_h__

#include "UT_API.h"
#include "UT_Vector2.h"
#include "UT_Vector3.h"

/// Use the following class in places where you can specify the bounding box of
/// all the points you will use with these geometry predicates. If the bounding
/// box is not known, use the functions at the global scope (note that those will
/// be slower since there are additional static filters based on the bounding box
/// size used in this class.

template<typename REAL, bool USEFILTER = true, bool EXACT = true>
class UT_API UT_GeometryPredicates
{
public:
    // constructors just call exactinit
    UT_GeometryPredicates(const UT_Vector3T<REAL> &bbox_size);
    UT_GeometryPredicates(const REAL bbox_size[3]);

    UT_GeometryPredicates();  // filters uninitialized, must call exactinit with proper bounding box
    ~UT_GeometryPredicates();

    void exactinit(const UT_Vector3T<REAL> &bbox_size);
    void exactinit(const REAL bbox_size[3]);
    void exactinit(); // filters are disabled with this initializer

    // Does point c lie on, to the left of, or to the right of line ab?
    REAL orient2d(
	    const UT_Vector2T<REAL> &pa,
	    const UT_Vector2T<REAL> &pb,
	    const UT_Vector2T<REAL> &pc) const
    { return orient2d(pa.data(), pb.data(), pc.data()); }
    REAL orient2d(
	    const REAL pa[2],
	    const REAL pb[2],
	    const REAL pc[2]) const;

    // Does point d lie on, to the left of, or to the right of the plane containing
    // the points a, b, and c?
    REAL orient3d(
	    const UT_Vector3T<REAL> &pa,
	    const UT_Vector3T<REAL> &pb,
	    const UT_Vector3T<REAL> &pc,
	    const UT_Vector3T<REAL> &pd) const
    { return orient3d(pa.data(), pb.data(), pc.data(), pd.data()); }
    REAL orient3d(
	    const REAL pa[3],
	    const REAL pb[3],
	    const REAL pc[3],
	    const REAL pd[3]) const;

    // Does point d lie on, inside, or outside of the circle containing the points
    // a, b, and c?
    REAL incircle(
	    const UT_Vector2T<REAL> &pa,
	    const UT_Vector2T<REAL> &pb,
	    const UT_Vector2T<REAL> &pc,
	    const UT_Vector2T<REAL> &pd) const
    { return incircle(pa.data(), pb.data(), pc.data(), pd.data()); }
    REAL incircle(
	    const REAL pa[2],
	    const REAL pb[2],
	    const REAL pc[2],
	    const REAL pd[2]) const;

    // Does point e lie on, inside, or outside of the sphere containing the points
    // a, b, c, and d?
    REAL insphere(
	    const UT_Vector3T<REAL> &pa,
	    const UT_Vector3T<REAL> &pb,
	    const UT_Vector3T<REAL> &pc,
	    const UT_Vector3T<REAL> &pd,
	    const UT_Vector3T<REAL> &pe) const
    { return insphere(pa.data(), pb.data(), pc.data(), pd.data(), pe.data()); }
    REAL insphere(
	    const REAL pa[3],
	    const REAL pb[3],
	    const REAL pc[3],
	    const REAL pd[3],
	    const REAL pe[3]) const;

    // Does point e lie on, to the left of, or to the right of the hyperplane containing
    // the points a, b, c, and d?
    REAL orient4d(
	    const UT_Vector3T<REAL> &pa,
	    const UT_Vector3T<REAL> &pb,
	    const UT_Vector3T<REAL> &pc,
	    const UT_Vector3T<REAL> &pd,
	    const UT_Vector3T<REAL> &pe,
	    REAL ah, REAL bh, REAL ch, REAL dh, REAL eh) const
    { return orient4d(pa.data(), pb.data(), pc.data(), pd.data(), pe.data(), ah,bh,ch,dh,eh); }
    REAL orient4d(
	    const REAL pa[3],
	    const REAL pb[3],
	    const REAL pc[3],
	    const REAL pd[3],
	    const REAL pe[3],
	    REAL ah, REAL bh, REAL ch, REAL dh, REAL eh) const;

protected:
    /* splitter = 2^ceiling(p / 2) + 1.  Used to split floats in half.           */
    REAL splitter;
    REAL epsilon;         /* = 2^(-p).  Used to estimate roundoff errors. */
    /* A set of coefficients used to calculate maximum roundoff errors.          */
    REAL resulterrbound;
    REAL ccwerrboundA, ccwerrboundB, ccwerrboundC;
    REAL o3derrboundA, o3derrboundB, o3derrboundC;
    REAL iccerrboundA, iccerrboundB, iccerrboundC;
    REAL isperrboundA, isperrboundB, isperrboundC;

    // Additional predicate specific static filters
    REAL filter1, filter2;
};

UT_API extern const UT_GeometryPredicates<fpreal32,false,true> ut_global_predicates_fpreal32;
UT_API extern const UT_GeometryPredicates<fpreal64,false,true> ut_global_predicates_fpreal64;

/// Function interface for geometric predicates.
/// More efficient predicates are availble through the UT_GeometryPredicates
/// class that defines a more thorough static filter
namespace UT_GeometryPredicate
{

// Get the default instantiation of the predicate context (this is convenient
// when defining your own predicate dependent functions)
template<typename REAL>
const UT_GeometryPredicates<REAL,false,true>& getDefault();

#define SPECIALIZE_GET_DEFAULT(REAL)			    \
template<>						    \
SYS_FORCE_INLINE                                            \
const UT_GeometryPredicates<REAL,false,true>&               \
getDefault<REAL>() { return ut_global_predicates_##REAL; }

SPECIALIZE_GET_DEFAULT(fpreal64)
SPECIALIZE_GET_DEFAULT(fpreal32)

// Does point c lie on, to the left of, or to the right of line ab?
template<typename REAL>
SYS_FORCE_INLINE
REAL orient2d(
    const REAL pa[2],
    const REAL pb[2],
    const REAL pc[2])
{ return getDefault<REAL>().orient2d(pa,pb,pc); }
template<typename REAL>
SYS_FORCE_INLINE
REAL orient2d(
    const UT_Vector2T<REAL> &pa,
    const UT_Vector2T<REAL> &pb,
    const UT_Vector2T<REAL> &pc)
{ return getDefault<REAL>().orient2d(pa,pb,pc); }

// Does point d lie on, to the left of, or to the right of the plane containing
// the points a, b, and c?
template<typename REAL>
SYS_FORCE_INLINE
REAL orient3d(
    const REAL pa[3],
    const REAL pb[3],
    const REAL pc[3],
    const REAL pd[3])
{ return getDefault<REAL>().orient3d(pa,pb,pc,pd); }
template<typename REAL>
SYS_FORCE_INLINE
REAL orient3d(
    const UT_Vector3T<REAL> &pa,
    const UT_Vector3T<REAL> &pb,
    const UT_Vector3T<REAL> &pc,
    const UT_Vector3T<REAL> &pd)
{ return getDefault<REAL>().orient3d(pa,pb,pc,pd); }

// Does point d lie one, inside, or outside of the circle containing the points
// a, b, and c?
template<typename REAL>
SYS_FORCE_INLINE
REAL incircle(
    const REAL pa[2],
    const REAL pb[2],
    const REAL pc[2],
    const REAL pd[2])
{ return getDefault<REAL>().incircle(pa,pb,pc,pd); }
template<typename REAL>
SYS_FORCE_INLINE
REAL incircle(
    const UT_Vector2T<REAL> &pa,
    const UT_Vector2T<REAL> &pb,
    const UT_Vector2T<REAL> &pc,
    const UT_Vector2T<REAL> &pd)
{ return getDefault<REAL>().incircle(pa,pb,pc,pd); }

// Does point e lie one, inside, or outside of the sphere containing the points
// a, b, c, and d?
template<typename REAL>
SYS_FORCE_INLINE
REAL insphere(
    const REAL pa[3],
    const REAL pb[3],
    const REAL pc[3],
    const REAL pd[3],
    const REAL pe[3])
{ return getDefault<REAL>().insphere(pa,pb,pc,pd,pe); }
template<typename REAL>
SYS_FORCE_INLINE
REAL insphere(
    const UT_Vector3T<REAL> &pa,
    const UT_Vector3T<REAL> &pb,
    const UT_Vector3T<REAL> &pc,
    const UT_Vector3T<REAL> &pd,
    const UT_Vector3T<REAL> &pe)
{ return getDefault<REAL>().insphere(pa,pb,pc,pd,pe); }

// Does point e lie on, to the left of, or to the right of the hyperplane containing
// the points a, b, c, and d?
template<typename REAL>
SYS_FORCE_INLINE
REAL orient4d(
    const REAL pa[3],
    const REAL pb[3],
    const REAL pc[3],
    const REAL pd[3],
    const REAL pe[3],
    REAL ah, REAL bh, REAL ch, REAL dh, REAL eh)
{ return getDefault<REAL>().orient4d(pa,pb,pc,pd,pe,ah,bh,ch,dh,eh); }
template<typename REAL>
SYS_FORCE_INLINE
REAL orient4d(
    const UT_Vector3T<REAL> &pa,
    const UT_Vector3T<REAL> &pb,
    const UT_Vector3T<REAL> &pc,
    const UT_Vector3T<REAL> &pd,
    const UT_Vector3T<REAL> &pe,
    REAL ah, REAL bh, REAL ch, REAL dh, REAL eh)
{ return getDefault<REAL>().orient4d(pa,pb,pc,pd,pe,ah,bh,ch,dh,eh); }

} // end of namespace UT_GeometryPredicate

template<typename REAL, bool USEFILTER>
SYS_FORCE_INLINE
UT_Vector3T<REAL> UTcrossExact(
	const UT_Vector3T<REAL> &v1,
       	const UT_Vector3T<REAL> &v2,
	const UT_GeometryPredicates<REAL,USEFILTER,true>& pred)
{
    // compute the cross product with exact signs
    typedef UT_Vector2T<REAL> Vec2;
    return UT_Vector3T<REAL>(
	    pred.orient2d( Vec2(v1.y(), v1.z()),
		Vec2(v2.y(), v2.z()),
		Vec2(0,0) ),
	    pred.orient2d( Vec2(v1.z(), v1.x()),
		Vec2(v2.z(), v2.x()),
		Vec2(0,0) ),
	    pred.orient2d( Vec2(v1.x(), v1.y()),
		Vec2(v2.x(), v2.y()),
		Vec2(0,0) ));
}

template<typename REAL>
SYS_FORCE_INLINE
UT_Vector3T<REAL> UTcrossExact(
	const UT_Vector3T<REAL> &v1,
       	const UT_Vector3T<REAL> &v2)
{
    return UTcrossExact<REAL,false>(v1,v2, UT_GeometryPredicate::getDefault<REAL>());
}

// Does point c lie on, to the left of, or to the right of line ab?
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::orient2d())
SYS_FORCE_INLINE double UTgeometryPredicateOrient2d(
    const UT_Vector2 &a,
    const UT_Vector2 &b,
    const UT_Vector2 &c)
{
    double pa[2] = {a[0], a[1]};
    double pb[2] = {b[0], b[1]};
    double pc[2] = {c[0], c[1]};
    return UT_GeometryPredicate::orient2d<fpreal64>(pa, pb, pc);
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::orient2d())
SYS_FORCE_INLINE double UTgeometryPredicateOrient2d(
    const UT_Vector2D &pa,
    const UT_Vector2D &pb,
    const UT_Vector2D &pc)
{
    return UT_GeometryPredicate::orient2d<fpreal64>(pa.data(), pb.data(), pc.data());
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::orient2d())
SYS_FORCE_INLINE double UTgeometryPredicateOrient2d(
    const double pa[2],
    const double pb[2],
    const double pc[2])
{
    return UT_GeometryPredicate::orient2d<fpreal64>(pa, pb, pc);
}
// Does point d lie on, to the left of, or to the right of the plane containing
// the points a, b, and c?
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::orient3d())
SYS_FORCE_INLINE double UTgeometryPredicateOrient3d(
    const UT_Vector3 &a,
    const UT_Vector3 &b,
    const UT_Vector3 &c,
    const UT_Vector3 &d)
{
    double pa[3] = {a[0], a[1], a[2]};
    double pb[3] = {b[0], b[1], b[2]};
    double pc[3] = {c[0], c[1], c[2]};
    double pd[3] = {d[0], d[1], d[2]};
    return UT_GeometryPredicate::orient3d<fpreal64>(pa, pb, pc, pd);
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::orient3d())
SYS_FORCE_INLINE double UTgeometryPredicateOrient3d(
    const UT_Vector3D &pa,
    const UT_Vector3D &pb,
    const UT_Vector3D &pc,
    const UT_Vector3D &pd)
{
    return UT_GeometryPredicate::orient3d<fpreal64>(pa.data(), pb.data(), pc.data(), pd.data());
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::orient3d())
SYS_FORCE_INLINE double UTgeometryPredicateOrient3d(
    const double pa[3],
    const double pb[3],
    const double pc[3],
    const double pd[3])
{
    return UT_GeometryPredicate::orient3d<fpreal64>(pa, pb, pc, pd);
}

// Does point d lie one, inside, or outside of the circle containing the points
// a, b, and c?
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::incircle())
SYS_FORCE_INLINE double UTgeometryPredicateIncircle(
    const UT_Vector2 &a,
    const UT_Vector2 &b,
    const UT_Vector2 &c,
    const UT_Vector2 &d)
{
    double pa[2] = {a[0], a[1]};
    double pb[2] = {b[0], b[1]};
    double pc[2] = {c[0], c[1]};
    double pd[2] = {d[0], d[1]};
    return UT_GeometryPredicate::incircle<fpreal64>(pa, pb, pc, pd);
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::incircle())
SYS_FORCE_INLINE double UTgeometryPredicateIncircle(
    const UT_Vector2D &pa,
    const UT_Vector2D &pb,
    const UT_Vector2D &pc,
    const UT_Vector2D &pd)
{
    return UT_GeometryPredicate::incircle<fpreal64>(pa.data(), pb.data(),
					pc.data(), pd.data());
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::incircle())
SYS_FORCE_INLINE double UTgeometryPredicateIncircle(
    const double pa[2],
    const double pb[2],
    const double pc[2],
    const double pd[2])
{
    return UT_GeometryPredicate::incircle<fpreal64>(pa, pb, pc, pd);
}

// Does point e lie one, inside, or outside of the sphere containing the points
// a, b, c, and d?
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::insphere())
SYS_FORCE_INLINE double UTgeometryPredicateInsphere(
    const UT_Vector3 &a,
    const UT_Vector3 &b,
    const UT_Vector3 &c,
    const UT_Vector3 &d,
    const UT_Vector3 &e)
{
    double pa[3] = {a[0], a[1], a[2]};
    double pb[3] = {b[0], b[1], b[2]};
    double pc[3] = {c[0], c[1], c[2]};
    double pd[3] = {d[0], d[1], d[2]};
    double pe[3] = {e[0], e[1], e[2]};
    return UT_GeometryPredicate::insphere<fpreal64>(pa, pb, pc, pd, pe);
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::insphere())
SYS_FORCE_INLINE double UTgeometryPredicateInsphere(
    const UT_Vector3D &pa,
    const UT_Vector3D &pb,
    const UT_Vector3D &pc,
    const UT_Vector3D &pd,
    const UT_Vector3D &pe)
{
    return UT_GeometryPredicate::insphere<fpreal64>(pa.data(), pb.data(), pc.data(),
					pd.data(), pe.data());
}
SYS_DEPRECATED_HDK_REPLACE(16.0, UT_GeometryPredicate::insphere())
SYS_FORCE_INLINE double UTgeometryPredicateInsphere(
    const double pa[3],
    const double pb[3],
    const double pc[3],
    const double pd[3],
    const double pe[3])
{
    return UT_GeometryPredicate::insphere<fpreal64>(pa, pb, pc, pd, pe);
}

#endif // __UT_GeometryPredicate_h__
