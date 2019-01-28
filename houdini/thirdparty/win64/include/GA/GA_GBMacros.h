/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GBMacros.h ( GA Library, C++)
 *
 * COMMENTS:	Macros which emulate the old GB macros
 */

#pragma once

#ifndef __GA_GBMacros__
#define __GA_GBMacros__

/// @file GA_GBMacros.h
/// @brief Create macros which emulate the macros in the obsolete GB library

/// TODO: The GA_ prefix is necessary while GA needs to coexist with GB.  Once
///       GB has been entirely replaced, we'll remove the GA_ prefix for source
///       compatibility.


#include "GA_AttributeDict.h"
#include "GA_BreakpointGroup.h"
#include "GA_Detail.h"
#include "GA_GroupTable.h"
#include "GA_ElementGroupTable.h"
#include "GA_Iterator.h"
#include "GA_GBIterators.h"
#include "GA_EdgeGroup.h"
#include "GA_Types.h"

/// @def GA_FOR_ALL_PRIMITIVES(gdp, prim)
/// Iterates over primitives, assigning prim to the current primitive
#define GA_FOR_ALL_PRIMITIVES(gdp, prim)	\
	for (GA_Iterator it((gdp)->getPrimitiveRange()); (!it.atEnd() || (prim = nullptr)) && \
		((prim) = GA_Detail::GB_MACRO_CAST((gdp), (gdp)->getPrimitive(*it))); ++it)

/// @def GA_FOR_SAFE_PRIMITIVES(gdp, prim, next)
/// Iterates over primitives, but primitives may be deleted during the
/// iteration.
#define GA_FOR_SAFE_PRIMITIVES(gdp, prim, next) \
	for (GA_GBPrimitiveIterator it(*(gdp), GA_Range::safedeletions()); \
		(prim=GA_Detail::GB_MACRO_CAST(gdp, it.getPrimitive())) && \
		((next=GA_Detail::GB_MACRO_CAST(gdp, it.getNextPrimitive())) \
		 || true); \
		++it)

/// @def GA_FOR_MASK_PRIMITIVES(gdp, prim, mask)
/// Iterates over primitives, filtering only primitives which match the type
/// mask specified.  mask can be either GA_PrimCompat::TypeMask or a family
/// mask.
/// @see GA_Primitive::getTypeId(), GA_Primitive::getFamilyMask(),
///   GA_Primitive::getPrimitiveId()
#define GA_FOR_MASK_PRIMITIVES(gdp, prim, mask) \
	for (GA_GBPrimitiveIterator it(*(gdp), mask); \
		(prim=GA_Detail::GB_MACRO_CAST(gdp, it.getPrimitive())); \
		++it)

/// @def GA_FOR_ALL_GROUP_PRIMITIVES(gdp, grp, prim)
/// Iterate over all primitives in group.  @see GA_RTIElementGroup
#define GA_FOR_ALL_GROUP_PRIMITIVES(gdp, grp, prim) \
	for (GA_Iterator it((gdp)->getPrimitiveRange(grp)); (!it.atEnd() || (prim = nullptr)) && \
		((prim)=GA_Detail::GB_MACRO_CAST((gdp), (gdp)->getPrimitive(*it))); \
		++it)

/// @def GA_FOR_SAFE_GROUP_PRIMITIVES(gdp, grp, prim, next)
/// Iterates over primitives in group, but primitives may be deleted during the
/// iteration. @see GA_RTIElementGroup
#define GA_FOR_SAFE_GROUP_PRIMITIVES(gdp, grp, prim, next) \
	for (GA_GBPrimitiveIterator it(*(gdp),grp,GA_Range::safedeletions()); \
		(prim=GA_Detail::GB_MACRO_CAST(gdp, it.getPrimitive())) && \
		((next=GA_Detail::GB_MACRO_CAST(gdp, it.getNextPrimitive())) \
		 || true); \
		++it)

