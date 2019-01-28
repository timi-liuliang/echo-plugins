/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LOP Library (C++)
 *
 * COMMENTS:	LOP manager node for other network types.
 *
 */

#ifndef __LOP_Network_h__
#define __LOP_Network_h__

#include "LOP_API.h"
#include "LOP_Node.h"
#include "LOP_SelectionRule.h"
#include <HUSD/HUSD_ExpansionState.h>
#include <HUSD/HUSD_FindPrims.h>
#include <HUSD/HUSD_Overrides.h>
#include <UT/UT_StringMap.h>

#define LOP_EXPANSION_DATA_BLOCK_TYPE "expansion"
#define LOP_SESSIONLAYER_DATA_BLOCK_TYPE "sessionlayer"

enum LOP_NetworkCustomData
{
    LOP_NETWORK_OVERRIDES,
    LOP_NETWORK_SELECTION_RULES
};

class LOP_API LOP_Network : public LOP_Node
{
public:
    // Standard OP_Network stuff:
    static const char	*theChildTableName;
    static OP_Node	*myConstructor(OP_Network *net, const char *name,
				OP_Operator *entry);
    static PRM_Template	 myTemplateList[];

    virtual int		 isSubNetwork(bool includemanagementops) const override;

    const HUSD_ExpansionState	&expansionState() const
				 { return myExpansionState; }
    HUSD_ExpansionState		&expansionState()
				 { return myExpansionState; }

    HUSD_ConstOverridesPtr	 overrides() const
				 { return myOverrides; }
    HUSD_OverridesPtr		 editableOverrides();
    void			 deleteOverrides();

    const UT_StringArray	&selection() const
				 { return mySelection; }
    const UT_StringHolder	&selectionLayerId() const
				 { return mySelectionLayerId; }
    const UT_StringHolder	&selectionCurrentPrim() const
				 { return mySelectionCurrentPrim; }
    void			 setSelection(const UT_StringArray &paths,
					const UT_StringHolder *layerid =
					    nullptr,
					const UT_StringHolder *currentprim =
					    nullptr);

    const LOP_SelectionRuleMap	&selectionRules() const;
    void			 clearSelectionRules();
    void			 removeSelectionRule(
					const UT_StringHolder &name);
    void			 addSelectionRule(
					const UT_StringHolder &name,
					const LOP_SelectionRule &collection);

    void			 saveSelectionRules(std::ostream &os) const;
    bool			 loadSelectionRules(UT_IStream &is);

protected:
	     LOP_Network(OP_Network *parent, const char *name,
			 OP_Operator *entry);
    virtual ~LOP_Network();

    virtual void	 clearAndDestroy() override;

    virtual OP_ERROR	 cookMyLop(OP_Context &context) override;

    virtual OP_ERROR	 save(std::ostream &os, const OP_SaveFlags &flags,
				const char *pathPrefix,
				const UT_String &name_override =
				    UT_String()) override;
    virtual bool	 load(UT_IStream &is, const char *extension,
				const char *path=0) override;

private:
    HUSD_ExpansionState		 myExpansionState;
    HUSD_OverridesPtr		 myOverrides;
    UT_StringArray		 mySelection;
    UT_StringHolder		 mySelectionLayerId;
    UT_StringHolder		 mySelectionCurrentPrim;
    LOP_SelectionRuleMap	 mySelectionRuleMap;
};

#endif
