/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_BindingDataInfo.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_BindingDataInfo__
#define __STY_BindingDataInfo__

#include "STY_API.h"

//  =========================================================================== 
/// Provides information about binding data.
class STY_API STY_BindingDataInfo
{
public:
		STY_BindingDataInfo()
		    : myDataType(TYPE_FLOAT),
		      myTupleSize(0),
		      myIsArray(false)
		{ }

    /// Encodes basic types for overide values.
    enum DataType
    {
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING,
    };

    /// Sets data type used for overrided binding.
    void	setDataType( DataType data_type ) 
			{ myDataType = data_type; }
    DataType	getDataType() const
			{ return myDataType; }

    /// Size of the tuple. 
    /// If it's an array then the array element is a tuple of that size.
    void	setTupleSize( int tuple_size ) 
			{ myTupleSize = tuple_size; }
    int		getTupleSize() const
			{ return myTupleSize; }

    /// Checks if the binding data is an array.
    void	setIsArray( bool is_array )
			{ myIsArray = is_array; }
    bool	isArray() const
			{ return myIsArray; }
   
private:
    DataType	myDataType;
    int		myTupleSize;
    bool	myIsArray;
};

#endif

