/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Conversion Utility Library (C++)
 *
 * COMMENTS:
 *	Save a gdp to RIB.  This is not able to load or convert any files.
 *
 * 
 */

#ifndef __GEO_IOBpoly_h__
#define __GEO_IOBpoly_h__

#include "GEO_API.h"
#include "GEO_IOTranslator.h"

class GEO_Detail;

class GEO_API GEO_IOBpoly : public GEO_IOTranslator
{
public:
    GEO_IOBpoly()
	: GEO_IOTranslator()
	, myFileType(-1)
	, myMagic(0)
    {}
    GEO_IOBpoly(const GEO_IOBpoly &src)
	: GEO_IOTranslator()
	, myFileType(src.myFileType)
	, myMagic(src.myMagic)
    { }
    virtual ~GEO_IOBpoly();

    /// This method is used to create a copy of the sub-class
    virtual GEO_IOTranslator	*duplicate() const;

    virtual const char	*formatName() const;

    // Method to check the extension of the name to see if it's one of these...
    virtual int		 checkExtension(const char *name);
    virtual int		 checkMagicNumber(unsigned magic);

    // Methods to save Polys/etc.
    virtual GA_Detail::IOStatus	 fileSave(const GEO_Detail *, std::ostream &os);
    virtual GA_Detail::IOStatus	 fileLoad(GEO_Detail *, UT_IStream &, bool);

private:
    int			 myFileType;
    int			 myMagic;
};

#endif
