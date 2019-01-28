/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_BindingDataSource.h (sty  Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_BindingDataSource__
#define __STY_BindingDataSource__

#include "STY_API.h"
#include "STY_Types.h"
#include <UT/UT_Array.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class STY_BindingDataInfo;
class UT_StringArray;

/// Represents a data source for the override script input argument binding.
class STY_API STY_BindingDataSource :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_BindingDataSource>
{
public:
    virtual ~STY_BindingDataSource();

    /// Returns true if the subject can provide data given its name.
    /// If so, it also returns the type of the data and the tuple size.
    virtual bool    getOverrideBindingInfo( 
			STY_BindingDataInfo &data_info ) const;

    /// Gets the int array data and returns true on success.
    virtual bool    getOverrideBindingData( UT_IntArray &data ) const;

    /// Gets the float array data and returns true on success.
    virtual bool    getOverrideBindingData( UT_FprealArray &data ) const;

    /// Gets the string array data and returns true on success.
    virtual bool    getOverrideBindingData( UT_StringArray &data ) const;

    /// Creates a STY_Binding set to a contsant value equivalent to the data
    /// returned by this source.
    STY_BindingHandle createConstantBinding(
			const UT_StringHolder &arg_name) const;
};

#endif

