/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_CommandManager.h ( OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_CommandManager__
#define __OP_CommandManager__

#include "OP_API.h"
#include <CMD/CMD_Manager.h>

class UT_WorkBuffer;
class OP_Node;
class PRM_Parm;
class CH_Channel;
class CH_ChannelRef;


class OP_API OP_CommandManager: public CMD_Manager
{
public:
	     OP_CommandManager(const char *appname);
    virtual ~OP_CommandManager();


    /// The following methods are used to extract information from channel
    /// references.
    virtual void	 getChRefData(const CH_ChannelRef &r, 
				      OP_Node **pnode, CH_Channel **pchp, 
				      PRM_Parm **ppparm, int *subindex) const;

    virtual OP_Node	*getChRefOpNode(const CH_ChannelRef &r) const;

    virtual int		 getChRefParmIndex(
			    const CH_ChannelRef &r,
			    int *subindex) const;
    virtual PRM_Parm	*getChRefParm(
			    const CH_ChannelRef &r,
			    int *subindex) const;

    virtual PRM_Parm	*getChRefFullParm(const CH_ChannelRef &r) const;

    virtual CH_Channel	*getChRefChannel(const CH_ChannelRef &r) const;

    virtual bool	 isOwnerExposed(const CH_ChannelRef &r) const;

    virtual void	 getChannelsWithNodeId(CH_ChannelList &chans, int node_id) const;

    /// This method will return true if the channel reference r matches the
    /// pattern patt, or false otherwise.
    bool		 match(const CH_ChannelRef &r, const char *patt) const;


    /// The following methods are used to build channel references using,
    /// nodes, parms or channels.  The passed in channel reference r will
    /// be filled in with the appropriate data.
    /// Return true if success, false otherwise.
    virtual bool	 buildChanRef(CH_ChannelRef &r,
					       const CH_Channel *chp) const;

    virtual bool	 buildChanRef(CH_ChannelRef &r, 
					       const OP_Node *node,
					       const PRM_Parm *parm) const;

    virtual bool	 buildChanRef(CH_ChannelRef &r, 
					       const OP_Node &node,
					       const char *channel_name) const;

    virtual bool	 buildChanRef(CH_ChannelRef &r, 
					       const OP_Node *node, 
					       const PRM_Parm *parm, 
					       int subindex) const;
    virtual bool	 buildChanRef(CH_ChannelRef &r,
					       const char *path) const;


    /// Returns 0 if r1 and r2 are equal, -1 if r1 < r2 and 1 if r1 > r2
    virtual int		 compareChanRefs(const CH_ChannelRef &r1,
					 const CH_ChannelRef &r2) const;

    /// Returns 0 if r1 and r2 are equal, -1 if r1 < r2 and 1 if r1 > r2
    virtual int		 compareLayerRefs(const CH_ChannelRef &r1,
					 const CH_ChannelRef &r2) const;

    // Returns a full path from a given channel reference
    virtual void	 getChanRefPath(const CH_ChannelRef &r,
						 UT_String &path,
						 bool use_alias = false) const;

    /// Returns Python code representing the list of channel references
    void		 getChanRefsPythonListCode(
					const CH_ChannelRefList &chanrefs,
					UT_WorkBuffer &result);

    /// Returns a list of unique nodes corresponding to the given channel ref
    /// list. The order of the nodes will match chanrefs.
    static void		 getChRefUniqueNodes(const CH_ChannelRefList &chanrefs,
					     UT_ValArray<OP_Node *> &nodes);

    // Query trust using the current operational context.
    bool		 inTrustedContextForNode(const OP_Node *node, int thread) const;
    bool		 inTrustedContextForNodeContents(
					 const OP_Node *node, int thread) const;

    virtual CH_ScriptAccessManager *getAccessManager(int thread);

    virtual bool	 resolveChanRefExport( const CH_ChannelRef& r, CH_ChannelRef& out_export );

    virtual bool	 getChannelAutoSelect(const CH_ChannelRef &chref) const;

    // Resolve channel labels and node path without exposing OP_Node*
    virtual bool getChRefLabelForUI(const CH_ChannelRef &r, UT_String &ret) const;
    virtual bool getChRefAlias(const CH_ChannelRef &r, UT_String &ret) const;
    virtual bool getChRefNodePath(const CH_ChannelRef &r, UT_String &ret) const;
};

#endif
