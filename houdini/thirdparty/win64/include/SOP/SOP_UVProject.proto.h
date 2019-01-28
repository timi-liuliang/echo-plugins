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
namespace SOP_UVProjectEnums
{
    enum class GroupType
    {
        VERTICES = 0,
        POINTS
    };
    enum class ProjType
    {
        TEXTURE = 0,
        POLAR,
        CYLIN,
        TORUS,
        WRAP
    };
    enum class Xord
    {
        SRT = 0,
        STR,
        RST,
        RTS,
        TSR,
        TRS
    };
    enum class Rord
    {
        XYZ = 0,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX
    };
    enum class Inittype
    {
        XY = 0,
        YZ,
        ZX,
        BEST
    };
}


class SOP_API SOP_UVProjectParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_UVProjectParms()
    {
        myUVAttrib = "uv"_sh;
        myGroup = ""_sh;
        myGroupType = 0;
        myProjType = 0;
        myTorRad = 0.2;
        myXord = 0;
        myRord = 0;
        myT = UT_Vector3D(0,0,0);
        myR = UT_Vector3D(0,0,0);
        myS = UT_Vector3D(1,1,1);
        myP = UT_Vector3D(0,0,0);
        myInittype = 3;
        myURange = UT_Vector2D(0,1);
        myVRange = UT_Vector2D(0,1);
        myAngle = 0;
        myFixSeams = false;
        myFixPolar = false;
        myPoleRad = 0.01;

    }

    explicit SOP_UVProjectParms(const SOP_UVProjectParms &) = default;

    virtual ~SOP_UVProjectParms() {}

    bool operator==(const SOP_UVProjectParms &src) const
    {
        if (myUVAttrib != src.myUVAttrib) return false;
        if (myGroup != src.myGroup) return false;
        if (myGroupType != src.myGroupType) return false;
        if (myProjType != src.myProjType) return false;
        if (myTorRad != src.myTorRad) return false;
        if (myXord != src.myXord) return false;
        if (myRord != src.myRord) return false;
        if (myT != src.myT) return false;
        if (myR != src.myR) return false;
        if (myS != src.myS) return false;
        if (myP != src.myP) return false;
        if (myInittype != src.myInittype) return false;
        if (myURange != src.myURange) return false;
        if (myVRange != src.myVRange) return false;
        if (myAngle != src.myAngle) return false;
        if (myFixSeams != src.myFixSeams) return false;
        if (myFixPolar != src.myFixPolar) return false;
        if (myPoleRad != src.myPoleRad) return false;

        return true;
    }
    bool operator!=(const SOP_UVProjectParms &src) const
    {
        return !operator==(src);
    }
    using GroupType = SOP_UVProjectEnums::GroupType;
    using ProjType = SOP_UVProjectEnums::ProjType;
    using Xord = SOP_UVProjectEnums::Xord;
    using Rord = SOP_UVProjectEnums::Rord;
    using Inittype = SOP_UVProjectEnums::Inittype;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myUVAttrib, node, "uvattrib", time, 0);
        else myUVAttrib = "uv"_sh;
        if (true)
            OP_Utils::evalOpParm(myGroup, node, "group", time, 0);
        else myGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myGroupType, node, "grouptype", time, 0);
        else myGroupType = 0;
        if (true)
            OP_Utils::evalOpParm(myProjType, node, "projtype", time, 0);
        else myProjType = 0;
        if (true && ( (!(((int64(getProjType())!=3)))) ) )
            OP_Utils::evalOpParm(myTorRad, node, "torrad", time, 0);
        else myTorRad = 0.2;
        if (true)
            OP_Utils::evalOpParm(myXord, node, "xOrd", time, 0);
        else myXord = 0;
        if (true)
            OP_Utils::evalOpParm(myRord, node, "rOrd", time, 0);
        else myRord = 0;
        if (true)
            OP_Utils::evalOpParm(myT, node, "t", time, 0);
        else myT = UT_Vector3D(0,0,0);
        if (true)
            OP_Utils::evalOpParm(myR, node, "r", time, 0);
        else myR = UT_Vector3D(0,0,0);
        if (true)
            OP_Utils::evalOpParm(myS, node, "s", time, 0);
        else myS = UT_Vector3D(1,1,1);
        if (true)
            OP_Utils::evalOpParm(myP, node, "p", time, 0);
        else myP = UT_Vector3D(0,0,0);
        if (true)
            OP_Utils::evalOpParm(myInittype, node, "inittype", time, 0);
        else myInittype = 3;
        if (true)
            OP_Utils::evalOpParm(myURange, node, "urange", time, 0);
        else myURange = UT_Vector2D(0,1);
        if (true)
            OP_Utils::evalOpParm(myVRange, node, "vrange", time, 0);
        else myVRange = UT_Vector2D(0,1);
        if (true)
            OP_Utils::evalOpParm(myAngle, node, "angle", time, 0);
        else myAngle = 0;
        if (true && ( (!(((int64(getProjType())==0))||((int64(getProjType())==4)))) ) )
            OP_Utils::evalOpParm(myFixSeams, node, "fixseams", time, 0);
        else myFixSeams = false;
        if (true && ( (!(((int64(getProjType())==0))||((int64(getProjType())==3)))) ) )
            OP_Utils::evalOpParm(myFixPolar, node, "fixpolar", time, 0);
        else myFixPolar = false;
        if (true && ( (!(((int64(getProjType())==0))||((int64(getProjType())==3))||((getFixPolar()==0)))) ) )
            OP_Utils::evalOpParm(myPoleRad, node, "polerad", time, 0);
        else myPoleRad = 0.01;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_UVProjectParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myUVAttrib);
                break;
            case 1:
                coerceValue(value, myGroup);
                break;
            case 2:
                coerceValue(value, myGroupType);
                break;
            case 3:
                coerceValue(value, myProjType);
                break;
            case 4:
                coerceValue(value, myTorRad);
                break;
            case 5:
                coerceValue(value, myXord);
                break;
            case 6:
                coerceValue(value, myRord);
                break;
            case 7:
                coerceValue(value, myT);
                break;
            case 8:
                coerceValue(value, myR);
                break;
            case 9:
                coerceValue(value, myS);
                break;
            case 10:
                coerceValue(value, myP);
                break;
            case 11:
                coerceValue(value, myInittype);
                break;
            case 12:
                coerceValue(value, myURange);
                break;
            case 13:
                coerceValue(value, myVRange);
                break;
            case 14:
                coerceValue(value, myAngle);
                break;
            case 15:
                coerceValue(value, myFixSeams);
                break;
            case 16:
                coerceValue(value, myFixPolar);
                break;
            case 17:
                coerceValue(value, myPoleRad);
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
                coerceValue(myUVAttrib, value);
                break;
            case 1:
                coerceValue(myGroup, value);
                break;
            case 2:
                coerceValue(myGroupType, value);
                break;
            case 3:
                coerceValue(myProjType, value);
                break;
            case 4:
                coerceValue(myTorRad, value);
                break;
            case 5:
                coerceValue(myXord, value);
                break;
            case 6:
                coerceValue(myRord, value);
                break;
            case 7:
                coerceValue(myT, value);
                break;
            case 8:
                coerceValue(myR, value);
                break;
            case 9:
                coerceValue(myS, value);
                break;
            case 10:
                coerceValue(myP, value);
                break;
            case 11:
                coerceValue(myInittype, value);
                break;
            case 12:
                coerceValue(myURange, value);
                break;
            case 13:
                coerceValue(myVRange, value);
                break;
            case 14:
                coerceValue(myAngle, value);
                break;
            case 15:
                coerceValue(myFixSeams, value);
                break;
            case 16:
                coerceValue(myFixPolar, value);
                break;
            case 17:
                coerceValue(myPoleRad, value);
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
                return "uvattrib";
            case 1:
                return "group";
            case 2:
                return "grouptype";
            case 3:
                return "projtype";
            case 4:
                return "torrad";
            case 5:
                return "xOrd";
            case 6:
                return "rOrd";
            case 7:
                return "t";
            case 8:
                return "r";
            case 9:
                return "s";
            case 10:
                return "p";
            case 11:
                return "inittype";
            case 12:
                return "urange";
            case 13:
                return "vrange";
            case 14:
                return "angle";
            case 15:
                return "fixseams";
            case 16:
                return "fixpolar";
            case 17:
                return "polerad";

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
                    return PARM_INTEGER;
                case 3:
                    return PARM_INTEGER;
                case 4:
                    return PARM_FLOAT;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_INTEGER;
                case 7:
                    return PARM_VECTOR3;
                case 8:
                    return PARM_VECTOR3;
                case 9:
                    return PARM_VECTOR3;
                case 10:
                    return PARM_VECTOR3;
                case 11:
                    return PARM_INTEGER;
                case 12:
                    return PARM_VECTOR2;
                case 13:
                    return PARM_VECTOR2;
                case 14:
                    return PARM_FLOAT;
                case 15:
                    return PARM_INTEGER;
                case 16:
                    return PARM_INTEGER;
                case 17:
                    return PARM_FLOAT;

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
        saveData(os, myUVAttrib);
        saveData(os, myGroup);
        saveData(os, myGroupType);
        saveData(os, myProjType);
        saveData(os, myTorRad);
        saveData(os, myXord);
        saveData(os, myRord);
        saveData(os, myT);
        saveData(os, myR);
        saveData(os, myS);
        saveData(os, myP);
        saveData(os, myInittype);
        saveData(os, myURange);
        saveData(os, myVRange);
        saveData(os, myAngle);
        saveData(os, myFixSeams);
        saveData(os, myFixPolar);
        saveData(os, myPoleRad);

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
        loadData(is, myUVAttrib);
        loadData(is, myGroup);
        loadData(is, myGroupType);
        loadData(is, myProjType);
        loadData(is, myTorRad);
        loadData(is, myXord);
        loadData(is, myRord);
        loadData(is, myT);
        loadData(is, myR);
        loadData(is, myS);
        loadData(is, myP);
        loadData(is, myInittype);
        loadData(is, myURange);
        loadData(is, myVRange);
        loadData(is, myAngle);
        loadData(is, myFixSeams);
        loadData(is, myFixPolar);
        loadData(is, myPoleRad);

        return true;
    }

    const UT_StringHolder & getUVAttrib() const { return myUVAttrib; }
    void setUVAttrib(const UT_StringHolder & val) { myUVAttrib = val; }
    UT_StringHolder opUVAttrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUVAttrib();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "uvattrib", cookparms.getCookTime(), 0);
        return result;
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
    GroupType getGroupType() const { return GroupType(myGroupType); }
    void setGroupType(GroupType val) { myGroupType = int64(val); }
    GroupType opGroupType(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getGroupType();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "grouptype", cookparms.getCookTime(), 0);
        return GroupType(result);
    }
    ProjType getProjType() const { return ProjType(myProjType); }
    void setProjType(ProjType val) { myProjType = int64(val); }
    ProjType opProjType(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getProjType();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "projtype", cookparms.getCookTime(), 0);
        return ProjType(result);
    }
    fpreal64 getTorRad() const { return myTorRad; }
    void setTorRad(fpreal64 val) { myTorRad = val; }
    fpreal64 opTorRad(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTorRad();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "torrad", cookparms.getCookTime(), 0);
        return result;
    }
    Xord getXord() const { return Xord(myXord); }
    void setXord(Xord val) { myXord = int64(val); }
    Xord opXord(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getXord();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "xOrd", cookparms.getCookTime(), 0);
        return Xord(result);
    }
    Rord getRord() const { return Rord(myRord); }
    void setRord(Rord val) { myRord = int64(val); }
    Rord opRord(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRord();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "rOrd", cookparms.getCookTime(), 0);
        return Rord(result);
    }
    UT_Vector3D getT() const { return myT; }
    void setT(UT_Vector3D val) { myT = val; }
    UT_Vector3D opT(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getT();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "t", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector3D getR() const { return myR; }
    void setR(UT_Vector3D val) { myR = val; }
    UT_Vector3D opR(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getR();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "r", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector3D getS() const { return myS; }
    void setS(UT_Vector3D val) { myS = val; }
    UT_Vector3D opS(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getS();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "s", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector3D getP() const { return myP; }
    void setP(UT_Vector3D val) { myP = val; }
    UT_Vector3D opP(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getP();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "p", cookparms.getCookTime(), 0);
        return result;
    }
    Inittype getInittype() const { return Inittype(myInittype); }
    void setInittype(Inittype val) { myInittype = int64(val); }
    Inittype opInittype(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getInittype();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "inittype", cookparms.getCookTime(), 0);
        return Inittype(result);
    }
    UT_Vector2D getURange() const { return myURange; }
    void setURange(UT_Vector2D val) { myURange = val; }
    UT_Vector2D opURange(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getURange();
        UT_Vector2D result;
        OP_Utils::evalOpParm(result, thissop, "urange", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector2D getVRange() const { return myVRange; }
    void setVRange(UT_Vector2D val) { myVRange = val; }
    UT_Vector2D opVRange(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVRange();
        UT_Vector2D result;
        OP_Utils::evalOpParm(result, thissop, "vrange", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getAngle() const { return myAngle; }
    void setAngle(fpreal64 val) { myAngle = val; }
    fpreal64 opAngle(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAngle();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "angle", cookparms.getCookTime(), 0);
        return result;
    }
    bool getFixSeams() const { return myFixSeams; }
    void setFixSeams(bool val) { myFixSeams = val; }
    bool opFixSeams(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getFixSeams();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "fixseams", cookparms.getCookTime(), 0);
        return result;
    }
    bool getFixPolar() const { return myFixPolar; }
    void setFixPolar(bool val) { myFixPolar = val; }
    bool opFixPolar(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getFixPolar();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "fixpolar", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getPoleRad() const { return myPoleRad; }
    void setPoleRad(fpreal64 val) { myPoleRad = val; }
    fpreal64 opPoleRad(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPoleRad();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "polerad", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myUVAttrib;
    UT_StringHolder myGroup;
    int64 myGroupType;
    int64 myProjType;
    fpreal64 myTorRad;
    int64 myXord;
    int64 myRord;
    UT_Vector3D myT;
    UT_Vector3D myR;
    UT_Vector3D myS;
    UT_Vector3D myP;
    int64 myInittype;
    UT_Vector2D myURange;
    UT_Vector2D myVRange;
    fpreal64 myAngle;
    bool myFixSeams;
    bool myFixPolar;
    fpreal64 myPoleRad;

};