/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *	Helper class for evaluating parms with override dropdown menus.
 */

#ifndef __SOP_ParmOverride_h__
#define __SOP_ParmOverride_h__

#include "SOP_API.h"
#include "SOP_NodeVerb.h"
#include "FS/FS_Info.h"
#include "TIL/TIL_TextureMap.h"

class SOP_ParmOverride
{
public:
    enum class OverrideType
    {
	NONE = 0,
	CURVEATTRIB,
	CLUMPATTRIB,
	SKINATTRIB,
	TEXTURE
    };

    class CacheData
    {
    public:
	fpreal baseparmvalue;
	OverrideType overridetype;
	GA_DataId curvePid;
	GA_DataId curvetopoid;
	GA_DataId clumpPid;
	GA_DataId clumptopoid;
	GA_DataId skinPid;
	GA_DataId skintopoid;
	GA_DataId curveattribid;
	GA_DataId clumpattribid;
	GA_DataId skinattribid;
	GA_DataId uvattribid;
	UT_StringHolder texturepath;
    };

    SOP_ParmOverride(
	    const SOP_NodeVerb::CookParms &_cookparms,
	    const SOP_NodeParms &_parms,
	    const UT_StringHolder &_baseparmname,
	    const GU_Detail *_gdp,
	    const GU_Detail *_clumpgdp,
	    const GU_Detail *_skingdp,
	    const GA_ROHandleV3 &_uvattrib,
	    const UT_ValArray<OverrideType> &_overridetypes);

    OverrideType overrideType() const
    {
	return overridetype;
    }

    fpreal baseValue() const
    {
	return baseparmvalue;
    }

    // eval curve, clump, skin and texture
    void eval(
	    UT_FloatArray			&out,
	    const GA_AttributeOwner	    	 target,
	    const GA_Offset		    	 skinprimoff,
	    const GA_OffsetArray	    	&skinvertoffsets,
	    const UT_FloatArray		    	&skinvertweights,
	    const UT_Vector3		    	&uv,
	    const GA_Offset		    	 curveprimoff,
	    const GA_OffsetListRef	    	&curvevertoffsets,
	    const GA_Offset		    	 clumpprimoff,
	    const UT_ValArray<GA_OffsetArray>	 &clumpvertoffsets,
	    const UT_ValArray<UT_FloatArray>	 &clumpvertweights)
    {
	eval(
	    out,
	    target,
	    skinprimoff,
	    &skinvertoffsets,
	    &skinvertweights,
	    uv,
	    curveprimoff,
	    &curvevertoffsets,
	    clumpprimoff,
	    &clumpvertoffsets,
	    &clumpvertweights);
    }

    // eval curve, skin and texture
    void eval(
	    UT_FloatArray		    &out,
	    const GA_AttributeOwner	     target,
	    const GA_Offset		     skinprimoff,
	    const GA_OffsetArray	    &skinvertoffsets,
	    const UT_FloatArray		    &skinvertweights,
	    const UT_Vector3		    &uv,
	    const GA_Offset		     curveprimoff,
	    const GA_OffsetListRef	    &curvevertoffsets)
    {
	eval(
	    out,
	    target,
	    skinprimoff,
	    &skinvertoffsets,
	    &skinvertweights,
	    uv,
	    curveprimoff,
	    &curvevertoffsets,
	    GA_INVALID_OFFSET, // no clump offset
	    nullptr, // no clump vert arrays
	    nullptr);
    }

    fpreal evalSkin(
	    const GA_Offset		     skinprimoff,
	    const GA_OffsetArray	    &skinvertoffsets,
	    const UT_FloatArray		    &skinvertweights,
	    const UT_Vector3		    &uv);

    fpreal evalSkinPoint(
	    const GA_Offset		     skinptoff);

    bool evaluatesToSingleValue(
	    GA_AttributeOwner target) const;

    bool needSkin() const;

    bool needUV() const;

    const GA_Attribute* getRequiredAttrib() const;

    bool hasChanged(const CacheData *cache) const;

    bool
    updateOverrideCache(
	    UT_UniquePtr<SOP_ParmOverride::CacheData> &cachedata) const
    {
	if (!cachedata ||
		hasChanged(cachedata.get()))
	{
	    cachedata.reset(getCacheData());
	    return true;
	}
	return false;
    }

private:
    typedef UT_Tuple<int, int> UdimCoord;
    struct MyHashCompare {
	static size_t hash( const UdimCoord& x ) {
	    int xhash = SYSwang_inthash(std::get<0>(x));
	    return SYSwang_inthash(xhash + std::get<1>(x));
	}
	//! True if strings are equal
	static bool equal( const UdimCoord& x, const UdimCoord& y ) {
	    return x==y;
	}
    };
    typedef UT_ConcurrentHashMap<UdimCoord, UT_UniquePtr<TIL_TextureMap>, MyHashCompare> TexPathMap;

