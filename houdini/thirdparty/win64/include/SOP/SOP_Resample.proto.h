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
namespace SOP_ResampleEnums
{
    enum class Method
    {
        DIST = 0,
        X,
        Y,
        Z
    };
    enum class Measure
    {
        ARC = 0,
        CHORD
    };
    enum class Treatpolysas
    {
        STRAIGHT = 0,
        SUBD,
        INTERP
    };
}


class SOP_API SOP_ResampleParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_ResampleParms()
    {
        myGroup = ""_sh;
        myLod = 1;
        myEdge = false;
        myMethod = 0;
        myMeasure = 0;
        myDolength = true;
        myLength = 0.1;
        myDosegs = false;
        mySegs = 10;
        myUseattribs = true;
        myAllequal = true;
        myLast = false;
        myRandomshift = false;
        myOnlypoints = false;
        myTreatpolysas = 0;
        myOutputsubdpoly = false;
        myDoptdistattr = false;
        myPtdistattr = "ptdist"_sh;
        myDotangentattr = false;
        myTangentattr = "tangentu"_sh;
        myDocurveuattr = false;
        myCurveuattr = "curveu"_sh;
        myDocurvenumattr = false;
        myCurvenumattr = "curvenum"_sh;

    }

    explicit SOP_ResampleParms(const SOP_ResampleParms &) = default;

    virtual ~SOP_ResampleParms() {}

    bool operator==(const SOP_ResampleParms &src) const
    {
        if (myGroup != src.myGroup) return false;
        if (myLod != src.myLod) return false;
        if (myEdge != src.myEdge) return false;
        if (myMethod != src.myMethod) return false;
        if (myMeasure != src.myMeasure) return false;
        if (myDolength != src.myDolength) return false;
        if (myLength != src.myLength) return false;
        if (myDosegs != src.myDosegs) return false;
        if (mySegs != src.mySegs) return false;
        if (myUseattribs != src.myUseattribs) return false;
        if (myAllequal != src.myAllequal) return false;
        if (myLast != src.myLast) return false;
        if (myRandomshift != src.myRandomshift) return false;
        if (myOnlypoints != src.myOnlypoints) return false;
        if (myTreatpolysas != src.myTreatpolysas) return false;
        if (myOutputsubdpoly != src.myOutputsubdpoly) return false;
        if (myDoptdistattr != src.myDoptdistattr) return false;
        if (myPtdistattr != src.myPtdistattr) return false;
        if (myDotangentattr != src.myDotangentattr) return false;
        if (myTangentattr != src.myTangentattr) return false;
        if (myDocurveuattr != src.myDocurveuattr) return false;
        if (myCurveuattr != src.myCurveuattr) return false;
        if (myDocurvenumattr != src.myDocurvenumattr) return false;
        if (myCurvenumattr != src.myCurvenumattr) return false;

        return true;
    }
    bool operator!=(const SOP_ResampleParms &src) const
    {
        return !operator==(src);
    }
    using Method = SOP_ResampleEnums::Method;
    using Measure = SOP_ResampleEnums::Measure;
    using Treatpolysas = SOP_ResampleEnums::Treatpolysas;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myGroup, node, "group", time, 0);
        else myGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myLod, node, "lod", time, 0);
        else myLod = 1;
        if (true)
            OP_Utils::evalOpParm(myEdge, node, "edge", time, 0);
        else myEdge = false;
        if (true)
            OP_Utils::evalOpParm(myMethod, node, "method", time, 0);
        else myMethod = 0;
        if (true)
            OP_Utils::evalOpParm(myMeasure, node, "measure", time, 0);
        else myMeasure = 0;
        if (true)
            OP_Utils::evalOpParm(myDolength, node, "dolength", time, 0);
        else myDolength = true;
        if (true && ( (!(((getDolength()==0)))) ) )
            OP_Utils::evalOpParm(myLength, node, "length", time, 0);
        else myLength = 0.1;
        if (true)
            OP_Utils::evalOpParm(myDosegs, node, "dosegs", time, 0);
        else myDosegs = false;
        if (true && ( (!(((getDosegs()==0)))) ) )
            OP_Utils::evalOpParm(mySegs, node, "segs", time, 0);
        else mySegs = 10;
        if (true)
            OP_Utils::evalOpParm(myUseattribs, node, "useattribs", time, 0);
        else myUseattribs = true;
        if (true && ( (!(((getDolength()==0))||((getEdge()==1))||((int64(getMeasure())!=0))||((int64(getMethod())!=0)))) ) )
            OP_Utils::evalOpParm(myAllequal, node, "allequal", time, 0);
        else myAllequal = true;
        if (true && ( (!(((getDolength()==0))||((getEdge()==1))||((getDolength()==1)&&(getEdge()==0)&&(int64(getMeasure())==0)&&(int64(getMethod())==0)&&(getAllequal()==1)))) ) )
            OP_Utils::evalOpParm(myLast, node, "last", time, 0);
        else myLast = false;
        if (true && ( (!(((getDolength()==0))||((getEdge()==1))||((getDolength()==1)&&(getEdge()==0)&&(int64(getMeasure())==0)&&(int64(getMethod())==0)&&(getAllequal()==1)))) ) )
            OP_Utils::evalOpParm(myRandomshift, node, "randomshift", time, 0);
        else myRandomshift = false;
        if (true)
            OP_Utils::evalOpParm(myOnlypoints, node, "onlypoints", time, 0);
        else myOnlypoints = false;
        if (true)
            OP_Utils::evalOpParm(myTreatpolysas, node, "treatpolysas", time, 0);
        else myTreatpolysas = 0;
        if (true)
            OP_Utils::evalOpParm(myOutputsubdpoly, node, "outputsubdpoly", time, 0);
        else myOutputsubdpoly = false;
        if (true)
            OP_Utils::evalOpParm(myDoptdistattr, node, "doptdistattr", time, 0);
        else myDoptdistattr = false;
        if (true && ( (!(((getDoptdistattr()==0)))) ) )
            OP_Utils::evalOpParm(myPtdistattr, node, "ptdistattr", time, 0);
        else myPtdistattr = "ptdist"_sh;
        if (true)
            OP_Utils::evalOpParm(myDotangentattr, node, "dotangentattr", time, 0);
        else myDotangentattr = false;
        if (true && ( (!(((getDotangentattr()==0)))) ) )
            OP_Utils::evalOpParm(myTangentattr, node, "tangentattr", time, 0);
        else myTangentattr = "tangentu"_sh;
        if (true)
            OP_Utils::evalOpParm(myDocurveuattr, node, "docurveuattr", time, 0);
        else myDocurveuattr = false;
        if (true && ( (!(((getDocurveuattr()==0)))) ) )
            OP_Utils::evalOpParm(myCurveuattr, node, "curveuattr", time, 0);
        else myCurveuattr = "curveu"_sh;
        if (true)
            OP_Utils::evalOpParm(myDocurvenumattr, node, "docurvenumattr", time, 0);
        else myDocurvenumattr = false;
        if (true && ( (!(((getDocurvenumattr()==0)))) ) )
            OP_Utils::evalOpParm(myCurvenumattr, node, "curvenumattr", time, 0);
        else myCurvenumattr = "curvenum"_sh;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_ResampleParms *)src);
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
                coerceValue(value, myLod);
                break;
            case 2:
                coerceValue(value, myEdge);
                break;
            case 3:
                coerceValue(value, myMethod);
                break;
            case 4:
                coerceValue(value, myMeasure);
                break;
            case 5:
                coerceValue(value, myDolength);
                break;
            case 6:
                coerceValue(value, myLength);
                break;
            case 7:
                coerceValue(value, myDosegs);
                break;
            case 8:
                coerceValue(value, mySegs);
                break;
            case 9:
                coerceValue(value, myUseattribs);
                break;
            case 10:
                coerceValue(value, myAllequal);
                break;
            case 11:
                coerceValue(value, myLast);
                break;
            case 12:
                coerceValue(value, myRandomshift);
                break;
            case 13:
                coerceValue(value, myOnlypoints);
                break;
            case 14:
                coerceValue(value, myTreatpolysas);
                break;
            case 15:
                coerceValue(value, myOutputsubdpoly);
                break;
            case 16:
                coerceValue(value, myDoptdistattr);
                break;
            case 17:
                coerceValue(value, myPtdistattr);
                break;
            case 18:
                coerceValue(value, myDotangentattr);
                break;
            case 19:
                coerceValue(value, myTangentattr);
                break;
            case 20:
                coerceValue(value, myDocurveuattr);
                break;
            case 21:
                coerceValue(value, myCurveuattr);
                break;
            case 22:
                coerceValue(value, myDocurvenumattr);
                break;
            case 23:
                coerceValue(value, myCurvenumattr);
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
                coerceValue(myLod, value);
                break;
            case 2:
                coerceValue(myEdge, value);
                break;
            case 3:
                coerceValue(myMethod, value);
                break;
            case 4:
                coerceValue(myMeasure, value);
                break;
            case 5:
                coerceValue(myDolength, value);
                break;
            case 6:
                coerceValue(myLength, value);
                break;
            case 7:
                coerceValue(myDosegs, value);
                break;
            case 8:
                coerceValue(mySegs, value);
                break;
            case 9:
                coerceValue(myUseattribs, value);
                break;
            case 10:
                coerceValue(myAllequal, value);
                break;
            case 11:
                coerceValue(myLast, value);
                break;
            case 12:
                coerceValue(myRandomshift, value);
                break;
            case 13:
                coerceValue(myOnlypoints, value);
                break;
            case 14:
                coerceValue(myTreatpolysas, value);
                break;
            case 15:
                coerceValue(myOutputsubdpoly, value);
                break;
            case 16:
                coerceValue(myDoptdistattr, value);
                break;
            case 17:
                coerceValue(myPtdistattr, value);
                break;
            case 18:
                coerceValue(myDotangentattr, value);
                break;
            case 19:
                coerceValue(myTangentattr, value);
                break;
            case 20:
                coerceValue(myDocurveuattr, value);
                break;
            case 21:
                coerceValue(myCurveuattr, value);
                break;
            case 22:
                coerceValue(myDocurvenumattr, value);
                break;
            case 23:
                coerceValue(myCurvenumattr, value);
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
        return 24;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "group";
            case 1:
                return "lod";
            case 2:
                return "edge";
            case 3:
                return "method";
            case 4:
                return "measure";
            case 5:
                return "dolength";
            case 6:
                return "length";
            case 7:
                return "dosegs";
            case 8:
                return "segs";
            case 9:
                return "useattribs";
            case 10:
                return "allequal";
            case 11:
                return "last";
            case 12:
                return "randomshift";
            case 13:
                return "onlypoints";
            case 14:
                return "treatpolysas";
            case 15:
                return "outputsubdpoly";
            case 16:
                return "doptdistattr";
            case 17:
                return "ptdistattr";
            case 18:
                return "dotangentattr";
            case 19:
                return "tangentattr";
            case 20:
                return "docurveuattr";
            case 21:
                return "curveuattr";
            case 22:
                return "docurvenumattr";
            case 23:
                return "curvenumattr";

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
                    return PARM_FLOAT;
                case 2:
                    return PARM_INTEGER;
                case 3:
                    return PARM_INTEGER;
                case 4:
                    return PARM_INTEGER;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_FLOAT;
                case 7:
                    return PARM_INTEGER;
                case 8:
                    return PARM_INTEGER;
                case 9:
                    return PARM_INTEGER;
                case 10:
                    return PARM_INTEGER;
                case 11:
                    return PARM_INTEGER;
                case 12:
                    return PARM_INTEGER;
                case 13:
                    return PARM_INTEGER;
                case 14:
                    return PARM_INTEGER;
                case 15:
                    return PARM_INTEGER;
                case 16:
                    return PARM_INTEGER;
                case 17:
                    return PARM_STRING;
                case 18:
                    return PARM_INTEGER;
                case 19:
                    return PARM_STRING;
                case 20:
                    return PARM_INTEGER;
                case 21:
                    return PARM_STRING;
                case 22:
                    return PARM_INTEGER;
                case 23:
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
        saveData(os, myGroup);
        saveData(os, myLod);
        saveData(os, myEdge);
        saveData(os, myMethod);
        saveData(os, myMeasure);
        saveData(os, myDolength);
        saveData(os, myLength);
        saveData(os, myDosegs);
        saveData(os, mySegs);
        saveData(os, myUseattribs);
        saveData(os, myAllequal);
        saveData(os, myLast);
        saveData(os, myRandomshift);
        saveData(os, myOnlypoints);
        saveData(os, myTreatpolysas);
        saveData(os, myOutputsubdpoly);
        saveData(os, myDoptdistattr);
        saveData(os, myPtdistattr);
        saveData(os, myDotangentattr);
        saveData(os, myTangentattr);
        saveData(os, myDocurveuattr);
        saveData(os, myCurveuattr);
        saveData(os, myDocurvenumattr);
        saveData(os, myCurvenumattr);

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
        loadData(is, myLod);
        loadData(is, myEdge);
        loadData(is, myMethod);
        loadData(is, myMeasure);
        loadData(is, myDolength);
        loadData(is, myLength);
        loadData(is, myDosegs);
        loadData(is, mySegs);
        loadData(is, myUseattribs);
        loadData(is, myAllequal);
        loadData(is, myLast);
        loadData(is, myRandomshift);
        loadData(is, myOnlypoints);
        loadData(is, myTreatpolysas);
        loadData(is, myOutputsubdpoly);
        loadData(is, myDoptdistattr);
        loadData(is, myPtdistattr);
        loadData(is, myDotangentattr);
        loadData(is, myTangentattr);
        loadData(is, myDocurveuattr);
        loadData(is, myCurveuattr);
        loadData(is, myDocurvenumattr);
        loadData(is, myCurvenumattr);

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
    fpreal64 getLod() const { return myLod; }
    void setLod(fpreal64 val) { myLod = val; }
    fpreal64 opLod(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLod();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "lod", cookparms.getCookTime(), 0);
        return result;
    }
    bool getEdge() const { return myEdge; }
    void setEdge(bool val) { myEdge = val; }
    bool opEdge(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getEdge();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "edge", cookparms.getCookTime(), 0);
        return result;
    }
    Method getMethod() const { return Method(myMethod); }
    void setMethod(Method val) { myMethod = int64(val); }
    Method opMethod(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMethod();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "method", cookparms.getCookTime(), 0);
        return Method(result);
    }
    Measure getMeasure() const { return Measure(myMeasure); }
    void setMeasure(Measure val) { myMeasure = int64(val); }
    Measure opMeasure(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMeasure();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "measure", cookparms.getCookTime(), 0);
        return Measure(result);
    }
    bool getDolength() const { return myDolength; }
    void setDolength(bool val) { myDolength = val; }
    bool opDolength(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDolength();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "dolength", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getLength() const { return myLength; }
    void setLength(fpreal64 val) { myLength = val; }
    fpreal64 opLength(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLength();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "length", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDosegs() const { return myDosegs; }
    void setDosegs(bool val) { myDosegs = val; }
    bool opDosegs(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDosegs();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "dosegs", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getSegs() const { return mySegs; }
    void setSegs(int64 val) { mySegs = val; }
    int64 opSegs(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSegs();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "segs", cookparms.getCookTime(), 0);
        return result;
    }
    bool getUseattribs() const { return myUseattribs; }
    void setUseattribs(bool val) { myUseattribs = val; }
    bool opUseattribs(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUseattribs();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "useattribs", cookparms.getCookTime(), 0);
        return result;
    }
    bool getAllequal() const { return myAllequal; }
    void setAllequal(bool val) { myAllequal = val; }
    bool opAllequal(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAllequal();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "allequal", cookparms.getCookTime(), 0);
        return result;
    }
    bool getLast() const { return myLast; }
    void setLast(bool val) { myLast = val; }
    bool opLast(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLast();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "last", cookparms.getCookTime(), 0);
        return result;
    }
    bool getRandomshift() const { return myRandomshift; }
    void setRandomshift(bool val) { myRandomshift = val; }
    bool opRandomshift(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRandomshift();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "randomshift", cookparms.getCookTime(), 0);
        return result;
    }
    bool getOnlypoints() const { return myOnlypoints; }
    void setOnlypoints(bool val) { myOnlypoints = val; }
    bool opOnlypoints(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getOnlypoints();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "onlypoints", cookparms.getCookTime(), 0);
        return result;
    }
    Treatpolysas getTreatpolysas() const { return Treatpolysas(myTreatpolysas); }
    void setTreatpolysas(Treatpolysas val) { myTreatpolysas = int64(val); }
    Treatpolysas opTreatpolysas(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTreatpolysas();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "treatpolysas", cookparms.getCookTime(), 0);
        return Treatpolysas(result);
    }
    bool getOutputsubdpoly() const { return myOutputsubdpoly; }
    void setOutputsubdpoly(bool val) { myOutputsubdpoly = val; }
    bool opOutputsubdpoly(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getOutputsubdpoly();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "outputsubdpoly", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDoptdistattr() const { return myDoptdistattr; }
    void setDoptdistattr(bool val) { myDoptdistattr = val; }
    bool opDoptdistattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDoptdistattr();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "doptdistattr", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getPtdistattr() const { return myPtdistattr; }
    void setPtdistattr(const UT_StringHolder & val) { myPtdistattr = val; }
    UT_StringHolder opPtdistattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPtdistattr();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "ptdistattr", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDotangentattr() const { return myDotangentattr; }
    void setDotangentattr(bool val) { myDotangentattr = val; }
    bool opDotangentattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDotangentattr();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "dotangentattr", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getTangentattr() const { return myTangentattr; }
    void setTangentattr(const UT_StringHolder & val) { myTangentattr = val; }
    UT_StringHolder opTangentattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTangentattr();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "tangentattr", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDocurveuattr() const { return myDocurveuattr; }
    void setDocurveuattr(bool val) { myDocurveuattr = val; }
    bool opDocurveuattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDocurveuattr();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "docurveuattr", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getCurveuattr() const { return myCurveuattr; }
    void setCurveuattr(const UT_StringHolder & val) { myCurveuattr = val; }
    UT_StringHolder opCurveuattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCurveuattr();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "curveuattr", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDocurvenumattr() const { return myDocurvenumattr; }
    void setDocurvenumattr(bool val) { myDocurvenumattr = val; }
    bool opDocurvenumattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDocurvenumattr();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "docurvenumattr", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getCurvenumattr() const { return myCurvenumattr; }
    void setCurvenumattr(const UT_StringHolder & val) { myCurvenumattr = val; }
    UT_StringHolder opCurvenumattr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCurvenumattr();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "curvenumattr", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myGroup;
    fpreal64 myLod;
    bool myEdge;
    int64 myMethod;
    int64 myMeasure;
    bool myDolength;
    fpreal64 myLength;
    bool myDosegs;
    int64 mySegs;
    bool myUseattribs;
    bool myAllequal;
    bool myLast;
    bool myRandomshift;
    bool myOnlypoints;
    int64 myTreatpolysas;
    bool myOutputsubdpoly;
    bool myDoptdistattr;
    UT_StringHolder myPtdistattr;
    bool myDotangentattr;
    UT_StringHolder myTangentattr;
    bool myDocurveuattr;
    UT_StringHolder myCurveuattr;
    bool myDocurvenumattr;
    UT_StringHolder myCurvenumattr;

};
