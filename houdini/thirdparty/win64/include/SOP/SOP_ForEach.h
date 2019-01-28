/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_ForEach.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_ForEach_h__
#define __SOP_ForEach_h__

#include "SOP_API.h"
#include "SOP_Node.h"

class UT_InfoTree;

class SOP_API SOP_ForEach : public SOP_Node
{
public:
	     SOP_ForEach(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_ForEach();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);

    // Fill in tree with this node's details
    virtual void		 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);

    // Special methods used to get the feedback geometry from Each
    // SOPs.
    virtual bool		 hasFeedbackGeo() const;
    virtual GU_DetailHandle	 getFeedbackGeo() const;

    // Registers all each sops that want to be told if the feedback
    // geo changes.
    virtual void		 registerEachSOP(SOP_Node *sop);
    virtual void		 unregisterEachSOP(SOP_Node *sop);

    // Not currently supported even thought is technically a subnetwork.
    virtual bool		 canCreateNewOpType() const { return false; }

    static const char	*theChildTableName;

protected:
    virtual void	 childDeleted(OP_Node *);
    virtual void	 childFlagChange(OP_Node *);

    virtual OP_ERROR	 cookMySop(OP_Context &context);

    virtual const char	*inputLabel(unsigned idx) const;

    void		 buildStampListFromGroup(fpreal t,
						UT_Vector3DArray &valuelist, 
						UT_StringArray &stringlist);
    void		 buildStampListFromAttrib(fpreal t,
						UT_Vector3DArray &valuelist, 
						UT_StringArray &stringlist);
    UT_Vector3R		 buildStampListFromNumber(fpreal t);
    void		 buildStampListFromPrimitive(fpreal t,
						UT_Vector3DArray &valuelist, 
						UT_StringArray &stringlist);
		
    virtual void	 dirtyAllEachSOPs();

    bool		 MERGERESULTS(fpreal t)
			 { return evalInt("mergeresults", 0, t); }
    void		 GROUPMASK(UT_String &str, fpreal t)
			 { evalString(str, "groupmask", 0, t); }
    void		 ATTRIB(UT_String &str, fpreal t)
			 { evalString(str, "attrib", 0, t); }
    float		 ATTRIBTOL(fpreal t)
			 { return evalFloat("attribtol", 0, t); }

    void		 NUMRANGE(UT_Vector3R &v, fpreal t)
			 {
			     v.x() = evalFloat("numrange", 0, t);
			     v.y() = evalFloat("numrange", 1, t);
			     v.z() = evalFloat("numrange", 2, t);
			 }

    bool		 USEMAXITER(fpreal t)
			 { return evalInt("usemaxiter", 0, t); }
    int			 MAXITER(fpreal t)
			 { return evalInt("maxiter", 0, t); }

    int			 FORTYPE(fpreal t)
			 { return evalInt("fortype", 0, t); }
    void		 FORSTAMP(UT_String &str, fpreal t)
			 { evalString(str, "forstamp", 0, t); }
    void		 FORIDXSTAMP(UT_String &str, fpreal t)
			 { evalString(str, "foridxstamp", 0, t); }

    bool		 EACHPOINT(fpreal t)
			 { return evalInt("eachpoint", 0, t); }

    bool		 STOPCONDITION(fpreal t)
			 { return evalInt("stopcondition", 0, t); }

    GU_DetailHandle	 myFeedbackGeo;
    UT_Set<int>		 myEachSOPList;
    // Kept for info.
    int			 myLastCookPasses;
private:
};

#endif
