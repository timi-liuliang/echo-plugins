/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This "submodule" is named hou.hmath instead of hou.math to prevent
 *	conflicts with the python math module, since expression evaluation
 *	contexts have run from hou import *.
 */

#ifndef __HOM_hmath_h__
#define __HOM_hmath_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include <SYS/SYS_Math.h>
#include <vector>
#include <map>
class HOM_Matrix4;
class HOM_Vector3;

SWIGOUT(%rename(hmath) HOM_hmath;)

class HOM_API HOM_hmath
{
public:
    virtual ~HOM_hmath()
    {}

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject identityTransform;)
    virtual HOM_Matrix4 *identityTransform() = 0;

    SWIGOUT(%newobject buildTranslate;)
    virtual HOM_Matrix4 *buildTranslate(double tx, double ty, double tz) = 0;
    virtual HOM_Matrix4 *buildTranslate(const std::vector<double> &values) = 0;
    virtual HOM_Matrix4 *buildTranslate(HOM_Vector3 &values) = 0;

    SWIGOUT(%newobject buildScale;)
    virtual HOM_Matrix4 *buildScale(double sx, double sy, double sz) = 0;
    virtual HOM_Matrix4 *buildScale(const std::vector<double> &values) = 0;
    virtual HOM_Matrix4 *buildScale(HOM_Vector3 &values) = 0;

    SWIGOUT(%newobject buildShear;)
    virtual HOM_Matrix4 *buildShear(double sxy, double sxz, double syz) = 0;
    virtual HOM_Matrix4 *buildShear(const std::vector<double> &values) = 0;
    virtual HOM_Matrix4 *buildShear(HOM_Vector3 &values) = 0;

    SWIGOUT(%newobject buildRotate;)
    virtual HOM_Matrix4 *buildRotate(
            double rx, double ry, double rz, const char *order="xyz") = 0;
    virtual HOM_Matrix4 *buildRotate(
            const std::vector<double> &values, const char *order="xyz") = 0;
    virtual HOM_Matrix4 *buildRotate(
            HOM_Vector3 &values, const char *order="xyz") = 0;

    SWIGOUT(%newobject buildRotateAboutAxis;)
    virtual HOM_Matrix4 *buildRotateAboutAxis(
            const std::vector<double> &axis, double angle_in_deg)= 0;
    virtual HOM_Matrix4 *buildRotateAboutAxis(
	HOM_Vector3 &axis, double angle_in_deg) = 0;

    virtual HOM_Matrix4* buildTransform(
	    const std::map<std::string, std::vector<double> > &values_dict,
	    const char *transform_order="srt",
	    const char *rotate_order="xyz") = 0;


    virtual double degToRad(double degrees) = 0;
    virtual double radToDeg(double radians) = 0;

    virtual double noise1d(const std::vector<double> &pos) const = 0;

    virtual HOM_Vector3	noise3d(const std::vector<double> &pos) const = 0;

    double fit(
	double value, double oldmin, double oldmax,
	double newmin, double newmax)
    { return SYSfit(value, oldmin, oldmax, newmin, newmax); }

    double fit01(double value, double newmin, double newmax)
    { return SYSfit(value, 0, 1, newmin, newmax); }

    double fit10(double value, double newmin, double newmax)
    { return SYSfit(value, 1, 0, newmin, newmax); }

    double fit11(double value, double newmin, double newmax)
    { return SYSfit(value, -1, 1, newmin, newmax); }

    double sign(double value)
    { return value < 0 ? -1 : (value > 0); }

    double clamp(double value, double min, double max)
    { return SYSclamp(value, min, max); }

    double smooth(double value, double min, double max)
    {
	// Caution, the order of the arguments is switched around here
	return SYSsmooth(min, max, value);
    }

    virtual double wrap(double value, double min, double max) const = 0;

    virtual double rand(double seed) const = 0;

    virtual double orient2d(
	    const std::vector<double> &pa,
	    const std::vector<double> &pb,
	    const std::vector<double> &point) = 0;
    virtual double orient3d(
	    const std::vector<double> &pa,
	    const std::vector<double> &pb,
	    const std::vector<double> &pc,
	    const std::vector<double> &point) = 0;
    virtual double inCircle(
	    const std::vector<double> &pa,
	    const std::vector<double> &pb,
	    const std::vector<double> &pc,
	    const std::vector<double> &point) = 0;
    virtual double inSphere(
	    const std::vector<double> &pa,
	    const std::vector<double> &pb,
	    const std::vector<double> &pc,
	    const std::vector<double> &pd,
	    const std::vector<double> &point) = 0;

    virtual HOM_Vector3 intersectPlane(
	HOM_Vector3 const& plane_point, 
	HOM_Vector3 const& plane_dir,
	HOM_Vector3 const& line_origin,
	HOM_Vector3 const& line_dir
	) = 0;
};

#endif
