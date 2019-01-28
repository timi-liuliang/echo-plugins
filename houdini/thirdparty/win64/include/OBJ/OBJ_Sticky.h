/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Sticky.h (Custom Library, C++)
 *
 * COMMENTS:    An object to fetch it's transform from another object.
 *
 */

#ifndef __OBJ_Sticky__
#define __OBJ_Sticky__

#include "OBJ_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3Array.h>
#include <GU/GU_DetailHandle.h>
#include "OBJ_StickyShared.h"

enum OBJ_StickyIndex
{
    I_STICKYGEOPATH = I_N_STICKYSHARED_INDICES,
    I_STICKYUV,
    I_STICKYROT,

    I_N_STICKY_INDICES
};

class obj_UVCache;
class GEO_Vertex;
class GEO_Hull;

class OBJ_API OBJ_Sticky : public OBJ_StickyShared
{
public:
				 OBJ_Sticky(OP_Network *net,
					   const char *name,
					   OP_Operator *op);

				 ~OBJ_Sticky() {}

    static OP_Node	         *myConstructor(OP_Network *net,
					    const char *name,
					    OP_Operator *entry);

    static PRM_Template		*getTemplateList();

    // 2d transform code:
    virtual bool		 get2DWorldTransform(UT_DMatrix4 &mat, 
						     OP_Context &context);

    virtual OBJ_Sticky		*castToOBJSticky()	{ return this; }

    virtual void		 STICKYGEOPATH(UT_String &str, fpreal t)
				 {
				     evalString(str, "stickysop", 0, t);
				 }
    virtual void		 STICKYUV(UT_Vector2R &uv, fpreal t)
			         {
				     uv.x() = evalFloat("stickyuv", 0, t);
				     uv.y() = evalFloat("stickyuv", 1, t);
				 }
    virtual fpreal		 STICKYROT(fpreal t)
				 {
				     return evalFloat("stickyrot", 0, t);
				 }

    void			 setSTICKYGEOPATH(UT_String &str,
					    CH_StringMeaning meaning, fpreal t)
				 {
				     setString(str, meaning, "stickysop", 0, t);
				 }
    void			 setSTICKYUV(const UT_Vector2R &uv, fpreal t)
				 {
				     setFloat("stickyuv", 0, t, uv.x());
				     setFloat("stickyuv", 1, t, uv.y());
				 }

protected:
    virtual void		 getUV(UT_Vector2R &uv, OP_Context &context);

    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return stickyIndirect; }

    static int			*stickyIndirect;
    
};
#endif

