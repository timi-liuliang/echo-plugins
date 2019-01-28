/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Options.h (C++, Utility Library)
 *
 * COMMENTS:	Subclass of UT_Options to provide some special functionality
 * 		used by the SIM library.
 *
 */

#ifndef _OP_OPTIONS_H_
#define _OP_OPTIONS_H_

#include "OP_API.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_Options.h>
#include <SYS/SYS_Types.h>

class OP_Node;
class PRM_Template;
class PRM_Parm;

/// This class stores a set of name/value pairs. Each value can be one of
/// several different data types. No automatic conversion between data types
/// is provided, so accessing existing data requires knowledge of both the
/// data name and the data type.
class OP_API OP_Options : public UT_Options
{
public:
    explicit		 OP_Options();
    virtual		~OP_Options();

    /// Set the data in this OP_Options to be an interpolation of the data
    /// in two other OP_Options. The actual interpolation of each piece of
    /// data is performed by the OP_OptionInterpolator, which allows
    /// the interpolation behavior to be customized.
    void		 interpolate(const OP_Options &source1,
				     const OP_Options &source2,
				     fpreal interp);


    /// Utility function for setting an option based on a parm template.
    /// This is useful for gathering information from operator parameters
    /// to be passed on to OP_Data::setParameters().
    /// This will harden op references to full paths, add parameter
    /// tuples as vector2/vector3/etc options, and also build ramp
    /// and multiparms.  Ramps become strings and multiparms are
    /// recursed and all their implied parms added.
    void		 setOptionFromTemplate(const OP_Node *node,
					       const PRM_Template &tplate,
					       int pi, fpreal time);
    void		 setOptionFromTemplate(const OP_Node *node,
					       const PRM_Parm &parm,
					       const PRM_Template &tplate,
					       fpreal time);

    /// Utility function for setting options based on channel names of a
    /// parameter.  This splits up a single parameter tuple into multiple
    /// scalar options.
    void		setOptionFromParm(const OP_Node *node, int pi, int vi,
						fpreal time);

    /// Utility function for setting an options based on the default value
    /// stored in a parm template. This is used to gather default values
    /// for initializing a OP_Data type. The parameter defaults are
    /// gethered into a OP_Options data structure which is passed to
    /// OP_Data::setParameters().
    void		 setOptionFromTemplateDefault(const PRM_Template &tp);

    /// Sets an option based on strings coming from a command. This is
    /// used by the dopsolvesetoption command. This funtion returns true
    /// if the parameters make sense together and result in setting an
    /// option. Otherwise false is returned.
    bool		 setOptionFromCommand(const char *optionname,
					      const char *optionvalue,
					      fpreal optionvaluefloat,
					      UT_OptionType optiontype);

    /// Calculates a transform matrix based on standard transform options
    /// like t, r, s, etc.
    UT_DMatrix4		 getTransformFromOptions() const;

    /// Set one OP_Options equal to another.
    const OP_Options	&operator=(const UT_Options &src);

protected:
    /// This function is called whenever data is added, removed, or changed.
    /// If the name parameter is null, then multiple pieces of data were
    /// changed simultaneously (probably through a load or assignment
    /// operation). This function just calls OP_OptionsUser::optionChanged()
    /// on each user of this data.
    virtual void	 optionChanged(const char *name);

    /// These functions perform the actual interpolation of each data type.
    void	 getInterpolatedInt(int64 &value,
				int source1,
				int source2,
				fpreal interp) const;
    void	 getInterpolatedBool(bool &value,
				bool source1,
				bool source2,
				fpreal interp) const;
    void	 getInterpolatedFpreal(fpreal64 &value,
				fpreal source1,
				fpreal source2,
				fpreal interp) const;
    void	 getInterpolatedString(UT_StringHolder &value,
				const UT_StringHolder &source1,
				const UT_StringHolder &source2,
				fpreal interp) const;
    void	 getInterpolatedVector2(UT_Vector2D &value,
				const UT_Vector2D &source1,
				const UT_Vector2D &source2,
				fpreal interp) const;
    void	 getInterpolatedVector3(UT_Vector3D &value,
				const UT_Vector3D &source1,
				const UT_Vector3D &source2,
				fpreal interp) const;
    void	 getInterpolatedVector4(UT_Vector4D &value,
				const UT_Vector4D &source1,
				const UT_Vector4D &source2,
				fpreal interp) const;
    void	 getInterpolatedQuaternion(UT_QuaternionD &value,
				const UT_QuaternionD &source1,
				const UT_QuaternionD &source2,
				fpreal interp) const;
    void	 getInterpolatedMatrix2(UT_Matrix2D &value,
				const UT_Matrix2D &source1,
				const UT_Matrix2D &source2,
				fpreal interp) const;
    void	 getInterpolatedMatrix3(UT_Matrix3D &value,
				const UT_Matrix3D &source1,
				const UT_Matrix3D &source2,
				fpreal interp) const;
    void	 getInterpolatedMatrix4(UT_Matrix4D &value,
				const UT_Matrix4D &source1,
				const UT_Matrix4D &source2,
				fpreal interp) const;
    void	 getInterpolatedIntArray(UT_Int64Array &value,
				const UT_Int64Array &source1,
				const UT_Int64Array &source2,
				fpreal interp);
    void	 getInterpolatedFprealArray(UT_Fpreal64Array &value,
				const UT_Fpreal64Array &source1,
				const UT_Fpreal64Array &source2,
				fpreal interp);
    void	 getInterpolatedStringArray(UT_StringArray &value,
				const UT_StringArray &source1,
				const UT_StringArray &source2,
				fpreal interp);

private:
    explicit		 OP_Options(const OP_Options &src);
};

#endif
