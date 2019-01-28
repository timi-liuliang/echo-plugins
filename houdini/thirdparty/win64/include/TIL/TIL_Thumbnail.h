/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Thumbnail.h (IPR Library, C++)
 *
 * COMMENTS:
 */

#ifndef __TIL_Thumbnail__
#define __TIL_Thumbnail__

#include "TIL_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_StringHolder.h>

class TIL_Raster;
class UT_Options;
class UT_JSONWriter;

/// Rendering parms for thumbnail generation.  Some options are currently
/// not supported.
class TIL_API TIL_ThumbnailParms
{
public:
    TIL_ThumbnailParms()
	: myTransform(1)
	, mySurface()
	, mySurfaceSource()
	, myDisplace()
	, myDisplaceSource()
	, myRes(64)
	, myShapeFile("shadersphere.bgeo")
    { }
    virtual ~TIL_ThumbnailParms()
    { }

    virtual void		 setOptions(const UT_Options &options);

    int				 res() const
				 { return myRes; }
    const UT_StringHolder	&shapeFile() const
				 { return myShapeFile; }
    const UT_Matrix4D		 transform() const
				 { return myTransform; }
    const UT_StringHolder	&surface() const
				 { return mySurface; }
    const UT_StringHolder	&surfaceArgs() const
				 { return mySurfaceArgs; }
    const UT_StringHolder	&surfaceSource() const
				 { return mySurfaceSource; }
    const UT_StringHolder	&displace() const
				 { return myDisplace; }
    const UT_StringHolder	&displaceArgs() const
				 { return myDisplaceArgs; }
    const UT_StringHolder	&displaceSource() const
				 { return myDisplaceSource; }

    void	 setRes(int res)
		 { myRes = res; }
    void	 setShapeFile(const UT_StringHolder &s)
		 { myShapeFile = s; }
    void	 transform(const UT_Matrix4D &x)
		 { myTransform *= x; }
    void	 setCamera(const UT_Vector3 &camera_origin)
		 { }
    void	 setTransform(const UT_Matrix4D &x)
		 { myTransform = x; }
    void	 setSurface(const UT_StringHolder &shader_name,
			const UT_StringHolder &args)
		 {
		    mySurface = shader_name;
		    mySurfaceArgs = args;
		 }
    void	 setSurfaceSource(const UT_StringHolder &s)
		 { mySurfaceSource=s; }
    void	 setDisplace(const UT_StringHolder &shader_name,
			const UT_StringHolder &args)
		 {
		    myDisplace = shader_name;
		    myDisplaceArgs = args;
		 }
    void	 setDisplaceSource(const UT_StringHolder &s)
		 { myDisplaceSource=s; }

    /// @{
    /// Dump parameters
    void	dump() const;
    void	dump(UT_JSONWriter &w) const;
    /// @}

private:
    static UT_DMatrix4	 shapeTransform(const char *values);

    UT_StringHolder	 myShapeFile;
    UT_StringHolder	 mySurface;
    UT_StringHolder	 mySurfaceArgs;
    UT_StringHolder	 mySurfaceSource;
    UT_StringHolder	 myDisplace;
    UT_StringHolder	 myDisplaceArgs;
    UT_StringHolder	 myDisplaceSource;
    UT_Matrix4D		 myTransform;
    int			 myRes;
};

/// Base class to generate a thumbnail for a shader
///
/// For each thumbnail required, create an TIL_Thumbnail object.  This acts
/// as a handle to a render.  By default, you'll get a sphere with the shader
/// specified in the parms applied to it.
class TIL_API TIL_Thumbnail
{
public:
    /// Reasons for update
    enum UpdateReason
    {
	TIL_THUMB_ERROR,	// Error creating thumbnail
	TIL_THUMB_UPDATE,	// Image has new pixels
	TIL_THUMB_DONE		// Render completed
    };

    TIL_Thumbnail();
    virtual ~TIL_Thumbnail();

    /// If you want to get updates on the rendering progress, you can implement
    /// this method.
    ///
    /// @warning This method will likely be called by a child thread.
    virtual void		 update(UpdateReason reason);

    /// Return the percent complete (0-100).
    virtual int			 percentComplete() const = 0;
    /// Return any error message.
    virtual UT_StringHolder	 errorMessage() const = 0;

    /// Tracks whether this thubnail is out of date.
    bool			 needsRefresh() const;
    void			 setNeedsRefresh();

    /// Start thumbnail generation.
    virtual void		 start(const TIL_ThumbnailParms &parms) = 0;
    /// Return the RGBA raster.  It's possible the raster is incomplete, but
    /// the raster will be updated as the render proceeds.
    virtual const TIL_Raster	*getImage() const = 0;

private:
    bool			 myNeedsRefresh;
};

#endif
