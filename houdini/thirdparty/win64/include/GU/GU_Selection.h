/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Selection (C++)
 *
 * COMMENTS:
 *	Base class of a gdp selection. This clas hides the actual type
 *	of selection (point, edge, primitive, etc)
 *
 */

#ifndef __GU_Selection_h__
#define __GU_Selection_h__

#include "GU_API.h"
#include "GU_SelectType.h"
#include "GU_Detail.h"

#include <GA/GA_Breakpoint.h>
#include <GA/GA_Edge.h>
#include <GA/GA_GroupTypeTraits.h>
#include <GA/GA_Types.h>
#include <UT/UT_Map.h>

class GA_BreakpointGroup;
class GEO_Primitive;
class GU_Detail;
class GU_Selection;
class UT_JSONWriter;
class UT_JSONParser;

class GU_API GU_SelectResult
{
public:
    GU_SelectResult()
	: myPrimitive(0), 
	  myPoint(GA_INVALID_OFFSET),
	  myVertex(GA_INVALID_OFFSET),
	  myDetail(0),
	  myGroupType(GA_GROUP_INVALID),
	  mySelectedNormal(false)
    {}
    ~GU_SelectResult() {}

    void	setSelectedNormal(bool selected_normal)
		{ mySelectedNormal = selected_normal; }

    const GA_Breakpoint	&getBreakpoint() const
			 { return myBreakpoint; }
    const GA_Edge	&getEdge() const
			 { return myEdge; }
    const GA_Primitive	*getPrimitive() const
			 { return myPrimitive; }

    const GU_Detail	*getDetail() const
			 { return myDetail; }
    GA_Offset		 getPointOffset() const
			 { return myPoint; }

    GA_Offset		 getVertexOffset() const
			 { return myVertex; }

    bool		 isSet() const
			 { return myGroupType != GA_GROUP_INVALID; }
    GA_GroupType	 getSelectType() const
			 { return myGroupType; }
    bool		 getSelectedNormal() const
			 { return mySelectedNormal; }

    void		 clear()
			 {
			     myGroupType = GA_GROUP_INVALID;
			     mySelectedNormal = false;
			 }

protected:
    friend class GU_BreakpointSelection;
    friend class GU_EdgeSelection;
    friend class GU_PointSelection;
    friend class GU_PrimSelection;
    friend class GU_VertexSelection;
    friend class GU_CookSelection;
    
    void	set(const GA_Breakpoint &b, const GU_Detail *gdp)
		{
		    myBreakpoint = b;
		    myDetail = gdp;
		    myGroupType = GA_GROUP_BREAKPOINT;
		}
    void	set(const GA_Edge &e, const GA_Primitive *prim,
		    const GU_Detail *gdp)
		{
		    myEdge = e;
		    myPrimitive = prim;
		    myDetail = gdp;
		    myGroupType = GA_GROUP_EDGE;
		}
    void	set(const GA_Primitive *prim, const GU_Detail *gdp)
		{
		    myPrimitive = prim;
		    myDetail = gdp;
		    myGroupType = GA_GROUP_PRIMITIVE;
		}
    void	setPoint(GA_Offset ptoff, const GU_Detail *gdp)
		{
                    myPoint = ptoff;
                    myDetail = gdp;
		    myGroupType = GA_GROUP_POINT;
		}
    void	setVertex(GA_Offset vtxoff, const GU_Detail *gdp)
		{
		    myVertex = vtxoff;
                    myDetail = gdp;
		    myGroupType = GA_GROUP_VERTEX;
		}
    
private:
    GA_Breakpoint	 myBreakpoint;
    GA_Edge		 myEdge;
    const GA_Primitive	*myPrimitive;
    GA_Offset            myPoint;
    const GU_Detail     *myDetail;
    GA_Offset		 myVertex;
    GA_GroupType	 myGroupType;
    bool		 mySelectedNormal;
};

