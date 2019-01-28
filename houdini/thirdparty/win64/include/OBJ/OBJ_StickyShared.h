/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_StickyShared.h (Custom Library, C++)
 *
 * COMMENTS:    A common parent class for the sticky and sticky-blend objects.
 *
 */

#ifndef __OBJ_StickyShared__
#define __OBJ_StickyShared__

#include "OBJ_Null.h"

#include "OBJ_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3Array.h>
#include <GU/GU_DetailHandle.h>
#include <GA/GA_Types.h>

enum OBJ_StickySharedIndex
{
    I_STICKYSHAREDATTRIB = I_N_NULL_INDICES,
    I_STICKYSHAREDORIENT,
    I_STICKYSHAREDFETCHWORLD,
    I_STICKYSHAREDWRAPU,
    I_STICKYSHAREDWRAPV,
    I_STICKYSHAREDURANGE,
    I_STICKYSHAREDVRANGE,
    I_STICKYSHAREDASSUMEUVCONSTANT,
    I_STICKYSHAREDCLEARUV,

    I_N_STICKYSHARED_INDICES
};

class OBJ_UVCache;
class GEO_Vertex;
class GEO_Hull;

class OBJ_API OBJ_StickyShared : public OBJ_Null
{
public:
				 OBJ_StickyShared(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_StickyShared();

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    static OP_Node	         *myConstructor(OP_Network *net,
					    const char *name,
					    OP_Operator *entry);

    static int			 clearUVCacheStatic(void *, int, fpreal, 
						    const PRM_Template *);
    void			 clearUVCache();

    // 2d transform code:
    virtual bool		 get2DWorldTransform(UT_DMatrix4 &mat, 
						     OP_Context &context)
				 { mat.identity(); return true; } 


    virtual void		 STICKYGEOPATH(UT_String &str, fpreal t) {}
    void			 STICKYATTRIB(UT_String &str, fpreal t)
				 {
				     evalString(str, "stickyattrib", 0, t);
				 }
    virtual void		 STICKYUV(UT_Vector2R &uv, fpreal t) {}
    virtual fpreal		 STICKYROT(fpreal t) { return 0; }
    bool			 FETCHWORLD()
				 {
				     return evalInt("fetchworld", 0, 0.0f);
				 }
    bool			 FETCHORIENT()
				 {
				     return evalInt("stickyorient", 0, 0.0f);
				 }

    bool			 ASSUMEUVCONSTANT(fpreal t)
				 {
				     return evalInt("stickyuvconstant", 0, t);
				 }

    bool			 WRAPU(fpreal t)
				 {
				     return evalInt("stickywrapu", 0, t);
				 }
    bool			 WRAPV(fpreal t)
				 {
				     return evalInt("stickywrapu", 0, t);
				 }
    
    void			 STICKYURANGE(UT_Vector2R &urange, fpreal t)
				 {
				     urange.x() = evalFloat("stickyurange",0,t);
				     urange.y() = evalFloat("stickyurange",1,t);
				 }
    void			 STICKYVRANGE(UT_Vector2R &vrange, fpreal t)
				 {
				     vrange.x() = evalFloat("stickyvrange",0,t);
				     vrange.y() = evalFloat("stickyvrange",1,t);
				 }
    
    // This gets an aproximation of how many real world units should
    // map into one UV unit.
    // It uses the current U/V location to seed itself, or the
    // last known good value otherwise.
    fpreal			 getWorldToUVScaleFactor(fpreal t);

    // This returns if the V axis is inverted with respect to the 
    // canonical coordinate system.
    bool			 isVAxisInverted() const
				 { return myVAxisInvert; }

protected:
    void			 zeroPrimCacheValues(const GU_Detail *gdp,
						     GA_Offset prim_off);
    void			 buildHullCacheValues(const GU_Detail *gdp,
					const char *uvname,
					const GEO_Hull *hull);
    void			 buildPrimCacheValues(const GU_Detail *gdp,
					const char *uvname,
					GA_Offset prim_off);
    void			 getVertexOrientation(UT_Vector3 &nml,
					UT_Vector3 &uaxis,
					UT_Vector3 &vaxis,
					const GU_Detail *gdp,
					const char *uvname,
					GA_Offset v1,
					GA_Offset v2,
					GA_Offset v3);
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const { return 0; }

    virtual OP_ERROR		 cookMyObj(OP_Context &context);

    virtual void		 getUV(UT_Vector2R &uv, OP_Context &context) {}

    // Rebuilds the UV cache, if necessary.
    void			 rebuildUVCache(fpreal t);

    GU_DetailHandle		 getStuckGdp(fpreal t);

    // Rebuild input dependencies
    virtual void		 createInputDependencies(fpreal t);
    virtual OP_ERROR		 lockStickyInputs(OP_Context& context);
    virtual void		 unlockStickyInputs(OP_Context &context);

private:
    OBJ_UVCache			*myUVCache;
    UT_Vector3Array		 myNmlCache;
    UT_Vector3Array		 myUAxisCache;
    UT_Vector3Array		 myVAxisCache;
    UT_Array<GA_OffsetArray>	 myRingZero;
    fpreal			 myLastScaleFactor;
    bool			 myVAxisInvert;
};

#endif

