/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        OP3D_InputSelector.h
 *
 * COMMENTS:
 *	This descendant of BM_InputSelector knows about the viewer.  It uses
 *	the viewer to find out display/current sop and to use the viewer's
 *	selection type buttons.
 */

#ifndef __OP3D_InputSelector_h__
#define __OP3D_InputSelector_h__

#include "OP3D_API.h"
#include "OP3D_InputSelectorBase.h"
#include "OP3D_SelectionManager.h"
#include "OP3D_GUSelectionSet.h"
#include <UI/UI_HotkeyHelper.h>
#include <OP/OP_DataTypes.h>
#include <PI/PI_ClassManager.h>
#include <GEO/GEO_PrimTypeCompat.h>
#include <GU/GU_SelectionSet.h>
#include <GUI/GUI_GUSelectionProxyHandle.h>
#include <UT/UT_Rect.h>
#include <UT/UT_Map.h>

class GU_SelectResult;
class GUI_DisplayOption;
class OP_Network;
class OP3D_InputSelectorUndoWorker;
class OP3D_EdgeLoopHelper;
class SOP_Node;
class GA_Attribute;

typedef	bool (*OP3D_ValidForPickFilter)(void *data, GUI_DetailLook *look);

struct OP3D_SelectionInfo
{
    OP3D_SelectionInfo() : 
	myGeoIndex(-1), myOpId(-1), 
	myIsSelectionSetOwner(false),
	myIsSelected(false), myIsLocalScope(true) {}

    GU_SelectionHandle	 selection() const 
			 { return mySelectionSet->selection(myActiveType); }
    GA_GroupType	 selectionType() const 
			 { return myActiveType; }

    void		 updateProxy(GU_SelectionHandle sel, bool create);
    GUI_DetailLook	*getLook() const;


    UT_StringHolder		 myOpPath;
    int				 myOpId;
    // The index within the geolook.
    // TODO: The DOP data name is probably more useful.
    int				 myGeoIndex;
    
    OP3D_GUSelectionSetHandle	 mySelectionSet;
    GA_GroupType		 myActiveType;

    // The auto converted selection is set if we had to convert to an allowed
    // selection type in selectorFinished() and should be cleared whenever we
    // respond to a selection type change event.
    GU_SelectionHandle		 myAutoConvertedSelection;

    typedef OP3D_SelectionManager::ComponentScopeKey ScopeKey;
    ScopeKey			 myCacheScope;

    // Proxy which represents the binding between the GUI_DetailLook and
    // our selection.  Manages our reference to the GUI_DetailLook, and
    // the look's reference to the active selection.
    GUI_GUSelectionProxyHandle	 myProxy;

    bool			 myIsSelectionSetOwner;
    bool			 myIsSelected;
    bool			 myIsLocalScope;
};

class OP3D_API OP3D_InputSelector : public OP3D_InputSelectorBase
{
public:

	     OP3D_InputSelector(OP3D_View &viewer, PI_SelectorTemplate &templ);
    virtual ~OP3D_InputSelector();

    virtual const char	*className() const override;

    // This selector has the ability to locate components.
    virtual int		 hasLocates() const override
			 { return 1; }

    // The selector handles mouse events and modifies the selection
    // accordingly.  Return 1 if the event was consumed and 0 otherwise.
    virtual int		 handleMouseEvent(UI_Event *event) override;

    // Handle double-click events. Returns true if handled.
    virtual bool	 handleDoubleClickEvent(UI_Event *event) override;

    // Selectors respond to keyboard events to know when to finish a selection
    // stage and possibly enter the next.
    virtual int		 handleKeyEvent(UI_Event *event, 
               		                DM_Viewport &viewport) override;
    virtual bool	 handleTransitoryKey(const UI_Event &event) override;
    bool		 willHandleKeyEvent(UI_Event *event);

    // Is the selector required to attach an input node to the current
    // node?  If it is but nothing was selected, an empty model sop will
    // be attached.  By default, an input is not required.
    bool		 inputRequired() const { return myInputRequiredFlag; }
    void		 setInputRequired(bool onoff)
			    { myInputRequiredFlag = onoff; }

    // Some sops (eg. group and delete) need a '*' in the input field if
    // everything is selected.  Other sops will operate on everything if
    // the input field is blank.
    bool		 useAsteriskToSelectAll() const
			    { return myUseAsteriskToSelectAll; }
    void		 setUseAsteriskToSelectAll(bool onoff)
			    { myUseAsteriskToSelectAll = onoff;}

    // Specifies whether or not to use primitive id's when selecting edges.
    // This setting allows the selector to distinguish between a general edge
    // (e.g p0-1) and an edge on a primitive (e.g. 0e1)
    void		 setUsePrimsInEdgeSelections(bool use_prims)
				  { myUsePrimsInEdgeSelectionFlag = use_prims; }

    // Specifies whether the selector can guess and return edge rings based
    // on the choice of end elements or should always stick to edge loops.
    void		 setAllowEdgeRingSelection(bool allow)
    			    { myAllowEdgeRingSelection = allow; }

