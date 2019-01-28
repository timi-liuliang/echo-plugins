/* quadric.h
 *
 * Copyright 1993,1994,1995 Algorithmic Arts, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Algorithmic Arts, Inc.;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Algorithmic Arts, Inc.
 *
 * AUTHOR:
 *	Dave Gordon
 *
 *
 * $XDate: 1995/10/19 19:14:32 $
 * $XRevision: 1.3 $
 *
 *  Basic quadric surface data types.
 *
 */

#ifndef _TS_QUADRIC_H_
#define _TS_QUADRIC_H_

#include "TS_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_DMatrix4.h>
#include <UT/UT_BoundingBox.h>

class Conic;

class TS_API TS_Quadric
{
  public:
		TS_Quadric();
		TS_Quadric(const UT_Matrix4 &);
    		operator UT_Matrix4() const;
    TS_Quadric	&operator*=(float rhs);
    TS_Quadric	&operator+=(float rhs);
    TS_Quadric	&operator/=(float rhs);
    TS_Quadric	&operator-=(float rhs);
    void	toMatrix4(UT_Matrix4 &m) const;
    void	fromMatrix4(const UT_Matrix4 &m);
    void	toMatrix4(UT_DMatrix4 &m) const;
    void	fromMatrix4(const UT_DMatrix4 &m);
    void	cylinder(const UT_Vector3 &p0, const UT_Vector3 &axis, float radius = 1.0F);
    void	sphere(const UT_Vector3 &center, float radius = 1.0F);
    void	planes(const UT_Vector3 &p0, const UT_Vector3 &n, float r);
    void 	transform(const UT_Matrix4 &m);
    void	zeroTransform();
    int 	computeBound(UT_Vector3 &min, UT_Vector3 &max) const;
    int 	computeBound(UT_BoundingBox &bound) const;
    fpreal64	eval(const UT_Vector3 &) const;
    fpreal64	eval2(const UT_Vector3 &) const;
    fpreal64	evalMix(const UT_Vector3 &p0, const UT_Vector3 &p1) const;
    void	evalGrad(const UT_Vector3 &p, UT_Vector3 &n) const;


    fpreal64   	xx, yy, zz, xy, xz, yz, xw, yw, zw, ww;
};


#endif	// _TS_QUADRIC_H_
