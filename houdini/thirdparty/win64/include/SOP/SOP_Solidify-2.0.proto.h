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

class SOP_API SOP_Solidify_2_0Parms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_Solidify_2_0Parms()
    {
        myTetGroup = ""_sh;
        myPolyGroup = ""_sh;
        myKeepPolygons = false;
        mySolidBdry = 0;
        myOutputSolidity = false;
        mySolidityAttrib = "solidity"_sh;

    }

    explicit SOP_Solidify_2_0Parms(const SOP_Solidify_2_0Parms &) = default;

    virtual ~SOP_Solidify_2_0Parms() {}

    bool operator==(const SOP_Solidify_2_0Parms &src) const
    {
        if (myTetGroup != src.myTetGroup) return false;
        if (myPolyGroup != src.myPolyGroup) return false;
        if (myKeepPolygons != src.myKeepPolygons) return false;
        if (mySolidBdry != src.mySolidBdry) return false;
        if (myOutputSolidity != src.myOutputSolidity) return false;
        if (mySolidityAttrib != src.mySolidityAttrib) return false;

        return true;
    }
    bool operator!=(const SOP_Solidify_2_0Parms &src) const
    {
        return !operator==(src);
    }



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myTetGroup, node, "tetgroup", time, 0);
        else myTetGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myPolyGroup, node, "polygroup", time, 0);
        else myPolyGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myKeepPolygons, node, "keeppolygons", time, 0);
        else myKeepPolygons = false;
        if (true)
            OP_Utils::evalOpParm(mySolidBdry, node, "solidbdry", time, 0);
        else mySolidBdry = 0;
        if (true)
            OP_Utils::evalOpParm(myOutputSolidity, node, "outputsolidity", time, 0);
        else myOutputSolidity = false;
        if (true && ( (!(((getOutputSolidity()==0)))) ) )
            OP_Utils::evalOpParm(mySolidityAttrib, node, "solidityattrib", time, 0);
        else mySolidityAttrib = "solidity"_sh;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_Solidify_2_0Parms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myTetGroup);
                break;
            case 1:
                coerceValue(value, myPolyGroup);
                break;
            case 2:
                coerceValue(value, myKeepPolygons);
                break;
            case 3:
                coerceValue(value, mySolidBdry);
                break;
            case 4:
                coerceValue(value, myOutputSolidity);
                break;
            case 5:
                coerceValue(value, mySolidityAttrib);
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
                coerceValue(myTetGroup, value);
                break;
            case 1:
                coerceValue(myPolyGroup, value);
                break;
            case 2:
                coerceValue(myKeepPolygons, value);
                break;
            case 3:
                coerceValue(mySolidBdry, value);
                break;
            case 4:
                coerceValue(myOutputSolidity, value);
                break;
            case 5:
                coerceValue(mySolidityAttrib, value);
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
        return 6;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "tetgroup";
            case 1:
                return "polygroup";
            case 2:
                return "keeppolygons";
            case 3:
                return "solidbdry";
            case 4:
                return "outputsolidity";
            case 5:
                return "solidityattrib";

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
                    return PARM_FLOAT;
                case 4:
                    return PARM_INTEGER;
                case 5:
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
        saveData(os, myTetGroup);
        saveData(os, myPolyGroup);
        saveData(os, myKeepPolygons);
        saveData(os, mySolidBdry);
        saveData(os, myOutputSolidity);
        saveData(os, mySolidityAttrib);

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
        loadData(is, myTetGroup);
        loadData(is, myPolyGroup);
        loadData(is, myKeepPolygons);
        loadData(is, mySolidBdry);
        loadData(is, myOutputSolidity);
        loadData(is, mySolidityAttrib);

        return true;
    }

    const UT_StringHolder & getTetGroup() const { return myTetGroup; }
    void setTetGroup(const UT_StringHolder & val) { myTetGroup = val; }
    UT_StringHolder opTetGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTetGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "tetgroup", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getPolyGroup() const { return myPolyGroup; }
    void setPolyGroup(const UT_StringHolder & val) { myPolyGroup = val; }
    UT_StringHolder opPolyGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPolyGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "polygroup", cookparms.getCookTime(), 0);
        return result;
    }
    bool getKeepPolygons() const { return myKeepPolygons; }
    void setKeepPolygons(bool val) { myKeepPolygons = val; }
    bool opKeepPolygons(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getKeepPolygons();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "keeppolygons", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getSolidBdry() const { return mySolidBdry; }
    void setSolidBdry(fpreal64 val) { mySolidBdry = val; }
    fpreal64 opSolidBdry(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSolidBdry();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "solidbdry", cookparms.getCookTime(), 0);
        return result;
    }
    bool getOutputSolidity() const { return myOutputSolidity; }
    void setOutputSolidity(bool val) { myOutputSolidity = val; }
    bool opOutputSolidity(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getOutputSolidity();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "outputsolidity", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSolidityAttrib() const { return mySolidityAttrib; }
    void setSolidityAttrib(const UT_StringHolder & val) { mySolidityAttrib = val; }
    UT_StringHolder opSolidityAttrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSolidityAttrib();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "solidityattrib", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myTetGroup;
    UT_StringHolder myPolyGroup;
    bool myKeepPolygons;
    fpreal64 mySolidBdry;
    bool myOutputSolidity;
    UT_StringHolder mySolidityAttrib;

};
