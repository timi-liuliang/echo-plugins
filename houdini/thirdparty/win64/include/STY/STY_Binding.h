/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Binding.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Binding__
#define __STY_Binding__

#include "STY_API.h"
#include "STY_Types.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class UT_JSONValue;
class UT_JSONValueMap;

//  =========================================================================== 
/// Defines a data binding for the script argument.
class STY_API STY_Binding :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_Binding>
{
public:
    /// Describes what kind of data the argument is bound to.
    enum DataSourceType
    {
	SOURCE_DEFAULT,		// Default (first try fully matched subject,
				// and if data not found, try pre-bindings).
	SOURCE_PREBINDING,	// Data taken from partial match subjects.
	SOURCE_CONSTANT,	// Constant value set right in the style sheet.
    };

    /// Creates a binding definition for the given argument name and
    /// the json value that describes the data source.
		 STY_Binding(const UT_StringHolder &arg_name,
			 const UT_JSONValue *json);
		 STY_Binding(const UT_StringHolder &arg_name,
			 const STY_ConstantHandle &const_value);
		 STY_Binding(DataSourceType data_source_type,
			 const UT_StringHolder &arg_name,
			 const UT_StringHolder &data_name,
			 const STY_ConstantHandle &const_value);
		~STY_Binding();
    
    /// Loads binding array from a given json value. The caller owns the
    /// binding objects returned in the array.
    static bool	loadBindings(UT_Array<STY_BindingHandle> &bindings,
		    const UT_JSONValue *json);

    /// @{ Getters for binding definition items.
    DataSourceType		getDataSourceType() const
				{ return myDataSourceType; }
    const UT_StringHolder	&getArgName() const
				{ return myArgName; }
    const UT_StringHolder	&getDataName() const
				{ return myDataName; }
    const STY_BindingDataSourceHandle &
				getConstantDataSource() const
				{ return myConstantDataSource; }
    STY_ConstantHandle		getConstantValue() const;
    /// @}

    /// Save this binding as JSON to a style sheet.
    void		saveStyleSheet(UT_JSONValueMap *map) const;

private:
    void		loadBindingFromMap(const UT_JSONValue *json);

    DataSourceType		 myDataSourceType;
    UT_StringHolder		 myArgName;
    UT_StringHolder		 myDataName;
    STY_BindingDataSourceHandle	 myConstantDataSource;
};

#endif

