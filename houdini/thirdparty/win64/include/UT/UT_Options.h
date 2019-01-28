/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Options.h (C++, Utility Library)
 *
 * COMMENTS:	This is a smaller, faster, more type-safe version of
 * 		UT_OptionFile. The reason UT_OptionFile was not simply
 * 		re-implemented is that when saving and loading UT_OptionFiles,
 * 		no information about the type of each option is saved. This
 * 		implementation writes out the type of each data item.
 *
 */

#ifndef _UT_OPTIONS_H_
#define _UT_OPTIONS_H_

#include "UT_API.h"
#include "UT_OptionEntry.h"
#include "UT_SmallObject.h"
#include "UT_StringArray.h"
#include "UT_StringHolder.h"
#include "UT_ArrayStringMap.h"
#include "UT_UniquePtr.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_TypeTraits.h>
#include <iosfwd>
#include <string>
#include <memory>


class UT_IStream;
class UT_JSONValue;
class UT_JSONValueMap;
class UT_JSONParser;
class UT_JSONWriter;
class UT_WorkBuffer;


/// A map of string to various well defined value types
class UT_API UT_Options
{
private:
    /// Storage type for the option map
    typedef UT_UniquePtr<UT_OptionEntry> OptionEntryPtr;
    typedef UT_ArrayStringMap<OptionEntryPtr> MapType;

public:
    // Default constructor (empty options)
    UT_Options();
    // Copy c-tor
    UT_Options(const UT_Options &src);
    virtual ~UT_Options();

    /// Variadic constructor that allows addition of options during
    /// construction.  Using this option requires very careful coding (as does
    /// all variadic coding).  For example, there is no type checking of
    /// variadic arguments.  If there are floating point arguments, they @b
    /// must be specified as double (i.e. Not "0", but rather "0.0").
    /// Type tokens expected are:
    ///	  - "int"		@n A single @b int value follows
    ///	  - "int64"		@n A single @b int64 value follows
    ///	  - "bool"		@n A single @b int follows
    ///	  - "float"		@n A single @b double follows
    ///	  - "string"		@n A single string value follows
    ///	  - "vector2"		@n Two (2) double arguments follow
    ///	  - "vector3"		@n Three (3) double arguments follow
    ///	  - "vector4"		@n Four (4) double arguments follow
    ///	  - "quaternion"	@n Four (4) double arguments follow
    ///   - "matrix2"		@n Four (4) double arguments follow
    ///   - "matrix3"		@n Nine (9) double arguments follow
    ///	  - "matrix4"		@n Sixteen (16) double arguments follow
    ///   - "uv"		@n Two (2) double arguments follow
    ///   - "uvw"		@n Three (3) double arguments follow
    ///   - "stringraw"		@n A single string value follows
    ///   - "int[N]"		@n
    ///		@c N @b int values follow (creating an integer array).
    ///		Replace @c N with the actual value (of course).
    ///		No spaces may occur in the type name.
    ///   - "int64[N]"		@n
    ///		@c N @b int64 values follow (creating an integer array).
    ///		Replace @c N with the actual value (of course).
    ///		No spaces may occur in the type name.
    ///   - "float[N]"		@n
    ///		@c N @b double values follow (creating a float array).
    ///		Replace @c N with the actual value (of course).
    ///		No spaces may occur in the type name.
    ///	   - "string[N]"	@n
    ///		@c N @b string values follow (creating a string array).
    ///		Replace @c N with the actual value (of course).
    ///		No spaces may occur in the type name.
    ///
    /// Special type tokens
    ///	  - "i_vector2", "i_vector3", "i_vector4", @n
    ///		A single @b double is used to set all components of the vector
    ///	  - "i_quaternion"		@n
    ///		A single @b double is used to set all components of the vector
    ///	  - "i_uv", "i_uvw"		@n
    ///		A single @b double is used to set all components of the vector
    ///	  - "i_matrix3", "i_matrix4"	@n
    ///		A single @b double is used to set the diagonal of the matrix3.
    ///	  - "i_int[N]"		@n
    ///		A single @b int64 is used to set all components of the array
    ///	  - "i_float[N]"	@n
    ///		A single @b double is used to set all components of the array
    ///
    /// The option name should follow the type specifier (separated by a single
    /// space).
    ///
    /// A nullptr keyword determines the end of the options.
    ///
    /// Examples: @code
    ///   UT_Options	options("int ival",	0,
    ///				"int64 i64val",	(int64)5,
    ///				"bool toggle",	(int)1,
    ///				"float fval",	(fpreal64)0,
    ///				"vector3 P",	0.0, 1.0, 2.0,
    ///				"string map",	"texture.pic",
    ///				"int[2] iarray", int64(0), int64(1),
    ///				nullptr); <======= MUST TERMINATE =======
    /// @code
    /// Common errors include: @code
    ///   UT_Options	options(
    ///		"int ival",	(int64)0,	// Error, must be int
    ///		"int64 ival",	(int)0,		// Error, must be int64
    ///		"vector3 fval",	0, 1.0, 2.0,	// First entry is an int!
    ///		"float[3] farray", 0.0, 1.0,	// Missing 3rd value
    ///		/* MISSING nullptr TERMINATOR */);
    /// @endcode
    UT_Options(const char *type_and_name, ...);

