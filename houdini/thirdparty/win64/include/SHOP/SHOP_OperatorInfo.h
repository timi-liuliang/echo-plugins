/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_OperatorInfo.h ( SHOP Library, C++)
 *
 * COMMENTS:	This is additional information available via every SHOP
 *		operator.
 */

#ifndef __SHOP_OperatorInfo__
#define __SHOP_OperatorInfo__

#include "SHOP_API.h"
#include <UT/UT_String.h>
#include <VEX/VEX_VexTypes.h>
#include <VEX/VEX_RslTypes.h>
#include "SHOP_NodeTypes.h"
class VOP_LanguageContextTypeList;


// Some clerks should be automatically added to every SHOP of a specify
// type (for example OGL shops should be added to all surface SHOPs).  The
// following enum controls which clerks can add themselves to the
// RenderMask.
enum {
    SHOP_AUTOADD_NONE	= 0,	// No clerks can add themselves
    SHOP_AUTOADD_STRING	= 1,	// Only clerks which have string data
    SHOP_AUTOADD_ANY	= 2	// Any clerk can add itself
};

class SHOP_API SHOP_OperatorInfo : public OP_SpecificData
{
public:
			 SHOP_OperatorInfo(int autoadd=SHOP_AUTOADD_ANY,
			       const char *rmask="*",
			       SHOP_TYPE type=SHOP_INVALID);
    virtual		~SHOP_OperatorInfo();

    const char		*getRenderMask() const	{ return myRenderMask; }
    SHOP_TYPE		 getShaderType() const	{ return myShaderType; }
    bool		 isRenderMaskRSL() const;

    void		 setShaderType(SHOP_TYPE t);
    void		 setRenderMask(const char *t);

    virtual void	 saveToDialogScript(std::ostream &os) const;
    virtual void	 updateExtraInfoBuffer(OP_ExtraInfoBuffer &b) const;

    void		 setNumOutputs(int n)	{ myNumOutputs = n; }
    int			 getNumOutputs() const	{ return myNumOutputs; }

    VEX_ContextType	 getVexContextType() const;
    RSL_ContextType	 getRslContextType() const;

    /// Obtains context types applicable for the shop. For single-context
    /// shaders, the list will contain a single entry. For multi-context shaders
    /// the list will contain several entries. If error, the list will be empty.
    /// Creates a new instance, thus it is caller's responsibility to delete it.
    VOP_LanguageContextTypeList *   newLanguageContextTypeList() const;


    bool		 skipInputEditor() const
				{ return mySkipInputEditor; }
    void		 setSkipInputEditor(bool v)
				{ mySkipInputEditor = v; }

    /// Flag that indicates whether the operator is a vopnet shop material
    /// containing many shading contexts (eg, surface, displacement, etc).
    /// @{
    void		setIsMultiContextMaterial( bool flag )
				{ myMultiContextMaterialFlag = flag; }
    bool		getIsMultiContextMaterial() const
				{ return myMultiContextMaterialFlag; }
    /// @}

    /// @{ Has node input connectors corresponding to co-shader parameters.
    void		setHasInputsForCoshaderParms( bool flag )
				{ myHasInputsForCoshaderParms = flag; }
    bool		hasInputsForCoshaderParms() const
				{ return myHasInputsForCoshaderParms; }
    /// @}

private:
    SHOP_TYPE		 myShaderType;
    UT_String		 myRenderMask;
    int			 myAutoAdd;
    int			 myNumOutputs;	// 1 for everything but subnet?
    bool		 mySkipInputEditor;
    bool		 myMultiContextMaterialFlag;
    bool		 myHasInputsForCoshaderParms;
};
#endif

