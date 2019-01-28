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
namespace SOP_BoneCaptureLinesEnums
{
    enum class Resample
    {
        OFF = 0,
        MAXAXIS,
        MAXLENGTH
    };
    enum class Cookat
    {
        CAPTUREFRAME = 0,
        EVERYFRAME
    };
    enum class Captureregionsop
    {
        DISPLAY = 0,
        RENDER,
        CAPTURE
    };
}


class SOP_API SOP_BoneCaptureLinesParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_BoneCaptureLinesParms()
    {
        myRootpath = ""_sh;
        myExtraregions = ""_sh;
        myResample = 1;
        myMaxaxisfraction = 0.01;
        myMaxlength = 0.1;
        myExcludeshortbones = false;
        myExcludethreshold = 0.001;
        myFusethreshold = 0.001;
        myUsebonelink = false;
        myUsecaptpose = false;
        myCookat = 0;
        myCaptframe = 0;
        myCaptureregionsop = 2;
        myDosubnets = false;
        myRelativeskel = false;

    }

    explicit SOP_BoneCaptureLinesParms(const SOP_BoneCaptureLinesParms &) = default;

    virtual ~SOP_BoneCaptureLinesParms() {}

    bool operator==(const SOP_BoneCaptureLinesParms &src) const
    {
        if (myRootpath != src.myRootpath) return false;
        if (myExtraregions != src.myExtraregions) return false;
        if (myResample != src.myResample) return false;
        if (myMaxaxisfraction != src.myMaxaxisfraction) return false;
        if (myMaxlength != src.myMaxlength) return false;
        if (myExcludeshortbones != src.myExcludeshortbones) return false;
        if (myExcludethreshold != src.myExcludethreshold) return false;
        if (myFusethreshold != src.myFusethreshold) return false;
        if (myUsebonelink != src.myUsebonelink) return false;
        if (myUsecaptpose != src.myUsecaptpose) return false;
        if (myCookat != src.myCookat) return false;
        if (myCaptframe != src.myCaptframe) return false;
        if (myCaptureregionsop != src.myCaptureregionsop) return false;
        if (myDosubnets != src.myDosubnets) return false;
        if (myRelativeskel != src.myRelativeskel) return false;

        return true;
    }
    bool operator!=(const SOP_BoneCaptureLinesParms &src) const
    {
        return !operator==(src);
    }
    using Resample = SOP_BoneCaptureLinesEnums::Resample;
    using Cookat = SOP_BoneCaptureLinesEnums::Cookat;
    using Captureregionsop = SOP_BoneCaptureLinesEnums::Captureregionsop;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myRootpath, node, "rootpath", time, 0);
        else myRootpath = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myExtraregions, node, "extraregions", time, 0);
        else myExtraregions = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myResample, node, "resample", time, 0);
        else myResample = 1;
        if (true && ( (!(((int64(getResample())!=1))||((int64(getResample())!=1)))) ) )
            OP_Utils::evalOpParm(myMaxaxisfraction, node, "maxaxisfraction", time, 0);
        else myMaxaxisfraction = 0.01;
        if (true && ( (!(((int64(getResample())!=2))||((int64(getResample())!=2)))) ) )
            OP_Utils::evalOpParm(myMaxlength, node, "maxlength", time, 0);
        else myMaxlength = 0.1;
        if (true)
            OP_Utils::evalOpParm(myExcludeshortbones, node, "excludeshortbones", time, 0);
        else myExcludeshortbones = false;
        if (true && ( (!(((getExcludeshortbones()==0)))) ) )
            OP_Utils::evalOpParm(myExcludethreshold, node, "excludethreshold", time, 0);
        else myExcludethreshold = 0.001;
        if (true)
            OP_Utils::evalOpParm(myFusethreshold, node, "fusethreshold", time, 0);
        else myFusethreshold = 0.001;
        if (true)
            OP_Utils::evalOpParm(myUsebonelink, node, "usebonelink", time, 0);
        else myUsebonelink = false;
        if (true)
            OP_Utils::evalOpParm(myUsecaptpose, node, "usecaptpose", time, 0);
        else myUsecaptpose = false;
        if (true && ( (!(((getUsecaptpose()!=0)))) ) )
            OP_Utils::evalOpParm(myCookat, node, "cookat", time, 0);
        else myCookat = 0;
        if (true && ( (!(((getUsecaptpose()!=0))||((int64(getCookat())==1)))) ) )
            OP_Utils::evalOpParm(myCaptframe, node, "captframe", time, 0);
        else myCaptframe = 0;
        if (true)
            OP_Utils::evalOpParm(myCaptureregionsop, node, "captureregionsop", time, 0);
        else myCaptureregionsop = 2;
        if (true)
            OP_Utils::evalOpParm(myDosubnets, node, "dosubnets", time, 0);
        else myDosubnets = false;
        if (true)
            OP_Utils::evalOpParm(myRelativeskel, node, "relativeskel", time, 0);
        else myRelativeskel = false;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_BoneCaptureLinesParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myRootpath);
                break;
            case 1:
                coerceValue(value, myExtraregions);
                break;
            case 2:
                coerceValue(value, myResample);
                break;
            case 3:
                coerceValue(value, myMaxaxisfraction);
                break;
            case 4:
                coerceValue(value, myMaxlength);
                break;
            case 5:
                coerceValue(value, myExcludeshortbones);
                break;
            case 6:
                coerceValue(value, myExcludethreshold);
                break;
            case 7:
                coerceValue(value, myFusethreshold);
                break;
            case 8:
                coerceValue(value, myUsebonelink);
                break;
            case 9:
                coerceValue(value, myUsecaptpose);
                break;
            case 10:
                coerceValue(value, myCookat);
                break;
            case 11:
                coerceValue(value, myCaptframe);
                break;
            case 12:
                coerceValue(value, myCaptureregionsop);
                break;
            case 13:
                coerceValue(value, myDosubnets);
                break;
            case 14:
                coerceValue(value, myRelativeskel);
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
                coerceValue(myRootpath, value);
                break;
            case 1:
                coerceValue(myExtraregions, value);
                break;
            case 2:
                coerceValue(myResample, value);
                break;
            case 3:
                coerceValue(myMaxaxisfraction, value);
                break;
            case 4:
                coerceValue(myMaxlength, value);
                break;
            case 5:
                coerceValue(myExcludeshortbones, value);
                break;
            case 6:
                coerceValue(myExcludethreshold, value);
                break;
            case 7:
                coerceValue(myFusethreshold, value);
                break;
            case 8:
                coerceValue(myUsebonelink, value);
                break;
            case 9:
                coerceValue(myUsecaptpose, value);
                break;
            case 10:
                coerceValue(myCookat, value);
                break;
            case 11:
                coerceValue(myCaptframe, value);
                break;
            case 12:
                coerceValue(myCaptureregionsop, value);
                break;
            case 13:
                coerceValue(myDosubnets, value);
                break;
            case 14:
                coerceValue(myRelativeskel, value);
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
        return 15;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "rootpath";
            case 1:
                return "extraregions";
            case 2:
                return "resample";
            case 3:
                return "maxaxisfraction";
            case 4:
                return "maxlength";
            case 5:
                return "excludeshortbones";
            case 6:
                return "excludethreshold";
            case 7:
                return "fusethreshold";
            case 8:
                return "usebonelink";
            case 9:
                return "usecaptpose";
            case 10:
                return "cookat";
            case 11:
                return "captframe";
            case 12:
                return "captureregionsop";
            case 13:
                return "dosubnets";
            case 14:
                return "relativeskel";

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
                    return PARM_FLOAT;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_FLOAT;
                case 7:
                    return PARM_FLOAT;
                case 8:
                    return PARM_INTEGER;
                case 9:
                    return PARM_INTEGER;
                case 10:
                    return PARM_INTEGER;
                case 11:
                    return PARM_FLOAT;
                case 12:
                    return PARM_INTEGER;
                case 13:
                    return PARM_INTEGER;
                case 14:
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
        saveData(os, myRootpath);
        saveData(os, myExtraregions);
        saveData(os, myResample);
        saveData(os, myMaxaxisfraction);
        saveData(os, myMaxlength);
        saveData(os, myExcludeshortbones);
        saveData(os, myExcludethreshold);
        saveData(os, myFusethreshold);
        saveData(os, myUsebonelink);
        saveData(os, myUsecaptpose);
        saveData(os, myCookat);
        saveData(os, myCaptframe);
        saveData(os, myCaptureregionsop);
        saveData(os, myDosubnets);
        saveData(os, myRelativeskel);

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
        loadData(is, myRootpath);
        loadData(is, myExtraregions);
        loadData(is, myResample);
        loadData(is, myMaxaxisfraction);
        loadData(is, myMaxlength);
        loadData(is, myExcludeshortbones);
        loadData(is, myExcludethreshold);
        loadData(is, myFusethreshold);
        loadData(is, myUsebonelink);
        loadData(is, myUsecaptpose);
        loadData(is, myCookat);
        loadData(is, myCaptframe);
        loadData(is, myCaptureregionsop);
        loadData(is, myDosubnets);
        loadData(is, myRelativeskel);

        return true;
    }

    const UT_StringHolder & getRootpath() const { return myRootpath; }
    void setRootpath(const UT_StringHolder & val) { myRootpath = val; }
    UT_StringHolder opRootpath(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRootpath();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "rootpath", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getExtraregions() const { return myExtraregions; }
    void setExtraregions(const UT_StringHolder & val) { myExtraregions = val; }
    UT_StringHolder opExtraregions(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getExtraregions();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "extraregions", cookparms.getCookTime(), 0);
        return result;
    }
    Resample getResample() const { return Resample(myResample); }
    void setResample(Resample val) { myResample = int64(val); }
    Resample opResample(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getResample();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "resample", cookparms.getCookTime(), 0);
        return Resample(result);
    }
    fpreal64 getMaxaxisfraction() const { return myMaxaxisfraction; }
    void setMaxaxisfraction(fpreal64 val) { myMaxaxisfraction = val; }
    fpreal64 opMaxaxisfraction(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaxaxisfraction();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "maxaxisfraction", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMaxlength() const { return myMaxlength; }
    void setMaxlength(fpreal64 val) { myMaxlength = val; }
    fpreal64 opMaxlength(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaxlength();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "maxlength", cookparms.getCookTime(), 0);
        return result;
    }
    bool getExcludeshortbones() const { return myExcludeshortbones; }
    void setExcludeshortbones(bool val) { myExcludeshortbones = val; }
    bool opExcludeshortbones(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getExcludeshortbones();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "excludeshortbones", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getExcludethreshold() const { return myExcludethreshold; }
    void setExcludethreshold(fpreal64 val) { myExcludethreshold = val; }
    fpreal64 opExcludethreshold(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getExcludethreshold();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "excludethreshold", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getFusethreshold() const { return myFusethreshold; }
    void setFusethreshold(fpreal64 val) { myFusethreshold = val; }
    fpreal64 opFusethreshold(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getFusethreshold();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "fusethreshold", cookparms.getCookTime(), 0);
        return result;
    }
    bool getUsebonelink() const { return myUsebonelink; }
    void setUsebonelink(bool val) { myUsebonelink = val; }
    bool opUsebonelink(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUsebonelink();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "usebonelink", cookparms.getCookTime(), 0);
        return result;
    }
    bool getUsecaptpose() const { return myUsecaptpose; }
    void setUsecaptpose(bool val) { myUsecaptpose = val; }
    bool opUsecaptpose(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUsecaptpose();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "usecaptpose", cookparms.getCookTime(), 0);
        return result;
    }
    Cookat getCookat() const { return Cookat(myCookat); }
    void setCookat(Cookat val) { myCookat = int64(val); }
    Cookat opCookat(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCookat();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "cookat", cookparms.getCookTime(), 0);
        return Cookat(result);
    }
    fpreal64 getCaptframe() const { return myCaptframe; }
    void setCaptframe(fpreal64 val) { myCaptframe = val; }
    fpreal64 opCaptframe(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCaptframe();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "captframe", cookparms.getCookTime(), 0);
        return result;
    }
    Captureregionsop getCaptureregionsop() const { return Captureregionsop(myCaptureregionsop); }
    void setCaptureregionsop(Captureregionsop val) { myCaptureregionsop = int64(val); }
    Captureregionsop opCaptureregionsop(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCaptureregionsop();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "captureregionsop", cookparms.getCookTime(), 0);
        return Captureregionsop(result);
    }
    bool getDosubnets() const { return myDosubnets; }
    void setDosubnets(bool val) { myDosubnets = val; }
    bool opDosubnets(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDosubnets();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "dosubnets", cookparms.getCookTime(), 0);
        return result;
    }
    bool getRelativeskel() const { return myRelativeskel; }
    void setRelativeskel(bool val) { myRelativeskel = val; }
    bool opRelativeskel(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRelativeskel();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "relativeskel", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myRootpath;
    UT_StringHolder myExtraregions;
    int64 myResample;
    fpreal64 myMaxaxisfraction;
    fpreal64 myMaxlength;
    bool myExcludeshortbones;
    fpreal64 myExcludethreshold;
    fpreal64 myFusethreshold;
    bool myUsebonelink;
    bool myUsecaptpose;
    int64 myCookat;
    fpreal64 myCaptframe;
    int64 myCaptureregionsop;
    bool myDosubnets;
    bool myRelativeskel;

};
