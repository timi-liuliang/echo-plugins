/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Error__
#define __SIM_Error__

#include <UT/UT_Error.h>

enum SIM_ErrorCodes
{
    SIM_MESSAGE = 0,		// %s
    SIM_INSUFFICIENTCACHE,	// Cache needs to have %s entries.
    SIM_MISSINGDATA,		// Required data %s is missing.
    SIM_COLLISIONS,		// Unresolved collisions detected.
    SIM_AFFECTORCYCLE,		// Cyclic metaobject affector relationship.
    SIM_MULTIPLEMUTUAL,		// Object in multiple metaobjects.
    SIM_BADDATALOCATION,	// This data violates a data hint.
    SIM_BADSUBDATA,		// A child of this data violates a data hint.
    SIM_BADSTATEFILE,		// Loaded a corrupted simulation state file.
    SIM_CONUNPROCESSED,		// Constraint was not processed by solver.
    SIM_CONBADOBJECT,		// Constraint has a bad object reference.
    SIM_CONBADPOINT,		// Constraint has a bad point number.
    SIM_POINTMISMATCH,		// Points do not match.
    SIM_BADGEOBLEND,		// Bad geometry blend.
    SIM_BADSOPPATH,		// Can't find SOP %s.
    SIM_BADPOPPATH,		// Can't find POP %s.
    SIM_BADOBJPATH,		// Can't find Object %s.
    SIM_BADOPPATH,		// Can't find node %s.
    SIM_BADPRIMARYSOLVER,	// Primary solver outof range.
    SIM_TOPOLOGYMISMATCH,	// Geometry topologies don't match for %s.
    SIM_BADVOLUME,		// Specified primitive in %s isn't a volume
    SIM_MISSINGATTRIBUTE,	// Required attribute %s is missing.
    SIM_BADSOPCOOK,		// Error cooking SOP %s.
    SIM_BADPOPCOOK,		// Error cooking POP %s.
    SIM_BADOBJCOOK,		// Error cooking Object %s.
    SIM_BADOPCOOK,		// Error cooking node %s.
    SIM_NO_OPENCL,		// OpenCL not supported in this version.
};

#endif
