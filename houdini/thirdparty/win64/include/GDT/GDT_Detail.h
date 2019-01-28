/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library. (Geo Delta)
 *
 * COMMENTS:	Stores a GEO_Detail delta, for point/primitive/vertex
 *              attributes and point positions.
 *
 */

#ifndef __GDT_Detail_h__
#define __GDT_Detail_h__

#include "GDT_API.h"
#include "GDT_PointList.h"
#include "GDT_PrimitiveList.h"
#include "GDT_VertexList.h"
#include "GDT_CoordinateFrames.h"
#include <GEO/GEO_Delta.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_IStream;
class GEO_Detail;

SYS_DEPRECATED_PUSH_DISABLE()

class GDT_API GDT_Detail : public GEO_Delta
{
public:

		 GDT_Detail() : myChangeType(GDT_CHANGE_NONE) {}
		 GDT_Detail(const GDT_Detail &other);
    virtual	~GDT_Detail() {}

    virtual int64   getMemoryUsage(bool inclusive=false) const;

    /////////////////////////////
    // Methods for use by SOPs //
    /////////////////////////////
    
    // Apply stored deltas to the given gdp (optionally inverted)
    void		applyDeltas(GEO_Detail &gdp) const;
    void		applyInverseDeltas(GEO_Detail &gdp) const;

    // Operates on a single element.  For orthoganality, functions
    // should be made for GEO_Primitive, etc, as required.
    virtual void	applyScaledPointDelta(GEO_Detail &gdp, GA_Offset pt,
					 float scale) const;
    virtual void	applyScaledVertexDelta(GEO_Detail &gdp,
					GA_Offset vtxoff,
					float scale) const;
    // Currently, this only supports point positions
    void		applyScaledPointPosDeltas(GEO_Detail &gdp,
				 float scale,
				 const GA_PointGroup *ptgroup = NULL) const;

    // Apply given deltas to the stored deltas
    void		mergeDeltas(const GDT_Detail &other);
    void		mergeInverseDeltas(const GDT_Detail &other);

    // Accessor for myPointList.
    GDT_PointList	    *getPointList() { return &myPointList; }

    // IMPORTANT:
    // The following methods are the main vehicles for optimization
    // of the GDT classes.  It is critical to properly determine
    // which one should be called.  They all have similar purposes
    // with huge differences in the resulting performance.
    // The calls to set*AttribDict set up the internal lists of
    // deltas.  It may or may not be necessary to destroy these
    // lists.
    //
    // Set all deltas to zero, but don't delete any memory.
    // This is the best option for interactive use where the
    // number of modified elements is expected to stay constant.
    // This method retains the internal lists of attributes.
    void		zeroDeltas();
    //
    // Destroy all deltas (delete any memory allocated for them).
    // This is the best option for when the number of modified
    // elements is expected to change, but the gdp you are working
    // on has not changed.
    // This method retains the internal lists of attributes.
    void		clearDeltas();
    //
    // Destroy all internal structures completely (including the
    // internal lists of attributes).  This should be used when
    // the gdp you are working on has changed.
    void		destroy();

    // Returns whether we have any deltas at all
    bool		hasChange() const;

    ///////////////////////////////////
    // Virtual methods               //
    // inherited from GEO_Delta,     //
    // for use by geometry functions //
    ///////////////////////////////////

    virtual void        beginPointPositionChange(
					const GA_Detail &gdp,
                                        GA_Offset pt);
    virtual void	beginPointPositionChange(
                                        const GA_Detail &gdp,
                                        const GA_Range &range);

    virtual void        beginPointAttributeChange(
					const GEO_Detail &gdp,
                                        GA_Offset pt);
    virtual void	beginPointAttributeChange(
                                        const GEO_Detail &gdp,
                                        const GA_Range &range);

    virtual void	beginPointListAttributeChange(
					const GEO_Detail &gdp);

