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

class SOP_API SOP_RemeshParms  : public SOP_NodeParms
{
public:
    static int version() { return 1; }

    SOP_RemeshParms()
    {
        myGroup = ""_sh;
        myHard_edges = ""_sh;
        myIterations = 1;
        myRecompute_normals = false;
        mySmoothing_level = 0;
        myTarget_edge = 0.2;
        myInput_pts_only = false;
        myElement_sizing = 0;
        myGradation = 0.25;
        myDensity = 1;
        myLimit_below = false;
        myMin_edge_length = 1e-05;
        myLimit_above = false;
        myMax_edge_length = 10000;
        myLfs_only = false;
        myVisualize_lfs = false;
        myLfs_ramp = UT_SharedPtr<UT_Ramp>(0);

    }

    explicit SOP_RemeshParms(const SOP_RemeshParms &) = default;

    virtual ~SOP_RemeshParms() {}

    bool operator==(const SOP_RemeshParms &src) const
    {
        if (myGroup != src.myGroup) return false;
        if (myHard_edges != src.myHard_edges) return false;
        if (myIterations != src.myIterations) return false;
        if (myRecompute_normals != src.myRecompute_normals) return false;
        if (mySmoothing_level != src.mySmoothing_level) return false;
        if (myTarget_edge != src.myTarget_edge) return false;
        if (myInput_pts_only != src.myInput_pts_only) return false;
        if (myElement_sizing != src.myElement_sizing) return false;
        if (myGradation != src.myGradation) return false;
        if (myDensity != src.myDensity) return false;
        if (myLimit_below != src.myLimit_below) return false;
        if (myMin_edge_length != src.myMin_edge_length) return false;
        if (myLimit_above != src.myLimit_above) return false;
        if (myMax_edge_length != src.myMax_edge_length) return false;
        if (myLfs_only != src.myLfs_only) return false;
        if (myVisualize_lfs != src.myVisualize_lfs) return false;
        if (myLfs_ramp != src.myLfs_ramp) return false;

        return true;
    }
    bool operator!=(const SOP_RemeshParms &src) const
    {
        return !operator==(src);
    }



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myGroup, node, "group", time, 0);
        else myGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myHard_edges, node, "hard_edges", time, 0);
        else myHard_edges = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myIterations, node, "iterations", time, 0);
        else myIterations = 1;
        if (true)
            OP_Utils::evalOpParm(myRecompute_normals, node, "recompute_normals", time, 0);
        else myRecompute_normals = false;
        if (true)
            OP_Utils::evalOpParm(mySmoothing_level, node, "smoothing_level", time, 0);
        else mySmoothing_level = 0;
        if (true)
            OP_Utils::evalOpParm(myTarget_edge, node, "target_edge", time, 0);
        else myTarget_edge = 0.2;
        if (true)
            OP_Utils::evalOpParm(myInput_pts_only, node, "input_pts_only", time, 0);
        else myInput_pts_only = false;
        if (true)
            OP_Utils::evalOpParm(myElement_sizing, node, "element_sizing", time, 0);
        else myElement_sizing = 0;
        if (true)
            OP_Utils::evalOpParm(myGradation, node, "gradation", time, 0);
        else myGradation = 0.25;
        if (true)
            OP_Utils::evalOpParm(myDensity, node, "density", time, 0);
        else myDensity = 1;
        if (true)
            OP_Utils::evalOpParm(myLimit_below, node, "limit_below", time, 0);
        else myLimit_below = false;
        if (true && ( (!(((getLimit_below()==0)))) ) )
            OP_Utils::evalOpParm(myMin_edge_length, node, "min_edge_length", time, 0);
        else myMin_edge_length = 1e-05;
        if (true)
            OP_Utils::evalOpParm(myLimit_above, node, "limit_above", time, 0);
        else myLimit_above = false;
        if (true && ( (!(((getLimit_above()==0)))) ) )
            OP_Utils::evalOpParm(myMax_edge_length, node, "max_edge_length", time, 0);
        else myMax_edge_length = 10000;
        if (true)
            OP_Utils::evalOpParm(myLfs_only, node, "lfs_only", time, 0);
        else myLfs_only = false;
        if (true)
            OP_Utils::evalOpParm(myVisualize_lfs, node, "visualize_lfs", time, 0);
        else myVisualize_lfs = false;
        if (true && ( (!(((getVisualize_lfs()==0)))) ) )
            OP_Utils::evalOpParm(myLfs_ramp, node, "lfs_ramp", time, 0);
        else myLfs_ramp = UT_SharedPtr<UT_Ramp>(0);

    }


    virtual void loadFromOpSubclass(const LoadParms &loadparms)
    {
        buildFromOp(loadparms.node(), loadparms.context().getTime(), loadparms.depnode());
    }


    virtual void copyFrom(const SOP_NodeParms *src)
    {
        *this = *((const SOP_RemeshParms *)src);
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
                coerceValue(value, myHard_edges);
                break;
            case 2:
                coerceValue(value, myIterations);
                break;
            case 3:
                coerceValue(value, myRecompute_normals);
                break;
            case 4:
                coerceValue(value, mySmoothing_level);
                break;
            case 5:
                coerceValue(value, myTarget_edge);
                break;
            case 6:
                coerceValue(value, myInput_pts_only);
                break;
            case 7:
                coerceValue(value, myElement_sizing);
                break;
            case 8:
                coerceValue(value, myGradation);
                break;
            case 9:
                coerceValue(value, myDensity);
                break;
            case 10:
                coerceValue(value, myLimit_below);
                break;
            case 11:
                coerceValue(value, myMin_edge_length);
                break;
            case 12:
                coerceValue(value, myLimit_above);
                break;
            case 13:
                coerceValue(value, myMax_edge_length);
                break;
            case 14:
                coerceValue(value, myLfs_only);
                break;
            case 15:
                coerceValue(value, myVisualize_lfs);
                break;
            case 16:
                coerceValue(value, myLfs_ramp);
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
                coerceValue(myHard_edges, value);
                break;
            case 2:
                coerceValue(myIterations, value);
                break;
            case 3:
                coerceValue(myRecompute_normals, value);
                break;
            case 4:
                coerceValue(mySmoothing_level, value);
                break;
            case 5:
                coerceValue(myTarget_edge, value);
                break;
            case 6:
                coerceValue(myInput_pts_only, value);
                break;
            case 7:
                coerceValue(myElement_sizing, value);
                break;
            case 8:
                coerceValue(myGradation, value);
                break;
            case 9:
                coerceValue(myDensity, value);
                break;
            case 10:
                coerceValue(myLimit_below, value);
                break;
            case 11:
                coerceValue(myMin_edge_length, value);
                break;
            case 12:
                coerceValue(myLimit_above, value);
                break;
            case 13:
                coerceValue(myMax_edge_length, value);
                break;
            case 14:
                coerceValue(myLfs_only, value);
                break;
            case 15:
                coerceValue(myVisualize_lfs, value);
                break;
            case 16:
                coerceValue(myLfs_ramp, value);
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
        return 17;
    }

    virtual const char *getParmName(exint fieldnum) const
    {
        switch (fieldnum)
        {
            case 0:
                return "group";
            case 1:
                return "hard_edges";
            case 2:
                return "iterations";
            case 3:
                return "recompute_normals";
            case 4:
                return "smoothing_level";
            case 5:
                return "target_edge";
            case 6:
                return "input_pts_only";
            case 7:
                return "element_sizing";
            case 8:
                return "gradation";
            case 9:
                return "density";
            case 10:
                return "limit_below";
            case 11:
                return "min_edge_length";
            case 12:
                return "limit_above";
            case 13:
                return "max_edge_length";
            case 14:
                return "lfs_only";
            case 15:
                return "visualize_lfs";
            case 16:
                return "lfs_ramp";

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
                    return PARM_INTEGER;
                case 4:
                    return PARM_FLOAT;
                case 5:
                    return PARM_FLOAT;
                case 6:
                    return PARM_INTEGER;
                case 7:
                    return PARM_INTEGER;
                case 8:
                    return PARM_FLOAT;
                case 9:
                    return PARM_FLOAT;
                case 10:
                    return PARM_INTEGER;
                case 11:
                    return PARM_FLOAT;
                case 12:
                    return PARM_INTEGER;
                case 13:
                    return PARM_FLOAT;
                case 14:
                    return PARM_INTEGER;
                case 15:
                    return PARM_INTEGER;
                case 16:
                    return PARM_RAMP;

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
        saveData(os, myHard_edges);
        saveData(os, myIterations);
        saveData(os, myRecompute_normals);
        saveData(os, mySmoothing_level);
        saveData(os, myTarget_edge);
        saveData(os, myInput_pts_only);
        saveData(os, myElement_sizing);
        saveData(os, myGradation);
        saveData(os, myDensity);
        saveData(os, myLimit_below);
        saveData(os, myMin_edge_length);
        saveData(os, myLimit_above);
        saveData(os, myMax_edge_length);
        saveData(os, myLfs_only);
        saveData(os, myVisualize_lfs);
        saveData(os, myLfs_ramp);

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
        loadData(is, myHard_edges);
        loadData(is, myIterations);
        loadData(is, myRecompute_normals);
        loadData(is, mySmoothing_level);
        loadData(is, myTarget_edge);
        loadData(is, myInput_pts_only);
        loadData(is, myElement_sizing);
        loadData(is, myGradation);
        loadData(is, myDensity);
        loadData(is, myLimit_below);
        loadData(is, myMin_edge_length);
        loadData(is, myLimit_above);
        loadData(is, myMax_edge_length);
        loadData(is, myLfs_only);
        loadData(is, myVisualize_lfs);
        loadData(is, myLfs_ramp);

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
    const UT_StringHolder & getHard_edges() const { return myHard_edges; }
    void setHard_edges(const UT_StringHolder & val) { myHard_edges = val; }
    UT_StringHolder opHard_edges(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getHard_edges();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "hard_edges", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getIterations() const { return myIterations; }
    void setIterations(int64 val) { myIterations = val; }
    int64 opIterations(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getIterations();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "iterations", cookparms.getCookTime(), 0);
        return result;
    }
    bool getRecompute_normals() const { return myRecompute_normals; }
    void setRecompute_normals(bool val) { myRecompute_normals = val; }
    bool opRecompute_normals(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getRecompute_normals();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "recompute_normals", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getSmoothing_level() const { return mySmoothing_level; }
    void setSmoothing_level(fpreal64 val) { mySmoothing_level = val; }
    fpreal64 opSmoothing_level(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSmoothing_level();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "smoothing_level", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getTarget_edge() const { return myTarget_edge; }
    void setTarget_edge(fpreal64 val) { myTarget_edge = val; }
    fpreal64 opTarget_edge(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getTarget_edge();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "target_edge", cookparms.getCookTime(), 0);
        return result;
    }
    bool getInput_pts_only() const { return myInput_pts_only; }
    void setInput_pts_only(bool val) { myInput_pts_only = val; }
    bool opInput_pts_only(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getInput_pts_only();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "input_pts_only", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getElement_sizing() const { return myElement_sizing; }
    void setElement_sizing(int64 val) { myElement_sizing = val; }
    int64 opElement_sizing(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getElement_sizing();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "element_sizing", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getGradation() const { return myGradation; }
    void setGradation(fpreal64 val) { myGradation = val; }
    fpreal64 opGradation(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getGradation();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "gradation", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getDensity() const { return myDensity; }
    void setDensity(fpreal64 val) { myDensity = val; }
    fpreal64 opDensity(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getDensity();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "density", cookparms.getCookTime(), 0);
        return result;
    }
    bool getLimit_below() const { return myLimit_below; }
    void setLimit_below(bool val) { myLimit_below = val; }
    bool opLimit_below(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLimit_below();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "limit_below", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMin_edge_length() const { return myMin_edge_length; }
    void setMin_edge_length(fpreal64 val) { myMin_edge_length = val; }
    fpreal64 opMin_edge_length(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMin_edge_length();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "min_edge_length", cookparms.getCookTime(), 0);
        return result;
    }
    bool getLimit_above() const { return myLimit_above; }
    void setLimit_above(bool val) { myLimit_above = val; }
    bool opLimit_above(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLimit_above();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "limit_above", cookparms.getCookTime(), 0);
        return result;
    }
    fpreal64 getMax_edge_length() const { return myMax_edge_length; }
    void setMax_edge_length(fpreal64 val) { myMax_edge_length = val; }
    fpreal64 opMax_edge_length(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMax_edge_length();
        fpreal64 result;
        OP_Utils::evalOpParm(result, thissop, "max_edge_length", cookparms.getCookTime(), 0);
        return result;
    }
    bool getLfs_only() const { return myLfs_only; }
    void setLfs_only(bool val) { myLfs_only = val; }
    bool opLfs_only(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLfs_only();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "lfs_only", cookparms.getCookTime(), 0);
        return result;
    }
    bool getVisualize_lfs() const { return myVisualize_lfs; }
    void setVisualize_lfs(bool val) { myVisualize_lfs = val; }
    bool opVisualize_lfs(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVisualize_lfs();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "visualize_lfs", cookparms.getCookTime(), 0);
        return result;
    }
    UT_SharedPtr<UT_Ramp> getLfs_ramp() const { return myLfs_ramp; }
    void setLfs_ramp(UT_SharedPtr<UT_Ramp> val) { myLfs_ramp = val; }
    UT_SharedPtr<UT_Ramp> opLfs_ramp(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getLfs_ramp();
        UT_SharedPtr<UT_Ramp> result;
        OP_Utils::evalOpParm(result, thissop, "lfs_ramp", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myGroup;
    UT_StringHolder myHard_edges;
    int64 myIterations;
    bool myRecompute_normals;
    fpreal64 mySmoothing_level;
    fpreal64 myTarget_edge;
    bool myInput_pts_only;
    int64 myElement_sizing;
    fpreal64 myGradation;
    fpreal64 myDensity;
    bool myLimit_below;
    fpreal64 myMin_edge_length;
    bool myLimit_above;
    fpreal64 myMax_edge_length;
    bool myLfs_only;
    bool myVisualize_lfs;
    UT_SharedPtr<UT_Ramp> myLfs_ramp;

};