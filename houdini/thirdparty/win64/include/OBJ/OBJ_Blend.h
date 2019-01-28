/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Blend.h (Custom Library, C++)
 *
 * COMMENTS:    An object which combines multiple parents as a single
 *		parent transform
 */

#ifndef __OBJ_Blend__
#define __OBJ_Blend__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"
#include <UT/UT_Quaternion.h>
#include <UT/UT_XformOrder.h>

#define MAX_BLEND_PARENTS	4

enum OBJ_BlendIndex
{
    I_BLENDTYPE = I_N_GEO_INDICES,
    
    I_USERESTANGLES,
    I_SHORTROTBLEND,
    I_BLENDW1,
    I_BLENDM1,
    I_BLENDREST1,
    I_BLENDRESTORD1,
    I_BLENDW2,
    I_BLENDM2,
    I_BLENDREST2,
    I_BLENDRESTORD2,
    I_BLENDW3,
    I_BLENDM3,
    I_BLENDREST3,
    I_BLENDRESTORD3,
    I_BLENDW4,
    I_BLENDM4,
    I_BLENDREST4,
    I_BLENDRESTORD4,
    I_NOFFSET,
    I_AXESORIENT,
    
    I_SEQUENCE1,
    I_SHORTROT,
    I_IGNORESCALES,
    
    I_SEQUENCE2,
    I_RESET,
    
    I_N_PXFORM_INDICES     // should always be last in the list
};


#define FLOAT_OBJ_PARM(name, idx, vi, t)    \
	return evalFloat(name, &getIndirect()[idx], vi, t);
#define INT_OBJ_PARM(name, idx, vi, t)      \
	return evalInt(name, &getIndirect()[idx], vi, t);
#define STR_OBJ_PARM(name, idx, vi, t)      \
	evalString(str, name, &getIndirect()[idx], vi, t);

class OBJ_API OBJ_Blend : public OBJ_Geometry
{
public:
				 OBJ_Blend(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Blend();

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		*getObsolete();
    virtual bool	 	 updateParmsFlags();
    virtual const char		*inputLabel(unsigned idx) const;

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    static PRM_Template		*getTemplateList();

    virtual OBJ_Blend		*castToOBJBlend()	{ return this; }
    virtual void	 resolveObsoleteParms(PRM_ParmList *obsolete_parms);

    // DO NOT CHANGE THE NUMBERS IN THIS ENUM! 
    enum {
	BLEND_BLEND	    = 0,
	BLEND_SEQUENCE	    = 1,
	BLEND_CONSTRAIN     = 2
    };

    int		BLENDTYPE()
		{ INT_OBJ_PARM("blendtype", I_BLENDTYPE, 0, 0); }
    void	setBLENDTYPE( int parent_type )
		{ setInt("blendtype", getIndirect()[I_BLENDTYPE], 0, 0,
			 parent_type); }

    const char *getSequenceName()
	{ return BLENDTYPE() == BLEND_SEQUENCE?"sequence_seq":"sequence_con"; }
    int		getSequenceIndex() 
	{ return BLENDTYPE() == BLEND_SEQUENCE?I_SEQUENCE1 : I_SEQUENCE2; }
    
    fpreal	SEQUENCE(fpreal t)
		{ FLOAT_OBJ_PARM(getSequenceName(), getSequenceIndex(), 0, t); }
    void	setSEQUENCE(fpreal t, fpreal seq)
		{ setFloat(getSequenceName(),
			   getIndirect()[getSequenceIndex()],  0, t, seq); }
    void	keySEQUENCE(fpreal t);
    void	keySEQUENCE(fpreal t, const char *expr,
			    CH_ExprLanguage language);

    fpreal	BLENDW(int i, fpreal t)
		{ 
		    switch(i)
		    {
			case 0:
			    FLOAT_OBJ_PARM("blendw1",  I_BLENDW1, 0, t); 
			case 1:
			    FLOAT_OBJ_PARM("blendw2",  I_BLENDW2, 0, t); 
			case 2:
			    FLOAT_OBJ_PARM("blendw3",  I_BLENDW3, 0, t); 
			case 3:
			    FLOAT_OBJ_PARM("blendw4",  I_BLENDW4, 0, t); 
		    }
		    return 0;
		}

    void	setIGNORESCALES(int ignore)
		{ setInt("ignorescales", getIndirect()[I_IGNORESCALES],
			 0, 0.0f, ignore); }
		
    virtual OP_ERROR	cookMyObj(OP_Context &context);
    virtual OP_ERROR	lockInput(unsigned idx, OP_Context &context);
    virtual void	unlockInput(unsigned idx);
    virtual OP_ERROR	lockInputs(OP_Context &context);
    virtual void	unlockInputs();

protected:
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return blendIndirect; }