class GU_API GU_SelectFinishData
{
public:
		 GU_SelectFinishData()
		 { }
    virtual	~GU_SelectFinishData()
		 { }
};

class GU_API GU_Selection
{
public:
    virtual ~GU_Selection();

    // Return a new selection of the specified type.  The creator prefix and
    // the suffix are used in naming the primitive and point groups of the
    // selection.
    // FIXME: Rename to 'create'
    SYS_DEPRECATED_PUSH_DISABLE()
    static GU_SelectionHandle	 newSelection(GU_SelectionType stype);
    SYS_DEPRECATED_POP_DISABLE()
    static GU_SelectionHandle	 newSelection(GA_GroupType type);
    static GU_SelectionHandle	 newSelection(const GU_Detail &dst_gdp,
                             	              const GU_Detail &src_gdp,
                             	              const GU_Selection &src_sel);

    // NB: Intended for use by GU_Detail only.
    //
    // Return a handle to a new GU_CookSelection of the specified type in the
    // supplied detail.
    // NB: This selection must be orphaned if still around when this detail
    //     is deleted.
    static GU_SelectionHandle	 newCookSelection(GU_Detail &gdp,
						  GA_GroupType type,
						  bool ordered);

    // NB: Intended for use by GU_Detail only.
    //
    // Return a handle to a new GU_CookSelection that references the supplied
    // group instead of creating one itself.  No changes will be made to this
    // group by the selection.  If any changes are necessary to the selection,
    // it will create a copy of this group first.
    // NB: This selection must be orphaned if still around when this detail
    //     or group is deleted, so be very careful.
    static GU_SelectionHandle	 newCookSelection(GU_Detail &gdp,
						  GA_Group &group);

    // Copy the contents of one selection to another, doing any conversions
    // that are necessary. This assumes that they are from the same gdp.
    virtual void		assign(const GU_Detail &src_gd,
                		       const GU_Selection &src_sel) = 0;

    // Called by GR_PickSelection to modify this selection based on user
    // selected components in the viewport. The first edge to be added to us
    // (if any), is returned in 'added', and so is the pickid unless the
    // pointer to is is zero.
    virtual bool	 select(const GU_Detail &gd,
                	        uint id1, uint id2, uint id3,
				GU_SelectionRule rule,
				GU_SelectResult &added,
				GU_SelectFinishData *&finish_data) = 0;
    // After one or more calls to select(), GR_PickSelection will call
    // selectFinish to give the selection a change to apply any changes
    // it stored in the finish_data parameter.
    virtual bool	 selectFinish(const GU_Detail &gd,
                	              GU_SelectionRule rule,
                	              GU_SelectResult &added,
                	              GU_SelectFinishData *finish_data)
				     { UT_ASSERT(!finish_data); return 0; }

    // Determines if the supplied ids represent a valid component within
    // the existing selection, or that could be added to the selection.
    virtual bool	 testSelect(const GU_Detail &gd,
                	            uint id1, uint id2, uint id3,
                	            bool accept_existing,
                	            bool accept_new,
                	            GU_SelectResult &result) const = 0;

    // Find the actual point picked in the selection: this could be a GEO 
    // point, a point on a normal, curve, edge, etc. 'added' indicates
    // what has been picked. 'pickid' is the type of hit that generated the
    // pick, and 'xsect' is the picked point (to be found). The methods return
    // 1 if xsect can be determined and 0 otherwise. If the 'xsect' is not
    // found, the 2nd method computes the bounding box of 'added' and returns
    // that one instead.
    virtual bool	 selectionPoint(const GU_SelectResult &added,
                	                UT_Vector3 &xsect) const = 0;
    virtual bool	 selectionPoint(const GU_SelectResult &added,
                	                UT_Vector3 rayorig, 
                	                UT_Vector3 &raydir, 
                	                UT_Vector3 &xsect,
                	                bool &normal,
                	                UT_Vector3 &vector,
                	                bool accurate, 
                	                float *u, float *v) const = 0;