    // This function does not call optionChanged.
    void clear();

    // only returns false if the file encounters an error. true is returned on
    // success or if the file didn't exist.
    // The load methods can handle files saved either with this save
    // or with saveAsJSON.  saveAsJSON is preferable as it handles 64bit
    // floats and ints, but can't be read before H10.5
    bool load(const char *filename);
    bool save(const char *filename) const;
    bool saveOrdered(const char *filename) const;
    bool load(const char *filename, UT_IStream &is);
    bool save(const char *filename, std::ostream &os) const;
    bool saveOrdered(const char *filename, std::ostream &os) const;
    
    bool saveAsJSON(const char *filename,
		    bool binary = true) const;
    bool saveAsJSON(const char *filename, std::ostream &os,
                    bool binary = true) const;

    // Remove an option.
    void		 removeOption(const UT_StringHolder &name);
    
    // Get the string representation of any of our options.  Note that, if
    // the value contains binary data with null characters, you can't determine
    // the length of the data with UT_String version of these methods.
    bool		 getOptionString(const UT_StringHolder &name,
					 UT_OptionFormat format,
					 UT_WorkBuffer &result) const;
    bool		 getOptionString(const UT_StringHolder &name,
					 UT_OptionFormat format,
					 UT_String &result) const;
    
    // Append the python dictionary representation of ALL our options to the
    // buffer
    bool		 appendPyDictionary(UT_WorkBuffer &result,
				bool sorted=false) const;
				
    // Parse a python dictionary string and merge the results into our
    // options.
    bool		 setFromPyDictionary(const char *dict);

    // Parse a python formatted value and set it as the named option.
    bool		 setPyOptionString(const UT_StringHolder &name, 
        		                   const char *value);

    /// Save a UT_Options to JSON format using UT_JSONWriter
    bool		 save(UT_JSONWriter &w) const;
    /// Save a UT_Options to a UT_JSONValue
    void		 save(UT_JSONValueMap &map) const;

    /// Load a UT_Options from a JSON parser.
    /// @param parser		Parser
    /// @param do_clear		Clear existing map before loading
    ///	  If true and the first object in the JSON stream is not a map object,
    ///	  then the parser will fail (with an error).  Otherwise a warning will
    ///	  be added and the object will be skipped.
    /// @param is   If this is specified, then data will be read from that
    ///   stream rather than the stream associated with the parser.
    bool		 load(UT_JSONParser &parser,
				bool do_clear,
				UT_IStream *is = 0);

    /// Writes a JSON dump to ostream of the current options.
    void		dump(std::ostream &os) const;
    /// Dump to stdout/cout
    void		dump() const;

    // Functions for accessing the options by name.
    bool		 hasOption(const UT_StringRef &name) const;
    UT_OptionType	 getOptionType(const UT_StringRef &name) const;
    const UT_OptionEntry*getOptionEntry(const UT_StringRef &name) const;

    // Functions for accessing all options sequentially.
    int			 getNumOptions() const;
    class UT_API iterator
    {
    public:
	iterator()
	    : myOptions(nullptr)
	{}
	iterator(const iterator &i)
	    : myOptions(i.myOptions)
	    , myIterator(i.myIterator)
            , myEnd(i.myEnd)
	{}
	const UT_StringHolder &name() const
			    { return myIterator->first; }
	const UT_OptionEntry	 *entry() const
			    { return myIterator->second.get(); }
	UT_OptionType	 type() const
			    { return entry()->getType(); }
	const UT_OptionEntry	*operator*() const { return entry(); }

