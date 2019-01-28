/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_AutoOperator.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_AutoOperator_h__
#define __DOP_AutoOperator_h__

#include "DOP_API.h"
#include "DOP_Operator.h"

class SIM_Engine;

/// OP_Operator subclass for operators described by a SIM_DopDescription.
class DOP_API DOP_AutoOperator : public DOP_Operator
{
public:
    DOP_AutoOperator(const char *name, const char *english,
                     OP_Constructor construct, PRM_Template *templates,
                     const char *child_table_name, unsigned min_sources,
                     unsigned max_sources, CH_LocalVariable *variables,
                     unsigned flags, unsigned num_outputs);

    virtual bool getVersion(UT_String &version) override;

private:
    bool getVersion(const SIM_Engine &engine, UT_String &version);
};

#endif
