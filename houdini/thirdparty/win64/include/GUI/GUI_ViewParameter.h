/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_ViewParameter (Geometry Render Library, C++)
 *
 * COMMENTS:
 *		ViewParameter contains information relating to the
 *		viewport's physical characteristics, eg aspect ratio,
 *		position, lookat.
 *
 */

#ifndef __GUI_ViewParameter__
#define __GUI_ViewParameter__

#include "GUI_API.h"
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Rect.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Plane.h>

class RE_Render;

typedef void	(*GRgetTransformCallback)(UT_DMatrix4 &, fpreal time, void *);

class GUI_API GUI_ViewFlag
{
public:
		 GUI_ViewFlag()
		 {
		    ortho		= 0;
		    applyAspect		= 0;
		    matrixDirty		= 1;
		    iMatrixDirty	= 1;
		    boxZoom		= 0;
		    viewportDirty	= 0;
		    rotoTrackWindow	= 0;
		    initXform		= 0;
		    rxyzDirty		= 1;
		 }

    // ASCII methods to save and load flags
    void	 save(std::ostream &os) const;
    bool	 load(UT_IStream &is);

    unsigned	 ortho:1,
		 applyAspect:1,
		 matrixDirty:1,
		 iMatrixDirty:1,
		 boxZoom:1,
		 viewportDirty:1,
    		 rotoTrackWindow:1,
    		 initXform:1,
		 rxyzDirty:1;
};

class GUI_API GUI_ViewParameter
{
public:
		 GUI_ViewParameter();
		~GUI_ViewParameter();

    enum	 axis { OBLIQUE, CUSTOM, XAXIS, YAXIS, ZAXIS,
			    FRONT, RIGHT, TOP, BACK, LEFT, BOTTOM };

    enum	 orientation { Y_UP, Z_UP };

    void	 getViewport(UT_DimRect &area) const
		 {
		     int w,h;
		     getViewportSize(w,h);
		     area.set(myViewportLeft, myViewportBottom, w,h);
		 }
    void	 getViewport(int &l, int &r, int &b, int &t) const;
    void	 getViewportSize(int &w, int &h) const
		 {
		     // Note that we have inclusive coordinates.
		     w = myViewportRight - myViewportLeft + 1;
		     h = myViewportTop - myViewportBottom + 1;
		 }
    int		 getViewportWidth() const
		    { return myViewportRight - myViewportLeft + 1; }
    int		 getViewportHeight() const
		    { return myViewportTop - myViewportBottom + 1; }
    void	 setViewport(RE_Render *r, int x, int y, int w, int h);
    void	 setViewport(RE_Render *r, const UT_DimRect &viewarea)
		    { setViewport(r, viewarea.x(), viewarea.y(),
				     viewarea.w(), viewarea.h()); }

    // Utility for converting our viewport into a UT_Rect.
    UT_InclusiveRect getViewport() const
		{ return UT_InclusiveRect(myViewportLeft, myViewportBottom,
					  myViewportRight, myViewportTop); }


    // Both setViewport and setView must be called before setProjection.
    // r may be NULL to set the projection matrix without loading it into
    // RE_Render.
    void	 setProjection(RE_Render *r);

    // Returns the projection matrix. If depth_adjust is false, Z is not
    // mapped to [0,1], and instead remains as-is.
    void	 getProjection(UT_Matrix4D &view, bool depth_adjust=true);


    // temporarily use this projection matrix. This class does not take
    // ownership.
    void	 setProjectionOverride(UT_Matrix4D *override);

    // Get the direction a particular point (px, py) in the camera's
    // near plane is pointing to in world coordinates. The range of
    // (px, py) is (0, 0) to (1, 1) with (0, 0) representing the 
    // lower-left corner and the default arguments represent the center of
    // the screen.
    UT_Vector3	 getCameraDir(fpreal px = 0.5, fpreal py = 0.5);

    // A utility method to return the world space plane representing our
    // view plane.  The point will be set to the center of the view and
    // the normal will point towards the camera.
    UT_PlaneD	 getViewPlane();

    // The view is a subregion of the viewport that represents the current
    // "camera" view.  Aspect ratio bars are used to dim the parts of the
    // viewport not in this view.  The window coordinates are relative to
    // this view, not the viewport.
    void	 setView(const UT_DimRect &area)
		    { setView(area.x(), area.y(), area.w(), area.h()); }
    void	 setView(int x, int y, int w, int h);
    void	 getView(int &l, int &r, int &b, int &t) const;
    int		 getViewWidth() const
		    { return myViewRight - myViewLeft + 1; }
    int		 getViewHeight() const
		    { return myViewTop - myViewBottom + 1; }