	iterator	&operator=(const iterator &i)
			 {
			     myOptions = i.myOptions;
			     myIterator = i.myIterator;
			     myEnd = i.myEnd;
			     return *this;
			 }
	bool		 operator==(const iterator &i) const
			 {
			     return myIterator == i.myIterator;
			 }
	bool		 operator!=(const iterator &i) const
			 { return !(*this == i); }
	iterator	&operator++()	{ advance(); return *this; }
	bool		 atEnd() const	{ return myIterator == myEnd; }
	void		 advance()	{ ++myIterator; }
    private:
	iterator(const UT_Options &opts, const MapType::const_iterator &it,
                 const MapType::const_iterator &end)
	    : myOptions(&opts)
	    , myIterator(it)
            , myEnd(end)
	{
	}
	const UT_Options	*myOptions;
	MapType::const_iterator	 myIterator;
	MapType::const_iterator	 myEnd;
	friend class		 UT_Options;
    };
    class UT_API ordered_iterator
    {
    public:
	ordered_iterator()
	    : myKeys()
	    , myValues()
	    , myOrder()
	    , myPos(0)
	    , myCurr(0)
	{
	}
	ordered_iterator(const ordered_iterator &src)
	    : myKeys(src.myKeys)
	    , myValues(src.myValues)
	    , myOrder(src.myOrder)
	    , myPos(src.myPos)
	    , myCurr(src.myCurr)
	{
	}
	const UT_StringHolder &name() const
			    { return myKeys(myCurr); }
	const UT_OptionEntry	 *entry() const
			    { return myValues(myCurr); }
	UT_OptionType	 type() const
			    { return entry()->getType(); }
	const UT_OptionEntry	*operator*() const { return entry(); }

	ordered_iterator &operator=(const ordered_iterator &i)
			 {
			     myKeys = i.myKeys;
			     myValues = i.myValues;
			     myOrder = i.myOrder;
			     myPos = i.myPos;
			     myCurr = i.myCurr;
			     return *this;
			 }
	bool		 operator==(const ordered_iterator &i) const
			 {
			     if (i.atEnd() && atEnd())
				 return true;
			     // Iterators are only equal if they are both at
			     // the end.
			     return false;
			 }
	bool		 operator!=(const ordered_iterator &i) const
			 { return !(*this == i); }
	ordered_iterator &operator++()	{ advance(); return *this; }
	bool		 atEnd() const	{ return myPos >= myOrder.entries(); }
	void		 advance()
			 {
			     myPos++;
			     if (myPos < myOrder.entries())
				 myCurr = myOrder(myPos);
			 }
    private:
	class comparator
	{
	public:
	    comparator(const UT_ValArray<UT_StringHolder> &keys)
		: myKeys(keys)
	    {
	    }
	    bool	operator()(int a, int b) const
			{
			    UT_ASSERT(a >= 0 && a < myKeys.entries());
			    UT_ASSERT(b >= 0 && b < myKeys.entries());
			    return myKeys(a) < myKeys(b);
			}
	private:
	    const UT_ValArray<UT_StringHolder>	&myKeys;
	};
	// If you get an error here, maybe you meant to call obegin()?
	explicit ordered_iterator(iterator it)
	    : myKeys()
	    , myValues()
	    , myOrder()
	    , myPos(0)
	    , myCurr(0)
	{
	    int		index = 0;
	    for (; !it.atEnd(); ++it, ++index)
	    {
		myKeys.append(it.name());
		myValues.append(it.entry());
		myOrder.append(index);
	    }
	    myOrder.stdsort( comparator(myKeys) );
	    if (myOrder.entries())
		myCurr = myOrder(0);
	}

	UT_ValArray<UT_StringHolder>		myKeys;
	UT_ValArray<const UT_OptionEntry *>	myValues;
	UT_IntArray				myOrder;
	int					myPos;
	int					myCurr;
	friend class		 UT_Options;
    };
    iterator		 begin() const
			 { return iterator(*this, myMap.begin(), myMap.end()); }
    iterator		 end() const
			 { return iterator(*this, myMap.end(), myMap.end()); }
    ordered_iterator	 obegin() const
			 { return ordered_iterator(begin()); }
    ordered_iterator	 oend() const
			 { return ordered_iterator(end()); }

    // Functions for accessing specific option values.
    int64		 getOptionI(const UT_StringRef &name) const;
    bool		 getOptionB(const UT_StringRef &name) const;
    fpreal64		 getOptionF(const UT_StringRef &name) const;
    const UT_Vector2D	 &getOptionV2(const UT_StringRef &name) const;
    const UT_Vector3D	 &getOptionV3(const UT_StringRef &name) const;
    const UT_Vector4D	 &getOptionV4(const UT_StringRef &name) const;
    const UT_QuaternionD &getOptionQ(const UT_StringRef &name) const;
    const UT_Matrix2D	 &getOptionM2(const UT_StringRef &name) const;
    const UT_Matrix3D	 &getOptionM3(const UT_StringRef &name) const;
    const UT_Matrix4D	 &getOptionM4(const UT_StringRef &name) const;
    const UT_Vector2D	 &getOptionUV(const UT_StringRef &name) const;
    const UT_Vector3D	 &getOptionUVW(const UT_StringRef &name) const;

