/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OptionFile.h (C++, Utility Library)
 *
 * COMMENTS:	General class for dealing with option files
 *
 */

#ifndef _UT_OPTION_FILE_H_INCLUDED_
#define _UT_OPTION_FILE_H_INCLUDED_

#include "UT_API.h"
#include <iosfwd>
#include <SYS/SYS_Types.h>
#include "UT_String.h"
#include "UT_SymbolTable.h"
#include "UT_Color.h"

#include "UT_VectorTypes.h"

typedef UT_SymbolMap<UT_StringHolder> UT_OptionValueMap;  

// This abstract class defines the parser used. If you pass NULL for the parser
// option into UT_OptionFile, then it will use a default parser
class UT_API UT_OptionParser
{
public:
    UT_OptionParser()		{}
    virtual ~UT_OptionParser()	{}

    // load the options into the given hash table. the user must do a strdup
    // of the string value when adding to the symbol table
    virtual bool load(const char *filename, UT_IStream &is,
                      UT_OptionValueMap &options) = 0;
    virtual bool save(const char *filename, std::ostream &os,
		      const UT_OptionValueMap &options) const = 0;
};

// Default parser used
class UT_API UT_UIOptionParser : public UT_OptionParser
{
public:
    virtual bool load(const char *filename, UT_IStream &is,
                      UT_OptionValueMap &options);
    virtual bool save(const char *filename, std::ostream &os,
		      const UT_OptionValueMap &options) const;

protected:
    virtual bool parseLine( const char *line, UT_OptionValueMap &options );
	    void addError( const char *error_str ) const;

private:    
    int		myLineCount;	//Used to output the line a load failed on
    UT_String	myFilename;	
};

// Same idea as UT_UIOptionParser, except we output delimiters around what we
// save so we can nest our output in a .hip file
class UT_API UT_BlockParser : public UT_UIOptionParser
{
public:
    virtual bool load(const char *filename, UT_IStream &is,
                      UT_OptionValueMap &options);

    virtual bool save(const char *filename, std::ostream &os,
		      const UT_OptionValueMap &options) const;

protected:
    virtual bool parseLine( const char *line, UT_OptionValueMap &options );
};

class UT_API UT_OptionFile
{
public:
    /// Construct a UT_OptionFile objects. If no parser is specified,
    /// it defaults to UT_UIOptionParser.
    UT_OptionFile( UT_OptionParser *parser = NULL );
    UT_OptionFile( const UT_OptionFile &src );
    ~UT_OptionFile();

    void clear();

    // only returns false if the file encounters an error. true is returned on
    // success or if the file didn't exist.
    bool load( const char *filename );
    bool save( const char *filename ) const;
    bool load( const char *filename, UT_IStream &is);
    bool save( const char *filename, std::ostream &os ) const;
    bool save( const char *filename, FILE *file ) const;

    bool hasOption( const char *name ) const;
    void removeOption( const char *name );

    void getOption( const char *name, int &value, int defvalue ) const;
    void getOption( const char *name, exint &value, exint defvalue ) const;
    void getOption( const char *name, bool &value, bool defvalue ) const;
    void getOption( const char *name, fpreal32 &value, fpreal32 defvalue ) const;
    void getOption( const char *name, fpreal64 &value, fpreal64 defvalue ) const;
    void getOption( const char *name, UT_String &value,
				      const UT_String &defvalue ) const;
    void getOption( const char *name, UT_String &value,
				      const char *defvalue ) const;
    void getOption( const char *name, UT_StringHolder &value,
				      const UT_StringHolder &defvalue ) const;
    void getOption( const char *name, UT_StringHolder &value,
				      const char *defvalue ) const;
    void getOption( const char *name, UT_Vector2 &value,
				      const UT_Vector2 &defvalue ) const;
    void getOption( const char *name, UT_Vector3 &value,
				      const UT_Vector3 &defvalue ) const;
    void getOption( const char *name, UT_Vector4 &value,
				      const UT_Vector4 &defvalue ) const;
    void getOption( const char *name, UT_Quaternion &value,
				      const UT_Quaternion &defvalue ) const;
    void getOption( const char *name, UT_Matrix3 &value,
				      const UT_Matrix3 &defvalue ) const;
    void getOption( const char *name, UT_Matrix4 &value,
				      const UT_Matrix4 &defvalue ) const;
    void getOption( const char *name, UT_Color &value,
				      const UT_Color &defvalue ) const;

