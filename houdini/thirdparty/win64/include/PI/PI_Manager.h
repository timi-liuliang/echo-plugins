/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_Manager.h.h ( PI Library, C++)
 *
 * COMMENTS:
 *
 *	Manager to update/remove/add PIs and bindings.
 */

#ifndef __PI_Manager__
#define __PI_Manager__

#include "PI_API.h"
#include <UT/UT_Pair.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <OP/OP_Error.h>
#include <iosfwd>
#include "PI_OHLGroup.h"

typedef enum {
    PI_CHANGE_NAME,
    PI_CHANGE_ADD,
    PI_CHANGE_REMOVE,
    PI_CHANGE_CHANGE,
    PI_CHANGE_EXPOSED,
    PI_CHANGE_HIDDEN,
    PI_CHANGE_PANEMASK,
    PI_CHANGE_NETFLAG,
    PI_CHANGE_COLOR,
    PI_CHANGE_REORDER,
    PI_CHANGE_SETTINGS
} PI_ChangeType;

typedef void (*PI_PICallback)(PI_OHLPersistent *pi,
				    PI_ChangeType ctype,
				    void *data);
typedef void (*PI_GroupCallback)(PI_OHLGroup *group,
				    PI_ChangeType ctype,
				    void *data);

class OP_Node;
class PI_OpHandleLink;

PI_API PI_Manager *PIgetManager();

class PI_API PI_Manager
{
public:
    UT_String		 getFreePIName(const char *name) const;
    int			 addPI(const char *name, const PI_OpHandleLink *refpi);
    int			 addPI(const char *name, const char *pitype);
    void		 removePI(PI_OHLPersistent *pi);
    void		 removeAllPIs();
    int			 getPIIndex(const char *name) const;
    int			 getPIIndex(const PI_OpHandleLink &pi) const;
    int			 getNumPIs() const;
    PI_OHLPersistent	*getPI(int index) const;
    void		 swapPIs(int idx1, int idx2);

    UT_String		 getFreeGroupName(const char *name) const;
    int			 addGroup(const char *name);
    void		 removeGroup(PI_OHLGroup *group);
    int			 getGroupIndex(const char *name) const;
    int			 getNumGroups() const;
    PI_OHLGroup		*getGroup(int index) const;
    void		 swapGroups(int idx1, int idx2);

    void		 addPItoGroup(int pi, int group);
    void		 removePIfromGroup(int pi, int group);
    void		 clearGroup(int group);

    void		 addPICallback(PI_PICallback cb, void *data);
    void		 removePICallback(PI_PICallback cb, void *data);
    void		 piChanged(PI_OHLPersistent *pi,
				   PI_ChangeType ctype) const;
    void		 addGroupCallback(PI_GroupCallback cb, void *data);
    void		 removeGroupCallback(PI_GroupCallback cb, void *data);
    void		 groupChanged(PI_OHLGroup *group,
				      PI_ChangeType ctype) const;

    OP_ERROR		 savePersistentPIs(std::ostream &os, int i = -1) const;
    OP_ERROR		 savePersistentGroups(std::ostream &os, int i = -1) const;

    // This function is used when collapsing or extracting subnets.
    // It takes all references to the "from" ops and changes them to
    // refer to the corresponding "to" op.
    void		 getRefreshPICommands(std::ostream &commands,
					      UT_ValArray<OP_Node *> &fromOps,
					      UT_ValArray<OP_Node *> &toOps);
    void		 getRefreshPICommands(std::ostream &commands, OP_Node *op);

private:
	     PI_Manager();
    virtual ~PI_Manager();

    void		 installCommands() const;
    void		 deinstallCommands() const;
    void		 addPIsWithOp(OP_Node &node, UT_IntArray &pis) const;

    static OP_ERROR	 savePIManagerCallback(std::ostream &os, void *data);

    typedef UT_Pair<PI_PICallback, void *> PI_PICallbackInfo;
    typedef UT_Pair<PI_GroupCallback, void *> PI_GroupCallbackInfo;
    
    PI_OHLGroup				 myAllGroup;
    UT_ValArray<PI_OHLGroup *>		 myOHLGroups;
    UT_Array<PI_PICallbackInfo>	 myPICallbacks;
    UT_Array<PI_GroupCallbackInfo>	 myGroupCallbacks;

    friend PI_API PI_Manager *PIgetManager();
};

#endif