    virtual void        beginPrimitiveTransformChange(
					const GEO_Primitive &prim);

    virtual void        beginPrimitiveAttributeChange(
					const GEO_Primitive &prim);

    virtual void        beginVertexAttributeChange(
					const GEO_Detail &gdp,
                                        GA_Offset vtx);
    virtual void	beginVertexAttributeChange(
                                        const GEO_Detail &gdp,
                                        const GA_Range &range);

    // interface for changing point capture weights
    void		getOpPaths( UT_StringList &paths );
    void		getOpPathsCopy( UT_ValArray<char *> &paths );
    void		setOpPaths( const UT_ValArray<char *> &paths );
    int			changeOpPaths( const char *old_name,
				       const char *new_name );
    bool		changeCaptureRegionRoot( const char *old_root,
						 const char *new_root );
    void		refreshCaptureFrameInfo( const GEO_Detail &gdp,
						 GDT_CaptureRegionCB get_region,
						 void *user_data );
    virtual void	initCaptureWeightChange( const GEO_Detail &gdp );
    virtual void	beginCaptureWeightChange(const GEO_Detail &gdp, GA_Offset pt);

    virtual void        endChange();

    // Computes the vertex offset array from the given geometry,
    // so that vertex deltas can be computed correctly.
    virtual void	updateVertexOffsets(const GEO_Detail &gdp);
    
    // Sets point, primitive, or vertex attribute dictionaries,
    // so that attribute deltas can be computed correctly.
    // These methods will _add_ new attribute delta lists if necessary.
    virtual void	setPointAttribDict(const GA_AttributeDict &dict);
    virtual void	setPrimAttribDict(const GA_AttributeDict &dict);
    virtual void	setVertexAttribDict(const GA_AttributeDict &dict);

    // Refresh attributes added from set.*AttribDict() functions.
    // These functions assume that your geometry has not changed but your
    // attribute offsets might have changed because of added/deleted attributes.
    // Note that any attributes added on the geometry will _not_ be added
    // as new delta lists.
    virtual void	refreshPointAttribDict(const GA_AttributeDict &dict);
    virtual void	refreshPrimAttribDict(const GA_AttributeDict &dict);
    virtual void	refreshVertexAttribDict(const GA_AttributeDict &dict);

    /////////////////
    // I/O Methods //
    /////////////////
    bool		save(std::ostream &os, int binary=0) const;
    bool		load(UT_IStream &is);

    bool		save(UT_JSONWriter &w) const;
    bool		load(UT_JSONParser &p);

    bool		legacyLoad(UT_IStream &is);

    void		setCoordinateFrames(const GDT_CoordinateFrames *coords);
    const GDT_CoordinateFrames *getCoordinateFrames();

    void		createSymmetryDeltas(const GDT_Detail &input, GDT_SymmetryTransform *transform);

private:

    // Types of possible changes
    enum GDT_CHANGE_TYPE
    {
	GDT_CHANGE_NONE,
	GDT_CHANGE_FAKE,
	GDT_CHANGE_POINT_POS,
	GDT_CHANGE_POINT_CAPTUREWEIGHT,
	GDT_CHANGE_POINT_ATTRIB,
	GDT_CHANGE_POINTLIST_ATTRIB,
	GDT_CHANGE_PRIM_TRANS,
	GDT_CHANGE_PRIM_ATTRIB,
	GDT_CHANGE_VERTEX_ATTRIB
    };

    // Data cached for changing attributes or transforms
    GDT_CHANGE_TYPE	 myChangeType;
    const GEO_Detail    *myChangeDetail;
    const GEO_Primitive *myChangePrim;

    GDT_PointList	 myPointList;	// point deltas
    GDT_PrimitiveList	 myPrimList;	// primitive deltas
    GDT_VertexList	 myVertexList;	// vertex deltas
};

SYS_DEPRECATED_POP_DISABLE()

#endif