    // A handy method to compute projection bounds for the viewport such
    // that the specified bounds map to the view subregion.
    void	 viewToViewportProjectionBounds(
		    fpreal &left, fpreal &right,
		    fpreal &bottom, fpreal &top) const;

    // Convenience methods to query the margins on each side of the view
    // subregion.
    int		 getLeftMargin() const
		    { return myViewLeft - myViewportLeft; }
    int		 getRightMargin() const
		    { return myViewportRight - myViewRight; }
    int		 getBottomMargin() const
		    { return myViewBottom - myViewportBottom; }
    int		 getTopMargin() const
		    { return myViewportTop - myViewTop; }

    void	 homeToBox(int width, int height, fpreal minzoom, 
			   UT_BoundingBox &box, const UT_Matrix3R *rot = NULL,
			   bool centre_to_origin = false, bool zoom_in = false);
    void	 homeRotation(axis a, UT_Matrix3R &rot) const;
    void	 homeRotation(axis a);

    void	 resetFOV();

    // Query the rotation that converts the view rotation from the old mode
    // to the new mode.
    static void	 getOrientationChangeRotation(
				    GUI_ViewParameter::orientation old_mode,
				    GUI_ViewParameter::orientation new_mode,
				    UT_Matrix3R &rot);

    // Convert a view transform relative to the old_mode to the corresponding
    // view transform relative to the new mode.
    void	 convertTransform(GUI_ViewParameter::axis home_axis,
				  GUI_ViewParameter::orientation old_mode,
				  GUI_ViewParameter::orientation new_mode);

    // To perform view operations that are being exported continuously, we
    // need to cache some data to account for view changes that occur as a
    // result of the exporting.  Generally, you'll want to call the begin
    // method on a UI_VALUE_START and the end method on a UI_VALUE_CHANGED.
    void	 beginContinuouslyExportedOperation();
    void	 endContinuouslyExportedOperation();
    bool	 continuouslyExportedOperationInProgress() const;

    void	 rotate(UT_Axis3::axis axis, fpreal dx);
    void	 rotate(const UT_Vector3R &axis, fpreal dx);
    void	 rotate(const UT_Quaternion &quat);
    void	 trackball(fpreal dx, fpreal dy);
    void	 incrementtrackball(fpreal dx, fpreal dy, int finish);
    void	 eulertumble(fpreal dx, fpreal dy, int finish, bool lock);
    void	 dotumble(fpreal dx, fpreal dy, int finish);

    void	 scroll(fpreal dx, fpreal dy);
    // Scroll using the specified viewport size instead of our settings.  Used
    // for scrolling linked ortho viewports.
    void	 scroll(fpreal dx, fpreal dy, const UT_InclusiveRect &viewport);

    void	 dolly(fpreal dx,
		       bool precise_zoom = false,
		       bool smooth_deltas = false);

    // Dolly using a specified world-space direction (instead of towards the
    // center of the view as in a regular dolly).  The use of world space is
    // a convenience for the GUI_ViewState wrapper that would otherwise have
    // to transform this to a view space direction for each instance of this
    // class.
    void	 directedDolly(const UT_Vector3R &unit_worldspace_dir,
			       fpreal dx, bool precise_zoom = false,
			       bool smooth_deltas = false);

    enum GUI_ZoomItem
    {
	GUI_WIN_SIZE,
	GUI_ORTHO_WIDTH
    };
    void	 zoom(fpreal dx, GUI_ZoomItem item, bool precise_zoom = false);
    // Zoom using the specified viewport size instead of our settings.  Used
    // for zooming linked ortho viewports.
    void	 zoom(fpreal dx, GUI_ZoomItem item,
		      const UT_InclusiveRect &viewport,
		      bool precise_zoom = false);

    // Versions of the zoom() methods that center the zoom around the given
    // (normalized) screen position (sx, sy).  This affects either the window
    // offset, when GUI_WIN_SIZE is specified, or the view position, when
    // GUI_ORTHO_WIDTH is specified.
    void	 offsetZoom(fpreal sx, fpreal sy, fpreal dx, GUI_ZoomItem item,
			    bool precise_zoom = false);
    void	 offsetZoom(fpreal sx, fpreal sy, fpreal dx, GUI_ZoomItem item,
			    const UT_InclusiveRect &viewport,
			    bool precise_zoom = false);

