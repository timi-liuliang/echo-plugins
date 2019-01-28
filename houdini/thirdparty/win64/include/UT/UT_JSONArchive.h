/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONArchive.h (UT Library, C++)
 *
 * COMMENTS:	JSON archive for use with UT_Serialization
 *
 */

#ifndef __UT_JSONARCHIVE_H_INCLUDED__
#define __UT_JSONARCHIVE_H_INCLUDED__

/// @file
/// JSON archive for use with UT_Serialization
///

#include "UT_JSONParser.h"
#include "UT_JSONWriter.h"
#include "UT_UniquePtr.h"
#include "UT_Serialization.h"

/// @class UT_JSONArchive
/// @brief Use this to create a class archive for serializing classes to/from
/// streams as JSON.
///
/// To serialize your data, you first have to implement serialization functions
/// as described in UT_Serialize.h. Then just this class to perform it.
/// eg.
/// @code
///	// form 1
///	cout << UT_JSONArchive<MyClassA>::Output(a_object);
///	cin  >> UT_JSONArchive<MyClassB>::Input(b_object);
///
///	// form 2
///     UT_JSONArchive<MyClassA>::Output output_archive(my_object);
///     UT_JSONArchive<MyClassA>::Input  input_archive(my_object);
///	output_archive(cout);
///	input_archive(cin);
/// @endcode
///

struct UT_JSONArchiver
{
    class Output : public UT_SaveArchiver<Output>
    {
    public:
	Output(UT_OStream &os)
	    : myOut(UT_JSONWriter::allocWriter(os.getOStream(), os.isBinary()))
	{
	}

	bool serializeMapBegin()
	{
	    return myOut->jsonBeginMap();
	}
	bool serializeMapEnd()
	{
	    return myOut->jsonEndMap();
	}
	bool serializeKey(const char *key)
	{
	    return myOut->jsonKeyToken(key);
	}

	template <typename T>
	bool serializeValue(const T &val)
	{
	    return myOut->jsonValue(val);
	}
	bool serializeStringValue(const char *str)
	{
	    return myOut->jsonStringToken(str);
	}

	bool serializeArrayBegin(int64 count)
	{
	    if (!myOut->jsonBeginArray())
		return false;
	    return serializeValue(count);
	}
	bool serializeArrayEnd()
	{
	    return myOut->jsonEndArray();
	}
	// serializeArray() provided by superclass

	template <typename T>
	bool serializeUniformArray(T *vec, int64 count)
	{
	    return myOut->jsonUniformArray(count, vec);
	}

    private:
	UT_UniquePtr<UT_JSONWriter> myOut;
    };

    class Input : public UT_LoadArchiver<Input>
    {
    public:
	Input(UT_IStream &is)
	    : myAutoParser(is)
	{
	}

	UT_JSONParser	&parser()
			{
			    return *myAutoParser;
			}

	bool serializeMapBegin()
	{
	    myTraverser = parser().beginMap();
	    return !myTraverser.atEnd();
	}
	bool serializeMapEnd() // return true when we hit the end
	{
	    myTraverser.advance();
	    return myTraverser.atEnd();
	}
	bool serializeKey(UT_WorkBuffer &key)
	{
	    return myTraverser.getLowerKey(key);
	}

	template <typename T>
	bool serializeValue(T &val)
	{
	    if (!parser().parseValue(val))
		return false; // end of file
	    return true;
	}
	bool serializeStringValue(UT_WorkBuffer &str)
	{
	    if (!parser().parseString(str))
		return false; // end of file
	    return true;
	}

	bool serializeArrayBegin(int64 &count)
	{
	    bool error;
	    if (!parser().parseBeginArray(error))
		return false;
	    return serializeValue(count);
	}
	bool serializeArrayEnd()
	{
	    bool error;
	    if (!parser().parseEndArray(error))
		return false;
	    return true;
	}
	// serializeArray() provided by superclass

	template <typename T>
	bool serializeUniformArray(T *vec, int64 count)
	{
	    return (parser().parseUniformArray(vec, count) == count);
	}

    private:
	UT_AutoJSONParser	    myAutoParser;
	UT_JSONParser::iterator    myTraverser;
    };
};

/// Main class for starting serialization
template <typename OBJ_T>
struct UT_JSONArchive
{
    typedef UT_LoadArchiverManip<OBJ_T,UT_JSONArchiver>  Input;
    typedef UT_SaveArchiverManip<OBJ_T,UT_JSONArchiver>  Output;
};

#endif // __UT_JSONARCHIVE_H_INCLUDED__