    // Specifies whether the selector can guess and return edge rings based
    // on the choice of end elements or should always stick to edge loops.
    void		 setOffsetVertexMarkersOverride(bool offset)
    			    { myOffsetVertexMarkersOverride = offset; }

    // Get or set the primitive mask used for picking. The virtual primMask
    // overrides DM_InputSelector which allows DM_Viewport to access the
    // primitive mask for pre-selection highlighting.
    virtual GEO_PrimTypeCompat::TypeMask primMask() const override
			    { return myPrimMask; }
    void		 setPrimMask(GEO_PrimTypeCompat::TypeMask primmask)
			    { myPrimMask = primmask; }

    // A selector can optionally be "sloppy" whereby the user can initially
    // pick any of the component types allowed by getSelectionAllowedTypes(),
    // regardless of the selection type UI value.  Once a component has been
    // picked in this mode, only components of that type can be picked until
    // the selections are cleared or a pick occurs as a GU_ReplaceSelect.
    //
    // The component type of area select operations when a component has not
    // yet been picked is determined by the selection type UI value.
    //
    // The selection type UI values can be used in this mode to convert the
    // current selection and are also automatically updated by the selector
    // as the user picks different component types.
    //
    // NB: setSloppy() should be called after setSelectionAllowedTypes().
    void		 setSloppyPick(bool on);
    bool		 getSloppyPick() const
			    { return mySloppyPick; }

    // Restrict sloppy picks to a subset of getSelectionAllowedTypes().  Must
    // be called after setSloppyPick(true) in order to have an effect.
    //
    // Note that this only restricts the subset of component types allowed for
    // sloppy picks and will not affect the current selection, even when it is
    // of a type we're excluding.
    bool		 restrictSloppyAllowedTypesToSubset(
				    const PI_GeometryTypeArray &types);

    // Some selectors allow picking components at the object level.
    void		 setPickAtObjLevel(bool flag)
			    { myPickAtObjLevelFlag = flag; }
    bool		 getPickAtObjLevel() const
			    { return myPickAtObjLevelFlag; }
    
    // The selector needs to be notified when it starts/stops selecting
    // so it can add/remove interests (the selection type, etc.). 
    virtual void	 startSelecting() override;
    virtual void	 stopSelecting() override;
    void		 restartWithStashedSelection();

    // Commit the pre-selection in the viewport to a geometry selection.
    virtual void	 commitViewportPreSelection(UI_Event *event,
				GU_SelectionRule sel_rule) override;

    // Return a list of looks from which we can select.
    virtual void	 getSelectableLooks(GUI_DetailList &look_list) override;

    // Add the cook selection, if any, from the specified SOP to our current
    // selection.
    bool		 addCookedSelection(SOP_Node *sop);

    // The selector needs to know the node for which it is reselecting so that
    // it can properly place visibility operators.
    void		 nodeIdForReselecting(int node_id)
			    { myNodeIdForReselecting = node_id; }

    // Clear the selections of the selection.  This method is called when,
    // for example, the selection stages are aborted.
    virtual void	 clearSelections();

    // Consume the user selections made for this selector by clearing any
    // correponding selections stashed in the selection manager/cache and
    // clear our selection infos.
    virtual void	 consumeSelections();

    // Remove the selections we assigned to detail looks.
    void		 removeSelectionsFromDetailLooks();

    // This method allows the resource manager can set the workbench to
    // something different if a selector is reused.  It should not be used
    // otherwise.  A nil pointer is allowed so the select can remove any
    // of its interests.  However, methods of the selector should never be
    // called when its workbench is nil.  The method is virtual so
    // descendants can also have a chance to clean up.
    virtual void	 setViewer(BM_View *viewer) override;

    // This method must be called after retrieving a new/recycled selector
    // from the resource manager.
    void		 setSelectionUIValues(UI_Value *sel_type,
					      UI_Value *sel_rule,
					      UI_Value *sel_style,
					      UI_Value *sel_visible,
					      UI_Value *sel_contained,
					      UI_Value *sel_full,
					      UI_Value *locate_always,
					      UI_Value *finished);

    // Any nodes created by this selector are tagged with the creator state's
    // name.
    void		 setCreatorStateName(const char *state_name)
			 { myCreatorStateName.harden(state_name); }
    const UT_String	&creatorStateName()	
			 { return myCreatorStateName; }

    // Can this selector use a box or lasso for picking, or just click picks?
    void		 setAllowDragSelect(bool allow_drag) 
			 { myAllowDragSelFlag = allow_drag; }

    // Should this selector only select the full geometry?
    void		 setSelectFullGeometry(bool select_full)
			 { myFullSelFlag = select_full; }
    
    // Should this selector start with the existing selection, if any?  The
    // cook group will be used for the chosen SOP if true and there is no
    // applicable pending temporary selection.  In some cases, you'll want
    // to control whether we use existing temp or cook selections separately.
    void		 setUseExistingSelection(bool use_existing)
			 {
			     setUseExistingTempSelection(use_existing);
			     setUseExistingCookSelection(use_existing);
			 }
    bool		 useExistingSelection() const 
			 {
			     return useExistingTempSelection() ||
				    useExistingCookSelection();
			 }