/// @def GA_FOR_ALL_OPT_GROUP_PRIMITIVES(gdp, grp, prim)
/// Iterate over all primitives in group, but the group can be NULL.
/// @see GA_RTIElementGroup
#define GA_FOR_ALL_OPT_GROUP_PRIMITIVES(gdp,grp,prim)	\
		GA_FOR_ALL_GROUP_PRIMITIVES(gdp,grp,prim)


/// @def GA_FOR_ALL_PTOFF(gdp, ptoff)
/// Iterate over all points via offsets.
/// WARNING: Because this uses nested loops, "break;" won't work!
#define GA_FOR_ALL_PTOFF(gdp, ptoff)	\
    GA_Offset	lcl_start, lcl_end;	\
    for (GA_Iterator lcl_it((gdp)->getPointRange()); lcl_it.blockAdvance(lcl_start, lcl_end); )	\
	for (ptoff = lcl_start; ptoff < lcl_end; ++ptoff)

/// @def GA_FOR_ALL_GROUP_PTOFF(gdp, grp, ptoff)
/// Iterate over all points in a group via point offsets.
/// WARNING: Because this uses nested loops, "break;" won't work!
#define GA_FOR_ALL_GROUP_PTOFF(gdp, grp, ptoff)	\
    GA_Offset	lcl_start, lcl_end;	\
    for (GA_Iterator lcl_it((gdp)->getPointRange(grp)); lcl_it.blockAdvance(lcl_start, lcl_end); )	\
	for (ptoff = lcl_start; ptoff < lcl_end; ++ptoff)

/// @def GA_FOR_PAIRS_OF_PRIMITIVES(gdp1, prim1, gdp2, prim2)
/// Iterate over primitives in both GA_Details in a pairwise fashion.
#define GA_FOR_PAIRS_OF_PRIMITIVES(gdp1, prim1, gdp2, prim2)	\
	for (GA_GBPrimitiveIterator it(*(gdp1), NULL, *(gdp2), NULL); \
		(prim1=GA_Detail::GB_MACRO_CAST(gdp1, it.getPrimitive())) && \
		(prim2=GA_Detail::GB_MACRO_CAST(gdp2, it.getPrimitive2())); \
		++it)

/// @def GA_FOR_PAIRS_OF_GROUP_PRIMITIVES(gdp1, group1, prim1, gdp2, group2, prim2)
/// Iterate over points in the specified groups across both GA_Details in a
/// pairwise fashion.
#define GA_FOR_PAIRS_OF_GROUP_PRIMITIVES(gdp1, grp1, prim1, gdp2, grp2, prim2) \
	for (GA_GBPrimitiveIterator it(*(gdp1), grp1, *(gdp2), grp2); \
		(prim1=GA_Detail::GB_MACRO_CAST(gdp1, it.getPrimitive())) && \
		(prim2=GA_Detail::GB_MACRO_CAST(gdp2, it.getPrimitive2())); \
		++it)

/// @def GA_FOR_ALL_GROUP_EDGES(group, edge)
/// Iterate over edges in the specified group.
#define GA_FOR_ALL_GROUP_EDGES(group, edge) \
	for (auto it=(group)->begin(); \
			!it.atEnd() && (edge = &it.getEdge()); \
			++it)

/// @def GA_FOR_ALL_GROUP_EDGES_INCL_PRIMS(group, edge, primoff)
/// Iterate over edges in the specified group.
#define GA_FOR_ALL_GROUP_EDGES_INCL_PRIMS(group, edge, primoff) \
	for (auto it=(group)->begin(); \
			!it.atEnd() && (edge = &it.getEdge()) && \
			(GAisValid(primoff = it.getPrimitive()) || true); \
			++it)

/// @def GA_FOR_SAFE_GROUP_EDGES(group, edge)
/// Iterate over edges in the specified group.  Edges may be deleted during
/// traversal.
#define GA_FOR_SAFE_GROUP_EDGES(group, edge) \
	GA_FOR_ALL_GROUP_EDGES(group, edge)

