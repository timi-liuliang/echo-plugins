/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Muscle.h (SOP Library, C++)
 *
 * COMMENTS: A SOP that generates a muscle object using a 
 *	     metaball surface.
 *
 */

#ifndef __SOP_Muscle_h__
#define __SOP_Muscle_h__

#include "SOP_API.h"
#include <CH/CH_Channel.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_Shared.h>
#include <PRM/PRM_Name.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_FloatArray.h>
#include "SOP_Node.h"

enum {
    SOP_MUSCLE_SWITCHER = 0,

#define SOP_MUSCLE_NUM_MUSCLE_PARMS (25)
    SOP_MUSCLE_NAME,
    SOP_MUSCLE_RESTANCHOR,
    SOP_MUSCLE_POSITIONBIAS,
    SOP_MUSCLE_SCALE,

#define SOP_MUSCLE_NUM_MODEL_PARMS (17)
    SOP_MUSCLE_HANDLEHEADING,

    SOP_MUSCLE_PRIMSPERSEGMENT,

    SOP_MUSCLE_CONTROL1,
    SOP_MUSCLE_CONTROL1SCALE,
    SOP_MUSCLE_SEPARATOR1,

    SOP_MUSCLE_CONTROL2,
    SOP_MUSCLE_CONTROL2SCALE,
    SOP_MUSCLE_CONTROL2POS,
    SOP_MUSCLE_SEPARATOR2,

    SOP_MUSCLE_CONTROL3,
    SOP_MUSCLE_CONTROL3SCALE,
    SOP_MUSCLE_CONTROL3POS,
    SOP_MUSCLE_SEPARATOR3,

    SOP_MUSCLE_CONTROL4,
    SOP_MUSCLE_CONTROL4SCALE,
    SOP_MUSCLE_CONTROL4POS,
    SOP_MUSCLE_SEPARATOR4,

    SOP_MUSCLE_CONTROL5,
    SOP_MUSCLE_CONTROL5SCALE,

    SOP_MUSCLE_ENDSEPARATOR,
    
    SOP_MUSCLE_NUMINPUTS,

#define SOP_MUSCLE_NUM_PREFERENCE_PARMS (5)
    SOP_MUSCLE_TYPE,
    SOP_MUSCLE_COLORTOGGLE,
    SOP_MUSCLE_COLOR,
    SOP_MUSCLE_ENABLEGRADIENT,
    SOP_MUSCLE_GRADIENT,

#define SOP_MUSCLE_NUM_CENTERLINE_PARMS (5)
    SOP_MUSCLE_BASIS,
    SOP_MUSCLE_SAMPLEDIVS,
    SOP_MUSCLE_FIRSTCV,
    SOP_MUSCLE_LASTCV,
    SOP_MUSCLE_CVTENSION,

    SOP_MUSCLE_NUMPARMS
};

// Display types
enum SOP_MuscleType {
    SOP_MUSCLE_META = 0,
    SOP_MUSCLE_SPHERE
};

// Contro types
enum sop_MuscleControlType {
    SOP_MUSCLE_PROFILES = 0,
    SOP_MUSCLE_HANDLES
};

#define NUM_SEGMENTS    4
#define DEFAULT_CENTER_GROUP "center"

class GU_DetailHandle;
class GU_PrimPoly;
class GU_MuscleParms;
class GU_Muscle;

class UT_String;

class SOP_API SOP_Muscle : public SOP_Node
{
public:
	    SOP_Muscle(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Muscle();

    virtual bool		     updateParmsFlags();

    static OP_Node		    *myConstructor(OP_Network *net, const char *name,
						   OP_Operator *entry);

    static PRM_Template		     myTemplateList[];

    // A function used to indicate to this node that its color gradient
    // channel has been changed
    void			     changeGradient() 
					{ myGradientChanged = true; }
    
    // Indicate to this node that the input names have been changed,
    // so that we can update myAnchorInputs
    void                             changeInputs();
   
    bool                             getTranslates(OP_Context &context,
                                        UT_Array<UT_Matrix4> &translates);

    bool                             getRotates(OP_Context &context,
                                         UT_Array<UT_Matrix4> &rotates);

    const GEO_PrimPoly              *getCenterCurve(OP_Context &context);

    void                             getCurrColorValues(UT_Vector3 &color)
                                     { 
                                         color[0] = myCurrColorValuesRGB[0];
                                         color[1] = myCurrColorValuesRGB[1];
                                         color[2] = myCurrColorValuesRGB[2];
                                     }

protected:
    virtual OP_ERROR		     cookMySop(OP_Context &context);
    virtual const char		    *inputLabel(unsigned idx) const;

    // Overridden function so that we can determine whether or not
    // any of the profile changes have been modified prior to a cook
    virtual void		     checkChannelDependencies(
						CH_Channel *ch,
						CH_CHANGE_TYPE reason);

private:

    // A function used to set up extra inputs based on the specified
    // muscle anchor points.  Also modifieds numInputs to store the
    // actual number of inputs added
    OP_ERROR			     setupInputs(int numInputs);

    // A function which is called when a failure occurs during cooking
    void			     cookFailed();

    // A function that sets up color gradient values, so that we don't
    // have to evaluate the parameter 20 times every time we cook.
    void			     setupGradientValues(int numMeta, fpreal t);

    // Verify that the muscle name specified is proper.
    void			     verifyNames();

    void                             setupMuscleParms(GU_MuscleParms &parms,
                                                      fpreal &t);
   
    bool                             getInputsChanged();

    GU_Muscle                       *getGUMuscle();

    // Functions for retrieving user-specified parameters
    int				     NUMINPUTS()
				     {
					 return evalInt(SOP_MUSCLE_NUMINPUTS, 0, 0);
				     }

    void			     INPUTSTRING(int i, UT_String &str)
				     {
					 evalStringInst("inputname#", &i, str, 0, 0);
				     }

    void			     RESTANCHOR(UT_String &str)
				     {
					 evalString(str, SOP_MUSCLE_RESTANCHOR, 0, 0);
				     }
    
    void                             CENTERGROUP(UT_String &str)
                                     {
                                         str = DEFAULT_CENTER_GROUP;
                                     }

    float			     POSITIONBIAS()
				     {
					 return evalFloat(SOP_MUSCLE_POSITIONBIAS, 0, 0);
				     }

    SOP_MuscleType		     MUSCLETYPE()
				     {
					 int type = evalInt(SOP_MUSCLE_TYPE, 0, 0);

					 switch (type)
					 {
					     case SOP_MUSCLE_META:
						return SOP_MUSCLE_META;
					     case SOP_MUSCLE_SPHERE:
						return SOP_MUSCLE_SPHERE;
					     default:
						return SOP_MUSCLE_META;
					 }
				     }

    void			     MUSCLENAME(UT_String &str)
				     {
					 evalString(str, SOP_MUSCLE_NAME, 0, 0);
				     }

    bool			     DOCOLOR()
				     {
					 return evalInt(SOP_MUSCLE_COLORTOGGLE, 0, 0);
				     }

    float			     CR(fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_COLOR, 0, t);
				     }

