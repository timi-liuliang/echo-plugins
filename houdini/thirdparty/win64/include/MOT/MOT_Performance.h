/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	MOT_Performance.h ( MOT Library, C++)
 *
 * COMMENTS:
 *
 *		The performance monitor.
 */

#ifndef __MOT_Performance__
#define __MOT_Performance__

#include "MOT_API.h"

#include <UT/UT_Performance.h>

class OP_Node;
class UT_PerfMonStats;

class MOT_API MOT_Performance : public UT_Performance
{
public:
     MOT_Performance();
    virtual ~MOT_Performance();

    /// Return the icon name to use for the given stats object.
    /// Return NULL if there is no known icon to use for the object.
    /// Subclasses can override this method to set up their own mappings
    /// of stats objects to icons.
    virtual const char	    *getStatsIcon(const UT_PerfMonStats *stats) const;

    /// Returns true if the particular node passed is allowed to
    /// participate in recording of the statistics, false otherwise.
    /// This is currently used for disallowing recording of the contents
    /// of compiled nodes.
    virtual bool	     isRecordingAllowedFor(OP_Node* node) const;

protected:

    /// Return the current frame.
    virtual int		     getCurrentFrameSubclass_();

    /// Return a pointer to the node specified by the given id.
    virtual OP_Node	    *getNodeSubclass_(int node_id) const;

    /// Return a pointer to the node that is bound to the given SIM solver.
    virtual OP_Node	    *getNodeFromSolverSubclass_(
				const SIM_Solver *solver) const;

    /// Pass back an object path that can be tied to a UT_PerfMonStats object.
    /// The path is generated from the given node and event name.
    virtual void	     getObjectPathFromNodeSubclass_(
				OP_Node *node, const char *event_name,
				UT_WorkBuffer &object_path) const;

    /// Pass back an object path that can be tied to a UT_PerfMonStats object.
    /// The path is generated from the given solver, node and event name.
    virtual void	     getObjectPathFromSolverAndNodeSubclass_(
				const SIM_Solver *solver, OP_Node *node,
				const char *event_name, UT_WorkBuffer &object_path)
				const;

    /// Pass back extra info that can be tied to a UT_PerfMonStats object.
    /// The extra info is generated from the given node.
    virtual void	     getExtraInfoFromNodeSubclass_(
				OP_Node *node, UT_WorkBuffer &extra_info) const;

};

#endif

