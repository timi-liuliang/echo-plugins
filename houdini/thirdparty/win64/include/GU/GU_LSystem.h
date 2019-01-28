#ifndef __GU_LSystem_h__
#define __GU_LSystem_h__

#include "GU_API.h"
#include "GU_LSysExpr.h"
#include <GA/GA_Handle.h>

#include <UT/UT_Array.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

#ifdef WIN32
    #include <float.h>
#endif

enum GU_LSystemGeoType
{
    GU_LSYSTEM_SKEL  = 0,
    GU_LSYSTEM_TUBE  = 1
};

class TIL_TextureMap;
class IMG_Raster;
class GA_ATIBlindData;
class GA_PrimitiveGroup;
class GEO_Face;
class GEO_PrimPoly;
class GU_Detail;
class gu_LSysTurtleStack;
class gu_LSysTurtleData;

class GU_API guLsysRule
{
public:

    guLsysRule(const char *r)
    {
	rule = r ? strdup(r) : 0;
	srule = 0;
	cond = 0;
	probs = 0;
	xyzRule = 0;
	if (r)
	{
	    // trim is a temporary buffer we store the space trimmed
	    // rule.
	    trim = new char [strlen(r) + 1];
	    trim[0] = '\0';
	}
	else
	    trim = 0;
    }

    ~guLsysRule()
    {
	if (rule)
	    free(rule);
	if (trim)
	    delete [] trim;
    }

    char	*rule;		// original
    char	*srule;		// substitution 
    char	*cond;		// condition 
    char	*probs;		// probability
    char	 xyzRule;	// contains refs to x,y,z

    char	*trim;		// no spaces rules
    char	 pred[LSYS_MAX_PARAMS+2];
    char	 lstr[LSYS_MAX_PARAMS+2];
    char	 rstr[LSYS_MAX_PARAMS+2];

};


class GU_API GU_LSystemParms
{
public:

    GU_LSystemParms();
    ~GU_LSystemParms();

    //  Pre-Construction initialization values 

    GU_LSystemGeoType geotype;			// skeleton or tube
    float	  generations;			// number of recursions
    int	  	  contAngl;			// fractional angles
    int	  	  contLength;			// fractional forward growth
    int	  	  contWidth;			// fractional tube growth
    bool	  myOldStyleContext;		// Context rules cross into
						// sibling branches.

    IMG_Raster		*picRaster;		// used in 'pic' function

    char		*groupLabel;		// group name base

    TIL_TextureMap	*colorMap;		// map to add colors with
    float	  	 colorUInc;		// U color texture increment
    float	  	 colorVInc;		// V color texture increment

    int			 doPointWidth;		// Add width pt attribute
    int			 doPointSort;		// Sort the points by age.
    int		 	 doTexture;
    float	 	 textVInc;		// V texture increment

    float	 	 b, c, d;		// extra expression variables

    UT_Vector3		 startpos;		// initial position
    float	 	 size;			// initial step size
    float	 	 thickness;		// tube thickness
    float	 	 angle;			// angle value to turn by

    float	 	 sizeFactor;		// factor to * step size by
    float	 	 thicknessFactor;	// factor to * thickness by
    float	 	 angleFactor;		// factor to * angle value by


    float	 	 rndScale;		// random factor
    int	  	 	 rndSeed;

    int		 	 tubeSides;		// dimensions of tube
    int 	 	 tubeSegs;
    float  	 	 tubeTension;
    float  	 	 tubeSmooth;

    float	 	 gravity;		// affect of gravity 

    char	 	*contextIgnore;		// characters  to ignore
						// in context checking

    char	 	*initial;		// premise
    GU_Detail 		*meta;			// inside test geometry
    GU_Detail	 	*gdp;			// where to add geometry


    void		clearRules();		// rules
    void		addRule(const char *);

    void		clearVariables();	// variables
    void		addVariable(const char *var, float val);


    // callback methods

    float       	(*getChanValue)(int ind, void *data);	  // chan func
    GU_Detail		*(*getLeaf)(char ind, float,
				    float, float, void *data); // leaf func
    void		*callbackData;

    //  Construction methods 

    int	  	  	 preprocessStrings(void);
    int	  	  	 executeLSystem(int len);
    int		  	 runThrough;
    gu_LSysTurtleData	*buildGeometry(char *buildStr, float numGen, int len,
				       gu_LSysTurtleData *startState,
				       int offset);

    // Post-construction retrieval methods

    const char	 *getExpandedString() { return str; }	// result of lsystem
    int		  getMaxSize()		{ return strSize; }

    int		  getParam(int i, int j, float &value)
		  {
		    int result = 0;
		    if (i < LSYS_MAX_PARAMS)
		    {
			value = var[i][j];
			result = (value != LSYS_EXPR_UNUSED);
		    }
		    return result;
		  }


private:

    GU_LSysExpr	  expr;

    char	  *str;			// resultant lsystem string
    char	  *strOriginal;		// temp copy of above
    char	  *strTmp;		// temp copy of above

    char	  *ran;			// random # for each str char
    int 	  *age;			// generation of each str char

    float	  *var[LSYS_MAX_PARAMS];	// evaluated args

    int		   strSize;		// current length of max premise


    UT_Array<guLsysRule *> myRules;	// subsitution rules


    GA_ATIBlindData *myPtInfoRef;	// pt. attribute ref
    GA_ATIBlindData *myPrimInfoRef;	// prim attribute ref
    GA_RWHandleV3 myClrHandle;	// clr attribute handle

    int		  numberOfRules;
    int		  isError;
    int		  forceTubeNmls;

    GU_Detail	 *skelgdp;

    UT_Array<gu_LSysTurtleData> *myStack;
    UT_Array<GEO_PrimPoly *> myPolyStack;

    unsigned		 myCurrSeed;

    UT_Vector3	  myLastBezierLocation;

    int		  startBuild(void);
    void	  stopBuild(void);
    int	  	  substituteRule(unsigned int p, int i, int slen, 
				 int *prune, int &len);

    void	  assignParameters(char *module, unsigned int p, 
				   GU_LSysExpr &expr);

    int		  increaseLSysSize(void);

    void	  addPointWidthAttribute();

    void	  expandBranches(int);
    int		  smoothBranches();
    UT_Matrix4	  bezier3D(GEO_Face *,float,float &, float &, UT_Vector3 &);

    void	  lsysPrune(int, int &);
    void	  lsysTurn(float);
    void	  lsysRandom(char , float );
    void	  lsysPitch(float);
    void	  lsysRoll(float);
    void	  lsysGravity(float);
    void	  lsysLookToSun(int, float, float, float);
    void	  lsysForward(float, float, int, int, int, int);
    void	  lsysLeaf(float, float, float, float, char, int);

    void	  lsysPushPoly(void);
    void	  lsysPopPoly(void);
    void	  lsysPolyVertex(void);

    void	  	 lsysSetGroup(int num);
    GA_PrimitiveGroup  *lsysLeafGroup(char c);

    gu_LSysTurtleData	*lsysLocation(int p, int offset, int &branch, 
				      float g, gu_LSysTurtleData *startState);
    gu_LSysTurtleData	*lsysPopTurtle(void);
    gu_LSysTurtleData	*lsysPushTurtle(void);

    void	  lsysAddAttribute(int atrname, float v1, float v2, float v3);


    void	  addPoint(gu_LSysTurtleData *, float, float, int, int, int);
    void  	  startLine(float, int, int, int);

};

#endif

