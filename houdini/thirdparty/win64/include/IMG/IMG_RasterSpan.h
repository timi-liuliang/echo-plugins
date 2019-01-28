#ifndef __IMG_RasterSpan__
#define __IMG_RasterSpan__

#include "IMG_API.h"
//
// This class a takes raster and creates a "span" data structure that holds
// all spans where alpha is 0.
//
// The access methods will take an arbitrary pixel and then return the
// closest pixel which has an alpha value of 0.
//
// The class could be expanded to create spans based upon different criteria.
//

#include	<stdio.h>

class IMG_Raster;
class IMG_Bucket;

class IMG_API IMG_RasterSpan
{
public:
			 IMG_RasterSpan(IMG_Raster *, unsigned long min = 0,
					      unsigned long max = 0);
			~IMG_RasterSpan();

    int			 getClosest(
				int scan_line,
				int pixel,
				int &x);
    int			 getClosest(
				int x,
				int y,
				float max,
				int &tx,
				int &ty,
				float &dist,
				float min = -1);

    void		 print(FILE *);

private:
    void		 buildSpan(IMG_Raster *);
    void		 cleanBuckets();
    void		 addBuckets(int num_buckets);

    IMG_Bucket		*myBuckets;
    int			 myBucketCount;

    float		*myDist;
    int			 myDistDim;
    unsigned long	 myMin;
    unsigned long	 myMax;
};

#endif // __IMG_Span__