    // Dolly onto a target located at a specified distance.
    // Note that the location and size of the "box" must be
    // normalized to [0, 1]
    // ** Note : Aspect ratio of the box is assumed to match aspect ratio 
    //		 of the viewport!
    void	 boxZoom(fpreal firstx, fpreal firsty, fpreal lastx, 
			 fpreal lasty, fpreal distance);

    // Returns the width of the frustum cross-section at the distance t.
    static fpreal getFrustumWidth(fpreal t, fpreal aperture, fpreal focal_length,
				  fpreal orthowidth, bool is_ortho);

    // Returns the centre of the frustum (cx, cy) and the half-width and
    // half-height (hw, hh), from these, you can compute the corners of the
    // frustum.
    static void  getFrustumCenter(fpreal frustum_width, fpreal aspect,
				   fpreal x_offset, fpreal y_offset,
				   fpreal x_window_size, fpreal y_window_size,
				   fpreal &cx, fpreal &cy,
				   fpreal &hw, fpreal &hh);

//  member data access methods

    bool	 isDirty() const	{ return (myFlag.viewportDirty ||
						  myFlag.matrixDirty || 
						  myFlag.iMatrixDirty); }
    // bumped whenever the view has changed.
    int		 getViewVersion() const { return myVersion; }
    
    // projection
    int		 initialized() const 		{ return myFlag.initXform;    }
    int		 getOrthoFlag()	const		{ return myFlag.ortho;        }
    void	 setOrthoFlag(int o)		{ myFlag.ortho = o;           }
    void	 setLimits(fpreal n, fpreal f) { setNearClip(n); setFarClip(f); }
    void	 getLimits(fpreal *n, fpreal* f)
		 {
		     *n = myView.myNearLimit;
		     *f = myView.myFarLimit;
		 }
    void	 setNearClip(fpreal near);
    void	 setFarClip(fpreal far);
    fpreal	 getNearClip() const { return myView.myNearLimit; }
    fpreal	 getFarClip() const { return myView.myFarLimit; }

    void	 setWindow(fpreal x, fpreal y, fpreal w, fpreal h)
		 {
		     myView.myWinX = x;
		     myView.myWinY = y;
		     myView.myWinW = SYSmax(w, fpreal(0.001));
		     myView.myWinH = SYSmax(h, fpreal(0.001));
		 }
    void	 getWindow(fpreal &x, fpreal &y,
			   fpreal &w, fpreal &h) const
		 {
		     x = myView.myWinX;
		     y = myView.myWinY;
		     w = myView.myWinW;
		     h = myView.myWinH;
		 }

    // The window is stored as an offset from 0, 0 with the scale applied prior
    // to translation.  However, mantra and other renderers expect the window
    // to be relative to the (0,1) rectangle.  getScreenWindow() will translate
    // the window parameters into a screen window.  setScreenWindow() will take
    // a screen window and set the appropriate parameters.
    void	 getScreenWindow(fpreal &l, fpreal &r,
				 fpreal &b, fpreal &t) const
		 {
		     convertViewToScreenWindow(myView.myWinX, myView.myWinY,
					       myView.myWinW, myView.myWinH,
					       l, r, b, t);
		 }
    void	 setScreenWindow(fpreal l, fpreal r, fpreal b, fpreal t)
		 {
		     convertScreenToViewWindow(l, r, b, t,
					       myView.myWinX, myView.myWinY,
					       myView.myWinW, myView.myWinH);
		 }

    static void	 convertScreenToViewWindow(
			fpreal l, fpreal r, fpreal b, fpreal t,
			fpreal &xoff, fpreal &yoff, fpreal &xsize, fpreal &ysz);
    static void	 convertViewToScreenWindow(
			fpreal xoff, fpreal yoff, fpreal xsize, fpreal ysize,
			fpreal &l, fpreal &r, fpreal &b, fpreal &t);

    fpreal	 getWinSizeX() const	{ return myView.myWinW; }
    fpreal	 getWinSizeY() const	{ return myView.myWinH; }
    fpreal	 getWinX() const	{ return myView.myWinX; }
    fpreal	 getWinY() const	{ return myView.myWinY; }

