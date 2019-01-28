/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_HilbertSequence.h (UT Library, C++)
 *
 * COMMENTS:	A class that generates a 2D Hilbert Curve within a given
 *		rectangle.
 */

#ifndef __UT_HILBERTSEQUENCE_H_INCLUDED__
#define __UT_HILBERTSEQUENCE_H_INCLUDED__

#include "UT_API.h"

class UT_API UT_HilbertCurve2D
{
public:
    UT_HilbertCurve2D(int level = 0);

    // Get the coordinates of a point on the Hilbert curve at the given
    // distance along the curve. Returns false if the distance exceeds the
    // curve length, as defined by its level.
    bool		 getPointFromDistance(int d, int &xp, int &yp) const;

    /// Retrieve the distance along the curve given coordinates of a point on
    /// the curve. If the coordinates are outside of the curve's bounding
    /// square, as defined by its level, the distance returned is -1.
    int			 getDistanceFromPoint(int xp, int yp) const;

    /// Advance the from the point at the given coordinates, to the next point
    /// along the curve and return those new coordinates. If the point is
    /// already at the end of the curve, this call returns false and the
    /// coordinates are left untouched.
    bool		 advancePointAlongCurve(int &xp, int &yp) const;

    int			 getLevel() const 	{ return myLevel; }
    int			 getSize() const 	{ return mySize; }
    int			 getLength() const 	{ return mySize * mySize; }
    
private:
    int			 myLevel;
    int			 mySize;
};

class UT_API UT_HilbertSequence2D
{
public:
    /// Initializes sequence data with the given parameters
    /// @param res_x @n Sequence resolution in x
    /// @param res_y @n Sequence resolution in y
    UT_HilbertSequence2D(int res_x = 0, int res_y = 0);

    class UT_API iterator
    {
    public:
	iterator();
	iterator(const UT_HilbertSequence2D &sequence);
	
	int 		 getDistance() const	{ return myDistance; }
	int 		 getIndex() const	{ return myIndex; }
	void 		 getCoord(int &x, int &y) const;
	
	void		 rewind();
	void		 advance();
	bool 		 atEnd() const;
	
	iterator	&operator++()		{ advance(); return *this; }
	// No post increment as it is harmful
    protected:
    private:
	int		 myResX, myResY;
	int		 myDistance;
	int		 myIndex;
	int		 myCoordX, myCoordY;
	UT_HilbertCurve2D myCurve;
    };

    iterator		 begin() const		{ return iterator(*this); }

    int			 getXRes() const	{ return myResX; }
    int			 getYRes() const	{ return myResY; }
    
private:
    int			 myResX;
    int			 myResY;
};

#endif // __UT_HILBERTSEQUENCE_H_INCLUDED__