    // Should this selector start with the existing temporary selection,
    // if any?
    void		 setUseExistingTempSelection(bool use_existing)
			 { myUseExistingTempSelection = use_existing; }
    bool		 useExistingTempSelection() const 
			 { return myUseExistingTempSelection; }

    // Should this selector start with the cook selection for the chosen SOP
    // either if useExistTempSelection() is false or there was no applicable
    // pending temporary selection.
    void		 setUseExistingCookSelection(bool use_existing)
			 { myUseExistingCookSelection = use_existing; }
    bool		 useExistingCookSelection() const 
			 { return myUseExistingCookSelection; }

    // TODO: Rename to setExportUserSelectionOnFinish().
    void		 setStashSelectionOnFinish(bool stash)
			 { myStashSelectionOnFinish = stash; }
    bool		 getStashSelectionOnFinish() const 
			 { return myStashSelectionOnFinish; }

    void		 setAllowMultiPickLoopStart(bool allow)
    			 { myAllowMultiPickLoopStart = allow; }
    
    // Set an initial selection for the given op path.
    //
    // If useExistingTempSelection() or useExistingCookSelection() is true,
    // then this initial selection will be in addition to those selections,
    // and may even overwrite them entirely.  Any number of op paths can be
    // set. Only the op paths that exist within the selector context will
    // have their initial selection set. 
    void		 setInitialSelection(const char *op_path,
        		                     GA_GroupType type,
        		                     const char *selection_string);

    // Should this selector save undo information?
    void		 setSaveUndos(bool yesno) { mySaveUndosFlag = yesno; }
    bool		 saveUndos() const     { return mySaveUndosFlag; }

    // Generate merge(s) and connect them to the given op, filling in the
    // specified input parm with the selection.  If there is a menu in the
    // op that needs to be set and menu_idx is not null, the value menu_idx
    // points to will be set to a non-negative index into the menu.  If menu_idx
    // is not null but there is no menu, *menu_idx = -1.
    // If the selection consists of the entire gdp, the group field
    // is filled with an empty string.  To force it to be filled
    // with a string (even for the entire gdp), use force_selection_string.
    // An array of the previous input nodes may optionally be provided to
    // suppress the automatic repositioning of new_node when it is simply
    // reconnected to the same inputs.
    bool		 connectOps(OP_Node &new_node, int op_input_index,
				const UT_String &input_parm,
				int *menu_idx = 0,
				bool branch_off = false,
				bool multi_input = false,
				bool force_selection_string = false,
				bool force_numeric = false,
				const UT_IntArray *prev_input_nodes = 0);

    /// Some convenience methods for managing mySelectedInfoIndices.
    void		 addSelectedInfo(int info_i, bool set_pick_order=true);
    void		 removeSelectedInfo(int info_i);

    /// Update our internal selection cache and the detail look, if any, with
    /// the provided selection.
    int			 updateSelection(const char *path, OP_Node *node,
				      GUI_DetailLook *look, int geo_idx,
				      GU_SelectionHandle selection,
				      const GU_Detail &gdp);

    /// Returns the current set of ops that have selections on them using this
    /// selector. The size of the array matches that of the array returned by
    /// \c selectedselections and \c selectedAutoConvertedSelections. 
    UT_StringArray	 selectedOpPaths() const;
    
    /// Returns the current set of selection objects used to mark the 
    /// component selection for each op used for selection. The size of this
    /// array matches that of the array returned by \c selectedOpPaths.
    ///
    /// In many cases you should use \c selectedAutoConvertedSelections
    /// instead of this method.
    OP3D_SelectionHandles selectedSelections() const;

    /// Returns the current set of auto converted (to the best allowed
    /// selection type) selection objects for each op used for selection.
    /// The size of this array matches that of the array returned by
    /// \c selectedOpPaths.
    //
    /// The conversion is done in selectorFinished(), so if this method is
    /// called before this it is identical to calling selectedSelections().
    /// Typically you'll want to use this method wherever you call either
    /// getGeometryTypeAtFinish() or getGroupTypeMenuValAtFinish().  Unlike
    /// selectedSelections(), entries may be empty or even NULL.
    OP3D_SelectionHandles selectedAutoConvertedSelections() const;
    
    /// Returns a selection handle, given an op path. If the selector is not
    /// holding a matching op path, then an empty handle is returned.
    GU_SelectionHandle selectionFromOpPath(UT_StringRef path) const;

    /// Returns a selection handle, given an info key. If the selector is not
    /// holding a matching info, then an empty handle is returned.
    class InfoKey;
    GU_SelectionHandle selectionFromInfoKey(const InfoKey &key) const;

    // Returns whether this selector has a non-empty selection instance.
    bool		 hasNonEmptySelection() const;