    // Note that, if the string value contains binary data with null
    // characters, the UT_String version of getOptionS will not be able to
    // tell you the length of the data, since UT_String doesn't store the
    // length.
    const UT_StringHolder &getOptionS(const UT_StringRef &name) const;
    void		 getOptionS(const UT_StringRef &name, 
        		            std::string &value) const;
    void		 getOptionS(const UT_StringRef &name, 
        		            UT_String &value) const;
    void		 getOptionS(const UT_StringRef &name,
        		            UT_StringHolder &value) const;
    void		 getOptionS(const UT_StringRef &,
        		            UT_WorkBuffer &value) const;

    const UT_Int64Array		&getOptionIArray(const UT_StringRef &) const;
    const UT_Fpreal64Array	&getOptionFArray(const UT_StringRef &) const;
    const UT_StringArray	&getOptionSArray(const UT_StringRef &) const;

    // Returns a valid number if this option is a float or an int.
    // In the case of an int, it is converted to a float.
    fpreal64		 getOptionNum(const UT_StringRef &) const;

    // Functions for accessing option values.  These methods will perform some
    // minimal "casting".
    //	importOption(... bool &value)	- Tests integers and string options
    //	importOption(... fpreal &value)	- Evaluates integer/bool options
    //	importOption(... UT_Vector2 &)	- Evaluates Vector2 or UV
    //	importOption(... UT_Vector3 &)	- Evaluates Vector3 or UVW
    //	
    bool	 importOption(const UT_StringRef &name, int &value) const;
    bool	 importOption(const UT_StringRef &name, int64 &value) const;
    bool	 importOption(const UT_StringRef &name, bool &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              fpreal32 &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              fpreal64 &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              std::string &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_String &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_StringHolder &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Vector2F &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Vector3F &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Vector4F &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_QuaternionF &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Matrix2F &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Matrix3F &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Matrix4F &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Vector2D &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Vector3D &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Vector4D &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_QuaternionD &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Matrix2D &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Matrix3D &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Matrix4D &value) const;

