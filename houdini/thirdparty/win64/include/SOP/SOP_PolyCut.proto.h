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
namespace SOP_PolyCutEnums
{
    enum class Type
    {
        POINTS = 0,
        EDGES
    };
    enum class Strategy
    {
        REMOVE = 0,
        CUT
    };
    enum class DetectEdgeChanges
    {
        ATTRIBCROSSING = 0,
        ATTRIBCHANGE
    };
}


class SOP_API SOP_PolyCutParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_PolyCutParms()
    {
        myPolygons = ""_sh;
        myType = 0;
        myCutPoints = ""_sh;
        myCutEdges = ""_sh;
        myStrategy = 0;
        myDetectEdgeChanges = 0;
        myCutAttrib = ""_sh;
        myCutValue = 0;
        myCutThreshold = 1;
        myKeepClosed = false;

    }

    explicit SOP_PolyCutParms(const SOP_PolyCutParms &) = default;

    virtual ~SOP_PolyCutParms() {}

    bool operator==(const SOP_PolyCutParms &src) const
    {
        if (myPolygons != src.myPolygons) return false;
        if (myType != src.myType) return false;
        if (myCutPoints != src.myCutPoints) return false;
        if (myCutEdges != src.myCutEdges) return false;
        if (myStrategy != src.myStrategy) return false;
        if (myDetectEdgeChanges != src.myDetectEdgeChanges) return false;
        if (myCutAttrib != src.myCutAttrib) return false;
        if (myCutValue != src.myCutValue) return false;
        if (myCutThreshold != src.myCutThreshold) return false;
        if (myKeepClosed != src.myKeepClosed) return false;

        return true;
    }
    bool operator!=(const SOP_PolyCutParms &src) const
    {
        return !operator==(src);
    }
    using Type = SOP_PolyCutEnums::Type;
    using Strategy = SOP_PolyCutEnums::Strategy;
    using DetectEdgeChanges = SOP_PolyCutEnums::DetectEdgeChanges;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myPolygons, node, "polygons", time, 0);
        else myPolygons = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myType, node, "type", time, 0);
        else myType = 0;
        if (true && ( (!(((int64(getType())!=0)))) ) )
            OP_Utils::evalOpParm(myCutPoints, node, "cutpoints", time, 0);
        else myCutPoints = ""_sh;
        if (true && ( (!(((int64(getType())!=1)))) ) )
            OP_Utils::evalOpParm(myCutEdges, node, "cutedges", time, 0);
        else myCutEdges = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myStrategy, node, "strategy", time, 0);
        else myStrategy = 0;
        if (true)
            OP_Utils::evalOpParm(myDetectEdgeChanges, node, "detectedgechanges", time, 0);
        else myDetectEdgeChanges = 0;
        if (true)
            OP_Utils::evalOpParm(myCutAttrib, node, "cutattrib", time, 0);
        else myCutAttrib = ""_sh;
        if (true && ( (!(((int64(getDetectEdgeChanges())!=0))||((getCutAttrib()=="")))) ) )
            OP_Utils::evalOpParm(myCutValue, node, "cutvalue", time, 0);
        else myCutValue = 0;
        if (true && ( (!(((int64(getDetectEdgeChanges())!=1))||((getCutAttrib()=="")))) ) )
            OP_Utils::evalOpParm(myCutThreshold, node, "cutthreshold", time, 0);
        else myCutThreshold = 1;
        if (true)
            OP_Utils::evalOpParm(myKeepClosed, node, "keepclosed", time, 0);
        else myKeepClosed = false;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_PolyCutParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myPolygons);
                break;
            case 1:
                coerceValue(value, myType);
                break;
            case 2:
                coerceValue(value, myCutPoints);
                break;
            case 3:
                coerceValue(value, myCutEdges);
                break;
            case 4:
                coerceValue(value, myStrategy);
                break;
            case 5:
                coerceValue(value, myDetectEdgeChanges);
                break;
            case 6:
                coerceValue(value, myCutAttrib);
                break;
            case 7:
                coerceValue(value, myCutValue);
                break;
            case 8:
                coerceValue(value, myCutThreshold);
                break;
            case 9:
                coerceValue(value, myKeepClosed);
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
                coerceValue(myPolygons, value);
                break;
            case 1:
                coerceValue(myType, value);
                break;
            case 2:
                coerceValue(myCutPoints, value);
                break;
            case 3:
                coerceValue(myCutEdges, value);
                break;
            case 4:
                coerceValue(myStrategy, value);
                break;
            case 5:
                coerceValue(myDetectEdgeChanges, value);
                break;
            case 6:
                coerceValue(myCutAttrib, value);
                break;
            case 7:
                coerceValue(myCutValue, value);
                break;
            case 8:
                coerceValue(myCutThreshold, value);
                break;
            case 9:
                coerceValue(myKeepClosed, value);
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
        return 10;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "polygons";
            case 1:
                return "type";
            case 2:
                return "cutpoints";
            case 3:
                return "cutedges";
            case 4:
                return "strategy";
            case 5:
                return "detectedgechanges";
            case 6:
                return "cutattrib";
            case 7:
                return "cutvalue";
            case 8:
                return "cutthreshold";
            case 9:
                return "keepclosed";

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
                    return PARM_STRING;
                case 3:
                    return PARM_STRING;
                case 4:
                    return PARM_INTEGER;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_STRING;
                case 7:
                    return PARM_FLOAT;
                case 8:
                    return PARM_FLOAT;
                case 9:
                    return PARM_INTEGER;

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
        saveData(os, myPolygons);
        saveData(os, myType);
        saveData(os, myCutPoints);
        saveData(os, myCutEdges);
        saveData(os, myStrategy);
        saveData(os, myDetectEdgeChanges);
        saveData(os, myCutAttrib);
        saveData(os, myCutValue);
        saveData(os, myCutThreshold);
        saveData(os, myKeepClosed);

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
        loadData(is, myPolygons);
        loadData(is, myType);
        loadData(is, myCutPoints);
        loadData(is, myCutEdges);
        loadData(is, myStrategy);
        loadData(is, myDetectEdgeChanges);
        loadData(is, myCutAttrib);
        loadData(is, myCutValue);
        loadData(is, myCutThreshold);
        loadData(is, myKeepClosed);

        return true;
    }

    const UT_StringHolder & getPolygons() const { return myPolygons; }
    void setPolygons(const UT_StringHolder & val) { myPolygons = val; }
    UT_StringHolder opPolygons(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPolygons();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "polygons", cookparms.getCookTime(), 0);
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
    const UT_StringHolder & getCutPoints() const { return myCutPoints; }
    void setCutPoints(const UT_StringHolder & val) { myCutPoints = val; }
    UT_StringHolder opCutPoints(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCutPoints();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "cutpoints", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getCutEdges() const { return myCutEdges; }
    void setCutEdges(const UT_StringHolder & val) { myCutEdges = val; }
    UT_StringHolder opCutEdges(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCutEdges();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "cutedges", cookparms.getCookTime(), 0);
        return result;
    }
    Strategy getStrategy() const { return Strategy(myStrategy); }
    void setStrategy(Strategy val) { myStrategy = int64(val); }
    Strategy opStrategy(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getStrategy();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "strategy", cookparms.getCookTime(), 0);
        return Strategy(result);
    }
    DetectEdgeChanges getDetectEdgeChanges() const { return DetectEdgeChanges(myDetectEdgeChanges); }
    void setDetectEdgeChanges(DetectEdgeChanges val) { myDetectEdgeChanges = int64(val); }
    DetectEdgeChanges opDetectEdgeChanges(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDetectEdgeChanges();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "detectedgechanges", cookparms.getCookTime(), 0);
        return DetectEdgeChanges(result);
    }
    const UT_StringHolder & getCutAttrib() const { return myCutAttrib; }
    void setCutAttrib(const UT_StringHolder & val) { myCutAttrib = val; }
    UT_StringHolder opCutAttrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCutAttrib();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "cutattrib", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getCutValue() const { return myCutValue; }
    void setCutValue(fpreal64 val) { myCutValue = val; }
    fpreal64 opCutValue(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCutValue();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "cutvalue", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getCutThreshold() const { return myCutThreshold; }
    void setCutThreshold(fpreal64 val) { myCutThreshold = val; }
    fpreal64 opCutThreshold(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCutThreshold();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "cutthreshold", cookparms.getCookTime(), 0);
        return result;
    }
    bool getKeepClosed() const { return myKeepClosed; }
    void setKeepClosed(bool val) { myKeepClosed = val; }
    bool opKeepClosed(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getKeepClosed();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "keepclosed", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myPolygons;
    int64 myType;
    UT_StringHolder myCutPoints;
    UT_StringHolder myCutEdges;
    int64 myStrategy;
    int64 myDetectEdgeChanges;
    UT_StringHolder myCutAttrib;
    fpreal64 myCutValue;
    fpreal64 myCutThreshold;
    bool myKeepClosed;

};