    virtual int			 isObjectRenderable(fpreal t) const;

    virtual int			 applyInputDependentTransform(
				    OP_Context &context, UT_DMatrix4 &mat);

private:

    int		simpleInputIndex(OP_Context &context);

    int		getBlendTransform(UT_DMatrix4 &pmat, OP_Context &context);
    int		getSequenceTransform(UT_DMatrix4 &pmat, OP_Context &context);
    int		getConstrainTransform(UT_DMatrix4 &pmat, OP_Context &context);

    int		calcConstrainTransform(UT_DMatrix4 &pmat, OP_Context &context,
	    				int force_cook);

    void	orientAxes(UT_DMatrix4 &pmat, const UT_Vector3R *p);

    bool	USERESTANGLES(fpreal t) const
		{ INT_OBJ_PARM("userestangles", I_USERESTANGLES, 0, t); }
    int		SHORTROT()
		{ INT_OBJ_PARM("shortrot", I_SHORTROT, 0, 0); }
    int		SHORTROTBLEND()
		{ INT_OBJ_PARM("shortrotblend", I_SHORTROTBLEND, 0, 0); }
    int		IGNORESCALES()
		{ INT_OBJ_PARM("ignorescales", I_IGNORESCALES, 0, 0); }
    fpreal	RESET(fpreal t)
		{ FLOAT_OBJ_PARM("reset", I_RESET, 0, t); }

    int		BLENDM(int i)
		{ 
		    switch(i)
		    {
			case 0:
			    INT_OBJ_PARM("blendm1", I_BLENDM1, 0, 0);
			case 1:
			    INT_OBJ_PARM("blendm2", I_BLENDM2, 0, 0);
			case 2:
			    INT_OBJ_PARM("blendm3", I_BLENDM3, 0, 0);
			case 3:
			    INT_OBJ_PARM("blendm4", I_BLENDM4, 0, 0);
		    }
		    return 0;
		}

    UT_QuaternionR BLENDREST(int i, fpreal t) const
		{ 
		    UT_ASSERT_P(i >= 0 && i < 4);
		    const char * const blendrest[4]
			= { "blendrest1", "blendrest2",
			    "blendrest3", "blendrest4" };
		    const char * const blendrestord[4]
			= { "blendrestord1", "blendrestord2",
			    "blendrestord3", "blendrestord4" };
		    const int indirects[4]
			= { I_BLENDREST1, I_BLENDREST2,
			    I_BLENDREST3, I_BLENDREST4 };

		    UT_Vector3R rot;
		    int xyz;

		    int *pi = &getIndirect()[indirects[i]];
		    evalFloats(blendrest[i], pi + 0, rot.data(), t);
		    xyz = evalInt(blendrestord[i], pi + 1, 0, t);

		    rot.degToRad();
		    return UT_QuaternionR(
				rot,
				UT_XformOrder(UT_XformOrder::SRT,
					      OP_Node::getRotOrder(xyz)));
		}

    fpreal	NOFFSET(fpreal t)
		{ FLOAT_OBJ_PARM("noffset",  I_NOFFSET, 0, t); }

    int		AXES_ORIENT()
		{ INT_OBJ_PARM("axesorient",  I_AXESORIENT, 0, 0); }

    static int		*blendIndirect;

    int			 mySimpleInputIndex;

    UT_DMatrix4		 myConstrainAdjustment;
    UT_DMatrix4		 myLastConstrainMat;
    UT_DMatrix4		 myLastParent[MAX_BLEND_PARENTS];
    UT_DMatrix4		 myCurrentParent[MAX_BLEND_PARENTS];
    int			 myConstrainIndex;

    fpreal		 myLastFrame;
    int			 myFirstConstrain;

};

#undef FLOAT_OBJ_PARM
#undef STR_OBJ_PARM
#undef INT_OBJ_PARM

#endif
