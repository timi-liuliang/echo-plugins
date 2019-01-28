/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __MGR_Node_h__
#define __MGR_Node_h__

#include "MGR_API.h"
#include <OP/OP_Network.h>

class SHOP_MaterialFilter;

// Note: order matches MGRmanagerNames and other tables table.
typedef enum {
    MGR_OBJECT_MANAGER = 0,
    MGR_ROP_MANAGER,
    MGR_PARTICLE_MANAGER,
    MGR_CHOPNET_MANAGER,
    MGR_SHOP_MANAGER,
    MGR_COP_MANAGER,
    MGR_VOPNET_MANAGER,
    MGR_MAT_MANAGER,
    MGR_LOP_MANAGER,
    MGR_NUM_MANAGERS	// sentinel
} MGR_MANAGER_TYPE;


// Be very cautious about renaming these manager level nodes.
// It's relatively safe to do, but if you do, all globbing breaks (i.e.
// ^/obj/light1).  Which makes it very unsafe right now.  Well, at least
// for /obj.
#define OBJECT_MANAGER_NAME	OBJ_SCRIPT_NAME
#define OUTPUT_MANAGER_NAME	ROP_SCRIPT_NAME
#define PARTICLE_MANAGER_NAME	POPNET_SCRIPT_NAME
#define CHOPNET_MANAGER_NAME	CHOPNET_SCRIPT_NAME
#define SHOP_MANAGER_NAME	SHOP_SCRIPT_NAME
#define COMPOSITE2_MANAGER_NAME COPNET_SCRIPT_NAME
#define VOPNET_MANAGER_NAME	VOPNET_SCRIPT_NAME
#define MAT_MANAGER_NAME	MAT_SCRIPT_NAME
#define LOP_MANAGER_NAME	"stage"

class MGR_API MGR_Node : public OP_Network
{
public:
    virtual const char	*getChildType() const;
    virtual const char	*getOpType() const;

    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;
    static const char	*theChildTableName;

    static PRM_Template	 myTemplateList[];

    virtual OP_Node	*getDisplayNodePtr();
    virtual OP_Node	*getRenderNodePtr();

    // deprecated method: interest gets cleared out before every cook
    virtual void	 addExtraInput(OP_Node *op, OP_InterestType type);

    // Manager nodes do not have outputs
    virtual unsigned	 maxOutputs() const { return 0; }

    static OP_Operator	*getManagerOperator(MGR_MANAGER_TYPE type);
    static const char	*getManagerName(MGR_MANAGER_TYPE type);
    static void		 initTable(MGR_MANAGER_TYPE type);

protected:
	     MGR_Node(OP_Network *parent, MGR_MANAGER_TYPE type);
    virtual ~MGR_Node();

    virtual OP_ERROR	 cookMe(OP_Context &);
    virtual OP_ERROR	 bypassMe(OP_Context &, int &);

    virtual OP_DataType	 getCookedDataType() const;
    virtual void	*getCookedData(OP_Context &);
    virtual void	 deleteCookedData();
    virtual int		 saveCookedData(const char *, OP_Context &);
    virtual int		 saveCookedData(std::ostream &os, OP_Context &, 
               		                int binary = 0);
	    int		 aliasMatch(const char *name) const;

    virtual const char	*getFileExtension(int binary) const;

    virtual void	 childFlagChange(OP_Node *);
    virtual void	 addNode(OP_Node *node, int notify=1, int explicitly=1);
    virtual OP_OperatorFilter	*getOperatorFilter();

private:
    static void		 buildObjTable(const char *);		// Objects
    static void		 buildIceTable(const char *);		// CompNets
    static void		 buildRopTable(const char *);		// RopNets
    static void		 buildPartTable(const char *);		// PopNets
    static void		 buildChopTable(const char *);		// ChopNets
    static void		 buildShopTable(const char *);		// Shops
    static void		 buildCopTable(const char *);		// Comp v2 Nets
    static void		 buildVopNetTable(const char *);	// VopNets
    static void		 buildDopTable(const char *);		// Dops
    static void		 buildVopTable(const char *);		// Vops
    static void		 buildTopTable(const char *);		// Tops
    static void		 buildLopTable(const char *);		// Lops

    // This adds all the management ops to the table.  Woe be us!
    static void		 addManagementOps(OP_OperatorTable *table);

    // This builds the funky tables which H_OPZ will install a callback
    // for.  A preposition it is bad to end a sentence with.
    void	 buildOpZTables();

    const char		**myAliases;
    MGR_MANAGER_TYPE	myType;
    UT_UniquePtr<SHOP_MaterialFilter>   myOpFilter;

    // Make a friend of the director so that he can create any manager he
    // wants.
    // TODO: Change the friend class when the library is defined
    friend class	MOT_Director;
};

#endif
