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

class SOP_API SOP_VolumePatchParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_VolumePatchParms()
    {
        myBasegroup = ""_sh;
        myPatchgroup = ""_sh;
        myMaskgroup = ""_sh;
        myMaskcutoff = 0.5;
        myPatchislaplacian = false;
        myTolerance = 0.0001;

    }

    explicit SOP_VolumePatchParms(const SOP_VolumePatchParms &) = default;

    virtual ~SOP_VolumePatchParms() {}

    bool operator==(const SOP_VolumePatchParms &src) const
    {
        if (myBasegroup != src.myBasegroup) return false;
        if (myPatchgroup != src.myPatchgroup) return false;
        if (myMaskgroup != src.myMaskgroup) return false;
        if (myMaskcutoff != src.myMaskcutoff) return false;
        if (myPatchislaplacian != src.myPatchislaplacian) return false;
        if (myTolerance != src.myTolerance) return false;

        return true;
    }
    bool operator!=(const SOP_VolumePatchParms &src) const
    {
        return !operator==(src);
    }



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myBasegroup, node, "basegroup", time, 0);
        else myBasegroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myPatchgroup, node, "patchgroup", time, 0);
        else myPatchgroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myMaskgroup, node, "maskgroup", time, 0);
        else myMaskgroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myMaskcutoff, node, "maskcutoff", time, 0);
        else myMaskcutoff = 0.5;
        if (true)
            OP_Utils::evalOpParm(myPatchislaplacian, node, "patchislaplacian", time, 0);
        else myPatchislaplacian = false;
        if (true)
            OP_Utils::evalOpParm(myTolerance, node, "tolerance", time, 0);
        else myTolerance = 0.0001;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_VolumePatchParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myBasegroup);
                break;
            case 1:
                coerceValue(value, myPatchgroup);
                break;
            case 2:
                coerceValue(value, myMaskgroup);
                break;
            case 3:
                coerceValue(value, myMaskcutoff);
                break;
            case 4:
                coerceValue(value, myPatchislaplacian);
                break;
            case 5:
                coerceValue(value, myTolerance);
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
                coerceValue(myBasegroup, value);
                break;
            case 1:
                coerceValue(myPatchgroup, value);
                break;
            case 2:
                coerceValue(myMaskgroup, value);
                break;
            case 3:
                coerceValue(myMaskcutoff, value);
                break;
            case 4:
                coerceValue(myPatchislaplacian, value);
                break;
            case 5:
                coerceValue(myTolerance, value);
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
                return "basegroup";
            case 1:
                return "patchgroup";
            case 2:
                return "maskgroup";
            case 3:
                return "maskcutoff";
            case 4:
                return "patchislaplacian";
            case 5:
                return "tolerance";

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
                    return PARM_STRING;
                case 3:
                    return PARM_FLOAT;
                case 4:
                    return PARM_INTEGER;
                case 5:
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
        saveData(os, myBasegroup);
        saveData(os, myPatchgroup);
        saveData(os, myMaskgroup);
        saveData(os, myMaskcutoff);
        saveData(os, myPatchislaplacian);
        saveData(os, myTolerance);

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
        loadData(is, myBasegroup);
        loadData(is, myPatchgroup);
        loadData(is, myMaskgroup);
        loadData(is, myMaskcutoff);
        loadData(is, myPatchislaplacian);
        loadData(is, myTolerance);

        return true;
    }

    const UT_StringHolder & getBasegroup() const { return myBasegroup; }
    void setBasegroup(const UT_StringHolder & val) { myBasegroup = val; }
    UT_StringHolder opBasegroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getBasegroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "basegroup", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getPatchgroup() const { return myPatchgroup; }
    void setPatchgroup(const UT_StringHolder & val) { myPatchgroup = val; }
    UT_StringHolder opPatchgroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPatchgroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "patchgroup", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getMaskgroup() const { return myMaskgroup; }
    void setMaskgroup(const UT_StringHolder & val) { myMaskgroup = val; }
    UT_StringHolder opMaskgroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaskgroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "maskgroup", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMaskcutoff() const { return myMaskcutoff; }
    void setMaskcutoff(fpreal64 val) { myMaskcutoff = val; }
    fpreal64 opMaskcutoff(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMaskcutoff();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "maskcutoff", cookparms.getCookTime(), 0);
        return result;
    }
    bool getPatchislaplacian() const { return myPatchislaplacian; }
    void setPatchislaplacian(bool val) { myPatchislaplacian = val; }
    bool opPatchislaplacian(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPatchislaplacian();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "patchislaplacian", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getTolerance() const { return myTolerance; }
    void setTolerance(fpreal64 val) { myTolerance = val; }
    fpreal64 opTolerance(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTolerance();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "tolerance", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myBasegroup;
    UT_StringHolder myPatchgroup;
    UT_StringHolder myMaskgroup;
    fpreal64 myMaskcutoff;
    bool myPatchislaplacian;
    fpreal64 myTolerance;

};
