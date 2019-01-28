/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_CEAttribute.h ( GA Library, C++)
 *
 * COMMENTS:
 *	A GA_Attribute backed on the Compute Engine
 */

#ifndef __GA_CEAttribute__
#define __GA_CEAttribute__

#include "GA_API.h"

#include <CE/CE_Context.h>
#include "GA_Detail.h"
#include "GA_Attribute.h"

#ifdef CE_ENABLED

class GA_API GA_CEAttribute
{
public:
    GA_CEAttribute();

    virtual ~GA_CEAttribute();

    void		clearBuffers();

    bool		isValid() const
    {
	return myBuffer();
    }

    cl::Buffer		buffer() const { return myBuffer; }
    cl::Buffer		indexBuffer() const { return myIndexBuffer; }
    GA_Size		entries() const { return myEntries; }
    int			tupleSize() const { return myTupleSize; }
    GA_StorageClass	storage() const { return myStorage; }
    bool		isArray() const { return myIsArray; }

    bool		attributeToOCL(const GA_Detail *gdp,
				    GA_AttributeOwner owner,
				    const char *aname,
				    GA_StorageClass storage,
				    int &tuplesize,
				    bool isarray,
				    bool docopy = true)
    {
	if (!gdp) return false;
	if (!UTisstring(aname))
	    return false;
	const GA_Attribute *attrib = gdp->getAttributes().findAttribute(owner, aname);
	if (!attrib)
	    return false;
	return attributeToOCL(attrib, storage, tuplesize, isarray, docopy);
    }
    bool		attributeToOCL(const GA_Attribute *attrib,
				    GA_StorageClass storage,
				    int &tuplesize,
				    bool isarray,
				    bool docopy = true);

    bool		oclToAttribute(GA_Detail *gdp, GA_AttributeOwner owner, const char *aname)
    {
	if (!gdp) return false;
	if (!UTisstring(aname))
	    return false;
	GA_Attribute *attrib = gdp->getAttributes().findAttribute(owner, aname);
	if (!attrib)
	    return false;
	return oclToAttribute(attrib);
    }
    bool		oclToAttribute(GA_Attribute *attrib);

    bool		isWriteBackRequired() const { return myWriteBackRequired; }
    void		setWriteBackRequired(bool isrequired) { myWriteBackRequired = isrequired; }

protected:

    // GPU Buffers:
    cl::Buffer		myBuffer;
    cl::Buffer		myIndexBuffer;

    // CPU Buffers:
    UT_Array<cl_int>	myIndexArray;
    UT_Array<cl_float>	myFloatArray;
    UT_Array<cl_int>	myIntArray;

    // Number of entries.
    GA_Size		myEntries;
    GA_Size		myTupleSize;
    GA_StorageClass	myStorage;
    bool		myIsArray;

    // Whether the buffer has been written to by CE
    bool		myWriteBackRequired;
};

#else

class GA_API GA_CEAttribute
{
public:
};

#endif
#endif
