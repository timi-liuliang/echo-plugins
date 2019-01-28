/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_InOutInfo.h ( DOP Library, C++)
 *
 * COMMENTS:	Input/output connection information for DOPs.
 */

#ifndef __DOP_InOutInfo__
#define __DOP_InOutInfo__

#include "DOP_API.h"

#include <UT/UT_Assert.h>

class DOP_Node;

typedef enum {
    DOP_INOUT_OBJECTS,
    DOP_INOUT_DATA,
    DOP_INOUT_RELDATA,
    DOP_INOUT_SOLVER,
    DOP_INOUT_ANY,		// Matches any data type.
    DOP_INOUT_INVALID
} DOP_InOutDataType;

class DOP_API DOP_InOutInfo
{
public:
				 DOP_InOutInfo()
				     : myDataType(DOP_INOUT_INVALID),
				       myIsRepetitive(false)
				 { }
				 DOP_InOutInfo(DOP_InOutDataType datatype,
					       bool isrepetitive)
				     : myDataType(datatype),
				       myIsRepetitive(isrepetitive)
				 { }
				 DOP_InOutInfo(const DOP_InOutInfo &src)
				 { *this = src; }
				~DOP_InOutInfo()
				 { }

    const DOP_InOutInfo		&operator=(const DOP_InOutInfo &src)
				 {
				     myDataType = src.getDataType();
				     myIsRepetitive = src.getIsRepetitive();

				     return *this;
				 }
    bool			 operator==(const DOP_InOutInfo &src)
				 {
				     return (myDataType == src.getDataType() &&
				     myIsRepetitive == src.getIsRepetitive());
				 }

    bool			 getIsRepetitive() const
				 { return myIsRepetitive; }

    /// Returns the primary type of this connection.  One should use
    /// matchesDataType to properly handle variadic DOPs.
    DOP_InOutDataType		 getDataType() const
				 { return myDataType; }

    static const char		*getLabelText(DOP_InOutDataType type)
				{
				    switch (type)
				    {
					case DOP_INOUT_OBJECTS:
					    return "Objects to be processed";
					case DOP_INOUT_DATA:
					    return "Data to be attached";
					case DOP_INOUT_SOLVER:
					    return "Solvers to be attached";
					case DOP_INOUT_RELDATA:
					    return "Relationships to create";
					case DOP_INOUT_ANY:
					    return "Any node type";
					case DOP_INOUT_INVALID:
					    return "Invalid node type";
					    break;
				    }
				    UT_ASSERT(0);
				    return "Invalid node type";
				}

    const char			*getLabelText() const
				 {
				     return getLabelText(getDataType());
				 }

    /// Returns true if this connection can match the given type.
    bool			 matchesDataType(DOP_InOutDataType type) const
				 {
				     // Trivially false cases.
				     if (type == DOP_INOUT_INVALID)
					 return false;
				     if (getDataType() == DOP_INOUT_INVALID)
					 return false;
				     // Trivially true cases.
				     if (type == DOP_INOUT_ANY)
					 return true;
				     if (getDataType() == DOP_INOUT_ANY)
					 return true;
				     // SOLVER and DATA are cross-compatible
				     if (getDataType() == DOP_INOUT_SOLVER &&
					 type == DOP_INOUT_DATA)
					 return true;
				     if (getDataType() == DOP_INOUT_DATA &&
					 type == DOP_INOUT_SOLVER)
					 return true;
				     // Final possibillity
				     return (type == getDataType());
				 }

private:
    DOP_InOutDataType		 myDataType;
    bool			 myIsRepetitive;
};

#endif
