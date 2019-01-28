/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Resample.h ( GU Library, C++)
 *
 * COMMENTS:	Resample polygons
 */

#pragma once

#ifndef __GU_Resample__
#define __GU_Resample__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_Lock.h>
#include <UT/UT_ThreadedAlgorithm.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>

class GEO_PrimPoly;
class GEO_Face;
class GEO_Detail;
class GA_PrimitiveGroup;
class GA_Attribute;

class GU_API GU_Resample
{
public:
    GU_Resample();
    ~GU_Resample() {}

    //
    // MeasureType controls how the segment length constraint is used:
    //	ARC	means to measure along the *source* segments.
    //	CHORD	means to measure along the *resampled* segments.
    //	EDGE	means to measure along the *source* segments but to retain the
    //		original breakpoints.
    //
    enum	MeasureType	{ ARC, CHORD, EDGE };
    enum	MethodType	{ DIST, X, Y, Z };
    enum PolyCurvesAs { STRAIGHT, SUBD, INTERP };

    void	setType(MeasureType type)	{ myType = type; }
    void	setMethod(MethodType type)	{ myMethod = type; }
    void        setTreatPolyCurvesAs(PolyCurvesAs type)
    { myTreatPolyCurvesAs = type; }
    void        setOutputAsSubD(bool outputassubd)
    { myOutputAsSubD = outputassubd; }
    void        setAllEqual(bool allequal)
    { myAllEqual = allequal; }
    void	setMaxSegments(int seg)		{ myMaxSegments = seg; }
    void        setMaxSegmentsAttrib(const GA_Attribute *attrib)
    {
        if (!attrib || attrib->getOwner() != GA_ATTRIB_PRIMITIVE)
            return;
        myMaxSegmentsAttrib.bind(attrib);
    }
    void	setLength(float len)		{ mySegmentLength = len; }
    void        setLengthAttrib(const GA_Attribute *attrib)
    {
        if (!attrib || attrib->getOwner() != GA_ATTRIB_PRIMITIVE)
            return;
        mySegmentLengthAttrib.bind(attrib);
    }
    void	setMaintainLast(int onoff)	{ myMaintain = onoff; }
    void	setLOD(float lod)		{ myLOD = lod; }

    class CreateAttrs {
    public:
	CreateAttrs()
	    : ptdistattr(0)
	    , tangentattr(0)
	    , curveuattr(0)
	    , curvenumattr(0)
	    {}

	GA_Attribute *ptdistattr;
	GA_Attribute *tangentattr;
	GA_Attribute *curveuattr;
	GA_Attribute *curvenumattr;
    };

    void resampleFaces(
        const GEO_Detail *src_gdp,
        const GA_Offset *src_curve_offsets,
        exint src_curve_count,
        GEO_Detail *gdp,
        const CreateAttrs &attrs,
        bool onlypoints,
        bool randomshift);

private:
    struct PrimInfo {
	UT_FloatArray	 myUVals;
	GA_Offset        myPoly;
	GA_Size		 myOff;
    };

    THREADED_METHOD5(GU_Resample,
		     prim_uvals.entries() > 1000,
		     resampleParametric,
		     UT_Array<PrimInfo> &, prim_uvals,
                     const GEO_Detail *, src_gdp,
                     const GA_Offset *, src_curve_offsets,
                     exint, src_curve_count,
                     bool, randomshift);

    void resampleParametricPartial(
	    UT_Array<PrimInfo> &prim_uvals,
            const GEO_Detail *src_gdp,
            const GA_Offset *src_curve_offsets,
            exint src_curve_count,
            bool randomshift,
	    const UT_JobInfo &info);

    THREADED_METHOD7(GU_Resample,
		     prim_uvals.entries() > 1000,
		     fillAttributes,
		     UT_Array<PrimInfo> &, prim_uvals,
                     const GEO_Detail *, src_gdp,
                     const GA_Offset *, src_curve_offsets,
                     exint, src_curve_count,
                     GEO_Detail *, gdp,
		     const CreateAttrs &, attrs,
		     GA_Offset, ptoff);

    void fillAttributesPartial(
            UT_Array<PrimInfo> &prim_uvals,
            const GEO_Detail *src_gdp,
            const GA_Offset *src_curve_offsets,
            exint src_curve_count,
            GEO_Detail *gdp,
            const CreateAttrs &attrs,
            GA_Offset ptoff,
            const UT_JobInfo &info);

private:
    MeasureType		myType;
    MethodType		myMethod;
    int			myMaxSegments;
    GA_ROHandleI        myMaxSegmentsAttrib;
    float		mySegmentLength;
    GA_ROHandleF        mySegmentLengthAttrib;
    float		myLOD;
    PolyCurvesAs        myTreatPolyCurvesAs;
    bool		myMaintain;
    bool                myOutputAsSubD;
    bool                myAllEqual;
    UT_Lock             myGroupCopyLock;
};

#endif

