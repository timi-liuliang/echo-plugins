/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GAS_Utils_h__
#define __GAS_Utils_h__

#include "GAS_API.h"

#include <SIM/SIM_Utils.h>
#include <UT/UT_SysSpecific.h>
#include <UT/UT_Vector3.h>

// Parameter and SimpleData names for data.

#define GAS_NAME_COLLISION		"collision"
#define GAS_NAME_COLLISIONVALUE		"collisionvalue"
#define GAS_NAME_COLLISIONVELOCITY	"collisionvelocity"
#define GAS_NAME_COLLISIONINDEX		"collisionindex"
#define GAS_NAME_DENSITY		"density"
#define GAS_NAME_DIVERGENCE		"divergence"
#define GAS_NAME_FIELD			"field"
#define GAS_NAME_FIELDDEST		"fielddst"
#define GAS_NAME_FIELDSOURCE		"fieldsrc"
#define GAS_NAME_GEOMETRY		"geo"
#define GAS_NAME_INDEX			"index"
#define GAS_NAME_INDEXSOURCE		"indexsrc"
#define GAS_NAME_PRESSURE		"pressure"
#define GAS_NAME_PTGROUP		"ptgroup"
#define GAS_NAME_SINK			"sink"
#define GAS_NAME_SOURCE			"source"
#define GAS_NAME_STRAIN			"strain"
#define GAS_NAME_SURFACE		"surface"
#define GAS_NAME_USETIMESTEP		"usetimestep"
#define GAS_NAME_TIMESCALE		"timescale"
#define GAS_NAME_TEMPERATURE		"temperature"
#define GAS_NAME_VELOCITY		"velocity"
#define GAS_NAME_TRACKERADDR		"address"
#define GAS_NAME_TRACKERPORT		"port"
#define GAS_NAME_JOBNAME		"jobname"

// Call this initialization function to register all data types, solvers,
// and colliders in this library.
GAS_API void	 initializeGAS();

#endif
