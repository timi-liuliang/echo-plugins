/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Histogram.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	
 */
#ifndef TIL_HISTOGRAM_H
#define TIL_HISTOGRAM_H

#include "TIL_API.h"
#include <UT/UT_String.h>
#include <UT/UT_Vector2.h>
#include "TIL_Defines.h"

class TIL_Raster;

class TIL_API TIL_Histogram
{
public:
    
			 TIL_Histogram();
    virtual		~TIL_Histogram();

    // setup.
    void		 setRaster(const TIL_Raster *generate_from,
				   int comp = -1);
    void		 setMaxBuckets(int max);

    void		 generate(TIL_HistogramType type,
				  bool usehue = true,
				  bool primary = true,
				  bool uvmode = false,
				  float uvpos = 0.0f);

    // query the generated histogram.
    int			 isHistogram() const;
    int			 isGraph() const;

    const char		*getName() const { return myGraphName; }
    void		 getXAxis(UT_String &label, int &limited_range,
				  float &start, float &end,
				  int &floatval, int &usesbwpoints,
				  int &usehue) const;
    void		 getYAxis(UT_String &label, int &limited_range,
				  float &start, float &end,
				  int &floatval, int &usesbwpoints,
				  int &usehue) const;

    void		 getBWPoints(float &black, float &white) const;
    
    unsigned int	 getHistogramSize(bool rebinned = true) const;
    const unsigned int	*getHistogram(bool rebinned = true) const;
    float		 getCutoffPoint() const;

    bool		 getUVPos(float &pos, int &dir) const;
    
    void		 getGraphDimensions(int &width, int &height) const;
    const TIL_Raster	*getGraph() const;
    unsigned int	 getGraphCount(int x, int y) const;

    // if hue is undefined, it will be set to -1 (normally it is [0, 360) ). 
    static void		 RGBtoHSV(float r, float g, float b,
				  float &h, float &s, float &v);
    static void		 HSVtoRGB(float h, float s, float v,
				  float &r, float &g, float &b);

    void		 rebin(const float histwidth);
    void	 	 getRangeForBar(int bar, float& start, float& end);
    
private:
    void		reset();
    void		scanRange(float &min, float &max);
    void		scanHistogram(float &min, float &max);
    static void		scanHistogram(	unsigned int const* histogram, int num_buckets,
					float &start, float &end, float& cutoff_point
				      );

    // both these methods return a map from size to max buckets if the size
    // is too big. Otherwise (the size is less than the max) returns null.
    // You must delete [] this map! myHistogram or myGraph will point the
    // alloc'd mem. size = -1 allocs the max amount of mem for the hist/graph.
    unsigned int	*allocHistogram(int size = -1);
    unsigned int	*allocGraph(int width, int depth = -1);

    void		convertGraphToRaster(int use_huex, int use_huey,
					     int cindex);
    
    void		generateRawHist();
    void		generateHSVHist(int which);

    void		generateRaw(int uv, bool uvmode, float uvpos);
    void		generateHSV(int which, int uv,
				    bool uvmode, float uvpos);
    void		generateHueGraph(int which);
    
private:
    const TIL_Raster *	myRaster;
    int			myComponent;
    int			myGraphType;

    TIL_HistogramType    myType;
    const char		*myGraphName;
    const char		*myXLabel;
    const char		*myYLabel;
    float		myXStart, myXEnd;
    float		myYStart, myYEnd;
    int			myXFloat, myYFloat;
    int			myXHue, myYHue;
    int			myXLimited, myYLimited;
    int			myWhitePointAxis;
    bool		myUVMode;
    float		myUVPos;

    // for histograms.
    int			myMaxBuckets;
    int			myNumBuckets;
    unsigned int       *myHistogram;
    float		myCutoffPoint;

    // for dynamic rebinning of histograms
    unsigned int*	myRebinnedHistogram;
    unsigned int*	myBinRangeTable;
    int			myAllocatedBins;
    int			myNumBins;
    float		myRebinnedCutoffPoint;
    bool		myHasRebinnedHistogram;

    // for graphs.
    unsigned int	myGraphWidth;
    unsigned int	myGraphHeight;
    unsigned char	*myGraph;
    TIL_Raster		*myDisplayGraph;
};

#endif
