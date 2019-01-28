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

class SOP_API SOP_AttribVopParms 
{
public:
    static int version() { return 1; }
    struct Bindings
    {
        UT_StringHolder bindname;
        UT_StringHolder bindparm;


        Bindings()
        {
            bindname = ""_sh;
            bindparm = ""_sh;

        }

        bool operator==(const Bindings &src) const
        {
            if (bindname != src.bindname) return false;
            if (bindparm != src.bindparm) return false;

            return true;
        }
        bool operator!=(const Bindings &src) const
        {
            return !operator==(src);
        }

    };

    UT_StringHolder createString(const UT_Array<Bindings> &list) const
    {
        UT_WorkBuffer   buf;

        buf.strcat("[ ");
        for (int i = 0; i < list.entries(); i++)
        {
            if (i)
                buf.strcat(", ");
            buf.strcat("( ");
            buf.append("");
            { UT_String tmp; tmp = UT_StringWrap(list(i).bindname).makeQuotedString('"'); buf.strcat(tmp); }
            buf.append(", ");
            { UT_String tmp; tmp = UT_StringWrap(list(i).bindparm).makeQuotedString('"'); buf.strcat(tmp); }

            buf.strcat(" )");
        }
        buf.strcat(" ]");

        UT_StringHolder result = buf;
        return result;
    }
    struct GroupBindings
    {
        UT_StringHolder bindgroupname;
        UT_StringHolder bindgroupparm;


        GroupBindings()
        {
            bindgroupname = ""_sh;
            bindgroupparm = ""_sh;

        }

        bool operator==(const GroupBindings &src) const
        {
            if (bindgroupname != src.bindgroupname) return false;
            if (bindgroupparm != src.bindgroupparm) return false;

            return true;
        }
        bool operator!=(const GroupBindings &src) const
        {
            return !operator==(src);
        }

    };

    UT_StringHolder createString(const UT_Array<GroupBindings> &list) const
    {
        UT_WorkBuffer   buf;

        buf.strcat("[ ");
        for (int i = 0; i < list.entries(); i++)
        {
            if (i)
                buf.strcat(", ");
            buf.strcat("( ");
            buf.append("");
            { UT_String tmp; tmp = UT_StringWrap(list(i).bindgroupname).makeQuotedString('"'); buf.strcat(tmp); }
            buf.append(", ");
            { UT_String tmp; tmp = UT_StringWrap(list(i).bindgroupparm).makeQuotedString('"'); buf.strcat(tmp); }

            buf.strcat(" )");
        }
        buf.strcat(" ]");

        UT_StringHolder result = buf;
        return result;
    }

    SOP_AttribVopParms()
    {
        myVexCWD = ""_sh;
        myVexOutputMask = "*"_sh;
        myAutobind = true;
        myGroupAutobind = true;
        myBindClass = 2;
        myBindGroup = ""_sh;
        myBindGroupType = 0;
        myMultiThread = true;
        myUpdateNormals = false;
        myNumCount = 10;
        myThreadJobSize = 1024;
        myMatchAttrib = "id"_sh;
        mySelectionGroup = ""_sh;
        myCookInPlace = false;

    }

    explicit SOP_AttribVopParms(const SOP_AttribVopParms &) = default;

    ~SOP_AttribVopParms() {}

    bool operator==(const SOP_AttribVopParms &src) const
    {
        if (myVexCWD != src.myVexCWD) return false;
        if (myVexOutputMask != src.myVexOutputMask) return false;
        if (myBindings != src.myBindings) return false;
        if (myGroupBindings != src.myGroupBindings) return false;
        if (myAutobind != src.myAutobind) return false;
        if (myGroupAutobind != src.myGroupAutobind) return false;
        if (myBindClass != src.myBindClass) return false;
        if (myBindGroup != src.myBindGroup) return false;
        if (myBindGroupType != src.myBindGroupType) return false;
        if (myMultiThread != src.myMultiThread) return false;
        if (myUpdateNormals != src.myUpdateNormals) return false;
        if (myNumCount != src.myNumCount) return false;
        if (myThreadJobSize != src.myThreadJobSize) return false;
        if (myMatchAttrib != src.myMatchAttrib) return false;
        if (mySelectionGroup != src.mySelectionGroup) return false;
        if (myCookInPlace != src.myCookInPlace) return false;

        return true;
    }
    bool operator!=(const SOP_AttribVopParms &src) const
    {
        return !operator==(src);
    }



