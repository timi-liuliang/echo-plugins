/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_TimingBase.h
 *
 * COMMENTS:
 *	Base class for COPs that only modify timing information. 
 */
#ifndef _COP2_TIMING_BASE_H_
#define _COP2_TIMING_BASE_H_

#include "COP2_API.h"
#include "COP2_Node.h"


class COP2_API COP2_TimingBase : public COP2_Node
{
public:
    // This is the method which timing COPs need to override. 
    virtual TIL_Sequence *cookSequenceInfo(OP_ERROR &err);
    virtual bool	  isTimeModifier() const { return true; }
    virtual bool	  isSlowOperation() const { return false; }

    virtual bool		isTimeDepOperation() { return true; }
protected:
	     COP2_TimingBase(OP_Network *parent, const char *name,
			    OP_Operator *entry);
    virtual ~COP2_TimingBase();

    void		 makeOutputAreaDependOnDifferentTime(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas,
			    float time);

    virtual void	getInputDependenciesForOutputArea(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);
    
};


#endif