    // Return the stored selection for the specified geometry, or, if none,
    // return the selection of the specified type passed in new_selection,
    // if any, or return an entirely new selection of the specified type
    // and set new_selection to point to it.
    GU_SelectionHandle	 getOrCreateSelection(int node_id, int detail_index,
					      int &info_i,
					      GU_SelectionHandle &new_selection,
					      GA_GroupType sel_type);
    GU_SelectionHandle	 getExistingSelection(int node_id, int detail_index,
					      int &info_i,
					      GA_GroupType sel_type);

    // The state needs to be able to get the current prompt and set the default
    // prompt.  The custom message is for the selector, and the default message
    // is displayed by the state.  The default prompt is passed on by the state
    // and should not be changed.  If you make the const versions public and
    // the non-const versions private it won't compile, so they're all public.
    const UT_String	&currentPrompt() const { return myCurrentPrompt; }
	  UT_String	&currentPrompt()       { return myCurrentPrompt; }
    const UT_String	&defaultPrompt() const { return myDefaultPrompt; }
	  UT_String	&defaultPrompt()       { return myDefaultPrompt; }

    // Clean up when selector is about to transmit done event. If drag is
    // true, the selector indicates to the state that the selection should
    // be dragged right away.
    void		 selectorFinished(bool drag = false);

    // Create an object merge for the given object-sop combination, or combine
    // the given path into the network if do_combine is true. In that case,
    // the sop path and selection instance is updated to point to the moved 
    // sop.
    static SOP_Node	*createObjectMerge(OP_Network *network,
				UT_String &path,
				GU_SelectionHandle selection,
				const UT_String &creator_state_name,
				bool keep_original_objects,
				bool display_origina_objects);

    // Override the undo worker that gets used for undos. Pass in NULL to use
    // a default worker.
    void		 setUndoWorker(OP3D_InputSelectorUndoWorker *worker=0);
    // Sets and returns a default undo worker.
    OP3D_InputSelectorUndoWorker *getDefaultUndoWorker();

    // Return the feel containing the RMB menu.
    virtual UI_Menu	*getSelectorMenu() const override;
    virtual UI_Menu	*getSelectorHotkeyMenu() const override;

    void		 setValidForPickFilter(void *data,
				OP3D_ValidForPickFilter filter);

    // Get or set the current selection type or selection rule explicitly.
    // Setting the selection type will change the selection type for all
    // displayed geometry and all geometry that has been selected.
    void		 setGeometryType(PI_GeometryType geo_type,
				bool update_buttons);
    GA_GroupType	 selectionType() const;

    // Get the component type as it was set when the selection was completed.
    PI_GeometryType	 getGeometryTypeAtFinish() const;
    int			 getGroupTypeMenuValAtFinish() const;
    bool		 getKeepOriginalObjects() const;

    // Query if the selector is currently updating the geometry selection
    // type button.  This can be useful in determining if the selector is
    // responsible for a given selection type change notification sent by
    // the workbench.
    bool		 currentlyUpdatingGeometryTypeButtons() const;

    const PI_GeometryTypeArray	&getSelectionAllowedTypes() const;
    void		 setSelectionAllowedTypes(
				const PI_GeometryTypeArray &allowedtypes);

    // Call this method to query the next best allowed selection type when the
    // given type is not allowed.  Returns PI_GEOTYPE_INVALID when we have no
    // explicit preference among the allowed selection types.
    PI_GeometryType	 getNextBestAllowedType(
				PI_GeometryType disallowed_type) const;

    // Call this method to query the allowed selection type to use for the
    // given type.  The failure_type is returned if there are no preferred
    // allowed selection types for the given type.
    PI_GeometryType	 mapToAllowedType(
				PI_GeometryType type,
				PI_GeometryType failure_type
						    = PI_GEOTYPE_INVALID) const;

    // This function uses the below static function to generate a single
    // selection string for all selections merged into a single detail.
    //
    // If consume_selections is true, the selections will be removed from
    // this selector and the viewer's selection cache.
    virtual void	 generateAllSelectionStrings(UT_String &sel_string,
				bool only_one_gdp,
				bool force_numeric,
				bool consume_selections) override;
    static void		 generateMergedSelectionString(UT_String &sel_string,
				UT_StringArray &paths,
				OP3D_SelectionHandles &selections,
				bool ordered,
				bool collapse,
				bool use_ast_to_select_all,
				bool force_numeric);

    // A utility method to get a pick record representing the current loop
    // start pick with the component indices mapped into a detail composed
    // by merging all the selected details.  Returns true on success, false
    // otherwise.  The record will be cleared on failure.  The look ID and
    // detail index returned in the GR_PickRecord are meaningless.  We are
    // unable to export the component info when myLoopStartPickPath is not
    // in the list of selected paths.
    bool		 getPostMergeLoopStart(GR_PickRecord &pick);

    // A method for bootstrapping the current loop start pick, typically with
    // a pick derived from an earlier selector.  Only the geometry component
    // data is relevant in the supplied GR_PickRecord.
    void		 bootstrapLoopStart(const char *path,
				const GR_PickRecord &pick,
				bool only_selected_path);

    // This method is used by HOM to replace the current selection for a
    // particular component type.
    void		 scriptReplaceSelection(
					PI_GeometryType geo_type,
					const UT_StringArray &paths,
					OP3D_SelectionHandles &selections);

