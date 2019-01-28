/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Rivet.h (Custom Library, C++)
 *
 * COMMENTS:    An object that attaches itself to another object (i.e.
 *		geometry). Based on three chosen points of the
 *		object, it constructs a local reference frame on the
 *		object's surface, and attaches itself to that frame.
 *		When surface moves, the attached rivet object also
 *		moves with it.
 *
 */

#ifndef __OBJ_Rivet__
#define __OBJ_Rivet__

#include "OBJ_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3Array.h>
#include <GU/GU_DetailHandle.h>
#include "OBJ_Null.h"

// order should correspond to OBJrivetTemplate
enum OBJ_RivetIndex
{
    I_RIVET_GEOPATH = I_N_NULL_INDICES,
    I_RIVET_POINTGROUP,
    I_RIVET_POINTWEIGHTS,
    I_RIVET_USEATTRIBS,
    I_RIVET_XATTRIBS,
    I_RIVET_ZATTRIBS,

    I_N_RIVET_INDICES
};


class OBJ_API OBJ_Rivet : public OBJ_Null
{
public:
				 OBJ_Rivet(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Rivet();

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		*getTemplateList();
    virtual OBJ_Rivet		*castToOBJRivet()	{ return this; }


    // accessors of the parameters
    void			 RIVET_GEOPATH( UT_String &str, fpreal t )
				 {
				     evalString(str, "rivetsop", 0, t);
				 }
    void			 RIVET_POINTGROUP(UT_String &str, fpreal t)
				 {
				     evalString(str, "rivetgroup", 0, t);
				 }
    bool			 RIVET_USE_ATTRIBS()
				 {
				     return evalInt("rivetuseattribs", 0, 0.0f);
				 }
    void			 RIVET_X_ATTRIB_NAME(UT_String &str, fpreal t)
				 {
				     evalString(str, "rivetxattrib", 0, t);
				 }
    void			 RIVET_Z_ATTRIB_NAME(UT_String &str, fpreal t)
				 {
				     evalString(str, "rivetzattrib", 0, t);
				 }

    void			 setRIVET_GEOPATH(UT_String &str,
					    CH_StringMeaning meaning, fpreal t)
				 {
				     setString(str, meaning, "rivetsop", 0, t);
				 }
    void			 setRIVET_POINTGROUP(UT_String &str,
					    CH_StringMeaning meaning, fpreal t)
				 {
				     setString(str, meaning, "rivetgroup", 0,t);
				 }

protected:
    virtual bool	 	 updateParmsFlags();
    
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return rivetIndirect; }

    virtual OP_ERROR		 cookMyObj(OP_Context &context);

    GU_DetailHandle		 getStuckGdp(fpreal t);

    virtual void		 buildPointGroupMenu( PRM_Name *menu,
					    int size, 
					    const PRM_SpareData *spare, 
					    const PRM_Parm *parm);
    fpreal			 RIVET_POINTWEIGHT( fpreal t, int k )
				 {
				     return evalFloat("rivetweights", k, t);
				 } 

private:
    // obtains array of points based on which a local surface frame is build
    // also, for each point, obtains normals (z-axis) and tangents (x-axis)
    // if the attribute names are provided for them by the rivet parameters
    void			 getRivetPoints( OP_Context &context,
						 SOP_Node *sop, 
						 UT_Vector3RArray &points,
						 UT_Vector3RArray &tangents,
						 UT_Vector3RArray &normals );

    // obtains the weights for the points 
    void			getPointWeights( fpreal t, 
						 UT_FprealArray &weights,
						 int point_count);

private:
    static int			*rivetIndirect;
};

#endif

