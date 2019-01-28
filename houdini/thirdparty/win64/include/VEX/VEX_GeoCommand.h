/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_GeoCommand.h ( VEX Library, C++)
 *
 * COMMENTS:	Generic commands and queues to store geometry
 *		edit operations
 */

#ifndef __VEX_GeoCommand__
#define __VEX_GeoCommand__

#include "VEX_API.h"

#include "VEX_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_SharedString.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_PageArray.h>

#include <hboost/variant.hpp>

// Do NOT use VEX_API here for template classes with all code in the header!

enum class VEX_GeoMode
{
    SET,
    ADD,
    MAX,
    MIN,
    MULTIPLY,
    TOGGLE,
    APPEND,

    INVALID
};
enum class VEX_GeoPrimType
{
    POLY_OPEN,
    POLY_CLOSED,
    TET,
    SPHERE,
    CIRCLE,
    TUBE,
    METABALL,
    METASQUAD,
    PACKEDALEMBIC,
    PACKEDDISK,

    INVALID
};
enum class VEX_GeoDictType
{
    DETAIL,
    PRIM,
    PRIMINTRINSIC,
    PRIMGROUP,
    POINT,
    POINTGROUP,
    VERTEX,
    VERTEXGROUP,
    
    INVALID
};
enum class VEX_GeoTypeInfo
{
    NOCHANGE = -1,
    NONE = 0,
    POINT,
    HPOINT,
    VECTOR,
    NORMAL,
    COLOR,
    TRANSFORM,
    QUATERNION,
    INDEXPAIR,
    NONARITHMETIC_INTEGER,
    ARITHMETIC_INTEGER,
    TEXTURE_COORD
};


namespace VEX_Geo
{
    VEX_API VEX_GeoMode modeFromString(const char*);
    VEX_API VEX_GeoPrimType typeFromString(const char*);
    VEX_API VEX_GeoDictType dictFromString(const char*);
    VEX_API VEX_GeoTypeInfo typeinfoFromString(const char*);

} // namespace VEX_Geo

template <VEX_Precision PREC>
class VEX_GeoCommand
{
public:
    VEX_GeoCommand(exint srcelem) { mySourceElem = srcelem; }
    VEX_GeoCommand(): mySourceElem(-1) {}

    VEXint<PREC> elem() const { return mySourceElem; }

protected:
    VEXint<PREC> mySourceElem;
};

template <VEX_Precision PREC>
class VEX_GeoCommandPrimAdd : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandPrimAdd(VEXint<PREC> srcelem, VEX_GeoPrimType primtype)
	: VEX_GeoCommand<PREC>(srcelem)
	, myType(primtype)
    {
    }
    VEX_GeoCommandPrimAdd(): myType(VEX_GeoPrimType::INVALID) {}

    VEX_GeoPrimType type() const { return myType; }

protected:
    VEX_GeoPrimType myType;
};

template <VEX_Precision PREC>
class VEX_GeoCommandPointAdd : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandPointAdd(VEXint<PREC> srcelem, VEXvec3<PREC> pos)
	: VEX_GeoCommand<PREC>(srcelem)
	, myPos(pos)
	, mySrcPt(-1)
    {
    }
    VEX_GeoCommandPointAdd(VEXint<PREC> srcelem, VEXint<PREC> srcpt)
	: VEX_GeoCommand<PREC>(srcelem)
	, myPos(0.0, 0.0, 0.0)
	, mySrcPt(srcpt)
    {
    }
    VEX_GeoCommandPointAdd(): mySrcPt(-1) {}

    VEXvec3<PREC>	pos() const { return myPos; }
    VEXint<PREC>	srcpt() const { return mySrcPt; }

protected:
    // Ideally we'd union, but C++ makes this frustrating.
    VEXint<PREC> mySrcPt;
    VEXvec3<PREC> myPos;
};

