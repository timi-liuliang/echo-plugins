/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttribXlateH9.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute translate info for .geo/.bgeo file I/O
 */

#ifndef __GA_AttribXlateH9__
#define __GA_AttribXlateH9__

#include "GA_API.h"

#include "GA_Types.h"
#include "GA_AIFFileH9.h"

#include <UT/UT_StringMap.h>


/// Class used to lookup additional properties for attributes loaded from
/// H9 geometry files.
class GA_API GA_AttribXlateH9
{
public:
    class GA_API XlateInfo
    {
	public:
	    XlateInfo(GA_AIFFileH9::AttribType src_type, int src_byte_size,
		      GA_AIFFileH9::AttribTypeInfo src_type_info)
		: mySourceType(src_type)
		, mySourceByteSize(src_byte_size)
		, mySourceTypeInfo(src_type_info)
		, myTypeInfo(GA_TYPE_VOID)
		, myNonTransformingSet(false)
		, myNonTransforming(false)
	    {
	    }

	    bool	 match(GA_AIFFileH9::AttribType src_type,
			       int src_byte_size,
			       GA_AIFFileH9::AttribTypeInfo src_type_info)
									const;

	    /// @{
	    /// Mapped GA type information.  For example, GB had no color,
	    /// while it usually makes sense to map the Cd attribute to
	    /// GA_TYPE_COLOR.
	    GA_TypeInfo	 getTypeInfo() const { return myTypeInfo; }
	    void	 setTypeInfo(GA_TypeInfo t) { myTypeInfo = t; }
	    /// @}

	    /// @{
	    /// Some attributes should have type information, but be flagged
	    /// with the non-tranforming property.  For example "rest" is a
	    /// point, but shouldn't be transformed.
	    bool	nonTransformingSet() const
			{ return myNonTransformingSet; }
	    bool	nonTransforming() const	{ return myNonTransforming; }
	    void	setNonTransforming(bool state)
			{
			    myNonTransformingSet = true;
			    myNonTransforming = state;
			}
	    /// @}

	private:
	    GA_AIFFileH9::AttribType		 mySourceType;
	    int					 mySourceByteSize;
	    GA_AIFFileH9::AttribTypeInfo	 mySourceTypeInfo;

	    GA_TypeInfo				 myTypeInfo;
	    bool				 myNonTransformingSet;
	    bool				 myNonTransforming;
    };

    const XlateInfo	*lookup(const char *name, GA_AIFFileH9::AttribType type,
				int byte_size,
				GA_AIFFileH9::AttribTypeInfo type_info) const;
    void		 append(const char *name, const XlateInfo &xlate_info);

    /// An absolute file path will load only that single file, otherwise the
    /// HOUDINI_PATH will be searched and all matching files will be loaded
    /// into a single xlate object.
    static const GA_AttribXlateH9	*getXlate(const char *file_name);

private:
     GA_AttribXlateH9() {}
    ~GA_AttribXlateH9() {}

    static GA_AttribXlateH9		*loadXlate(const char *file_name);

private:
    UT_StringMap<UT_Array<XlateInfo> > myTable;
};

#endif
