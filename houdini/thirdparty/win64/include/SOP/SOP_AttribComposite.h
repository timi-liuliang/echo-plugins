/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:	SOP which composites attributes from two or more geometries
 *
 */

#ifndef __SOP_AttribComp_h__
#define __SOP_AttribComp_h__

#include "SOP_API.h"
class GA_Primitive;
class GD_Primitive;

#include "SOP_Node.h"
#include "SOP_NodeParmsOptions.h"

#include <GA/GA_Types.h>

#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>

class SOP_API SOP_AttribComp : public SOP_Node
{
public:
			SOP_AttribComp(OP_Network *net, const char *name,
			    OP_Operator *entry);
    virtual	       ~SOP_AttribComp();

    virtual void	syncNodeVersion(const char *old_version,
					const char *current_version,
					bool *node_deleted) override;
    static OP_Node     *myConstructor(OP_Network *net, const char *name,
			    OP_Operator *entry);

    void		setSelectionOverride(bool selection_override);
    void		setBlendValue(int i, float value, fpreal t);
    void		setBlendAlias(int i, const char *alias);
    void		removeBlend(int i);

    static PRM_Template myTemplateList[];

    const SOP_NodeVerb	*cookVerb() const override;

protected:
    virtual const char *inputLabel(unsigned) const override;
    virtual bool	updateParmsFlags() override;

    virtual OP_ERROR    cookInputGroups(OP_Context &context,
				int alone = 0) override;
    virtual OP_ERROR	cookMySop(OP_Context &context) override;

private:
    static void		buildAlphaMenu(void *data, PRM_Name *entries, int size,
				       const PRM_SpareData *, const PRM_Parm *);

    void  setNBLENDS(int n)
	    { setInt(		"nblends",	0, 0, n); }
    int   SELECTIONIDX(void)
	    { return evalInt("selectionidx",	0, 0); }
    int   DETAILATTRIBCOMP()
	    { return evalInt(	"detailattribs",	0, 0); }
    void  DETAILATTRIBS(UT_String &str)
	    { evalString(str,	"detailattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int   PRIMATTRIBCOMP()
	    { return evalInt(	"primitiveattribs",	0, 0); }
    void  PRIMATTRIBS(UT_String &str)
	    { evalString(str,	"primattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int   PTATTRIBCOMP()
	    { return evalInt(	"pointattribs",	0, 0); }
    void  PTATTRIBS(UT_String &str)
	    { evalString(str,	"pointattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int   VTXATTRIBCOMP()
	    { return evalInt(	"vertexattribs",	0, 0); }
    void  VTXATTRIBS(UT_String &str)
	    { evalString(str,	"vertexattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int	  MATCHPATTRIB()
	    { return evalInt("matchpattrib", 0, 0); }
    int	  NBLENDS()
	    { return evalInt(	"nblends",	0, 0); }
    float BLEND(int i, fpreal t)
	    { return evalFloatInst("blend#", &i,		0, t); }

    void  ALPHAATTRIB(UT_String &str)
	    { evalString(str,	"alphaattrib",	0, 0); }
    int   COMPOP()
	    { return evalInt(	"compop",		0, 0); }

    static PRM_ChoiceList       ourAlphaAttribMenu;
};


class SOP_API SOP_AttribCompositeCache : public SOP_NodeCacheOptions
{
public:
	     SOP_AttribCompositeCache();
    virtual ~SOP_AttribCompositeCache();

protected:
    virtual OP_ERROR	cook(OP_Context &context) override;
    

    void		parseMySop(
			    UT_ValArray<GA_Attribute *> &detailattribs,
			    UT_ValArray<GA_Attribute *> &primattribs,
			    UT_ValArray<GA_Attribute *> &pointattribs,
			    UT_ValArray<GA_Attribute *> &vertexattribs);
    bool		verifyGeometries(void);
    void		verifyAttribsExist(UT_ValArray<GA_Attribute *> &attribs,
			    GA_AttributeOwner owner);
    void		setupOpFN(void);
    void		setupWeights(int numWeights, fpreal t);
    void		setAlphaAttrib(GA_AttributeOwner owner,
				UT_Array<GA_ROHandleR> &alphaattribs,
				const UT_String &alpha);
    GA_Attribute *	createAttrib(GA_Attribute *destattrib,
					UT_ValArray<UT_IntArray *> &new_indices);
    void		compAttrib(
                            GA_AttributeOwner owner,
                            GA_Attribute *destattrib,
                            const UT_Array<GA_ROHandleR> &alphaattribs,
                            const UT_ValArray<UT_IntArray *> &new_indices,
                            const UT_Array<GA_Offset> &arrsrcoffsets,
                            GA_Offset destoffset, int compop);
    void		compGrpAttribs(
			    UT_ValArray<GA_Attribute *> &detailattribs,
			    UT_ValArray<GA_Attribute *> &primattribs,
			    UT_ValArray<GA_Attribute *> &pointattribs,
			    UT_ValArray<GA_Attribute *> &vertexattribs);


    int   SELECTIONIDX(void)
	    { return evalInt("selectionidx",	0, 0); }
    int   DETAILATTRIBCOMP()
	    { return evalInt(	"detailattribs",	0, 0); }
    void  DETAILATTRIBS(UT_String &str)
	    { evalString(str,	"detailattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int   PRIMATTRIBCOMP()
	    { return evalInt(	"primitiveattribs",	0, 0); }
    void  PRIMATTRIBS(UT_String &str)
	    { evalString(str,	"primattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int   PTATTRIBCOMP()
	    { return evalInt(	"pointattribs",	0, 0); }
    void  PTATTRIBS(UT_String &str)
	    { evalString(str,	"pointattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int   VTXATTRIBCOMP()
	    { return evalInt(	"vertexattribs",	0, 0); }
    void  VTXATTRIBS(UT_String &str)
	    { evalString(str,	"vertexattriblist",	0, 0);
	      str.harden();
	      str.trimSpace(true);
	    }
    int	  MATCHPATTRIB()
	    { return evalInt("matchpattrib", 0, 0); }
    int	  NBLENDS()
	    { return evalInt(	"nblends",	0, 0); }
    float BLEND(int i, fpreal t)
	    { return evalFloatInst("blend#", &i,		0, t); }

    void  ALPHAATTRIB(UT_String &str)
	    { evalString(str,	"alphaattrib",	0, 0); }
    int   COMPOP()
	    { return evalInt(	"compop",		0, 0); }

    exint                       myGdpIdx;
    UT_Array<const GU_Detail *> mySrcGdps;
    UT_Array<const GA_Attribute *> mySrcAttributes;
    void                        (*myOpFN)(fpreal, fpreal*, fpreal);
    bool                        myOverUnder;
    UT_FloatArray               myWeights;
};

#endif
