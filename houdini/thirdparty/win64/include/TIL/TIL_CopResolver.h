/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_CopResolver.h ( TIL Library, C++)
 *
 * COMMENTS:
 *	Class to get a pointer to a COPs raster.  This class is used by the
 *	texture mapping code to resolve names of the form:
 *
 *		op:/comp/copnet/cop{C,A}[frame]
 *		file:filename{xres,yres}
 *
 *	The frame is optional.  Sub-classes don't need to worry about parsing
 *	out the [frame], this is all handled by the static methods.
 */

#ifndef __TIL_CopResolver__
#define __TIL_CopResolver__

#include "TIL_API.h"
#include "TIL_Defines.h"
#include <IMG/IMG_FileTypes.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_WorkBuffer.h>
#include <stddef.h>


#define TIL_DEFAULT_COLOR_PLANE	"(Default Color)"
#define TIL_DEFAULT_ALPHA_PLANE	"(Default Alpha)"
#define TIL_NO_PLANE		"(None)"

class TIL_Raster;
class TIL_Sequence;
class TIL_ImageSource;
class UT_String;

class TIL_API TIL_CopResolver
{
public:
	     TIL_CopResolver();
    virtual ~TIL_CopResolver();

    static void			 setResolver(TIL_CopResolver *resolver);
    static TIL_CopResolver	*getResolver();

    // Get an TIL_Raster associated with the COP in the path specified
    static TIL_Raster		*getRaster(const char *fullpath,
					    int &opid,
                                            bool *specific_frame = nullptr);
    static fpreal		 getRasterTime(int opid);
    static size_t		 getRasterVersion(int opid);

    /// Tell the resolver we're done with the raster.
    /// Some resolvers will delete rasters, others will hold onto them.  If the
    /// raster actually dirties/deletes the raster, @c doneWithRaster() will
    /// return true.
    static bool	doneWithRaster(const TIL_Raster *r);

    // Returns 1 if the frame is specified in the filename.  The id will be -1
    // if there is no such path.
    static int			 getIdFrame(const char *fullpath, int &id,
					    fpreal &frame);
    static void			 splitFile(const char *fullpath,
					   UT_String &name, 
					   int &xres,int &yres,
					   float &quality);

    // These split net/node methods are now out of style with networks
    // within networks.  Now you should specify full, absolute, paths.
    static int			 getNodeId(const char *net, const char *node);
    static void			 getNodeRes(const char *net, const char *node,
					    int &xres, int &yres);
    
    static int			 getNodeId(const char *fullpath);
    static void			 getNodeRes(const char *fullpath, 
					    int &xres, int &yres);
    
    static void			 getFileRes(const char *file,
					    int &xres, int &yres);
    
    static int			 splitPath(const char *cpath, int &id,
					   fpreal &frame,
					   UT_WorkBuffer &color, int &cindex,
					   UT_WorkBuffer &alpha, int &aindex,
					   int &xres, int &yres);

    // builds menus for plane selection (COPs2 only).
    static void			 buildColorMenu(const char *net,
						const char *node,
						UT_ValArray<char *> &items);

    static void			 buildAlphaMenu(const char *net,
						const char *node,
						UT_ValArray<char *> &items);

    static bool			 canResolve(const char *fullpath);

    // These methods should really only be used inside the TIL_CopResolver
    // class.  They shouldn't be called directly.
    virtual int		 resolveId(const char *fullpath) = 0;
    
    virtual TIL_Raster	*resolveRaster(int id) = 0;
    virtual TIL_Raster	*resolveRasterFrame(int id, fpreal frame,
					    int xres=0, int yres=0) = 0;
    virtual TIL_Raster	*resolveRasterPlane(int id, fpreal frame,
					    const char *color, int cindex,
					    const char *alpha, int aindex,
					    int xres=0, int yres = 0, 
					    TIL_DataFormat f =
						TILE_MAX_DATA_FORMAT) = 0;

    // Returns whether or not the resolver knows how to handle this path.
    virtual bool	 resolveIsPossible(const char *fullpath) = 0;

    virtual TIL_Raster  *getFileRaster(const char *file,
				       IMG_DataType type = IMG_DT_ANY,
				       int xres = 0, int yres = 0);
    virtual TIL_Raster	*getNodeRaster(const char *fullpath,
				       const char *cmenu, const char *amenu,
				       bool override_frame = false,
				       fpreal frame = 1.0,
				       TIL_DataFormat f =
					   TILE_MAX_DATA_FORMAT);

    virtual void	getRes(int id, int &xres, int &yres);

    virtual TIL_Sequence *getSequence(int id);

    /// This cooks a bunch of planes simulataneously, and adds the created
    /// rasters to 'new_rasters'. You are responsible for deleting them.
    virtual bool	resolveRasters(int id,
				       const UT_StringArray &plane_names,
				       fpreal t,
				       UT_ValArray<TIL_Raster *> &new_rasters,
				       TIL_DataFormat format=TILE_MAX_DATA_FORMAT);
				       

    virtual void	buildPlaneMenu(const char *net, const char *node,
				       UT_ValArray<char *> &items);

    virtual TIL_ImageSource *resolveImageSource(int id);

    /// Caller is finished with the raster for now, so delete memory if
    /// possible.  Returns true if the raster is no longer valid, but false if
    /// the raster hasn't been touched (and is still valid).  This is
    /// implementation dependent.
    virtual bool	returnRaster(const TIL_Raster *raster);

    // If the id returned is really an OP_Node, then this will return 1.
    virtual int		 isOpBased();

    virtual fpreal	 getTimeForRaster(int id);
    virtual size_t	 getVersionForRaster(int id);

protected:
    // Returns 1 if the frame is defined, 0 otherwise.  It's possible that
    // the name returned may be a reference to the path.  Please harden if
    // you need to keep it around.
    int			splitPath(const char *fullpath,
				  UT_String &name, fpreal &frame);
};

#endif
