/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __RBD_Utils_h__
#define __RBD_Utils_h__

#include "RBD_API.h"

#include <SIM/SIM_Utils.h>
#include <UT/UT_SysSpecific.h>
#include <UT/UT_Vector3.h>

// Parameter and SimpleData names for data.
#define RBD_NAME_ITENSOR		"itensor"
#define RBD_NAME_ITENSORLOCALINV	"itensorlocalinv"
#define RBD_NAME_MASS			"mass"
#define RBD_NAME_COMPUTEMASS		"computemass"
#define RBD_NAME_DENSITY		"density"
#define RBD_NAME_COMPUTECOM		"computecom"
#define RBD_NAME_COMPUTEINERTIALTENSOR	"computeinertialtensor"
#define RBD_NAME_COMPUTEINERTIALTENSORTYPE "computeinertialtensortype"
#define RBD_NAME_INERTIALTENSORSTIFFNESS "inertialtensorstiffness"
#define RBD_NAME_USEPOINTVELOCITY	"usepointvelocity"
#define RBD_NAME_USESDFVELOCITY		"usevolumevelocity"
#define RBD_NAME_INHERITVELOCITY	"inheritvelocity"
#define RBD_NAME_GLUEOBJECT		"glueobject"
#define RBD_NAME_GLUETHRESHOLD		"gluethreshold"
#define RBD_NAME_GLUEIMPULSE		"glueimpulse"
#define RBD_NAME_GLUEIMPULSEHALFLIFE	"glueimpulsehalflife"
#define RBD_NAME_CULLMODE		"cullmode"	
#define RBD_NAME_ADDIMPACTS		"addimpacts"
#define RBD_NAME_CONTACTGROUPMETHOD	"contactgroupmethod"
#define RBD_NAME_CONTACTGROUPTOL	"contactgrouptol"
#define RBD_NAME_POLYCONVERT		"polyconvert"
#define RBD_NAME_POLYLOD		"polylod"
#define RBD_NAME_ADDBARYCENTER		"addbarycenter"
#define RBD_NAME_TRIANGULATE		"triangulate"


// Call this initialization function to register all data types, solvers,
// and colliders in this library.
RBD_API void	 initializeRBD();


class RBD_SphereTreeNode;
#if 0
class RBD_API rbd_isect : public sim_isect
{
public:
    rbd_isect(const UT_Vector3 &position, 
	     const UT_Vector3 &normal,
	     fpreal isectdepth)
	    : sim_isect(position, normal),
	      depth (isectdepth)
    { }
    fpreal depth;
    virtual fpreal getPriority() const 
    { return depth; }
};
#endif

class RBD_API rbd_nodeentry
{
public:
    const RBD_SphereTreeNode 	*node;

    /// This is the minimum value the sdf can have within this
    /// node.
    fpreal		 dist_minus_radius;
};

class RBD_API rbd_nodeentryCompare
{
public:
    bool operator()(rbd_nodeentry *a, rbd_nodeentry *b) const;
};

#endif


