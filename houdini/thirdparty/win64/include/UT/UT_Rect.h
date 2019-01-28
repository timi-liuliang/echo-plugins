/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Rect.h (C++)
 *
 * COMMENTS:
 *
 *	This file provides a bunch of integer rectangle classes.
 *
 */

#ifndef __UT_Rect_h__
#define __UT_Rect_h__

#include "UT_API.h"
#include "UT_Swap.h"

#include <SYS/SYS_Types.h>

#include <iosfwd>

#include <stddef.h>


class UT_WorkBuffer;

/// @file


/// @brief Flip a particular coordinate value according to the given dimension
inline int UTinclusiveFlip( int v, int d )	{ return d - 1 - v; }


class UT_DimRectImpl;
class UT_InclusiveRectImpl;
class UT_ExclusiveRectImpl;

template <typename T> class UT_Rect;


/// @var typedef UT_Rect<UT_DimRectImpl> UT_DimRect
/// @brief Integer rectangle class which stores its data as
/// (x, y, width, height).
/// @see UT_Rect
typedef UT_Rect<UT_DimRectImpl>		UT_DimRect;

/// @var typedef UT_Rect<UT_InclusiveRectImpl> UT_InclusiveRect
/// @brief Integer rectangle class which stores its data as inclusive
/// lower-left to upper-right (x1, y1, x2, y2).
/// @see UT_Rect
typedef UT_Rect<UT_InclusiveRectImpl>	UT_InclusiveRect;

/// @var typedef UT_Rect<UT_ExclusiveRectImpl> UT_ExclusiveRect
/// @brief Integer rectangle class which stores its data as exclusive
/// lower-left to upper-right (x1, y1, x2e, y2e). The upper-right corner
/// position (x2e, y2e) is equivalent to (x2+1, y2+1).
/// @see UT_Rect
typedef UT_Rect<UT_ExclusiveRectImpl>	UT_ExclusiveRect;


/// @brief Base Integer Rectangle class
///
/// UT_Rect is an integer rectangle class. You do *NOT* instantiate this but
/// instead choose from one of the 3 different types of underlying rectangle
/// implementations, each with the same interface:
///
/// @code
/// UT_DimRect:       Stores the data as (x, y, width, height)
/// UT_InclusiveRect: Stores the data as inclusive (x1, y1, x2, y2).
///                   ie. lower-left and upper-right corner positions
/// UT_ExclusiveRect: Stores the data as exclusive (x1, y1, x2e, y2e).
///                   The upper-right corner position (x2e, y2e) is
///                   equivalent to (x2+1, y2+1).
/// @endcode
///
/// The "lower-left"/"upper-right" terms here assume (0,0) is in the lower-left
/// corner of your coordinate system. If your (0,0) is in the top-left corner
/// instead, substitute these terms with "top-left"/"bottom-right".
///
/// To convert internal data representations from one type to another, use
/// the copy constructor and assignment operators. Otherwise, use the methods
/// found in UT_Rect.
///
template <typename T>
class UT_Rect
{
public:
    UT_Rect()					    { clear(); }

    /// @brief Construct with native rect type values
    ///
    /// The actual construction signatures are:
    /// @code
    ///   UT_DimRect(int x, int y, int width, int height)
    ///   UT_InclusiveRect(int x1, int y1, int x2, int y2)
    ///   UT_ExclusiveRect(int x1, int y1, int x2e, int y2e)
    /// @endcode
    /// @{
    UT_Rect(int v1, int v2, int v3, int v4)	    { set(v1, v2, v3, v4); }
    UT_Rect(int v[4])				    { set(v); }
    /// @}

    /// @brief Conversion constructor
    /// @{
    UT_Rect(const UT_DimRect &r) : _r(r)	    { }
    UT_Rect(const UT_InclusiveRect &r) : _r(r)	    { }
    UT_Rect(const UT_ExclusiveRect &r) : _r(r)	    { }
    /// @}