    // Convert to a particular geometry selection from the given list of
    // objects
    static bool		 createSelectionFromObjects(
				const OP_NodeList &objects,
				DM_Viewport &viewport,
				PI_GeometryType geo_type,
				UT_StringArray &paths,
				OP3D_SelectionHandles &selections);

    virtual bool getStateParmNames(UT_StringArray &ret, const char* prefix=nullptr ) override;
    virtual bool evalStateParm(const char *name, UT_StringHolder &ret) override;
    virtual bool setStateParm(const char *name, const UT_StringHolder &val) override;
    virtual bool pressStateButton(const char *name) override;

    class OP3D_API InfoKey
    {
    public:
	InfoKey(int node_id, int detail_index)
	    : myNodeId(node_id), myDetailIndex(detail_index)
	{
	}
	bool operator==(const InfoKey &key) const
	{
	    return myNodeId == key.myNodeId &&
		    myDetailIndex == key.myDetailIndex;
	}
	int	 getNodeId() const { return myNodeId; }
	int	 getDetailIndex() const { return myDetailIndex; }
    
    private:
	int	 myNodeId;
	int	 myDetailIndex;
    };

    using LocateFilter = std::function<bool(DM_Viewport *)>;
    void		 setLocateFilter(LocateFilter filter)
    			    { myLocateFilter = filter; }

protected:
    // React to the user pressing a button to change the component type.
    virtual void	 handleSelectionTypeChange(UI_Event *event);

    /// Called when selection finishes and we need to stash away the group
    /// menu index that gets set for the target operator of this selector.
    virtual void	 updateGroupMenuValue(PI_GeometryType geo_type, 
				int &group_menu_type) = 0;
    
    // Called when the selection changes and nothing is removed.  Added_geo is
    // a pointer to the added geometry, allowing derived classes to process it 
    // if they so wish.
    virtual void	 addToSelection(UI_Event *event,
					const char *path,
					GU_SelectionHandle sel,
					GU_SelectResult &added_geo)
			    { }
	
    // Handle a pick during which nothing was selected.  Return true if 
    // any changes were made to the existing selections.
    virtual bool	 handleNoPick(UI_Event *event);

    // Insert additional nodes between the last created node and the
    // state's node.
    virtual void	 appendToLastNode(SOP_Node *&/*input_node*/,
				const UT_String &/*creator_state_name*/,
				bool /*branch_off*/) {}

    // Add additional input nodes for the new node.
    virtual void	generateAdditionalInputNodes(SOP_Node & /*new_node*/,
				SOP_Node * /*last_selected_node */,
				const UT_String & /*sel_string*/,
				const UT_String & /*creator_state_name*/) {}

    // Modify default parameters if necessary.
    virtual void	 setNodeParameters(OP_Node &) {}

    // Return the pick masks to be used for single or area picks.  These masks
    // are a function of the current selection type for all non-sloppy (normal)
    // selectors, and both the current selection type and the current selection
    // state for sloppy selectors.  These are the masks used to select entities
    // that the selection knows how to handle.
    int			 singlePickMask(GU_SelectionRule sel_rule) const;
    int			 areaPickMask(GU_SelectionRule sel_rule) const;

    // Return the pick mask to be set in the workbench to control the automatic
    // display of useful visual markers.
    int			 displayPickMask() const;

    // THESE TWO METHODS ARE DEPRECATED.
    // Overriding selection type in this manner is dangerous as there
    // is no guarantee the button bar will remain disabled.
    // If you find yourself wanting to use this (ie: MSS_JoinSelector)
    // your selector is likely a state in disguise.
    
    // This method is called to overwrite UI values.  Disables the
    // toolbox which allows the user to change the values.
    void		 overrideSelectionValues(int sel_type, int sel_rule,
						 int sel_style, int sel_full,
						 int locate);
    // Restore the values indicated by the UI.
    void		 restoreSelectionValues();

    // Save the contents of the selector so later changes can be undone/redone.
    virtual void	 saveForUndo() override;

    // Implements the getPickName function to return the string representation
    // of a pick id.
    virtual bool	 getPickName(const OP3D_PickId &pickid,
				     UT_String &name,
				     bool descriptivename) const override;

    // Override the base class needObjectPick() method to add support for our
    // getPickAtObjLevel() flag.
    virtual bool	 needObjectPick() const override;

    // Clear the selections of all the gdp's with selections in them.
    // This method is called when failing to select something at all.
    bool		 emptySelections(const char *path_to_skip = 0,
					 bool refresh = true);

    // Mark this selection as dirty so that the SOP node and the viewport
    // will be properly refreshed with the new selection state.
    void dirtySelection(SOP_Node *sop_node);
    void dirtySelection(const GUI_DetailLook &geo_look, int geo_idx);
    void dirtySelection(const OP3D_SelectionInfo &sel_info);
    
    

    // Clear mySelectionInfos as well as mySelectedInfoIndices.
    void		 clearSelectionInfos();

    // Clear mySelectedInfoIndices, but keep mySelectionInfos.
    void		 clearSelectedInfoIndices();

