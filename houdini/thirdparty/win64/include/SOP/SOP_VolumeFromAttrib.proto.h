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
namespace SOP_VolumeFromAttribEnums
{
    enum class Calculationtype
    {
        COPY = 0,
        ADD,
        SUB,
        MUL,
        DIV,
        MAX,
        MIN,
        AVERAGE
    };
}


class SOP_API SOP_VolumeFromAttribParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_VolumeFromAttribParms()
    {
        myGroup = ""_sh;
        myPointgrp = ""_sh;
        myUseattrib = false;
        myAttrib = ""_sh;
        myDisableonmissing = false;
        myAccumulate = false;
        myExtrapolate = false;
        myUsemaxextrapolate = false;
        myMaxextrapolate = 2;
        myUsemaxextrapolatedist = false;
        myMaxextrapolatedist = 0;
        myThreshold = 1;
        myBandwidth = 0;
        myCalculationtype = 0;
        myDstpreadd = 0;
        myDstpremul = 1;
        myScalebyvolume = false;
        mySrcpreadd = 0;
        mySrcpremul = 1;
        myPostadd = 0;
        myPostmul = 1;

    }

    explicit SOP_VolumeFromAttribParms(const SOP_VolumeFromAttribParms &) = default;

    virtual ~SOP_VolumeFromAttribParms() {}

    bool operator==(const SOP_VolumeFromAttribParms &src) const
    {
        if (myGroup != src.myGroup) return false;
        if (myPointgrp != src.myPointgrp) return false;
        if (myUseattrib != src.myUseattrib) return false;
        if (myAttrib != src.myAttrib) return false;
        if (myDisableonmissing != src.myDisableonmissing) return false;
        if (myAccumulate != src.myAccumulate) return false;
        if (myExtrapolate != src.myExtrapolate) return false;
        if (myUsemaxextrapolate != src.myUsemaxextrapolate) return false;
        if (myMaxextrapolate != src.myMaxextrapolate) return false;
        if (myUsemaxextrapolatedist != src.myUsemaxextrapolatedist) return false;
        if (myMaxextrapolatedist != src.myMaxextrapolatedist) return false;
        if (myThreshold != src.myThreshold) return false;
        if (myBandwidth != src.myBandwidth) return false;
        if (myCalculationtype != src.myCalculationtype) return false;
        if (myDstpreadd != src.myDstpreadd) return false;
        if (myDstpremul != src.myDstpremul) return false;
        if (myScalebyvolume != src.myScalebyvolume) return false;
        if (mySrcpreadd != src.mySrcpreadd) return false;
        if (mySrcpremul != src.mySrcpremul) return false;
        if (myPostadd != src.myPostadd) return false;
        if (myPostmul != src.myPostmul) return false;

        return true;
    }
    bool operator!=(const SOP_VolumeFromAttribParms &src) const
    {
        return !operator==(src);
    }
    using Calculationtype = SOP_VolumeFromAttribEnums::Calculationtype;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myGroup, node, "group", time, 0);
        else myGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myPointgrp, node, "pointgrp", time, 0);
        else myPointgrp = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myUseattrib, node, "useattrib", time, 0);
        else myUseattrib = false;
        if (true && ( (!(((getUseattrib()==0)))) ) )
            OP_Utils::evalOpParm(myAttrib, node, "attrib", time, 0);
        else myAttrib = ""_sh;
        if (true && ( (!(((getUseattrib()==0)))) ) )
            OP_Utils::evalOpParm(myDisableonmissing, node, "disableonmissing", time, 0);
        else myDisableonmissing = false;
        if (true)
            OP_Utils::evalOpParm(myAccumulate, node, "accumulate", time, 0);
        else myAccumulate = false;
        if (true && ( (!(((getAccumulate()==1)))) ) )
            OP_Utils::evalOpParm(myExtrapolate, node, "extrapolate", time, 0);
        else myExtrapolate = false;
        if (true && ( (!(((getAccumulate()==1))||((getExtrapolate()==0)))) ) )
            OP_Utils::evalOpParm(myUsemaxextrapolate, node, "usemaxextrapolate", time, 0);
        else myUsemaxextrapolate = false;
        if (true && ( (!(((getAccumulate()==1))||((getExtrapolate()==0))||((getUsemaxextrapolate()==0)))) ) )
            OP_Utils::evalOpParm(myMaxextrapolate, node, "maxextrapolate", time, 0);
        else myMaxextrapolate = 2;
        if (true && ( (!(((getAccumulate()==1))||((getExtrapolate()==0)))) ) )
            OP_Utils::evalOpParm(myUsemaxextrapolatedist, node, "usemaxextrapolatedist", time, 0);
        else myUsemaxextrapolatedist = false;
        if (true && ( (!(((getAccumulate()==1))||((getExtrapolate()==0))||((getUsemaxextrapolatedist()==0)))) ) )
            OP_Utils::evalOpParm(myMaxextrapolatedist, node, "maxextrapolatedist", time, 0);
        else myMaxextrapolatedist = 0;
        if (true && ( (!(((getAccumulate()==1))||((getExtrapolate()==1)))) ) )
            OP_Utils::evalOpParm(myThreshold, node, "threshold", time, 0);
        else myThreshold = 1;
        if (true && ( (!(((getAccumulate()==1))||((getExtrapolate()==1)))) ) )
            OP_Utils::evalOpParm(myBandwidth, node, "bandwidth", time, 0);
        else myBandwidth = 0;
        if (true)
            OP_Utils::evalOpParm(myCalculationtype, node, "calculationtype", time, 0);
        else myCalculationtype = 0;
        if (true)
            OP_Utils::evalOpParm(myDstpreadd, node, "dstpreadd", time, 0);
        else myDstpreadd = 0;
        if (true)
            OP_Utils::evalOpParm(myDstpremul, node, "dstpremul", time, 0);
        else myDstpremul = 1;
        if (true)
            OP_Utils::evalOpParm(myScalebyvolume, node, "scalebyvolume", time, 0);
        else myScalebyvolume = false;
        if (true)
            OP_Utils::evalOpParm(mySrcpreadd, node, "srcpreadd", time, 0);
        else mySrcpreadd = 0;
        if (true)
            OP_Utils::evalOpParm(mySrcpremul, node, "srcpremul", time, 0);
        else mySrcpremul = 1;
        if (true)
            OP_Utils::evalOpParm(myPostadd, node, "postadd", time, 0);
        else myPostadd = 0;
        if (true)
            OP_Utils::evalOpParm(myPostmul, node, "postmul", time, 0);
        else myPostmul = 1;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_VolumeFromAttribParms *)src);
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
                coerceValue(value, myPointgrp);
                break;
            case 2:
                coerceValue(value, myUseattrib);
                break;
            case 3:
                coerceValue(value, myAttrib);
                break;
            case 4:
                coerceValue(value, myDisableonmissing);
                break;
            case 5:
                coerceValue(value, myAccumulate);
                break;
            case 6:
                coerceValue(value, myExtrapolate);
                break;
            case 7:
                coerceValue(value, myUsemaxextrapolate);
                break;
            case 8:
                coerceValue(value, myMaxextrapolate);
                break;
            case 9:
                coerceValue(value, myUsemaxextrapolatedist);
                break;
            case 10:
                coerceValue(value, myMaxextrapolatedist);
                break;
            case 11:
                coerceValue(value, myThreshold);
                break;
            case 12:
                coerceValue(value, myBandwidth);
                break;
            case 13:
                coerceValue(value, myCalculationtype);
                break;
            case 14:
                coerceValue(value, myDstpreadd);
                break;
            case 15:
                coerceValue(value, myDstpremul);
                break;
            case 16:
                coerceValue(value, myScalebyvolume);
                break;
            case 17:
                coerceValue(value, mySrcpreadd);
                break;
            case 18:
                coerceValue(value, mySrcpremul);
                break;
            case 19:
                coerceValue(value, myPostadd);
                break;
            case 20:
                coerceValue(value, myPostmul);
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
                coerceValue(myPointgrp, value);
                break;
            case 2:
                coerceValue(myUseattrib, value);
                break;
            case 3:
                coerceValue(myAttrib, value);
                break;
            case 4:
                coerceValue(myDisableonmissing, value);
                break;
            case 5:
                coerceValue(myAccumulate, value);
                break;
            case 6:
                coerceValue(myExtrapolate, value);
                break;
            case 7:
                coerceValue(myUsemaxextrapolate, value);
                break;
            case 8:
                coerceValue(myMaxextrapolate, value);
                break;
            case 9:
                coerceValue(myUsemaxextrapolatedist, value);
                break;
            case 10:
                coerceValue(myMaxextrapolatedist, value);
                break;
            case 11:
                coerceValue(myThreshold, value);
                break;
            case 12:
                coerceValue(myBandwidth, value);
                break;
            case 13:
                coerceValue(myCalculationtype, value);
                break;
            case 14:
                coerceValue(myDstpreadd, value);
                break;
            case 15:
                coerceValue(myDstpremul, value);
                break;
            case 16:
                coerceValue(myScalebyvolume, value);
                break;
            case 17:
                coerceValue(mySrcpreadd, value);
                break;
            case 18:
                coerceValue(mySrcpremul, value);
                break;
            case 19:
                coerceValue(myPostadd, value);
                break;
            case 20:
                coerceValue(myPostmul, value);
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
        return 21;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "group";
            case 1:
                return "pointgrp";
            case 2:
                return "useattrib";
            case 3:
                return "attrib";
            case 4:
                return "disableonmissing";
            case 5:
                return "accumulate";
            case 6:
                return "extrapolate";
            case 7:
                return "usemaxextrapolate";
            case 8:
                return "maxextrapolate";
            case 9:
                return "usemaxextrapolatedist";
            case 10:
                return "maxextrapolatedist";
            case 11:
                return "threshold";
            case 12:
                return "bandwidth";
            case 13:
                return "calculationtype";
            case 14:
                return "dstpreadd";
            case 15:
                return "dstpremul";
            case 16:
                return "scalebyvolume";
            case 17:
                return "srcpreadd";
            case 18:
                return "srcpremul";
            case 19:
                return "postadd";
            case 20:
                return "postmul";

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
                    return PARM_INTEGER;
                case 8:
                    return PARM_FLOAT;
                case 9:
                    return PARM_INTEGER;
                case 10:
                    return PARM_FLOAT;
                case 11:
                    return PARM_FLOAT;
                case 12:
                    return PARM_FLOAT;
                case 13:
                    return PARM_INTEGER;
                case 14:
                    return PARM_FLOAT;
                case 15:
                    return PARM_FLOAT;
                case 16:
                    return PARM_INTEGER;
                case 17:
                    return PARM_FLOAT;
                case 18:
                    return PARM_FLOAT;
                case 19:
                    return PARM_FLOAT;
                case 20:
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
        saveData(os, myGroup);
        saveData(os, myPointgrp);
        saveData(os, myUseattrib);
        saveData(os, myAttrib);
        saveData(os, myDisableonmissing);
        saveData(os, myAccumulate);
        saveData(os, myExtrapolate);
        saveData(os, myUsemaxextrapolate);
        saveData(os, myMaxextrapolate);
        saveData(os, myUsemaxextrapolatedist);
        saveData(os, myMaxextrapolatedist);
        saveData(os, myThreshold);
        saveData(os, myBandwidth);
        saveData(os, myCalculationtype);
        saveData(os, myDstpreadd);
        saveData(os, myDstpremul);
        saveData(os, myScalebyvolume);
        saveData(os, mySrcpreadd);
        saveData(os, mySrcpremul);
        saveData(os, myPostadd);
        saveData(os, myPostmul);

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
        loadData(is, myPointgrp);
        loadData(is, myUseattrib);
        loadData(is, myAttrib);
        loadData(is, myDisableonmissing);
        loadData(is, myAccumulate);
        loadData(is, myExtrapolate);
        loadData(is, myUsemaxextrapolate);
        loadData(is, myMaxextrapolate);
        loadData(is, myUsemaxextrapolatedist);
        loadData(is, myMaxextrapolatedist);
        loadData(is, myThreshold);
        loadData(is, myBandwidth);
        loadData(is, myCalculationtype);
        loadData(is, myDstpreadd);
        loadData(is, myDstpremul);
        loadData(is, myScalebyvolume);
        loadData(is, mySrcpreadd);
        loadData(is, mySrcpremul);
        loadData(is, myPostadd);
        loadData(is, myPostmul);

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
    const UT_StringHolder & getPointgrp() const { return myPointgrp; }
    void setPointgrp(const UT_StringHolder & val) { myPointgrp = val; }
    UT_StringHolder opPointgrp(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPointgrp();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "pointgrp", cookparms.getCookTime(), 0);
        return result;
    }
    bool getUseattrib() const { return myUseattrib; }
    void setUseattrib(bool val) { myUseattrib = val; }
    bool opUseattrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUseattrib();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "useattrib", cookparms.getCookTime(), 0);
        return result;
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
    bool getDisableonmissing() const { return myDisableonmissing; }
    void setDisableonmissing(bool val) { myDisableonmissing = val; }
    bool opDisableonmissing(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDisableonmissing();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "disableonmissing", cookparms.getCookTime(), 0);
        return result;
    }
    bool getAccumulate() const { return myAccumulate; }
    void setAccumulate(bool val) { myAccumulate = val; }
    bool opAccumulate(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAccumulate();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "accumulate", cookparms.getCookTime(), 0);
        return result;
    }
    bool getExtrapolate() const { return myExtrapolate; }
    void setExtrapolate(bool val) { myExtrapolate = val; }
    bool opExtrapolate(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getExtrapolate();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "extrapolate", cookparms.getCookTime(), 0);
        return result;
    }
    bool getUsemaxextrapolate() const { return myUsemaxextrapolate; }
    void setUsemaxextrapolate(bool val) { myUsemaxextrapolate = val; }
    bool opUsemaxextrapolate(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUsemaxextrapolate();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "usemaxextrapolate", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMaxextrapolate() const { return myMaxextrapolate; }
    void setMaxextrapolate(fpreal64 val) { myMaxextrapolate = val; }
    fpreal64 opMaxextrapolate(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaxextrapolate();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "maxextrapolate", cookparms.getCookTime(), 0);
        return result;
    }
    bool getUsemaxextrapolatedist() const { return myUsemaxextrapolatedist; }
    void setUsemaxextrapolatedist(bool val) { myUsemaxextrapolatedist = val; }
    bool opUsemaxextrapolatedist(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUsemaxextrapolatedist();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "usemaxextrapolatedist", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMaxextrapolatedist() const { return myMaxextrapolatedist; }
    void setMaxextrapolatedist(fpreal64 val) { myMaxextrapolatedist = val; }
    fpreal64 opMaxextrapolatedist(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaxextrapolatedist();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "maxextrapolatedist", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getThreshold() const { return myThreshold; }
    void setThreshold(fpreal64 val) { myThreshold = val; }
    fpreal64 opThreshold(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getThreshold();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "threshold", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getBandwidth() const { return myBandwidth; }
    void setBandwidth(fpreal64 val) { myBandwidth = val; }
    fpreal64 opBandwidth(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getBandwidth();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "bandwidth", cookparms.getCookTime(), 0);
        return result;
    }
    Calculationtype getCalculationtype() const { return Calculationtype(myCalculationtype); }
    void setCalculationtype(Calculationtype val) { myCalculationtype = int64(val); }
    Calculationtype opCalculationtype(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCalculationtype();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "calculationtype", cookparms.getCookTime(), 0);
        return Calculationtype(result);
    }
    fpreal64 getDstpreadd() const { return myDstpreadd; }
    void setDstpreadd(fpreal64 val) { myDstpreadd = val; }
    fpreal64 opDstpreadd(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDstpreadd();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "dstpreadd", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getDstpremul() const { return myDstpremul; }
    void setDstpremul(fpreal64 val) { myDstpremul = val; }
    fpreal64 opDstpremul(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDstpremul();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "dstpremul", cookparms.getCookTime(), 0);
        return result;
    }
    bool getScalebyvolume() const { return myScalebyvolume; }
    void setScalebyvolume(bool val) { myScalebyvolume = val; }
    bool opScalebyvolume(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getScalebyvolume();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "scalebyvolume", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getSrcpreadd() const { return mySrcpreadd; }
    void setSrcpreadd(fpreal64 val) { mySrcpreadd = val; }
    fpreal64 opSrcpreadd(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSrcpreadd();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "srcpreadd", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getSrcpremul() const { return mySrcpremul; }
    void setSrcpremul(fpreal64 val) { mySrcpremul = val; }
    fpreal64 opSrcpremul(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSrcpremul();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "srcpremul", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getPostadd() const { return myPostadd; }
    void setPostadd(fpreal64 val) { myPostadd = val; }
    fpreal64 opPostadd(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPostadd();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "postadd", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getPostmul() const { return myPostmul; }
    void setPostmul(fpreal64 val) { myPostmul = val; }
    fpreal64 opPostmul(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPostmul();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "postmul", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myGroup;
    UT_StringHolder myPointgrp;
    bool myUseattrib;
    UT_StringHolder myAttrib;
    bool myDisableonmissing;
    bool myAccumulate;
    bool myExtrapolate;
    bool myUsemaxextrapolate;
    fpreal64 myMaxextrapolate;
    bool myUsemaxextrapolatedist;
    fpreal64 myMaxextrapolatedist;
    fpreal64 myThreshold;
    fpreal64 myBandwidth;
    int64 myCalculationtype;
    fpreal64 myDstpreadd;
    fpreal64 myDstpremul;
    bool myScalebyvolume;
    fpreal64 mySrcpreadd;
    fpreal64 mySrcpremul;
    fpreal64 myPostadd;
    fpreal64 myPostmul;

};