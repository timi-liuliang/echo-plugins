/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VIS_Mount.h ( GR Library, C++)
 *
 * COMMENTS:
 *      A VIS_Mount specifies the location, scope and other local settings
 *      pertinent to the installation location of a visualizer.
 */

#ifndef __VIS_Mount__
#define __VIS_Mount__

#include "VIS_API.h"
#include "VIS_Defines.h"

#include <PRM/PRM_ParmList.h>
#include <PRM/PRM_Template.h>

#include <UT/UT_Array.h>
#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONWriter.h>
#include <UT/UT_Options.h>

#include <sstream>


class VIS_Visualizer;
class VIS_Handle;
class VIS_Mount;
class VIS_Table;


#define VIS_TOKEN_SCOPE		"scope"
#define VIS_TOKEN_NAME		"name"
#define VIS_TOKEN_LABEL		"label"
#define VIS_TOKEN_ICON		"icon"
#define VIS_TOKEN_FLAGS		"flags"
#define VIS_TOKEN_TYPE		"type"
#define VIS_TOKEN_PARMS		"parameters"

typedef UT_Array<VIS_Mount *>	VIS_MountPtrArray;

// MicroNode for passing modifications to the visualizer's parmlist to
// the relevant mount table micronode.
class VIS_MountMicroNode : public DEP_MicroNode
{
public:
		 VIS_MountMicroNode(VIS_Mount *mnt) : myMount(mnt) {}

    void 	 getInputs(DEP_MicroNodeList &inputs) const;
    void 	 getOutputs(DEP_MicroNodeList &outputs) const;
    int 	 getIndex();

private:
    VIS_Mount 	*myMount;
};

class VIS_API VIS_Mount
{
public:
			 VIS_Mount(VIS_Visualizer *vis = NULL);
			~VIS_Mount();

    VIS_Visualizer	*getVisualizer();
    int			 getTypeId();
    PRM_ParmList 	*getParmList();
    VIS_Handle		 getHandle();

    int 		 getUniqueId() const;
    VIS_TableRef  	 getTableRef() const;
    int 		 getIndex() const;

    const char		*getName() const;
    const char		*getLabel() const;
    const char		*getIcon() const;

    // Scene Geometry Activation:
    bool 		 actsOnUnselected() const;
    bool 		 actsOnSelected() const;
    bool 		 actsOnGhosted() const;

    // Model Geometry Activation:
    bool 		 actsOnDisplayed() const;
    bool 		 actsOnCurrent() const;
    bool 		 actsOnTemplated() const;

    bool		 showInToolbar() const;

    VIS_VisualizerScope  getScope() const;

    void		 saveToOptions(UT_Options &options) const;
    bool	 	 save(std::ostream &os) const;

    void		 loadFromOptions(UT_Options &options);
    bool	 	 load(UT_IStream &is);


    uint32		 getFlags(uint32 mask) const;
    void		 setFlags(uint32 mask, bool onoff);

    uint32		 getVisualizerFlags(uint32 mask) const;
    void		 setVisualizerFlags(uint32 mask, bool onoff);

    DEP_MicroNode	*getMicroNode() { return &myMicroNode; }
    VIS_Table		*getTable() { return myTable; }

    exint		 getUpdateRequestTime() const;
    exint		 getRefineRequestTime() const;

    void 		 setActOnUnselected(bool active);
    void 		 setActOnSelected(bool active);
    void 		 setActOnGhosted(bool active);
    void 		 setActOnDisplayed(bool active);
    void 		 setActOnCurrent(bool active);
    void 		 setActOnTemplated(bool active);
    void		 setShowInToolbar(bool show);

    void		 setParmMicroNodeInputs(DEP_MicroNode *data,
        		                        DEP_MicroNode *active);

    void		 armActiveParmMicroNode(bool enabled);
    void		 armDataParmMicroNode(bool enabled);


    bool		 isActiveParmMicroNodeArmed();
    bool		 isDataParmMicroNodeArmed();

    void		 handleActiveParmChange();
    void		 handleDataParmChange();
    bool		 isActive(VIS_Context context);

    static int	 	 registerMount(VIS_Mount *mnt);

    // Lookup the mount from the handle in constant time.
    static VIS_Mount 	*lookupHandleMount(VIS_Handle vh);

private:
    friend class VIS_Table;
    friend class VIS_Manager;

			 VIS_Mount(VIS_Table *table, VIS_Visualizer *vis,
				 const char *name = NULL, int index = -1,
				 const char *label = NULL, const char *icon = NULL);

			 VIS_Mount(const VIS_Mount &src);

    void		 init(VIS_Visualizer *vis = NULL);
    VIS_Mount	 	*duplicate(bool deep_copy = false) const;

    void 		 setIndex(int index);
    void 		 setName(const char* name);
    void 		 setLabel(const char* label);
    void		 setIcon(const char* name);

    void 		 setScope(VIS_VisualizerScope scope);
    void 		 setVisualizer(VIS_Visualizer* vis);

    void		 refreshUpdateRequestTime();
    void		 refreshRefineRequestTime();

    void		 setTable(VIS_Table *mtable) { myTable = mtable; }

    bool		 bumpName(bool label_too = false);

    int			 getContextKey(VIS_Context context);
    bool		 activate(VIS_Context context, bool active);
    void		 resetActiveState();
    int		 	 getNumActiveContexts();
    int			 getNumContexts();

    UT_JSONWriter	*getJSONWriter();

    static
    VIS_MountPtrArray 	 theVisualizerMounts;




    VIS_Visualizer	*myVisualizer;	// the visualizer object

    uint32 		 myFlags;	// mount flags

    VIS_VisualizerScope	 myScope;	// visualizer scope

    VIS_Table		*myTable;	// table to which this mount belongs
    int			 myIndex;	// index in table
    int			 myUniqueId;	// global mount unique id

    UT_String		 myName;
    UT_String		 myLabel;
    UT_String		 myIcon;

    bool		 myAutoLabel;

    std::stringstream 	 myWriterSStream;
    UT_JSONWriter 	*myJSONWriter;

    VIS_MountMicroNode	 myMicroNode;	// dirtied to indicated change in
					// mount or visualizer

    /// Latest update or refine request time
    exint		 myUpdateRequestTime;
    exint		 myRefineRequestTime;

    UT_IntArray	 	 myIsActiveInContext;
    int			 myNumActiveContexts;

    struct ActiveParmMNode: public VIS_MicroNode
    {
	ActiveParmMNode(VIS_Mount *mount) { myMount = mount; }
	void becameDirty(DEP_MicroNode &src, const DEP_PropagateData &data)
	{
	    myMount->handleActiveParmChange();
	}

    private:
	VIS_Mount *myMount;
    };

    struct DataParmMNode: public VIS_MicroNode
    {
    	DataParmMNode(VIS_Mount *mount) { myMount = mount; }
    	void becameDirty(DEP_MicroNode &src, const DEP_PropagateData &data)
    	{
    	    myMount->handleDataParmChange();
    	}

        private:
    	VIS_Mount *myMount;
    };

    ActiveParmMNode	 myActiveParmMNode;
    DataParmMNode	 myDataParmMNode;
};






#endif
