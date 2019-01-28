/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_Names.h (GA Library, C++)
 *
 * COMMENTS:    Cached const UT_StringHolder's for common attribute names,
 *              so that the hash and length only get computed once,
 *              and equality can be checked using the pointers first.
 */

#pragma once

#ifndef __GA_Names__
#define __GA_Names__

#include "GA_API.h"
#include <UT/UT_StringHolder.h>

namespace GA_Names
{
    extern GA_API const UT_StringHolder P;
    extern GA_API const UT_StringHolder Pw;
    extern GA_API const UT_StringHolder N;
    extern GA_API const UT_StringHolder uv;
    extern GA_API const UT_StringHolder v;
    extern GA_API const UT_StringHolder w;
    extern GA_API const UT_StringHolder shop_materialpath;
    extern GA_API const UT_StringHolder material_override;
    extern GA_API const UT_StringHolder material_stylesheet;
    extern GA_API const UT_StringHolder ogl_shop;
    extern GA_API const UT_StringHolder Cd;
    extern GA_API const UT_StringHolder Alpha;
    extern GA_API const UT_StringHolder com;
    extern GA_API const UT_StringHolder dist;
    extern GA_API const UT_StringHolder lod;
    extern GA_API const UT_StringHolder rest;
    extern GA_API const UT_StringHolder mass;
    extern GA_API const UT_StringHolder drag;
    extern GA_API const UT_StringHolder tension;
    extern GA_API const UT_StringHolder springk;
    extern GA_API const UT_StringHolder life;
    extern GA_API const UT_StringHolder age;
    extern GA_API const UT_StringHolder up;
    extern GA_API const UT_StringHolder orient;
    extern GA_API const UT_StringHolder id;
    extern GA_API const UT_StringHolder pscale;
    extern GA_API const UT_StringHolder creaseweight;
    extern GA_API const UT_StringHolder area;
    extern GA_API const UT_StringHolder perimeter;
    extern GA_API const UT_StringHolder width;
    extern GA_API const UT_StringHolder instancefile;

    extern GA_API const UT_StringHolder layermerge;
    extern GA_API const UT_StringHolder segs;
    extern GA_API const UT_StringHolder div;
    extern GA_API const UT_StringHolder lage;
    extern GA_API const UT_StringHolder arc;
    extern GA_API const UT_StringHolder gen;

    extern GA_API const UT_StringHolder boneCapture;
    extern GA_API const UT_StringHolder tubeCapt;
    extern GA_API const UT_StringHolder wireCapture;
    extern GA_API const UT_StringHolder clothCapture;
    extern GA_API const UT_StringHolder metaCapture;
    extern GA_API const UT_StringHolder muscleCapture;

    extern GA_API const UT_StringHolder name;
    extern GA_API const UT_StringHolder rixlate;
    extern GA_API const UT_StringHolder varmap;
    extern GA_API const UT_StringHolder layercount;
    extern GA_API const UT_StringHolder currentlayer;

    extern GA_API const UT_StringHolder trans;
    extern GA_API const UT_StringHolder scale;
    extern GA_API const UT_StringHolder rot;
    extern GA_API const UT_StringHolder pivot;
    extern GA_API const UT_StringHolder transform;

    extern GA_API const UT_StringHolder metaExpression;

    extern GA_API const UT_StringHolder _3d_hidden_primitives;

    // these two are for path objects
    extern GA_API const UT_StringHolder twist;
    extern GA_API const UT_StringHolder initial_twist;

    /// Remaps attribute names to more readable label names.
    GA_API const UT_StringHolder &
    mapAttribNameToLabel(const UT_StringRef &name);

    // Internal: DO NOT USE IN HDK!

    extern GA_API const UT_StringHolder _vertex_pointRef;
    extern GA_API const UT_StringHolder _vertex_primitiveRef;
    extern GA_API const UT_StringHolder _point_vertexRef;
    extern GA_API const UT_StringHolder _point_vertexPrevRef;
    extern GA_API const UT_StringHolder _point_vertexNextRef;
    extern GA_API const UT_StringHolder _vertex_hedgePrevRef;
    extern GA_API const UT_StringHolder _vertex_hedgeNextRef;

    extern GA_API const UT_StringHolder internalN;
};

#endif
