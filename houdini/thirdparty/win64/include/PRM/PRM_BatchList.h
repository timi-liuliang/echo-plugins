/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_BatchList.h ( PRM Library, C++)
 *
 * COMMENTS:	Definition of a list of parameters for evaluation.  Instead of
 *		evaluating each parameter individually, the parameters are
 *		evaluated in batch mode.
 *		
 *		This class specifies the list of parameters to evaluate.
 *		Similar to the PRM_Template (which defines the parameters).
 */

#ifndef __PRM_BatchList__
#define __PRM_BatchList__

#include "PRM_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>
#include "PRM_BatchParm.h"

class PRM_Template;
class prm_BatchOpInfo;

class PRM_API PRM_BatchList {
public:
     PRM_BatchList();
    ~PRM_BatchList();

    PRM_BatchParm *addParm(int index, const char *name, PRM_BatchType type,
				int size, void *data=0);
    PRM_BatchParm *addParm(PRM_Template *tplate, void *data=0);
    PRM_BatchParm *addParm(const char *name, PRM_BatchType type,
				int size, void *data=0)
    {
	return addParm(myParms.entries(), name, type, size, data);
    }

    // Validate that there are parameter entries for each entry.
    bool		 validate() const;

    // Clear the evaluated flags on all of the evaluation parameters
    int			 entries() const	{ return myParms.entries(); }
    PRM_BatchParm	*operator()(int i)	{ return myParms(i); }

    // When an OP_Node evaluates a parameter list, it MUST call setEvaluator
    // prior to setting the evaluated flags on any parameters.  This is used to
    // keep track of op interests.
    // WARNING:  There is no stack, so if another OP_Node is called to
    //	evaluate batch parameters, then the op-id will be set incorrectly
    void		setEvaluator(int opid);

    // Get a list of all the OP_Nodes which are used for evaluating this list
    int			getEvaluatingOps(UT_IntArray &list);

    // When a parameter is evaluated, please set the OP_Node's parameter
    void		setEvaluated(int batch_idx, int parmlist_idx);

    // Prior to evaluating the list, clear the evaluated status of all the
    // parameters.
    void		clearEvaluated()
			{
			    clearOpList();
			    for (int i = myParms.entries(); i-- > 0; )
				myParms(i)->clearEvaluated();
			}
    void		clearValueSet()
		        {
			    for (int i = myParms.entries(); i-- > 0; )
				myParms(i)->clearValueSet();
			}

    void		dump();

private:
    void		clearOpList();

    UT_ValArray<PRM_BatchParm *>	myParms;
    UT_ValArray<prm_BatchOpInfo *>	myOpList;
    int					myEvaluatingOp;
};

#endif