    enum GUI_ViewAdjustLimits
    {
	GUI_VIEW_ADJUST_NONE = 0,
	GUI_VIEW_ADJUST_FAR  = 1,
	GUI_VIEW_ADJUST_NEAR = 2,
	GUI_VIEW_ADJUST_BOTH = 3
    };
    void	 setAdjustLimits(GUI_ViewAdjustLimits which)
						{ myAdjustLimits = which;    }
    GUI_ViewAdjustLimits getAdjustLimits() const { return myAdjustLimits; }

    void       setWindowAffectRotoscope(int v)	{ myFlag.rotoTrackWindow = v; }
    int		 rotoscopeTracksWindowVals() const 
					    { return myFlag.rotoTrackWindow; }

    // aspectRatio
    void	 setApplyAspectFlag(int a)	{ myFlag.applyAspect = a;     }
    int		 getApplyAspectFlag() const	{ return myFlag.applyAspect;  }
    void	 setAspectRatio(fpreal a)	{ myViewAspectRatio = a;      }
    fpreal	 getAspectRatio() const		{ return myViewAspectRatio;   }

    void	 setFrustumScale(fpreal xsc, fpreal ysc)
		 {
		     myFrustumWidthScale  = xsc;
		     myFrustumHeightScale = ysc;
		 }

    // position
    void	 setT(const UT_Vector3D &t)
	    { myView.myT = t; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}
    void	 setTx(fpreal v)
	    { myView.myT.x()=v; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}
    void	 setTy(fpreal v)
	    { myView.myT.y()=v; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}
    void	 setTz(fpreal v)
	    { myView.myT.z() = v; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}

    UT_Vector3D  getT() const   { return myView.myT; }
    fpreal	 getTx() const	{ return myView.myT.x(); }
    fpreal	 getTy() const	{ return myView.myT.y(); }
    fpreal	 getTz() const	{ return myView.myT.z(); }

    // pivot
    UT_Vector3D  getP() const   { return myView.myP; }
    fpreal	 getPx() const	{ return myView.myP.x(); }
    fpreal	 getPy() const	{ return myView.myP.y(); }
    fpreal	 getPz() const	{ return myView.myP.z(); }
    void	 setP(const UT_Vector3D &p)
	    { myView.myP = p; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}
    void	 setPx(fpreal v)
	    { myView.myP.x() = v; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}
    void	 setPy(fpreal v)
	    { myView.myP.y() = v; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}
    void	 setPz(fpreal v)
	    { myView.myP.z() = v; myFlag.matrixDirty=1; myFlag.iMatrixDirty=1;}

    static void		setOrientationMode(orientation o)
						    { myOrientationMode = o; }
    static orientation	getOrientationMode() { return myOrientationMode; }
    static bool		isOrientationYUp()
			    { return myOrientationMode == Y_UP; }
    static bool		isOrientationZUp()
			    { return myOrientationMode == Z_UP; }

    // Rotation style:
    static void	 setTumbleStyleFlag(int style)
		 { myTumbleStyle = style; }
    static int	 getTumbleStyleFlag() { return myTumbleStyle; }
    static void	 setAltKeyViewControlsFlag(int style)
		 { myAltKeyViewControls = style; }
    static int	 getAltKeyViewControlsFlag() { return myAltKeyViewControls; }
    static void  setScrollStyleFlag(int style)
		 { myScrollStyle = style; }
    static int	 getScrollStyleFlag() { return myScrollStyle; }
    static void  setDefaultHomingStyleFlag(int style)
		 { myDefaultHomingStyle = GUI_ViewParameter::axis(style); }
    static axis	 getDefaultHomingStyleFlag() { return myDefaultHomingStyle; }

    // the resulting matrices
    void	 setRotateMatrix(const UT_Matrix3D &mat) 
						{ myView.myRotateMatrix = mat;
						  myFlag.matrixDirty = 1;
						  myFlag.iMatrixDirty = 1;    
						  myFlag.rxyzDirty = 1;	      }
    const UT_Matrix4D &getRotateMatrix() const { return myView.myRotateMatrix; }

    const UT_Matrix4D &getProjectionMatrix() const
	    { return myProjectionOverride ? *myProjectionOverride
					  : myView.myProjectionMatrix;  }
    const UT_Matrix4D &getTransformMatrix()	{ updateTransformMatrix();
						  return myTransformMatrix;   }
    const UT_Matrix4D &getItransformMatrix()	{ updateItransformMatrix();
						  return myItransformMatrix;  }

