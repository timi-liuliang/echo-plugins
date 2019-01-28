/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Conversion Utility Library (C++)
 *
 * COMMENTS:
 *	This is the base class for all conversion types.
 *
 */

#ifndef __GEO_IOTranslator_h__
#define __GEO_IOTranslator_h__

#include "GEO_API.h"
#include <iosfwd>
#include <GA/GA_Detail.h>
#include <UT/UT_StringMap.h>

class GEO_Detail;
class UT_IStream;

class GEO_API GEO_IOTranslator
{
public:
    /// Define a map where the key is the IOTranslator format name and the
    /// value is the DSO file defining the translator.  Built-in translators
    /// don't appear in this list.
    typedef UT_StringMap<UT_StringHolder>	geo_DSODefinitions;

				GEO_IOTranslator();
    virtual			~GEO_IOTranslator();

    /// This method is used to create a copy of the sub-class
    virtual GEO_IOTranslator *	duplicate() const = 0;

    /// Returns the label for the geometry format that this
    /// translator supports.
    virtual const char *	formatName() const = 0;

    /// Method to check the extension of the name to see if it matches one that
    /// we can handle.  Returns true if there's a match
    virtual int		checkExtension(const char *name) = 0;

    /// Method to return a list of the file extensions matched by this
    /// translator.  The strings should contain the extensions (with dots).
    /// For exmple [".eps", ".ai"].  The default method returns nothing.
    virtual void	getFileExtensions(UT_StringArray &extensions) const;

    /// Method to check if the given magic number matches the magic number.
    /// Return true on a match.
    virtual int		checkMagicNumber(unsigned magic) = 0;

    /// Method to "stat" a file.  If possible, stat shouldn't have to load the
    /// entire geometry.
    virtual bool	fileStat(const char *filename,
				GA_Stat &stat,
				uint level);

    /// Method to load from a stream. If the ate_magic flag is on, then the
    ///	library has already read the magic number and the loader should
    ///	not expect it.
    /// If the file format doesn't support reading from streams, it
    /// can use UT_IStream::isRandomAccessFile to get the raw name
    /// to read from.
    /// Return false if this translator doesn't support loading.
    virtual GA_Detail::IOStatus	fileLoad(GEO_Detail *gdp,
					UT_IStream &is,
					bool ate_magic) = 0;

    /// Method to save a gdp to another format. Return false if this translator
    /// does not support saving.
    virtual GA_Detail::IOStatus	 fileSave(const GEO_Detail *gdp,
					std::ostream &os) = 0;

    /// Method to save a gdp to another format via a filename. Return false if
    /// this translator does not support saving.
    ///
    /// This is an optional method. By default, it will create a stream and
    /// call the streaming version of fileSave().
    virtual GA_Detail::IOStatus	 fileSaveToFile(const GEO_Detail *gdp,
					const char *filename);

    /// @private
    /// Due to the library hierarchy, it isn't possible to implement a default
    /// stat method (i.e. load the file and stat) at the GEO level (since
    /// GEO_Detail is pure virtual).  A higher library is responsible for
    /// setting the default stat method.
    class GEO_API DefaultStatFunction
    {
    public:
	DefaultStatFunction();
	virtual ~DefaultStatFunction();
	virtual bool	stat(GEO_IOTranslator &io,
				const char *filename,
				GA_Stat &stat,
				uint level) const = 0;
	virtual bool	statJSON(UT_IStream &is,
				GA_Stat &stat,
				uint level) const = 0;
    };
    /// @private
    /// The stat functor must stay as a persistent object (ownership is not
    /// maintained by this class).
    static void	setDefaultStat(DefaultStatFunction *func)
		    { theDefaultStat = func; }

    /// Return the list of DSO based translators
    static const geo_DSODefinitions	&dsoDefinitions();

    /// @{
    /// @private
    void	addDSODefinition();
    void	eraseDSODefinition();
    /// @}

protected:
    /// Stat a .bgeo JSON stream
    bool	statJSONStream(UT_IStream &is, GA_Stat &stat, uint level) const;

private:
    static DefaultStatFunction	*theDefaultStat;
};

#endif
