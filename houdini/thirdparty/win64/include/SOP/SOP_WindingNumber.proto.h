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
namespace SOP_WindingNumberEnums
{
    enum class Type
    {
        XYZ = 0,
        XY,
        YZ,
        ZX
    };
}


class SOP_API SOP_WindingNumberParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_WindingNumberParms()
    {
        myQueryPoints = ""_sh;
        myMeshPrims = ""_sh;
        myType = 0;
        myAttrib = "winding_number"_sh;
        myAsSolidAngle = false;
        myNegate = false;
        myFullAccuracy = false;
        myAccuracyScale = 2;

    }

    explicit SOP_WindingNumberParms(const SOP_WindingNumberParms &) = default;

    virtual ~SOP_WindingNumberParms() {}

    bool operator==(const SOP_WindingNumberParms &src) const
    {
        if (myQueryPoints != src.myQueryPoints) return false;
        if (myMeshPrims != src.myMeshPrims) return false;
        if (myType != src.myType) return false;
        if (myAttrib != src.myAttrib) return false;
        if (myAsSolidAngle != src.myAsSolidAngle) return false;
        if (myNegate != src.myNegate) return false;
        if (myFullAccuracy != src.myFullAccuracy) return false;
        if (myAccuracyScale != src.myAccuracyScale) return false;

        return true;
    }
    bool operator!=(const SOP_WindingNumberParms &src) const
    {
        return !operator==(src);
    }
    using Type = SOP_WindingNumberEnums::Type;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myQueryPoints, node, "querypoints", time, 0);
        else myQueryPoints = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myMeshPrims, node, "meshprims", time, 0);
        else myMeshPrims = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myType, node, "type", time, 0);
        else myType = 0;
        if (true)
            OP_Utils::evalOpParm(myAttrib, node, "attrib", time, 0);
        else myAttrib = "winding_number"_sh;
        if (true)
            OP_Utils::evalOpParm(myAsSolidAngle, node, "assolidangle", time, 0);
        else myAsSolidAngle = false;
        if (true)
            OP_Utils::evalOpParm(myNegate, node, "negate", time, 0);
        else myNegate = false;
        if (true)
            OP_Utils::evalOpParm(myFullAccuracy, node, "fullaccuracy", time, 0);
        else myFullAccuracy = false;
        if (true && ( (!(((getFullAccuracy()==1)))) ) )
            OP_Utils::evalOpParm(myAccuracyScale, node, "accuracyscale", time, 0);
        else myAccuracyScale = 2;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_WindingNumberParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myQueryPoints);
                break;
            case 1:
                coerceValue(value, myMeshPrims);
                break;
            case 2:
                coerceValue(value, myType);
                break;
            case 3:
                coerceValue(value, myAttrib);
                break;
            case 4:
                coerceValue(value, myAsSolidAngle);
                break;
            case 5:
                coerceValue(value, myNegate);
                break;
            case 6:
                coerceValue(value, myFullAccuracy);
                break;
            case 7:
                coerceValue(value, myAccuracyScale);
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
                coerceValue(myQueryPoints, value);
                break;
            case 1:
                coerceValue(myMeshPrims, value);
                break;
            case 2:
                coerceValue(myType, value);
                break;
            case 3:
                coerceValue(myAttrib, value);
                break;
            case 4:
                coerceValue(myAsSolidAngle, value);
                break;
            case 5:
                coerceValue(myNegate, value);
                break;
            case 6:
                coerceValue(myFullAccuracy, value);
                break;
            case 7:
                coerceValue(myAccuracyScale, value);
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
        return 8;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "querypoints";
            case 1:
                return "meshprims";
            case 2:
                return "type";
            case 3:
                return "attrib";
            case 4:
                return "assolidangle";
            case 5:
                return "negate";
            case 6:
                return "fullaccuracy";
            case 7:
                return "accuracyscale";

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
                    return PARM_STRING;
                case 4:
                    return PARM_INTEGER;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_INTEGER;
                case 7:
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
        saveData(os, myQueryPoints);
        saveData(os, myMeshPrims);
        saveData(os, myType);
        saveData(os, myAttrib);
        saveData(os, myAsSolidAngle);
        saveData(os, myNegate);
        saveData(os, myFullAccuracy);
        saveData(os, myAccuracyScale);

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
        loadData(is, myQueryPoints);
        loadData(is, myMeshPrims);
        loadData(is, myType);
        loadData(is, myAttrib);
        loadData(is, myAsSolidAngle);
        loadData(is, myNegate);
        loadData(is, myFullAccuracy);
        loadData(is, myAccuracyScale);

        return true;
    }

    const UT_StringHolder & getQueryPoints() const { return myQueryPoints; }
    void setQueryPoints(const UT_StringHolder & val) { myQueryPoints = val; }
    UT_StringHolder opQueryPoints(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getQueryPoints();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "querypoints", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getMeshPrims() const { return myMeshPrims; }
    void setMeshPrims(const UT_StringHolder & val) { myMeshPrims = val; }
    UT_StringHolder opMeshPrims(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMeshPrims();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "meshprims", cookparms.getCookTime(), 0);
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
    const UT_StringHolder & getAttrib() const { return myAttrib; }
    void setAttrib(const UT_StringHolder & val) { myAttrib = val; }
    UT_StringHolder opAttrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAttrib();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "attrib", cookparms.getCookTime(), 0);
        return result;
    }
    bool getAsSolidAngle() const { return myAsSolidAngle; }
    void setAsSolidAngle(bool val) { myAsSolidAngle = val; }
    bool opAsSolidAngle(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAsSolidAngle();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "assolidangle", cookparms.getCookTime(), 0);
        return result;
    }
    bool getNegate() const { return myNegate; }
    void setNegate(bool val) { myNegate = val; }
    bool opNegate(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getNegate();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "negate", cookparms.getCookTime(), 0);
        return result;
    }
    bool getFullAccuracy() const { return myFullAccuracy; }
    void setFullAccuracy(bool val) { myFullAccuracy = val; }
    bool opFullAccuracy(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getFullAccuracy();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "fullaccuracy", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getAccuracyScale() const { return myAccuracyScale; }
    void setAccuracyScale(fpreal64 val) { myAccuracyScale = val; }
    fpreal64 opAccuracyScale(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAccuracyScale();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "accuracyscale", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myQueryPoints;
    UT_StringHolder myMeshPrims;
    int64 myType;
    UT_StringHolder myAttrib;
    bool myAsSolidAngle;
    bool myNegate;
    bool myFullAccuracy;
    fpreal64 myAccuracyScale;

};