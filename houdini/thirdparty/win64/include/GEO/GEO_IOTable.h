/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This is a class used for importing/exporting different geometry
 *	file formats.  There is a table of programs which can read
 *	or write different formats based on extension.
 *
 */

#ifndef __GEO_IOTable_h__
#define __GEO_IOTable_h__

#include "GEO_API.h"
#include "GEO_IOTranslator.h"
#include <GA/GA_Detail.h>

class GEO_Detail;

class GEO_API GEO_IOTable : public GEO_IOTranslator
{
public:
    GEO_IOTable()
	: GEO_IOTranslator()
	, myTranslator(NULL)
    {}
    GEO_IOTable(const GEO_IOTable &src)
	: GEO_IOTranslator()
	, myTranslator(src.myTranslator)
	, myMagic(src.myMagic)
    {}
    virtual ~GEO_IOTable();

    virtual GEO_IOTranslator	*duplicate() const;
    virtual const char		*formatName() const;

    // Method to check the extension of the name to see if it's one of these...
    virtual int		 checkExtension(const char *name);
    virtual int		 checkMagicNumber(unsigned magic);

    virtual GA_Detail::IOStatus	 fileLoad(GEO_Detail *, UT_IStream &, bool ate);
    virtual GA_Detail::IOStatus	 fileSave(const GEO_Detail *, std::ostream &);
    virtual GA_Detail::IOStatus	 fileSaveToFile(const GEO_Detail *gdp, 
                               	                const char *filename);
    virtual bool		 fileStat(const char *filename,
					GA_Stat &stat,
					uint level);

    /// Return the number of extensions which can be written to
    static int		getSaveFormatCount();
    /// Return the Nth save format
    static const char	*getSaveFormat(int i);

    /// Return the number of formats loaded from the GEOio table
    static int		 getFormatCount();
    static const char	*getFormatDescription(int i);
    static void		 getFormatRW(int i, bool &can_read, bool &can_write);
    static void		 getFormatExtensions(int i, UT_StringArray &extensions);

private:
    const void	*myTranslator;
    int		 myMagic;
};
#endif
