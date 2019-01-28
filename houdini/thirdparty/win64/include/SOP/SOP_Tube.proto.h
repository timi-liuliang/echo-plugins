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
namespace SOP_TubeEnums
{
    enum class Type
    {
        PRIM = 0,
        POLY,
        MESH,
        NURBS,
        BEZIER,
        POLYSOUP
    };
    enum class Surftype
    {
        ROWS = 0,
        COLS,
        ROWCOL,
        TRIANGLES,
        QUADS,
        ALTTRIANGLES,
        REVTRIANGLES
    };
    enum class Orient
    {
        X = 0,
        Y,
        Z
    };
}


class SOP_API SOP_TubeParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_TubeParms()
    {
        myType = 0;
        mySurftype = 4;
        myOrient = 1;
        myCap = false;
        myConsolidatepts = true;
        myVertexnormals = false;
        myT = UT_Vector3D(0,0,0);
        myR = UT_Vector3D(0,0,0);
        myRad = UT_Vector2D(1,1);
        myRadscale = 1;
        myHeight = 1;
        myRows = 2;
        myOrderu = 4;
        myOrderv = 2;
        myImperfect = true;
        myCols = 12;

    }

    explicit SOP_TubeParms(const SOP_TubeParms &) = default;

    virtual ~SOP_TubeParms() {}

    bool operator==(const SOP_TubeParms &src) const
    {
        if (myType != src.myType) return false;
        if (mySurftype != src.mySurftype) return false;
        if (myOrient != src.myOrient) return false;
        if (myCap != src.myCap) return false;
        if (myConsolidatepts != src.myConsolidatepts) return false;
        if (myVertexnormals != src.myVertexnormals) return false;
        if (myT != src.myT) return false;
        if (myR != src.myR) return false;
        if (myRad != src.myRad) return false;
        if (myRadscale != src.myRadscale) return false;
        if (myHeight != src.myHeight) return false;
        if (myRows != src.myRows) return false;
        if (myOrderu != src.myOrderu) return false;
        if (myOrderv != src.myOrderv) return false;
        if (myImperfect != src.myImperfect) return false;
        if (myCols != src.myCols) return false;

        return true;
    }
    bool operator!=(const SOP_TubeParms &src) const
    {
        return !operator==(src);
    }
    using Type = SOP_TubeEnums::Type;
    using Surftype = SOP_TubeEnums::Surftype;
    using Orient = SOP_TubeEnums::Orient;



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myType, node, "type", time, 0);
        else myType = 0;
        if (true && ( (!(((int64(getType())==0)))) ) )
            OP_Utils::evalOpParm(mySurftype, node, "surftype", time, 0);
        else mySurftype = 4;
        if (true)
            OP_Utils::evalOpParm(myOrient, node, "orient", time, 0);
        else myOrient = 1;
        if (true)
            OP_Utils::evalOpParm(myCap, node, "cap", time, 0);
        else myCap = false;
        if (true && ( (!(((getCap()==0))||((int64(getType())!=1)&&(int64(getType())!=5)))) ) )
            OP_Utils::evalOpParm(myConsolidatepts, node, "consolidatepts", time, 0);
        else myConsolidatepts = true;
        if (true)
            OP_Utils::evalOpParm(myVertexnormals, node, "vertexnormals", time, 0);
        else myVertexnormals = false;
        if (true && ( (!((((node->getInput(0)!=0)!=0)))) ) )
            OP_Utils::evalOpParm(myT, node, "t", time, 0);
        else myT = UT_Vector3D(0,0,0);
        if (true && ( (!((((node->getInput(0)!=0)!=0)))) ) )
            OP_Utils::evalOpParm(myR, node, "r", time, 0);
        else myR = UT_Vector3D(0,0,0);
        if (true && ( (!((((node->getInput(0)!=0)!=0)))) ) )
            OP_Utils::evalOpParm(myRad, node, "rad", time, 0);
        else myRad = UT_Vector2D(1,1);
        if (true && ( (!((((node->getInput(0)!=0)!=0)))) ) )
            OP_Utils::evalOpParm(myRadscale, node, "radscale", time, 0);
        else myRadscale = 1;
        if (true && ( (!((((node->getInput(0)!=0)!=0)))) ) )
            OP_Utils::evalOpParm(myHeight, node, "height", time, 0);
        else myHeight = 1;
        if (true && ( (!(((int64(getType())==0)))) ) )
            OP_Utils::evalOpParm(myRows, node, "rows", time, 0);
        else myRows = 2;
        if (true && ( (!(((int64(getType())!=3)&&(int64(getType())!=4)))) ) )
            OP_Utils::evalOpParm(myOrderu, node, "orderu", time, 0);
        else myOrderu = 4;
        if (true && ( (!(((int64(getType())!=3)&&(int64(getType())!=4)))) ) )
            OP_Utils::evalOpParm(myOrderv, node, "orderv", time, 0);
        else myOrderv = 2;
        if (true && ( (!(((int64(getType())!=3)&&(int64(getType())!=4)))) ) )
            OP_Utils::evalOpParm(myImperfect, node, "imperfect", time, 0);
        else myImperfect = true;
        if (true && ( (!(((int64(getType())==0))||((int64(getType())==4)&&(getImperfect()==0))||((int64(getType())==3)&&(getImperfect()==0)))) ) )
            OP_Utils::evalOpParm(myCols, node, "cols", time, 0);
        else myCols = 12;

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_TubeParms *)src);
    }

    template <typename T>
    void
    doGetParmValue(exint idx, T &value) const
    {
        switch (idx)
        {
            case 0:
                coerceValue(value, myType);
                break;
            case 1:
                coerceValue(value, mySurftype);
                break;
            case 2:
                coerceValue(value, myOrient);
                break;
            case 3:
                coerceValue(value, myCap);
                break;
            case 4:
                coerceValue(value, myConsolidatepts);
                break;
            case 5:
                coerceValue(value, myVertexnormals);
                break;
            case 6:
                coerceValue(value, myT);
                break;
            case 7:
                coerceValue(value, myR);
                break;
            case 8:
                coerceValue(value, myRad);
                break;
            case 9:
                coerceValue(value, myRadscale);
                break;
            case 10:
                coerceValue(value, myHeight);
                break;
            case 11:
                coerceValue(value, myRows);
                break;
            case 12:
                coerceValue(value, myOrderu);
                break;
            case 13:
                coerceValue(value, myOrderv);
                break;
            case 14:
                coerceValue(value, myImperfect);
                break;
            case 15:
                coerceValue(value, myCols);
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
                coerceValue(myType, value);
                break;
            case 1:
                coerceValue(mySurftype, value);
                break;
            case 2:
                coerceValue(myOrient, value);
                break;
            case 3:
                coerceValue(myCap, value);
                break;
            case 4:
                coerceValue(myConsolidatepts, value);
                break;
            case 5:
                coerceValue(myVertexnormals, value);
                break;
            case 6:
                coerceValue(myT, value);
                break;
            case 7:
                coerceValue(myR, value);
                break;
            case 8:
                coerceValue(myRad, value);
                break;
            case 9:
                coerceValue(myRadscale, value);
                break;
            case 10:
                coerceValue(myHeight, value);
                break;
            case 11:
                coerceValue(myRows, value);
                break;
            case 12:
                coerceValue(myOrderu, value);
                break;
            case 13:
                coerceValue(myOrderv, value);
                break;
            case 14:
                coerceValue(myImperfect, value);
                break;
            case 15:
                coerceValue(myCols, value);
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
        return 16;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "type";
            case 1:
                return "surftype";
            case 2:
                return "orient";
            case 3:
                return "cap";
            case 4:
                return "consolidatepts";
            case 5:
                return "vertexnormals";
            case 6:
                return "t";
            case 7:
                return "r";
            case 8:
                return "rad";
            case 9:
                return "radscale";
            case 10:
                return "height";
            case 11:
                return "rows";
            case 12:
                return "orderu";
            case 13:
                return "orderv";
            case 14:
                return "imperfect";
            case 15:
                return "cols";

        }
        return 0;
    }

    virtual ParmType getParmType(exint fieldnum) const
    {
        switch (fieldnum)
        {
                case 0:
                    return PARM_INTEGER;
                case 1:
                    return PARM_INTEGER;
                case 2:
                    return PARM_INTEGER;
                case 3:
                    return PARM_INTEGER;
                case 4:
                    return PARM_INTEGER;
                case 5:
                    return PARM_INTEGER;
                case 6:
                    return PARM_VECTOR3;
                case 7:
                    return PARM_VECTOR3;
                case 8:
                    return PARM_VECTOR2;
                case 9:
                    return PARM_FLOAT;
                case 10:
                    return PARM_FLOAT;
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
        saveData(os, myType);
        saveData(os, mySurftype);
        saveData(os, myOrient);
        saveData(os, myCap);
        saveData(os, myConsolidatepts);
        saveData(os, myVertexnormals);
        saveData(os, myT);
        saveData(os, myR);
        saveData(os, myRad);
        saveData(os, myRadscale);
        saveData(os, myHeight);
        saveData(os, myRows);
        saveData(os, myOrderu);
        saveData(os, myOrderv);
        saveData(os, myImperfect);
        saveData(os, myCols);

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
        loadData(is, myType);
        loadData(is, mySurftype);
        loadData(is, myOrient);
        loadData(is, myCap);
        loadData(is, myConsolidatepts);
        loadData(is, myVertexnormals);
        loadData(is, myT);
        loadData(is, myR);
        loadData(is, myRad);
        loadData(is, myRadscale);
        loadData(is, myHeight);
        loadData(is, myRows);
        loadData(is, myOrderu);
        loadData(is, myOrderv);
        loadData(is, myImperfect);
        loadData(is, myCols);

        return true;
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
    Surftype getSurftype() const { return Surftype(mySurftype); }
    void setSurftype(Surftype val) { mySurftype = int64(val); }
    Surftype opSurftype(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSurftype();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "surftype", cookparms.getCookTime(), 0);
        return Surftype(result);
    }
    Orient getOrient() const { return Orient(myOrient); }
    void setOrient(Orient val) { myOrient = int64(val); }
    Orient opOrient(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getOrient();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "orient", cookparms.getCookTime(), 0);
        return Orient(result);
    }
    bool getCap() const { return myCap; }
    void setCap(bool val) { myCap = val; }
    bool opCap(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCap();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "cap", cookparms.getCookTime(), 0);
        return result;
    }
    bool getConsolidatepts() const { return myConsolidatepts; }
    void setConsolidatepts(bool val) { myConsolidatepts = val; }
    bool opConsolidatepts(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getConsolidatepts();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "consolidatepts", cookparms.getCookTime(), 0);
        return result;
    }
    bool getVertexnormals() const { return myVertexnormals; }
    void setVertexnormals(bool val) { myVertexnormals = val; }
    bool opVertexnormals(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVertexnormals();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "vertexnormals", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector3D getT() const { return myT; }
    void setT(UT_Vector3D val) { myT = val; }
    UT_Vector3D opT(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getT();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "t", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector3D getR() const { return myR; }
    void setR(UT_Vector3D val) { myR = val; }
    UT_Vector3D opR(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getR();
        UT_Vector3D result;
        OP_Utils::evalOpParm(result, thissop, "r", cookparms.getCookTime(), 0);
        return result;
    }
    UT_Vector2D getRad() const { return myRad; }
    void setRad(UT_Vector2D val) { myRad = val; }
    UT_Vector2D opRad(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRad();
        UT_Vector2D result;
        OP_Utils::evalOpParm(result, thissop, "rad", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getRadscale() const { return myRadscale; }
    void setRadscale(fpreal64 val) { myRadscale = val; }
    fpreal64 opRadscale(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRadscale();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "radscale", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getHeight() const { return myHeight; }
    void setHeight(fpreal64 val) { myHeight = val; }
    fpreal64 opHeight(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getHeight();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "height", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getRows() const { return myRows; }
    void setRows(int64 val) { myRows = val; }
    int64 opRows(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRows();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "rows", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getOrderu() const { return myOrderu; }
    void setOrderu(int64 val) { myOrderu = val; }
    int64 opOrderu(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getOrderu();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "orderu", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getOrderv() const { return myOrderv; }
    void setOrderv(int64 val) { myOrderv = val; }
    int64 opOrderv(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getOrderv();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "orderv", cookparms.getCookTime(), 0);
        return result;
    }
    bool getImperfect() const { return myImperfect; }
    void setImperfect(bool val) { myImperfect = val; }
    bool opImperfect(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getImperfect();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "imperfect", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getCols() const { return myCols; }
    void setCols(int64 val) { myCols = val; }
    int64 opCols(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCols();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "cols", cookparms.getCookTime(), 0);
        return result;
    }

private:
    int64 myType;
    int64 mySurftype;
    int64 myOrient;
    bool myCap;
    bool myConsolidatepts;
    bool myVertexnormals;
    UT_Vector3D myT;
    UT_Vector3D myR;
    UT_Vector2D myRad;
    fpreal64 myRadscale;
    fpreal64 myHeight;
    int64 myRows;
    int64 myOrderu;
    int64 myOrderv;
    bool myImperfect;
    int64 myCols;

};