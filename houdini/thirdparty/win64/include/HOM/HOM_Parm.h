/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Parm_h__
#define __HOM_Parm_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_BaseKeyframe.h"
#include "HOM_PtrOrNull.h"
#include <string>
#include <map>
#include <vector>
class HOM_ParmTemplate;
class HOM_ParmTuple;
class HOM_Track;
class HOM_Ramp;
class HOM_Geometry;
class UT_WorkBuffer;

SWIGOUT(%rename(Parm) HOM_Parm;)

class HOM_API HOM_Parm
{
public:
    HOM_Parm()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Parm(const HOM_Parm &parm)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Parm()
    { HOM_DESTRUCT_OBJECT(this) }

    // We compare against a pointer instead of a reference because we want
    // to be able to compare against None.
    virtual bool operator==(HOM_PtrOrNull<HOM_Parm> parm) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_Parm> parm) = 0;
    SWIGOUT(%ignore operator<(HOM_Parm *parm);)
    virtual bool operator<(HOM_Parm *parm) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject node;)
    virtual HOM_Node *node() = 0;

    SWIGOUT(%newobject tuple;)
    virtual HOM_ParmTuple *tuple() = 0;

    SWIGOUT(%newobject parmTemplate;)
    virtual HOM_ParmTemplate *parmTemplate() = 0;

    virtual std::string name() = 0;
    virtual std::string path() = 0;
    virtual std::string description() = 0;
    virtual int componentIndex() = 0;

    virtual std::vector<std::string> menuLabels() = 0;
    virtual std::vector<std::string> menuItems() = 0;

    SWIGOUT(%kwargs pressButton;)
    virtual void pressButton(std::map<std::string, hboost::any> const & arguments=std::map<std::string, hboost::any>()) = 0;

    virtual void _set(double float_value) = 0;
    virtual void setPending(double float_value) = 0;
    virtual void _set(int int_value) = 0;
    virtual void setPending(int int_value) = 0;
    virtual void _set(const char *string_value) = 0;
    virtual void setPending(const char *string_value) = 0;
    virtual void _set(HOM_Geometry *geo) = 0;
    virtual void setPending(HOM_Geometry *geo) = 0;
    virtual void _set(const std::map<std::string, std::string> &map) = 0;
    virtual void setPending(const std::map<std::string, std::string> &map) = 0;
    virtual void _set(HOM_Ramp *ramp_value) = 0;
    virtual void setPending(HOM_Ramp *ramp_value) = 0;

    virtual void _set(
	HOM_Parm &parm, HOM_EnumValue *language=nullptr, 
	bool follow_parm_reference=true) = 0;

    virtual void setFromParm(HOM_Parm &src) = 0;

    virtual void revertToDefaults() = 0;
    virtual void revertToAndRestorePermanentDefaults() = 0;
    virtual bool hasTemporaryDefaults() = 0;
    SWIGOUT(%kwargs isAtDefault;)
    virtual bool isAtDefault(bool compare_temporary_defaults=true,
			     bool compare_expressions=true) = 0;

    SWIGOUT(%kwargs setExpression;)
    virtual void setExpression(const char *expression,
	    HOM_EnumValue *language = NULL,
	    bool replace_expression=true) = 0;
    virtual std::string expression() = 0;

    virtual HOM_EnumValue *expressionLanguage() = 0;

    virtual std::string rawValue() = 0;


    virtual void setKeyframe(HOM_BaseKeyframe &keyframe) = 0;
    virtual void setKeyframes(const std::vector<HOM_BaseKeyframe *> &keyframes) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_BaseKeyframe> > keyframes() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_BaseKeyframe> >
	keyframesInRange(double start_frame, double end_frame) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_BaseKeyframe> > keyframesBefore(double frame) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_BaseKeyframe> > keyframesAfter(double frame) = 0;

    virtual double evalAsFloat() = 0;
    virtual double evalAsFloatAtFrame(double frame) = 0;
    virtual int evalAsInt() = 0;
    virtual int evalAsIntAtFrame(double frame) = 0;
    virtual std::string evalAsString() = 0;
    virtual std::string evalAsStringAtFrame(double frame) = 0;
    virtual HOM_Geometry *evalAsGeometry() = 0;
    virtual HOM_Geometry *evalAsGeometryAtFrame(double frame) = 0;
    virtual std::map<std::string, std::string> evalAsJSONMap() = 0;
    virtual std::map<std::string, std::string> 
	evalAsJSONMapAtFrame(double frame) = 0;
    SWIGOUT(%newobject evalAsRamp;)
    virtual HOM_Ramp *evalAsRamp() = 0;
    SWIGOUT(%newobject evalAsRampAtFrame;)
    virtual HOM_Ramp *evalAsRampAtFrame(double frame) = 0;
    SWIGOUT(%newobject evalAsNode;)
    virtual HOM_Node *evalAsNode() = 0;
    SWIGOUT(%newobject evalAsNodeAtFrame;)
    virtual HOM_Node *evalAsNodeAtFrame(double frame) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Node> > evalAsNodes() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Node> > evalAsNodesAtFrame(double frame) = 0;

    virtual std::string unexpandedString() = 0;

    virtual void deleteKeyframeAtFrame(double frame) = 0;

    virtual void deleteAllKeyframes() = 0;

    // TODO: Rename this to referencedParm()!
    virtual HOM_Parm *getReferencedParm() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Parm> > parmsReferencingThis() = 0;

    virtual bool isLocked() = 0;
    virtual void lock(bool on) = 0;

    virtual bool isAutoscoped() = 0;
    virtual void setAutoscope(bool on) = 0;

    virtual bool isScoped() = 0;
    virtual void setScope(bool on) = 0;

    virtual bool isAutoSelected() = 0;
    virtual void setAutoSelect(bool on) = 0;

    virtual bool isSelected() = 0;
    virtual void setSelect(bool on) = 0;

    virtual std::string alias() = 0;
    virtual void setAlias(const char *alias_name) = 0;

    virtual bool isSpare() = 0;

    virtual bool isTimeDependent() = 0;

    virtual bool isMultiParmInstance() = 0;
    SWIGOUT(%newobject parentMultiParm;)
    virtual HOM_Parm *parentMultiParm() = 0;
    virtual std::vector<int> multiParmInstanceIndices() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Parm> > multiParmInstances() = 0;

    virtual void insertMultiParmInstance(int inst_index) = 0;

    virtual void removeMultiParmInstance(int inst_index) = 0;

    virtual std::vector<std::string> containingFolders() = 0;

    virtual std::vector<int> containingFolderIndices() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ParmTuple> >
	    containingFolderSetParmTuples() = 0;

    SWIGOUT(%kwargs asCode;)
    virtual std::string asCode(
	    bool brief = false,
	    bool save_values = true,
	    bool save_keyframes = true,
	    bool save_keys_in_frames=false,
	    bool save_flag_values=true,
	    bool save_aliases=true,
	    const char *function_name = NULL) = 0;

    // You can get the parm data type with parmTemplate().dataType().
    // However, we need a way to get the parm data type's enum value
    // from C++ code without creating a temporary ParmTemplate object,
    // so that's why parmDataTypeEnumId() exists.
    SWIGOUT(%ignore parmDataTypeEnumId;)
    virtual int parmDataTypeEnumId() = 0;

    // For data parameters only.
    // You can get the data parm type with parmTemplate().dataParmType().
    // However, we need a way to get the data parm type's enum value
    // from C++ code without creating a temporary ParmTemplate object,
    // so that's why dataParmTypeEnumId() exists.
    SWIGOUT(%ignore dataParmTypeEnumId;)
    virtual int dataParmTypeEnumId() = 0;

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

    SWIGOUT(%newobject overrideTrack;)
    virtual HOM_Track *overrideTrack() = 0;

    virtual void copyToParmClipboard() = 0;

    virtual bool isDisabled() = 0;
    
    virtual void disable(bool on) = 0;

    virtual bool isHidden() = 0;
    
    virtual void hide(bool on) = 0;

    virtual bool isVisible() = 0;

    virtual bool isConstrained() = 0;

    virtual HOM_EnumValue &keyframeExtrapolation( bool before ) = 0;

    virtual void setKeyframeExtrapolation( bool before, HOM_EnumValue &e ) = 0;

    virtual void keyframesRefit(
	bool refit, double refit_tol, bool refit_preserve_extremas,
	bool refit_bezier, bool resample, double resample_rate, double resample_tol,
	bool range, double range_start, double range_end, HOM_EnumValue &bake_chop, const char *expression="") = 0;

#ifdef SWIG
%extend
{
    InterpreterObject eval()
    {
	return HOMevalParm(*self);
    }

    InterpreterObject evalAtFrame(double frame)
    {
	return HOMevalParmAtFrame(*self, frame);
    }

    InterpreterObject evalAtTime(double time)
    {
	return HOMevalParmAtFrame(*self, HOM().timeToFrame(time));
    }
}
#endif
};

#endif