    // Select the contents of the group even if the group type does not
    // match our type. Return 1 if the selection has changed, 0 otherwise.
    // Subclasses should implement overrides that intelligently handle
    // groups that match our selection type.
    bool		 modifyGroup(const GU_Detail &gd,
				     const GA_Group &group,
        		             GU_SelectionRule rule);
    // Fairly direct access to the group token list. Use with caution or
    // you'll end up with a group token list that doesn't match your actual
    // selection.
    void		 modifyGroupToken(const char *token,
				GU_SelectionRule rule);
    // Initialize this selection to a combination of groups (named in the
    // group_string) which have already been combined into a group. This
    // is used when reselecting a SOP with a group parm containing only
    // group names.
    void		 initGroupString(const GU_Detail &gd,
        		                 const GA_Group &group,
        		                 const char *group_string);
    // Get or control the use of group tokens in this selection.
    bool		 getGroupTokenString(UT_WorkBuffer &buf) const;
    bool		 getGroupTokensValid() const
			 { return myGroupTokensValid; }
    // Force the selection to maintain its group token list and valid flag.
    // This should always be a temporary setting for the duration of one or
    // more selection changes that are known to preserve the group token
    // validity.
    void		 setGroupTokensLock(bool lock);

    // Modify a geometric entity on this selection. If the type being modified
    // does not correspond to the selection type, the selection will 
    // interpret the modification to the best of its abilities (e.g. removing
    // a point from a primitive selection will remove all primitives that share
    // that point; adding a point to a primitive selection, will select all 
    // primitives that shared that point; and so forth).
    virtual bool	modifyPoint(const GU_Detail &gd, GA_Offset ptoff, 
                	            GU_ModifyType type) = 0;
    virtual bool	modifyVertex(const GU_Detail &gd, GA_Offset vtxoff, 
                	             GU_ModifyType type) = 0;
    virtual bool	modifyPrimitive(const GU_Detail &gd, GA_Offset primoff, 
                	                GU_ModifyType type) = 0;
    
    virtual bool	modifyEdge(const GU_Detail &gd, const GA_Edge &edge, 
                	           const GEO_Primitive *prim, 
                	           GU_ModifyType type) = 0;
    virtual bool	modifyBreakpoint(const GU_Detail &gd, 
                	                 const GA_Breakpoint &bkp, 
                	                 GU_ModifyType type) = 0;
    
    /// Call this method once mass-modifications are finished with the above
    // modify* methods, but only if any of them returned \c true.
    void		modifyDone(const GU_Detail &gd);

    // Select all the points in the gdp. Return 1 if the selection has changed
    // and 0 otherwise.
    virtual bool	selectAll(const GU_Detail &gd) = 0;

    // Toggle the selection. Return true if something has been toggled and 0
    // otherwise.
    virtual bool	toggleAll(const GU_Detail &gd) = 0;

    /// This method is used to select the boundary. Not valid for all selection
    /// types.
    virtual bool selectBoundary(const GU_Detail &gd, bool checkuv, const UT_StringHolder &uvattribname = "uv"_UTsh) = 0;
    virtual bool growSelection(const GU_Detail &gd, bool checkuv, const UT_StringHolder &uvattribname = "uv"_UTsh) = 0;
    virtual bool shrinkSelection(const GU_Detail &gd, bool checkuv, const UT_StringHolder &uvattribname = "uv"_UTsh) = 0;
    
    // Filter the selection based on primitive coverage. 
    virtual bool	 filterByPrimitiveMask(const GU_Detail &gd, 
					       const GA_PrimitiveTypeMask &mask) = 0;

