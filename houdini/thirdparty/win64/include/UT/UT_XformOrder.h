/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This class stores a transformation order of scales, rotates, and
 *	translates.
 */

#ifndef __UT_XformOrder_h__
#define __UT_XformOrder_h__

#include "UT_API.h"
#include <iosfwd>
#include "UT_Assert.h"

class UT_API UT_XformOrder 
{
public:
    // The xforms. Their values are used as indices in an array, so they
    // should start at 0 and increase by 1.
    enum xform { T=0, S=1, RX=2, RY=3, RZ=4};

    // Xform ordering for R,S,T and for the rotates respectively. If you
    // change any of these, make sure you update the ...FromString()
    // methods in the private section.
    enum rstOrder { TRS, TSR, RTS, RST, STR, SRT };
    enum xyzOrder { XYZ, XZY, YXZ, YZX, ZXY, ZYX };

    // Simple class constructors and destructor. The first constructor 
    // allows you to choose one of the RST permutations and one of the
    // rotation permutation, while the second c-tor offers full flexibility
    // in specifying the xform  order (you could even have more xforms of
    // the same type, but then the behaviour of mainOrder() and rotOrder()
    // will be undefined).
    UT_XformOrder(rstOrder rst = UT_XformOrder::TSR,
		  xyzOrder rot = UT_XformOrder::XYZ)
    {
	reorder(rst, rot);
    }

    UT_XformOrder(xform xf1, xform xf2,
		  xform xf3 = UT_XformOrder::RX, 
		  xform xf4 = UT_XformOrder::RY,
		  xform xf5 = UT_XformOrder::RZ)
    { 
	reorder(xf1, xf2, xf3, xf4, xf5);
    }

    // Constructor that takes strings specifying the xform order. Any 
    // combination of lower and uppercase is acceptable ("TsR", "XZY", "rst").
    // We do NOT check the string lengths, but we do check for null pointers.
    UT_XformOrder(const char *rst, const char *rot = "xyz");

    // Copy constructor (deep)
    UT_XformOrder(const UT_XformOrder &xf);

    // Clas destructor -- trivial.
   ~UT_XformOrder() {}

    // Shuffle the order of the xforms. If all you'll ever do is traverse the
    // xform order to build a transformation matrix, you can even have more
    // xforms of the same type in the argument list; then, however, the
    // behaviour of mainOrder() and rotOrder() will be undefined.
    void		reorder(UT_XformOrder::xform xf1,
			    UT_XformOrder::xform xf2, UT_XformOrder::xform xf3, 
			    UT_XformOrder::xform xf4, UT_XformOrder::xform xf5);
    void		reorder(UT_XformOrder::rstOrder rst,
			    UT_XformOrder::xyzOrder rot);

    // Query or set the main xform order or the rotation order respectively.
    // These methods assume the rotates are bundled together, and search for
    // the position of the first rotate they find. The methods that take
    // strings accept lower and uppercase combinations, check to see whether 
    // the argument is zero, but don't check its length.
    UT_XformOrder::rstOrder	mainOrder(void) const;
    void			mainOrder(UT_XformOrder::rstOrder rst);
    bool			mainOrder(const char *rst);
    UT_XformOrder::xyzOrder	rotOrder (void) const;
    void		 	rotOrder (UT_XformOrder::xyzOrder rot);
    bool		 	rotOrder (const char *rot);

    // Compute a mapping from the source to destination rotation order by
    // filling in the given 'map' array.
    static void			getRotOrderMapping(xyzOrder src, xyzOrder dst,
						   int map[3]);

    // Swap two xforms:
    void		swap(UT_XformOrder::xform xf1,UT_XformOrder::xform xf2)
			{
			    if (xf1 != xf2)
			    {
				unsigned short i1 = prefList[xf1];
				unsigned short i2 = prefList[xf2];
				prefList[xf1] = i2; permList[i1] = xf2;
				prefList[xf2] = i1; permList[i2] = xf1;
			    }
			}

    void		invert();
    
    // Two operator: operator() returns the index of a given transform without
    // having to check any bounds; operator[] returns the i'th transform and
    // checks if it's between valid bounds.
    unsigned short		operator()(UT_XformOrder::xform xf) const	
				{
			    	    return prefList[xf]; 
				}
    UT_XformOrder::xform	operator[](unsigned short i) const	
				{
				    UT_ASSERT_P(i < 5);
			    	    return (i<5) 
					? (UT_XformOrder::xform)permList[i] 
					: (UT_XformOrder::xform)permList[4];
				}

    // Return the number of transforms we store:
    int			 count(void) const	{ return 5; }

    // Get a hard copy of the elements of xf.
    UT_XformOrder	&operator=(const UT_XformOrder &xf);

    // Save or load the xform order. We save the sequence of transforms in the
    // order in which they appear in the permutation list.
    int			 save(std::ostream &os, int binary=0) const;

    friend std::ostream	&operator<<(std::ostream &os, const UT_XformOrder &t)
			 {
			     t.save(os);
			     return os;
			 }

    const char *mainOrderLabel() const;
    const char *rotOrderLabel() const;

private:
    // The permulation list. Should be of type UT_XformOrder::xform, but chars
    // are more compact.
    char		 permList[5];

    // Permutation reference: stores the index of each transform in the 
    // permutation list. It's handy for locating and shuffling xforms fast.
    unsigned short	 prefList[5];

    // Translate a string into an xform order, returning whether or not the
    // conversion was successful.  If the conversion wasn't successful,
    // the xform order will be set to the "default" order.
    static bool		 rstFromString(const char *rst,
				       UT_XformOrder::rstOrder &result);
    static bool		 xyzFromString(const char *rot,
				       UT_XformOrder::xyzOrder &result);

    static const char *	theXYZNames[6];
    static const char *	theXYZLabels[6];
    static const char *	theTRSLabels[6];
};

#endif
