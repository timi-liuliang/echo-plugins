/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_SubDivPacking.h ( GU Library, C++)
 *
 * COMMENTS:	The subdivision code "packs" floating point attributes into
 *		smaller data structures for efficiency.  This class keeps track
 *		of the mapping between the packed and unpacked data.
 */

#ifndef __GU_SubDivPacking__
#define __GU_SubDivPacking__

#include "GU_API.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_IntArray.h>
#include <GA/GA_Types.h>

class GA_GBElement;
class GA_Attribute;
class GA_AttributeDict;

class GU_API GU_SubDivPacking
{
public:
     /// Default constructor
     GU_SubDivPacking();
     /// Constructor which builds all float attributes for a dictionary
     GU_SubDivPacking(const GA_AttributeDict &dict);
     /// Copy constructor
     GU_SubDivPacking(const GU_SubDivPacking &src);
     /// Destructor
    ~GU_SubDivPacking();

    /// Assignment operator
    GU_SubDivPacking	&operator=(const GU_SubDivPacking &src);

    /// Clear the packing tables
    void	clear();

    /// Add all float attributes from the given dictionary
    void	addFloatAttributes(const GA_AttributeDict &dict);

    /// Add a single attribute.  If a tuple size is specified, it will
    /// override the tuple size fo the attribute.  Tuple sizes must be
    /// <= the actual attribute tuple size.
    void	addAttribute(GA_Attribute *atr,
			     bool check_dups, int tuple_size = -1);

    /// Pack floats from an object's attributes into a packed buffer
    template <typename T>
    void	packFloats(T *data, GA_Offset off) const;
    /// Unpack data in the buffer into the object's attributes
    template <typename T>
    void	unpackFloats(GA_Offset off, const T *data) const;

    /// Number of attributes
    int			 entries() const { return myAttributes.entries(); }

    /// Number of floats required for the packed array
    int			getCount() const	{ return myCount; }

    /// Offset array into the packed array.  This is the offset (not the
    /// byte-count)
    const UT_StringArray		&getNames() const
					    { return myNames; }
    /// Offset array into the packed array.  This is the offset (not the
    /// byte-count)
    const UT_IntArray			&getOffsets() const
					    { return myOffsets; }
    /// Number of floats for each attribute (in floats, not bytes)
    const UT_IntArray			&getSizes() const
					    { return mySizes; }

    /// Get the attribute offset (i.e. offset into the packed data)
    int			getPackedOffset(const GA_Attribute *atr) const;

private:
    UT_ValArray<GA_Attribute *>		myAttributes;
    UT_StringArray			myNames;
    UT_IntArray				myOffsets;
    UT_IntArray				mySizes;
    int					myCount;
};

#endif