    /// @brief Assignment operator. Can convert from any rect type.
    /// @{
    UT_Rect &operator=(const UT_DimRect &r)	    { _r = r; return *this; }
    UT_Rect &operator=(const UT_InclusiveRect &r)   { _r = r; return *this; }
    UT_Rect &operator=(const UT_ExclusiveRect &r)   { _r = r; return *this; }
    /// @}

    /// Convenience method for creating an empty rect
    static inline UT_Rect zero();

    /// Clear rect to empty
    void clear()		    { _r.clear(); }

    /// @brief Set the native rect type values
    ///
    /// Interpretation will differ depending on type T.
    /// The actual signatures are:
    /// @code
    ///   UT_DimRect::set(int x, int y, int width, int height)
    ///   UT_InclusiveRect::set(int x1, int y1, int x2, int y2)
    ///   UT_ExclusiveRect::set(int x1, int y1e, int x2, int y2e)
    /// @endcode
    /// @{
    void set(int v1, int v2, int v3, int v4)
				    { _r.set(v1, v2, v3, v4); }
    void set(int v[4])
				    { _r.set(v[0], v[1], v[2], v[3]); }
    /// @}

    /// Return true if rectangle is empty
    bool isEmpty() const	    { return (width() == 0 || height() == 0); }

    /// Return true if rectangle is valid (true if empty)
    bool isValid() const	    { return _r.isValid(); }

    /// Modify the rectangle so that it has non-negative width/height values
    /// (ie. isValid() returns true)
    void standardize()		    { _r.standardize(); }

    /// @brief Returns true if the rectangle contains the given coord
    /// @{
    bool isInsideX(int px) const    { return (x1() <= px && px <= x2()); }
    bool isInsideY(int py) const    { return (y1() <= py && py <= y2()); }
    /// @}
    
    /// Returns true if the given point is within the rectangle
    bool isInside(int px, int py) const
				    { return isInsideX(px) && isInsideY(py); }

    /// Returns true if T is entirely contained by this rectangle
    bool contains(const T &r) const;

    /// Get relative coordinates from absolute
    void getRelative(int &px, int &py) const
				    { px -= x1(); py -= y1(); }

    /// Get absolute coordinates from relative
    void getAbsolute(int &px, int &py) const
				    { px += x1(); py += y1(); }

    /// @brief Flip rectangle within the given width/height
    /// @{
    void flipX(int awidth)	    { _r.flipX( awidth ); }
    void flipY(int aheight)	    { _r.flipY( aheight ); }
    /// @}

    /// Intersect this rectangle with the given one. Returns if result is valid.
    bool intersect(const T &r);

    /// @brief Returns true if the given side would overlap the other rectangle
    /// if projected onto it.
    /// @{
    bool overlapX(const T &r) const;
    bool overlapY(const T &r) const;
    /// @}

    /// Enlarge this rectangle to include the given rectangle (ie. a union)
    void enlarge(const T &r);

    /// @brief Enlarge this rectangle to include the given point
    /// @{
    void enlarge(int x, int y);
    void enlarge(float x, float y);
    /// @}

    /// Translate by given offset
    void translate(int x, int y);

    /// Inset the rect by a dx and dy margins
    void inset(int dx, int dy);

    /// Replace by the inclusive rect (x1()+dx1, y1()+dy1, x2()-dx2, y2()-dy2)
    void inset(int dx1, int dy1, int dx2, int dy2);

    /// Scale by given factor
    void scale(fpreal factor);

    /// Clamp the coordinates to be inclusive of this rect (ie. make it inside)
    void clamp(int &px, int &py) const
    {
	if (px < x1())
	    px = x1();
	else if (px > x2())
	    px = x2();

	if (py < y1())
	    py = y1();
	else if (py > y2())
	    py = y2();
    }

    /// @brief Get lower-left corner.
    /// @{
    int x() const		{ return _r.x(); }
    int y() const		{ return _r.y(); }
    int x1() const		{ return _r.x1(); }
    int y1() const		{ return _r.y1(); }
    /// @}

    /// @brief Set the lower-left corner position, always maintaining the size.
    /// @{
    void setX(int x_)		{ _r.setX(x_); }
    void setY(int y_)		{ _r.setY(y_); }
    /// @}