    // This sets the transform matrix to the given value, bypassing the
    // PX, TX, etc.  The purpose of this is to get accurate camera transforms
    // you can first set all the PX, Rotate, etc, according to the cracked
    // values, and then do a forceTransformMatrix to ensure the user sees
    // the most accurate possible matrix.
    void	 forceTransformMatrix(const UT_DMatrix4 &mat);
    void	 getTransformMatrix(UT_DMatrix4 &mat, fpreal time);
    void	 setTransformCallback(GRgetTransformCallback callback,
				      void *data)
		 {
		     myTransformCallback = callback;
		     myTransformCallbackData = data;
		 }

    // Class I/O methods.  They return 0 if successful, -1 if not.
    int		 save(std::ostream &os, char separator = '\n') const;
    bool	 load(UT_IStream &is);
    bool	 oldLoad(UT_IStream &is, int version);

    // inline for non-virtual class
    const char	*className() const { return "GUI_ViewParameter"; }

    fpreal 	 getLastHomeRadius() const	{ return myLastHomeRadius;    }

    void	 setHomeAxis(axis homeaxis)
		    { myHomeAxis = homeaxis; myFlag.rxyzDirty = 1; }
    axis	 getHomeAxis() const { return myHomeAxis; }
    void	 setCustomHomeRot(UT_Matrix3R &rot) { myCustomHomeRot = rot; }
    const UT_Matrix3R	&getCustomHomeRot() const { return myCustomHomeRot; }


    void	 setOrthoWidth(fpreal w) { myView.myOrthoWidth = w; }
    fpreal	 getOrthoWidth() const	{ return myView.myOrthoWidth; }
    
    void	 setFocalLength(fpreal f) { myView.myFocalLength = f; }
    fpreal	 getFocalLength() const	{ return myView.myFocalLength; }
    
    void	 setAperture(fpreal a)	{ myView.myAperture = a; }
    fpreal	 getAperture() const	{ return myView.myAperture; }
    fpreal	 getNearLimit() const	{ return myView.myNearLimit; }
    fpreal	 getFarLimit() const	{ return myView.myFarLimit; }

    fpreal	 getZBufferNear() const { return myZBufferNear; } 
    fpreal	 getZBufferFar() const  { return myZBufferFar; } 

  // viewport clearing
    fpreal	 myClear;

  // fog
    int		 myFog;
    float	 myFogDensity;
    float	 myFogNear;
    float	 myFogFar;
    float	 myFogColor[3];
    float	 myFogAlpha;

  // used for rotoscoping only
    fpreal	 myWinRoll;

    // container for all view home information 
    class gui_View
    {
    public:
	gui_View()
	    : myT(0.0, 0.0, 0.1),
	      myP(0.0, 0.0, 0.0),
	      myRotateMatrix(1.0),
	      myProjectionMatrix(1.0),
	      myWinX(0.0),
	      myWinY(0.0),
	      myWinW(1.0),
	      myWinH(1.0),
	      myOrthoWidth(1.0),
	      myFocalLength(50.0),
	      myAperture(41.4214),
	      myNearLimit(0.1),
	      myFarLimit(10000.0)
	    {}
	
	UT_Vector3D	 myT;
	UT_Vector3D	 myP;

	UT_Matrix4D	 myRotateMatrix;
	UT_Matrix4D	 myProjectionMatrix;
	
	// window (zooming)
	fpreal myWinX;
	fpreal myWinY;
	fpreal myWinW;
	fpreal myWinH;
	
	// projection information
	// ortho specific
	fpreal	 myOrthoWidth;
	
	// perspective specific sSee diagram below)
	fpreal	 myFocalLength;
	fpreal	 myAperture;
  
	fpreal	 myNearLimit;
	fpreal	 myFarLimit;
    };
    
    // methods to quickly stash and recall the view state.
    void	 getViewState(gui_View &view_state) const
		     { view_state = myView; }
    void	 setViewState(const gui_View &view_state)
		    {
			myView = view_state;
			myFlag.matrixDirty=1;
			myFlag.iMatrixDirty=1;
			myFlag.rxyzDirty=1;
		    }

  // clipping
private:
    fpreal	 myZBufferNear;
    fpreal	 myZBufferFar;
    GUI_ViewAdjustLimits  myAdjustLimits;

  // aspect ratio
    fpreal	 myViewAspectRatio;
public:
    fpreal	 myPixelAspect;	// depends on output device

// projection information (see docs below)
    fpreal	 myAdjustedNear;
    fpreal	 myAdjustedFar;
    fpreal	 myAdjustedX;
    fpreal	 myAdjustedY;
    fpreal	 myAdjustedW;
    fpreal	 myAdjustedH;
    fpreal	 myFullW;
    fpreal	 myFullH;
    fpreal	 myFrustumWidthScale;
    fpreal	 myFrustumHeightScale;