/// @def GA_FOR_SAFE_GROUP_EDGES_INCL_PRIMS(group, edge)
/// Iterate over edges in the specified group.  Edges may be deleted during
/// traversal.
#define GA_FOR_SAFE_GROUP_EDGES_INCL_PRIMS(group, edge, prim) \
	GA_FOR_ALL_GROUP_EDGES_INCL_PRIMS(group, edge, prim)

/// @def GA_FOR_ALL_GROUP_BREAKPOINTS(group, bkp)
/// Iterate over breakpoints in the specified group.
#define GA_FOR_ALL_GROUP_BREAKPOINTS(group, bkp) \
	for (GA_BreakpointGroup::iterator it=(group)->begin(); \
			!it.atEnd() && (bkp = &it.getBreakpoint()); \
			it.advance())

/// @def GA_FOR_ALL_POINTGROUPS(gdp, group)
/// Iterate over all point GA_ElementGroups in an arbitrary order
/// @warning It is NOT safe to add or remove point groups during this loop.
#define GA_FOR_ALL_POINTGROUPS(gdp, grp) \
    for (GA_GroupTable::iterator<GA_PointGroup> \
	    __iter = (gdp)->pointGroups().beginTraverse(); \
	 !__iter.atEnd() && \
	    ((grp) = __iter.group()); \
	 ++__iter)

/// @def GA_FOR_ALL_POINTGROUPS_SORTED(gdp, group)
/// Iterate over all point GA_ElementGroups in alphabetical order
/// @warning It is NOT safe to add or remove point groups during this loop.
#define GA_FOR_ALL_POINTGROUPS_SORTED(gdp, grp) \
    for (GA_PointGroupTable::ordered_iterator \
	    __iter = (gdp)->pointGroups().obegin(); \
	 !__iter.atEnd() && \
	    ((grp) = *__iter); \
	 ++__iter)

/// @def GA_FOR_ALL_PRIMGROUPS(gdp, group)
/// Iterate over all primitive GA_ElementGroups in an arbitrary order
/// @warning It is NOT safe to add or remove point groups during this loop.
#define GA_FOR_ALL_PRIMGROUPS(gdp, grp) \
    for (GA_GroupTable::iterator<GA_PrimitiveGroup> \
	    __iter = (gdp)->primitiveGroups().beginTraverse(); \
	 !__iter.atEnd() && \
	    ((grp) = __iter.group()); \
	 ++__iter)

/// @def GA_FOR_ALL_PRIMGROUPS_SORTED(gdp, group)
/// Iterate over all primitive GA_ElementGroups in alphabetical order
/// @warning It is NOT safe to add or remove point groups during this loop.
#define GA_FOR_ALL_PRIMGROUPS_SORTED(gdp, grp) \
    for (GA_PrimitiveGroupTable::ordered_iterator \
	    __iter = (gdp)->primitiveGroups().obegin(); \
	 !__iter.atEnd() && \
	    ((grp) = *__iter); \
	 ++__iter)

/// @def GA_FOR_ALL_VERTEXGROUPS(gdp, group)
/// Iterate over all vertex GA_ElementGroups in an arbitrary order
/// @warning It is NOT safe to add or remove point groups during this loop.
#define GA_FOR_ALL_VERTEXGROUPS(gdp, grp) \
    for (GA_GroupTable::iterator<GA_VertexGroup> \
	    __iter = (gdp)->vertexGroups().beginTraverse(); \
	 !__iter.atEnd() && \
	    ((grp) = __iter.group()); \
	 ++__iter)