template <VEX_Precision PREC>
class VEX_GeoCommandVertexAdd : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandVertexAdd(
    	VEXint<PREC> srcelem, VEXint<PREC> primnum, VEXint<PREC> ptnum)
	: VEX_GeoCommand<PREC>(srcelem)
	, myPrimNum(primnum)
	, myPtNum(ptnum)
    {
    }
    VEX_GeoCommandVertexAdd(): myPrimNum(-1), myPtNum(-1) {}

    VEXint<PREC>	primnum() const { return myPrimNum; }
    VEXint<PREC>	ptnum() const { return myPtNum; }

protected:
    VEXint<PREC> myPrimNum;
    VEXint<PREC> myPtNum;
};

template <VEX_Precision PREC>
class VEX_GeoCommandObjectRemove : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandObjectRemove(VEXint<PREC> srcelem, VEXint<PREC> objnum)
	: VEX_GeoCommand<PREC>(srcelem)
	, myObjNum(objnum)
    {
    }
    VEX_GeoCommandObjectRemove(): myObjNum(-1) {}

    VEXint<PREC> objnum() const { return myObjNum; }

protected:
    VEXint<PREC> myObjNum;
};

template <VEX_Precision PREC, typename T>
class VEX_GeoCommandAttribEdit : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandAttribEdit(
    	VEXint<PREC> srcelem, VEXint<PREC> objnum, VEXint<PREC> secnum,
	VEX_GeoMode mode, T data)
	: VEX_GeoCommand<PREC>(srcelem)
	, myObjNum(objnum)
	, mySecNum(secnum)
	, myMode(mode)
	, myData(data)
    {
    }
    VEX_GeoCommandAttribEdit()
	: myObjNum(-1)
	, mySecNum(-1)
	, myMode(VEX_GeoMode::SET)
	, myData(T()) {}

    VEXint<PREC>	objnum() const { return myObjNum; }
    VEXint<PREC>	secnum() const { return mySecNum; }
    VEX_GeoMode		mode() const { return myMode; }
    T			data() const { return myData; }

protected:
    VEXint<PREC>	myObjNum;
    VEXint<PREC>	mySecNum;
    VEX_GeoMode		myMode;
    T			myData;
};

template <VEX_Precision PREC>
class VEX_GeoCommandVertexRewire : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandVertexRewire(
    	VEXint<PREC> srcelem, VEXint<PREC> primnum,
    	VEXint<PREC> vtxidx, VEXint<PREC> newptnum)
	: VEX_GeoCommand<PREC>(srcelem)
	, myPrimNum(primnum)
	, myVtxIdx(vtxidx)
	, myPtNum(newptnum)
    {
    }
    VEX_GeoCommandVertexRewire(): myPrimNum(-1), myVtxIdx(-1), myPtNum(-1) {}

    // -1 for primnum implies a linear vertex
    VEXint<PREC>	primnum() const { return myPrimNum; }
    VEXint<PREC>	vtxidx() const { return myVtxIdx; }
    VEXint<PREC>	ptnum() const { return myPtNum; }

protected:
    VEXint<PREC>	myPrimNum, myVtxIdx, myPtNum;
};

template <VEX_Precision PREC>
class VEX_GeoCommandAgentXformEdit : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandAgentXformEdit(VEXint<PREC> srcelem, VEXint<PREC> primnum,
                                 const VEXmat4<PREC> &xform, VEXint<PREC> xformidx,
                                 bool worldspace)
        : VEX_GeoCommand<PREC>(srcelem),
          myPrimNum(primnum),
          myData(SingleEdit(xform, xformidx)),
          myWorldSpace(worldspace)
    {
    }
    VEX_GeoCommandAgentXformEdit(VEXint<PREC> srcelem, VEXint<PREC> primnum,
				 const UT_Array<VEXmat4<PREC>> &xforms,
				 bool worldspace)
	: VEX_GeoCommand<PREC>(srcelem),
	  myPrimNum(primnum),
	  myData(FullEdit(xforms)),
	  myWorldSpace(worldspace)
    {
    }
    VEX_GeoCommandAgentXformEdit(): myPrimNum(-1) {}

    VEXint<PREC>         primnum() const { return myPrimNum; }
    const VEXmat4<PREC> &xform() const
    {
	UT_ASSERT(myData.which() == 0);
	return hboost::get<SingleEdit>(myData).first;
    }
    VEXint<PREC>         xformidx() const
    {
	UT_ASSERT(myData.which() == 0);
	return hboost::get<SingleEdit>(myData).second;
    }
    const UT_Array<VEXmat4<PREC>> &xforms() const
    {
	UT_ASSERT(myData.which() == 1);
	return hboost::get<FullEdit>(myData);
    }
    bool                worldspace() const { return myWorldSpace; }

    bool		isSingleEdit() const { return myData.which() == 0; }

