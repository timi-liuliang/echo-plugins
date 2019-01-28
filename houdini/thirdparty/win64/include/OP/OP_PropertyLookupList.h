/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_PropertyLookupList.h ( OP Library, C++)
 *
 * COMMENTS:	Class used to perform batch lookup of parameters/properties.
 * 		The list should be initialized with the parameter names.
 * 		Calling OP_Parameters::lookupParameterOrProperties() will fill
 * 		out the objects and parameter information.
 */

#ifndef __OP_PropertyLookupList__
#define __OP_PropertyLookupList__

#include "OP_API.h"

#include <UT/UT_ValArray.h>

class OP_Node;
class PRM_Parm;

class OP_API OP_PropertyLookupList {
public:
     OP_PropertyLookupList()	{}
    ~OP_PropertyLookupList()	{}

    void	reserve(int size)		// Reserve space for parameters
		{
		    myNames.setCapacityIfNeeded(size);
		    mySrcNodes.setCapacityIfNeeded(size);
		    myNodes.setCapacityIfNeeded(size);
		    myParmPtrs.setCapacityIfNeeded(size);
		    myLookupData.setCapacityIfNeeded(size);
		}
    void	clear()
		{
		    myNames.entries(0);
		    mySrcNodes.entries(0);
		    myNodes.entries(0);
		    myParmPtrs.entries(0);
		    myLookupData.entries(0);
		}

    // The names are passed by reference and must be maintained by the caller.
    // The strings are *not* duplicated in this list.
    void	 addParm(const char *name, OP_Node *src, int data)
		 {
		    myNames.append(name);
		    mySrcNodes.append(src);
		    myNodes.append((OP_Node *)0);
		    myParmPtrs.append((PRM_Parm *)0);
		    myLookupData.append(data);
		 }
    int		 entries() const		{ return myNames.entries(); }
    const char	*getName(int i) const		{ return myNames(i); }
    OP_Node	*getSourceNode(int i) const	{ return mySrcNodes(i); }

    // After the lookup has been performed, the following methods may be called
    bool	 isFound(int i) const		{ return myNodes(i) != 0; }
    OP_Node	*getNode(int i) const		{ return myNodes(i); }
    PRM_Parm	*getParmPtr(int i) const	{ return myParmPtrs(i); }
    int		 getLookupData(int i) const	{ return myLookupData(i); }

    // The following method is 
    void	 setLookupInfo(int i, OP_Node *node, PRM_Parm *parm)
		 {
		    UT_ASSERT(i >= 0 && i < myNodes.entries());
		    myNodes(i) = node;
		    myParmPtrs(i) = parm;
		 }

private:
    UT_ValArray<const char *>	myNames;
    UT_ValArray<OP_Node *>	mySrcNodes;
    UT_ValArray<OP_Node *>	myNodes;
    UT_ValArray<PRM_Parm *>	myParmPtrs;
    UT_ValArray<int>		myLookupData;
};

#endif
