/** \file mikktspace/mikktspace.h
 *  \ingroup mikktspace
 */
/**
 *  Copyright (C) 2011 by Morten S. Mikkelsen
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 */

#ifndef __GU_MIKKTSPACE_H__
#define __GU_MIKKTSPACE_H__

#include "GU_API.h"
#include <GA/GA_Handle.h>
#include <UT/UT_Vector3.h>

class GU_Detail;
class GA_PrimitiveGroup;

class GU_API GU_MikkT
{
public:

    //! Compute MikkT tangents
    /// The result of the computation will be written out to per-vertex
    /// attributes: tangents, bitangents, and signs.  Not all output handles
    /// need to be valid (invalid handles will be ignored).
    static bool computeTangentsBasic(const GU_Detail *gdp,
				     const GA_PrimitiveGroup *group,
				     const UT_StringHolder &uvattrname,
				     const GA_RWHandleV3 &tangents, 
				     const GA_RWHandleV3 &bitangents, 
				     const GA_RWHandleF &signs);

};

#endif
