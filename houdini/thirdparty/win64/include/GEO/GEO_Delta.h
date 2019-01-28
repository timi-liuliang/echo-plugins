/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO library (C++)
 *
 * COMMENTS:	GEO_Delta is an abstract class for recording changes to
 * 		point positions, as well as point, primitive, or vertex
 * 		attributes.
 *
 * 		This class is a simple interface to the GDT library classes,
 * 		specifically GDT_Detail, which inherits from GEO_Delta.
 *
 * 		Existing geometry-modifying functions which are called
 * 		from the Edit SOP must be modified to take a GEO_Delta
 * 		pointer as a parameter, and must record all changes
 * 		to the geometry through this interface.
 * 		
 * 		Example of a point position change:
 * 			void geo_foo(... , GEO_Delta *geodelta, ...)
 * 			{
 * 				.
 * 				.
 * 				.
 * 				GA_Offset ptoff = gdp->pointOffset(i);
 * 				geodelta->beginPointPositionChange(*gdp, ptoff);
 * 				gdp->setPos4(ptoff, gdp->getPos4(ptoff).multiply3(xform));
 * 				geodelta->endChange();
 * 				.
 * 				.
 * 				.
 * 			}
 * 			
 * 		IMPORTANT IMPLEMENTATION NOTES:
 * 			The functions in this class are designed to
 * 			be as fast as possible, while maintaining some
 * 			degree of flexibility.  The following rules are
 * 			in place to ensure very fast operation:
 *
 * 		1. 	Every begin*Change() call must be accompanied by
 * 			a following endChange() call.
 * 			
 * 		2.	If a call to beginPointPositionChange() (for example)
 * 			is made, then ONLY the position of the given point
 * 			must change before the call to endChange.  Any other
 * 			changes will NOT be recorded.  This applies to any
 * 			begin*Change() call.
 *
 * 		3.	The attribute dictionary MUST be specifed with a
 * 			call to set*AttribDict.  Deltas will only be computed
 * 			for the attributes found in this dictionary.
 * 			A call to set*AttribDict should be found in the
 * 			Edit SOP, and in any place where the dictionary is
 * 			modified (which is why the set*AttribDict functions
 * 			are included in this interface, just in case.
 * 			Caution - this introduces a lot of additional cases
 * 			to consider).
 *
 * 		4.	It is important that the given attribute dictionary
 * 			stays constant for future begin/end calls, since the
 * 			computation of deltas depends on its structure.
 */

#ifndef __GEO_Delta_h__
#define __GEO_Delta_h__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include <GA/GA_Types.h>
class GEO_Detail;
class GEO_Primitive;
class GA_AttributeDict;

class GEO_API GEO_Delta
{
public:
		GEO_Delta() {}
    virtual    ~GEO_Delta() {}

    virtual int64   getMemoryUsage(bool inclusive=false) const = 0;

    /// Applies the delta to a point.
    /// For orthogonality, additional methods should be created as needed.
    virtual void	applyScaledPointDelta(GEO_Detail &gdp, GA_Offset pt,
					float scale) const = 0;
    virtual void	applyScaledVertexDelta(GEO_Detail &gdp,
					GA_Offset vtxoff,
					float scale) const = 0;

    // Begin changes to a point's position
    virtual void	beginPointPositionChange(
                                        const GA_Detail &gdp,
                                        GA_Offset pt) = 0;
    virtual void	beginPointPositionChange(
                                        const GA_Detail &gdp,
                                        const GA_Range &range) = 0;

    // Begin changes to a point's attributes
    virtual void	beginPointAttributeChange(
                                        const GEO_Detail &gdp,
                                        GA_Offset pt) = 0;
    virtual void	beginPointAttributeChange(
                                        const GEO_Detail &gdp,
                                        const GA_Range &range) = 0;

    // Begin changes to a point list's attributes
    // NOTES:
    // 	- This function will use MUCH more memory than
    // 	  beginPointAttributeChange.
    // 	- It should only be used when you KNOW that ALL the point
    // 	  attributes on the gdp will change.  beginPointAttributeChange is
    // 	  much faster when only some point attributes will change, since
    // 	  you can ignore the point attributes that don't matter.
    virtual void	beginPointListAttributeChange(
					const GEO_Detail &gdp) = 0;

    // Begin changes to a primitive's transform
    virtual void	beginPrimitiveTransformChange(
					const GEO_Primitive &prim) = 0;

    // Begin changes to a primitive's attributes
    virtual void	beginPrimitiveAttributeChange(
					const GEO_Primitive &prim) = 0;

    // Begin changes to a vertex attribute;
    // linear_index is the index which would be passed
    // to GA_Primitive::getVertexOffset(GA_Size idx) to retrieve
    // the desired vertex.
    virtual void	beginVertexAttributeChange(
                                        const GEO_Detail &gdp,
                                        GA_Offset vtx) = 0;
    virtual void	beginVertexAttributeChange(
                                        const GEO_Detail &gdp,
                                        const GA_Range &range) = 0;

    // interface for changing point capture weights
    virtual void	initCaptureWeightChange( const GEO_Detail &gdp ) = 0;
    virtual void	beginCaptureWeightChange(const GEO_Detail &gdp,
                                                 GA_Offset pt) = 0;

    // End any of the above changes
    virtual void	endChange() = 0;

    // Specify the attribute dictionary of the points to be modified
    // in between subsequent calls to beginPointAttributeChange
    // and endChange.
    virtual void	setPointAttribDict(
					const GA_AttributeDict &dict) = 0;

    // Specify the attribute dictionary of the primitives to be modified
    // in between subsequent calls to beginPointAttributeChange
    // and endChange.
    virtual void	setPrimAttribDict(
					const GA_AttributeDict &dict) = 0;
    
    // Specify the attribute dictionary of the vertices to be modified
    // in between subsequent calls to beginPointAttributeChange
    // and endChange.
    virtual void	setVertexAttribDict(
					const GA_AttributeDict &dict) = 0;

    // Refresh attributes added from GDT_Detail::set.*AttribDict() functions.
    // These functions assume that your geometry has not changed but your
    // attribute offsets might have changed because of re-cooking.
    // If a geometry function changes the attribute dictionary, then it should
    // call these.
    virtual void	refreshPointAttribDict(
					const GA_AttributeDict &dict) = 0;
    virtual void	refreshPrimAttribDict(
					const GA_AttributeDict &dict) = 0;
    virtual void	refreshVertexAttribDict(
					const GA_AttributeDict &dict) = 0;

};

#endif
