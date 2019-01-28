/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_AttribDefn_h__
#define __SIM_AttribDefn_h__

#include <GEO/GEO_Detail.h>

// Density of each point (float)
#define SIM_ATT_DENSITY	    GA_ATTRIB_POINT, "density", 1

// Classification of attributes
//
// * Storage: detail, primitive, point, vertex, edge
//   - Note that edge attributes are actually stored in the vertex
//   - If multiple storage is possible, shown from highest precedence to
//     lowest


// Storage: point
// Velocity of each point. Can be used to define initial velocity of each
// point.
#define SIM_ATT_VEL	    GEO_STD_ATTRIB_VELOCITY, \
					sizeof(UT_Vector3), GB_ATTRIB_VECTOR
// Storage: point
// Mass of each point, proportional to point density multiplier, triangle
// area surrounding point, and global density.
#define SIM_ATT_MASS	    GEO_STD_ATTRIB_MASS, \
					sizeof(float),	    GB_ATTRIB_FLOAT

// Storage: primitive
// Disable cloth/volume collisions.
#define SIM_ATT_NOCLOTHVOLUME \
			    "noclothvolume", \
					sizeof(int),	    GB_ATTRIB_INT
// Same as above, but allow float value - mostly just for convenience,
// since the Paint SOP requires floats.
#define SIM_ATT_NOCLOTHVOLUME_F \
			    "noclothvolume", \
					sizeof(float),	    GB_ATTRIB_FLOAT

#define SIM_ATT_NOPOINTVOLUME	GA_ATTRIB_POINT, "nopointvolume", 1
#define SIM_ATT_NOEDGEVOLUME 	GA_ATTRIB_POINT, "noedgevolume", 1

#define	SIM_ATT_SURFACEREPTYPE \
			    "surfacereptype", \
					sizeof(int),	    GB_ATTRIB_INT

// Used by colliders to help mark points that are fused together during
// simulation. It defines the "simulation point id" associated with a
// point, and id comparison can be used to detect fused points.
#define SIM_ATT_FUSEMAPPING "simfusemap",sizeof(int),       GB_ATTRIB_INT

#endif
