/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_RampUtils.h (Parameter Library)
 */

#ifndef __PRM_RampUtils_h__
#define __PRM_RampUtils_h__

#include "PRM_API.h"
#include "PRM_Parm.h"
#include "PRM_Type.h"
#include <UT/UT_Spline.h>
#include <SYS/SYS_Types.h>

class UT_OStringStream;
class UT_Ramp;

PRM_API UT_SPLINE_BASIS	    PRMgetRampBasis(int ramp_interp);
PRM_API PRM_RampInterpType  PRMgetRampInterp(UT_SPLINE_BASIS basis);

/// Utility functions for working with ramp parameters.
class PRM_API PRM_RampUtils
{
public:
    /// Helper enumeration for stating the type of value that should be set
    /// when updating multi-parms from ramps.
    enum PRM_ValueType
    {
	PRM_INT_TYPE,
	PRM_FLOAT_TYPE
    };

    /// Helper class for passing data to callback functions
    /// when updating multi-parms from ramps.
    class ValueData
    {
    public:
	inline ValueData(
	    void *callee, fpreal t, PRM_Parm &p, int vector_index, int val, 
	    PRM_AddKeyType add_key);
	inline ValueData(
	    void *callee, fpreal t, PRM_Parm &p, int vector_index, fpreal val, 
	    PRM_AddKeyType add_key);

	inline void *callee() const;
	inline fpreal time() const;
	inline PRM_Parm &parm() const;
	inline int vectorIndex() const;
	inline PRM_ValueType valueType() const;
	inline int intValue() const;
	inline fpreal floatValue() const;
	inline PRM_AddKeyType addKey() const;

    private:
	void *myCallee;
	fpreal myTime;
	PRM_Parm &myParm;
	int myVectorIndex;
	PRM_ValueType myValueType;
	PRM_AddKeyType myAddKey;
	int myIntValue;
	fpreal myFloatValue;
    };

    /// Callback function which sets values when updating multi-parms from
    /// ramps.
    typedef void (*SetValueCB)(ValueData value_info);

    /// Update ramp value from a multi parm.
    static void updateRampFromMultiParm(
	fpreal t, const PRM_Parm &m, UT_Ramp *ramp, 
	bool *time_dep = NULL, DEP_MicroNode *depnode = NULL);

    /// Update multi parm from a ramp value.
    static void updateMultiParmFromRamp(
	fpreal t, const UT_Ramp &ramp, PRM_Parm &m, bool add_wrap, 
	PRM_AddKeyType add_key);

    /// Update multi parm from a ramp value.
    /// This method version takes in a callback function that sets the values
    /// in the multi parm.
    static void updateMultiParmFromRamp(
	fpreal t, const UT_Ramp &ramp, PRM_Parm &m, bool add_wrap, 
	PRM_AddKeyType add_key, SetValueCB set_value_cb, void *cb_callee);

    /// Write out the ramp parameter values as an encoding of ramp defaults.
    static void		setRampDefaultFromParm(UT_OStringStream &defaults_os,
				const PRM_Parm &ramp_parm);

    /// Parse the ramp defaults and set them as values on the parameter
    static void		getRampParmFromDefault(PRM_Parm &ramp_parm,
				const char *defaults_string);

    /// Get the basis, positions, value (as component sequence array), and 
    /// value tuple size that define the ramp parameter value.
    static void		getRampValuesFromParm(
				UT_DoubleArray &values, int &tuple_size,
				UT_DoubleArray &keys, UT_StringArray &basis,
				const UT_Ramp &ramp, bool is_rgb);

    /// Construct the string that defines the ramp default value based
    /// on provided basis, positions, and value arrays.
    static void		getRampDefaultString(UT_WorkBuffer &wbuf,
				const UT_DoubleArray &values, int tuple_size,
				const UT_DoubleArray *keys,
				const UT_StringArray *basis);
};

// ==========================================================================
// PRM_RampUtils::ValueData implementation.
// ==========================================================================
PRM_RampUtils::ValueData::ValueData(
    void *callee, fpreal t, PRM_Parm &p, int vector_index, int val, 
    PRM_AddKeyType add_key)
    : myCallee(callee)
    , myTime(t)
    , myParm(p)
    , myVectorIndex(vector_index)
    , myValueType(PRM_RampUtils::PRM_INT_TYPE)
    , myIntValue(val)
    , myFloatValue(0.0)
    , myAddKey(add_key)
{
}

PRM_RampUtils::ValueData::ValueData(
    void *callee, fpreal t, PRM_Parm &p, int vector_index, fpreal val, 
    PRM_AddKeyType add_key)
    : myCallee(callee)
    , myTime(t)
    , myParm(p)
    , myVectorIndex(vector_index)
    , myValueType(PRM_RampUtils::PRM_FLOAT_TYPE)
    , myIntValue(0)
    , myFloatValue(val)
    , myAddKey(add_key)
{
}

inline void *
PRM_RampUtils::ValueData::callee() const { return myCallee; }

inline fpreal
PRM_RampUtils::ValueData::time() const { return myTime; }

inline PRM_Parm &
PRM_RampUtils::ValueData::parm() const { return myParm; }

inline int
PRM_RampUtils::ValueData::vectorIndex() const { return myVectorIndex; }

inline PRM_RampUtils::PRM_ValueType
PRM_RampUtils::ValueData::valueType() const { return myValueType; }

inline int
PRM_RampUtils::ValueData::intValue() const { return myIntValue; }

inline fpreal
PRM_RampUtils::ValueData::floatValue() const { return myFloatValue; }

inline PRM_AddKeyType
PRM_RampUtils::ValueData::addKey() const { return myAddKey; }

#endif
