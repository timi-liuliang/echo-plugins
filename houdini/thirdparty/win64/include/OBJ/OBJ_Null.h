/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Null.h (Custom Library, C++)
 *
 * COMMENTS:    An object to describe a motion capture handle
 *
 */

#ifndef __OBJ_Null__
#define __OBJ_Null__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"

enum OBJ_NullIndex
{
    I_RENDERSPACE = I_N_GEO_INDICES,
    I_GEOSCALE,
    I_DISPLAYICON,
    I_CONTROLTYPE,
    I_ORIENTATION,
    I_SHADEDMODE,
    I_GEOCENTER,
    I_GEOROTATE,
    I_GEOSIZE,
    I_GEOCUSTOM,
    I_N_NULL_INDICES	// should always be last in the list
};

class OBJ_API OBJ_Null : public OBJ_Geometry
{
public:
				 OBJ_Null(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Null();

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		*getTemplateList();

    virtual OBJ_OBJECT_TYPE	 getObjectType() const { return OBJ_STD_NULL; }

    int				RENDERSPACE(fpreal t)
				{
				    return evalInt("renderspace",
						&getIndirect()[I_RENDERSPACE],
						0, t);
				}
    
    fpreal			GEOSCALE(fpreal t, int i=0)
				{
				    return evalFloat("geoscale", 
						&getIndirect()[I_GEOSCALE], 
						0, t)
				    * evalFloat("geosize", 
						&getIndirect()[I_GEOSIZE], 
						i, t);
				}

    fpreal			GEOCENTER(fpreal t, int i=0)
				{
				    return evalFloat("geocenter", 
						&getIndirect()[I_GEOCENTER], 
						i, t);
				}

    fpreal			GEOROTATE(fpreal t, int i=0)
				{
				    return evalFloat("georotate", 
						&getIndirect()[I_GEOROTATE], 
						i, t);
				}

    void			GEOCUSTOM( UT_String &s )
				{
				    evalStringRaw( s, 
						"geocustom", 
						&getIndirect()[I_GEOCUSTOM], 
						0, 0);
				}
    
    int				DISPLAYICON(fpreal t)	
				{ 
				    return evalInt("displayicon",	
						&getIndirect()[I_DISPLAYICON],
						0, t); 
				}
    
    int				CONTROLTYPE(fpreal t)	
				{ 
				    return evalInt("controltype",
						&getIndirect()[I_CONTROLTYPE],
						0, t); 
				}
    
    int				ORIENTATION(fpreal t)	
				{ 
				    return evalInt("orientation",
						&getIndirect()[I_ORIENTATION],
						0, t); 
				}
    
    int				SHADEDMODE(fpreal t)	
				{ 
				    return evalInt("shadedmode",
						&getIndirect()[I_SHADEDMODE],
						0, t); 
				}

    virtual OBJ_Null	       *castToOBJNull() { return this; }

    virtual void		syncNodeVersion(const char *old_version,
						const char *cur_version,
                                                bool *node_deleted);

protected:
    virtual bool	 	 updateParmsFlags();
    
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return nullIndirect; }

    virtual int			 isObjectRenderable(fpreal t) const;

private:
    static int			*nullIndirect;
};

#endif