    // The following methods are used for selected front/back facing faces
    // (in the UV sense).
    bool	 uvSelectAllFrontFace(const GU_Detail &gd, const UT_StringHolder &uvattribname = "uv"_UTsh)
	    { return uvSelectAllByWindingUsingClosure(gd, true, false, uvattribname); }
    bool	 uvSelectAllBackFace(const GU_Detail &gd, const UT_StringHolder &uvattribname = "uv"_UTsh)
	    { return uvSelectAllByWindingUsingClosure(gd, false, true, uvattribname); }

    // Return a group for the current selection. 
    virtual const GA_PointGroup		*points(const GU_Detail &gd) const = 0;
    virtual const GA_PrimitiveGroup	*primitives(const GU_Detail &gd) const = 0;
    virtual const GA_VertexGroup	*vertices(const GU_Detail &gd) const = 0;
    virtual const GA_EdgeGroup		*edges(const GU_Detail &gd) const = 0;
    virtual const GA_BreakpointGroup 	*breakpoints(const GU_Detail &gd) const = 0;
    
    /// These functions provide a fast path to check for existence. They only
    /// work for the selection type their name indicates they should work for.
    /// No translation is done for other types and those will return \c false.
    virtual bool hasPointIndex(GA_Index index) const { return false; } 
    virtual bool hasPrimitiveIndex(GA_Index index) const { return false; } 
    virtual bool hasVertexIndex(GA_Index index) const { return false; } 
    virtual bool hasEdgeIndexPair(GA_Index p0, GA_Index p1) const 
	    { return false; }
    virtual bool hasBreakpointIndexSet(GA_Index prim_index, int u_index, 
				       int v_index = -1) const { return false; } 
    
    // Returns the group containing the selection.
    virtual const GA_Group	*mainGroup(const GU_Detail &gd) const = 0;
    
    /// Returns a group for this selection, of the given group type. If the
    /// selection is not of that type, then the selection will return a 
    /// best-case conversion of the selection from the original type.
    const GA_Group 		*group(const GU_Detail &gd, 
                   		       GA_GroupType type);
    
    // Just clear the bits of the groups, or destroy the internal groups and
    // arrays too.
    virtual void	 clear();
    virtual void	 clearAndDestroy();

    // How many entries does the selection contain?
    virtual GA_Size	 entries()   const = 0;

    // Compute the bounding box of the selection, and return \c false if the box 
    // is empty and \c true otherwise.
    virtual bool 	 getBoundingBox(const GU_Detail &gd,
                 	                UT_BoundingBox &bbox) const = 0;
    virtual bool	 getBoundingBox(const GU_Detail &gd,
                	                UT_BoundingBox &bbox,
                	                const UT_Matrix4R &transform) const = 0;

    // Compute the UV bounding box of the selection, and return \c false if the
    // box is empty.
    virtual bool	 getBoundingBoxUV(const GU_Detail &gd,
                	                  UT_BoundingBox &bbox,
                	                  const char *name,
                	                  int isvertex) const = 0;

    /// Save/load the contents of the selection
    bool save(UT_JSONWriter &w) const;
    bool load(UT_JSONParser &p, bool restore_id_and_rev = true);
    
    // Human-readable output of the selection for debugging purposes
    void		 	dump(std::ostream &os) const;

    // Query the type of selection:
    virtual GA_GroupType	 classType() const = 0;

    // Creates a duplicate selection identical to this one in the passed in
    // gdp.
    virtual GU_Selection	*clone() = 0;

    // Generate a string to represent the selection. This string is formatted
    // for use in SOP Group parameters. If the selection was built using
    // group or attribute tokens, those tokesn will be returned unless the
    // force_numeric field is set to true.
    bool			 generateSelectionString(UT_String &sel_string,
					const GU_Detail &gdp,
					bool ordered,
					bool collapse_where_possible,
					bool use_ast_to_select_all,
					bool force_numeric,
					GA_Index prim_offset = 0,
					GA_Index point_offset = 0) const;

