/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIMZ_Utils_h__
#define __SIMZ_Utils_h__

#include "SIMZ_API.h"
#include <SIM/SIM_Names.h>

// Parameter and SimpleData names for data.
#define SIM_NAME_CONTEXTINDEX		"contextindex"
#define SIM_NAME_DATAPATH		"datapath"
#define SIM_NAME_EXTRAATTRIBUTES	"extraattribs"
#define SIM_NAME_GENERATOR		"generator"
#define SIM_NAME_HASHADPARMSSET		"hashadparmsset"
#define SIM_NAME_INHERITVELOCITY	"inheritvelocity"
#define SIM_NAME_LIFESPAN		"lifespan"
#define SIM_NAME_MAKEPOINTSPARTICLES	"makepointsparticles"
#define SIM_NAME_NEEDSRESET		"needsreset"
#define SIM_NAME_OBJPATH		"objpath"
#define SIM_NAME_POINTGROUP		"pointgroup"
#define SIM_NAME_POINTINDEX		"pointindex"
#define SIM_NAME_POPPATH		"poppath"
#define SIM_NAME_PRIMGROUP		"primgroup"
#define SIM_NAME_ROTATETONORMAL		"nml"
#define SIM_NAME_SCRIPT			"script"
#define SIM_NAME_SOPPATH		"soppath"
#define SIM_NAME_TIME			"time"
#define SIM_NAME_TRANSFORMSOPOUTPUT	"transformsopoutput"
#define SIM_NAME_USETRANSFORM		"usetransform"
#define SIM_NAME_LETSOPSINTERPOLATE	"letsopsinterpolate"
#define SIM_NAME_SOLVEINITFRAME		"solveinitframe"

SIMZ_API void initializeSIMZ();

#endif