    // This eval() accepts pointers to the various offset arrays,
    // so the public functions, which accept references, can pass nullptrs
    // for unused arrays.
    void eval(
	    UT_FloatArray			&out,
	    const GA_AttributeOwner	    	 target,
	    const GA_Offset		    	 skinprimoff,
	    const GA_OffsetArray	    	*skinvertoffsets,
	    const UT_FloatArray		    	*skinvertweights,
	    const UT_Vector3		    	&uv,
	    const GA_Offset		    	 curveprimoff,
	    const GA_OffsetListRef	    	*curvevertoffsets,
	    const GA_Offset		    	 clumpprimoff,
	    const UT_ValArray<GA_OffsetArray>	*clumpvertoffsets,
	    const UT_ValArray<UT_FloatArray>	*clumpvertweights);

    float lookupTexture(UT_Vector3 uv);

    static exint
    findParmIndex(
	    const SOP_NodeParms &parms,
	    const UT_StringHolder &parmname);

    CacheData *getCacheData() const;

    void check();

    const SOP_NodeVerb::CookParms		&cookparms;
    const SOP_NodeParms		    		&parms;
    const UT_StringHolder	    		&baseparmname;
    const GU_Detail		    		*curvegdp;
    const GU_Detail		    		*clumpgdp;
    const GU_Detail		    		*skingdp;
    const GA_ROHandleV3		    		&uvattrib;
    exint			    		 baseparmindex;
    SOP_NodeParms::ParmType	    		 baseparmtype;
    exint 			    		 overrideparmindex;
    exint 			    		 curveattribparmindex;
    exint 			    		 clumpattribparmindex;
    exint 			    		 skinattribparmindex;
    exint 			    		 textureparmindex;

    fpreal			    		 baseparmvalue;
    OverrideType		    		 overridetype;
    UT_StringHolder		    		 curveattribname;
    UT_StringHolder		    		 clumpattribname;
    UT_StringHolder		    		 skinattribname;
    UT_StringHolder		    		 texturepath;
    bool			    		 texturevalid;
    GA_ROHandleF		    		 curveattrib;
    GA_ROHandleF		    		 clumpattrib;
    GA_ROHandleF		    		 skinattrib;
    UT_UniquePtr<TIL_TextureMap>    		 texturemap;
    TexPathMap					 udimmaps;
    //UT_ValArray<UT_UniquePtr<TIL_TextureMap>>	 udimmaps;
    UT_Lock					 udimlock;
    bool					 useudim;

public:
    template<typename T>
    static T
    evalAttrib(
	    const GU_Detail *gdp,
	    const GA_AttributeOwner target,
	    const GA_ROHandleT<T> &attrib,
	    const GA_Offset &primoff,
	    const GA_Offset &vertoff)
    {
	if (!attrib.isValid())
	    return 1.0;

	GA_AttributeOwner owner = attrib.getAttribute()->getOwner();
	if (owner == GA_ATTRIB_VERTEX)
	{
	    return attrib.get(vertoff);
	}
	else if (owner == GA_ATTRIB_POINT)
	{
	    GA_Offset pointoff = gdp->vertexPoint(vertoff);
	    return attrib.get(pointoff);
	}
	else if (owner == GA_ATTRIB_PRIMITIVE)
	{
	    return attrib.get(primoff);
	}
	else // (owner == GA_ATTRIB_DETAIL)
	{
	    return attrib.get(GA_Offset(0));
	}
    }

    template<typename T>
    static T
    interpAttrib(
	    const GU_Detail *gdp,
	    const GA_ROHandleT<T> &attrib,
	    const GA_Offset &primoff,
	    const GA_OffsetArray &vertoffsets,
	    const UT_FloatArray &vertweights,
	    const T defaultvalue=T(0.0f))
    {
	if (!attrib.isValid())
	{
	    return defaultvalue;
	}

	T value;
	GA_AttributeOwner owner = attrib.getAttribute()->getOwner();
	if (owner == GA_ATTRIB_VERTEX)
	{
	    value = 0;
	    for (exint i = 0; i < vertoffsets.entries(); ++i)
	    {
		T vertexvalue = attrib.get(vertoffsets(i));
		value += vertweights(i) * vertexvalue;
	    }
	}
	else if (owner == GA_ATTRIB_POINT)
	{
	    value = 0;
	    for (exint i = 0; i < vertoffsets.entries(); ++i)
	    {
		T pointvalue = attrib.get(
			gdp->vertexPoint(vertoffsets(i)));
		value += vertweights(i) * pointvalue;
	    }
	}
	else if (owner == GA_ATTRIB_PRIMITIVE)
	{
	    if (GAisValid(primoff))
		value = attrib.get(primoff);
	    else
		value = defaultvalue;
	}
	else // (owner == GA_ATTRIB_DETAIL)
	{
	    value = attrib.get(GA_Offset(0));
	}

	return value;
    }

};

#endif

