/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: Base class for SOPs that use the GDT data structures.
 *
 */

#ifndef __SOP_GDT_h__
#define __SOP_GDT_h__

#include "SOP_API.h"
#include <GDT/GDT_Detail.h>
#include "SOP_Node.h"
#include "SOP_UndoGDT.h"

class PRM_MenuEntry;

#define	SOP_GDT_GRP_IDX		0

class SOP_API SOP_GDT : public SOP_Node
{
friend class SOP_UndoGDT;
friend class SOP_UndoGDTOpDepend;
friend class MSS_EditState;

public:
	     SOP_GDT(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_GDT();
    
    const UT_String &		 getUndoLabel() { return myNewOpLabel; }

    /// This function allows an external caller to tell us to start a new
    /// operation.  For GDT SOPs, this involves applying the current operation
    /// and starting a new one.
    virtual void		 beginNewOperation();


    /// Clear the effect of the current and permanent deltas, restoring the
    /// original geometry.
    virtual void	 	 clearAllDeltas(fpreal t);

    GDT_Detail			*getPermanentDelta() { return myPermanentDelta; }

protected:
    virtual const char	*inputLabel(unsigned idx) const override;


    /// This determines whether the given undoblock name matches one of our
    /// undoblocks. If it matches, then our undo is collapsed with this block.
    /// Subclasses should override this to add additional undo names that they
    /// want matched. Remember to call the base class if you do this!
    virtual bool	matchesUndo(const char *undoname) const;

    //
    //  I/O methods      
    //                           
    virtual OP_ERROR	save(std::ostream &os, const OP_SaveFlags &flags,
				const char *pathPrefix,
				const UT_String &name_override = UT_String()
			) override;
    virtual bool	load(UT_IStream &is, const char *extension,
				const char *path=0) override;

    // Rebuild the group and/or the selection if necessary:
    OP_ERROR		 updateInputGroups(OP_Context &context,
				       bool changed_input, bool changed_group);
    // This will rebuild the selection if it is necessary to do so.
    // It should be called in code paths where we don't do cookInputGroups.
    void		 updateCookSelection(
				       bool changed_input, bool changed_group);


    /// Get the selection group string:
    virtual void	 getSelString(UT_String &str)
				{ evalString(str, SOP_GDT_GRP_IDX, 0, 0.0f); }

    // Returns true if the given input idx has been modified since the last
    // cook. Note that it also records information to do the checking.
    bool		 checkChangedSource(unsigned idx, OP_Context &ctx);

    /// Applies the inverse of myCurrentDelta to the gdp
    virtual void	 rollbackCurrentDelta();

    /// Applies myPermanentDelta to the gdp
    virtual void	 applyPermanentDelta();

    // Apply current delta to permanent delta
    static int		 applyStatic(void *, int idx,
				     fpreal t, const PRM_Template *);

    virtual void	 applyAndClearCurrentDelta();

    // Respond to button press to clear the effect of all the deltas:
    static int		 clearallStatic(void *, int idx,
					fpreal t, const PRM_Template *);

    static bool		 cloneAndCopyAttrib(const GU_Detail *src_gdp,
					    GU_Detail *dst_gdp,
					    GA_AttributeOwner owner,
					    const char *name);

    // Clear all the deltas and restore the original geometry:

    // Current and Cumulative ("Permanent") deltas;
    // the permanent one is saved to disk.
    GDT_Detail		*myPermanentDelta;
    GDT_Detail		*myCurrentDelta;

    bool		 myCookedFlag; // have we cooked yet?
    bool		 myNotifyCacheFlag;

    // Selection group
    const GA_Group	*myGroup;

private:
    // myNewOpLabel cannot be static or else we will have only one copy for
    // all derived classes!
    UT_String		 myNewOpLabel; // label for undos

    // Undo/Redo
    void		 undoDeltas(const GDT_Detail &geodelta,
				    SOP_UndoGDTType undotype);
    void		 redoDeltas(const GDT_Detail &geodelta,
				    SOP_UndoGDTType redotype);
};

#endif
