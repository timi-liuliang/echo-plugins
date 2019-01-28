/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ParmTuple_h__
#define __HOM_ParmTuple_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_BaseKeyframe.h"
#include "HOM_PtrOrNull.h"
#include <string>
#include <vector>
#include <stdexcept>
class HOM_ParmTemplate;
class HOM_Parm;
class HOM_Ramp;

SWIGOUT(%rename(ParmTuple) HOM_ParmTuple;)

class HOM_API HOM_ParmTuple
{
public:
    HOM_ParmTuple()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ParmTuple(const HOM_ParmTuple &parm_tuple)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ParmTuple()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_ParmTuple> parm_tuple) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_ParmTuple> parm_tuple) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject node;)
    virtual HOM_Node *node() = 0;

    SWIGOUT(%newobject parmTemplate;)
    virtual HOM_ParmTemplate *parmTemplate() = 0;

    virtual std::string name() = 0;
    virtual std::string description() = 0;

    // Wrapping a version of set() that takes a vector of doubles will
    // let you pass a vector of doubles or a vector of ints to it.  So,
    // we don't bother to provide a version that takes a vector of ints.
    virtual void _set(const std::vector<double> &float_values) = 0;
    virtual void setPending(const std::vector<double> &float_values) = 0;
    virtual void _set(const std::vector<std::string> &string_values) = 0;
    virtual void setPending(const std::vector<std::string> &string_values) = 0;
    virtual void _set(const std::vector<HOM_Ramp *> &ramp_values) = 0;
    virtual void setPending(const std::vector<HOM_Ramp *> &ramp_values) = 0;

    virtual void revertToDefaults() = 0;
    virtual void revertToAndRestorePermanentDefaults() = 0;

    SWIGOUT(%kwargs isAtDefault;)
    virtual bool isAtDefault(bool compare_temporary_defaults=true,
			     bool compare_expressions=true) = 0;

    virtual std::vector<double> evalAsFloats() = 0;

    virtual std::vector<double> evalAsFloatsAtFrame(double frame) = 0;

    virtual std::vector<int> evalAsInts() = 0;

    virtual std::vector<int> evalAsIntsAtFrame(double frame) = 0;

    virtual std::vector<std::string> evalAsStrings() = 0;

    virtual std::vector<std::string> evalAsStringsAtFrame(double frame) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Ramp> > evalAsRamps() = 0;

    SWIGOUT(%newobject evalAsRampAtFrame;)
    virtual std::vector<HOM_ElemPtr<HOM_Ramp> > evalAsRampsAtFrame(double frame) = 0;

    SWIGOUT(%newobject evalAsGeometries;)
    virtual std::vector<HOM_ElemPtr<HOM_Geometry> > evalAsGeometries() = 0;

    SWIGOUT(%newobject evalAsGeometryAtFrame;)
    virtual std::vector<HOM_ElemPtr<HOM_Geometry> > evalAsGeometriesAtFrame(double frame) = 0;

    virtual void lock(const std::vector<bool> &bool_values) = 0;
    virtual void setAutoscope(const std::vector<bool> &bool_values) = 0;

    virtual bool isSpare() = 0;

    virtual bool isMultiParmInstance() = 0;
    SWIGOUT(%newobject parentMultiParm;)
    virtual HOM_Parm *parentMultiParm() = 0;
    virtual std::vector<int> multiParmInstanceIndices() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ParmTuple> > multiParmInstances() = 0;

    virtual void deleteAllKeyframes() = 0;

    SWIGOUT(%newobject __getitem__;)
    virtual HOM_Parm *__getitem__(int index) = 0;
    virtual int __len__() = 0;

    SWIGOUT(%newobject createClip;)
    virtual HOM_Node *createClip(HOM_Node &parent_node,
				 const char *name,
				 bool create_new,
				 bool apply_immediately,
				 bool current_value_only=false,
				 bool create_locked=false,
				 bool set_value_to_default=false) = 0;

    virtual void appendClip(HOM_Node &chop_node,
			    bool apply_immediately,
			    bool current_value_only=false,
			    bool create_locked=false,
			    bool set_value_to_default=false) = 0;

    virtual HOM_BinaryString _clipData(double start,
				       double end,
				       bool binary = true,
				       bool use_blosc_compression = true,
				       double sample_rate = 0) = 0;

    SWIGOUT(%kwargs setClipData;)
    virtual void setClipData(HOM_BinaryString data,
			     bool binary = true,
			     bool blosc_compressed = true,
			     double sample_rate = 0,
			     double start = 1) = 0;

    virtual void _saveClip(std::string file_name,
			   double start,
			   double end,
			   double sample_rate = 0) = 0;

    SWIGOUT(%kwargs loadClip;)
    virtual void loadClip(std::string file_name,
			  double sample_rate = 0,
			  double start = 1) = 0;

    virtual void copyToParmClipboard() = 0;

    virtual void *_asVoidPointer() = 0;

    SWIGOUT(%kwargs asCode;)
    virtual std::string asCode(bool brief=false, bool save_values=true,
			       bool save_keyframes=true,
			       bool save_keys_in_frames=false,
			       bool save_flag_values=true,
			       bool save_aliases=true,
			       const char *function_name=NULL) = 0;

    // You can get the parm data type with parmTemplate().dataType().
    // However, we need a way to get the parm data type's enum value
    // from C++ code without creating a temporary ParmTemplate object,
    // so that's why parmDataTypeEnumId() exists.
    SWIGOUT(%ignore parmDataTypeEnumId;)
    virtual int parmDataTypeEnumId() = 0;

    virtual std::string help() = 0;
    virtual std::string helpUrl() = 0;

    virtual bool isDisabled() = 0;

    virtual void disable(bool on) = 0;

    virtual bool isHidden() = 0;

    virtual void hide(bool on) = 0;

    virtual bool isConstrained() = 0;	
	
#ifdef SWIG
%extend
{
    InterpreterObject eval()
    {
	return HOMevalParmTuple(*self);
    }

    InterpreterObject evalAtFrame(double frame)
    {
	return HOMevalParmTupleAtFrame(*self, frame);
    }

    InterpreterObject evalAtTime(double time)
    {
	return HOMevalParmTupleAtFrame(*self, HOM().timeToFrame(time));
    }
}
#endif
};

#endif