    void setOption( const char *name, int value );
    void setOption( const char *name, exint value );
    void setOption( const char *name, bool value );
    void setOption( const char *name, fpreal32 value );
    void setOption( const char *name, fpreal64 value );
    // Null strings are not allowed.
    void setOption( const char *name, const UT_String &value );
    void setOption( const char *name, const UT_StringHolder &value );
    void setOption( const char *name, const char *value );
    void setOption( const char *name, const UT_Vector2 &value );
    void setOption( const char *name, const UT_Vector3 &value );
    void setOption( const char *name, const UT_Vector4 &value );
    void setOption( const char *name, const UT_Quaternion &value );
    void setOption( const char *name, const UT_Matrix3 &value );
    void setOption( const char *name, const UT_Matrix4 &value );
    void setOption( const char *name, const UT_Color &value);

    const UT_OptionFile	&operator=(const UT_OptionFile &src);
    void		 merge(const UT_OptionFile &src);

    // Iterators
    class const_iterator
    {
	public:
	    const_iterator(const UT_OptionFile &option_file)
		: myOptionFile(option_file)
	    {
	    }
	    const_iterator(const UT_OptionFile &option_file,
			   UT_OptionValueMap::const_iterator iter)
		: myOptionFile(option_file), myIter(iter)
	    {
	    }

	    const char *name() const
	    {
		return myIter.name();
	    }

	    template <typename _T>
	    void getValue(_T &value, const _T &defvalue) const
	    {
		myOptionFile.getOption(myIter.name(), value, defvalue);
	    }

	    bool operator==(const const_iterator &cmp)
	    {
		 return (&myOptionFile == &cmp.myOptionFile
			 && myIter == cmp.myIter);
	    }
	    bool operator!=(const const_iterator &cmp)
	    {
		 return !(*this == cmp);
	    }

	    const_iterator& operator++()    // preincrement
	    {
		++myIter;
		return (*this);
	    }
	    const_iterator operator++(int)  // postincrement
	    {
		const_iterator tmp = *this;
		++*this;
		return (tmp);
	    }

	protected:
	    // right now, we don't have to define any copy/assignment operators
	    // because element-wise assignment works fine.
	    const UT_OptionFile &	myOptionFile;
	    UT_OptionValueMap::const_iterator myIter;
    };
    class UT_API iterator : public const_iterator
    {
	public:
	    iterator(UT_OptionFile &option_file)
		: const_iterator(option_file)
	    {
	    }
	    iterator(UT_OptionFile &option_file,
	             UT_OptionValueMap::const_iterator iter)
		: const_iterator(option_file, iter)
	    {
	    }

	    template <typename _T>
	    void setValue(_T &value)
	    {
		const_cast<UT_OptionFile *>(&myOptionFile)->setOption(
						myIter.name(), value);
	    }

	    bool operator==(const iterator &cmp) const
	    {
		 return (&myOptionFile == &cmp.myOptionFile
			 && myIter == cmp.myIter);
	    }
	    bool operator!=(const iterator &cmp) const
	    {
		 return !(*this == cmp);
	    }

	private:
    };


    const_iterator begin() const
    {
	return const_iterator(*this, myOptions.begin());
    }
    const_iterator end() const
    {
	return const_iterator(*this);
    }
    iterator begin()
    {
	return iterator(*this, myOptions.begin());
    }
    iterator end()
    {
	return iterator(*this);
    }


private:
    UT_OptionParser *	myParser;
    UT_OptionValueMap 	myOptions;
};

#endif // _UT_OPTION_FILE_H_INCLUDED_

