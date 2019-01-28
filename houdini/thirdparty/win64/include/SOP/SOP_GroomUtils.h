/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *	Helper classes for working with hair and fur grooms.
 */

#ifndef __SOP_GroomUtils_h__
#define __SOP_GroomUtils_h__

#include <GA/GA_AttributeRefMap.h>
#include <GEO/GEO_PointTree.h>
#include <GU/GU_Detail.h>
#include <UT/UT_Ramp.h>
#include <UT/UT_Tuple.h>
#include <UT/UT_UniquePtr.h>
#include "SOP_NodeVerb.h"

// A tree that can be built using an arbitrary prim or point attribute as the
// point position. When using a prim attribute, this allows direct lookup of
// curve primitives.
class sop_CurveTree : public GEO_PointTreeGAOffset
{
public:
    /// Rebuilds the tree with the given detail and primitive group,
    /// This creates a point per primitive using the primitive attribute of the
    /// specified name as the point position.
    /// if primgroup is NULL then all primitives are used.
    void
    build(
	const GEO_Detail *gdp,
	const GA_PrimitiveGroup *primgroup,
	const char *attrib,
	bool enable_multithreading=true);

    // Methods that must delegate to base class due to overloads
    void build(
	const GEO_Detail *gdp,
	const GA_PointGroup *ptgroup,
	bool enable_multithreading = true)
    {
	GEO_PointTreeGAOffset::build(gdp, ptgroup, enable_multithreading);
    }

    void build(
	const GEO_Detail *gdp,
	const GA_PointGroup *ptgroup,
	const char *attrib,
	bool enable_multithreading = true)
    {
	GEO_PointTreeGAOffset::build(gdp, ptgroup, attrib, enable_multithreading);
    }
};

class sop_FastRamp
{
public:
    sop_FastRamp(int numsamples=1000)
    : numsamples(numsamples)
    {}

    bool update(const UT_Ramp *widthramp);

    float lookup(float u) const;

    float lookupNearest(float u) const;

private:
    UT_FloatArray samples;
    int numsamples;
};

class sop_SourceAttribList
{
public:
    sop_SourceAttribList(
	GU_Detail *dest,
	const GU_Detail *source,
	const GA_AttributeOwner destowner)
    : dest(dest)
    , source(source)
    , destowner(destowner)
    {}

    typedef UT_Tuple<GA_AttributeOwner, UT_StringHolder> AttribMatchTuple;

    void addSourceAttribs(
	const GA_AttributeOwner sourceowner,
	const char *matchpattern,
	UT_Set<AttribMatchTuple> &alreadymapped);

    enum AttribChange
    {
	NONE,
	DATA,
	STORAGE
    };

    class CacheData
    {
    private:
	UT_ValArray<int> targetindices;
	UT_ValArray<GA_AttributeOwner> owners;
	UT_ValArray<UT_StringHolder> names;
	UT_ValArray<int> typeids;
	UT_ValArray<int> tuplesizes;
	UT_ValArray<GA_DataId> dataids;

	friend sop_SourceAttribList;

    };

    void mapAndCreateAttribs(
	GA_AttributeRefMap &map) const
    {
	mapAndCreateAttribs(map, nullptr);
    }

    void mapAndCreateAttribs(
	GA_AttributeRefMap &map,
	const UT_ValArray<AttribChange> *changes) const;

    bool updateCache(
	    UT_UniquePtr<CacheData> &cachedata,
	    UT_ValArray<AttribChange> &changes) const;

private:
    CacheData* getCacheData() const;

    const GU_Detail *source;
    UT_ValArray<const GA_Attribute*> sourceattribs;
    GU_Detail *dest;
    const GA_AttributeOwner destowner;
};

uint sop_vexHash(float seed);
uint sop_vexHash(float seed1, float seed2);
float sop_vexRandom(uint seed);
float sop_vexRand(uint seed);

fpreal sop_calcPerimeter(UT_Vector3Array &pos);
float sop_unitLengthToUnitDomain(UT_Vector3Array &pos, float ulength);
float sop_unitToUnitLengthDomain(UT_Vector3Array &pos, float uparm);

void sop_skinPrimError(const SOP_NodeVerb::CookParms &cookparms);

#endif
