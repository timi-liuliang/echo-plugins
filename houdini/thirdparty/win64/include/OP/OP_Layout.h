/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        OP_Layout.C (Operator AutoLayout, C++)
 *
 * COMMENT: 	Layout of the operators on the worksheet.
 * 
 *
 */

#ifndef __OP_Layout__
#define __OP_Layout__

#include "OP_API.h"
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>

class OP_Node;
class OP_Network;
class OP_IndirectInput;
class OP_NetworkBoxItem;
class op_LayoutTile;
class op_LayoutTree;
class op_LayoutBranch;

enum OP_LayoutDirection
{
   OP_LAYOUT_BOT_TO_TOP, 	// root at the bottom
   OP_LAYOUT_RIGHT_TO_LEFT,	// root at the right
   OP_LAYOUT_TOP_TO_BOT  	// root at the top
};

enum OP_LayoutSort
{
   OP_LAYOUT_SORT_NONE, 	// sort off
   OP_LAYOUT_SORT_ABC,		// sort alphabetically
   OP_LAYOUT_SORT_OPTYPE  	// sort by op type
};

typedef UT_ValArray<op_LayoutTile   *>  op_TileList;
typedef UT_ValArray<op_LayoutTree   *>  op_TreeList;
typedef UT_ValArray<op_LayoutBranch *>  op_BranchList;
 
//
// Note: 	A trunk tile is a tile with no input if the direction is Top
// 		to Bottom, else it is a tile with no output. A branch starts 
//		with a trunk tile. Thus, the number of branches
//		in a tree is the number of trunk tiles in that tree. Even 
// 		though there are cycles in the graph, we call it a tree.
// 		A tile which has multiple connectors (eg. Blend object) will 
//		be considered to be part of the branch that is the left most of
//		its input. The x and y location of a tile will be the 
//		position of the bottom left corner of the tile.
//

class OP_API OP_Layout
{
public:
    OP_Layout(OP_Network *net, enum OP_LayoutSort sorted = OP_LAYOUT_SORT_NONE);
   ~OP_Layout();

    // Add all nodes on the worksheet to the list of layout 
    // OPs using this function.
    void	 addLayoutItem(OP_NetworkBoxItem  *item, int selected = -1);
    void	 layoutOps(OP_LayoutDirection    direction, 
	    		   OP_NetworkBoxItem	*currentItem,
			   bool 		 currentTreeOnly     = false,
			   bool 		 selectedOnly        = false,
			   bool 		 stagger             = true,
			   fpreal   		 child_space_factor  = -1, 
			   fpreal   		 parent_space_factor = -1,
			   bool			 snapGrid            = false,
			   fpreal		 gridSizeX           = 0,
			   fpreal		 gridSizeY           = 0);

private:
    void		 initSpaceFactor(fpreal child_s_f, fpreal parent_s_f);
    void		 buildTrees();
    void		 buildTree(op_LayoutTile *current_tile, 
				   int 		    tree_number);

    void		 getTileConnectorList(op_LayoutTile *tile);

    // This is used to move the tile (and all its children) which has 
    // multiple inputs to the left most branch it belongs.
    void		 moveTilesFirstBranch(op_LayoutTree *tree, 
					      op_LayoutTile *tile);

    void		 moveTileFirstBranch(op_LayoutTree *tree, 
					     op_LayoutTile *tile,
					     int 	      index);

    void		 layoutForest();
    void		 layoutSingleTree(op_LayoutTree	*tree);

    // This is to layout the trees which has only one tile (group or op).
    void                 layoutOneTileTreeList(
				const op_TreeList &oneTileTreeList,
				op_TreeList &prev_tree_list);

    op_TileList		*layoutSingleBranch(op_LayoutBranch	*branch,
					    op_LayoutTile  	*listTile,
					    fpreal 		 xleft, 
					    fpreal 		 ybottom);

    void		 squeezeTree(op_LayoutTree *tree, 
				     op_TreeList &prev_tree_list);

    // Squeeze branches from one parent to the branches of another parent
    // that has some branches in common.
    void		 squeezeBranch(const op_BranchList &prev_branch_list,
					op_LayoutBranch *branch);

    fpreal		 squeezeSubBranch(op_TileList  *subBranch,
					  op_TileList  *prevSubBranch,
					  bool		extra_space = false);

    bool		 isOverlapped(op_LayoutTile 	*node,
				      fpreal      	 xleft,
				      fpreal     	 xright,
				      fpreal    	  	 ybottom,
				      fpreal       	 ytop);

    void		 postAdjustCorner(); 

    void		 avoidOverlap();

    void		 updateTiles(int treeNumber = 0);

    op_LayoutTile	*getLayoutTile(OP_NetworkBoxItem *item,
				       op_TileList *search_list = 0);

    op_LayoutTile  	*getConnectorTile(op_LayoutTile *tile, int index, 
				int want_output, op_TileList *search_list = 0);

    void		 getTrunkTileList(op_TileList *trunkTileList);

    // If the given tile is a trunk tile, it will add it to the list and
    // return true, otherwise false is returned.
    bool		 appendTrunkToList(op_TileList *trunk_tile_list,
					   op_LayoutTile *tile);

    // List of all the trees
    op_TreeList 	 myForest;

    op_TreeList 	*myMergeTreeList;

    op_TileList 	*myTempTileList;
    op_TileList 	 myTileList;
    op_TileList 	 mySelectedTileList;

    OP_NetworkBoxItem	*myCurrentItem;

    int			 myCurrentTreeNumber;

    // Factor is -1 if Direction is Top to Bot, else factor is +1. 
    int			 myFactor; 

    fpreal		 myNodeWidth;  		// Width  of an op node.
    fpreal		 myNodeHeight;		// Height of an op node.
    
    bool		 mySnapToGrid;
    fpreal		 myGridSizeX;
    fpreal		 myGridSizeY;

    enum OP_LayoutSort	 mySortedTiles;

    // The space factor is a fraction of the width and height of a tile.
    // It is used to determine space between parent and children and cousins.
    fpreal		 myChildSpaceFactor;	// Space between children.
    fpreal		 myParentSpaceFactor;   // Space between parent and
						// child and also between
						// one tile trees along the
						// parent direction.
    fpreal		 myCousinSpaceFactor1;  // Space between group of
						// multi-tile trees and the
						// group of one tile treees.
    fpreal		 myCousinSpaceFactor2;  // For space between trees
						// and space between parents.
    fpreal		 myCousinSpaceFactor3;  // Extra space for cousin
						// branches that contain more
						// than one tile.
    fpreal		 mySinglesFactor;	// Space between single tile
						// trees.

    bool		 myHitOldTree;
    bool		 myHitCurrentTree;
    
    OP_LayoutDirection   myLayoutDirection;
    bool 		 myStaggerSinglesFlag; // Stagger flag for one tile tree

    bool		 myLayoutThisTreeOnly;
    bool		 mySelectedOnly;

    int			 debugCode;
};
#endif