    // Flags that control edge selections and whether or not they should include
    // primitive ids.  These are only handled by Edge selectors.
    virtual void		 setUsePrimEdges(bool /*use_prim_edges*/) { }
    virtual bool		 getUsePrimEdges() const { return false; }

    void setPickPath(const UT_StringHolder &p) { myPickPath = p; }
    UT_StringHolder pickPath() const { return myPickPath; }
    
    void setPickOrder(int pick_order) { myPickOrder = pick_order; }
    int pickOrder() const { return myPickOrder;  }
    void resetPickOrder() { myPickOrder = -1;  }

    /// Selection id and revision
    int				 getId() const { return myId; }
    int				 getRevision() const { return myRevision; }
    
    /// Return an approximation of how much memory we use
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
	mem += myGroupTokens.getMemoryUsage(true);
        return mem;
    }
    
protected:
    /// Validate the current selection against the given detail. This will
    /// prune out any selection components that are invalid against the detail
    /// given. Call this prior to, or after, a mass modification via calls to
    /// modify*(). Those calls do not do any verification of the current
    /// selection, for performance reasons. Returns \c true if the selection
    /// was already valid and no entries were eliminated.
    virtual bool	validate(const GU_Detail &gd) = 0;

    /// Convenience method to make/clear ordered status of a group
    static inline void	setOrdered(GA_ElementGroup &g, bool ordered)
			{
			    if (ordered)
				g.makeOrdered();
			    else
				g.clearOrdered();
			}

    GU_Selection(const GU_Selection &s);

    virtual bool	 uvSelectAllByWindingUsingClosure(
				const GU_Detail &gd,
				bool front_facing,
				bool back_facing,
                                const UT_StringHolder &uvattribname);

    virtual bool	 modifyMatchingGroup(const GU_Detail &gd,
                	                     const GA_Group &group,
                	                     GU_SelectionRule rule) = 0;
    
    static const GA_PrimitiveGroup *hiddenPrimitives(const GU_Detail &gd); 
    
    void		 updateRevision()
			 {
			     myRevision++;
			     if (!myGroupTokensLock && myGroupTokensValid)
			     {
				 myGroupTokensValid = false;
				 myGroupTokens.clear();
			     }
			 }

protected:
    // Class constructor and destructor. The prefix is used for naming the 
    // two internal groups, myPoints, myPrims.
    GU_Selection();
    
    /// @}
    /// @{
    /// Save/load class specific data elements.
    virtual bool	saveElements(UT_JSONWriter &w) const = 0;
    virtual bool	loadElements(UT_JSONParser &p) = 0;
    /// @}
    
    template<typename T>
    inline T &getGroup(const GU_Detail &gd, bool &needs_update) const
    {
	return static_cast<T &>(getGroup(gd,
	                                 GA_GroupTypeTraits<T>::groupType(),
	                                 needs_update));
    }

    UT_Lock &updateLock() const { return myUpdateLock; }

    static GU_Selection		*allocSelection(GA_GroupType type);

private:
    GA_Group &getGroup(const GU_Detail &gd,
                       GA_GroupType type, bool &needs_update) const;

    UT_StringArray		 myGroupTokens;
    bool			 myGroupTokensValid;
    bool			 myGroupTokensLock;

    UT_StringHolder		 myPickPath;
    int				 myPickOrder;
    
    int			 	 myId;
    int				 myRevision;
    
    struct GroupCacheEntry
    {
	GroupCacheEntry() : myDetailId(-1), myRevision(-1), myElementCount(-1) {}
	exint		 myDetailId;
	int		 myRevision;
	GA_Size		 myElementCount;
	UT_SharedPtr<GA_Group> myGroup;
    };
    typedef UT_Map<GA_GroupType, GroupCacheEntry> GroupCache;
    mutable GroupCache	myGroupCache;

    mutable UT_Lock	myUpdateLock;
    static SYS_AtomicInt32	 theUniqueSelectionId;
};

#endif
