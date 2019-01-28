/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        OBJ_Muscle.h (Custom Library, C++)
 *
 * COMMENTS:    An object that creates muscle geometry.  Essentially
 *		just a container for the muscle SOP.
 */

#ifndef __OBJ_Muscle__
#define __OBJ_Muscle__

#include <CH/CH_Channel.h>
#include <UT/UT_Array.h>
#include <UT/UT_Vector3Array.h>
#include "OBJ_API.h"
#include "OBJ_Geometry.h"

class SOP_Muscle;
class UT_String;
class GEO_PrimPoly;

enum OBJ_MuscleIndex {
    // Muscle tab
    I_MUSCLE_NAME = I_N_GEO_INDICES,
    I_MUSCLE_RESTANCHOR,
    I_MUSCLE_POSITIONBIAS,
    I_MUSCLE_SCALE,

    // Muscle/Controls tab
    I_MUSCLE_HANDLEHEADING,
    I_MUSCLE_PRIMSPERSEGMENT,
    I_MUSCLE_CONTROL1,
    I_MUSCLE_CONTROL1SCALE,
    I_MUSCLE_SEPARATOR1,
    I_MUSCLE_CONTROL2,
    I_MUSCLE_CONTROL2SCALE,
    I_MUSCLE_CONTROL2POS,
    I_MUSCLE_SEPARATOR2,
    I_MUSCLE_CONTROL3,
    I_MUSCLE_CONTROL3SCALE,
    I_MUSCLE_CONTROL3POS,
    I_MUSCLE_SEPARATOR3,
    I_MUSCLE_CONTROL4,
    I_MUSCLE_CONTROL4SCALE,
    I_MUSCLE_CONTROL4POS,
    I_MUSCLE_SEPARATOR4,
    I_MUSCLE_CONTROL5,
    I_MUSCLE_CONTROL5SCALE,

    I_MUSCLE_ENDSEPARATOR,

    // Preferences tab
    I_MUSCLE_TYPE,
    I_MUSCLE_DOCOLOR,
    I_MUSCLE_COLOR,
    I_MUSCLE_ENABLEGRADIENT,
    I_MUSCLE_GRADIENT,

    // Center line tab
    I_MUSCLE_BASIS,
    I_MUSCLE_SAMPLEDIVS,
    I_MUSCLE_FIRSTCV,
    I_MUSCLE_LASTCV,
    I_MUSCLE_TENSION,

    I_N_MUSCLE_INDICES
};

class OBJ_API OBJ_Muscle : public OBJ_Geometry
{
public:
				 OBJ_Muscle(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Muscle();

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		*getTemplateList();

    virtual OBJ_OBJECT_TYPE	 getObjectType() const { return OBJ_STD_MUSCLE; }

    // Override this method so that we know when our input
    // connections have been modified, and so we can
    // update our child SOP muscle accordingly
    virtual void		 inputConnectChanged(int which);

    bool                         getTranslates(OP_Context &context,
                                               UT_Array<UT_Matrix4>
                                                       &translates);

    bool                         getRotates(OP_Context &context,
                                            UT_Array<UT_Matrix4> &rotates);

    const GEO_PrimPoly          *getCenterCurve(OP_Context &context);

    void                         getCurrColorValues(UT_Vector3 &color);

protected:
    virtual bool	 	 updateParmsFlags();
    
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return muscleIndirect; }

    virtual int			 isObjectRenderable(fpreal t) const;

    virtual OP_ERROR		 cookMyObj(OP_Context &context);

    // Override this method so that the profile channels can be checked, and
    // can indicate to the child node if they have changed
    virtual void		 checkChannelDependencies(
					    CH_Channel *ch,
					    CH_CHANGE_TYPE reason);

private:
    // A function used to return our child SOP muscle,
    // if it can be found
    SOP_Muscle			*getSOPMuscle();

    void			verifyNames();

    // Paramter retrieval functions
    bool			 DOPROFILES()
				 {
				     return evalInt("profiletoggle", 0, 0);
				 }

    bool			 DOCOLOR()
				 {
				     return evalInt("togglecolor", 0, 0);
				 }

    bool			 DOGRADIENT()
				 {
				     return evalInt("enablegradient", 0, 0);
				 }

    void			 MUSCLENAME(UT_String &str)
			         {
				    evalString(str, "musclename", 0, 0);
			         }

    bool			 USEHANDLES()
				 {
				     return (evalInt("typeswitcher", 0, 0) == 1);
				 }

private:
    static int			*muscleIndirect;
    
    UT_Array<UT_String>	 myInputNames;

    // Stores the total number of inputs for this object
    int				 myNumInputs;

    // Stores the total number of actual connected
    // inputs for this object
    int				 myNumConnectedInputs;

};

#endif
