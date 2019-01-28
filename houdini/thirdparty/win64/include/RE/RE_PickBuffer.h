/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:
 *	Holds a frame buffer object that can receive pick information.
 */

#ifndef __RE_PickBuffer_h__
#define __RE_PickBuffer_h__

#include "RE_API.h"
#include "RE_Texture.h"
#include "RE_OGLFramebuffer.h"

class UT_TokenString;
class RE_Geometry;
class re_BoxPickData;

class RE_API RE_PickBuffer
{ 
public:
		 RE_PickBuffer();
		~RE_PickBuffer();

    bool		 resize(RE_Render *r, int w, int h);
    void		 invalidate();
    void		 startRender(RE_Render *r);
    void		 drawToBuffer(RE_Render *r,
				RE_BufferAttachment buftype = RE_COLOR_BUFFER,
				int colorbufnum = 0);
    void		 drawToAllBuffers(RE_Render *r);
    void		 endRender(RE_Render *r, UT_TokenString *&new_hash);

    // The picks returned by this method are sequential in pick_dist in that
    // if a pick_dist of d returns a sequence of n picks, these will also be
    // the first n picks in the sequence for a pick_dist of d + 1.
    //
    // The sample_counts_by_dist parameter is provided to aid in determining
    // which of the returned picks would also be picks for distances less
    // than pick_dist.  If a nonzero pointer is supplied, it will be set to
    // point to an array such that the first (*sample_counts_by_dist)[i] of
    // the returned picks would also be picks for a pick_dist of i, for i
    // in [0 .. RE_MAX_PICK_DIST].  This array pointer will only be set if
    // this method returns a nonzero count, and should *NOT* be deleted.
    int			 getPicks(RE_Render *r,
				int x, int y, int pick_dist,
				RE_VertexArray *&high_array,
				RE_VertexArray *&low_array,
				RE_VertexArray *&z_array,
				const UT_IntArray **sample_counts_by_dist = 0);

    int			 getPicks(RE_Render *r,
				const UT_DimRect &area,
				uint8 *texture,
				UT_Vector3iArray &pick_high,
				UT_Vector3iArray &pick_low);
    RE_Texture		*getZTexture();

    bool		 initialized() const;
    bool		 checkHash(const UT_TokenString *hash) const;

    void		 saveBuffers(RE_Render *r,
				const char *path,
				const char *prefix,
				bool mid_render) const;

private:
    int			 normalPickFetch(RE_Render *r,
				const UT_DimRect &area,
				uint8 *texture,
				UT_Vector3iArray &pick_high,
				UT_Vector3iArray &pick_low);
    int			 slowPickFetch(RE_Render *r,
				const UT_DimRect &area,
				uint8 *texture,
				UT_Vector3iArray &pick_high,
				UT_Vector3iArray &pick_low);
    RE_FBOStatus	 create(RE_Render *r, int w, int h);
    void		 destroy();

    UT_UniquePtr<RE_OGLFramebuffer>	 myBuffer;
    UT_UniquePtr<RE_Texture>		 myHighIdTexture;
    UT_UniquePtr<RE_Texture>		 myLowIdTexture;
    UT_UniquePtr<RE_Texture>		 myZTexture;
    UT_UniquePtr<RE_Geometry>		 mySinglePickGeometry;
    UT_UniquePtr<re_BoxPickData>	 myBoxPickData;
    UT_TokenString			*myBufferHash;
    int					 myBufferVecSize;
    mutable int				 mySaveCount;
};

#endif
