/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_ExtractGeo.h (Custom Library, C++)
 *
 * COMMENTS:    An object to fetch it's transform from another object.
 *
 */

#ifndef __OBJ_ExtractGeo__
#define __OBJ_ExtractGeo__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"

enum OBJ_ExtractGeoIndex
{
    I_EXTRACTGEOSRCPATH = I_N_GEO_INDICES,
    I_EXTRACTGEODSTPATH,
    I_EXTRACTGEOTRANSLATE,
    I_EXTRACTGEOROTATE,
//    I_EXTRACTGEOSCALE,

    I_N_EXTRACTGEO_INDICES
};

class OBJ_API OBJ_ExtractGeo : public OBJ_Geometry
{
public:
				 OBJ_ExtractGeo(OP_Network *net,
					   const char *name,
					   OP_Operator *op);

				 ~OBJ_ExtractGeo() {}

    static OP_Node	         *myConstructor(OP_Network *net,
					    const char *name,
					    OP_Operator *entry);

    static PRM_Template		*getTemplateList();

    void		 	 SRCPATH(UT_String &str, fpreal t)
				 {
				     evalString(str, "srcpath", 0, t);
				 }
    void		 	 DSTPATH(UT_String &str, fpreal t)
				 {
				     evalString(str, "dstpath", 0, t);
				 }
    bool			 EXTRACTTRANSLATE(fpreal t)
				 {
				     return evalInt("extracttranslate", 0, t);
				 }
    bool			 EXTRACTROTATE(fpreal t)
				 {
				     return evalInt("extractrotate", 0, t);
				 }
    bool			 EXTRACTSCALE(fpreal t)
				 {
				     return false;
//				     return evalInt("extractscale", 0, t);
				 }

protected:
    virtual OP_ERROR		 cookMyObj(OP_Context &context);

    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return extractgeoIndirect; }

    static int			*extractgeoIndirect;
    
};
#endif

