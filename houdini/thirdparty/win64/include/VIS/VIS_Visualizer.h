/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VIS_Visualizer.h (VIS Library, C++)
 *
 * COMMENTS:
 *      A VIS_Visualizer is a custom display option which can be toggled on/off
 *      and contain any arbitrary settings.
 */

#ifndef __VIS_Visualizer__
#define __VIS_Visualizer__

#include "VIS_API.h"
#include "VIS_Defines.h"
#include "VIS_Handle.h"

#include <UT/UT_Lock.h>
#include <UT/UT_Options.h>
#include <UT/UT_String.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONWriter.h>

#include <CH/CH_EventManager.h>

#include <PRM/PRM_ParmList.h>
#include <PRM/PRM_Template.h>

#include <GA/GA_Types.h>


class VIS_Visualizer;
class VIS_Mount;
class VIS_Type;
class VIS_Primitive;
class VISF_Visualizer;
class GUI_DetailLook;
class GUI_GeoRender;
class UT_Ramp;

class VIS_ParmsMicroNode : public DEP_MicroNode
{
public:
    VIS_ParmsMicroNode(VIS_Visualizer *vis) : myVisualizer(vis) {}
    void getInputs(DEP_MicroNodeList &inputs) const;
    void getOutputs(DEP_MicroNodeList &inputs) const;

private:
    VIS_Visualizer *myVisualizer;
};

class VIS_API VIS_Visualizer
{
public:
			 VIS_Visualizer();
    virtual 		~VIS_Visualizer();

    PRM_ParmList	*getParmList();

    /// This virtual is called when a visualizer parameter is changed
    /// to allow changing the visibility or enabled state of related
    /// parameters.
    virtual bool	 updateParmsFlags() { return false; }

    virtual
    PRM_Template	*getTemplateList() const = 0;

    /// Helper methods to enable/disable or set visibility of parameters
    bool		 enableParm(const char *pn, int state, int v = -1);
    bool		 setVisibleState(const char *pn, bool state, int v = -1);

    /// Save and load method: these only write or read the visualizers
    /// parameters to or from the given stream.
    bool	 	 save(std::ostream &os);
    bool	 	 load(UT_IStream &is);

    int			 getTypeId() const { return myTypeId; }

    VIS_ParmsMicroNode	*getParmsMicroNode() { return &myParmsMicroNode; }

    /// Request for an update or refinement due to changes in visualizer
    /// parameters. These are to be used only from the visualizers cook
    /// method (cookMyVisualizer). They take effect if the visualizer is
    /// active in some context or else when the visualizer becomes first
    /// active in some context. Note that the flags NeedsRefine and
    /// NeedsUpdate ensure that a refinement or update is triggered for
    /// the visualizer if it has "missed" the latest occurrence of these
    /// events due to being inactive. However, the visualizer must explicitly
    /// request for updates or refinements after cooking its parameters or else
    /// its parameter list version is not bumped. This is to prevent
    /// automatic updates and refinements due to parameter change and to only
    /// trigger them for changes to parameters that truly affect the
    /// existing or created geometry.
    void		 requestRefinement();
    void		 requestUpdate();

    /// Request the named attribute be "filled" during the update
    /// of any GR_Primitive in a detail on which this visualizer acts.
    /// This is used, e.g., for preparing decoration rendering based
    /// on a particualr attribute.
    void		 requestAttribute(const char *attr,
        		                  GA_AttributeOwner owner =
        		                      GA_ATTRIB_INVALID);

    VIS_Mount		*getMount() const { return myMount; }

    /// Get or set one or more of the visualizer flags.
    uint32		 getFlags(uint32 mask) const;
    void		 setFlags(uint32 mask, bool onoff);

    // Helper callbacks for Attribute and Group Parameters
    static void		 buildGroups(void *data,
				PRM_Name *menuEntries,
				int thelistsize,
				const PRM_SpareData *spare,
				const PRM_Parm *parm);

    static void		 buildAttributes(void *data,
				PRM_Name *menuEntries,
				int thelistsize,
				const PRM_SpareData *spare,
				const PRM_Parm *parm);