    // Utility method to compute the next unused pick order from our current
    // selections.
    int			 computeNextPickOrder(GA_GroupType sel_type) const;

    int			 findInfo(int node_id, int detail_index) const;
    int			 findInfo(const char *path) const;


private:
    void		 handleStartSelecting(UI_Event *event);

    // Rather than putting the undo code in this file, it's in a friend class.
    friend class	 OP3D_InputSelectorUndo;

    void		 getPickableGeometry(GUI_DetailList &geos,
					     bool objects_only) const;

    // Return true if the selection has changed and false otherwise.
    bool		 singleSelectGeometry(UI_Event *event,
				bool save_undo,
				bool selecting_loop,
				GU_SelectionRule sel_rule = GU_InvalidSelect);
    bool		 boxSelectGeometry(UI_Event *event,
				int xcenter, int ycenter,
				int xsize, int ysize);
    bool		 lassoSelectGeometry(UI_Event *event,
				int *lasso_points);
    bool		 brushSelectGeometry(UI_Event *event,
					     uint8 *enablemap,
					     int minx, int miny,
					     int maxx, int maxy,
					     bool visible_only,
					     bool contained_only);
    bool		 singleSelectObject(UI_Event *event,
				UT_Array<GR_PickRecord> &pick_records);

    // Use the contents of the pick buffer to modify the selections.  Return
    // true if the selection has changed and false otherwise.
    bool		 modifySelections(UI_Event *event,
				const UT_Array<GR_PickRecord> &pick_records,
				GU_SelectionRule sel_rule);

    /// Handles a toggle from the "Select Whole Geometry" menu option
    void		 fullSelectionChanged(UI_Event *);

    // Select everything that's visible in the viewport, select nothing, or
    // toggle the selections of everything in the viewport.
    void		 selectAllOrNoneOrToggle(OP3D_SelectionAction action,
						 DM_Viewport &viewport);

    // Revert the current selection to only the cook selection on the current
    // node.
    void		 revertToCookSelection(DM_Viewport &viewport);

    // Select the boundary of the current selection.  This means
    // everything which is selected and has a neighbour which is
    // unselected.
    void		 selectBoundary(DM_Viewport &viewport);
    void		 selectEdgeBoundary(DM_Viewport &viewport);
    void		 shrinkSelection(DM_Viewport &viewport);
    void		 growSelection(DM_Viewport &viewport);
    void		 selectPrimitivesWithVertexCount(
			    DM_Viewport &viewport,
			    const char *command_name,
			    GA_Size min_vtx, GA_Size max_vtx);


    // Select everything that's visible in the uv viewport, but only if the
    // face has a specific winding (orientation).
    void		 uvSelectAllByWinding(DM_Viewport &viewport,
				bool front_facing,
				bool back_facing);

    void		 convertSelection(DM_Viewport &viewport,
        		                  GA_GroupType target_type);
    
    bool		 handlePick(UI_Event *event,
        				const UT_Array<GR_PickRecord> &in_pick_records,
        				GU_SelectionRule sel_rule,
        				bool &added_something);

    virtual void	 handleBoxPick(UI_Event *event) override;
    virtual void	 activeBoxPick(UI_Event *event) override;
    virtual void	 handleLassoPick(UI_Event *event) override;
    virtual void	 activeLassoPick(UI_Event *event) override;
    virtual void	 handleBrushPick(UI_Event *event) override;
    virtual void	 activeBrushPick(UI_Event *event) override;
    virtual void	 handleLaserPick(UI_Event *event) override;
    virtual void	 activeLaserPick(UI_Event *event) override;

    // Respond to the user using the selection visibility menu in the toolbar.
    void		 handleSelectionHideOrExpose(UI_Event *event);

    // Should we allow picking from this GUI_DetailLook?  (eg. Templates
    // can't be picked from, and ordered selectors need to pick from the
    // merges, even if the inputs of the merges are displayed.)
    bool		 validForPickRender(GUI_DetailLook *detail,
				const GUI_DisplayOption &dopt) const;

    // A higher level method than validForPickRender(), used to consolidate
    // additional common restrictions.
    //
    // TODO: Merge with validForPickRender().
    bool		 allowPickOperation(GUI_DetailLook *detail,
				const GUI_DisplayOption &dopt) const;

    UT_Array<const OP3D_SelectionInfo *> getNonEmptySelectedInfos() const;
    
    // Is any of the geometry visible on the screen selected?
    bool		 isAnythingSelected();

    // Create and connect a visibility sop to modify the visibility of geometry
    // in the specified sop.  Returns the newly created sop, if any, and a flag
    // indicating whether the visibility sop was inserted above the specified
    // sop instead of below.
    SOP_Node		*createAndConnectVisibilitySop(SOP_Node *sop,
						       bool *inserted_above);

    // Set the parameters in the visibility sop to match the action being
    // performed.
    void		 setVisibilitySopParameters(SOP_Node &visibility_sop,
				bool exposing,
				bool applying_to_selection,
				bool cumulative,
				const UT_String &sel_string);