    // Generic import which handles arbitrary length int/float data
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Array<int32> &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Array<int64> &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Array<fpreal32> &value) const;
    bool	 importOption(const UT_StringRef &name, 
        	              UT_Array<fpreal64> &value) const;

    // Import a string array
    bool	 importOption(const UT_StringRef &name,
			      UT_StringArray &value) const;

    // Import a single element from a scalar, vector, matrix or array
    bool	 importElement(const UT_StringRef &name,
			       fpreal &value,
			       exint index = 0) const;
    bool	 importElement(const UT_StringRef &name,
			       int64 &value,
			       exint index = 0) const;
    

    // All these functions cause optionChanged to be called.
    UT_Options		 &setOptionI(const UT_StringHolder &name,
				     int64 value);
    UT_Options		 &setOptionB(const UT_StringHolder &name,
				     bool value);
    UT_Options		 &setOptionF(const UT_StringHolder &name,
				     fpreal64 value);
    // Note that you can store binary data (that can contain null characters
    // or that isn't null-terminated) in a string option by passing in a
    // UT_WorkBuffer, or std::string.
    UT_Options		 &setOptionS(const UT_StringHolder &name,
              		             const UT_StringHolder &value);
    // setOptionSRaw() is the same as setOptionS() instead of it being
    // additionally tagged as a "raw" string so that getPyOptionString() and
    // appendPyDictionary() won't add quotes around the value. To retrieve its
    // value, use getOptionS().
    UT_Options		 &setOptionSRaw(const UT_StringHolder &name,
              		                const UT_StringHolder &value);
    UT_Options		 &setOptionV2(const UT_StringHolder &name,
              		              const UT_Vector2F &value);
    UT_Options		 &setOptionV2(const UT_StringHolder &name,
              		              const UT_Vector2D &value);
    UT_Options		 &setOptionV2(const UT_StringHolder &name,
              		              fpreal64 x, fpreal64 y);
    UT_Options		 &setOptionV3(const UT_StringHolder &name,
              		              const UT_Vector3F &value);
    UT_Options		 &setOptionV3(const UT_StringHolder &name,
              		              const UT_Vector3D &value);
    UT_Options		 &setOptionV3(const UT_StringHolder &name,
              		              fpreal64 x, fpreal64 y, fpreal64 z);
    UT_Options		 &setOptionV4(const UT_StringHolder &name,
              		              const UT_Vector4F &value);
    UT_Options		 &setOptionV4(const UT_StringHolder &name,
              		              const UT_Vector4D &value);
    UT_Options		 &setOptionV4(const UT_StringHolder &name,
              		              fpreal64 x, fpreal64 y,
              		              fpreal64 z, fpreal64 w);
    UT_Options		 &setOptionQ(const UT_StringHolder &name,
              		             const UT_QuaternionF &value);
    UT_Options		 &setOptionQ(const UT_StringHolder &name,
              		             const UT_QuaternionD &value);
    UT_Options		 &setOptionM2(const UT_StringHolder &name,
              		              const UT_Matrix2F &value);
    UT_Options		 &setOptionM2(const UT_StringHolder &name,
              		              const UT_Matrix2D &value);
    UT_Options		 &setOptionM3(const UT_StringHolder &name,
              		              const UT_Matrix3F &value);
    UT_Options		 &setOptionM3(const UT_StringHolder &name,
              		              const UT_Matrix3D &value);
    UT_Options		 &setOptionM4(const UT_StringHolder &name,
              		              const UT_Matrix4F &value);
    UT_Options		 &setOptionM4(const UT_StringHolder &name,
              		              const UT_Matrix4D &value);
    UT_Options		 &setOptionUV(const UT_StringHolder &name,
              		              const UT_Vector2F &value);
    UT_Options		 &setOptionUV(const UT_StringHolder &name,
              		              const UT_Vector2D &value);
    UT_Options		 &setOptionUVW(const UT_StringHolder &name,
              		               const UT_Vector3F &value);
    UT_Options		 &setOptionUVW(const UT_StringHolder &name,
              		               const UT_Vector3D &value);
    /// @{
    /// Set integer array options
    UT_Options		 &setOptionIArray(const UT_StringHolder &name,
              		                  const int32 *values, size_t size);
    UT_Options		 &setOptionIArray(const UT_StringHolder &name,
              		                  const int64 *values, size_t size);
    UT_Options		 &setOptionIArray(const UT_StringHolder &name,
              		                  const UT_Array<int64> &value);
    /// @}
    /// @{
    /// Set float/double array options
    UT_Options		 &setOptionFArray(const UT_StringHolder &name,
              		                  const fpreal32 *values, size_t size);
    UT_Options		 &setOptionFArray(const UT_StringHolder &name,
              		                  const fpreal64 *values, size_t size);
    UT_Options		 &setOptionFArray(const UT_StringHolder &name,
              		                  const UT_Array<fpreal64> &value);
    /// @}

    /// Set string array options
    UT_Options		 &setOptionSArray(const UT_StringHolder &name,
              		                  const UT_StringHolder *values,
					  size_t size);
    UT_Options		 &setOptionSArray(const UT_StringHolder &name,
              		                  const char *const*values,
					  size_t size);
    UT_Options		 &setOptionSArray(const UT_StringHolder &name,
              		                  const UT_StringArray &value);

    /// Get a hash code for the options
    unsigned		 hash() const;

    /// Comparison operator
    bool		 operator==(const UT_Options &src) const;
    bool		 operator!=(const UT_Options &src) const
			    { return !(*this == src); }

    /// Assignment operator
    const UT_Options	&operator=(const UT_Options &src);
    void		 merge(const UT_Options &src);
    void		 merge(const UT_Options &src,
			       bool (*match)(const UT_StringHolder &name,
					     const UT_OptionEntry *, void *),
			       void *data);
    virtual int64	 getMemoryUsage(bool inclusive) const;

    /// The map will take ownership of the UT_OptionEntry
    void		 setOption(const UT_StringHolder &name, 
        		           UT_OptionEntry *value);

protected:
    const UT_StringHolder	*findOptionS(const UT_StringRef &name) const;
    void		 addError(const char *filename, int linecount,
				  const char *error_str) const;
    bool		 sendOptionChanges() const
			    { return mySendOptionChanges; }
    void		 setSendOptionChanges(bool f)
			    { mySendOptionChanges = f; }
    virtual void	 optionChanged(const char *name);
    UT_OptionEntry	*parsePyValue(const char *&value,
				      char sep_delim,
				      char close_delim = '\0');

private:
    template <class ITER>
    bool save(ITER start, const char *filename, std::ostream &os) const;

    MapType				 myMap;
    bool				 mySendOptionChanges;
};

UT_API size_t format(char *buffer, size_t buffer_size, const UT_Options &v);

#endif
