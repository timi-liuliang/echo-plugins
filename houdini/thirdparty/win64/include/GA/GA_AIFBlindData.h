/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFBlindData.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_AIFBlindData__
#define __GA_AIFBlindData__

#include "GA_API.h"
#include "GA_Types.h"

#include <stddef.h>


class GA_Attribute;


/// @brief Attribute Interface for blind data per element
///
/// This attribute interface allows control over a "blind" attribute.
/// Each element is viewed as having a block of N bytes associated with it.
/// You can get a blind pointer to this data to read/write data.
/// @warning No constructor/destructors are called on the blind data, the blind
/// data should be used for POD types like floats or ints, and only if there's
/// no way to use friendlier attribute types.
class GA_API GA_AIFBlindData 
{
public:
	     GA_AIFBlindData();
    virtual ~GA_AIFBlindData();

    enum GA_BlindDataEndian
    {
	GA_BLINDDATA_LITTLE_ENDIAN,	// Data is little endian
	GA_BLINDDATA_BIG_ENDIAN		// Data is big endian
    };

    /// Set the data size.  Each element will have this many bytes of data
    /// allocated for it.
    virtual bool	 setDataSize(GA_Attribute *attrib,
				GA_Size size_in_bytes,
				const void *default_value=NULL) const = 0;

    /// Set data alignment.  This method will force data to be aligned on this
    /// many bytes.  An alignment of 0 specifies the data will be aligned with
    /// @c sizeof(void*) on the particular architecture.
    virtual bool	 setDataAlignment(GA_Attribute *attrib,
				GA_Size alignment_bytes) const = 0;

    /// @{
    /// Query state of attribute
    virtual GA_Size	 getDataSize(const GA_Attribute *attrib) const = 0;
    virtual GA_Size	 getDataAlignment(const GA_Attribute *attrib) const = 0;
    /// @}

    /// @{
    /// Get/Set the save flag.  If the save flag is set, the blind data will be
    /// saved with geometry (if the attribute supports it)
    virtual bool	 getSaveFlag(const GA_Attribute *a) const = 0;
    virtual void	 setSaveFlag(GA_Attribute *a, bool save) const = 0;
    /// @}

    /// @{
    /// Query or set the endian flag.
    /// @note Calling @c swapEndian() will change the state of the endian flag,
    /// but will @b not swap the data (this is the caller's responsibility).
    virtual GA_BlindDataEndian	getEndianFlag(const GA_Attribute *a) const = 0;
    virtual void		swapEndianFlag(GA_Attribute *a) const = 0;
    /// @}

    /// Get a read pointer to the data for a given element
    virtual const void	*getReadData(const GA_Attribute *attrib,
				GA_Offset offset) const = 0;

    /// Get a write pointer to the data for a given element
    virtual void	*getWriteData(GA_Attribute *attrib,
				GA_Offset offset) const = 0;

    /// Convenience function to access value as a read-only object
    template <typename T>
    const T	&getValue(const GA_Attribute *atr, GA_Offset off) const
		    { return *static_cast<const T *>(getReadData(atr, off)); }
    /// Convenience function to access value as a writable object
    template <typename T>
    T		&getWriteValue(GA_Attribute *atr, GA_Offset off) const
		    { return *static_cast<T *>(getWriteData(atr, off)); }
    /// Convenience function to set value to an object
    template <typename T>
    void	 setValue(GA_Attribute *atr, GA_Offset off,
			    const T &value) const
		    { *static_cast<T *>(getWriteData(atr, off)) = value; }
};

#endif
