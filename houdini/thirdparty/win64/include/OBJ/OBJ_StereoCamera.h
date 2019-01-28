/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_StereoCamera.h ( OBJ Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OBJ_StereoCamera__
#define __OBJ_StereoCamera__

#include "OBJ_API.h"
#include "OBJ_Camera.h"

enum OBJ_StereoCameraIndex
{
    I_LEFT_CAMERA = I_N_CAM_INDICES,
    I_RIGHT_CAMERA,

    I_N_STEREOCAM_INDICES		// should always be last in the list
};


class OBJ_API OBJ_StereoCamera : public OBJ_Camera
{
public:
    // constructor and destructor.
	     OBJ_StereoCamera(OP_Network *, const char *, OP_Operator *);
    virtual ~OBJ_StereoCamera();

    // node factory
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name, 
					       OP_Operator *entry);

    // a few standard methods for OBJs
    static PRM_Template		*getTemplateList();
    virtual OBJ_OBJECT_TYPE	 getObjectType() const;
    virtual OBJ_StereoCamera	*castToOBJStereoCamera();

    // Utility methods that obtain left and right camera objects
    OBJ_Node *			getLeftCamera( fpreal t );
    OBJ_Node *			getRightCamera( fpreal t );

protected:
    // Used to get pointer to indirection indices for each object type
    virtual int *		 getIndirect() const;

private:
    // the parameter index indirection (lookup table)
    static int			*theStereoCameraIndirect;
};

#endif
