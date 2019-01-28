/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_StyleManager.h ( OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_StyleManager__
#define __OP_StyleManager__

#include "OP_API.h"
#include "OP_Output.h"
#include "OP_Value.h"
#include <DEP/DEP_MicroNode.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_UniquePtr.h>
#include <iosfwd>

class UT_IStream;
class OP_Director;

#define INVALID_STYLE_VERSION	(-1)

class OP_API OP_StyleManager
{
public:
				 OP_StyleManager(OP_Director *dir);
				~OP_StyleManager();

    /// For saving to and loading from a hip file.
    bool			 save(std::ostream &os, int binary) const;
    bool			 load(UT_IStream &is);

    /// Creation and deletion of styles.
    bool			 addStyle(const UT_StringHolder &name,
					const UT_StringHolder &description,
					const UT_StringHolder &style);
    bool			 renameStyle(const UT_StringHolder &old_name,
					const UT_StringHolder &new_name);
    bool			 reorderStyles(const UT_StringArray &names);
    void			 removeStyle(const UT_StringRef &name);
    void			 removeAll();

    /// Fetching information about existing styles.
    bool			 hasStyle(const UT_StringRef &name) const;
    const UT_StringArray	&getStyleNames() const;
    const UT_StringHolder	&getStyleDescription(
				    const UT_StringRef &name) const;
    const UT_StringHolder	&getStyleSheet(
				    const UT_StringRef &name) const;

    /// A simpe way to check for changes in a specific style without using
    /// micro-nodes for dependency tracking. Good for maintaining a cache.
    /// Version numbers will be unique for all style sheets within this
    /// Houdini session.
    int				 getStyleVersion(
				    const UT_StringRef &name) const;

    /// Get the micronode for tracking the set of available styles.
    /// Propagates dirtiness when a style is added or removed.
    DEP_MicroNode		&getMicroNode();
    /// Get the micronode for tracking a articular style.
    /// Propagates dirtiness when the style is changed or removed.
    DEP_MicroNode		*getMicroNode(const UT_StringRef &name);

    /// Get a reference to the internal style sheet map.
    const UT_StringMap<UT_StringHolder> &
				 getStyleSheetMap() const
				 { return myStyleSheets; }

    /// Update references to moved SHOPs (called from
    /// OP_Node::moveAndUpdateDependencies).
    void			 handleOpMoved(const OP_NodeList &src_nodes,
					const OP_NodeList &dest_nodes);

private:
    static void			 handleOpChanged(OP_Node *node,
					OP_EventType reason,
					void *data,
					void *cbdata);
    void			 handleOpNameChanged(
					const UT_StringArray &old_names,
					const UT_StringArray &new_names);

    UT_StringArray				 myStyleNames;
    UT_StringMap<UT_StringHolder>		 myStyleSheets;
    UT_StringMap<UT_StringHolder>		 myStyleDescriptions;
    UT_StringMap<int>				 myStyleVersions;
    UT_StringMap<UT_UniquePtr<DEP_MicroNode> >	 myStyleMicroNodes;
    DEP_MicroNode				 myStyleListMicroNode;
    OP_Director					*myDirector;
};

#endif
