/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_PixelFunctions.h
 *
 * COMMENTS:
 *	Defines the base class for simple pixel functions (ie, functions that
 * depend only on the value of the pixel being operated on; bright, gamma, etc,
 * not blurs or transforms)
 */

#ifndef RU_PixelFunction_h
#define RU_PixelFunction_h

#include "RU_API.h"
#include <UT/UT_ComputeGPU.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Lock.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>

class RU_PixelFunction;

typedef float (*RUPixelFunc)(RU_PixelFunction *, float, int);
typedef void  (*RUVectorFunc)(RU_PixelFunction *, float **, const bool *);

class RU_API RU_PixelFunction
{
public:
			 RU_PixelFunction() : myNextFunc(0), myBuildLock(0L)
		{ myScope[0] = myScope[1] = myScope[2] = myScope[3] = true; }
    virtual		~RU_PixelFunction();

    void		 setScope(bool *scope);
    bool		 isScoped(int i) const { return myScope[i]; }
    const bool		*getScope() const { return myScope; }
    bool		 isPartialScope() const;
    
    // returns true if different components have different function outcomes.
    bool		 isComponentDependent() const;

    // returns true if all components must be computed at once.
    bool		 areAllComponentsNeeded() const;

    void		 buildPixelEngine();
    void		 buildVectorEngine();
    void		 buildShaderEngine(UT_ComputeGPU *gpgpu);
    
    // this function runs the entire linked list of functions on val. Normally
    // you will want to call this from the head of the list.
    inline float	 processValue(float val, int comp) const
	{
	    for(int i= 0; i<myFunctions.entries(); i++)
		if(myParms(i)->isScoped(comp))
		    val = myFunctions(i)( myParms(i), val, comp );
	    return val;
	}
    
    inline void		 processValues(float **vals) const
	{
	    for(int i= 0; i<myVectorFunctions.entries(); i++)
	    {
		myVectorFunctions(i)( myParms(i), vals,myParms(i)->getScope());
	    }
	}

    const char		*getShaderCode() const   { return myShaderCode; }
    const char		*getFunctionCode() const { return myFunctionCode; }
    const char		*getLocals() const	 { return myLocalVars; }
    bool		 usesNumCompsGlobal() const { return myUseNumComps;}

    void		 setNext(RU_PixelFunction *next) { myNextFunc = next; }
    RU_PixelFunction	*next() const			 { return myNextFunc; }

    void		 setNumComps(int comps) { myNumComps = comps; }
    
protected:
    static float	 defaultPixelFunction(RU_PixelFunction *func,
					       float val, int comp);
    static void		 defaultVectorFunction(RU_PixelFunction *func,
					       float **vals,const bool *scope);
    
    // most functions can be done piece-wise. HSV correct, ie, cannot - it
    // needs a chunk of 3.
    virtual bool	 needAllComponents() const { return false; }

    // each component different?
    virtual bool	 eachComponentDifferent() const { return false; }

    // Override this to return the function to modify the actual pixel.
    virtual RUPixelFunc		getPixelFunction() const;

    // Override this to return the function to modify the pixel instead of
    // getPixelFunction if needAllComponents() returns true.
    virtual RUVectorFunc	getVectorFunction() const;

    virtual void		getPixelShader(UT_String &frag_shader);

    void			addConstant(const char *name,
					    UT_GPUType type, int vectorsize,
					    const void *data,
					    int array_size = 1);
    void			addLocal(const char *name,
					 UT_GPUType type,
					 int vectorsize);
    void			addArray(const char *name,
					 UT_GPUType type,
					 int vectorsize,
					 int width,
					 int height,
					 const void *data);
    void			addArray(const char *name,
					 UT_GPUType type,
					 int vectorsize,
					 int width,
					 int height,
					 const void *data1,
					 const void *data2,
					 const void *data3,
					 const void *data4);

    // function_name should be a unique name to the shader; append or prepend
    // the pixel function name to it. 'code' still needs to include this name
    // in the declaration.
    void			addFunction(const char *function_name,
					    const char *code);

    // call if you need to access a global variable. Variables are:
    //	num_comps - number of components being processed (1-4)
    void			usesGlobal(const char *name);

    int				getFunctionIndex() const
				{ return myHead->myFunctionIndex; }
    int				getNumComps() const
				{ return myHead->myNumComps; }
    
private:    
    RU_PixelFunction		       *myNextFunc;

    UT_Lock				myBuildLock;
    UT_ValArray<RUPixelFunc>		myFunctions;
    UT_ValArray<RUVectorFunc>		myVectorFunctions;
    UT_ValArray<RU_PixelFunction *>	myParms;

    mutable RUPixelFunc			myVectorizedFunction;
    bool				myScope[4];

    // For GPU pixel shaders
    RU_PixelFunction		       *myHead;
    UT_String				myShaderCode, myFunctionCode;
    UT_String				myLocalVars;
    UT_String				myParmCall;
    UT_String				myParmDecl;
    UT_String				myLocals;
    UT_StringArray			myUserFunctions;
    UT_String				myUserFunctionCode;
    UT_ComputeGPU		       *myGPU;
    int					myFunctionIndex;
    int					myNumComps;
    bool				myUseNumComps;

    friend class RU_ColorCurve;
};

#endif
