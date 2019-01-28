/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This handles trimming primitives, which are a linked list of
 * trimming curves.
 * 
 */

#ifndef __GD_TrimPiece_h_
#define __GD_TrimPiece_h_

#include "GD_API.h"
#include <UT/UT_BoundingRect.h>
#include <UT/UT_Array.h>
#include <UT/UT_Vector3Array.h>
#include <UT/UT_Matrix.h>
#include "GD_PrimType.h"

class GD_Face;
class GD_Detail;

class gdTrimIntersection;

// Holds the info for a hit:
class GD_API GD_TrimHitInfo 
{
public:
    int operator==(const GD_TrimHitInfo &x) const
    {
	return (t==x.t && u==x.u && d2==x.d2);
    }
    float	 t, u;
    float	 d2;		// How close hit is, squared.
    int		 boundary;	// GD_DomainBoundary value.
    gdTrimIntersection	*isect;
};


// Piece of a trimming curve
class GD_API GD_TrimPiece 
{
public:
    GD_TrimPiece();
    // Deletes self and all siblings.
    virtual ~GD_TrimPiece();

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myCV.getMemoryUsage(false);
        // We own our next, so count it (uses virtual calls)
        if (myNext)
            mem += myNext->getMemoryUsage(true);
        return mem;
    }

    void			 init(int order, float ustart, float ulength,
				      int rational = 1);

    // Copies this piece, does not copy siblings.
    virtual GD_TrimPiece	*copy() = 0;

    // Reverses this piece, does not reverse siblings. The U start remains
    // unchanged but can be accessed externally.
    virtual void		 reverse();

    // Remove a piece from this curve
    virtual GD_TrimPiece	*cut(float u1, float u2) const = 0;

    // Find length of curve: Rough estimate, uses the length of the
    // hull.
    float			 length() const;

    // Intersects with another piece, adding all intersections blindly
    // to the refarray (Caller should remove any duplicates)
    virtual int			 intersect(GD_TrimPiece &curve, 
				       UT_Array<GD_TrimHitInfo> &hitlist,
					float tol = 0.0001F) = 0;

    // Intersects with an isoparm.
    virtual int			 intersectIsoparm(float val, int isoparm,
					UT_Array<GD_TrimHitInfo> &hitlist,
					float tol = 1E-4F, int depth = 4) = 0;

    virtual GD_Face		*createFace(GD_Detail *gdp) = 0;

    // Find what type this piece is.
    virtual unsigned 		 getPrimitiveTypeId() const = 0; 

    virtual int			 evaluate(float u, UT_Vector2 &pos) const;
    virtual int			 evaluate(float u, UT_Vector2 &pos, 
						UT_Vector2 &der) const;
    // Req's u to be in [0,1] parameterization.
    virtual int			 evaluateUnit(float u, UT_Vector2 &pos) const=0;
    virtual int			 evaluateUnit(float u, UT_Vector2 &pos,
					   UT_Vector2 &der, UT_Vector2 &der2)
					    const = 0;

    // Check if the piece is straight, and if so reduce it to its two
    // end points. Returns one if straight.
    int				 checkStraight(void);

    virtual void		 buildBBox();

    void			 print() const;

// Data:
public:
    GD_TrimPiece		*myNext;
    float			 myUStart, myULength;
    UT_Vector3Array     	 myCV;
    UT_BoundingRect		 myBBox;
    int				 myRational;

    int				 myImplicitFlag;
    UT_MatrixF			 myXCoeff, myYCoeff, myWCoeff;
};

class GD_API GD_TrimPieceRBezCurve : public GD_TrimPiece 
{
public:
    GD_TrimPieceRBezCurve();
    // Deletes self and all siblings.
    virtual ~GD_TrimPieceRBezCurve();

    void		 buildCoeff(UT_MatrixF &A, int index1, int index2);

    // Copies this piece, does not copy siblings.
    virtual GD_TrimPiece	*copy();

    // Remove a piece from this curve
    virtual GD_TrimPiece	*cut(float u1, float u2) const;

    // Intersects with another piece, adding all intersections blindly
    // to the refarray (Caller should remove any duplicates)
    virtual int			 intersect(GD_TrimPiece &curve, 
				       UT_Array<GD_TrimHitInfo> &hitlist,
					float tol = 0.0001F);
    // Actual workhorse function:
    int			 	 intersect(GD_TrimPieceRBezCurve &curve, 
				       UT_Array<GD_TrimHitInfo> &hitlist,
					int depth,
					float tol = 0.0001F);
    // Another version, uses eigenvalue methods:
    int				 intersectQR(GD_TrimPieceRBezCurve &curve,
				       UT_Array<GD_TrimHitInfo> &hitlist,
					float tol = 0.0001F);

    // Intersects with an isoparm.
    virtual int			 intersectIsoparm(float val, int isoparm,
					UT_Array<GD_TrimHitInfo> &hitlist,
					float tol = 1E-4F, int depth = 4);

    // Req's u to be in [0,1] parameterization.
    virtual int			 evaluateUnit(float u, UT_Vector2 &pos) const;
    virtual int			 evaluateUnit(float u, UT_Vector2 &pos,
					   UT_Vector2 &der, UT_Vector2 &der2)
					    const;

    virtual GD_Face		*createFace(GD_Detail *gdp);

    // Find what type this piece is.
    virtual unsigned		 getPrimitiveTypeId() const
					     { return GD_PRIMBEZCURVE; }
    // Requires both left & right to exist
    void			 splitInHalf(GD_TrimPieceRBezCurve *left,
					   GD_TrimPieceRBezCurve *right, 
					   float tol = 1E-4F) const;
    // If left or right NULL, does not evaluate it.
    int				 splitAt(GD_TrimPieceRBezCurve *left,
					 GD_TrimPieceRBezCurve *right,
					 float ucut, float tol = 1E-4F) const;
    
};


class GD_API GD_TrimPiecePoly : public GD_TrimPiece 
{
public:
    GD_TrimPiecePoly();
    // Deletes self and all siblings.
    virtual ~GD_TrimPiecePoly();

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += GD_TrimPiece::getMemoryUsage(false);
        mem += myCurve.getMemoryUsage(false);
        return mem;
    }

    // Copies this piece, does not copy siblings.
    virtual GD_TrimPiece	*copy();

    // Remove a piece from this curve
    virtual GD_TrimPiece	*cut(float u1, float u2) const;

    // Intersects with another piece, adding all intersections blindly
    // to the refarray (Caller should remove any duplicates)
    virtual int			 intersect(GD_TrimPiece &curve, 
				       UT_Array<GD_TrimHitInfo> &hitlist,
					float tol = 0.0001F);

    // Intersects with an isoparm.
    virtual int			 intersectIsoparm(float val, int isoparm,
					UT_Array<GD_TrimHitInfo> &hitlist,
					float tol = 1E-4F, int depth = 4);

    // Req's u to be in [0,1] parameterization.
    virtual int			 evaluateUnit(float u, UT_Vector2 &pos) const;
    virtual int			 evaluateUnit(float u, UT_Vector2 &pos,
					   UT_Vector2 &der, UT_Vector2 &der2)
					    const;

    virtual GD_Face		*createFace(GD_Detail *gdp);

    // Find what type this piece is.
    virtual unsigned 		 getPrimitiveTypeId() const
					     { return GD_PRIMPOLY; }

    void			 buildCurve(int idx);
    void			 appendPoint(float x, float y);
public:
    GD_TrimPieceRBezCurve	 myCurve;
};

#endif
