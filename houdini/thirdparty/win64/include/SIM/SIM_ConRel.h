/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConRel_h__
#define __SIM_ConRel_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

class UT_IStream;
class SIM_ConAnchor;
class SIM_Relationship;

/// This enumeration defines the error codes that can be set on a SIM_ConRel.
typedef enum {
    SIM_CONRELERR_NONE = 0,
    SIM_CONRELERR_UNPROCESSED = 1,
    SIM_CONRELERR_BADOBJECT = 2,
    SIM_CONRELERR_BADPOINT = 3,
    SIM_CONRELERR_BADPRIMITIVE = 4
} SIM_ConRelError;

/// Definition of the relationship between the anchors in a constraint -
/// i.e., how a constraint affects its anchors.
///
/// Also contains state information - the force applied to enforce the
/// constraint, and the distance separating the anchors. This can be very
/// useful for switching relationships in response to the constraint's
/// performance.
class SIM_API SIM_ConRel : public SIM_Data
{
public:
    
    /// A utility class for getting values of the constraint options.
    /// An instance of this class is passed as an argument to buildConRelGude(..)
    ///	so that rendering of the constraint guide geometry can be done accuratly
    /// depending on who calls it.
    /// The diffuse attribute (color_attrib) is passed in for efficiency reasons,
    /// so that each call of buildConRelGuide does not need to call addDiffuseAttribute
    ///	on its own
    class ConRelData
    {
    public:
        ConRelData(const SIM_Data* op, const GA_RWHandleV3 &cd)
          : myOptions(dynamic_cast<const SIM_OptionsUser*>(op)),
            color_attrib(cd)
        {
        }
        virtual ~ConRelData() { }
        virtual bool getDataB(const UT_StringRef &name) const
        {
            return myOptions ? myOptions->getOptions().getOptionB(name) : 0;
        }
        virtual int getDataI(const UT_StringRef &name) const
        {
            return myOptions ? myOptions->getOptions().getOptionI(name) : 0;
        }
        virtual fpreal getDataF(const UT_StringRef &name) const
        {
            return myOptions ? myOptions->getOptions().getOptionF(name) : 0;
        }
        virtual UT_Vector3 getDataV3(const UT_StringRef &name) const
        {
            return myOptions ? myOptions->getOptions().getOptionV3(name) : UT_Vector3D(0,0,0);
        }
        virtual UT_Quaternion getDataQ(const UT_StringRef &name) const
        {
            return myOptions ? myOptions->getOptions().getOptionQ(name) : UT_QuaternionD(0,0,0,1);
        }
        virtual UT_Vector2 getDataV2(const UT_StringRef &name) const
        {
            return myOptions ? myOptions->getOptions().getOptionV2(name) : UT_Vector2D(0,0);
        }
        const GA_RWHandleV3 &getColorAttrib() const { return color_attrib; }
    protected:
        const SIM_OptionsUser* myOptions;
        GA_RWHandleV3 color_attrib;
    };

    SIM_ConRel		*getSubConRel();
    /// Be careful using the const version. It should really only be used
    /// when SIM_Constraint is generating guide geometry.
    const SIM_ConRel	*getConstSubConRel() const;
    void		 makeStateTransition(const SIM_Time &time);

    /// Functions to get and set state information.
    void		 setStateForce(fpreal force);
    fpreal		 getStateForce() const;
    void		 setStateDistance(fpreal distance);
    fpreal		 getStateDistance() const;

    /// A constraint is invalid if its parameters cannot be resolved.
    void		 setStateErrorCode(SIM_ConRelError errorcode);
    SIM_ConRelError	 getStateErrorCode() const;

    /// Build guide geometry for the conrel, and also get the anchors to build
    /// their own guide geometry.
    void		 buildConRelGuideGeometry(
				const GU_DetailHandle &gdh,
				const SIM_Time &time,
				const SIM_Relationship &rel,
				const SIM_ConAnchor &anchor1,
				const SIM_ConAnchor &anchor2) const;
    virtual void	 buildConRelGuide(
				const GU_DetailHandle &gdh,
				const SIM_Options &options,
				const UT_Vector3 &anchor_pos_1,
				const UT_Vector3 &anchor_pos_2,
				const UT_Quaternion &anchor_rot_1,
				const UT_Quaternion &anchor_rot_2,
                                const ConRelData &c_data) const;
protected:
    explicit		 SIM_ConRel(const SIM_DataFactory *factory);
    virtual		~SIM_ConRel();

    virtual void	 initializeSubclass();
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    virtual void	 saveSubclass(std::ostream &os) const;
    virtual bool	 loadSubclass(UT_IStream &is);

    virtual SIM_Query	*createQueryObjectSubclass() const;

    virtual SIM_ConRel	*getSubConRelSubclass();
    virtual const SIM_ConRel
			*getConstSubConRelSubclass() const;
    virtual void	 makeStateTransitionSubclass(const SIM_Time &time);
    virtual void	 setStateForceSubclass(fpreal force);
    virtual fpreal	 getStateForceSubclass() const;
    virtual void	 setStateDistanceSubclass(fpreal distance);
    virtual fpreal	 getStateDistanceSubclass() const;
    virtual void	 buildConRelGuideGeometrySubclass(
				const GU_DetailHandle &gdh,
				const SIM_Time &time,
				const SIM_Relationship &rel,
				const SIM_ConAnchor &anchor1,
				const SIM_ConAnchor &anchor2) const;

private:
    /// Store the error code that is part of our state.
    SIM_ConRelError	 myStateErrorCode;
    /// This value gets set to true when we are calling makeStateTransition
    /// to avoid infinite recursions.
    bool		 myMakingStateTransition;
    /// This value gets set to true when we are calling getSubConRel
    /// to avoid infinite recursions.
    mutable bool	 myGettingConRel;
    
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConRel, SIM_Data);
};

#endif