    OP3D_EdgeLoopHelper	&getEdgeLoopHelper();

    SOP_Node *getChosenSOP() const;
    
    SOP_Node *		 findChosenSop(bool &need_object_pick);
    void		 handleSwitchToChosenOp(UI_Event *event);

    void		 doneNormalSelection();

    bool		 areaSelectGeometry(UI_Event *e,
					    const UT_DimRect &area,
					    uint8 *mask,
					    unsigned pick_mask,
					    GEO_PrimTypeCompat::TypeMask prims,
					    bool visible_only,
					    bool contained,
					    bool add_to_existing_picks);
    
    // Add any pending (i.e. saved temporary selections) for visible (and
    // pickable) geometry in the viewer.  Returns whether any non-empty
    // selections were added.  We also set have_chosen_sop_temp_sel flag
    // to indicate if we found any non-null pending selection for the chosen
    // SOP, including an empty one.
    bool		 initWithPendingSelectionForVisibleOps(
					    SOP_Node *chosen_sop,
					    bool &have_chosen_sop_temp_sel);
    void		 stashPendingSelection();

    void		 autoConvertToFinishGeometryType();

    void		 addInitialSelections();
    
    void		 handleGeoChanged(UI_Event *event);

    void		 updateSelectMask();

    // Reset mySloppyPickMask from myAllowedTypes.
    void		 resetSloppyPickMask();

    // Update the set sloppy selection type, if any, to the specified type.
    void		 updateExistingSloppySelectionType(
				GA_GroupType sel_type);

    // Initialize an unset sloppy selection type from the given pick records
    // and selection rule.
    void		 initSloppySelectionType(
				const UT_Array<GR_PickRecord> &pick_records,
				GU_SelectionRule sel_rule);

    // Set the sloppy selection type to the specified type along with any
    // extra book-keeping.
    void		 setSloppySelectionType(
				GA_GroupType sel_type);

    static OP_Node *	 mergeViaCombine(fpreal t,
				OP_Network *network,
				UT_String &path,
				GU_SelectionHandle selection,
				const UT_String &creator_state_name);

    bool		 acceptKey(UI_Event *event);
    bool		 fullSelectionKey(UI_Event *event);
    bool		 keepOriginalObjKey(UI_Event *event);
    bool		 locateKey(UI_Event *event);
    bool		 selectAllKey(UI_Event *event);
    bool		 invertSelectionKey(UI_Event *event);
    bool		 selectNoneKey(UI_Event *event);
    bool		 revertToCookSelectionKey(UI_Event *event);
    bool		 selectBoundaryKey(UI_Event *event);
    bool		 selectEdgeBoundaryKey(UI_Event *event);
    bool		 shrinkSelectionKey(UI_Event *event);
    bool		 growSelectionKey(UI_Event *event);
    bool		 selectUVSelectAllFrontKey(UI_Event *event);
    bool		 selectUVSelectAllBackKey(UI_Event *event);
    bool		 visibleSelectToggleKey(UI_Event *event);
    bool		 containedSelectToggleKey(UI_Event *event);
    bool		 redoSelectionKey(UI_Event *event);
    bool		 selectNextGroupKey(UI_Event *event);
    bool		 selectPrevGroupKey(UI_Event *event);
    bool		 copyCurrentGroupNameKey(UI_Event *event);
    bool		 copyCurrentSelectionKey(UI_Event *event);

    bool		 selectionConvertPointKey(UI_Event *event);
    bool		 selectionConvertEdgeKey(UI_Event *event);
    bool		 selectionConvertPrimitiveKey(UI_Event *event);
    bool		 selectionConvertVertexKey(UI_Event *event);
    bool		 selectionConvertBreakpointKey(UI_Event *event);

    bool		 selectPrimitiveTrianglesKey(UI_Event *event);
    bool		 selectPrimitiveQuadsKey(UI_Event *event);
    bool		 selectPrimitiveNgonsKey(UI_Event *event);
    
    void		 handleLocatedPickComplete(UI_Event *event);
    void		 handleMouseActionComplete();

    // Utility method to update myLoopStartPick to refer to the detail look
    // corresponding to myLoopStartPickPath when necessary, and, optionally,
    // note the fact that we've done this in myLoopStartPickRecordMatchesPath
    // when we expect it to remain synchronized over an extended operation.
    void		 updateLoopStartPickRecordFromPath(
						    bool record_match_state);
    
    bool selectFromSingleRecord(UI_Event *event,
				const GR_PickRecord &pick,
				GU_SelectionRule sel_rule);

    void		 addInfoLookup(const OP3D_SelectionInfo &sel_info,
				       int sel_info_index);

    const GA_Attribute	*getUVAttrib(const GU_Detail *gdp);
    bool		 areVerticesOfSamePoint(
	    			const UT_Array<GR_PickRecord> &pick_records);

    void		 extendVertexPickRecords(
				const UT_Array<GR_PickRecord> &pick_records,
				UT_Array<GR_PickRecord> &extended_pick_records);

    // Hotkey methods
    static UI_HotkeyHelper::Entry   theHotkeyList[];

