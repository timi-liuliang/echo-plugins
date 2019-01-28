/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_Transform.h (GU Library, C++)
 *
 */

// TODO should there be a class or namespace or anything, or just totally stand
// alone functions?

#ifndef __GEO_Transform_h__
#define __GEO_Transform_h__

#include "GEO_API.h"

#include <GA/GA_AttributeTransformer.h>
#include <GA/GA_Names.h>

#include "GEO_Delta.h"

bool GEO_API GEOgetAttributeList(const GA_AttributeDict &dict,
                                        UT_Array<GA_RWHandleV3> &attribs,
                                        UT_Array<GA_RWHandleV3D> &doubleattribs,
                                        UT_Array<GA_RWHandleM4> &m4attribs,
                                        UT_Array<GA_RWHandleM4D> &m4dattribs,
                                        const char *attribpattern);

// Computes on which side of the symmetry plane a given position lies.  The
// side in the direction of the normal is 1, the opposite side is -1, and,
// right on the plane is 0.
int GEO_API GEOcomputeSymmetrySide(
                                const GEO_Detail::SoftSymmetryParms &symmetry,
                                const UT_Vector3 pos);

void GEO_API inline GEOxformElement(
                            const GA_AttributeTransformer &attribs,
                            GA_Offset offset,
                            GA_AttributeOwner owner,
                            GA_AttributeTransformer::Transform<fpreal32> &ctx32,
                            GA_AttributeTransformer::Transform<fpreal64> &ctx64,
                            bool just_P,
                            int keep_length,
                            GEO_Delta *geodelta,
                            const char *attribpattern = NULL)
{
    if (geodelta)
    {
        GEO_Detail &detail = static_cast<GEO_Detail &>(attribs.getDetail());
        if (owner == GA_ATTRIB_POINT && (!attribpattern || GA_Names::P.multiMatch(attribpattern)))
        {
            geodelta->beginPointPositionChange(detail, offset);
            if (UTverify_cast<GA_ATINumeric*>(detail.getP())->getStorage() == GA_STORE_REAL64)
            {
                GA_RWHandleV3D pd(detail.getP());
                pd.set(offset, pd.get(offset) * ctx64.xform());
            }
            else
                detail.setPos3(offset, detail.getPos3(offset) * ctx32.xform());
            geodelta->endChange();
        }

	if (!just_P && attribs.entries() > 0)
	{
            if (owner == GA_ATTRIB_POINT)
                geodelta->beginPointAttributeChange(detail, offset);
            else if (owner == GA_ATTRIB_VERTEX)
                geodelta->beginVertexAttributeChange(detail, offset);
            else if (owner == GA_ATTRIB_PRIMITIVE)
                geodelta->beginPrimitiveAttributeChange(*detail.getGEOPrimitive(offset));
	    attribs.transform(offset, ctx32, ctx64);
	    geodelta->endChange();
	}
    }
    else
    {
        if (attribs.entries() == 0)
            return;
	attribs.transform(offset, ctx32, ctx64);
    }
}

// prepares matx
int GEO_API
GEOcomputeSoftXform(float dist2, const GEO_Detail::LocalXformContext *local,
		    const UT_XformOrder &order,
		    float tx, float ty, float tz,
		    float rx, float ry, float rz,
		    float sx, float sy, float sz,
		    float s_xy, float s_xz, float s_yz,
		    float px, float py, float pz,
		    float prx, float pry, float prz,
		    const GEO_Rolloff *rolloff,
		    UT_Matrix4 &matx, float *falloff);

template<typename T>
struct GEO_API GEO_Tolerance{
    static constexpr T value() { return 0; }
};

template<>
struct GEO_API GEO_Tolerance<fpreal32> {
    static constexpr fpreal32 value() { return SYS_FTOLERANCE; }
};

template<>
struct GEO_API GEO_Tolerance<fpreal64> {
    static constexpr fpreal64 value() { return 1e-14; }
};

template<typename T>
inline T GEO_API
GEOconditionScale(T f)
{
    if (f > -GEO_Tolerance<T>::value() && f < GEO_Tolerance<T>::value())
	f = f < 0 ? -GEO_Tolerance<T>::value() : GEO_Tolerance<T>::value();
    return f;
}

void GEO_API GEOgetFloatOrDoubleP(GEO_Detail &detail, GA_RWHandleV3 &floatp,
                                  GA_RWHandleV3D &doublep);

void GEO_API GEOupdatePointNormals(GEO_Detail &gdp, GEO_Delta *geodelta);
#endif

