/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_DetailInfo.h (C++)
 *
 * COMMENTS:	This class takes in a GU_Detail object and calculates/stores a 
 * 		bunch of stats based on info extracted from that object. This 
 * 		code used to be repeated in a few different place elsewhere, but 
 *		has now been encapsulated here because we're better than that!
 */

#ifndef __GU_DetailInfo_h__
#define __GU_DetailInfo_h__

#include "GU_API.h"
#include "GU_Detail.h"

#include <UT/UT_Array.h>
#include <UT/UT_ArraySet.h>

class UT_InfoTree;

//////////////////////////////////////////////////////////////////////////////
//
//	gu_NameValuePair
//
//	A simple class to store name/value pairs, where both name and value
//	are strings.
//
//////////////////////////////////////////////////////////////////////////////

class GU_API gu_NameValuePair
{
public:
		     gu_NameValuePair (const char *label, const char *value);
		     gu_NameValuePair (const char *label, int value);
		     ~gu_NameValuePair();

    void 	     setName(const char *label)	  { myLabel.harden(label); }
    const UT_String &getName() const 		  { return myLabel;        }
    
    void 	     setValue(const char *value)  { myValue.harden(value); }
    void	     setValue(int value);
    const UT_String &getValue() const		  { return myValue;        }

    static int	     compare(gu_NameValuePair **a, gu_NameValuePair **b);

private:
    UT_String	myLabel;
    UT_String	myValue;
};

//////////////////////////////////////////////////////////////////////////////
//
//	GU_DetailInfo
//
//////////////////////////////////////////////////////////////////////////////

class GU_API GU_DetailInfo 
{
public:
    GU_DetailInfo();
    ~GU_DetailInfo();

    /// Fill 'branch' with info taken from 'geo'. Note: This method calls 
    /// extractInfo(..)
    void		fillInfoTree(const GU_Detail &geo,
				const UT::ArraySet<const void *> *ignore_memory,
				bool instanced,
				bool debug,
				UT_InfoTree &branch);
    
private:
    void		addGroups(UT_InfoTree &branch,
				UT_ValArray<const GA_ElementGroup *> &groups,
				UT_ValArray<const GA_ElementGroup *> &mixgroups,
				const char *group_name,
                                bool debug);
    void		addGroups(UT_InfoTree &branch,
				UT_ValArray<const GA_EdgeGroup *> &groups,
				const char *group_name,
                                bool debug);
    
    void		addAttribute(UT_InfoTree *tree,
				const GA_Attribute *atr,
				bool debug);
    void		addAllAttributesToTree(UT_InfoTree *tree, 
				const GA_AttributeDict &dict,
				bool debug);

    // Clear all member variable data
    void 		clearDetails();

    /// Delete all heap member data
    void		deleteHeapDataMembers();

    /// Clears old member data and extracts new data from 'geo' and
    /// recalculates all stats
    void 		extractInfo(const GU_Detail &geo,
				const UT::ArraySet<const void *> *ignore_memory,
				bool instanced,
				bool debug);

    // Attributes. Note: Ct stands for "Count"
    GA_Size	myNurbcRatCt;
    GA_Size	myNurbsRatCt;
    GA_Size	myRbezcRatCt;
    GA_Size	myRbezsRatCt;
    GA_Size	myVpartCt;
    GA_Size	myVolumeCt;
    GA_Size	myVDBCt;
    GA_Size	myProfCt;
    GA_Size	myVisProfCt;
    GA_Size	myPackHidden;
    GA_Size	myPackLoaded;
    GA_Size	myPackTotal;
    GA_Size	myPtGrpsCt;
    GA_Size	myPrimGrpsCt;
    GA_Size	myEdgeGrpsCt;
    int64	myFullMemUseInBytes;
    int64	myNewMemUseInBytes;
    int64	myUniqueMemUseInBytes;
    bool	myInstancedMem;
    bool	myDegenerate;

    // Custom Variable Mappings
    UT_Array<gu_NameValuePair *>	myCustomVarMappings;
};

#endif
