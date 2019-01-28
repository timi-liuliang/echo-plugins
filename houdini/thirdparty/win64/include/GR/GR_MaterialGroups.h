/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_MaterialGroups.h (GR Library, C++)
 *
 * COMMENTS:
 */
#ifndef GR_MaterialGroups_h
#define GR_MaterialGroups_h

#include <GT/GT_Primitive.h>
#include <RE/RE_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_Map.h>

#include "GR_API.h"

class RE_MaterialAtlas;


/// A class that manages material assignments to different groups of primitives.
class GR_API GR_MaterialGroups
{
public:
		 GR_MaterialGroups();
		~GR_MaterialGroups();
    
    int		getNumGroups() const	{ return myMatGroups.entries(); }
    bool	isGroupVisible(int i) const
					{ return myMatGroups(i).visible; }
    RE_MaterialPtr
		getGroupMaterial(int group_index,
				 const RE_MaterialAtlas &atlas,
				 int default_material = -1) const;
    int		getGroupMaterialID(int group_index) const;
    int		getGroupUnmappedID(int group_index) const
    { return myMatGroups(group_index).mat_id; }

    void	resetGroups()	{ myMatGroups.clear(); }
    void	appendGroup(int mat_id)
				{ myMatGroups.append();
				  myMatGroups.last().mat_id = mat_id; }
    void	setNumGroups(int num)
				{ myMatGroups.entries(num); }
    void	setGroupVisibility(int group_index, bool visible)
				{ myMatGroups(group_index).visible = visible; }
    void	setAllGroupsVisible(bool visible = true);

    void	setGroupMaterial(int group_index, int mat_id)
				{ myMatGroups(group_index).mat_id = mat_id; }

    void	updateMaterialRemap(const GT_PrimitiveHandle &for_prim);

private:
    class gr_MatGroup
    {
    public:
	gr_MatGroup() : visible(true), mat_id(-1) {}

	bool visible;
	int  mat_id;
    };

    UT_Array<gr_MatGroup> myMatGroups;
    UT_Map<int,int>	  myMatRemap;
    int64		  myMatRemapDataID;
};

#endif
