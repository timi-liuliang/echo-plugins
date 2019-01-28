/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Levels.h ( COP2 Library, C++)
 *
 * COMMENTS:
 */

#ifndef __COP2_Levels__
#define __COP2_Levels__

#define ARG_LEVELS_GAMMA		(myParmBase + 2)
#define ARG_LEVELS_GAMMA_RED		(myParmBase + 6)
#define ARG_LEVELS_GAMMA_GREEN		(myParmBase + 10)
#define ARG_LEVELS_GAMMA_BLUE		(myParmBase + 14)
#define ARG_LEVELS_GAMMA_A		(myParmBase + 18)
#define ARG_LEVELS_GAMMA_W		(myParmBase + 22)

#define ARG_LEVELS_INPUT		(myParmBase + 1)
#define ARG_LEVELS_RED_INPUT		(myParmBase + 5)
#define ARG_LEVELS_GREEN_INPUT		(myParmBase + 9)
#define ARG_LEVELS_BLUE_INPUT		(myParmBase + 13)
#define ARG_LEVELS_ALPHA_INPUT		(myParmBase + 17)
#define ARG_LEVELS_W_INPUT		(myParmBase + 21)

#define ARG_LEVELS_OUTPUT		(myParmBase + 4)
#define ARG_LEVELS_RED_OUTPUT		(myParmBase + 8)
#define ARG_LEVELS_GREEN_OUTPUT		(myParmBase + 12)
#define ARG_LEVELS_BLUE_OUTPUT		(myParmBase + 16)
#define ARG_LEVELS_ALPHA_OUTPUT		(myParmBase + 20)
#define ARG_LEVELS_W_OUTPUT		(myParmBase + 24)

#include "COP2_PixelOp.h"

class COP2_API COP2_Levels : public COP2_PixelOp
{
public:
    
    static OP_Node		*myConstructor(OP_Network*, const char *,
					       OP_Operator *);
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];
    static const char *		 myInputLabels[];

    static const float		 theGammaMax;
    static const float		 theGammaMin;
    static const char* 		 theName;
    static const char* 		 theEnglish;

    virtual const char     *className() const;

protected:
    virtual RU_PixelFunction	*addPixelFunction(const TIL_Plane *plane,
						  int array_index, float t,
						  int xres, int yres,
						  int thread);
     
    static int			 editHist(void *, int, float,
					  const PRM_Template *);
private:
		COP2_Levels(OP_Network *parent, const char *name,
			   OP_Operator *entry);
    virtual	~COP2_Levels();

    virtual const char  *getOperationInfo();

    float	GAMMA(float t)
		{ return evalFloat(ARG_LEVELS_GAMMA,0,t); }
    
    float	GAMMA_RED(float t)
		{ return evalFloat(ARG_LEVELS_GAMMA_RED,0,t); }
    float	GAMMA_GREEN(float t)
		{ return evalFloat(ARG_LEVELS_GAMMA_GREEN,0,t); }
    float	GAMMA_BLUE(float t)
		{ return evalFloat(ARG_LEVELS_GAMMA_BLUE,0,t); }
    float	GAMMA_W(float t)
		{ return evalFloat(ARG_LEVELS_GAMMA_W,0,t); }
    float	GAMMA_ALPHA(float t)
		{ return evalFloat(ARG_LEVELS_GAMMA_A,0,t); }
    
    void	INPUT_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_INPUT, t, range_min, range_max);
		}
    
    void	INPUT_RED_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_RED_INPUT, t, range_min, range_max);
		}
    
    void	INPUT_GREEN_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_GREEN_INPUT, t, range_min, range_max);
		}
    
    void	INPUT_BLUE_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_BLUE_INPUT, t, range_min, range_max);
		}
    
    void	INPUT_ALPHA_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_ALPHA_INPUT, t, range_min, range_max);
		}
    
    void	INPUT_W_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_W_INPUT, t, range_min, range_max);
		}
    
    
    void	OUTPUT_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_OUTPUT, t, range_min, range_max);
		}
    
    void	OUTPUT_RED_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_RED_OUTPUT, t, range_min, range_max);
		}
    
    void	OUTPUT_GREEN_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_GREEN_OUTPUT, t, range_min, range_max);
		}
    
    void	OUTPUT_BLUE_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_BLUE_OUTPUT, t, range_min, range_max);
		}
    
    void	OUTPUT_ALPHA_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_ALPHA_OUTPUT, t, range_min, range_max);
		}
    
    void	OUTPUT_W_MAP(float t, float &range_min, float &range_max)
		{ loadMapParm(ARG_LEVELS_W_OUTPUT, t, range_min, range_max);
		}
    
    void	loadMapParm(const int pi, float t, 
			    float& range_min, float& range_max);
    
    int		myParmBase;

    static PRM_Name	theNames[];
    static PRM_Range	theGammaRange;
    static PRM_Range	theLevelsRange;
    static PRM_Default	theLevelsDef[];
    static PRM_Default	theCompDefs[];

};
#endif
