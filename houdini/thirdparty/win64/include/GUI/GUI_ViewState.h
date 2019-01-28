/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_ViewState.h (Geometry Render Library, C++)
 *
 * COMMENTS:
 *		This class basically envelopes a DisplayOption and
 *		a ViewParameter. So it holds all the information
 *		that is relevant to viewing.
 *
 */

#ifndef __GUI_ViewState__
#define __GUI_ViewState__

#include "GUI_API.h"
#include "GUI_DisplayOption.h"
#include "GUI_ViewParameter.h"
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Plane.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Vector3.h>

// Essentially a callback class to allow options from above GUI
// to be saved to the default file.
class GUI_API GUI_ExternalDefaultCB
{
public:
    virtual ~GUI_ExternalDefaultCB() {}
    virtual bool isDefaultToken(const char *token) = 0;
    virtual bool parseDefaultOption(const char *token, UT_IStream &is) = 0;

    virtual bool save(std::ostream &os) = 0;
};


class UT_StringArray;
class TIL_Raster;
class GUI_SnapshotImage;

class GUI_API GUI_ViewState
{
public:
		 GUI_ViewState();
		~GUI_ViewState();

    // member data accessors
    const GUI_DisplayOption	&getDisplayOptionRef() const
				{ return myOption; }
    const GUI_ViewParameter	&getViewParameterRef()  const
				{ return myView;   }
    GUI_DisplayOption	&getDisplayOptionRef()	{ return myOption; }
    GUI_ViewParameter	&getViewParameterRef() 	{ return myView;   }

    // sets the OpenGL state according to the view parameter
    void	 setViewport(RE_Render *r, int x, int y, int w, int h)
					{ myView.setViewport(r, x, y, w, h); }
    void	 setProjection(RE_Render *r)
					{ myView.setProjection(r); }

    // methods for getting and setting the view point
    void 	 homeToBox(int w, int h, UT_BoundingBox &box,
			   const UT_Matrix3R *rot = NULL, bool centred = false,
			   bool zoom_in = false)
				{
				    fpreal minh = getDisplayOptionRef().common()
					.getMinHomeZoom();
				    myView.homeToBox(w, h, minh, box, rot,
						     centred, zoom_in);
				    myLeftView.homeToBox(w, h, minh, box, rot,
						     centred, zoom_in);
				    myRightView.homeToBox(w, h, minh, box, rot,
						     centred, zoom_in);
				}
    void	 getViewport(int &l, int &r, int &b, int &t) const
					{ myView.getViewport(l, r, b, t); }
    UT_DimRect	 getViewport() const	{ return myView.getViewport(); }
    void	 getView(int &l, int &r, int &b, int &t) const
					{ myView.getView(l, r, b, t); }
    int	 getViewWidth() const { return myView.getViewWidth(); }
    int	 getViewHeight() const { return myView.getViewHeight(); }

    const UT_Matrix4D  &getItransformMatrix()
				        { return myView.getItransformMatrix();}
    const UT_Matrix4D  &getTransformMatrix()
				        { return myView.getTransformMatrix(); }
    const UT_Matrix4D  &getProjectionMatrix() const
					{ return myView.getProjectionMatrix();}
    const UT_Matrix4D  &getRotateMatrix() const
					{ return myView.getRotateMatrix();    }
    void	 setRotateMatrix(const UT_Matrix3R &mat)
					{ myView.setRotateMatrix(mat);        }

    int		 initialized() const    { return myView.initialized(); }

    void	 setOrthoFlag(int onoff)
				    {
					myView.setOrthoFlag(onoff);
					myLeftView.setOrthoFlag(onoff);
					myRightView.setOrthoFlag(onoff);
				    }
    int		 getOrthoFlag() const	{ return myView.getOrthoFlag();       }

    void	 setApplyAspectFlag(int onoff) 
					{ myView.setApplyAspectFlag(onoff);   }
    int		 getApplyAspectFlag() const
					{return myView.getApplyAspectFlag();  }
    void	 setAspectRatio(fpreal value) 
					{ myView.setAspectRatio(value);       }
    fpreal	 getAspectRatio() const
					{return myView.getAspectRatio();      }
    void	 setLimits(fpreal n, fpreal f)
					{ myView.setLimits(n, f); }