    /// @brief Set lower-left corner, without changing the upper-right corner
    /// @{
    void setX1(int x_)		{ _r.setX1(x_); }
    void setY1(int y_)		{ _r.setY1(y_); }
    /// @}

    /// @brief Get width/height dimensions
    /// @{
    int w() const		{ return _r.width(); }
    int h() const		{ return _r.height(); }
    int width() const		{ return _r.width(); }
    int height() const		{ return _r.height(); }
    /// @}

    /// @brief Set the width/height, always maintaining the lower-left position
    /// @{
    void setWidth(int awidth)	{ _r.setWidth(awidth); }
    void setHeight(int aheight)	{ _r.setHeight(aheight); }
    /// @}

    /// @brief Get upper-right corner (inclusive)
    /// @{
    int x2() const		{ return _r.x2(); }
    int y2() const		{ return _r.y2(); }
    /// @}
    /// @brief Set upper-right corner (inclusive), without changing lower-left.
    /// @{
    void setX2(int x_)		{ _r.setX2(x_); }
    void setY2(int y_)		{ _r.setY2(y_); }
    /// @}
    /// @brief Get upper-right corner (exclusive)
    /// @{
    int x2e() const		{ return _r.x2e(); }
    int y2e() const		{ return _r.y2e(); }
    /// @}
    /// @brief Set upper-right corner (exclusive), without changing lower-left.
    /// @{
    void setX2e(int x_)		{ _r.setX2e(x_); }
    void setY2e(int y_)		{ _r.setY2e(y_); }
    /// @}

    /// @brief Get center
    /// @{
    int centerX() const		{ return (_r.x1() + _r.x2()) / 2; }
    int centerY() const		{ return (_r.y1() + _r.y2()) / 2; }
    /// @}

    // Scalar <-> Rectangle conversions
    
    /// @brief Get rectangle values.
    ///
    /// This will return you the desired semantic
    /// regardless of the underlying rectangle type.
    /// @{
    void getDim(int &x_, int &y_, int &w_, int &h_) const
		{ x_ = x(); y_ = y(); w_ = width(); h_ = height(); }
    void getIncl(int &x1_, int &y1_, int &x2_, int &y2_) const
		{ x1_ = x(); y1_ = y(); x2_ = x2(); y2_ = y2(); }
    void getExcl(int &x1_, int &y1_, int &x2_, int &y2_) const
		{ x1_ = x(); y1_ = y(); x2_ = x2e(); y2_ = y2e(); }
    /// @}

    /// @brief Set rectangle values.
    ///
    /// Regardless of the underlying type of rectangle, it will set it up
    /// properly using the given semantic.
    /// @{
    void setDim(int x_, int y_, int w_, int h_)
		{ setX(x_); setY(y_); setWidth(w_); setHeight(h_); }
    void setDim(int v[4])
		{ setDim(v[0], v[1], v[2], v[3]); }
    void setIncl(int x1_, int y1_, int x2_, int y2_)
		{ setX1(x1_); setX2(x2_); setY1(y1_); setY2(y2_); }
    void setIncl(int v[4])
		{ setIncl(v[0], v[1], v[2], v[3]); }
    void setExcl(int x1_, int y1_, int x2_, int y2_)
		{ setX1(x1_); setX2e(x2_); setY1(y1_); setY2e(y2_); }
    void setExcl(int v[4])
		{ setExcl(v[0], v[1], v[2], v[3]); }
    /// @}

    /// @brief Native data access. @see UT_Rect::UT_Rect()
    /// @{
    int &operator()(int i)	{ return _r(i); }
    int operator()(int i) const	{ return _r(i); }
    const int *data() const	{ return _r.data(); }
    int *data()			{ return _r.data(); }
    /// @}

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += _r.getMemoryUsage(false);
        return mem;
    }

    void dump(UT_WorkBuffer &wbuf, const char *msg="") const;
    void dump(const char *msg="") const;

private:
    T	_r;
};

//////////////////////////////////////////////////////////////////////////////
// 
// Implementation
//
/// @cond

