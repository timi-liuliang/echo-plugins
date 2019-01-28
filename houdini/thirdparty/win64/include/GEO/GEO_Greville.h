/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class implements a Greville abscissa, which is an average
 *      of "order" knots in parametric space.
 *
 */

#ifndef __GEO_Greville_h__
#define __GEO_Greville_h__

#include "GEO_API.h"
#include <UT/UT_Vector4.h>

class GEO_API GEO_Greville
{
public:
    // Trivial constructor that sets all the private data to 0. Needed when
    // instantiating arrays of Greville objects.
    GEO_Greville() : uVal((float)0.), vVal((float)0.)
    {
	image.assign(0.0F, 0.0F, 0.0F, 1.0F);
    }

    // Constructor that takes the (u,v) or (u) position of the Greville abscissa
    // in parametric space, and its image on the surface. Later, the image
    // info might represent the CV displacement. img is copied memberwise.
    GEO_Greville(const UT_Vector4 &img, float u = 0.0f, float v = 0.0f)
    { 
	uVal  = u; vVal = v;
	image = img;
    }

    // Class destructor. Does nothing.
    ~GEO_Greville() {}

    // Set either image: in domain space or in surface (CV) space 
    // respectively. img is copied memberwise.
    void setUVImage(float u)			{ uVal = u;           }
    void setUVImage(float u, float v)		{ uVal = u; vVal = v; }
    void setCVImage(const UT_Vector4& img)	{ image = img; }

    // Grab the private info.
    void		getUVImage(float *u) const { *u = uVal; }
    void		getUVImage(float *u, float *v) const
			{
			    *u = uVal; *v = vVal; 
			}
    const UT_Vector4&	getCVImage(void) const	{ return image; }

    // Deeply copy the data of the source.
    GEO_Greville&	operator=(const GEO_Greville &src)
			{
			    if (this != &src)
			    {
				uVal  = src.uVal; vVal = src.vVal;
				image = src.image;
			    }
			    return *this;
			}

    // Compare two Greville points:
    int			operator==(const GEO_Greville &grev) const
			{
			    return (uVal  == grev.uVal && vVal == grev.vVal &&
				    image == grev.image);
			}
    
protected:
private:
    // The (u,v) position in domain space, and the image on the surface.
    float	uVal;
    float	vVal;
    UT_Vector4	image;
};

#endif