    void	 rotate(UT_Axis3::axis axis, fpreal dx)
				    { 
					myView.rotate(axis, dx);
					myLeftView.rotate(axis, dx);
					myRightView.rotate(axis, dx);
				    }
    void	 rotate(const UT_Vector3R &axis, fpreal dx)
				    { 
					myView.rotate(axis, dx);
					myLeftView.rotate(axis, dx); 
					myRightView.rotate(axis, dx); 
				    }
    void	 rotate(const UT_Quaternion &quat)
				    { 
					myView.rotate(quat);
					myLeftView.rotate(quat); 
					myRightView.rotate(quat); 
				    }
    void	 trackball(fpreal dx, fpreal dy)
				    { 
					myView.trackball(dx, dy); 
					myLeftView.trackball(dx, dy); 
					myRightView.trackball(dx, dy); 
				    }
    void	 dotumble(fpreal dx, fpreal dy, int changed)
				    { 
					myView.dotumble(dx, dy, changed); 
					myLeftView.dotumble(dx, dy, changed); 
					myRightView.dotumble(dx, dy, changed); 
				    }
    void	 scroll(fpreal dx, fpreal dy)
				    { 
					myView.scroll(dx, dy);
					myLeftView.scroll(dx, dy);
					myRightView.scroll(dx, dy);
				    }
    void	 scroll(fpreal dx, fpreal dy,
			const UT_InclusiveRect &viewport)
				    { 
					myView.scroll(dx, dy, viewport);
					myLeftView.scroll(dx, dy, viewport);
					myRightView.scroll(dx, dy, viewport);
				    }
    void	 dolly(fpreal dx,
		       bool precise_zoom = false,
		       bool smooth_dolly = false)
				    {
					myView.dolly(dx, precise_zoom,
						     smooth_dolly); 
					myLeftView.dolly(dx, precise_zoom,
							 smooth_dolly); 
					myRightView.dolly(dx, precise_zoom,
							  smooth_dolly); 
				    }
    void	 directedDolly(const UT_Vector3R &unit_worldspace_dir,
			       fpreal dx,
			       bool precise_zoom = false,
			       bool smooth_dolly = false)
				    {
					myView.directedDolly(
					    unit_worldspace_dir, dx,
					    precise_zoom,
					    smooth_dolly); 
					myLeftView.directedDolly(
					    unit_worldspace_dir, dx,
					    precise_zoom,
					    smooth_dolly); 
					myRightView.directedDolly(
					    unit_worldspace_dir, dx,
					    precise_zoom,
					    smooth_dolly); 
				    }
    void	 zoom(fpreal dx, GUI_ViewParameter::GUI_ZoomItem item,
		      bool precise_zoom = false)
				    { 
					myView.zoom(dx, item, precise_zoom);
					myLeftView.zoom(dx, item, precise_zoom);
					myRightView.zoom(dx, item,precise_zoom);
				    }
    void	 zoom(fpreal dx, GUI_ViewParameter::GUI_ZoomItem item,
		      const UT_InclusiveRect &viewport,
		      bool precise_zoom = false)
				    { 
					myView.zoom(dx, item, viewport,
						    precise_zoom);
					myLeftView.zoom(dx, item, viewport,
							precise_zoom);
					myRightView.zoom(dx, item, viewport,
							 precise_zoom);
				    }
    void	 offsetZoom(fpreal sx, fpreal sy, fpreal dx,
			    GUI_ViewParameter::GUI_ZoomItem item,
			    bool precise_zoom = false)
				    { 
					myView.offsetZoom(sx, sy, dx, item,
							  precise_zoom);
					myLeftView.offsetZoom(sx, sy, dx, item,
							  precise_zoom);
					myRightView.offsetZoom(sx, sy, dx, item,
							  precise_zoom);
				    }
    void	 offsetZoom(fpreal sx, fpreal sy, fpreal dx,
			    GUI_ViewParameter::GUI_ZoomItem item,
			    const UT_InclusiveRect &viewport,
			    bool precise_zoom = false)
				    { 
					myView.offsetZoom(sx, sy, dx, item,
							  viewport,
							  precise_zoom);
					myLeftView.offsetZoom(sx, sy, dx, item,
							  viewport,
							  precise_zoom);
					myRightView.offsetZoom(sx, sy, dx, item,
							  viewport,
							  precise_zoom);
				    }
    void	 boxZoom(fpreal frstx, fpreal frsty, fpreal lastx, 
			 fpreal lasty, fpreal distance)
			{ myView.boxZoom(frstx,frsty,lastx,lasty, distance); }