class UT_BaseRectImpl
{
public:
    UT_BaseRectImpl() { }
    UT_BaseRectImpl( int v0, int v1, int v2, int v3 )
	{ set( v0, v1, v2, v3 ); }
    UT_BaseRectImpl( const UT_BaseRectImpl &r )
	{ set( r.myVals[0], r.myVals[1], r.myVals[2], r.myVals[3] ); }

    UT_BaseRectImpl &operator=( const UT_BaseRectImpl &r )
    {
	if( &r != this )
	    set( r.myVals[0], r.myVals[1], r.myVals[2], r.myVals[3] );
	return *this;
    }

    // The actual signatures are:
    //   UT_DimRect::set(int x, int y, int width, int height)
    //   UT_InclusiveRect::set(int x1, int y1, int x2, int y2)
    //   UT_ExclusiveRect::set(int x1, int y1e, int x2, int y2e)
    void set( int v0, int v1, int v2, int v3 )
	{ myVals[0] = v0; myVals[1] = v1; myVals[2] = v2; myVals[3] = v3; }

    int &operator()(int i)		{ return myVals[i]; }
    int operator()(int i) const		{ return myVals[i]; }

    const int *	data() const		{ return myVals; }
    int *	data()			{ return myVals; }

    int64 getMemoryUsage(bool inclusive) const
    { return inclusive ? sizeof(*this) : 0; }

protected:
    // D == Dimension Rect, I == Inclusive Rect, E == Exclusive Rect
    int I2D( int v1, int v2 ) const	{ return v2 - v1 + 1; }
    int D2I( int v, int d ) const	{ return v + d - 1; }
    int E2D( int v1, int v2 ) const	{ return v2 - v1; }
    int D2E( int v, int d ) const	{ return v + d; }
    int E2I( int v2 ) const		{ return v2 - 1; }
    int I2E( int v2 ) const		{ return v2 + 1; }

    // Flip a particular inclusive coord value according to the given dimension
    static int	flipI( int v, int d )	{ return UTinclusiveFlip( v, d ); }

protected:
    int	myVals[4];
};

class UT_DimRectImpl : public UT_BaseRectImpl
{
public:
    UT_DimRectImpl()	    { }
    UT_DimRectImpl( const UT_DimRect &r );
    UT_DimRectImpl( const UT_InclusiveRect &r );
    UT_DimRectImpl( const UT_ExclusiveRect &r );

    UT_DimRectImpl &operator=( const UT_DimRect &r );
    UT_DimRectImpl &operator=( const UT_InclusiveRect &r );
    UT_DimRectImpl &operator=( const UT_ExclusiveRect &r );

    // clear it to empty
    void clear()	    { set( 0, 0, 0, 0 ); }

    // Return true if rectangle is valid (true if empty)
    bool isValid() const    { return width() >= 0 && height() >= 0; }

    // Modify the rectangle so that it has non-negative width/height values
    void standardize()
    {
	if (width() < 0)
	{
	    setX(x2());
	    setWidth(-width() + 2); // nw = nx2-nx1+1 = x1-(x1+w-1)+1 = -w + 2
	}
	if (height() < 0)
	{
	    setY(y2());
	    setHeight(-height() + 2);
	}
    }

    // Flip rectangle within the given width/height
    void flipX(int w)	    { myVals[0] = flipI(x2(), w); }
    void flipY(int h)	    { myVals[1] = flipI(y2(), h); }

    // Accessors
    int x() const	    { return myVals[0]; }
    int y() const	    { return myVals[1]; }
    int width() const	    { return myVals[2]; }
    int height() const	    { return myVals[3]; }

    int x1() const	    { return x(); }
    int y1() const	    { return y(); }
    int x2() const	    { return D2I(x(), width()); }
    int y2() const	    { return D2I(y(), height()); }
    int x2e() const	    { return I2E(x2()); }
    int y2e() const	    { return I2E(y2()); }

    void setWidth(int w)	{ myVals[2] = w; }
    void setHeight(int h)	{ myVals[3] = h; }

