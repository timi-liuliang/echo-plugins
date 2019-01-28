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
namespace SOP_SBlend_2_0Enums
{
    enum class VoxelBlend
    {
        NONE = 0,
        BYGRIDINDEX,
        BYVOXELPOS,
        ADVECTED
    };
    enum class Interp
    {
        LINEAR = 0,
        CUBIC,
        SUBD
    };
}


class SOP_API SOP_SBlend_2_0Parms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_SBlend_2_0Parms()
    {
        myBlend = 0;
        myAttribs = "*"_sh;
        myPtIDAttr = "id"_sh;
        myPrimIDAttr = "name"_sh;
        myVoxelBlend = 0;
        myAdvectionDT = 0.0416667;
        myVelocityField = "vel"_sh;
        myDoSlerp = false;
        myInterp = 0;
        myUseVForPInterp = false;
        myTimeStep = 0;

    }

    explicit SOP_SBlend_2_0Parms(const SOP_SBlend_2_0Parms &) = default;

    virtual ~SOP_SBlend_2_0Parms() {}

    bool operator==(const SOP_SBlend_2_0Parms &src) const
    {
        if (myBlend != src.myBlend) return false;
        if (myAttribs != src.myAttribs) return false;
        if (myPtIDAttr != src.myPtIDAttr) return false;
        if (myPrimIDAttr != src.myPrimIDAttr) return false;
        if (myVoxelBlend != src.myVoxelBlend) return false;
        if (myAdvectionDT != src.myAdvectionDT) return false;
        if (myVelocityField != src.myVelocityField) return false;
        if (myDoSlerp != src.myDoSlerp) return false;
        if (myInterp != src.myInterp) return false;
        if (myUseVForPInterp != src.myUseVForPInterp) return false;
        if (myTimeStep != src.myTimeStep) return false;

        return true;
    }
    bool operator!=(const SOP_SBlend_2_0Parms &src) const
    {
        return !operator==(src);
    }
    using VoxelBlend = SOP_SBlend_2_0Enums::VoxelBlend;
    using Interp = SOP_SBlend_2_0Enums::Interp;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myBlend, node, "blend", time, 0);
        else myBlend = 0;
        if (true)
            OP_Utils::evalOpParm(myAttribs, node, "attribs", time, 0);
        else myAttribs = "*"_sh;
        if (true)
            OP_Utils::evalOpParm(myPtIDAttr, node, "ptidattr", time, 0);
        else myPtIDAttr = "id"_sh;
        if (true)
            OP_Utils::evalOpParm(myPrimIDAttr, node, "primidattr", time, 0);
        else myPrimIDAttr = "name"_sh;
        if (true)
            OP_Utils::evalOpParm(myVoxelBlend, node, "voxelblend", time, 0);
        else myVoxelBlend = 0;
        if (true && ( (!(((int64(getVoxelBlend())!=3)))) ) )
            OP_Utils::evalOpParm(myAdvectionDT, node, "adt", time, 0);
        else myAdvectionDT = 0.0416667;
        if (true && ( (!(((int64(getVoxelBlend())!=3)))) ) )
            OP_Utils::evalOpParm(myVelocityField, node, "velocity", time, 0);
        else myVelocityField = "vel"_sh;
        if (true)
            OP_Utils::evalOpParm(myDoSlerp, node, "doslerp", time, 0);
        else myDoSlerp = false;
        if (true)
            OP_Utils::evalOpParm(myInterp, node, "interp", time, 0);
        else myInterp = 0;
        if (true && ( (!(((int64(getInterp())!=0)))) ) )
            OP_Utils::evalOpParm(myUseVForPInterp, node, "usevforpinterp", time, 0);
        else myUseVForPInterp = false;
        if (true && ( (!(((getUseVForPInterp()==0)&&(int64(getVoxelBlend())!=3)))) ) )
            OP_Utils::evalOpParm(myTimeStep, node, "timestep", time, 0);
        else myTimeStep = 0;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_SBlend_2_0Parms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myBlend);
                break;
            case 1:
                coerceValue(value, myAttribs);
                break;
            case 2:
                coerceValue(value, myPtIDAttr);
                break;
            case 3:
                coerceValue(value, myPrimIDAttr);
                break;
            case 4:
                coerceValue(value, myVoxelBlend);
                break;
            case 5:
                coerceValue(value, myAdvectionDT);
                break;
            case 6:
                coerceValue(value, myVelocityField);
                break;
            case 7:
                coerceValue(value, myDoSlerp);
                break;
            case 8:
                coerceValue(value, myInterp);
                break;
            case 9:
                coerceValue(value, myUseVForPInterp);
                break;
            case 10:
                coerceValue(value, myTimeStep);
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
                coerceValue(myBlend, value);
                break;
            case 1:
                coerceValue(myAttribs, value);
                break;
            case 2:
                coerceValue(myPtIDAttr, value);
                break;
            case 3:
                coerceValue(myPrimIDAttr, value);
                break;
            case 4:
                coerceValue(myVoxelBlend, value);
                break;
            case 5:
                coerceValue(myAdvectionDT, value);
                break;
            case 6:
                coerceValue(myVelocityField, value);
                break;
            case 7:
                coerceValue(myDoSlerp, value);
                break;
            case 8:
                coerceValue(myInterp, value);
                break;
            case 9:
                coerceValue(myUseVForPInterp, value);
                break;
            case 10:
                coerceValue(myTimeStep, value);
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
        return 11;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "blend";
            case 1:
                return "attribs";
            case 2:
                return "ptidattr";
            case 3:
                return "primidattr";
            case 4:
                return "voxelblend";
            case 5:
                return "adt";
            case 6:
                return "velocity";
            case 7:
                return "doslerp";
            case 8:
                return "interp";
            case 9:
                return "usevforpinterp";
            case 10:
                return "timestep";

        }
        return 0;
    }

    virtual ParmType getParmType(exint fieldnum) const
    {
        switch (fieldnum)
        {
                case 0:
                    return PARM_FLOAT;
                case 1:
                    return PARM_STRING;
                case 2:
                    return PARM_STRING;
                case 3:
                    return PARM_STRING;
                case 4:
                    return PARM_INTEGER;
                case 5:
                    return PARM_FLOAT;
                case 6:
                    return PARM_STRING;
                case 7:
                    return PARM_INTEGER;
                case 8:
                    return PARM_INTEGER;
                case 9:
                    return PARM_INTEGER;
                case 10:
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
        saveData(os, myBlend);
        saveData(os, myAttribs);
        saveData(os, myPtIDAttr);
        saveData(os, myPrimIDAttr);
        saveData(os, myVoxelBlend);
        saveData(os, myAdvectionDT);
        saveData(os, myVelocityField);
        saveData(os, myDoSlerp);
        saveData(os, myInterp);
        saveData(os, myUseVForPInterp);
        saveData(os, myTimeStep);

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
        loadData(is, myBlend);
        loadData(is, myAttribs);
        loadData(is, myPtIDAttr);
        loadData(is, myPrimIDAttr);
        loadData(is, myVoxelBlend);
        loadData(is, myAdvectionDT);
        loadData(is, myVelocityField);
        loadData(is, myDoSlerp);
        loadData(is, myInterp);
        loadData(is, myUseVForPInterp);
        loadData(is, myTimeStep);

        return true;
    }

    fpreal64 getBlend() const { return myBlend; }
    void setBlend(fpreal64 val) { myBlend = val; }
    fpreal64 opBlend(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getBlend();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "blend", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getAttribs() const { return myAttribs; }
    void setAttribs(const UT_StringHolder & val) { myAttribs = val; }
    UT_StringHolder opAttribs(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAttribs();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "attribs", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getPtIDAttr() const { return myPtIDAttr; }
    void setPtIDAttr(const UT_StringHolder & val) { myPtIDAttr = val; }
    UT_StringHolder opPtIDAttr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPtIDAttr();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "ptidattr", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getPrimIDAttr() const { return myPrimIDAttr; }
    void setPrimIDAttr(const UT_StringHolder & val) { myPrimIDAttr = val; }
    UT_StringHolder opPrimIDAttr(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getPrimIDAttr();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "primidattr", cookparms.getCookTime(), 0);
        return result;
    }
    VoxelBlend getVoxelBlend() const { return VoxelBlend(myVoxelBlend); }
    void setVoxelBlend(VoxelBlend val) { myVoxelBlend = int64(val); }
    VoxelBlend opVoxelBlend(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVoxelBlend();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "voxelblend", cookparms.getCookTime(), 0);
        return VoxelBlend(result);
    }
    fpreal64 getAdvectionDT() const { return myAdvectionDT; }
    void setAdvectionDT(fpreal64 val) { myAdvectionDT = val; }
    fpreal64 opAdvectionDT(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAdvectionDT();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "adt", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getVelocityField() const { return myVelocityField; }
    void setVelocityField(const UT_StringHolder & val) { myVelocityField = val; }
    UT_StringHolder opVelocityField(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVelocityField();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "velocity", cookparms.getCookTime(), 0);
        return result;
    }
    bool getDoSlerp() const { return myDoSlerp; }
    void setDoSlerp(bool val) { myDoSlerp = val; }
    bool opDoSlerp(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDoSlerp();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "doslerp", cookparms.getCookTime(), 0);
        return result;
    }
    Interp getInterp() const { return Interp(myInterp); }
    void setInterp(Interp val) { myInterp = int64(val); }
    Interp opInterp(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getInterp();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "interp", cookparms.getCookTime(), 0);
        return Interp(result);
    }
    bool getUseVForPInterp() const { return myUseVForPInterp; }
    void setUseVForPInterp(bool val) { myUseVForPInterp = val; }
    bool opUseVForPInterp(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUseVForPInterp();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "usevforpinterp", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getTimeStep() const { return myTimeStep; }
    void setTimeStep(fpreal64 val) { myTimeStep = val; }
    fpreal64 opTimeStep(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTimeStep();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "timestep", cookparms.getCookTime(), 0);
        return result;
    }

private:
    fpreal64 myBlend;
    UT_StringHolder myAttribs;
    UT_StringHolder myPtIDAttr;
    UT_StringHolder myPrimIDAttr;
    int64 myVoxelBlend;
    fpreal64 myAdvectionDT;
    UT_StringHolder myVelocityField;
    bool myDoSlerp;
    int64 myInterp;
    bool myUseVForPInterp;
    fpreal64 myTimeStep;

};