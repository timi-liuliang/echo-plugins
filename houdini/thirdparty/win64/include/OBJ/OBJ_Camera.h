/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ library (C++)
 *
 * COMMENTS:	Camera object definition (base class for lights too)
 *
 */

#ifndef __OBJ_Camera_h__
#define __OBJ_Camera_h__

#include "OBJ_API.h"
#include "OBJ_Node.h"

#include <UT/UT_String.h>

enum OBJ_CamIndex
{
    I_N_CAM_INDICES = I_N_BASE_INDICES
};

// Projections for cameras
enum OBJ_ProjectionType
{
    OBJ_PROJ_PERSPECTIVE,
    OBJ_PROJ_ORTHO,
    OBJ_PROJ_POLAR,
    OBJ_PROJ_CYLINDRICAL
};


class OBJ_API OBJ_CameraParms
{
public:
    OBJ_CameraParms()	{ setDefaults(); }
    void		setDefaults();
    float		focal;			// Focal length
    float		aperture;		// Aperture
    float		aspect;			// Aspect ratio
    float		focus;			// Focus distance
    float		fstop;			// F-Stop;
    float		curvature;		// Lens curvature
    int			xres, yres;		// Resolution for camera
    float		cropx[2];		// Min max for cropping in x
    float		cropy[2];		// Min max for cropping in y
    float		winx[2];		// Sub-windowing in X
    float		winy[2];		// Sub-windowing in Y
    float		mynear, myfar;		// Near/Far clipping planes
    float		shutterTime;		// Shutter time for the camera
    OBJ_ProjectionType 	projection;
    float		orthoZoom;		// Orthographic zoom
};




class OBJ_API OBJ_Camera : public OBJ_Node
{
public:
	     OBJ_Camera(OP_Network *, const char *, OP_Operator *);
    virtual ~OBJ_Camera();

    /// Get the IPR camera - This may create a camera object
    static OBJ_Camera		*getIPRCamera(bool *created=nullptr);

    static PRM_Template		*getObsolete();
    static OP_Node		*myConstructor(OP_Network *net,
					const char *name, OP_Operator *entry);

    void		 	 getCameraParms(OBJ_CameraParms &parms,
						fpreal t);

    void			 addInterestOnCameraParms(OP_Node *node);
    
    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    virtual void		 saveDialogScriptExtraInfo(std::ostream &os);

    static PRM_Template		*getTemplateList(OBJ_ParmsStyle style);


    virtual OBJ_Camera		*castToOBJCamera() { return this; }
    virtual OBJ_Camera		*getControllingCamera(fpreal now);

protected:
   // Used to get pointer to indirection indices for each object type
    virtual int                 *getIndirect() const
				{ return camIndirect; }

    virtual OP_ERROR	 cookMyObj(OP_Context &ctx);

    virtual int		 collectPropertiesFromParms(PI_EditScriptedParms &,
					UT_StringArray &names,
					PRM_ParmNameMap &nmap);
    virtual int		 collectPropertiesFromChannels(PI_EditScriptedParms &,
					UT_BitArray &selection,
					const CH_ChannelList &channels);

public:		// Public so that other people can get this info
    /// Get the camera's projection matrix.  For example, if you have a point
    /// @c P in world-space, to get it's NDC space: @code
    ///    UT_Matrix4D	worldToCamera, projection;
    ///    cam.getInverseObjectToWorldTransform(context, worldToCamera);
    ///    cam.getProjectionMatrix(context, projection);
    ///    UT_Vector3 P_object = P * worldToCamera;
    ///    UT_Vector3 P_ndc = P_object * projection;
    /// @endcode
    void	getProjectionMatrix(const OP_Context &context,
					UT_Matrix4D &matrix);

    // Take the winroll into account correctly
    void		 getWinParms(fpreal &winx, fpreal &winy,
				    fpreal &sizex, fpreal &sizey,
				    fpreal &winroll, fpreal now);

    // All projection methods are virtual to allow for blending of camera view.
    virtual int		PROJECTION(fpreal t);
    virtual int		REMOVESCALE(fpreal t);
    virtual fpreal	ORTHOW(fpreal t);
    virtual int		RESX(fpreal);
    virtual int		RESY(fpreal);
    virtual fpreal	ASPECT(fpreal t);
    virtual fpreal	FOCAL(fpreal t);
    virtual fpreal	APERTURE(fpreal t);
    virtual fpreal	SHUTTER(fpreal t);
    virtual fpreal	getNEAR(fpreal t);
    virtual fpreal	getFAR(fpreal t);
    virtual fpreal	CURVATURE(fpreal t);
    virtual fpreal	FSTOP(fpreal t);
    virtual fpreal	FOCUS(fpreal t);
    virtual fpreal	CROPL(fpreal t);
    virtual fpreal	CROPR(fpreal t);
    virtual fpreal	CROPB(fpreal t);
    virtual fpreal	CROPT(fpreal t);
    virtual fpreal	WINPX(fpreal t);
    virtual fpreal	WINPY(fpreal t);
    virtual fpreal	WINX(fpreal t);
    virtual fpreal	WINY(fpreal t);
    virtual fpreal	WINSIZEX(fpreal t);
    virtual fpreal	WINSIZEY(fpreal t);
    virtual fpreal	WINROLL(fpreal t);
    
    int			ROTOSOURCE(fpreal now);
    void		ROTOFILE(UT_String &str, fpreal t);
    void		ROTOFILE_RAW(UT_String &str, fpreal t);
    int			ROTO_OVERRIDE(fpreal now);
    int			ROTO_RESX(fpreal t);
    int			ROTO_RESY(fpreal t);
    void		COPPATH(UT_String &str, fpreal t);
    int			TRACKWINCHANS(fpreal t);
    fpreal		COPFRAME(fpreal t);
    void		COPFRAME_RAW(UT_String &str, fpreal t);
    void		COLOR_PLANE(UT_String &str, fpreal t);
    int			ALPHA_PLANE(fpreal);

    fpreal		CLEAR(fpreal t);
    int			FOG(fpreal t);
    fpreal		FOGDENSITY(fpreal t);
    fpreal		FOGNEAR(fpreal t);
    fpreal		FOGFAR(fpreal t);
    fpreal		FOGCOLOR(fpreal t, int i);
    fpreal		FOGALPHA(fpreal t);

    void		VIEWCOMMENT(UT_String &str, fpreal t);
    virtual fpreal	MASKASPECT(fpreal t);
    virtual fpreal	MASKOPACITY(fpreal t);
    virtual void	MASKOVERLAY(UT_String &str, fpreal t);

protected:
    // This method should be called when evaluating "spare" parameters which
    // influence the cook.  Currently used by winroll.
    fpreal		cookDependentFloat(const char *name, fpreal now,
				int vi, fpreal defval);
    int			cookDependentInt(const char *name, fpreal now,
				int vi, int defval);

private:
    static int			*camIndirect;
};

#endif
