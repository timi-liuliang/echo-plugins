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
namespace SOP_PolyFrameEnums
{
    enum class Entity
    {
        PRIMITIVE = 0,
        POINT
    };
}


class SOP_API SOP_PolyFrameParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_PolyFrameParms()
    {
        myGroup = ""_sh;
        myEntity = 0;
        myStyle = "edge2"_sh;
        myAttribname = ""_sh;
        myNon = false;
        myN = "N"_sh;
        myTangentuon = false;
        myTangentu = "tangentu"_sh;
        myTangentvon = false;
        myTangentv = "tangentv"_sh;
        myOrtho = false;
        myLeftHanded = false;

    }

    explicit SOP_PolyFrameParms(const SOP_PolyFrameParms &) = default;

    virtual ~SOP_PolyFrameParms() {}

    bool operator==(const SOP_PolyFrameParms &src) const
    {
        if (myGroup != src.myGroup) return false;
        if (myEntity != src.myEntity) return false;
        if (myStyle != src.myStyle) return false;
        if (myAttribname != src.myAttribname) return false;
        if (myNon != src.myNon) return false;
        if (myN != src.myN) return false;
        if (myTangentuon != src.myTangentuon) return false;
        if (myTangentu != src.myTangentu) return false;
        if (myTangentvon != src.myTangentvon) return false;
        if (myTangentv != src.myTangentv) return false;
        if (myOrtho != src.myOrtho) return false;
        if (myLeftHanded != src.myLeftHanded) return false;

        return true;
    }
    bool operator!=(const SOP_PolyFrameParms &src) const
    {
        return !operator==(src);
    }
    using Entity = SOP_PolyFrameEnums::Entity;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myGroup, node, "group", time, 0);
        else myGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myEntity, node, "entity", time, 0);
        else myEntity = 0;
        if (true)
            OP_Utils::evalOpParm(myStyle, node, "style", time, 0);
        else myStyle = "edge2"_sh;
        if (true && ( (!(((getStyle()!="attrib")&&(getStyle()!="texuv")))) ) )
            OP_Utils::evalOpParm(myAttribname, node, "attribname", time, 0);
        else myAttribname = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myNon, node, "Non", time, 0);
        else myNon = false;
        if (true && ( (!(((getNon()==0)))) ) )
            OP_Utils::evalOpParm(myN, node, "N", time, 0);
        else myN = "N"_sh;
        if (true)
            OP_Utils::evalOpParm(myTangentuon, node, "tangentuon", time, 0);
        else myTangentuon = false;
        if (true && ( (!(((getTangentuon()==0)))) ) )
            OP_Utils::evalOpParm(myTangentu, node, "tangentu", time, 0);
        else myTangentu = "tangentu"_sh;
        if (true)
            OP_Utils::evalOpParm(myTangentvon, node, "tangentvon", time, 0);
        else myTangentvon = false;
        if (true && ( (!(((getTangentvon()==0)))) ) )
            OP_Utils::evalOpParm(myTangentv, node, "tangentv", time, 0);
        else myTangentv = "tangentv"_sh;
        if (true)
            OP_Utils::evalOpParm(myOrtho, node, "ortho", time, 0);
        else myOrtho = false;
        if (true && ( (!(((getOrtho()==0)))) ) )
            OP_Utils::evalOpParm(myLeftHanded, node, "lefthanded", time, 0);
        else myLeftHanded = false;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_PolyFrameParms *)src);
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
                coerceValue(value, myEntity);
                break;
            case 2:
                coerceValue(value, myStyle);
                break;
            case 3:
                coerceValue(value, myAttribname);
                break;
            case 4:
                coerceValue(value, myNon);
                break;
            case 5:
                coerceValue(value, myN);
                break;
            case 6:
                coerceValue(value, myTangentuon);
                break;
            case 7:
                coerceValue(value, myTangentu);
                break;
            case 8:
                coerceValue(value, myTangentvon);
                break;
            case 9:
                coerceValue(value, myTangentv);
                break;
            case 10:
                coerceValue(value, myOrtho);
                break;
            case 11:
                coerceValue(value, myLeftHanded);
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
                coerceValue(myEntity, value);
                break;
            case 2:
                coerceValue(myStyle, value);
                break;
            case 3:
                coerceValue(myAttribname, value);
                break;
            case 4:
                coerceValue(myNon, value);
                break;
            case 5:
                coerceValue(myN, value);
                break;
            case 6:
                coerceValue(myTangentuon, value);
                break;
            case 7:
                coerceValue(myTangentu, value);
                break;
            case 8:
                coerceValue(myTangentvon, value);
                break;
            case 9:
                coerceValue(myTangentv, value);
                break;
            case 10:
                coerceValue(myOrtho, value);
                break;
            case 11:
                coerceValue(myLeftHanded, value);
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
        return 12;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "group";
            case 1:
                return "entity";
            case 2:
                return "style";
            case 3:
                return "attribname";
            case 4:
                return "Non";
            case 5:
                return "N";
            case 6:
                return "tangentuon";
            case 7:
                return "tangentu";
            case 8:
                return "tangentvon";
            case 9:
                return "tangentv";
            case 10:
                return "ortho";
            case 11:
                return "lefthanded";

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
                    return PARM_STRING;
                case 6:
                    return PARM_INTEGER;
                case 7:
                    return PARM_STRING;
                case 8:
                    return PARM_INTEGER;
                case 9:
                    return PARM_STRING;
                case 10:
                    return PARM_INTEGER;
                case 11:
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
        saveData(os, myEntity);
        saveData(os, myStyle);
        saveData(os, myAttribname);
        saveData(os, myNon);
        saveData(os, myN);
        saveData(os, myTangentuon);
        saveData(os, myTangentu);
        saveData(os, myTangentvon);
        saveData(os, myTangentv);
        saveData(os, myOrtho);
        saveData(os, myLeftHanded);

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
        loadData(is, myEntity);
        loadData(is, myStyle);
        loadData(is, myAttribname);
        loadData(is, myNon);
        loadData(is, myN);
        loadData(is, myTangentuon);
        loadData(is, myTangentu);
        loadData(is, myTangentvon);
        loadData(is, myTangentv);
        loadData(is, myOrtho);
        loadData(is, myLeftHanded);

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
    Entity getEntity() const { return Entity(myEntity); }
    void setEntity(Entity val) { myEntity = int64(val); }
    Entity opEntity(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getEntity();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "entity", cookparms.getCookTime(), 0);
        return Entity(result);
    }
    const UT_StringHolder & getStyle() const { return myStyle; }
    void setStyle(const UT_StringHolder & val) { myStyle = val; }
    UT_StringHolder opStyle(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getStyle();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "style", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getAttribname() const { return myAttribname; }
    void setAttribname(const UT_StringHolder & val) { myAttribname = val; }
    UT_StringHolder opAttribname(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAttribname();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "attribname", cookparms.getCookTime(), 0);
        return result;
    }
    bool getNon() const { return myNon; }
    void setNon(bool val) { myNon = val; }
    bool opNon(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getNon();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "Non", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getN() const { return myN; }
    void setN(const UT_StringHolder & val) { myN = val; }
    UT_StringHolder opN(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getN();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "N", cookparms.getCookTime(), 0);
        return result;
    }
    bool getTangentuon() const { return myTangentuon; }
    void setTangentuon(bool val) { myTangentuon = val; }
    bool opTangentuon(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTangentuon();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "tangentuon", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getTangentu() const { return myTangentu; }
    void setTangentu(const UT_StringHolder & val) { myTangentu = val; }
    UT_StringHolder opTangentu(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTangentu();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "tangentu", cookparms.getCookTime(), 0);
        return result;
    }
    bool getTangentvon() const { return myTangentvon; }
    void setTangentvon(bool val) { myTangentvon = val; }
    bool opTangentvon(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTangentvon();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "tangentvon", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getTangentv() const { return myTangentv; }
    void setTangentv(const UT_StringHolder & val) { myTangentv = val; }
    UT_StringHolder opTangentv(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTangentv();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "tangentv", cookparms.getCookTime(), 0);
        return result;
    }
    bool getOrtho() const { return myOrtho; }
    void setOrtho(bool val) { myOrtho = val; }
    bool opOrtho(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getOrtho();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "ortho", cookparms.getCookTime(), 0);
        return result;
    }
    bool getLeftHanded() const { return myLeftHanded; }
    void setLeftHanded(bool val) { myLeftHanded = val; }
    bool opLeftHanded(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLeftHanded();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "lefthanded", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myGroup;
    int64 myEntity;
    UT_StringHolder myStyle;
    UT_StringHolder myAttribname;
    bool myNon;
    UT_StringHolder myN;
    bool myTangentuon;
    UT_StringHolder myTangentu;
    bool myTangentvon;
    UT_StringHolder myTangentv;
    bool myOrtho;
    bool myLeftHanded;

};