    void setX(int x_)		{ myVals[0] = x_; }
    void setY(int y_)		{ myVals[1] = y_; }
    void setX1(int x_)		{ myVals[2] += myVals[0]-x_; myVals[0] = x_; }
    void setY1(int y_)		{ myVals[3] += myVals[1]-y_; myVals[1] = y_; }
    void setX2(int x_)		{ myVals[2] = I2D(myVals[0], x_); }
    void setY2(int y_)		{ myVals[3] = I2D(myVals[1], y_); }
    void setX2e(int x_)		{ myVals[2] = E2D(myVals[0], x_); }
    void setY2e(int y_)		{ myVals[3] = E2D(myVals[1], y_); }
};


class UT_InclusiveRectImpl : public UT_BaseRectImpl
{
public:
    UT_InclusiveRectImpl()  { }
    UT_InclusiveRectImpl( const UT_DimRect &r );
    UT_InclusiveRectImpl( const UT_InclusiveRect &r );
    UT_InclusiveRectImpl( const UT_ExclusiveRect &r );

    UT_InclusiveRectImpl &operator=( const UT_DimRect &r );
    UT_InclusiveRectImpl &operator=( const UT_InclusiveRect &r );
    UT_InclusiveRectImpl &operator=( const UT_ExclusiveRect &r );

    // clear it to empty
    void clear()	    { set( 0, 0, -1, -1 ); }

    // Return true if rectangle is valid (true if empty)
    bool isValid() const    { return (x1() <= x2e()) && (y1() <= y2e()); }

    // Modify the rectangle so that it has non-negative width/height values
    void standardize()
    {
	if (myVals[0] > myVals[2])
	    UTswap(myVals[2], myVals[0]);
	if (myVals[1] > myVals[3])
	    UTswap(myVals[1], myVals[3]);
    }

    // Flip rectangle within the given width/height
    void flipX( int w )
    {
	myVals[0] = flipI(x1(), w);
	myVals[2] = flipI(x2(), w);
	UTswap(myVals[0], myVals[2]);
    }
    void flipY( int h )
    {
	myVals[1] = flipI(y1(), h);
	myVals[3] = flipI(y2(), h);
	UTswap(myVals[1], myVals[3]);
    }

    // Accessors
    int x1() const	    { return myVals[0]; }
    int y1() const	    { return myVals[1]; }
    int x2() const	    { return myVals[2]; }
    int y2() const	    { return myVals[3]; }
    int x2e() const	    { return I2E(x2()); }
    int y2e() const	    { return I2E(y2()); }

    int x() const	    { return x1(); }
    int y() const	    { return y1(); }
    int width() const	    { return I2D(x1(), x2()); }
    int height() const	    { return I2D(y1(), y2()); }
    
    void setWidth(int w)	{ myVals[2] = D2I(myVals[0], w); }
    void setHeight(int h)	{ myVals[3] = D2I(myVals[1], h); }

    void setX(int x_)		{ myVals[2] = x_ + width()-1; myVals[0] = x_; }
    void setY(int y_)		{ myVals[3] = y_ +height()-1; myVals[1] = y_; }
    void setX1(int x_)		{ myVals[0] = x_; }
    void setY1(int y_)		{ myVals[1] = y_; }
    void setX2(int x_)		{ myVals[2] = x_; }
    void setY2(int y_)		{ myVals[3] = y_; }
    void setX2e(int x_)		{ myVals[2] = E2I(x_); }
    void setY2e(int y_)		{ myVals[3] = E2I(y_); }
};

class UT_ExclusiveRectImpl : public UT_BaseRectImpl
{
public:
    UT_ExclusiveRectImpl()  { }
    UT_ExclusiveRectImpl( const UT_DimRect &r );
    UT_ExclusiveRectImpl( const UT_InclusiveRect &r );
    UT_ExclusiveRectImpl( const UT_ExclusiveRect &r );

    UT_ExclusiveRectImpl &operator=( const UT_DimRect &r );
    UT_ExclusiveRectImpl &operator=( const UT_InclusiveRect &r );
    UT_ExclusiveRectImpl &operator=( const UT_ExclusiveRect &r );

    // clear it to empty
    void clear()	    { set( 0, 0, 0, 0 ); }

    // Return true if rectangle is valid (true if empty)
    bool isValid() const    { return x1() <= x2e() && y1() <= y2e(); }

