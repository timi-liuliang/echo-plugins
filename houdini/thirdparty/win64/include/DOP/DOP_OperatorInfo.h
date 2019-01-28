/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_OperatorInfo.h ( DOP Library, C++)
 *
 * COMMENTS:	This is additional information available via every DOP
 *		operator.
 */

#ifndef __DOP_OperatorInfo__
#define __DOP_OperatorInfo__

#include "DOP_API.h"
#include <OP/OP_Operator.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>

class DOP_API DOP_OperatorInfo : public OP_SpecificData
{
public:
	     DOP_OperatorInfo();
    virtual ~DOP_OperatorInfo();

    /// Returns the number of outputs for operators of this type.
    int			 getNumOutputs() const;
    /// Sets the number of outputs for operators of this type.
    void		 setNumOutputs(int outputs);

    /// Returns the name of the data that should be created by this node.
    /// This function is used by DOP_Auto nodes, which create a SIM_Data
    /// of a particular type.
    const UT_String	&getDataName() const;
    /// Sets the name of the data that should be created by this node.
    /// This function is used by DOP_Auto nodes, which create a SIM_Data
    /// of a particular type.
    void		 setDataName(const char *dataname);

    /// Returns the type of data that should be created by this node.
    /// This function is used by DOP_Auto nodes, which create a SIM_Data
    /// of a particular type.
    const UT_StringHolder &getDataTypeName() const
			    { return myDataTypeName; }
    /// Sets the type of data that should be created by this node.
    /// This function is used by DOP_Auto nodes, which create a SIM_Data
    /// of a particular type.
    void		 setDataTypeName(const UT_StringHolder &datatype)
			    { myDataTypeName = datatype; }

    /// Returns the type of data that represents the "lowest common
    /// demoninator" of data types that we are compatible with.
    const UT_String	&getLCDDataTypeName() const;
    /// Sets the type of data that represents the "lowest common
    /// demoninator" of data types that we are compatible with.
    void		 setLCDDataTypeName(const char *lcddatatype);

    /// Returns what type of data should be added if the node is
    /// bypassed.  Non-string means it should be removed in this case.
    const UT_String	&getBypassDataTypeName() const { return myBypassDataTypeName; }
    void		 setBypassDataTypeName(const char *name) { myBypassDataTypeName.harden(name); }

    bool		 getPureApplyData() const { return myPureApplyData; }
    void		 setPureApplyData(bool pureapply) { myPureApplyData = pureapply; }

    bool		 getInitialTemplateFlag() const { return myInitialTemplateFlag; }
    void		 setInitialTemplateFlag(bool val) { myInitialTemplateFlag = val; }

    virtual void	 saveToDialogScript(std::ostream &os) const;

private:
    UT_String		 myDataName;
    UT_StringHolder	 myDataTypeName;
    UT_String		 myBypassDataTypeName;
    UT_String		 myLCDDataTypeName;
    int			 myNumOutputs;
    bool		 myPureApplyData;
    bool		 myInitialTemplateFlag;
};

#endif