protected:
    typedef std::pair<VEXmat4<PREC>,VEXint<PREC>> SingleEdit;
    typedef UT_Array<VEXmat4<PREC>>               FullEdit;

    VEXint<PREC> myPrimNum;
    // Allow setting a single transform or setting all of them
    hboost::variant<SingleEdit, FullEdit>	myData;
    bool                myWorldSpace;
};

template <VEX_Precision PREC>
class VEX_GeoCommandAgentClipEdit : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandAgentClipEdit(VEXint<PREC> srcelem, VEXint<PREC> primnum,
                                const UT_StringArray &clipnames,
                                const UT_Array<VEXfloat<PREC>> &cliptimes,
                                const UT_Array<VEXfloat<PREC>> &clipweights,
                                const UT_StringArray &clipxformgroups,
                                const UT_Array<VEXint<PREC>> &cliplayerids,
                                const UT_Array<VEXint<PREC>> &cliplayermodes,
                                const UT_Array<VEXfloat<PREC>> &cliplayerweights,
                                const UT_Array<VEXint<PREC>> &cliplayerparents)
        : VEX_GeoCommand<PREC>(srcelem),
          myPrimNum(primnum),
          myClipNames(clipnames),
          myClipTimes(cliptimes),
          myClipWeights(clipweights),
          myClipXformGroups(clipxformgroups),
          myClipLayerIds(cliplayerids),
          myClipLayerModes(cliplayermodes),
          myClipLayerWeights(cliplayerweights),
          myClipLayerParents(cliplayerparents)
    {
    }

    VEX_GeoCommandAgentClipEdit() = default;
    VEX_GeoCommandAgentClipEdit(const VEX_GeoCommandAgentClipEdit &) = default;
    VEX_GeoCommandAgentClipEdit(VEX_GeoCommandAgentClipEdit &&) = default;
    VEX_GeoCommandAgentClipEdit &operator=(const VEX_GeoCommandAgentClipEdit &) = default;
    VEX_GeoCommandAgentClipEdit &operator=(VEX_GeoCommandAgentClipEdit &&) = default;

    const VEXint<PREC>			 primnum()	    const { return myPrimNum; }
    const UT_StringArray		&clipnames()	    const { return myClipNames; }
    const UT_Array<VEXfloat<PREC>>	&cliptimes()	    const { return myClipTimes; }
    const UT_Array<VEXfloat<PREC>> 	&clipweights()	    const { return myClipWeights; }
    const UT_StringArray		&clipxformgroups()  const { return myClipXformGroups; }
    const UT_Array<VEXint<PREC>>	&cliplayerids()	    const { return myClipLayerIds; }
    const UT_Array<VEXint<PREC>>	&cliplayermodes()   const { return myClipLayerModes; }
    const UT_Array<VEXfloat<PREC>>	&cliplayerweights() const { return myClipLayerWeights; }
    const UT_Array<VEXint<PREC>>	&cliplayerparents() const { return myClipLayerParents; }

protected:
    VEXint<PREC>		myPrimNum;
    UT_StringArray		myClipNames;
    UT_Array<VEXfloat<PREC>>	myClipTimes;
    UT_Array<VEXfloat<PREC>>	myClipWeights;
    UT_StringArray		myClipXformGroups;
    UT_Array<VEXint<PREC>>	myClipLayerIds;
    UT_Array<VEXint<PREC>>	myClipLayerModes;
    UT_Array<VEXfloat<PREC>>	myClipLayerWeights;
    UT_Array<VEXint<PREC>>	myClipLayerParents;
};

class CL_Clip;

