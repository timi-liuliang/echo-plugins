/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_TakeParm.h ( OP Library, C++)
 *
 * COMMENTS:	Take information for node parms
 */

#ifndef __OP_TakeParm__
#define __OP_TakeParm__

#include "OP_API.h"
#include "OP_TakeData.h"
#include <iosfwd>

class PRM_Parm;

class OP_API OP_TakeParm : public OP_TakeData 
{
public:
	     OP_TakeParm(TAKE_DataId &id, const PRM_Parm *parm);
	     OP_TakeParm(const OP_TakeParm &src)
		 : OP_TakeData(src.getId(), src.getPriority())
	     {
		 myParmData.harden(src.myParmData);
		 myChanData.harden(src.myChanData);
	     }
	     OP_TakeParm(const TAKE_DataId &id, const OP_TakeParm &src)
		 : OP_TakeData(id, src.getPriority())
	     {
		 myParmData.harden(src.myParmData);
		 myChanData.harden(src.myChanData);
	     }
    virtual ~OP_TakeParm();

    virtual TAKE_Data	*duplicate() const;

    virtual bool	 canDuplicateForNode(int op_id) const;
    virtual TAKE_Data	*duplicateForNode(int op_id) const;

    virtual int		 save(std::ostream &os, int binary) const;
    virtual bool	 load(UT_IStream &is);

    virtual void	 storeCurrent();
    virtual void	 applyData();
    virtual void	 markActive();
    virtual void	 markInactive();
    virtual void	 generateIncludeCommand(UT_WorkBuffer &buf);
    virtual void	 generateCommand(UT_WorkBuffer &buf);
    virtual void	 getDescription(UT_WorkBuffer &buf, int brief);

    OP_TakeParm		&operator=(const OP_TakeParm &src);
private:
    void		 saveParmData(OP_Node *node, PRM_Parm *parm, int pidx,
				      UT_String &pdata, UT_String &cdata);
    void		 loadParmData(OP_Node *node, int pidx,
				      const char *pdata, const char *cdata,
				      bool send_event);
    virtual int64	 getMemoryUsage() const;
    PRM_Parm		*lookupParm(OP_Node *node=0) const;
    int			 lookupParmIndex(OP_Node *) const;

    UT_String		 myParmData;
    UT_String		 myChanData;
};

#endif