    UI_HotkeyHelper	 myHotkeyHelper;
    DM_Viewport		*myHotkeyViewport;

    UT_String		 myCurrentPrompt;
    UT_String		 myDefaultPrompt;

    UT_String		 myCreatorStateName;

    UI_Value		*myGeoChangedValue;
    UI_Value		*mySelectionTypeValue;	// prims, points, etc.
    UI_Value		*myFullSelection;	// select whole gdp
    UI_Value		*myAlwaysLocate;	// always do locating

    // A selector can optionally be "sloppy" as described in the comment for
    // setSloppyPick(), whereby the user can pick any of the component types
    // allowed by mySloppyPickMask (automatically built from myAllowedTypes).
    // Once a component is picked in this mode, mySloppySelectionType will be
    // set and only components of that type can be selected until selections
    // are cleared.
    unsigned		 mySloppyPickMask;
    GA_GroupType	 mySloppySelectionType;
    GA_GroupType	 mySloppyFallbackSelectionType;
    bool		 mySloppyPick;
    bool		 mySloppySelectionTypeIsSet;

    // When overriding the values indicated by the UI buttons for the
    // above, keep previous values so we can restore.
    bool		 myCustomSelValFlag;
    int			 mySavedSelType;
    int			 mySavedSelRule;
    int			 mySavedSelStyle;
    int			 mySavedFullSel;
    int			 mySavedAlwaysLocate;

    GEO_PrimTypeCompat::TypeMask myPrimMask;	// polygon, nurbs, etc.
    
    // NB: The relative order of the selection infos only matters when the
    //     individual selections have the same pick order set.
    UT_Array<OP3D_SelectionInfo> mySelectionInfos;
    UT_Map<InfoKey, int>	 mySelectionInfoLookup;

    UT_IntArray			 mySelectedInfoIndices;
    int				 myNextPickOrder;

    typedef OP3D_SelectionManager::ComponentScopeKey ScopeKey;
    ScopeKey			 myScope;
    
    int			 myLastMouseDown;	// needed for changed events
    int			 myLastMouseStartX;	//   "     "     "      "
    int			 myLastMouseStartY;	//   "     "     "      "

    int			 myNodeIdForReselecting;// reselecting for this node

    bool		 myUseExistingTempSelection;
    bool		 myUseExistingCookSelection;
    bool		 myStashSelectionOnFinish;
    bool		 myInputRequiredFlag;	// is an input op required?
    bool		 myAllowDragSelFlag;	// allow box/lasso selecting?
    bool		 myFullSelFlag;		// do only full selections?
    bool		 mySaveUndosFlag;	// save undo information?
    bool		 myUseAsteriskToSelectAll; // use '*' to select all?
    bool		 myUsePrimsInEdgeSelectionFlag; // use primitives when
						// selecting edges (e.g. 0e1)
    bool		 myPickAtObjLevelFlag;	// pick geo at OBJ level
    bool		 myAllowEdgeRingSelection;
    int		 	 myOffsetVertexMarkersOverride;
    int		 	 myOffsetVertexMarkersSaved;

    // Flag to track whether the auto converted selections stored in the
    // selection info have been set.
    bool		 myAutoConvertedSelectionsFlag;

    // After we finish selecting we must remember our type.
    PI_GeometryType	 myFinishGeometryType;
    int			 myFinishGroupTypeMenuVal;

    // Component type of current selection.
    PI_GeometryType	 myCurrentComponentType;

    // A flag to track whether this selector is currently updating the geometry
    // type buttons in setGeometryType().
    bool		 myUpdatingGeometryTypeButtons;

    bool		 myHadDoubleClick;
    
    struct InitialSelection
    {
	GA_GroupType type;
	int index;
	UT_StringHolder selection_string;
    };
    UT_StringMap<InitialSelection>	 myInitialSelections;

    OP3D_InputSelectorUndoWorker	*myUndoWorker;
    bool				 myOwnUndoWorker;
    PI_GeometryTypeArray		 myAllowedTypes;

    // Utility for edge loops.  The loop start pick is persistent across
    // multiple locate events, and so myLoopStartPickPath should be used
    // to identify the geometry to use with myLoopStartPick instead of
    // myLoopStartPick.getLookId() and myLoopStartPick.getDetailIndex().
    // To help avoid unnecessary lookups using the path, we track when
    // we've already updated the myLoopStartPick record to match the path
    // across extended operations in myLoopStartPickRecordMatchesPath.
    OP3D_EdgeLoopHelper			*myEdgeLoopHelper;
    UT_String				 myLoopStartPickPath;
    GR_PickRecord			 myLoopStartPick;
    GR_PickRecord			 myLoopPrevPick;
    bool				 myLoopStartPickOnlyLocated;
    bool				 myLoopStartPickRecordMatchesPath;
    OP3D_ValidForPickFilter		 myValidForPickFilter;
    void				*myValidForPickFilterData;

    LocateFilter	 myLocateFilter 			= nullptr;
    bool		 myAllowMultiPickLoopStart		= false;
};

#endif