template <VEX_Precision PREC>
class VEX_GeoCommandAgentAddClip : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandAgentAddClip(
	    VEXint<PREC> srcelem,
	    VEXint<PREC> primnum,
	    const UT_StringHolder &clip_name,
	    const UT_StringHolder &clip_path,
	    bool keep_ref)
        : VEX_GeoCommand<PREC>(srcelem)
        , myPrimNum(primnum)
        , myClipName(clip_name)
        , myClipPath(clip_path)
	, myKeepRef(keep_ref)
    {
    }
    VEX_GeoCommandAgentAddClip()
	: VEX_GeoCommand<PREC>()
	, myPrimNum(-1)
	, myClipName()
	, myClipPath()
	, myKeepRef(true)
    {
    }

    VEXint<PREC> primnum() const { return myPrimNum; }

    VEXint<PREC>    myPrimNum;
    UT_StringHolder myClipName;
    UT_StringHolder myClipPath;
    bool	    myKeepRef;
};

template <VEX_Precision PREC>
class VEX_API VEX_GeoAttribEdit
{
public:
    //
    // A variant of all VEXint,VEXfloat,VEXvecN,VEXmatN types
    class DefaultValue
    {
    public:
	DefaultValue() { memset(f, 0, sizeof(VEXfloat<PREC>)*16); i = 0; }

	void		set(VEXint<PREC> v) { i = v; }
	void		set(VEXfloat<PREC> v) { f[0] = v; }
	void		set(VEXvec2<PREC> v) { memcpy(f, &v, sizeof(v)); }
	void		set(VEXvec3<PREC> v) { memcpy(f, &v, sizeof(v)); }
	void		set(VEXvec4<PREC> v) { memcpy(f, &v, sizeof(v)); }
	void		set(VEXmat2<PREC> v) { memcpy(f, &v, sizeof(v)); }
	void		set(VEXmat3<PREC> v) { memcpy(f, &v, sizeof(v)); }
	void		set(VEXmat4<PREC> v) { memcpy(f, &v, sizeof(v)); }

	VEXfloat<PREC> f[16];
	VEXint<PREC>   i;
    };

    VEX_GeoAttribEdit(VEX_GeoDictType dict, const char *name);
    ~VEX_GeoAttribEdit();
    void		setTypeInfo(VEXint<PREC> srcelem,
				    const char *typeinfo);

    void		addAttribute(VEXint<PREC> srcelem,
				    VEX_Type type,
				    bool isarray,
				    const DefaultValue &value);
				    
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXfloat<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXint<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXvec2<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXvec3<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXvec4<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXmat2<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXmat3<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   VEXmat4<PREC> data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   const char *data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   const UT_Array<VEXfloat<PREC>> *data);
    void                appendEdit(VEXint<PREC> srcelem,
                                   VEXint<PREC> objnum,
                                   VEXint<PREC> secnum,
                                   VEX_GeoMode mode,
                                   const UT_Array<VEXvec2<PREC>> *data);
    void                appendEdit(VEXint<PREC> srcelem,
                                   VEXint<PREC> objnum,
                                   VEXint<PREC> secnum,
                                   VEX_GeoMode mode,
                                   const UT_Array<VEXvec3<PREC>> *data);
    void                appendEdit(VEXint<PREC> srcelem,
                                   VEXint<PREC> objnum,
                                   VEXint<PREC> secnum,
                                   VEX_GeoMode mode,
                                   const UT_Array<VEXvec4<PREC>> *data);
    void                appendEdit(VEXint<PREC> srcelem,
                                   VEXint<PREC> objnum,
                                   VEXint<PREC> secnum,
                                   VEX_GeoMode mode,
                                   const UT_Array<VEXmat2<PREC>> *data);
    void                appendEdit(VEXint<PREC> srcelem,
                                   VEXint<PREC> objnum,
                                   VEXint<PREC> secnum,
                                   VEX_GeoMode mode,
                                   const UT_Array<VEXmat3<PREC>> *data);
    void                appendEdit(VEXint<PREC> srcelem,
                                   VEXint<PREC> objnum,
                                   VEXint<PREC> secnum,
                                   VEX_GeoMode mode,
                                   const UT_Array<VEXmat4<PREC>> *data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   const UT_Array<VEXint<PREC>> *data);
    void		appendEdit(VEXint<PREC> srcelem,
				   VEXint<PREC> objnum,
				   VEXint<PREC> secnum,
				   VEX_GeoMode mode,
				   const UT_Array<const char *> *data);


    // This may steal the command list from the source!
    void		merge(VEX_GeoAttribEdit &src);

    void		sort();

    void		clear();

    // These convert our command list to the specified type.
    //
    // If the stored VEX_Type matches the return type, the result will be
    // valid. Otherwise nullptr is returned.
    
    // Functions:
    // fCmd, iCmd, v2Cmd, v3Cmd, v4Cmd, m2Cmd, m3Cmd, m4Cmd, sCmd
    // afCmd, aiCmd, av2Cmd, av3Cmd, av4Cmd, am2Cmd, am3Cmd, am4Cmd, asCmd
