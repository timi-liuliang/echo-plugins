/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_FloatTupleAdapter.h ( GA Library, C++)
 *
 * COMMENTS:	A simple class that adapts GA_AttributeRefMap interfaces to
 *		produce float tuples.  This class can be used in
 *		combination with functions like GEO_TPSurf::evaluateMesh()
 *		to produce float tuples even when the interface requires a
 *		GA_AttributeRefMap and produces results in vertices or
 *		points.
 */

#ifndef __GA_FloatTupleAdapter__
#define __GA_FloatTupleAdapter__

#include <UT/UT_UniquePtr.h>
#include <UT/UT_StackBuffer.h>
#include "GA_AttributeRefMap.h"
#include "GA_Detail.h"

class GA_FloatTupleAdapter
{
public:
    /// Create a float tuple adapter for a given attribute.  The attribute
    /// should be a float tuple.  This operation will allocate temporary
    /// storage for `size` results.
    GA_FloatTupleAdapter(const GA_Attribute *attr, int size)
	: myDest(attr->getDetail().cloneEmptyDetail(false))
	, myDAttr(myDest->getAttributes().cloneAttribute(
		    attr->getOwner(),
		    attr->getName(),
		    *attr, true))
	, myList(*myDest, &attr->getDetail())
	, myResults(size)
	, mySize(size)
    {
	myList.append(myDAttr, attr);
	for (int i = 0; i < size; i++)
	    myResults[i] = myList.getTempVertex(myList.appendTempVertex());
    }

    GA_AttributeRefMap	&getList()	{ return myList; }
    GA_Offset		*getResults()	{ return myResults; }

    /// Extract results to a float array.  The float array has stride
    /// specified by `asize`.
    void extractToFloats(float *data, int asize)
    {
	GA_ROHandleF	handle(myDAttr);
	for (int i = 0; i < mySize; i++)
	{
	    if (myDAttr->getOwner() == GA_ATTRIB_POINT)
		handle.getV(myDest->vertexPoint(myResults[i]), data, asize);
	    else
		handle.getV(myResults[i], data, asize);
	    data += asize;
	}
    }

    /// Extract to a templated type, which must have value_type of float
    template <typename T>
    void extract(T *data)
    {
	extractToFloats(data, sizeof(T)/sizeof(float));
    }

private:
    UT_UniquePtr<GA_Detail>	 myDest;
    GA_Attribute		*myDAttr;
    GA_AttributeRefMap		 myList;
    UT_StackBuffer<GA_Offset>	 myResults;
    int				 mySize;
};

#endif