    void	 setPrimitiveMask(GA_PrimCompat::TypeMask m) 
				    { myOption.common().primMask(m); }
    GA_PrimCompat::TypeMask getPrimitiveMask(void) const 
				    { return myOption.common().primMask(); }
    
    // Enumeration of the types of view points:
    enum GUI_ViewType
    {
	GUI_MAIN,
	GUI_LEFT,
	GUI_RIGHT
    };

    // type dependent view parameter accessor
    GUI_ViewParameter	&getViewParameter(GUI_ViewState::GUI_ViewType view_type)
			 { 
			     if( view_type == GUI_MAIN )
				 return myView;
			     if( view_type == GUI_LEFT )
				 return myLeftView;
			     if( view_type == GUI_RIGHT )
				 return myRightView;
			     return myView;
			 }

    // for general synchronization of L/R view points with the main one
    void	 syncViewsWithMain()
			{ 
			    myLeftView  = myView;
			    myRightView = myView;
			}

    // type dependent versions
    void	 setViewport(GUI_ViewState::GUI_ViewType vt,
			     RE_Render *r, int x, int y, int w, int h)
			    { getViewParameter(vt).setViewport(r, x, y, w, h); }
    void	 setProjection(GUI_ViewState::GUI_ViewType vt, RE_Render *r)
			    { getViewParameter(vt).setProjection(r); }

    // The UT_Plane will be transformed with the inverted ViewState transform
    // matrix
    void	 alignPlaneWithView(UT_Plane *plane);

    const char	*className() const;

    GUI_ViewState &operator=(const GUI_ViewState &src);

    // Method to override the default preference file name.  In order to for
    // the defaults to be loaded from this file, this must be called before
    // the first instance of this class is created.
    static void	 overridePrefFile(const char *filename); 	

    void	 saveAsDefault();
    void	 resetToDefaults();

    // setting the image that overrides the regular rendering
    void	     setSnapshotImage(TIL_Raster *texture);
    TIL_Raster	    *getSnapshotImage() const;

    // setting the frame information of the snapshot image 
    void	     setSnapshotImageFrame( int frame );
    int		     getSnapshotImageFrame() const;

    // the snapshot blending coefficient (alpha)
    void	     setSnapshotAlpha( float alpha);
    float	     getSnapshotAlpha() const;

    // snapshot image flags
    void	     setHideSnapshotAtSourceFrame( bool flag );
    bool	     getHideSnapshotAtSourceFrame() const;

    void	     setTumbling(bool tumble)	{ myTumbleFlag = tumble; }
    bool	     isTumbling() const		{ return myTumbleFlag; }


    // Returns the near and far clipping planes that have saved in the pref file
    static void	 getDefaultNearFarLimits(fpreal &near_limit, fpreal &far_limit);

    // Methods to manipulate the external callback class for saving/loading
    // defaults.
    static GUI_ExternalDefaultCB	*getExternalDefaultCB()
			{ return externalDefaultValues; }
    static void 	setExternalDefaultCB(GUI_ExternalDefaultCB *cb)
			{ externalDefaultValues = cb; }

    static void		ensureDefaultsLoaded();

private:
    static GUI_ViewState		*defaultValues;
    static GUI_ExternalDefaultCB	*externalDefaultValues;
    static UT_StringArray		*defaultEnabledUserOptions;

    // Display options for the viewport
    GUI_DisplayOption	 	 myOption;

    // Defines the point of view for simple cameras (non-stereoscopic). 
    GUI_ViewParameter 		 myView;

    // For stereoscopic cameras we have two view points
    GUI_ViewParameter	 	 myLeftView;
    GUI_ViewParameter	 	 myRightView;

    // data about the snapshot image that is displayed instead of showing
    // the rendered scene (or is blended with the scene framebuffer)
    GUI_SnapshotImage		*mySnapshotImage;
    bool			 myTumbleFlag;
};

#endif
