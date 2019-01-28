/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_OptionsUser_h__
#define __SIM_OptionsUser_h__

#include "SIM_API.h"
#include "SIM_Options.h"

#include <UT/UT_IStream.h>
#include <UT/UT_Ramp.h>

class SIM_Data;

/// This class can be included as a base class of any SIM_Data subclass.
/// Doing so provides an easy way to establish and store a set of simple
/// values that define that data type. The macros at the end of this file
/// make it easy to create get and set access functions for this data.
class SIM_API SIM_OptionsUser
{
public:
			 SIM_OptionsUser(SIM_Data *owner);
    virtual		~SIM_OptionsUser();

    /// This callback function that is run by SIM_Options::optionChanged().
    /// It calls the virtual optionChangedSubclass().
    void		 optionChanged(const char *name);

    const SIM_Options	&getOptions() const;

protected:
    SIM_Options		&getOptions();

    virtual void	optionChangedSubclass(const char *name);

private:
    SIM_Data		*myOwner;
    SIM_Options		 myOptions;
};

#define GET_DATA_FUNC_I_sh(DataName, FuncName) \
	int64			get##FuncName() const \
	{ return getOptions().getOptionI(DataName); }
#define GET_DATA_FUNC_I(DataName, FuncName) \
	int64			get##FuncName() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return getOptions().getOptionI(dname.asRef()); }
#define GET_DATA_FUNC_E_sh(DataName, FuncName, EnumType) \
	EnumType		get##FuncName() const \
	{ return static_cast<EnumType> \
		 (getOptions().getOptionI(DataName)); }
#define GET_DATA_FUNC_E(DataName, FuncName, EnumType) \
	EnumType		get##FuncName() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return static_cast<EnumType> \
                 (getOptions().getOptionI(dname.asRef())); }
#define GET_DATA_FUNC_B_sh(DataName, FuncName) \
	bool			get##FuncName() const \
	{ return getOptions().getOptionB(DataName); }
#define GET_DATA_FUNC_B(DataName, FuncName) \
	bool			get##FuncName() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return getOptions().getOptionB(dname.asRef()); }
#define GET_DATA_FUNC_F_sh(DataName, FuncName) \
	fpreal64		get##FuncName() const \
	{ return getOptions().getOptionF(DataName); }
#define GET_DATA_FUNC_F(DataName, FuncName) \
	fpreal64		get##FuncName() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return getOptions().getOptionF(dname.asRef()); }

#define GET_DATA_FUNC_RAMP_sh(DataName, FuncName) \
	bool get##FuncName(UT_Ramp &ramp) const \
	{   UT_StringHolder rs;		\
	    rs = getOptions().getOptionS(DataName); \
	    UT_IStream	is((const char *) rs, rs.length(), UT_ISTREAM_ASCII); \
	    return ramp.load(is); \
	}
#define GET_DATA_FUNC_RAMP(DataName, FuncName) \
	bool get##FuncName(UT_Ramp &ramp) const \
	{   static constexpr UT_StringLit dname(DataName); \
	    UT_StringHolder rs;		\
	    rs = getOptions().getOptionS(dname.asRef()); \
	    UT_IStream	is((const char *) rs, rs.length(), UT_ISTREAM_ASCII); \
	    return ramp.load(is); \
	}

#define _BUILD_SIM_OPTION_GETTERS(DataName, FuncName, BaseType, Accessor) \
	const BaseType##D get##FuncName##D() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return getOptions().getOption##Accessor(dname.asRef()); } \
	const BaseType##F get##FuncName##F() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return getOptions().getOption##Accessor(dname.asRef()); } \
	const BaseType##F get##FuncName() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return getOptions().getOption##Accessor(dname.asRef()); }

