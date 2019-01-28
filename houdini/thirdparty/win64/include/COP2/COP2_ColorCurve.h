/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Bright.h
 *
 * COMMENTS:
 *	ColorCurve COP.
 */
#ifndef _COP2_COLORCURVE_H_
#define _COP2_COLORCURVE_H_

#include "COP2_API.h"
#define ARG_COLORCURVE_EDIT	(myParmBase + 0)
#define ARG_COLORCURVE_EXTEND	(myParmBase + 1)
// label
#define ARG_COLORCURVE_USE_COMP (myParmBase + 3)
#define ARG_COLORCURVE_ALL	(myParmBase + 4)
#define ARG_COLORCURVE_RED	(myParmBase + 5)
#define ARG_COLORCURVE_GREEN	(myParmBase + 6)
#define ARG_COLORCURVE_BLUE	(myParmBase + 7)
#define ARG_COLORCURVE_ALPHA	(myParmBase + 8)

#include "COP2_PixelOp.h"

class COP2_API COP2_ColorCurve : public COP2_PixelOp
{
public:
    
    static OP_Node		*myConstructor(OP_Network*, const char *,
					       OP_Operator *);
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];
    static const char *		 myInputLabels[];

    static int			 componentModeChanged(void *, int, float,
						      const PRM_Template *);
    static int			 globalChanged(void *, int, float,
					       const PRM_Template *);
    static int			 redChanged(void *, int, float,
					       const PRM_Template *);
    static int			 greenChanged(void *, int, float,
					       const PRM_Template *);
    static int			 blueChanged(void *, int, float,
					       const PRM_Template *);
    static int			 alphaChanged(void *, int, float,
					       const PRM_Template *);

    bool	areComponentsModified();

    void	componentModeChanged();
protected:
    virtual RU_PixelFunction	*addPixelFunction(const TIL_Plane *plane,
						  int array_index, float t,
						  int xres, int yres,
						  int thread);
    
    virtual bool isFrameEffectHandled() { return false; }
    
    virtual bool		 updateParmsFlags();
private:
		 COP2_ColorCurve(OP_Network *parent, const char *name,
				 OP_Operator *entry);
    virtual	~COP2_ColorCurve();



    bool	EXTEND_SLOPE()
		{ return (bool)evalInt(ARG_COLORCURVE_EXTEND,0,0.0f); }
		
    int		COLOR_CURVE_ALL() {return evalInt(ARG_COLORCURVE_ALL,0,0.0f);}
    bool	COMPONENTS() {return evalInt(ARG_COLORCURVE_USE_COMP,0,0.0f);}

    int		COLOR_CURVE_COMP(int index)
		{ return evalInt(ARG_COLORCURVE_RED+index,0,0.0f); }

    void	COLORCURVE_POINT_ALL(int index, float &x, float &y, float &s,
				     float t);
    void	COLORCURVE_POINT_COMP(int comp, int index, float &x, float &y,
				      float &s, float t);
    int		 myParmBase;
};
#endif
