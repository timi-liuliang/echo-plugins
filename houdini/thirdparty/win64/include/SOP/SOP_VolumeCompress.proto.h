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
namespace SOP_VolumeCompressEnums
{
    enum class Compression
    {
        NONE = 0,
        COMPRESS,
        UNCOMPRESS,
        UNCOMPRESSCONSTANT
    };
    enum class Dither
    {
        NONE = 0,
        ORDERED
    };
}


class SOP_API SOP_VolumeCompressParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_VolumeCompressParms()
    {
        myGroup = ""_sh;
        myCompression = 1;
        myUpdateSettings = false;
        myConstantTol = 0;
        myQuantizeTol = 0;
        myDither = 1;
        myUseFP16 = false;
        myMaskGroup = ""_sh;
        myDoMaskMin = false;
        myMaskMin = 0;
        myDoMaskMax = false;
        myMaskMax = 0;
        myInvertMask = false;

    }

    explicit SOP_VolumeCompressParms(const SOP_VolumeCompressParms &) = default;

    virtual ~SOP_VolumeCompressParms() {}

    bool operator==(const SOP_VolumeCompressParms &src) const
    {
        if (myGroup != src.myGroup) return false;
        if (myCompression != src.myCompression) return false;
        if (myUpdateSettings != src.myUpdateSettings) return false;
        if (myConstantTol != src.myConstantTol) return false;
        if (myQuantizeTol != src.myQuantizeTol) return false;
        if (myDither != src.myDither) return false;
        if (myUseFP16 != src.myUseFP16) return false;
        if (myMaskGroup != src.myMaskGroup) return false;
        if (myDoMaskMin != src.myDoMaskMin) return false;
        if (myMaskMin != src.myMaskMin) return false;
        if (myDoMaskMax != src.myDoMaskMax) return false;
        if (myMaskMax != src.myMaskMax) return false;
        if (myInvertMask != src.myInvertMask) return false;

        return true;
    }
    bool operator!=(const SOP_VolumeCompressParms &src) const
    {
        return !operator==(src);
    }
    using Compression = SOP_VolumeCompressEnums::Compression;
    using Dither = SOP_VolumeCompressEnums::Dither;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myGroup, node, "group", time, 0);
        else myGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myCompression, node, "compression", time, 0);
        else myCompression = 1;
        if (true)
            OP_Utils::evalOpParm(myUpdateSettings, node, "updatesettings", time, 0);
        else myUpdateSettings = false;
        if (true && ( (!(((getUpdateSettings()==0)))) ) )
            OP_Utils::evalOpParm(myConstantTol, node, "constanttol", time, 0);
        else myConstantTol = 0;
        if (true && ( (!(((getUpdateSettings()==0)))) ) )
            OP_Utils::evalOpParm(myQuantizeTol, node, "quantizetol", time, 0);
        else myQuantizeTol = 0;
        if (true && ( (!(((getUpdateSettings()==0)))) ) )
            OP_Utils::evalOpParm(myDither, node, "dither", time, 0);
        else myDither = 1;
        if (true && ( (!(((getUpdateSettings()==0)))) ) )
            OP_Utils::evalOpParm(myUseFP16, node, "usefp16", time, 0);
        else myUseFP16 = false;
        if (true)
            OP_Utils::evalOpParm(myMaskGroup, node, "maskgrp", time, 0);
        else myMaskGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myDoMaskMin, node, "domaskmin", time, 0);
        else myDoMaskMin = false;
        if (true && ( (!(((getDoMaskMin()==0)))) ) )
            OP_Utils::evalOpParm(myMaskMin, node, "maskmin", time, 0);
        else myMaskMin = 0;
        if (true)
            OP_Utils::evalOpParm(myDoMaskMax, node, "domaskmax", time, 0);
        else myDoMaskMax = false;
        if (true && ( (!(((getDoMaskMax()==0)))) ) )
            OP_Utils::evalOpParm(myMaskMax, node, "maskmax", time, 0);
        else myMaskMax = 0;
        if (true)
            OP_Utils::evalOpParm(myInvertMask, node, "invertmask", time, 0);
        else myInvertMask = false;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_VolumeCompressParms *)src);
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
                coerceValue(value, myCompression);
                break;
            case 2:
                coerceValue(value, myUpdateSettings);
                break;
            case 3:
                coerceValue(value, myConstantTol);
                break;
            case 4:
                coerceValue(value, myQuantizeTol);
                break;
            case 5:
                coerceValue(value, myDither);
                break;
            case 6:
                coerceValue(value, myUseFP16);
                break;
            case 7:
                coerceValue(value, myMaskGroup);
                break;
            case 8:
                coerceValue(value, myDoMaskMin);
                break;
            case 9:
                coerceValue(value, myMaskMin);
                break;
            case 10:
                coerceValue(value, myDoMaskMax);
                break;
            case 11:
                coerceValue(value, myMaskMax);
                break;
            case 12:
                coerceValue(value, myInvertMask);
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
                coerceValue(myCompression, value);
                break;
            case 2:
                coerceValue(myUpdateSettings, value);
                break;
            case 3:
                coerceValue(myConstantTol, value);
                break;
            case 4:
                coerceValue(myQuantizeTol, value);
                break;
            case 5:
                coerceValue(myDither, value);
                break;
            case 6:
                coerceValue(myUseFP16, value);
                break;
            case 7:
                coerceValue(myMaskGroup, value);
                break;
            case 8:
                coerceValue(myDoMaskMin, value);
                break;
            case 9:
                coerceValue(myMaskMin, value);
                break;
            case 10:
                coerceValue(myDoMaskMax, value);
                break;
            case 11:
                coerceValue(myMaskMax, value);
                break;
            case 12:
                coerceValue(myInvertMask, value);
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
        return 13;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "group";
            case 1:
                return "compression";
            case 2:
                return "updatesettings";
            case 3:
                return "constanttol";
            case 4:
                return "quantizetol";
            case 5:
                return "dither";
            case 6:
                return "usefp16";
            case 7:
                return "maskgrp";
            case 8:
                return "domaskmin";
            case 9:
                return "maskmin";
            case 10:
                return "domaskmax";
            case 11:
                return "maskmax";
            case 12:
                return "invertmask";

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
                    return PARM_FLOAT;
                case 4:
                    return PARM_FLOAT;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_INTEGER;
                case 7:
                    return PARM_STRING;
                case 8:
                    return PARM_INTEGER;
                case 9:
                    return PARM_FLOAT;
                case 10:
                    return PARM_INTEGER;
                case 11:
                    return PARM_FLOAT;
                case 12:
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
        saveData(os, myGroup);
        saveData(os, myCompression);
        saveData(os, myUpdateSettings);
        saveData(os, myConstantTol);
        saveData(os, myQuantizeTol);
        saveData(os, myDither);
        saveData(os, myUseFP16);
        saveData(os, myMaskGroup);
        saveData(os, myDoMaskMin);
        saveData(os, myMaskMin);
        saveData(os, myDoMaskMax);
        saveData(os, myMaskMax);
        saveData(os, myInvertMask);

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
        loadData(is, myCompression);
        loadData(is, myUpdateSettings);
        loadData(is, myConstantTol);
        loadData(is, myQuantizeTol);
        loadData(is, myDither);
        loadData(is, myUseFP16);
        loadData(is, myMaskGroup);
        loadData(is, myDoMaskMin);
        loadData(is, myMaskMin);
        loadData(is, myDoMaskMax);
        loadData(is, myMaskMax);
        loadData(is, myInvertMask);

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
    Compression getCompression() const { return Compression(myCompression); }
    void setCompression(Compression val) { myCompression = int64(val); }
    Compression opCompression(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCompression();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "compression", cookparms.getCookTime(), 0);
        return Compression(result);
    }
    bool getUpdateSettings() const { return myUpdateSettings; }
    void setUpdateSettings(bool val) { myUpdateSettings = val; }
    bool opUpdateSettings(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUpdateSettings();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "updatesettings", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getConstantTol() const { return myConstantTol; }
    void setConstantTol(fpreal64 val) { myConstantTol = val; }
    fpreal64 opConstantTol(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getConstantTol();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "constanttol", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getQuantizeTol() const { return myQuantizeTol; }
    void setQuantizeTol(fpreal64 val) { myQuantizeTol = val; }
    fpreal64 opQuantizeTol(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getQuantizeTol();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "quantizetol", cookparms.getCookTime(), 0);
        return result;
    }
    Dither getDither() const { return Dither(myDither); }
    void setDither(Dither val) { myDither = int64(val); }
    Dither opDither(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDither();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "dither", cookparms.getCookTime(), 0);
        return Dither(result);
    }
    bool getUseFP16() const { return myUseFP16; }
    void setUseFP16(bool val) { myUseFP16 = val; }
    bool opUseFP16(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUseFP16();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "usefp16", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getMaskGroup() const { return myMaskGroup; }
    void setMaskGroup(const UT_StringHolder & val) { myMaskGroup = val; }
    UT_StringHolder opMaskGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaskGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "maskgrp", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDoMaskMin() const { return myDoMaskMin; }
    void setDoMaskMin(bool val) { myDoMaskMin = val; }
    bool opDoMaskMin(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDoMaskMin();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "domaskmin", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMaskMin() const { return myMaskMin; }
    void setMaskMin(fpreal64 val) { myMaskMin = val; }
    fpreal64 opMaskMin(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaskMin();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "maskmin", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDoMaskMax() const { return myDoMaskMax; }
    void setDoMaskMax(bool val) { myDoMaskMax = val; }
    bool opDoMaskMax(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDoMaskMax();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "domaskmax", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMaskMax() const { return myMaskMax; }
    void setMaskMax(fpreal64 val) { myMaskMax = val; }
    fpreal64 opMaskMax(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaskMax();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "maskmax", cookparms.getCookTime(), 0);
        return result;
    }
    bool getInvertMask() const { return myInvertMask; }
    void setInvertMask(bool val) { myInvertMask = val; }
    bool opInvertMask(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getInvertMask();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "invertmask", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myGroup;
    int64 myCompression;
    bool myUpdateSettings;
    fpreal64 myConstantTol;
    fpreal64 myQuantizeTol;
    int64 myDither;
    bool myUseFP16;
    UT_StringHolder myMaskGroup;
    bool myDoMaskMin;
    fpreal64 myMaskMin;
    bool myDoMaskMax;
    fpreal64 myMaskMax;
    bool myInvertMask;

};