    // Modify the rectangle so that it has non-negative width/height values
    void standardize()
    {
	if (myVals[0] > myVals[2])
	{
	    UTswap(myVals[2], myVals[0]);
	    myVals[0] -= 1; // convert first coord from excl to incl
	    myVals[2] += 1; // convert last coord from incl to excl
	}
	if (myVals[1] > myVals[3])
	{
	    UTswap(myVals[1], myVals[3]);
	    myVals[1] -= 1;
	    myVals[3] += 1;
	}
    }

    // Flip rectangle within the given width/height
    void flipX( int w )
    {
	myVals[0] = I2E(flipI(x1(), w));    // convert to exclusive for swap
	myVals[2] = flipI(x2(), w);
	UTswap(myVals[0], myVals[2]);
    }
    void flipY( int h )
    {
	myVals[1] = I2E(flipI(y1(), h));    // convert to exclusive for swap
	myVals[3] = flipI(y2(), h);
	UTswap(myVals[1], myVals[3]);
    }

    // Accessors
    int x1() const	    { return myVals[0]; }
    int y1() const	    { return myVals[1]; }
    int x2e() const	    { return myVals[2]; }
    int y2e() const	    { return myVals[3]; }
    int x2() const	    { return E2I(x2e()); }
    int y2() const	    { return E2I(y2e()); }

    int x() const	    { return x1(); }
    int y() const	    { return y1(); }
    int width() const	    { return E2D(x1(), x2e()); }
    int height() const	    { return E2D(y1(), y2e()); }

    void setWidth(int w)	{ myVals[2] = D2E(myVals[0], w); }
    void setHeight(int h)	{ myVals[3] = D2E(myVals[1], h); }
    
    void setX(int x_)		{ myVals[2] = x_ +  width(); myVals[0] = x_; }
    void setY(int y_)		{ myVals[3] = y_ + height(); myVals[1] = y_; }
    void setX1(int x_)		{ myVals[0] = x_; }
    void setY1(int y_)		{ myVals[1] = y_; }
    void setX2(int x_)		{ myVals[2] = I2E(x_); }
    void setY2(int y_)		{ myVals[3] = I2E(y_); }
    void setX2e(int x_)		{ myVals[2] = x_; }
    void setY2e(int y_)		{ myVals[3] = y_; }
};

template<typename T>
UT_Rect<T> UT_Rect<T>::zero()
{ 
    return UT_Rect(UT_DimRect(0,0,0,0)); 
}

UT_API std::ostream & operator<<(std::ostream &os, const UT_DimRect &r);
UT_API std::ostream & operator<<(std::ostream &os, const UT_InclusiveRect &r);
UT_API std::ostream & operator<<(std::ostream &os, const UT_ExclusiveRect &r);

template <typename T>
UT_API size_t format(char *buffer, size_t bufsize, const UT_Rect<T> &r);

//UT_API size_t format(char *buffer, size_t bufsize, const UT_DimRect &r);
//UT_API size_t format(char *buffer, size_t bufsize, const UT_InclusiveRect &r);
//UT_API size_t format(char *buffer, size_t bufsize, const UT_ExclusiveRect &r);

//////////////////////////////////////////////////////////////////////////
// Inline Implementations
//////////////////////////////////////////////////////////////////////////

#define UT_RECT_OP_EQUALS(T1,T2) \
    inline bool operator==( const T1 &r1, const T2 &r2 ) \
	{ return (r1.x() == r2.x() && r1.y() == r2.y() \
		&& r1.width() == r2.width() \
		&& r1.height() == r2.height()); }

    UT_RECT_OP_EQUALS(UT_DimRect,UT_DimRect)
    UT_RECT_OP_EQUALS(UT_DimRect,UT_InclusiveRect)
    UT_RECT_OP_EQUALS(UT_DimRect,UT_ExclusiveRect)
    UT_RECT_OP_EQUALS(UT_InclusiveRect,UT_DimRect)
    UT_RECT_OP_EQUALS(UT_InclusiveRect,UT_InclusiveRect)
    UT_RECT_OP_EQUALS(UT_InclusiveRect,UT_ExclusiveRect)
    UT_RECT_OP_EQUALS(UT_ExclusiveRect,UT_DimRect)
    UT_RECT_OP_EQUALS(UT_ExclusiveRect,UT_InclusiveRect)
    UT_RECT_OP_EQUALS(UT_ExclusiveRect,UT_ExclusiveRect)
