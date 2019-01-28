/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CVEX_ValueList.h ( CVEX Library, C++)
 *
 * COMMENTS:	C++ interface to VEX.  This class is used to specify
 *		a list of input/output values
 */

#ifndef __CVEX_ValueList__
#define __CVEX_ValueList__

#include "CVEX_API.h"
#include "CVEX_Value.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_SymbolTable.h>

/// @brief List of input or output values for a CVEX_Context
///
/// This class maintains a list of the input and output parameters for a CVEX
/// context.
///
/// Both input and output variables are associated with parameters on the VEX
/// function being run.  Output variables are those flagged with the @c export
/// keyword.
class CVEX_API CVEX_ValueList 
{
public:
     CVEX_ValueList();
    ~CVEX_ValueList();

    /// Returns the number of values in the list
    int		 entries() const	{ return myValues.entries(); }

    /// Get a value by index
    const CVEX_Value *getValue(int i) const	{ return myValues(i); }
    CVEX_Value	*getValue(int i) { return myValues(i); }

    /// Get a value by name and type. Returns NULL if there's no value by
    /// that name and type.
    const CVEX_Value *getValue(const UT_StringRef &name, CVEX_Type type) const;
    CVEX_Value	*getValue(const UT_StringRef &name, CVEX_Type type);

    /// Get a value by name. Returns NULL if there's no value by that name.
    const CVEX_Value *getValue(const UT_StringRef &name) const
		 { return getValue(name, CVEX_TYPE_INVALID); }
    CVEX_Value	*getValue(const UT_StringRef &name)
		 { return getValue(name, CVEX_TYPE_INVALID); }
    
    /// Add a value by name type and varying flag.  This fails if:
    ///	- There's already a symbol with the name
    ///	- The type is invalid
    /// @note Users should call CVEX_Context::addInput()
    bool	 addValue(const UT_StringHolder &name,
			  CVEX_Type type, VEX_Precision, bool varying);

    /// Add a value by name, type and provide data.
    /// @note Users should call CVEX_Context::addInput()
    bool	 addValue(const UT_StringHolder &name, CVEX_Type type,
			  VEX_Precision,
        	          void *data, int array_size,
			  bool isconstant);
    
    /// Add a string value (with data)
    /// @note Users should call CVEX_Context::addInput()
    bool	 addValue(const UT_StringHolder &name,
			  CVEX_StringArray &strings,
			  bool isconstant);

    /// @{
    /// Convenience operators
    const CVEX_Value *operator[](int i) const { return getValue(i); }
    CVEX_Value	*operator[](int i)	{ return getValue(i); }

    CVEX_Value	*operator()(int i)	{ return getValue(i); }
    CVEX_Value	*operator()(const UT_StringRef &name, CVEX_Type type)
			{ return getValue(name, type); }
    /// @}

private:
    void		clear();
    void		addValue(CVEX_Value *value);

    // Clear the value but leave the null entry in myValues
    void		clearValue(int index);

    UT_StringMap<CVEX_Value *>	 mySymbols;
    UT_ValArray<CVEX_Value *>	 myValues;
    UT_String			 myError;

    friend class		 CVEX_Context;
};

#endif
