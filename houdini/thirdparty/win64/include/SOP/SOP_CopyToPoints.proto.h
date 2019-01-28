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
namespace SOP_CopyToPointsEnums
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
}


class SOP_API SOP_CopyToPointsParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_CopyToPointsParms()
    {
        mySourceGroup = ""_sh;
        myTargetGroup = ""_sh;
        myShowguide = true;
        myPack = false;
        myPivot = 1;
        myViewportlod = 0;
        myTransform = true;
        myDoAttr = true;
        mySetpt = "*,^v,^Alpha"_sh;
        myMulpt = "Alpha"_sh;
        myAddpt = "v"_sh;
        mySubpt = ""_sh;
        mySetprim = ""_sh;
        myMulprim = ""_sh;
        myAddprim = ""_sh;
        mySubprim = ""_sh;
        mySetvtx = ""_sh;
        myMulvtx = ""_sh;
        myAddvtx = ""_sh;
        mySubvtx = ""_sh;

    }

    explicit SOP_CopyToPointsParms(const SOP_CopyToPointsParms &) = default;

    virtual ~SOP_CopyToPointsParms() {}

    bool operator==(const SOP_CopyToPointsParms &src) const
    {
        if (mySourceGroup != src.mySourceGroup) return false;
        if (myTargetGroup != src.myTargetGroup) return false;
        if (myShowguide != src.myShowguide) return false;
        if (myPack != src.myPack) return false;
        if (myPivot != src.myPivot) return false;
        if (myViewportlod != src.myViewportlod) return false;
        if (myTransform != src.myTransform) return false;
        if (myDoAttr != src.myDoAttr) return false;
        if (mySetpt != src.mySetpt) return false;
        if (myMulpt != src.myMulpt) return false;
        if (myAddpt != src.myAddpt) return false;
        if (mySubpt != src.mySubpt) return false;
        if (mySetprim != src.mySetprim) return false;
        if (myMulprim != src.myMulprim) return false;
        if (myAddprim != src.myAddprim) return false;
        if (mySubprim != src.mySubprim) return false;
        if (mySetvtx != src.mySetvtx) return false;
        if (myMulvtx != src.myMulvtx) return false;
        if (myAddvtx != src.myAddvtx) return false;
        if (mySubvtx != src.mySubvtx) return false;

        return true;
    }
    bool operator!=(const SOP_CopyToPointsParms &src) const
    {
        return !operator==(src);
    }
    using Pivot = SOP_CopyToPointsEnums::Pivot;
    using Viewportlod = SOP_CopyToPointsEnums::Viewportlod;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(mySourceGroup, node, "sourcegroup", time, 0);
        else mySourceGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myTargetGroup, node, "targetgroup", time, 0);
        else myTargetGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myShowguide, node, "showguide", time, 0);
        else myShowguide = true;
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
            OP_Utils::evalOpParm(myTransform, node, "transform", time, 0);
        else myTransform = true;
        if (true)
            OP_Utils::evalOpParm(myDoAttr, node, "doattr", time, 0);
        else myDoAttr = true;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(mySetpt, node, "setpt", time, 0);
        else mySetpt = "*,^v,^Alpha"_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(myMulpt, node, "mulpt", time, 0);
        else myMulpt = "Alpha"_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(myAddpt, node, "addpt", time, 0);
        else myAddpt = "v"_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(mySubpt, node, "subpt", time, 0);
        else mySubpt = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(mySetprim, node, "setprim", time, 0);
        else mySetprim = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(myMulprim, node, "mulprim", time, 0);
        else myMulprim = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(myAddprim, node, "addprim", time, 0);
        else myAddprim = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(mySubprim, node, "subprim", time, 0);
        else mySubprim = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(mySetvtx, node, "setvtx", time, 0);
        else mySetvtx = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(myMulvtx, node, "mulvtx", time, 0);
        else myMulvtx = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(myAddvtx, node, "addvtx", time, 0);
        else myAddvtx = ""_sh;
        if (true && ( (!(((getDoAttr()==0)))) ) )
            OP_Utils::evalOpParm(mySubvtx, node, "subvtx", time, 0);
        else mySubvtx = ""_sh;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_CopyToPointsParms *)src);
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
                coerceValue(value, myTargetGroup);
                break;
            case 2:
                coerceValue(value, myShowguide);
                break;
            case 3:
                coerceValue(value, myPack);
                break;
            case 4:
                coerceValue(value, myPivot);
                break;
            case 5:
                coerceValue(value, myViewportlod);
                break;
            case 6:
                coerceValue(value, myTransform);
                break;
            case 7:
                coerceValue(value, myDoAttr);
                break;
            case 8:
                coerceValue(value, mySetpt);
                break;
            case 9:
                coerceValue(value, myMulpt);
                break;
            case 10:
                coerceValue(value, myAddpt);
                break;
            case 11:
                coerceValue(value, mySubpt);
                break;
            case 12:
                coerceValue(value, mySetprim);
                break;
            case 13:
                coerceValue(value, myMulprim);
                break;
            case 14:
                coerceValue(value, myAddprim);
                break;
            case 15:
                coerceValue(value, mySubprim);
                break;
            case 16:
                coerceValue(value, mySetvtx);
                break;
            case 17:
                coerceValue(value, myMulvtx);
                break;
            case 18:
                coerceValue(value, myAddvtx);
                break;
            case 19:
                coerceValue(value, mySubvtx);
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
                coerceValue(myTargetGroup, value);
                break;
            case 2:
                coerceValue(myShowguide, value);
                break;
            case 3:
                coerceValue(myPack, value);
                break;
            case 4:
                coerceValue(myPivot, value);
                break;
            case 5:
                coerceValue(myViewportlod, value);
                break;
            case 6:
                coerceValue(myTransform, value);
                break;
            case 7:
                coerceValue(myDoAttr, value);
                break;
            case 8:
                coerceValue(mySetpt, value);
                break;
            case 9:
                coerceValue(myMulpt, value);
                break;
            case 10:
                coerceValue(myAddpt, value);
                break;
            case 11:
                coerceValue(mySubpt, value);
                break;
            case 12:
                coerceValue(mySetprim, value);
                break;
            case 13:
                coerceValue(myMulprim, value);
                break;
            case 14:
                coerceValue(myAddprim, value);
                break;
            case 15:
                coerceValue(mySubprim, value);
                break;
            case 16:
                coerceValue(mySetvtx, value);
                break;
            case 17:
                coerceValue(myMulvtx, value);
                break;
            case 18:
                coerceValue(myAddvtx, value);
                break;
            case 19:
                coerceValue(mySubvtx, value);
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
        return 20;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "sourcegroup";
            case 1:
                return "targetgroup";
            case 2:
                return "showguide";
            case 3:
                return "pack";
            case 4:
                return "pivot";
            case 5:
                return "viewportlod";
            case 6:
                return "transform";
            case 7:
                return "doattr";
            case 8:
                return "setpt";
            case 9:
                return "mulpt";
            case 10:
                return "addpt";
            case 11:
                return "subpt";
            case 12:
                return "setprim";
            case 13:
                return "mulprim";
            case 14:
                return "addprim";
            case 15:
                return "subprim";
            case 16:
                return "setvtx";
            case 17:
                return "mulvtx";
            case 18:
                return "addvtx";
            case 19:
                return "subvtx";

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
                    return PARM_INTEGER;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_INTEGER;
                case 7:
                    return PARM_INTEGER;
                case 8:
                    return PARM_STRING;
                case 9:
                    return PARM_STRING;
                case 10:
                    return PARM_STRING;
                case 11:
                    return PARM_STRING;
                case 12:
                    return PARM_STRING;
                case 13:
                    return PARM_STRING;
                case 14:
                    return PARM_STRING;
                case 15:
                    return PARM_STRING;
                case 16:
                    return PARM_STRING;
                case 17:
                    return PARM_STRING;
                case 18:
                    return PARM_STRING;
                case 19:
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
        saveData(os, myTargetGroup);
        saveData(os, myShowguide);
        saveData(os, myPack);
        saveData(os, myPivot);
        saveData(os, myViewportlod);
        saveData(os, myTransform);
        saveData(os, myDoAttr);
        saveData(os, mySetpt);
        saveData(os, myMulpt);
        saveData(os, myAddpt);
        saveData(os, mySubpt);
        saveData(os, mySetprim);
        saveData(os, myMulprim);
        saveData(os, myAddprim);
        saveData(os, mySubprim);
        saveData(os, mySetvtx);
        saveData(os, myMulvtx);
        saveData(os, myAddvtx);
        saveData(os, mySubvtx);

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
        loadData(is, myTargetGroup);
        loadData(is, myShowguide);
        loadData(is, myPack);
        loadData(is, myPivot);
        loadData(is, myViewportlod);
        loadData(is, myTransform);
        loadData(is, myDoAttr);
        loadData(is, mySetpt);
        loadData(is, myMulpt);
        loadData(is, myAddpt);
        loadData(is, mySubpt);
        loadData(is, mySetprim);
        loadData(is, myMulprim);
        loadData(is, myAddprim);
        loadData(is, mySubprim);
        loadData(is, mySetvtx);
        loadData(is, myMulvtx);
        loadData(is, myAddvtx);
        loadData(is, mySubvtx);

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
    const UT_StringHolder & getTargetGroup() const { return myTargetGroup; }
    void setTargetGroup(const UT_StringHolder & val) { myTargetGroup = val; }
    UT_StringHolder opTargetGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTargetGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "targetgroup", cookparms.getCookTime(), 0);
        return result;
    }
    bool getShowguide() const { return myShowguide; }
    void setShowguide(bool val) { myShowguide = val; }
    bool opShowguide(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getShowguide();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "showguide", cookparms.getCookTime(), 0);
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
    bool getTransform() const { return myTransform; }
    void setTransform(bool val) { myTransform = val; }
    bool opTransform(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTransform();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "transform", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDoAttr() const { return myDoAttr; }
    void setDoAttr(bool val) { myDoAttr = val; }
    bool opDoAttr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDoAttr();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "doattr", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSetpt() const { return mySetpt; }
    void setSetpt(const UT_StringHolder & val) { mySetpt = val; }
    UT_StringHolder opSetpt(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSetpt();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "setpt", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getMulpt() const { return myMulpt; }
    void setMulpt(const UT_StringHolder & val) { myMulpt = val; }
    UT_StringHolder opMulpt(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMulpt();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "mulpt", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getAddpt() const { return myAddpt; }
    void setAddpt(const UT_StringHolder & val) { myAddpt = val; }
    UT_StringHolder opAddpt(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAddpt();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "addpt", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSubpt() const { return mySubpt; }
    void setSubpt(const UT_StringHolder & val) { mySubpt = val; }
    UT_StringHolder opSubpt(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSubpt();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "subpt", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSetprim() const { return mySetprim; }
    void setSetprim(const UT_StringHolder & val) { mySetprim = val; }
    UT_StringHolder opSetprim(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSetprim();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "setprim", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getMulprim() const { return myMulprim; }
    void setMulprim(const UT_StringHolder & val) { myMulprim = val; }
    UT_StringHolder opMulprim(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMulprim();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "mulprim", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getAddprim() const { return myAddprim; }
    void setAddprim(const UT_StringHolder & val) { myAddprim = val; }
    UT_StringHolder opAddprim(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAddprim();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "addprim", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSubprim() const { return mySubprim; }
    void setSubprim(const UT_StringHolder & val) { mySubprim = val; }
    UT_StringHolder opSubprim(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSubprim();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "subprim", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSetvtx() const { return mySetvtx; }
    void setSetvtx(const UT_StringHolder & val) { mySetvtx = val; }
    UT_StringHolder opSetvtx(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSetvtx();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "setvtx", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getMulvtx() const { return myMulvtx; }
    void setMulvtx(const UT_StringHolder & val) { myMulvtx = val; }
    UT_StringHolder opMulvtx(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMulvtx();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "mulvtx", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getAddvtx() const { return myAddvtx; }
    void setAddvtx(const UT_StringHolder & val) { myAddvtx = val; }
    UT_StringHolder opAddvtx(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAddvtx();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "addvtx", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSubvtx() const { return mySubvtx; }
    void setSubvtx(const UT_StringHolder & val) { mySubvtx = val; }
    UT_StringHolder opSubvtx(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSubvtx();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "subvtx", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder mySourceGroup;
    UT_StringHolder myTargetGroup;
    bool myShowguide;
    bool myPack;
    int64 myPivot;
    int64 myViewportlod;
    bool myTransform;
    bool myDoAttr;
    UT_StringHolder mySetpt;
    UT_StringHolder myMulpt;
    UT_StringHolder myAddpt;
    UT_StringHolder mySubpt;
    UT_StringHolder mySetprim;
    UT_StringHolder myMulprim;
    UT_StringHolder myAddprim;
    UT_StringHolder mySubprim;
    UT_StringHolder mySetvtx;
    UT_StringHolder myMulvtx;
    UT_StringHolder myAddvtx;
    UT_StringHolder mySubvtx;

};