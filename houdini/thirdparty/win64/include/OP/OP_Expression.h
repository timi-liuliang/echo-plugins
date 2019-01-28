/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Expression.h ( OP Library, C++)
 *
 * COMMENTS:	This contains a base class for expressions which reference OPs.
 *		This class should be allocated by a function callback (i.e. see
 *		ev_SetFunctionInstanceAllocator in EXPR/EXPR.h) and can be used
 *		to quickly get the correct OP_Node given a path.
 */

#ifndef __OP_Expression__
#define __OP_Expression__

#include "OP_API.h"
#include "OP_Error.h"
#include <UT/UT_String.h>
#include <SYS/SYS_Types.h>
#include <stddef.h>

class OP_Node;
class PRM_Parm;
class PRM_ParmList;
class CH_Channel;

class OP_API OP_ExprFindOp {
public:
     OP_ExprFindOp();
    ~OP_ExprFindOp() {}

    // Count all memory owned by the OP_ExprFindOp.  Remember to
    // include sizeof(*this) if inclusive, else don't.
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myPath.getMemoryUsage(false);
        return mem;
    }

    // Find the node specified by "path" relative to the current location
    // specified by "cwd".  Returns null if there's no such op. If add_warnings
    // is true, then it will also call UTaddWarning() with something
    // appropriate when the return value is NULL.
    OP_Node *getNode(const char *path, OP_Node *cwd,
		     bool add_warnings);

    // The following functions can be used by the expression lib allocator
    // However, they may not be used if this class is sub-classed
    static void	*allocFindOp();
    static void	 freeFindOp(void *data);

private:
    int		 myOpId;
    int		 myCwdOpId;
    UT_String	 myPath;
};


class OP_API OP_ExprFindCh
{
public:
    OP_ExprFindCh();
    ~OP_ExprFindCh() {}

    // Count all memory owned by the OP_ExprFindCh.  Remember to
    // include sizeof(*this) if inclusive, else don't.
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myPath.getMemoryUsage(false);
        mem += myDestName.getMemoryUsage(false);
        return mem;
    }

    // If is_for_channel_name is false, look up the parm name (e.g. "t")
    // instead of the channel name (e.g. "tx").  When looking up a parm
    // name, the subindex result will always be -1.
    bool findParmRelativeTo(
	    OP_Node &relative_to_node,
	    const char *path,
	    fpreal now, OP_Node *&node,
	    int &parmindex, int &subindex,
	    bool is_for_channel_name);

    // Returns the node and/or parm that is currently evaluating.
    // parm_index might be NULL if the node is not currently cooking.
    void getEvaluatingSource(OP_Node *&node, int &parm_index, int &sub_index,
			     int thread);

    // If there is no parameter currently being evaluated, node will be null
    // and parm_index will be -1.
    void getEvaluatingParm(OP_Node *&node, int &parm_index, int &sub_index,
			   int thread);

private:
    int getDestIndex(
	    OP_Node *node, const char *channel_name, int &sub_index,
	    bool allow_alias);

    // myOpId is for the node that was found (i.e. the node being referenced).
    int		    myOpId;
    bool	    myIsForChannelName;
    int		    myRelativeToOpId;
    // myPath is the relative path from myRelativeToOpId's node to myOpId.
    UT_String	    myPath;

    int		    myParmIndex;
    int		    mySubIndex;
    PRM_ParmList   *myParmList;
    size_t	    myParmListCounter;

    // The dest parameter is the one being evaluated.  It's the one that will
    // get node interests.
    OP_Node *	    myDestNode;
    int		    myDestIndex;
    int		    myDestSubIndex;
    size_t	    myDestListCounter;
    UT_String	    myDestName;
};


// Get a parameter relative to the currently evaluating node/the pwd.
//
// This function has the important option of adding parm interests in
// the referenced parameter from the currently evaluating parameter.  It
// basically assumes that that referenced parameter will be evaluated from the
// currently evaluating parameter (e.g. like when we do a channel reference).
//
// This function will use OP_ExprFindCh to cache parameter lookups.  Houdini
// will allocate OP_ExprFindCh objects when some expression functions
// (ch(), chs(), etc.) are compiled into bytecode, so you don't often have
// to deal with OP_ExprFindCh manually.  Because HOM needs finer-grained
// control over when dependencies are added, though, it does deal with them
// manually.
OP_API extern void OPgetParameter(const char *path, fpreal &time,
				  int *retsubindex, PRM_Parm **retparm, 
				  CH_Channel **retchannel, OP_Node **retnode,
				  bool allow_alias,
				  bool add_parm_interests /*= true*/,
				  int thread, bool add_warnings = false);

// Possible error code values:
//  OP_WARN_BAD_CHREF, OP_WARN_BAD_OPREF
OP_API extern void OPaddBadRefWarning(OP_ErrorCodes err_code,
				      const char *path, int thread);

#endif
