/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ParmMicroNode.h (PRM Library, C++)
 *
 * COMMENTS:	DEP_MicroNode subclass representing a PRM_Parm component
 */

#ifndef __PRM_PARMMICRONODE_H_INCLUDED__
#define __PRM_PARMMICRONODE_H_INCLUDED__

#include "PRM_API.h"
#include <DEP/DEP_MicroNode.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>
#include <stddef.h>

class PRM_Parm;
class PRM_ParmMicroNode;


typedef UT_ValArray<PRM_ParmMicroNode *> PRM_ParmMicroNodeList;


class PRM_API PRM_ParmMicroNode : public DEP_MicroNode
{
public:
			    PRM_ParmMicroNode()
				: myParm(NULL)
				, mySubIndex(-1)
			    {
			    }

			    PRM_ParmMicroNode(PRM_Parm &parm, int sub_index)
			    {
				init(parm, sub_index);
			    }

    void		    init(PRM_Parm &parm, int sub_index);

    virtual void	    clearInputs();

    virtual const char *    className() const
				{ return "PRM_ParmMicroNode"; }

    virtual int64	    getMemoryUsage(bool inclusive) const;

    virtual void	    dump(
				std::ostream &os,
				const char *prefix = 0,
				int indent_level = 0) const;

    const PRM_Parm &	    ownerParm() const
				{ return *myParm; }
    PRM_Parm &		    ownerParm()
				{ return *myParm; }

    int			    parmIndex() const;
    int			    subIndex() const
				{ return mySubIndex; }

private:

    PRM_Parm *		    myParm;
    int			    mySubIndex;
};

#endif // __PRM_PARMMICRONODE_H_INCLUDED__
