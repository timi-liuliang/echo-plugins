/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_SoftTransform.h (GU Library, C++)
 *
 * COMMENTS:
 *      Applies a transformation based on a supplied distance attribute and a
 *      rolloff.
 *      This is related to the code in GEO_Transform.C, but we don't want to add
 *      yet another function to GEO_Detail, so it has been split off. The code
 *      there is still used for backwards compatibility, but should not be used
 *      in new code.
 *      Note that you are responsible for making empty group parameters become
 *      groups on the whole gdp. An empty group passed to one of these functions
 *      will cause an immediate return (meaning point normals won't be updated
 *      because nothing has been changed).
 */

#ifndef __GU_SoftTransform_h__
#define __GU_SoftTransform_h__

#include "GU_API.h"

#include <GA/GA_Handle.h>

#include <GEO/GEO_Detail.h>

#include <UT/UT_VectorTypes.h>

class GA_AttributeTransformer;
class GA_PointGroup;

class GEO_Delta;
class GEO_Rolloff;

void GU_API GUsoftTransformPoints(GEO_Detail* gdp,
                                const GEO_Detail::LocalXformContext* local,
                                const UT_XformOrder& order,
                                float tx, float ty, float tz,
                                float rx, float ry, float rz,
                                float sx, float sy, float sz,
                                float s_xy, float s_xz, float s_yz,
                                float px, float py, float pz,
                                float prx, float pry, float prz,
                                const GEO_Rolloff* rolloff,
                                const GA_ROHandleF& distances,
                                const GA_PointGroup* ptgroup,
                                const bool keep_normal_length = true,
                                GEO_Delta* geodelta = NULL,
                                const GA_RWHandleF* falloff_output = NULL,
                                bool updateaffectednormals = false,
                                bool updateallnormals = false,
                                const char* attribpattern = NULL);

void GU_API GUsoftTransformEdges(GEO_Detail* gdp,
                                const GEO_Detail::LocalXformContext* local,
                                const UT_XformOrder& order,
                                float tx, float ty, float tz,
                                float rx, float ry, float rz,
                                float sx, float sy, float sz,
                                float s_xy, float s_xz, float s_yz,
                                float px, float py, float pz,
                                float prx, float pry, float prz,
                                const GEO_Rolloff* rolloff,
                                const GA_ROHandleF& distances,
                                const GA_EdgeGroup* ptgroup,
                                const bool keep_normal_length = true,
                                GEO_Delta* geodelta = NULL,
                                const GA_RWHandleF* falloff_output = NULL,
                                bool updateaffectednormals = false,
                                bool updateallnormals = false,
                                const char* attribpattern = NULL);

void GU_API GUsoftTranslatePoints(GEO_Detail* gdp,
                                  const UT_Vector3& delta,
                                  const GEO_Rolloff* rolloff,
                                  const GA_ROHandleF& distances,
                                  const GA_PointGroup* ptgroup,
                                  GEO_Delta* geodelta = NULL,
                                  const GA_RWHandleF* falloff_output = NULL,
                                  bool update_affected_normals = false,
                                  bool update_all_normals = false,
                                  const char* attrib_pattern = NULL);

void GU_API GUsoftTranslateEdges(GEO_Detail* gdp,
                                 const UT_Vector3& delta,
                                 const GEO_Rolloff* rolloff,
                                 const GA_ROHandleF& distances,
                                 const GA_EdgeGroup* edge_group,
                                 GEO_Delta* geodelta = NULL,
                                 const GA_RWHandleF* falloff_output = NULL,
                                 bool update_affected_normals = false,
                                 bool update_all_normals = false,
                                 const char* attrib_pattern = NULL);

// closest points is an attribute containing point indices for the lead point to
// follow normals from. If it doesn't contain a valid point index, it will be
// ignored for that point.
void GU_API GUsoftNormalTranslatePoints(
                                GEO_Detail* gdp,
                                const float distance,
                                const GEO_Rolloff* rolloff,
                                const GA_ROHandleF& distances,
                                const GA_PointGroup* ptgroup,
                                GEO_Delta* geodelta = NULL,
                                const GA_RWHandleF* falloff_output = NULL,
                                const GA_ROHandleID* closest_points = NULL,
                                const bool update_affected_normals = false,
                                bool update_all_normals = false);

#endif