#define _BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, BaseType, Accessor) \
	const BaseType##D get##FuncName##D() const \
	{ return getOptions().getOption##Accessor(DataName); } \
	const BaseType##F get##FuncName##F() const \
	{ return getOptions().getOption##Accessor(DataName); } \
	const BaseType##F get##FuncName() const \
	{ return getOptions().getOption##Accessor(DataName); }

#define GET_DATA_FUNC_V2(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector2, V2);
#define GET_DATA_FUNC_V2D(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector2D, V2);
#define GET_DATA_FUNC_UV(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector2, V2);
#define GET_DATA_FUNC_V3(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector3, V3);
#define GET_DATA_FUNC_V3D(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector3D, V3);
#define GET_DATA_FUNC_UVW(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector3, V3);
#define GET_DATA_FUNC_V4(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector4, V4);
#define GET_DATA_FUNC_V4D(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Vector4D, V4);
#define GET_DATA_FUNC_Q(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Quaternion, Q);
#define GET_DATA_FUNC_QD(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_QuaternionD, Q);
#define GET_DATA_FUNC_M3(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Matrix3, M3);
#define GET_DATA_FUNC_M4(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS(DataName, FuncName, UT_Matrix4, M4);

#define GET_DATA_FUNC_V2_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector2, V2);
#define GET_DATA_FUNC_V2D_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector2D, V2);
#define GET_DATA_FUNC_UV_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector2, V2);
#define GET_DATA_FUNC_V3_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector3, V3);
#define GET_DATA_FUNC_V3D_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector3D, V3);
#define GET_DATA_FUNC_UVW_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector3, V3);
#define GET_DATA_FUNC_V4_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector4, V4);
#define GET_DATA_FUNC_V4D_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Vector4D, V4);
#define GET_DATA_FUNC_Q_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Quaternion, Q);
#define GET_DATA_FUNC_QD_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_QuaternionD, Q);
#define GET_DATA_FUNC_M3_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Matrix3, M3);
#define GET_DATA_FUNC_M4_sh(DataName, FuncName) \
	_BUILD_SIM_OPTION_GETTERS_sh(DataName, FuncName, UT_Matrix4, M4);

#define GET_DATA_FUNC_S_sh(DataName, FuncName) \
	const UT_StringHolder &get##FuncName() const \
	{ return getOptions().getOptionS(DataName); } \
	void			get##FuncName(UT_String &value) const \
	{ value = get##FuncName(); }
#define GET_DATA_FUNC_S(DataName, FuncName) \
	const UT_StringHolder &get##FuncName() const \
	{ static constexpr UT_StringLit dname(DataName); \
	  return getOptions().getOptionS(dname.asRef()); } \
	void                   get##FuncName(UT_String &value) const \
	{ value = get##FuncName(); }

#define _BUILD_SIM_OPTION_GUIDE_GETTERS(DataName, FuncName, Default, BaseType, Accessor) \
	const BaseType##D get##FuncName##D(const SIM_Options &options) const \
	{ return options.hasOption(DataName) \
	    ? options.getOption##Accessor(DataName) \
	    : BaseType##D Default; \
	}	\
	const BaseType##F get##FuncName##F(const SIM_Options &options) const \
	{ return options.hasOption(DataName) \
	    ? options.getOption##Accessor(DataName) \
	    : BaseType##D Default; \
	}	\
	const BaseType##F get##FuncName(const SIM_Options &options) const \
	{ return options.hasOption(DataName) \
	    ? options.getOption##Accessor(DataName) \
	    : BaseType##D Default; \
	}

#define GET_GUIDE_FUNC_I(DataName, FuncName, Default) \
	int64		get##FuncName(const SIM_Options &options) const \
	{ return options.hasOption(DataName) \
	    ? options.getOptionI(DataName) \
	    : Default; \
	}
#define GET_GUIDE_FUNC_E(DataName, FuncName, EnumType, Default) \
	EnumType	get##FuncName(const SIM_Options &options) const \
	{ return static_cast<EnumType> (options.hasOption(DataName) \
	    ? options.getOptionI(DataName) \
	    : Default); \
	}
#define GET_GUIDE_FUNC_B(DataName, FuncName, Default) \
	bool		get##FuncName(const SIM_Options &options) const \
	{ return options.hasOption(DataName) \
	    ? options.getOptionB(DataName) \
	    : Default; \
	}
#define GET_GUIDE_FUNC_F(DataName, FuncName, Default) \
	fpreal64	get##FuncName(const SIM_Options &options) const \
	{ return options.hasOption(DataName) \
	    ? options.getOptionF(DataName) \
	    : Default; \
	}
#define GET_GUIDE_FUNC_V2(DataName, FuncName, Default) \
	_BUILD_SIM_OPTION_GUIDE_GETTERS(DataName, FuncName, Default, UT_Vector2, V2)
#define GET_GUIDE_FUNC_V3(DataName, FuncName, Default) \
	_BUILD_SIM_OPTION_GUIDE_GETTERS(DataName, FuncName, Default, UT_Vector3, V3)
#define GET_GUIDE_FUNC_V4(DataName, FuncName, Default) \
	_BUILD_SIM_OPTION_GUIDE_GETTERS(DataName, FuncName, Default, UT_Vector4, V4)

#define GET_GUIDE_FUNC_S(DataName, FuncName, Default) \
	void get##FuncName(UT_String &value, const SIM_Options &options) const \
	{ if (options.hasOption(DataName)) \
	    options.getOptionS(DataName, value); \
	  else \
	    value = Default; \
	}

#define GET_GUIDE_FUNC_RAMP(DataName, FuncName) \
	bool get##FuncName(UT_Ramp &ramp, const SIM_Options &options) const \
	{ if (options.hasOption(DataName)) \
	    { UT_String rs;		\
	    options.getOptionS(DataName, rs); \
	    UT_IStream	is((const char *) rs, rs.length(), UT_ISTREAM_ASCII); \
	    return ramp.load(is); } \
	 return false; \
	}

#define SET_DATA_FUNC_I(DataName, FuncName) \
	void			set##FuncName(const int64 value) \
	{ getOptions().setOptionI(DataName, value); }
#define SET_DATA_FUNC_E(DataName, FuncName, EnumType) \
	void			set##FuncName(const EnumType value) \
	{ getOptions().setOptionI(DataName, \
		 static_cast<int>(value)); }
#define SET_DATA_FUNC_B(DataName, FuncName) \
	void			set##FuncName(const bool value) \
	{ getOptions().setOptionB(DataName, value); }
#define SET_DATA_FUNC_F(DataName, FuncName) \
	void			set##FuncName(const fpreal64 value) \
	{ getOptions().setOptionF(DataName, value); }
#define SET_DATA_FUNC_V2(DataName, FuncName) \
	void			set##FuncName(const UT_Vector2F &value) \
	{ getOptions().setOptionV2(DataName, value); } \
	void			set##FuncName(const UT_Vector2D &value) \
	{ getOptions().setOptionV2(DataName, value); }
#define SET_DATA_FUNC_UV(DataName, FuncName) \
	void			set##FuncName(const UT_Vector2F &value) \
	{ getOptions().setOptionUV(DataName, value); } \
	void			set##FuncName(const UT_Vector2D &value) \
	{ getOptions().setOptionUV(DataName, value); }
#define SET_DATA_FUNC_V3(DataName, FuncName) \
	void			set##FuncName(const UT_Vector3F &value) \
	{ getOptions().setOptionV3(DataName, value); } \
	void			set##FuncName(const UT_Vector3D &value) \
	{ getOptions().setOptionV3(DataName, value); }
#define SET_DATA_FUNC_UVW(DataName, FuncName) \
	void			set##FuncName(const UT_Vector3F &value) \
	{ getOptions().setOptionUVW(DataName, value); } \
	void			set##FuncName(const UT_Vector3D &value) \
	{ getOptions().setOptionUVW(DataName, value); }
#define SET_DATA_FUNC_V4(DataName, FuncName) \
	void			set##FuncName(const UT_Vector4F &value) \
	{ getOptions().setOptionV4(DataName, value); } \
	void			set##FuncName(const UT_Vector4D &value) \
	{ getOptions().setOptionV4(DataName, value); }
#define SET_DATA_FUNC_Q(DataName, FuncName) \
	void			set##FuncName(const UT_QuaternionF &value) \
	{ getOptions().setOptionQ(DataName, value); } \
	void			set##FuncName(const UT_QuaternionD &value) \
	{ getOptions().setOptionQ(DataName, value); }
#define SET_DATA_FUNC_M3(DataName, FuncName) \
	void			set##FuncName(const UT_Matrix3F &value) \
	{ getOptions().setOptionM3(DataName, value); } \
	void			set##FuncName(const UT_Matrix3D &value) \
	{ getOptions().setOptionM3(DataName, value); }
#define SET_DATA_FUNC_M4(DataName, FuncName) \
	void			set##FuncName(const UT_Matrix4F &value) \
	{ getOptions().setOptionM4(DataName, value); } \
	void			set##FuncName(const UT_Matrix4D &value) \
	{ getOptions().setOptionM4(DataName, value); }
#define SET_DATA_FUNC_S(DataName, FuncName) \
	void			set##FuncName(const UT_StringHolder &value) \
	{ getOptions().setOptionS(DataName, value); }

#define SET_DATA_FUNC_RAMP(DataName, FuncName) \
	void set##FuncName(const UT_Ramp &ramp) const \
	{   UT_OStringStream	os; \
	    ramp.save(os); \
	    getOptions().setOptionS(DataName, os.str().buffer()); \
	}

#define GETSET_DATA_FUNCS_I(DataName, FuncName) \
	GET_DATA_FUNC_I(DataName, FuncName) \
	SET_DATA_FUNC_I(DataName, FuncName)
#define GETSET_DATA_FUNCS_E(DataName, FuncName, EnumType) \
	GET_DATA_FUNC_E(DataName, FuncName, EnumType) \
	SET_DATA_FUNC_E(DataName, FuncName, EnumType)
#define GETSET_DATA_FUNCS_B(DataName, FuncName) \
	GET_DATA_FUNC_B(DataName, FuncName) \
	SET_DATA_FUNC_B(DataName, FuncName)
#define GETSET_DATA_FUNCS_F(DataName, FuncName) \
	GET_DATA_FUNC_F(DataName, FuncName) \
	SET_DATA_FUNC_F(DataName, FuncName)
#define GETSET_DATA_FUNCS_V2(DataName, FuncName) \
	GET_DATA_FUNC_V2(DataName, FuncName) \
	SET_DATA_FUNC_V2(DataName, FuncName)
#define GETSET_DATA_FUNCS_UV(DataName, FuncName) \
	GET_DATA_FUNC_UV(DataName, FuncName) \
	SET_DATA_FUNC_UV(DataName, FuncName)
#define GETSET_DATA_FUNCS_V3(DataName, FuncName) \
	GET_DATA_FUNC_V3(DataName, FuncName) \
	SET_DATA_FUNC_V3(DataName, FuncName)
#define GETSET_DATA_FUNCS_UVW(DataName, FuncName) \
	GET_DATA_FUNC_UVW(DataName, FuncName) \
	SET_DATA_FUNC_UVW(DataName, FuncName)
#define GETSET_DATA_FUNCS_V4(DataName, FuncName) \
	GET_DATA_FUNC_V4(DataName, FuncName) \
	SET_DATA_FUNC_V4(DataName, FuncName)
#define GETSET_DATA_FUNCS_Q(DataName, FuncName) \
	GET_DATA_FUNC_Q(DataName, FuncName) \
	SET_DATA_FUNC_Q(DataName, FuncName)
#define GETSET_DATA_FUNCS_M3(DataName, FuncName) \
	GET_DATA_FUNC_M3(DataName, FuncName) \
	SET_DATA_FUNC_M3(DataName, FuncName)
#define GETSET_DATA_FUNCS_M4(DataName, FuncName) \
	GET_DATA_FUNC_M4(DataName, FuncName) \
	SET_DATA_FUNC_M4(DataName, FuncName)
#define GETSET_DATA_FUNCS_S(DataName, FuncName) \
	GET_DATA_FUNC_S(DataName, FuncName) \
	SET_DATA_FUNC_S(DataName, FuncName)
#define GETSET_DATA_FUNCS_RAMP(DataName, FuncName) \
	GET_DATA_FUNC_RAMP(DataName, FuncName) \
	SET_DATA_FUNC_RAMP(DataName, FuncName)

#define GETSET_DATA_FUNCS_I_sh(DataName, FuncName) \
	GET_DATA_FUNC_I_sh(DataName, FuncName) \
	SET_DATA_FUNC_I(DataName, FuncName)
#define GETSET_DATA_FUNCS_E_sh(DataName, FuncName, EnumType) \
	GET_DATA_FUNC_E_sh(DataName, FuncName, EnumType) \
	SET_DATA_FUNC_E(DataName, FuncName, EnumType)
#define GETSET_DATA_FUNCS_B_sh(DataName, FuncName) \
	GET_DATA_FUNC_B_sh(DataName, FuncName) \
	SET_DATA_FUNC_B(DataName, FuncName)
#define GETSET_DATA_FUNCS_F_sh(DataName, FuncName) \
	GET_DATA_FUNC_F_sh(DataName, FuncName) \
	SET_DATA_FUNC_F(DataName, FuncName)
#define GETSET_DATA_FUNCS_V2_sh(DataName, FuncName) \
	GET_DATA_FUNC_V2_sh(DataName, FuncName) \
	SET_DATA_FUNC_V2(DataName, FuncName)
#define GETSET_DATA_FUNCS_UV_sh(DataName, FuncName) \
	GET_DATA_FUNC_UV_sh(DataName, FuncName) \
	SET_DATA_FUNC_UV(DataName, FuncName)
#define GETSET_DATA_FUNCS_V3_sh(DataName, FuncName) \
	GET_DATA_FUNC_V3_sh(DataName, FuncName) \
	SET_DATA_FUNC_V3(DataName, FuncName)
#define GETSET_DATA_FUNCS_UVW_sh(DataName, FuncName) \
	GET_DATA_FUNC_UVW_sh(DataName, FuncName) \
	SET_DATA_FUNC_UVW(DataName, FuncName)
#define GETSET_DATA_FUNCS_V4_sh(DataName, FuncName) \
	GET_DATA_FUNC_V4_sh(DataName, FuncName) \
	SET_DATA_FUNC_V4(DataName, FuncName)
#define GETSET_DATA_FUNCS_Q_sh(DataName, FuncName) \
	GET_DATA_FUNC_Q_sh(DataName, FuncName) \
	SET_DATA_FUNC_Q(DataName, FuncName)
#define GETSET_DATA_FUNCS_M3_sh(DataName, FuncName) \
	GET_DATA_FUNC_M3_sh(DataName, FuncName) \
	SET_DATA_FUNC_M3(DataName, FuncName)
#define GETSET_DATA_FUNCS_M4_sh(DataName, FuncName) \
	GET_DATA_FUNC_M4_sh(DataName, FuncName) \
	SET_DATA_FUNC_M4(DataName, FuncName)
#define GETSET_DATA_FUNCS_S_sh(DataName, FuncName) \
	GET_DATA_FUNC_S_sh(DataName, FuncName) \
	SET_DATA_FUNC_S(DataName, FuncName)
#define GETSET_DATA_FUNCS_RAMP_sh(DataName, FuncName) \
	GET_DATA_FUNC_RAMP_sh(DataName, FuncName) \
	SET_DATA_FUNC_RAMP(DataName, FuncName)

#endif