#define VEX_GEOATTRIBEDIT_CASE(vtype, type, name, isarray) \
    UT_Array<VEX_GeoCommandAttribEdit<PREC, type>> * name##Cmd()\
    {\
	if (myType == vtype && myIsArray == isarray)\
	    return (UT_Array<VEX_GeoCommandAttribEdit<PREC, type>>*) myCmdList;\
	return nullptr;\
    }
#define VEX_GEOATTRIBEDIT_CASE_DIRECT(ty, name) \
    VEX_GEOATTRIBEDIT_CASE(VEX_VexTypeResolver<VEX##ty<PREC>>::type,VEX##ty<PREC>,name,false)
#define VEX_GEOATTRIBEDIT_CASE_ARRAY(ty, name) \
    VEX_GEOATTRIBEDIT_CASE(VEX_VexTypeResolver<VEX##ty<PREC>>::type,const UT_Array<VEX##ty<PREC>>*,name,true)

    VEX_GEOATTRIBEDIT_CASE_DIRECT(float, f)
    VEX_GEOATTRIBEDIT_CASE_DIRECT(int, i)
    VEX_GEOATTRIBEDIT_CASE_DIRECT(vec2, v2)
    VEX_GEOATTRIBEDIT_CASE_DIRECT(vec3, v3)
    VEX_GEOATTRIBEDIT_CASE_DIRECT(vec4, v4)
    VEX_GEOATTRIBEDIT_CASE_DIRECT(mat2, m2)
    VEX_GEOATTRIBEDIT_CASE_DIRECT(mat3, m3)
    VEX_GEOATTRIBEDIT_CASE_DIRECT(mat4, m4)

    VEX_GEOATTRIBEDIT_CASE(VEX_TYPE_STRING, const char*, s, false)

    VEX_GEOATTRIBEDIT_CASE_ARRAY(float, af)
    VEX_GEOATTRIBEDIT_CASE_ARRAY(int, ai)
    VEX_GEOATTRIBEDIT_CASE_ARRAY(vec2, av2)
    VEX_GEOATTRIBEDIT_CASE_ARRAY(vec3, av3)
    VEX_GEOATTRIBEDIT_CASE_ARRAY(vec4, av4)
    VEX_GEOATTRIBEDIT_CASE_ARRAY(mat2, am2)
    VEX_GEOATTRIBEDIT_CASE_ARRAY(mat3, am3)
    VEX_GEOATTRIBEDIT_CASE_ARRAY(mat4, am4)
    
    VEX_GEOATTRIBEDIT_CASE(VEX_TYPE_STRING, const UT_Array<const char*>*, as, true)

#undef VEX_GEOATTRIBEDIT_CASE_ARRAY
#undef VEX_GEOATTRIBEDIT_CASE_DIRECT
#undef VEX_GEOATTRIBEDIT_CASE

    VEX_GeoDictType	 dict() const { return myDict; }
    const char		*name() const { return myName; }
    VEX_Type		 type() const { return myType; }
    bool		 isArray() const { return myIsArray; }
    bool		 shouldcreate() const { return myDoCreate; }
    VEX_GeoTypeInfo	 typeinfo() const { return myTypeInfo; }
    const DefaultValue	&defaultValue() const { return myDefValue; }

protected:
    // The earliest proc that refered to us.
    VEXint<PREC>	 myDefiningElem;
    VEX_GeoDictType	 myDict;
    UT_String		 myName;
    VEX_Type		 myType;
    bool		 myIsArray;
    // Who set the typeinfo.
    // Because we are logically overwriting, it makes sense for this
    // to be the last person to set it!
    VEXint<PREC>	 myTypeInfoElem; 
    VEX_GeoTypeInfo	 myTypeInfo;

    void		*myCmdList;
    // The proc that marked us for creation
    VEXint<PREC>	 myCreationElem;
    bool		 myDoCreate;
    DefaultValue	 myDefValue;
};

template <VEX_Precision PREC>
class VEX_GeoCommandEdgeGroupEdit : public VEX_GeoCommand<PREC>
{
public:
    VEX_GeoCommandEdgeGroupEdit(
            VEXint<PREC> srcelem,
            VEXint<PREC> ptnum0, VEXint<PREC> ptnum1,
            VEXint<PREC> data)
        : VEX_GeoCommand<PREC>(srcelem)
        , myPtNum0(ptnum0)
        , myPtNum1(ptnum1)
        , myData(data)
    {
    }
    VEX_GeoCommandEdgeGroupEdit()
	: VEX_GeoCommand<PREC>(), myPtNum0(-1) , myPtNum1(-1), myData(-1)
    {}

    VEXint<PREC> ptNum0() const { return myPtNum0; }
    VEXint<PREC> ptNum1() const { return myPtNum1; }
    VEXint<PREC> data() const { return myData; }

protected:
    VEXint<PREC> myPtNum0;
    VEXint<PREC> myPtNum1;
    VEXint<PREC> myData;
};

template <VEX_Precision PREC>
class VEX_API VEX_GeoEdgeGroupEdit
{
public:
    VEX_GeoEdgeGroupEdit(const UT_StringHolder &groupname): myGroupName(groupname) {}

    const UT_StringHolder &groupName() const { return myGroupName; }

    const UT_Array<VEX_GeoCommandEdgeGroupEdit<PREC>> &cmd() const { return myCmdList; }

    void appendEdit(VEXint<PREC> srcelem,
            VEXint<PREC> ptnum0,
            VEXint<PREC> ptnum1,
            VEXint<PREC> data);

    void merge(VEX_GeoEdgeGroupEdit &src);

    void sort();

protected:
    UT_StringHolder     myGroupName;
    UT_Array<VEX_GeoCommandEdgeGroupEdit<PREC>> myCmdList;
};

template <VEX_Precision PREC>
class VEX_API VEX_GeoCommandQueue
{
public:
    VEX_GeoCommandQueue();
    ~VEX_GeoCommandQueue();

    // Converts the given string to one in our string table, ensuring
    // the pointer remains valid and avoids duplication.
    const char *hardenString(const char *str);
    
    // Creates a new array which is owned by this queue.  Does
    // not de-duplicate, but will return 0 for empty lists.
    const UT_Array<VEXfloat<PREC>> *hardenArray(const UT_Array<VEXfloat<PREC>> &arr);
    const UT_Array<VEXvec2<PREC>> *hardenArray(const UT_Array<VEXvec2<PREC>> &arr);
    const UT_Array<VEXvec3<PREC>> *hardenArray(const UT_Array<VEXvec3<PREC>> &arr);
    const UT_Array<VEXvec4<PREC>> *hardenArray(const UT_Array<VEXvec4<PREC>> &arr);
    const UT_Array<VEXmat2<PREC>> *hardenArray(const UT_Array<VEXmat2<PREC>> &arr);
    const UT_Array<VEXmat3<PREC>> *hardenArray(const UT_Array<VEXmat3<PREC>> &arr);
    const UT_Array<VEXmat4<PREC>> *hardenArray(const UT_Array<VEXmat4<PREC>> &arr);
    const UT_Array<VEXint<PREC>>  *hardenArray(const UT_Array<VEXint<PREC>> &arr);
    // ALSO hardens the contents:
    const UT_Array<const char *>   *hardenArray(const UT_Array<const char *> &arr);

    // Merges the src into us.  Note that the result may still
    // reference strings in the stringtable of src, so src must *not*
    // be deleted until the queue is processed!
    // This also may destroy src in the merging if stealing is more
    // efficient.
    void	merge(VEX_GeoCommandQueue &src);
    // Ensure our commands are in srcelem order.
    void	sort();

    bool	isEmpty() const
    {
	return myPointAdd.isEmpty() &&
	       myPrimAdd.isEmpty() &&
	       myVertexAdd.isEmpty() &&
	       myVertexRewire.isEmpty() &&
	       myAttribEdit.isEmpty() &&
	       myEdgeGroupEdit.isEmpty() &&
               myAgentXformEdit.isEmpty() &&
               myAgentClipEdit.isEmpty() &&
               myAgentAddClip.isEmpty() &&
	       myPointRemove.isEmpty() &&
               myPointWithPrimRemove.isEmpty() &&
	       myPrimRemove.isEmpty() &&
	       myPrimWithPointRemove.isEmpty() &&
	       myLocalVariables.empty() &&
	       !myNewVisualizers;
    }

    // These return the new element number *relative* to the srcelem!
    VEXint<PREC>	addPrim(VEXint<PREC> srcelem, VEX_GeoPrimType primtype);
    VEXint<PREC>	addPoint(VEXint<PREC> srcelem, VEXvec3<PREC> pos);
    VEXint<PREC>	addPoint(VEXint<PREC> srcelem, VEXint<PREC> srcpt);
    VEXint<PREC>	addVertex(VEXint<PREC> srcelem, VEXint<PREC> primnum, VEXint<PREC> ptnum);

    void	setEdgeGroup(
                    VEXint<PREC> srcelem,
                    const UT_StringHolder &groupname,
                    VEXint<PREC> ptnum0, VEXint<PREC> ptnum1,
                    VEXint<PREC> data);

    void	addLocalVariable(VEXint<PREC> srcelem, const char *attrib, const char *local);
    void	addVisualizer(const UT_StringHolder &visualizer);

    // If the given primnum is -1, it will treat vtxidx as a linear vertex
    void	rewireVertex(
    	VEXint<PREC> srcelem, VEXint<PREC> primum,
    	VEXint<PREC> vtxidx, VEXint<PREC> ptnum);

    void	removePoint(VEXint<PREC> srcelem, VEXint<PREC> ptnum, bool includeprims);
    void	removePrim(VEXint<PREC> srcelem, VEXint<PREC> primnum, bool includepoints);

    void        setAgentTransform(VEXint<PREC> srcelem, VEXint<PREC> primnum,
                                  const VEXmat4<PREC> &xform, VEXint<PREC> xformidx,
                                  bool worldspace);

    void        setAgentTransforms(VEXint<PREC> srcelem, VEXint<PREC> primnum,
				   const UT_Array<VEXmat4<PREC>> &xforms,
                                   bool worldspace);

    void        setAgentClips(VEXint<PREC> srcelem, VEXint<PREC> primnum,
                              const UT_StringArray &clipnames,
                              const UT_Array<VEXfloat<PREC>> &cliptimes,
                              const UT_Array<VEXfloat<PREC>> &clipweights,
                              const UT_StringArray &clipxformgroups,
                              const UT_Array<VEXint<PREC>> &cliplayerids,
                              const UT_Array<VEXint<PREC>> &cliplayermodes,
                              const UT_Array<VEXfloat<PREC>> &cliplayerweights,
                              const UT_Array<VEXint<PREC>> &cliplayerparents);

    void        agentAddClip(
		    VEXint<PREC> srcelem,
		    VEXint<PREC> primnum,
		    const UT_StringHolder &clip_name,
		    const UT_StringHolder &clip,
		    const bool keep_ref);

    VEX_GeoAttribEdit<PREC> *findEdit(
    	VEX_GeoDictType dict,
	const char *name);
    VEX_GeoAttribEdit<PREC> *findOrCreateEdit(
    	VEX_GeoDictType dict,
	const char *name);

    VEX_GeoEdgeGroupEdit<PREC> *findOrCreateEdgeGroupEdit(const UT_StringHolder &groupname);

    // Creators
    UT_Array<VEX_GeoCommandPointAdd<PREC>>	myPointAdd;
    UT_Array<VEX_GeoCommandPrimAdd<PREC>>	myPrimAdd;
    UT_Array<VEX_GeoCommandVertexAdd<PREC>>	myVertexAdd;
    // Requested local variables.
    struct	VariableEntry
    {
	VEXint<PREC> elem;
	const char *local;		// hardened!
    };
    UT_StringMap<VariableEntry>		myLocalVariables;

    UT_StringSet			*myNewVisualizers;

    // Edits
    UT_Array<VEX_GeoCommandVertexRewire<PREC>>	myVertexRewire;
    UT_Array<VEX_GeoAttribEdit<PREC> *>		myAttribEdit;
    UT_Array<VEX_GeoEdgeGroupEdit<PREC> *>	        myEdgeGroupEdit;
    UT_Array<VEX_GeoCommandAgentXformEdit<PREC>>      myAgentXformEdit;
    UT_Array<VEX_GeoCommandAgentClipEdit<PREC>>       myAgentClipEdit;
    UT_Array<VEX_GeoCommandAgentAddClip<PREC>>	myAgentAddClip;

    // Destructors
    // Note we don't have to sort these.  We do need to track
    // the source element to be able to lookup the relevant point
    // or primitive number, however.
    UT_Array<VEX_GeoCommandObjectRemove<PREC>> myPointRemove;
    UT_Array<VEX_GeoCommandObjectRemove<PREC>> myPointWithPrimRemove;
    UT_Array<VEX_GeoCommandObjectRemove<PREC>> myPrimRemove;
    UT_Array<VEX_GeoCommandObjectRemove<PREC>> myPrimWithPointRemove;

    typedef UT_PageArray<
    	/* DATA_T */ VEXint<PREC>,
    	/* TSIZE */ 1,
    	/* TABLEHARDENED (owns its data) */ true,
    	/* PAGESHARDENED */ false,
    	/* THEPAGEBITS (2^10 tuples/page) */ 10,
    	/* INDEX_T */ VEXint<PREC>
    	>	BlockArray;
    BlockArray				myNetPoints;
    BlockArray				myNetPrims;
    BlockArray				myNetVertices;

    int					myNumPrim, myNumPoint, myNumVertex;

private:
    UT_SharedString			*myStrings;
    UT_Array< UT_Array<VEXint<PREC>> *>		myIntArrays;
    UT_Array< UT_Array<VEXfloat<PREC>> *>	myFloatArrays;
    UT_Array< UT_Array<VEXvec2<PREC>> *>	myV2Arrays;
    UT_Array< UT_Array<VEXvec3<PREC>> *>	myV3Arrays;
    UT_Array< UT_Array<VEXvec4<PREC>> *>	myV4Arrays;
    UT_Array< UT_Array<VEXmat2<PREC>> *>	myM2Arrays;
    UT_Array< UT_Array<VEXmat3<PREC>> *>	myM3Arrays;
    UT_Array< UT_Array<VEXmat4<PREC>> *>	myM4Arrays;
    UT_Array< UT_Array<const char *> *>	myCstArrays;
};

VEX_EXTERN_TEMPLATE(VEX_GeoAttribEdit<VEX_32>);
VEX_EXTERN_TEMPLATE(VEX_GeoEdgeGroupEdit<VEX_32>);
VEX_EXTERN_TEMPLATE(VEX_GeoCommandQueue<VEX_32>);
VEX_EXTERN_TEMPLATE(VEX_GeoAttribEdit<VEX_64>);
VEX_EXTERN_TEMPLATE(VEX_GeoEdgeGroupEdit<VEX_64>);
VEX_EXTERN_TEMPLATE(VEX_GeoCommandQueue<VEX_64>);

#endif