#undef UT_RECT_OP_EQUALS

#define UT_RECT_OP_NOT_EQUALS(T1,T2) \
    inline bool operator!=( const T1 &r1, const T2 &r2 ) \
	{ return !(r1 == r2); }

    UT_RECT_OP_NOT_EQUALS(UT_DimRect,UT_DimRect)
    UT_RECT_OP_NOT_EQUALS(UT_DimRect,UT_InclusiveRect)
    UT_RECT_OP_NOT_EQUALS(UT_DimRect,UT_ExclusiveRect)
    UT_RECT_OP_NOT_EQUALS(UT_InclusiveRect,UT_DimRect)
    UT_RECT_OP_NOT_EQUALS(UT_InclusiveRect,UT_InclusiveRect)
    UT_RECT_OP_NOT_EQUALS(UT_InclusiveRect,UT_ExclusiveRect)
    UT_RECT_OP_NOT_EQUALS(UT_ExclusiveRect,UT_DimRect)
    UT_RECT_OP_NOT_EQUALS(UT_ExclusiveRect,UT_InclusiveRect)
    UT_RECT_OP_NOT_EQUALS(UT_ExclusiveRect,UT_ExclusiveRect)
#undef UT_RECT_OP_NOT_EQUALS

#define UT_RECT_COPY_TO_DIM(T1,T2) \
    inline T1::T1( const T2 &r ) \
	: UT_BaseRectImpl( r.x(), r.y(), r.width(), r.height() ) { } \
    inline T1 & T1::operator=(const T2 &r) \
		{ set( r.x(), r.y(), r.width(), r.height() ); return *this; }
#define UT_RECT_COPY_TO_INCLUSIVE(T1,T2) \
    inline T1::T1( const T2 &r ) \
	: UT_BaseRectImpl( r.x1(), r.y1(), r.x2(), r.y2() ) { } \
    inline T1 & T1::operator=(const T2 &r) \
		{ set( r.x1(), r.y1(), r.x2(), r.y2() ); return *this; }
#define UT_RECT_COPY_TO_EXCLUSIVE(T1,T2) \
    inline T1::T1( const T2 &r ) \
	: UT_BaseRectImpl( r.x1(), r.y1(), r.x2e(), r.y2e() ) { } \
    inline T1 & T1::operator=(const T2 &r) \
		{ set( r.x1(), r.y1(), r.x2e(), r.y2e() ); return *this; }

    UT_RECT_COPY_TO_DIM(UT_DimRectImpl,UT_DimRect)
    UT_RECT_COPY_TO_DIM(UT_DimRectImpl,UT_InclusiveRect)
    UT_RECT_COPY_TO_DIM(UT_DimRectImpl,UT_ExclusiveRect)
    UT_RECT_COPY_TO_INCLUSIVE(UT_InclusiveRectImpl,UT_DimRect)
    UT_RECT_COPY_TO_INCLUSIVE(UT_InclusiveRectImpl,UT_InclusiveRect)
    UT_RECT_COPY_TO_INCLUSIVE(UT_InclusiveRectImpl,UT_ExclusiveRect)
    UT_RECT_COPY_TO_EXCLUSIVE(UT_ExclusiveRectImpl,UT_DimRect)
    UT_RECT_COPY_TO_EXCLUSIVE(UT_ExclusiveRectImpl,UT_InclusiveRect)
    UT_RECT_COPY_TO_EXCLUSIVE(UT_ExclusiveRectImpl,UT_ExclusiveRect)
#undef UT_RECT_COPY_TO_DIM
#undef UT_RECT_COPY_TO_INCLUSIVE
#undef UT_RECT_COPY_TO_EXCLUSIVE

/// @endcond

#endif // __UT_Rect_h__