/// @def GA_FOR_ALL_VERTEXGROUPS_SORTED(gdp, group)
/// Iterate over all vertex GA_ElementGroups in alphabetical order
/// @warning It is NOT safe to add or remove point groups during this loop.
#define GA_FOR_ALL_VERTEXGROUPS_SORTED(gdp, grp) \
    for (GA_VertexGroupTable::ordered_iterator \
	    __iter = (gdp)->vertexGroups().obegin(); \
	 !__iter.atEnd() && \
	    ((grp) = *__iter); \
	 ++__iter)

/// @def GA_FOR_ALL_EDGEGROUPS(gdp, group)
/// Iterate over all GA_EdgeGroups in an arbitrary order
/// @warning It is NOT safe to add or remove edge groups during this loop.
#define GA_FOR_ALL_EDGEGROUPS(gdp, grp) \
    for (GA_GroupTable::iterator<GA_EdgeGroup> \
	    __iter = (gdp)->edgeGroups().beginTraverse(); \
	 !__iter.atEnd() && \
	    ((grp) = __iter.group()); \
	 ++__iter)

/// @def GA_FOR_ALL_EDGEGROUPS_SORTED(gdp, group)
/// Iterate over all GA_EdgeGroups in alphabetical order
/// @warning It is NOT safe to add or remove edge groups during this loop.
#define GA_FOR_ALL_EDGEGROUPS_SORTED(gdp, grp) \
    for (GA_EdgeGroupTable::ordered_iterator \
	    __iter = (gdp)->edgeGroups().obegin(); \
	 !__iter.atEnd() && \
	    ((grp) = *__iter); \
	 ++__iter)

/// @def GA_FOR_ALL_ATTRIBUTES(dict, attribute)
/// Iterate over all attributes in a GA_AttributeDict
#define GA_FOR_ALL_ATTRIBUTES(dict, A) \
    for (GA_AttributeDict::iterator it=dict.begin(); \
		!it.atEnd() && (A = it.attrib()); ++it)

/// @def GA_FOR_ALL_DETAIL_ATTRIBUTES(gdp, attribute)
/// Iterate over all global/detail attributes in the geometry
#define GA_FOR_ALL_DETAIL_ATTRIBUTES(gdp, A) \
	GA_FOR_ALL_ATTRIBUTES((gdp)->getAttributeDict(GA_ATTRIB_GLOBAL), A)

/// @def GA_FOR_ALL_PRIMITIVE_ATTRIBUTES(gdp, attribute)
/// Iterate over all primitive attributes in the geometry
#define GA_FOR_ALL_PRIMITIVE_ATTRIBUTES(gdp, A) \
	GA_FOR_ALL_ATTRIBUTES((gdp)->getAttributeDict(GA_ATTRIB_PRIMITIVE), A)

/// @def GA_FOR_ALL_POINT_ATTRIBUTES(gdp, attribute)
/// Iterate over all point attributes in the geometry
#define GA_FOR_ALL_POINT_ATTRIBUTES(gdp, A) \
	GA_FOR_ALL_ATTRIBUTES((gdp)->getAttributeDict(GA_ATTRIB_POINT), A)

/// @def GA_FOR_ALL_VERTEX_ATTRIBUTES(gdp, attribute)
/// Iterate over all vertex attributes in the geometry
#define GA_FOR_ALL_VERTEX_ATTRIBUTES(gdp, A) \
	GA_FOR_ALL_ATTRIBUTES((gdp)->getAttributeDict(GA_ATTRIB_VERTEX), A)

#define GA_FOR_ALL_GROUPS(grptable, grp) \
    for (GA_GroupTable::iterator<GA_ElementGroup> lcl_it = (grptable).beginTraverse(); !lcl_it.atEnd() && ((grp) = lcl_it.group()); ++lcl_it)

#define GA_FOR_ALL_GROUPS_SORTED(grptable, grp) \
    for (GA_ElementGroupTable::ordered_iterator lcl_it = (grptable).obegin(); !lcl_it.atEnd() && ((grp) = *lcl_it); ++lcl_it)

#endif