    float			     CG(fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_COLOR, 1, t);
				     }

    float			     CB(fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_COLOR, 2, t);
				     }

    int				     BASIS()
				     {
					 return evalInt(SOP_MUSCLE_BASIS, 0, 0);
				     }

    int				     SAMPLEDIVS(fpreal t)
				     {
					 return evalInt(SOP_MUSCLE_SAMPLEDIVS, 0, t);
				     }

    int				     FIRSTCV(fpreal t)
				     {
					 return evalInt(SOP_MUSCLE_FIRSTCV, 0, t);
				     }

    int				     LASTCV(fpreal t)
				     {
					 return evalInt(SOP_MUSCLE_LASTCV, 0, t);
				     }

    float			     CVTENSION(fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CVTENSION, 0, t);
				     }

    bool			     DOGRADIENT()
				     {
					 return evalInt(SOP_MUSCLE_ENABLEGRADIENT, 0, 0);
				     }

    float			     GRADIENT(fpreal t)
				     {
					return evalFloat(SOP_MUSCLE_GRADIENT, 0, t);
				     }

    float			     CONTROL1SCALE(int i, fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL1SCALE, i, t);
				     }

    float			     CONTROL2SCALE(int i, fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL2SCALE, i, t);
				     }

    float			     CONTROL3SCALE(int i, fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL3SCALE, i, t);
				     }

    float			     CONTROL4SCALE(int i, fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL4SCALE, i, t);
				     }

    float			     CONTROL5SCALE(int i, fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL5SCALE, i, t);
				     }

    float			     CONTROL2POS(fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL2POS, 0, t);
				     }

    float			     CONTROL3POS(fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL3POS, 0, t);
				     }

    float			     CONTROL4POS(fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_CONTROL4POS, 0, t);
				     }

    int				     PRIMSPERSEGMENT()
				     {
					 return evalInt(SOP_MUSCLE_PRIMSPERSEGMENT, 0, 0);
				     }

    float			     MUSCLESCALE(int i, fpreal t)
				     {
					 return evalFloat(SOP_MUSCLE_SCALE, i, t);
				     }

private:

    // Stores the primitives (spheres or metaballs)
    // currently used by this muscle
    GA_PrimitiveGroup		    *myPrimitives;

    // Stores the number of metaballs we are currently
    // storing
    int				     myNumPrimitives;

    // Stores the centre curve for this muscle
    //GU_PrimPoly			    *myCenterCurve;

    // Stores the centre curve points for this
    // muscle
    GA_PointGroup		    *myCenterPoints;

    // An array storing the object inputs for the muscle
    UT_ValArray<OP_Node *>	     myAnchorInputs;

    // Stores SOP inputs for the muscle anchor points, if
    // they are to be used
    UT_ValArray<SOP_Node *>	     mySOPInputs;

    // An array of inputs storing the gdh's for our SOP inputs
    UT_Array<GU_DetailHandle>     myAnchorGdhs;

    // An array of gdps for our SOP inputs
    UT_ValArray<const GU_Detail *>   myAnchorGdps;

    // Cached input paths, so that we can avoid having to do
    // parameter evaluation every cook
    UT_Array<UT_String>	     myAnchorPaths;

    // Flags to indicate when the colour gradient has been changed
    // and needs to be updated
    bool			     myGradientChanged;

    // An array used to store gradient values
    UT_FloatArray		     myGradientValues;
    
    // Color values of the muscle as it was last cooked
    UT_Vector3                       myCurrColorValuesRGB;

    // A flag to indicate that this node is "dirty" - set after
    // a cook fails so that everything gets rebuilt in the next
    // cook
    bool			     myIsDirty;

    // Flag to indicate that the input names have changed - 
    // TODO: when it is actually used, clear at the end of each cook
    bool                             myInputsChanged;

    // The current muscle type
    SOP_MuscleType		     myCurrentType;

    // An extra gdp used to generate any intermediate geometry
    // required during a cook
    GU_Detail			    *myExtraGdp;
    
    GU_Muscle                       *myMuscle;

    GU_MuscleParms                  *myMuscleParms;
};

#endif