    void		 updateRampFromMultiParm(fpreal t,
				const PRM_Parm &m,
				UT_Ramp &ramp,
				bool *time_dep = NULL);

    /// Returns the visualizer's construction serial number. This
    /// is copied from a global counter which is incremented each time
    /// a visualizer (of any type) is created. It is used, e.g., in
    /// VISF_Primtive to ensure that visualizer being referenced by the
    /// primitive is the same as the one which has created it.
    int			 getSerialNum() { return mySerialNum; }

    /// We may need to bump the serial number manually. One example is
    /// when the visualizer is moved from one table to another, particularly
    /// from one node's table to another.
    void		 bumpSerialNum();


    /// Set the values of common parameters to those of other_vis.
    virtual void	 copyParmValues(const VIS_Visualizer *other_vis);

    virtual VISF_Visualizer *castToVISF();

protected:
    /// Copy constructore
			 VIS_Visualizer(const VIS_Visualizer &src);

    /// cookMyVisualizer is called during prior to a viewport render
    /// if since the last "cook" the visualizer parameters have changed.
    /// The purpose of cookMyVisualizer method is to:
    ///     1. read the parameters and cache their values,
    ///     2. determine whether any of the changes in parameters warrant
    ///        requesting for an update or refinement and submit the request,
    ///     3. set visualizer flags according to its current state,
    ///     4. possibly anything else that involves changing the member
    ///        variables of the visualizer particularly since essentially
    ///        all other virtual methods on a visualizer, most importatly
    ///        createPrimitive and filterPrimitive are const due to
    ///        fact that at the state of the visualizer should not change
    ///        by or depend on its processing of any particular primitive.
    virtual void 	 cookMyVisualizer() = 0;

    PRM_ParmList	*myParmList;

private:
    // Type is set by the VIS_VisualizerType class which is
    // used to create a visualizer instances.
    friend class 	 VIS_Type;
    friend class	 VIS_Table;
    friend class 	 VIS_Mount;
    friend class	 VIS_Manager;
    friend class	 VISF_Dispatcher;
    friend class	 VISF_Setup;

    void		 setTypeId(int id) { myTypeId = id; }
    void		 setMount(VIS_Mount *mnt) { myMount = mnt; }

    bool		 refinementRequested(bool reset = true);
    bool		 updateRequested(bool reset = true);

    UT_StringArray	&getRequestedAttributes()
			    { return myRequestedAttributes; }

    /// Called whenever a parameter is changed
    void		 bumpVersion() { myParmsVersion++; }

    /// Call by DM_VPortAgent in each render cycle to
    bool		 setup();

    static void		 handleParmsChange(void *, CH_CHANGE_TYPE, int);

    static int		 theLastVisSerialNum;

    // data members

    int 		 myTypeId;
    uint32 		 myFlags;

    bool		 myRefinementRequested : 1;
    bool		 myUpdateRequested: 1;
    bool		 myClearRequestedAttribs : 1;
    UT_StringArray	 myRequestedAttributes;

    int			 mySerialNum;		// bumped upon creation
						// to notice mount type changes
    int			 myParmsVersion;	// bumped when parms change
    int			 myCookVersion;		// cathes up with myParmsVersion
						// after cooking

    UT_Lock		 myCookLock;		// must acquired before
						// cooking the visualiezer

    VIS_Mount		*myMount;
    VIS_ParmsMicroNode	 myParmsMicroNode;

};

class VIS_API VIS_CallbackData
{
public:
			 VIS_CallbackData() :
			     myVisUniqueId(-1),
			     myVisualizedNodes(NULL)
			 {}
    void 		 setVisUniqueId(int id)
			    { myVisUniqueId = id; }
    int			 getVisUniqueId()
			    { return myVisUniqueId; }
    void 		 setVisualizedNode(UT_IntArray const *nodes)
			    { myVisualizedNodes = nodes; }
    UT_IntArray	const	*getVisualizedNodes()
			    { return myVisualizedNodes; }
private:
    int			 myVisUniqueId;
    UT_IntArray	const 	*myVisualizedNodes;	// Used as hint of what nodes
						// are being visualized to help
						// populate attrib/group menu
};

#endif
