/* Automagically Generated by generate_proto.py
 * Do not Edit
 */
#pragma once

#include <SOP/SOP_NodeVerb.h>
#include <OP/OP_Utils.h>
#include <PRM/PRM_Parm.h>
#include <UT/UT_IStream.h>
#include <UT/UT_NTStreamUtil.h>
#include <UT/UT_Ramp.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringStream.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Types.h>

using namespace UT::Literal;

class DEP_MicroNode;
namespace SOP_AttribFromVolumeEnums
{
    enum class Type
    {
        FLOAT = 0,
        INT,
        VECTOR
    };
    enum class MonoRampMode
    {
        CLAMP = 0,
        PERIODIC
    };
    enum class Monopreset
    {
        NONE = 0,
        INCREASING,
        DESCREASING,
        HILL,
        VALLEY,
        STEP,
        SQUARE
    };
    enum class VectorRampMode
    {
        CLAMP = 0,
        PERIODIC
    };
    enum class Vectorpreset
    {
        NONE = 0,
        _FALSE,
        PINK,
        MONO,
        BLACKBODY,
        BIPARTITE
    };
}


class SOP_API SOP_AttribFromVolumeParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_AttribFromVolumeParms()
    {
        myGroup = ""_sh;
        myField = ""_sh;
        myName = "Cd"_sh;
        myCreateVarMap = false;
        myVarName = ""_sh;
        myType = 0;
        mySize = 3;
        myDefault = UT_Vector4D(0,0,0,0);
        myRangeIn = UT_Vector2D(0,1);
        myMonoEnable = false;
        myMonoRampMode = 0;
        myMonopreset = 0;
        myMonoRamp = UT_SharedPtr<UT_Ramp>(0);
        myVectorEnable = false;
        myVectorRampMode = 0;
        myVectorpreset = 0;
        myVectorRamp = UT_SharedPtr<UT_Ramp>(0);
        myRangeOut = UT_Vector2D(0,1);

    }

    explicit SOP_AttribFromVolumeParms(const SOP_AttribFromVolumeParms &) = default;

    virtual ~SOP_AttribFromVolumeParms() {}

    bool operator==(const SOP_AttribFromVolumeParms &src) const
    {
        if (myGroup != src.myGroup) return false;
        if (myField != src.myField) return false;
        if (myName != src.myName) return false;
        if (myCreateVarMap != src.myCreateVarMap) return false;
        if (myVarName != src.myVarName) return false;
        if (myType != src.myType) return false;
        if (mySize != src.mySize) return false;
        if (myDefault != src.myDefault) return false;
        if (myRangeIn != src.myRangeIn) return false;
        if (myMonoEnable != src.myMonoEnable) return false;
        if (myMonoRampMode != src.myMonoRampMode) return false;
        if (myMonopreset != src.myMonopreset) return false;
        if (myMonoRamp != src.myMonoRamp) return false;
        if (myVectorEnable != src.myVectorEnable) return false;
        if (myVectorRampMode != src.myVectorRampMode) return false;
        if (myVectorpreset != src.myVectorpreset) return false;
        if (myVectorRamp != src.myVectorRamp) return false;
        if (myRangeOut != src.myRangeOut) return false;

        return true;
    }
    bool operator!=(const SOP_AttribFromVolumeParms &src) const
    {
        return !operator==(src);
    }
    using Type = SOP_AttribFromVolumeEnums::Type;
    using MonoRampMode = SOP_AttribFromVolumeEnums::MonoRampMode;
    using Monopreset = SOP_AttribFromVolumeEnums::Monopreset;
    using VectorRampMode = SOP_AttribFromVolumeEnums::VectorRampMode;
    using Vectorpreset = SOP_AttribFromVolumeEnums::Vectorpreset;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myGroup, node, "group", time, 0);
        else myGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myField, node, "field", time, 0);
        else myField = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myName, node, "name", time, 0);
        else myName = "Cd"_sh;
        if (true)
            OP_Utils::evalOpParm(myCreateVarMap, node, "createvarmap", time, 0);
        else myCreateVarMap = false;
        if (true && ( (!(((getCreateVarMap()==0)))) ) )
            OP_Utils::evalOpParm(myVarName, node, "varname", time, 0);
        else myVarName = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myType, node, "type", time, 0);
        else myType = 0;
        if (true)
            OP_Utils::evalOpParm(mySize, node, "size", time, 0);
        else mySize = 3;
        if (true)
            OP_Utils::evalOpParm(myDefault, node, "default", time, 0);
        else myDefault = UT_Vector4D(0,0,0,0);
        if (true)
            OP_Utils::evalOpParm(myRangeIn, node, "rangein", time, 0);
        else myRangeIn = UT_Vector2D(0,1);
        if (true)
            OP_Utils::evalOpParm(myMonoEnable, node, "monoenable", time, 0);
        else myMonoEnable = false;
        if (true && ( (!(((getMonoEnable()==0)))) ) )
            OP_Utils::evalOpParm(myMonoRampMode, node, "monorampmode", time, 0);
        else myMonoRampMode = 0;
        if (true && ( (!(((getMonoEnable()==0)))) ) )
            OP_Utils::evalOpParm(myMonopreset, node, "monopreset", time, 0);
        else myMonopreset = 0;
        if (true && ( (!(((getMonoEnable()==0)))) ) )
            OP_Utils::evalOpParm(myMonoRamp, node, "monoramp", time, 0);
        else myMonoRamp = UT_SharedPtr<UT_Ramp>(0);
        if (true)
            OP_Utils::evalOpParm(myVectorEnable, node, "vectorenable", time, 0);
        else myVectorEnable = false;
        if (true && ( (!(((getVectorEnable()==0)))) ) )
            OP_Utils::evalOpParm(myVectorRampMode, node, "vectorrampmode", time, 0);
        else myVectorRampMode = 0;
        if (true && ( (!(((getVectorEnable()==0)))) ) )
            OP_Utils::evalOpParm(myVectorpreset, node, "vectorpreset", time, 0);
        else myVectorpreset = 0;
        if (true && ( (!(((getVectorEnable()==0)))) ) )
            OP_Utils::evalOpParm(myVectorRamp, node, "vectorramp", time, 0);
        else myVectorRamp = UT_SharedPtr<UT_Ramp>(0);
        if (true)
            OP_Utils::evalOpParm(myRangeOut, node, "rangeout", time, 0);
        else myRangeOut = UT_Vector2D(0,1);

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_AttribFromVolumeParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myGroup);
                break;
            case 1:
                coerceValue(value, myField);
                break;
            case 2:
                coerceValue(value, myName);
                break;
            case 3:
                coerceValue(value, myCreateVarMap);
                break;
            case 4:
                coerceValue(value, myVarName);
                break;
            case 5:
                coerceValue(value, myType);
                break;
            case 6:
                coerceValue(value, mySize);
                break;
            case 7:
                coerceValue(value, myDefault);
                break;
            case 8:
                coerceValue(value, myRangeIn);
                break;
            case 9:
                coerceValue(value, myMonoEnable);
                break;
            case 10:
                coerceValue(value, myMonoRampMode);
                break;
            case 11:
                coerceValue(value, myMonopreset);
                break;
            case 12:
                coerceValue(value, myMonoRamp);
                break;
            case 13:
                coerceValue(value, myVectorEnable);
                break;
            case 14:
                coerceValue(value, myVectorRampMode);
                break;
            case 15:
                coerceValue(value, myVectorpreset);
                break;
            case 16:
                coerceValue(value, myVectorRamp);
                break;
            case 17:
                coerceValue(value, myRangeOut);
                break;

        }
    }

    void        getParmValue(exint idx, exint &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, fpreal &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_Vector2D &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_Vector3D &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_Vector4D &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_Matrix2D &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_Matrix3D &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_Matrix4D &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_StringHolder &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, UT_SharedPtr<UT_Ramp> &value) const
    { doGetParmValue(idx, value); }
    void        getParmValue(exint idx, PRM_DataItemHandle &value) const
    { doGetParmValue(idx, value); }

    template <typename T>
    void
    doSetParmValue(exint idx, const T &value) 
    {
        switch (idx)
        {
            case 0:
                coerceValue(myGroup, value);
                break;
            case 1:
                coerceValue(myField, value);
                break;
            case 2:
                coerceValue(myName, value);
                break;
            case 3:
                coerceValue(myCreateVarMap, value);
                break;
            case 4:
                coerceValue(myVarName, value);
                break;
            case 5:
                coerceValue(myType, value);
                break;
            case 6:
                coerceValue(mySize, value);
                break;
            case 7:
                coerceValue(myDefault, value);
                break;
            case 8:
                coerceValue(myRangeIn, value);
                break;
            case 9:
                coerceValue(myMonoEnable, value);
                break;
            case 10:
                coerceValue(myMonoRampMode, value);
                break;
            case 11:
                coerceValue(myMonopreset, value);
                break;
            case 12:
                coerceValue(myMonoRamp, value);
                break;
            case 13:
                coerceValue(myVectorEnable, value);
                break;
            case 14:
                coerceValue(myVectorRampMode, value);
                break;
            case 15:
                coerceValue(myVectorpreset, value);
                break;
            case 16:
                coerceValue(myVectorRamp, value);
                break;
            case 17:
                coerceValue(myRangeOut, value);
                break;

        }
    }

    void        setParmValue(exint idx, const exint &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const fpreal &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_Vector2D &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_Vector3D &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_Vector4D &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_Matrix2D &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_Matrix3D &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_Matrix4D &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_StringHolder &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const UT_SharedPtr<UT_Ramp> &value) 
    { doSetParmValue(idx, value); }
    void        setParmValue(exint idx, const PRM_DataItemHandle &value) 
    { doSetParmValue(idx, value); }

    virtual exint    getNumParms() const
    {
        return 18;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "group";
            case 1:
                return "field";
            case 2:
                return "name";
            case 3:
                return "createvarmap";
            case 4:
                return "varname";
            case 5:
                return "type";
            case 6:
                return "size";
            case 7:
                return "default";
            case 8:
                return "rangein";
            case 9:
                return "monoenable";
            case 10:
                return "monorampmode";
            case 11:
                return "monopreset";
            case 12:
                return "monoramp";
            case 13:
                return "vectorenable";
            case 14:
                return "vectorrampmode";
            case 15:
                return "vectorpreset";
            case 16:
                return "vectorramp";
            case 17:
                return "rangeout";

        }
        return 0;
    }

    virtual ParmType getParmType(exint fieldnum) const
    {
        switch (fieldnum)
        {
                case 0:
                    return PARM_STRING;
                case 1:
                    return PARM_STRING;
                case 2:
                    return PARM_STRING;
                case 3:
                    return PARM_INTEGER;
                case 4:
                    return PARM_STRING;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_INTEGER;
                case 7:
                    return PARM_VECTOR4;
                case 8:
                    return PARM_VECTOR2;
                case 9:
                    return PARM_INTEGER;
                case 10:
                    return PARM_INTEGER;
                case 11:
                    return PARM_INTEGER;
                case 12:
                    return PARM_RAMP;
                case 13:
                    return PARM_INTEGER;
                case 14:
                    return PARM_INTEGER;
                case 15:
                    return PARM_INTEGER;
                case 16:
                    return PARM_RAMP;
                case 17:
                    return PARM_VECTOR2;

        }
        return PARM_UNSUPPORTED;
    }

    // Boiler plate to load individual types.
    static void  loadData(UT_IStream &is, int64 &v)
    { is.bread(&v, 1); }
    static void  loadData(UT_IStream &is, bool &v)
    { int64     iv; is.bread(&iv, 1); v = iv; }
    static void  loadData(UT_IStream &is, fpreal64 &v)
    { is.bread<fpreal64>(&v, 1); }
    static void  loadData(UT_IStream &is, UT_Vector2D &v)
    { is.bread<fpreal64>(&v.x(), 1); is.bread<fpreal64>(&v.y(), 1); }
    static void  loadData(UT_IStream &is, UT_Vector3D &v)
    { is.bread<fpreal64>(&v.x(), 1); is.bread<fpreal64>(&v.y(), 1);
      is.bread<fpreal64>(&v.z(), 1); }
    static void  loadData(UT_IStream &is, UT_Vector4D &v)
    { is.bread<fpreal64>(&v.x(), 1); is.bread<fpreal64>(&v.y(), 1);
      is.bread<fpreal64>(&v.z(), 1); is.bread<fpreal64>(&v.w(), 1); }
    static void  loadData(UT_IStream &is, UT_Matrix2D &v)
    { for (int r = 0; r < 2; r++) for (int c = 0; c < 2; c++) is.bread<fpreal64>(&v(r, c), 1); }
    static void  loadData(UT_IStream &is, UT_Matrix3D &v)
    { for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) is.bread<fpreal64>(&v(r, c), 1); }
    static void  loadData(UT_IStream &is, UT_Matrix4D &v)
    { for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) is.bread<fpreal64>(&v(r, c), 1); }
    static void  loadData(UT_IStream &is, UT_Vector2I &v)
    { is.bread<int64>(&v.x(), 1); is.bread<int64>(&v.y(), 1); }
    static void  loadData(UT_IStream &is, UT_Vector3I &v)
    { is.bread<int64>(&v.x(), 1); is.bread<int64>(&v.y(), 1);
      is.bread<int64>(&v.z(), 1); }
    static void  loadData(UT_IStream &is, UT_Vector4I &v)
    { is.bread<int64>(&v.x(), 1); is.bread<int64>(&v.y(), 1);
      is.bread<int64>(&v.z(), 1); is.bread<int64>(&v.w(), 1); }
    static void  loadData(UT_IStream &is, UT_StringHolder &v)
    { is.bread(v); }
    static void  loadData(UT_IStream &is, UT_SharedPtr<UT_Ramp> &v)
    {   UT_StringHolder   rampdata;
        loadData(is, rampdata);
        if (rampdata.isstring())
        {
            v.reset(new UT_Ramp());
            UT_IStream  istr((const char *) rampdata, rampdata.length(), UT_ISTREAM_ASCII);
            v->load(istr);
        }
        else v.reset();
    }
    static void  loadData(UT_IStream &is, PRM_DataItemHandle &v)
    {   UT_StringHolder   data;
        loadData(is, data);
        if (data.isstring())
        {
            // Find the data type.
            char *colon = UT_StringWrap(data).findChar(':');
            if (colon)
            {
                int             typelen = colon - data.buffer();
                UT_WorkBuffer   type;
                type.strncpy(data.buffer(), typelen);
                UT_IStream  istr(((const char *) data) + typelen + 1, data.length() - (typelen + 1), UT_ISTREAM_BINARY);
                
                v = PRM_DataFactory::parseBinary(type.buffer(), istr);
            }
        }
        else v.reset();
    }

    static void  saveData(std::ostream &os, int64 v)
    { UTwrite(os, &v); }
    static void  saveData(std::ostream &os, bool v)
    { int64 iv = v; UTwrite(os, &iv); }
    static void  saveData(std::ostream &os, fpreal64 v)
    { UTwrite<fpreal64>(os, &v); }
    static void  saveData(std::ostream &os, UT_Vector2D v)
    { UTwrite<fpreal64>(os, &v.x()); UTwrite<fpreal64>(os, &v.y()); }
    static void  saveData(std::ostream &os, UT_Vector3D v)
    { UTwrite<fpreal64>(os, &v.x()); UTwrite<fpreal64>(os, &v.y());
      UTwrite<fpreal64>(os, &v.z()); }
    static void  saveData(std::ostream &os, UT_Vector4D v)
    { UTwrite<fpreal64>(os, &v.x()); UTwrite<fpreal64>(os, &v.y());
      UTwrite<fpreal64>(os, &v.z()); UTwrite<fpreal64>(os, &v.w()); }
    static void  saveData(std::ostream &os, UT_Matrix2D v)
    { for (int r = 0; r < 2; r++) for (int c = 0; c < 2; c++) UTwrite<fpreal64>(os, &v(r, c)); }
    static void  saveData(std::ostream &os, UT_Matrix3D v)
    { for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) UTwrite<fpreal64>(os, &v(r, c)); }
    static void  saveData(std::ostream &os, UT_Matrix4D v)
    { for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) UTwrite<fpreal64>(os, &v(r, c)); }
    static void  saveData(std::ostream &os, UT_StringHolder s)
    { UT_StringWrap(s).saveBinary(os); }
    static void  saveData(std::ostream &os, UT_SharedPtr<UT_Ramp> s)
    {   UT_StringHolder         result;
        UT_OStringStream        ostr;
        if (s) s->save(ostr);
        result = ostr.str();
        saveData(os, result);
    }
    static void  saveData(std::ostream &os, PRM_DataItemHandle s)
    {   UT_StringHolder         result;
        UT_OStringStream        ostr;
        if (s) 
        {
            ostr << s->getDataTypeToken();
            ostr << ":";
            s->saveBinary(ostr);
        }
        result = ostr.str();
        saveData(os, result);
    }


    void         save(std::ostream &os) const
    {
        int32           v = version();
        UTwrite(os, &v);
        saveData(os, myGroup);
        saveData(os, myField);
        saveData(os, myName);
        saveData(os, myCreateVarMap);
        saveData(os, myVarName);
        saveData(os, myType);
        saveData(os, mySize);
        saveData(os, myDefault);
        saveData(os, myRangeIn);
        saveData(os, myMonoEnable);
        saveData(os, myMonoRampMode);
        saveData(os, myMonopreset);
        saveData(os, myMonoRamp);
        saveData(os, myVectorEnable);
        saveData(os, myVectorRampMode);
        saveData(os, myVectorpreset);
        saveData(os, myVectorRamp);
        saveData(os, myRangeOut);

    }

    bool         load(UT_IStream &is)
    {
        int32           v;
        is.bread(&v, 1);
        if (version() != v)
        {
            // Fail incompatible versions
            return false;
        }
        loadData(is, myGroup);
        loadData(is, myField);
        loadData(is, myName);
        loadData(is, myCreateVarMap);
        loadData(is, myVarName);
        loadData(is, myType);
        loadData(is, mySize);
        loadData(is, myDefault);
        loadData(is, myRangeIn);
        loadData(is, myMonoEnable);
        loadData(is, myMonoRampMode);
        loadData(is, myMonopreset);
        loadData(is, myMonoRamp);
        loadData(is, myVectorEnable);
        loadData(is, myVectorRampMode);
        loadData(is, myVectorpreset);
        loadData(is, myVectorRamp);
        loadData(is, myRangeOut);

        return true;
    }

    const UT_StringHolder & getGroup() const { return myGroup; }
    void setGroup(const UT_StringHolder & val) { myGroup = val; }
    UT_StringHolder opGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "group", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getField() const { return myField; }
    void setField(const UT_StringHolder & val) { myField = val; }
    UT_StringHolder opField(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getField();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "field", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getName() const { return myName; }
    void setName(const UT_StringHolder & val) { myName = val; }
    UT_StringHolder opName(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getName();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "name", cookparms.getCookTime(), 0);
        return result;
    }
    bool getCreateVarMap() const { return myCreateVarMap; }
    void setCreateVarMap(bool val) { myCreateVarMap = val; }
    bool opCreateVarMap(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCreateVarMap();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "createvarmap", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getVarName() const { return myVarName; }
    void setVarName(const UT_StringHolder & val) { myVarName = val; }
    UT_StringHolder opVarName(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVarName();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "varname", cookparms.getCookTime(), 0);
        return result;
    }
    Type getType() const { return Type(myType); }
    void setType(Type val) { myType = int64(val); }
    Type opType(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getType();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "type", cookparms.getCookTime(), 0);
        return Type(result);
    }
    int64 getSize() const { return mySize; }
    void setSize(int64 val) { mySize = val; }
    int64 opSize(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSize();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "size", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector4D getDefault() const { return myDefault; }
    void setDefault(UT_Vector4D val) { myDefault = val; }
    UT_Vector4D opDefault(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDefault();
        UT_Vector4D result;
        OP_Utils::evalOpParm(result, thissop, "default", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector2D getRangeIn() const { return myRangeIn; }
    void setRangeIn(UT_Vector2D val) { myRangeIn = val; }
    UT_Vector2D opRangeIn(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRangeIn();
        UT_Vector2D result;
        OP_Utils::evalOpParm(result, thissop, "rangein", cookparms.getCookTime(), 0);
        return result;
    }
    bool getMonoEnable() const { return myMonoEnable; }
    void setMonoEnable(bool val) { myMonoEnable = val; }
    bool opMonoEnable(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMonoEnable();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "monoenable", cookparms.getCookTime(), 0);
        return result;
    }
    MonoRampMode getMonoRampMode() const { return MonoRampMode(myMonoRampMode); }
    void setMonoRampMode(MonoRampMode val) { myMonoRampMode = int64(val); }
    MonoRampMode opMonoRampMode(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMonoRampMode();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "monorampmode", cookparms.getCookTime(), 0);
        return MonoRampMode(result);
    }
    Monopreset getMonopreset() const { return Monopreset(myMonopreset); }
    void setMonopreset(Monopreset val) { myMonopreset = int64(val); }
    Monopreset opMonopreset(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMonopreset();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "monopreset", cookparms.getCookTime(), 0);
        return Monopreset(result);
    }
    UT_SharedPtr<UT_Ramp> getMonoRamp() const { return myMonoRamp; }
    void setMonoRamp(UT_SharedPtr<UT_Ramp> val) { myMonoRamp = val; }
    UT_SharedPtr<UT_Ramp> opMonoRamp(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMonoRamp();
        UT_SharedPtr<UT_Ramp> result;
        OP_Utils::evalOpParm(result, thissop, "monoramp", cookparms.getCookTime(), 0);
        return result;
    }
    bool getVectorEnable() const { return myVectorEnable; }
    void setVectorEnable(bool val) { myVectorEnable = val; }
    bool opVectorEnable(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVectorEnable();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "vectorenable", cookparms.getCookTime(), 0);
        return result;
    }
    VectorRampMode getVectorRampMode() const { return VectorRampMode(myVectorRampMode); }
    void setVectorRampMode(VectorRampMode val) { myVectorRampMode = int64(val); }
    VectorRampMode opVectorRampMode(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVectorRampMode();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "vectorrampmode", cookparms.getCookTime(), 0);
        return VectorRampMode(result);
    }
    Vectorpreset getVectorpreset() const { return Vectorpreset(myVectorpreset); }
    void setVectorpreset(Vectorpreset val) { myVectorpreset = int64(val); }
    Vectorpreset opVectorpreset(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVectorpreset();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "vectorpreset", cookparms.getCookTime(), 0);
        return Vectorpreset(result);
    }
    UT_SharedPtr<UT_Ramp> getVectorRamp() const { return myVectorRamp; }
    void setVectorRamp(UT_SharedPtr<UT_Ramp> val) { myVectorRamp = val; }
    UT_SharedPtr<UT_Ramp> opVectorRamp(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVectorRamp();
        UT_SharedPtr<UT_Ramp> result;
        OP_Utils::evalOpParm(result, thissop, "vectorramp", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector2D getRangeOut() const { return myRangeOut; }
    void setRangeOut(UT_Vector2D val) { myRangeOut = val; }
    UT_Vector2D opRangeOut(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRangeOut();
        UT_Vector2D result;
        OP_Utils::evalOpParm(result, thissop, "rangeout", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myGroup;
    UT_StringHolder myField;
    UT_StringHolder myName;
    bool myCreateVarMap;
    UT_StringHolder myVarName;
    int64 myType;
    int64 mySize;
    UT_Vector4D myDefault;
    UT_Vector2D myRangeIn;
    bool myMonoEnable;
    int64 myMonoRampMode;
    int64 myMonopreset;
    UT_SharedPtr<UT_Ramp> myMonoRamp;
    bool myVectorEnable;
    int64 myVectorRampMode;
    int64 myVectorpreset;
    UT_SharedPtr<UT_Ramp> myVectorRamp;
    UT_Vector2D myRangeOut;

};