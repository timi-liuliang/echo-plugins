/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_MDD.h (SOP Library, C++)
 *
 * COMMENTS:
 * This SOP accepts as input an MDD file specifying a sequence of point clouds
 * and a geometry with an equal number of points. At each time t, the SOP
 * assigns the position of each point to its position in the point cloud.
 */

#ifndef __SOP_MDD__
#define __SOP_MDD__

#include "SOP_API.h"
#include <sys/types.h>

#include <UT/UT_Vector3Array.h>

#include "SOP_Node.h"

class sopMDDFile;

class SOP_API SOP_MDD : public SOP_Node
{
public:
	     SOP_MDD(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_MDD();

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);

    virtual void                 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);

    virtual bool		 evalVariableValue(
					fpreal &v,
					int index,
					int thread);
    // create overload that delegates to super class to avoid shadow warnings
    virtual bool		 evalVariableValue(
					UT_String &v,
					int i,
					int thread)
				{
				    return SOP_Node::evalVariableValue(
							    v, i, thread);
				}

    static OP_Node		*myConstructor(OP_Network *net, 
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template	 	 myTemplateList[];
    static CH_LocalVariable	 myVariables[];

    enum CoordSysType {
	COORDSYS_LEFT,	// left-handed coordinate system (Houdini)
	COORDSYS_RIGHT	// right-handed coordinate system (Lightwave)
    };

protected:
    virtual OP_ERROR		 cookMySop(OP_Context &context);
    virtual int			 reloadNewFiles(fpreal now);

    /// Reload the MDD file forcibly
    static int			 reloadMDDStatic(void *op, int idx, 
					fpreal t, const PRM_Template *);
private:
    bool 			 evaluateMDD(fpreal t, UT_Vector3Array *&);

    void			 FNAME(UT_String &str, fpreal t)
					{ evalString(str, 0, 0, t);        }
    int				 INTERP(fpreal t)
					{ return evalInt("interp", 0, t);  }
    fpreal			 FRAME(fpreal t) 
					{ return evalFloat("frame", 0, t); }
    CoordSysType		 COORDSYS(fpreal t)
					{ return (CoordSysType)
						    evalInt("coordsys", 0, t); }
    bool			 SHIFT_START(fpreal t)
					{ return evalInt("shiftstart", 0, t); }

    time_t		 	 myLastModified;
    UT_String			 myLastFilename;
    sopMDDFile			*myFile;
};

#endif
