/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Filter.h ( UT Library, C++)
 *
 * COMMENTS:	Circular symmetric separable filters.  This class allows you to
 *		perform approximate filter weights given different filter
 *		kernels
 *
 *		The lookup tables are valid for 0-1 and should be called with
 *		the square of the distance...  The getWeight() methods can be
 *		used if given simple cartesian coordinates.
 */

#ifndef __UT_Filter__
#define __UT_Filter__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include "UT_FilterType.h"
#include "UT_ValArray.h"

class UT_Filter;

// Build a convolution window for a given filter
class UT_API UT_FilterWindow {
public:
     UT_FilterWindow()
	 : myWeights(myBuffer)
	 , myStart(0)
	 , mySize(0)
	 , myCapacity(BUFSIZE)
	 , myVisible(1)
     {
     }

    ~UT_FilterWindow()
     {
	 if (myWeights != myBuffer)
	     delete [] myWeights;
     }

    bool	setWeights(const UT_Filter &filter, float center,
			    float radius, int res,
			    UT_FilterWrap mode);

    // This should be called if houdini13 filtering is enabled
    template <UT_FilterWrap>
    bool	setWeights(const UT_Filter &filter, float center,
			    float radius, int res);

    // A more efficient version of setWeights().  However, there seems to be an
    // issue with computing the rounding when filtering.  $RTM/texture seems to
    // have a more "linear" result, but $RT/itools/stdin_stdout has an obvious
    // shift in the image.  So, this isn't quite ready for prime time.
    template <UT_FilterWrap>
    bool	setWeightsFast(const UT_Filter &filter, float center,
			    float radius, int res);


    void	 blendWithNextMip(int res, UT_FilterWrap mode, 
				  float mipinterp);

    // Trim out zero weights.  This is typically not done by default, but if
    // you're reusing these weights a lot, you might consider trimming.
    void	trimWeights()
    {
	if (myWeights[0] == 0 || myWeights[mySize-1] == 0)
	    pruneZeroWeights();
    }

    const float	*getWeights() const	{ return myWeights; }
    int		 getStart() const	{ return myStart; }
    int		 getEnd() const		{ return myStart + mySize - 1; }
    int		 getSize() const	{ return mySize; }
    float	 getVisible() const	{ return myVisible; }

    void	dump(const char *msg="") const;

private:
    void	pruneZeroWeights();

    // Ensure we have enough space to fill the weights.  Prefer to use the
    // local buffer, since this allows the filter to operate without memory
    // allocation.
    template <bool COPY>
    void	resizeWeights(int prevsize=0)
    {
	if (mySize > myCapacity)
	{
	    float *weights = myWeights;
	    myWeights = new float[mySize];
	    myCapacity = mySize;
	    if (COPY)
		memcpy(myWeights, weights, prevsize*sizeof(float));
	    if (weights != myBuffer)
		delete [] weights;
	}
    }

private:
    static const int	BUFSIZE = 16;

    float	 myBuffer[BUFSIZE];
    float	*myWeights;
    int		 myStart, mySize;
    int		 myCapacity;
    float	 myVisible;
};


class UT_API UT_Filter
{
public:
    // Method to lookup the filter by name.
    static UT_FilterType	 lookupFilter(const char *name);
    static const char		*getFilterName(UT_FilterType type);
    static const char		*getFilterLabel(UT_FilterType type);

    // Get a well known filter type
    static UT_Filter		*getFilter(UT_FilterType type);

    // Look up the filter wrap type given a string. If the type is unknown,
    // return def.
    static UT_FilterWrap	 lookupWrap(const char *keyword,
					    UT_FilterWrap def);

    // Get a custom filter kernel
    static UT_Filter		*getFilter(float (*evalfunc)(float, void *),
					   int size, int support = 2,
					   void *data = 0);

    static void			 releaseFilter(UT_Filter *filter);
    void			 access()	{ myRefCount++; }

    /// Fill an array of weights.  The @c center variable specifies the center
    /// of the filter (in resolution space - not 0-1).
    ///
    /// For example, if you have a filter with a radius of 1.3 centered at
    /// pixel 8.4, you should set:
    ///  - @c center = 8.4
    ///  - @c radius = 1.3
    ///  - @c size = floor(8.4+1.3) - floor(8.4-1.3) + 1 = 9-7+1 = 3
    virtual void	fillWeights(float umin, float radius,
				float *weights, int size) const;

    // Compute average weight over the span l-r.  The valid range for
    // samples is -0.5F to 0.5F (i.e. a filter width of 1)
    virtual float	getWeight(float l, float r) const = 0;

    // Retrieve the filter area to the left of l
    virtual float	getArea(float l) const = 0;

    // Return the filter type
    UT_FilterType	getType() const		{ return myType; }

    // Get the filters desired support radius (i.e. 2 for gaussian)
    float		getSupport() const	{ return mySupport; }

public:
    virtual ~UT_Filter();

protected:
     UT_Filter() : myRefCount(0), myType(UT_FILTER_BOX) {}

    int			 myRefCount;
    UT_FilterType	 myType;
    float		 mySupport;
};

#endif