    fpreal	 myLastZoomDelta;
    
private:
    void	 updateTransformMatrix();
    void	 updateItransformMatrix();
    void	 updateRXYZValues();
    fpreal	 getDistanceToBoxCenter(int clamp = 0) const;

    void	 internalScroll(fpreal dx, fpreal dy,
				const UT_InclusiveRect &viewport,
				bool decouple_axes);
    void	 internalZoom(fpreal dx, GUI_ZoomItem item, int width);
    void	 internalOffsetZoom(fpreal sx, fpreal sy, fpreal dx,
				    GUI_ZoomItem item,
				    const UT_InclusiveRect &viewport);

    void	 getFrustumClip(fpreal &l, fpreal &r, fpreal &t, fpreal &b,
				fpreal &n, fpreal &f);
    fpreal	 computeZoomDelta(fpreal dx,
				  bool precise_zoom,
				  bool smooth_deltas);

    GUI_ViewFlag myFlag;

// transform information

    gui_View	 myView;
    fpreal	 myDx, myDy;
    static orientation myOrientationMode;
    static int	 myTumbleStyle;
    static int	 myAltKeyViewControls;
    static int	 myScrollStyle;
    static axis	 myDefaultHomingStyle;

    fpreal	 myRx, myRy, myRz;	// Used by euler method.  In degrees

    // always call updateTransformMatrix before using myTransformMatrix.
    UT_Matrix4D	 myTransformMatrix;	// this one is generated from the above
    UT_Matrix4D	 myItransformMatrix;	// inverse of myTransformMatrix;

    GRgetTransformCallback	 myTransformCallback;
    void			*myTransformCallbackData;

// home information to be used in scaling of view transformation
    fpreal	 myLastHomeDistance;
    fpreal	 myLastHomePixelWidthOBSOLETE;
    fpreal	 myLastHomeAdjustedBoxWidth;
    fpreal	 myLastHomeOrthoWidth;
    fpreal	 myLastHomeRadius;

    // myLastHomeAdjustedBoxWidth supercedes myLastHomePixelWidth, but to load
    // view transforms from old hip files without screwed up scrolling, we may
    // need to use the obsolete quantity until the next manual home operation.
    bool	 myObsoleteScrollIsActive;

    axis	 myHomeAxis;
    UT_Matrix3R	 myCustomHomeRot;

    // A collection of data needed for calculating the appropriate changes to
    // the view transform to perform view operations under continuous export
    // of view changes to a camera.
    class ContinuousExportCache
    {
    public:
	ContinuousExportCache() {}
	
	UT_Matrix4D	 myRotateMatrix;
	fpreal		 myAdjustedNear;
	fpreal		 myAdjustedX;
	fpreal		 myAdjustedY;
    };

    typedef ContinuousExportCache	 ViewOpCache;

    ViewOpCache	 myContinuousExportCache;
    bool	 myContinuousExportInProgress;

    // These might be needed for other guys.
    int		 myViewportLeft,   myViewportRight;
    int		 myViewportBottom, myViewportTop;

    // The view is a subregion of the viewport that represents the current
    // "camera" view.  Aspect ratio bars are used to dim the parts of the
    // viewport not in this view.  The window coordinates are relative to
    // this view, not the viewport.
    int		 myViewLeft,   myViewRight;
    int		 myViewBottom, myViewTop;

    int		 myVersion;

    UT_Matrix4D	 *myProjectionOverride;
};


#ifdef __DOCUMENTATION__

FOV can be computed from aperture and focal length.

	<--aperture--->
	|             |
	|             |
        +------+------+-----------
	 \     |     /		 ^
	  \    |    /		 |
	   \_--+--_/	       Focal
Unzoomed    \  |  /	       Length
     FOV --> \ | /		 |
	      \|/		 v
	       +------------------

myFullW and myFullH are the dimensions of the plane onto which the
image is projected before any adjustments for zoom or window parms.
the plane is located at "myAdjustedNear" along the z axis.
After taking the zoom and win channels into account, myAdjustedX and
myAdjustedY refer to the center of the view on the same plane, and
myAdjustedW and myAdjustedH contain the dimensions of the adjusted view.

#endif

#endif
