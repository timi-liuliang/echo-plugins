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
namespace SOP_CopyTransformEnums
{
    enum class Pivot
    {
        ORIGIN = 0,
        CENTROID
    };
    enum class Viewportlod
    {
        FULL = 0,
        POINTS,
        BOX,
        CENTROID,
        HIDDEN
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
}


class SOP_API SOP_CopyTransformParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_CopyTransformParms()
    {
        mySourceGroup = ""_sh;
        myNcy = 2;
        myPack = false;
        myPivot = 1;
        myViewportlod = 0;
        myXord = 0;
        myRord = 0;
        myT = UT_Vector3D(0,0,0);
        myR = UT_Vector3D(0,0,0);
        myS = UT_Vector3D(1,1,1);
        myShear = UT_Vector3D(0,0,0);
        myScale = 1;
        myP = UT_Vector3D(0,0,0);
        myPr = UT_Vector3D(0,0,0);
        myNewgroups = false;
        myNewgroupprefix = "copyGroup"_sh;
        myDocopyattrib = false;
        myCopyattrib = "copynum"_sh;

    }

    explicit SOP_CopyTransformParms(const SOP_CopyTransformParms &) = default;

    virtual ~SOP_CopyTransformParms() {}

    bool operator==(const SOP_CopyTransformParms &src) const
    {
        if (mySourceGroup != src.mySourceGroup) return false;
        if (myNcy != src.myNcy) return false;
        if (myPack != src.myPack) return false;
        if (myPivot != src.myPivot) return false;
        if (myViewportlod != src.myViewportlod) return false;
        if (myXord != src.myXord) return false;
        if (myRord != src.myRord) return false;
        if (myT != src.myT) return false;
        if (myR != src.myR) return false;
        if (myS != src.myS) return false;
        if (myShear != src.myShear) return false;
        if (myScale != src.myScale) return false;
        if (myP != src.myP) return false;
        if (myPr != src.myPr) return false;
        if (myNewgroups != src.myNewgroups) return false;
        if (myNewgroupprefix != src.myNewgroupprefix) return false;
        if (myDocopyattrib != src.myDocopyattrib) return false;
        if (myCopyattrib != src.myCopyattrib) return false;

        return true;
    }
    bool operator!=(const SOP_CopyTransformParms &src) const
    {
        return !operator==(src);
    }
    using Pivot = SOP_CopyTransformEnums::Pivot;
    using Viewportlod = SOP_CopyTransformEnums::Viewportlod;
    using Xord = SOP_CopyTransformEnums::Xord;
    using Rord = SOP_CopyTransformEnums::Rord;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(mySourceGroup, node, "sourcegroup", time, 0);
        else mySourceGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myNcy, node, "ncy", time, 0);
        else myNcy = 2;
        if (true)
            OP_Utils::evalOpParm(myPack, node, "pack", time, 0);
        else myPack = false;
        if (true && ( (!(((getPack()==0)))) ) )
            OP_Utils::evalOpParm(myPivot, node, "pivot", time, 0);
        else myPivot = 1;
        if (true && ( (!(((getPack()==0)))) ) )
            OP_Utils::evalOpParm(myViewportlod, node, "viewportlod", time, 0);
        else myViewportlod = 0;
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
            OP_Utils::evalOpParm(myShear, node, "shear", time, 0);
        else myShear = UT_Vector3D(0,0,0);
        if (true)
            OP_Utils::evalOpParm(myScale, node, "scale", time, 0);
        else myScale = 1;
        if (true)
            OP_Utils::evalOpParm(myP, node, "p", time, 0);
        else myP = UT_Vector3D(0,0,0);
        if (true)
            OP_Utils::evalOpParm(myPr, node, "pr", time, 0);
        else myPr = UT_Vector3D(0,0,0);
        if (true)
            OP_Utils::evalOpParm(myNewgroups, node, "newgroups", time, 0);
        else myNewgroups = false;
        if (true && ( (!(((getNewgroups()==0)))) ) )
            OP_Utils::evalOpParm(myNewgroupprefix, node, "newgroupprefix", time, 0);
        else myNewgroupprefix = "copyGroup"_sh;
        if (true)
            OP_Utils::evalOpParm(myDocopyattrib, node, "docopyattrib", time, 0);
        else myDocopyattrib = false;
        if (true && ( (!(((getDocopyattrib()==0)))) ) )
            OP_Utils::evalOpParm(myCopyattrib, node, "copyattrib", time, 0);
        else myCopyattrib = "copynum"_sh;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_CopyTransformParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, mySourceGroup);
                break;
            case 1:
                coerceValue(value, myNcy);
                break;
            case 2:
                coerceValue(value, myPack);
                break;
            case 3:
                coerceValue(value, myPivot);
                break;
            case 4:
                coerceValue(value, myViewportlod);
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
                coerceValue(value, myShear);
                break;
            case 11:
                coerceValue(value, myScale);
                break;
            case 12:
                coerceValue(value, myP);
                break;
            case 13:
                coerceValue(value, myPr);
                break;
            case 14:
                coerceValue(value, myNewgroups);
                break;
            case 15:
                coerceValue(value, myNewgroupprefix);
                break;
            case 16:
                coerceValue(value, myDocopyattrib);
                break;
            case 17:
                coerceValue(value, myCopyattrib);
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
                coerceValue(mySourceGroup, value);
                break;
            case 1:
                coerceValue(myNcy, value);
                break;
            case 2:
                coerceValue(myPack, value);
                break;
            case 3:
                coerceValue(myPivot, value);
                break;
            case 4:
                coerceValue(myViewportlod, value);
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
                coerceValue(myShear, value);
                break;
            case 11:
                coerceValue(myScale, value);
                break;
            case 12:
                coerceValue(myP, value);
                break;
            case 13:
                coerceValue(myPr, value);
                break;
            case 14:
                coerceValue(myNewgroups, value);
                break;
            case 15:
                coerceValue(myNewgroupprefix, value);
                break;
            case 16:
                coerceValue(myDocopyattrib, value);
                break;
            case 17:
                coerceValue(myCopyattrib, value);
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
                return "sourcegroup";
            case 1:
                return "ncy";
            case 2:
                return "pack";
            case 3:
                return "pivot";
            case 4:
                return "viewportlod";
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
                return "shear";
            case 11:
                return "scale";
            case 12:
                return "p";
            case 13:
                return "pr";
            case 14:
                return "newgroups";
            case 15:
                return "newgroupprefix";
            case 16:
                return "docopyattrib";
            case 17:
                return "copyattrib";

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
                    return PARM_INTEGER;
                case 2:
                    return PARM_INTEGER;
                case 3:
                    return PARM_INTEGER;
                case 4:
                    return PARM_INTEGER;
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
                    return PARM_FLOAT;
                case 12:
                    return PARM_VECTOR3;
                case 13:
                    return PARM_VECTOR3;
                case 14:
                    return PARM_INTEGER;
                case 15:
                    return PARM_STRING;
                case 16:
                    return PARM_INTEGER;
                case 17:
                    return PARM_STRING;

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
        saveData(os, mySourceGroup);
        saveData(os, myNcy);
        saveData(os, myPack);
        saveData(os, myPivot);
        saveData(os, myViewportlod);
        saveData(os, myXord);
        saveData(os, myRord);
        saveData(os, myT);
        saveData(os, myR);
        saveData(os, myS);
        saveData(os, myShear);
        saveData(os, myScale);
        saveData(os, myP);
        saveData(os, myPr);
        saveData(os, myNewgroups);
        saveData(os, myNewgroupprefix);
        saveData(os, myDocopyattrib);
        saveData(os, myCopyattrib);

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
        loadData(is, mySourceGroup);
        loadData(is, myNcy);
        loadData(is, myPack);
        loadData(is, myPivot);
        loadData(is, myViewportlod);
        loadData(is, myXord);
        loadData(is, myRord);
        loadData(is, myT);
        loadData(is, myR);
        loadData(is, myS);
        loadData(is, myShear);
        loadData(is, myScale);
        loadData(is, myP);
        loadData(is, myPr);
        loadData(is, myNewgroups);
        loadData(is, myNewgroupprefix);
        loadData(is, myDocopyattrib);
        loadData(is, myCopyattrib);

        return true;
    }

    const UT_StringHolder & getSourceGroup() const { return mySourceGroup; }
    void setSourceGroup(const UT_StringHolder & val) { mySourceGroup = val; }
    UT_StringHolder opSourceGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSourceGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "sourcegroup", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getNcy() const { return myNcy; }
    void setNcy(int64 val) { myNcy = val; }
    int64 opNcy(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getNcy();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "ncy", cookparms.getCookTime(), 0);
        return result;
    }
    bool getPack() const { return myPack; }
    void setPack(bool val) { myPack = val; }
    bool opPack(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPack();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "pack", cookparms.getCookTime(), 0);
        return result;
    }
    Pivot getPivot() const { return Pivot(myPivot); }
    void setPivot(Pivot val) { myPivot = int64(val); }
    Pivot opPivot(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPivot();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "pivot", cookparms.getCookTime(), 0);
        return Pivot(result);
    }
    Viewportlod getViewportlod() const { return Viewportlod(myViewportlod); }
    void setViewportlod(Viewportlod val) { myViewportlod = int64(val); }
    Viewportlod opViewportlod(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getViewportlod();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "viewportlod", cookparms.getCookTime(), 0);
        return Viewportlod(result);
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
    UT_Vector3D getShear() const { return myShear; }
    void setShear(UT_Vector3D val) { myShear = val; }
    UT_Vector3D opShear(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getShear();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "shear", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getScale() const { return myScale; }
    void setScale(fpreal64 val) { myScale = val; }
    fpreal64 opScale(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getScale();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "scale", cookparms.getCookTime(), 0);
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
    UT_Vector3D getPr() const { return myPr; }
    void setPr(UT_Vector3D val) { myPr = val; }
    UT_Vector3D opPr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPr();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "pr", cookparms.getCookTime(), 0);
        return result;
    }
    bool getNewgroups() const { return myNewgroups; }
    void setNewgroups(bool val) { myNewgroups = val; }
    bool opNewgroups(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getNewgroups();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "newgroups", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getNewgroupprefix() const { return myNewgroupprefix; }
    void setNewgroupprefix(const UT_StringHolder & val) { myNewgroupprefix = val; }
    UT_StringHolder opNewgroupprefix(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getNewgroupprefix();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "newgroupprefix", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDocopyattrib() const { return myDocopyattrib; }
    void setDocopyattrib(bool val) { myDocopyattrib = val; }
    bool opDocopyattrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDocopyattrib();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "docopyattrib", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getCopyattrib() const { return myCopyattrib; }
    void setCopyattrib(const UT_StringHolder & val) { myCopyattrib = val; }
    UT_StringHolder opCopyattrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCopyattrib();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "copyattrib", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder mySourceGroup;
    int64 myNcy;
    bool myPack;
    int64 myPivot;
    int64 myViewportlod;
    int64 myXord;
    int64 myRord;
    UT_Vector3D myT;
    UT_Vector3D myR;
    UT_Vector3D myS;
    UT_Vector3D myShear;
    fpreal64 myScale;
    UT_Vector3D myP;
    UT_Vector3D myPr;
    bool myNewgroups;
    UT_StringHolder myNewgroupprefix;
    bool myDocopyattrib;
    UT_StringHolder myCopyattrib;

};