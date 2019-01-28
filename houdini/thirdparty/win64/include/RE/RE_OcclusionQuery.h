/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OcclusionQuery.h ( RE Library, C++)
 *
 * COMMENTS:
 *
 *    This class implements the GL_ARB_occlusion_query feature, which allows
 *    multi-pass rendering to determine how many pixels in a batch of drawing
 *    commands were accepted by Z-culling.
 *
 *    Only one query can be active at a time.
 *
 *    The body of this class is implemented in RE_OGLQuery.
 *
 *    If 'boolean_result' is passed to RE_OcclusionQuery, then a zero/one
 *    result will be passed back from getNumDrawn(). This may have a 
 *    performance benefit for some hardware. 
 */
#ifndef RE_OCCLUSION_QUERY_H
#define RE_OCCLUSION_QUERY_H

#include "RE_OGLQuery.h"

class RE_Render;

class RE_API RE_OcclusionQuery : public RE_OGLQuery
{
public:
    enum RE_OcclusionResult
    {
	BOOLEAN,
	NUM_SAMPLES
    };
    
		RE_OcclusionQuery(RE_OcclusionResult result = NUM_SAMPLES);
   virtual     ~RE_OcclusionQuery() {}

   virtual bool		begin(RE_Render *r);
   virtual void		end(RE_Render *r);

   bool		getAnySamplesDrawn(RE_Render *r)
		   { return (bool)(getIntResult(r)!=0); }

   int		getNumDrawn(RE_Render *r)
		{
		    if(myBooleanQuery)
			return (getIntResult(r)!=0) ? 1 : 0;
		    return getIntResult(r);
		}

private:
   bool		myBooleanQuery;
};

#endif