    void        buildFromOp(const OP_Node *node, fpreal time, DEP_MicroNode *depnode)
    {
        if (true)
            OP_Utils::evalOpParm(myVexCWD, node, "vex_cwdpath", time, 0);
        else myVexCWD = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myVexOutputMask, node, "vex_outputmask", time, 0);
        else myVexOutputMask = "*"_sh;
            if (true)
            {
                int64   length;
                OP_Utils::evalOpParm(length, node, "bindings", time, 0);
                myBindings.entries(length);
                for (exint i = 0; i < length; i++)
                {
                    int     parmidx = i+1;
                    auto && _curentry = myBindings(i);
                    (void) _curentry;
                    if (true)
                        OP_Utils::evalOpParmInst(myBindings(i).bindname, node, "bindname#", &parmidx, time, 0);
                    else myBindings(i).bindname = ""_sh;
                    if (true)
                        OP_Utils::evalOpParmInst(myBindings(i).bindparm, node, "bindparm#", &parmidx, time, 0);
                    else myBindings(i).bindparm = ""_sh;

                }
            }
            else
                myBindings.clear();
            if (true)
            {
                int64   length;
                OP_Utils::evalOpParm(length, node, "groupbindings", time, 0);
                myGroupBindings.entries(length);
                for (exint i = 0; i < length; i++)
                {
                    int     parmidx = i+1;
                    auto && _curentry = myGroupBindings(i);
                    (void) _curentry;
                    if (true)
                        OP_Utils::evalOpParmInst(myGroupBindings(i).bindgroupname, node, "bindgroupname#", &parmidx, time, 0);
                    else myGroupBindings(i).bindgroupname = ""_sh;
                    if (true)
                        OP_Utils::evalOpParmInst(myGroupBindings(i).bindgroupparm, node, "bindgroupparm#", &parmidx, time, 0);
                    else myGroupBindings(i).bindgroupparm = ""_sh;

                }
            }
            else
                myGroupBindings.clear();
        if (true)
            OP_Utils::evalOpParm(myAutobind, node, "autobind", time, 0);
        else myAutobind = true;
        if (true)
            OP_Utils::evalOpParm(myGroupAutobind, node, "groupautobind", time, 0);
        else myGroupAutobind = true;
        if (true)
            OP_Utils::evalOpParm(myBindClass, node, "bindclass", time, 0);
        else myBindClass = 2;
        if (true)
            OP_Utils::evalOpParm(myBindGroup, node, "bindgroup", time, 0);
        else myBindGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myBindGroupType, node, "bindgrouptype", time, 0);
        else myBindGroupType = 0;
        if (true)
            OP_Utils::evalOpParm(myMultiThread, node, "vex_multithread", time, 0);
        else myMultiThread = true;
        if (true)
            OP_Utils::evalOpParm(myUpdateNormals, node, "vex_updatenmls", time, 0);
        else myUpdateNormals = false;
        if (true && ( (getBindClass()==4) ) )
            OP_Utils::evalOpParm(myNumCount, node, "vex_numcount", time, 0);
        else myNumCount = 10;
        if (true && ( (getBindClass()==4) ) )
            OP_Utils::evalOpParm(myThreadJobSize, node, "vex_threadjobsize", time, 0);
        else myThreadJobSize = 1024;
        if (true)
            OP_Utils::evalOpParm(myMatchAttrib, node, "vex_matchattrib", time, 0);
        else myMatchAttrib = "id"_sh;
        if (true)
            OP_Utils::evalOpParm(mySelectionGroup, node, "vex_selectiongroup", time, 0);
        else mySelectionGroup = ""_sh;
        if (true)
            OP_Utils::evalOpParm(myCookInPlace, node, "vex_inplace", time, 0);
        else myCookInPlace = false;

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
        saveData(os, myVexCWD);
        saveData(os, myVexOutputMask);
        {
            int64   length = myBindings.entries();
            UTwrite(os, &length);
            for (exint i = 0; i < length; i++)
            {
                saveData(os, myBindings(i).bindname);
                saveData(os, myBindings(i).bindparm);

            }
        }
        {
            int64   length = myGroupBindings.entries();
            UTwrite(os, &length);
            for (exint i = 0; i < length; i++)
            {
                saveData(os, myGroupBindings(i).bindgroupname);
                saveData(os, myGroupBindings(i).bindgroupparm);

            }
        }
        saveData(os, myAutobind);
        saveData(os, myGroupAutobind);
        saveData(os, myBindClass);
        saveData(os, myBindGroup);
        saveData(os, myBindGroupType);
        saveData(os, myMultiThread);
        saveData(os, myUpdateNormals);
        saveData(os, myNumCount);
        saveData(os, myThreadJobSize);
        saveData(os, myMatchAttrib);
        saveData(os, mySelectionGroup);
        saveData(os, myCookInPlace);

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
        loadData(is, myVexCWD);
        loadData(is, myVexOutputMask);
        {
            int64   length;
            is.read(&length, 1);
            myBindings.entries(length);
            for (exint i = 0; i < length; i++)
            {
                loadData(is, myBindings(i).bindname);
                loadData(is, myBindings(i).bindparm);

            }
        }
        {
            int64   length;
            is.read(&length, 1);
            myGroupBindings.entries(length);
            for (exint i = 0; i < length; i++)
            {
                loadData(is, myGroupBindings(i).bindgroupname);
                loadData(is, myGroupBindings(i).bindgroupparm);

            }
        }
        loadData(is, myAutobind);
        loadData(is, myGroupAutobind);
        loadData(is, myBindClass);
        loadData(is, myBindGroup);
        loadData(is, myBindGroupType);
        loadData(is, myMultiThread);
        loadData(is, myUpdateNormals);
        loadData(is, myNumCount);
        loadData(is, myThreadJobSize);
        loadData(is, myMatchAttrib);
        loadData(is, mySelectionGroup);
        loadData(is, myCookInPlace);

        return true;
    }

    const UT_StringHolder & getVexCWD() const { return myVexCWD; }
    void setVexCWD(const UT_StringHolder & val) { myVexCWD = val; }
    UT_StringHolder opVexCWD(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVexCWD();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "vex_cwdpath", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getVexOutputMask() const { return myVexOutputMask; }
    void setVexOutputMask(const UT_StringHolder & val) { myVexOutputMask = val; }
    UT_StringHolder opVexOutputMask(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getVexOutputMask();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "vex_outputmask", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_Array<Bindings> &getBindings() const { return myBindings; }
    void setBindings(const UT_Array<Bindings> &val) { myBindings = val; }
    exint opBindings(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getBindings().entries();
        exint result;
        OP_Utils::evalOpParm(result, thissop, "bindings", cookparms.getCookTime(), 0);
        return result;
    }
            UT_StringHolder opBindings_bindname(const SOP_NodeVerb::CookParms &cookparms, exint _idx) const
        {
            SOP_Node *thissop = cookparms.getNode();
            if (!thissop) return (myBindings(_idx).bindname);
            int _parmidx = _idx + 1;
            UT_StringHolder result;
            OP_Utils::evalOpParmInst(result, thissop, "bindname#", &_parmidx, cookparms.getCookTime(), 0);
            return (result);
        }
        UT_StringHolder opBindings_bindparm(const SOP_NodeVerb::CookParms &cookparms, exint _idx) const
        {
            SOP_Node *thissop = cookparms.getNode();
            if (!thissop) return (myBindings(_idx).bindparm);
            int _parmidx = _idx + 1;
            UT_StringHolder result;
            OP_Utils::evalOpParmInst(result, thissop, "bindparm#", &_parmidx, cookparms.getCookTime(), 0);
            return (result);
        }

    const UT_Array<GroupBindings> &getGroupBindings() const { return myGroupBindings; }
    void setGroupBindings(const UT_Array<GroupBindings> &val) { myGroupBindings = val; }
    exint opGroupBindings(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getGroupBindings().entries();
        exint result;
        OP_Utils::evalOpParm(result, thissop, "groupbindings", cookparms.getCookTime(), 0);
        return result;
    }
            UT_StringHolder opGroupBindings_bindgroupname(const SOP_NodeVerb::CookParms &cookparms, exint _idx) const
        {
            SOP_Node *thissop = cookparms.getNode();
            if (!thissop) return (myGroupBindings(_idx).bindgroupname);
            int _parmidx = _idx + 1;
            UT_StringHolder result;
            OP_Utils::evalOpParmInst(result, thissop, "bindgroupname#", &_parmidx, cookparms.getCookTime(), 0);
            return (result);
        }
        UT_StringHolder opGroupBindings_bindgroupparm(const SOP_NodeVerb::CookParms &cookparms, exint _idx) const
        {
            SOP_Node *thissop = cookparms.getNode();
            if (!thissop) return (myGroupBindings(_idx).bindgroupparm);
            int _parmidx = _idx + 1;
            UT_StringHolder result;
            OP_Utils::evalOpParmInst(result, thissop, "bindgroupparm#", &_parmidx, cookparms.getCookTime(), 0);
            return (result);
        }

    bool getAutobind() const { return myAutobind; }
    void setAutobind(bool val) { myAutobind = val; }
    bool opAutobind(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getAutobind();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "autobind", cookparms.getCookTime(), 0);
        return result;
    }
    bool getGroupAutobind() const { return myGroupAutobind; }
    void setGroupAutobind(bool val) { myGroupAutobind = val; }
    bool opGroupAutobind(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getGroupAutobind();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "groupautobind", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getBindClass() const { return myBindClass; }
    void setBindClass(int64 val) { myBindClass = val; }
    int64 opBindClass(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getBindClass();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "bindclass", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getBindGroup() const { return myBindGroup; }
    void setBindGroup(const UT_StringHolder & val) { myBindGroup = val; }
    UT_StringHolder opBindGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getBindGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "bindgroup", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getBindGroupType() const { return myBindGroupType; }
    void setBindGroupType(int64 val) { myBindGroupType = val; }
    int64 opBindGroupType(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getBindGroupType();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "bindgrouptype", cookparms.getCookTime(), 0);
        return result;
    }
    bool getMultiThread() const { return myMultiThread; }
    void setMultiThread(bool val) { myMultiThread = val; }
    bool opMultiThread(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMultiThread();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "vex_multithread", cookparms.getCookTime(), 0);
        return result;
    }
    bool getUpdateNormals() const { return myUpdateNormals; }
    void setUpdateNormals(bool val) { myUpdateNormals = val; }
    bool opUpdateNormals(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getUpdateNormals();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "vex_updatenmls", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getNumCount() const { return myNumCount; }
    void setNumCount(int64 val) { myNumCount = val; }
    int64 opNumCount(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getNumCount();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "vex_numcount", cookparms.getCookTime(), 0);
        return result;
    }
    int64 getThreadJobSize() const { return myThreadJobSize; }
    void setThreadJobSize(int64 val) { myThreadJobSize = val; }
    int64 opThreadJobSize(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getThreadJobSize();
        int64 result;
        OP_Utils::evalOpParm(result, thissop, "vex_threadjobsize", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getMatchAttrib() const { return myMatchAttrib; }
    void setMatchAttrib(const UT_StringHolder & val) { myMatchAttrib = val; }
    UT_StringHolder opMatchAttrib(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getMatchAttrib();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "vex_matchattrib", cookparms.getCookTime(), 0);
        return result;
    }
    const UT_StringHolder & getSelectionGroup() const { return mySelectionGroup; }
    void setSelectionGroup(const UT_StringHolder & val) { mySelectionGroup = val; }
    UT_StringHolder opSelectionGroup(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getSelectionGroup();
        UT_StringHolder result;
        OP_Utils::evalOpParm(result, thissop, "vex_selectiongroup", cookparms.getCookTime(), 0);
        return result;
    }
    bool getCookInPlace() const { return myCookInPlace; }
    void setCookInPlace(bool val) { myCookInPlace = val; }
    bool opCookInPlace(const SOP_NodeVerb::CookParms &cookparms) const
    { 
        SOP_Node *thissop = cookparms.getNode();
        if (!thissop) return getCookInPlace();
        bool result;
        OP_Utils::evalOpParm(result, thissop, "vex_inplace", cookparms.getCookTime(), 0);
        return result;
    }

private:
    UT_StringHolder myVexCWD;
    UT_StringHolder myVexOutputMask;
    UT_Array<Bindings> myBindings;
    UT_Array<GroupBindings> myGroupBindings;
    bool myAutobind;
    bool myGroupAutobind;
    int64 myBindClass;
    UT_StringHolder myBindGroup;
    int64 myBindGroupType;
    bool myMultiThread;
    bool myUpdateNormals;
    int64 myNumCount;
    int64 myThreadJobSize;
    UT_StringHolder myMatchAttrib;
    UT_StringHolder mySelectionGroup;
    bool myCookInPlace;

};
