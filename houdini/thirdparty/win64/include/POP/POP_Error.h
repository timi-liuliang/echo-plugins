/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_Error__
#define __POP_Error__

enum POP_ErrorCodes
{
    POP_MESSAGE = 0,

    POP_BAD_VARIABLE,		// variable not defined
    POP_BAD_GROUP,    		// group doesn't exist
    POP_DUPLICATE_PRIMITIVE,   	// pop processes same primitive twice
    POP_INVALID_SRC,    	// invalid source
    POP_NO_DETAIL,    		// POP cannot be cooked. Missing detail.
    POP_ILLEGAL_GROUPNAME,	// not a valid name for a group
    POP_ILLEGAL_GROUPPREFIX,	// not a valid prefix for a group
    POP_BAD_SOP,    		// error cooking specified SOP
    POP_NEG_SPEEDLIMIT,     	// warning for negative speed limit
    POP_WARN_DISPLAY_FLAG,      // display POP outside cook chain
    POP_NO_ATTRIBUTE_NAME,	// missing attribute name
    POP_DUPLICATE_VARNAME,	// user variable already in use
    POP_NO_SOP_DETAIL,		// input sop cannot be cooked
    POP_INVALID_MULTI_OUTPUT,	// pop cannot provide multi output
    POP_VEX_ERROR,		// vex pop error
    POP_DRAG_MISSING_ATTR,	// Drag pop compaining of missing rot, angvel,
				// or torque attributes.
    POP_NONRBDPARTICLE,		// UnifyRBD source group contained a non-rbd
				// particle
    POP_NONEXISTANT_SRC_ATTRIB,	// No such attribute in source geometry
    POP_NONCONSTANT_TOPOLOGY,	// Topology of geometry changes with time
    POP_ERR_BADNODE,		// Node %s not found or wrong type.
    POP_WARN_BADSURFACESHOP,	// "%s" doesn't exist or isn't a surface shop.
    POP_WARN_BADOGLSURFACESHOP,	// "%s" doesn't export an OpenGL material.
    POP_ATTRIBUTE_INVALID,	// Invalid attribute specification: "%s"
    POP_UNSUPPORTED_PRIMITIVES,	// Geometry contains unsupported primitives.
    POP_SELF_POP_IMPORTED,	// Cannot use oneself as the source of import
    POP_NO_SPH_ATTRIBS,		// Cannot locate SPH attributes for SPH
				// Density Test POP
    POP_WARN_SAMPLE_FAILURE	// Sample failures exceeded threshold
};

#endif
