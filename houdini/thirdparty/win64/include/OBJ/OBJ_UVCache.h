/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        OBJ_UVCache.h (Custom Library, C++)
 *
 * COMMENTS:    An object used to calculate world coordinates from UV coordinates.
 *
 */

#include <UT/UT_String.h>
#include <UT/UT_IntArray.h>

class GEO_PrimPoly;
class GEO_Hull;
class GA_ROAttributeRef;
class GEO_Primitive;
class GU_Detail;

void obj_getHullIndexFromUV(int &r, int &c, float &fracu, float &fracv, 
			    const GEO_Hull *hull, float u, float v);

class OBJ_UVCache
{
public:
    OBJ_UVCache(const GU_Detail *gdp, const char *uvname,
                bool wrapu, float minu, float maxu, 
                bool wrapv, float minv, float maxv);
    ~OBJ_UVCache();

    bool                 isCacheValid(const GU_Detail *gdp,
                                      const char *uvname,
                                      bool assumeuvconstant,
                                      bool wrapu, float minu, float maxu,
                                      bool wrapv, float minv, float maxv) const;

    void                 addPrimitive(const GEO_Primitive *prim,
					const GA_ROAttributeRef &uvoff,
					bool isvertex);

    void                 wrapUV(float &newu, float &newv,
                                float oldu, float oldv);
    bool                 getCell(float u, float v, int &cu, int &cv);

    // This returns the primitive number and the parametric u/v
    // of the point on that primitive.  The parametric u/v are sent
    // out through pu, pv.  The incoming u/v should be texture uv.
    int                  getPrimitive(const GU_Detail *gdp,
                                 float u, float v, float &pu, float &pv);

    // This returns whether the given polygon contains the given
    // u/v value.  It fills out pu & pv with the relevant parametric
    // coordinates.
    bool                 testPolygon(const GEO_PrimPoly *poly,
                                const GA_ROAttributeRef &uvoff, bool isvertex,
                                float u, float v,
                                float &pu, float &pv);

    // Same as testPolygon, but for meshes.
    bool                 testHull(const GEO_Hull *hull,
                                const GA_ROAttributeRef &uvoff, bool isvertex,
                                float u, float v,
                                float &pu, float &pv);

protected:
    UT_String            myUVName;
    int                  myNumPoly;
    float                myMinU, myMaxU;
    float                myMinV, myMaxV;
    bool                 myWrapU, myWrapV;
    int                  myResU, myResV;
    // Accessed myPrimTable[vcell][ucell]
    // Each entry is a list of the primitive Ids that lie within
    // that range.
    UT_IntArray         ***myPrimTable;

    int                  myGdpID, myGdpMetaCount;
};

