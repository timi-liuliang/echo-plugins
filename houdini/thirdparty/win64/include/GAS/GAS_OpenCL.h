/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_OpenCL.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_OpenCL_h__
#define __GAS_OpenCL_h__

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <GA/GA_SplittableRange.h>
#include <CE/CE_Context.h>
#include <SIM/SIM_Query.h>
#include <SIM/SIM_QueryCombine.h>
#include "GAS_OpenCLParms.proto.h"

class GA_CEAttribute;

class GAS_API GAS_OpenCL : public GAS_SubSolver
{
public:
    GAS_OpenCLParms		&param() { return myParms; }
    const GAS_OpenCLParms	&param() const { return myParms; }

// static call for retrieving values
    static bool  getParmB(OP_Node * me, const char * str, int idx, fpreal t );
    static int	 getParmI(OP_Node * me, const char * str, int idx, fpreal t );
    static void  getParmS(OP_Node * me, const char * str, int idx, fpreal t,
						UT_String &ret );

// Generate a kernel argument signature for the selected parameters
    static void getArgumentSign( OP_Node * me, int param, int maxparm, fpreal t,
						UT_WorkBuffer &sign,
						UT_String &firstwriteable
						);

// Generate kernel
    static void generateKernel( OP_Node * me, fpreal t, UT_WorkBuffer &str );

// Static display callback
    static int displayKernel(void * data, int, fpreal t, const PRM_Template*);

protected:
    explicit   GAS_OpenCL(const SIM_DataFactory *factory);
    virtual  ~GAS_OpenCL();

    /// Creates a SIM_QueryArrays object to treat impact as a record
    virtual SIM_Query	*createQueryObjectSubclass() const
    {
	return new SIM_QueryCombine(BaseClass::createQueryObjectSubclass(), param().createQueryObject(this));
    }
    virtual void makeEqualSubclass(const SIM_Data *source)
    {
	BaseClass::makeEqualSubclass(source);
	const GAS_OpenCL *src = SIM_DATA_CASTCONST(source, GAS_OpenCL);
	if (src)
	{
	    param() = src->param();
	}
    }
    virtual void	 saveSubclass(std::ostream &os) const
    {
	BaseClass::saveSubclass(os);
	myParms.save(os);
    } 
    virtual bool	 loadSubclass(UT_IStream &is)
    {
	if (!BaseClass::loadSubclass(is))
	    return false;
	if (!myParms.load(is))
	    return false;
	return true;
    }
    /// The overloaded callback that GAS_SubSolver will invoke to
    /// perform our actual computation.  We are giving a single object
    /// at a time to work on.
    virtual bool  solveGasSubclass(SIM_Engine &engine,
				   SIM_Object *obj,
				   SIM_Time time,
				   SIM_Time timestep);


private:
    GAS_OpenCLParms		myParms;

    /// We define this to be a DOP_Auto node which means we do not
    /// need to implement a DOP_Node derivative for this data.	Instead,
    /// this description is used to define the interface.
    static const SIM_DopDescription *getDopDescription();

#ifdef CE_ENABLED

    // forces alignment of CE_Grids
    void forceAlign( CE_Grid *&ret, SIM_RawField *src, const SIM_RawField *ex );


    bool setAndExecuteKernel(cl::Kernel  &kernel, const UT_Array<UT_Array< CE_Grid *> > &fields,
			     cl::NDRange global, 
			     cl::NDRange local,
			     SIM_Object  *obj,
			     fpreal time,
			     fpreal timestep);

    bool setAndExecuteKernel(cl::Kernel  &kernel, const UT_Array<UT_Array< CE_Grid *> > &fields,
			     const UT_Array<cl::NDRange> &offsets,
			     const UT_Int32Array &sizes,
			     const UT_Array<cl::NDRange> &globals,
			     const UT_Array<cl::NDRange> &locals,
			     SIM_Object  *obj,
			     fpreal time,
			     fpreal timestep,
			     int singleworkitems = 0);

    bool setKernelArguments(cl::Kernel &kernel, int paramcount,
			    const UT_Array<UT_Array< CE_Grid *> > &grids,
			    SIM_Object * obj,
			    UT_Array<cl::Buffer> &buffers,
			    fpreal time, fpreal timestep,
			    int singleworkitems = 0);

    bool setKernelArgument( cl::Kernel &kernel, int i, int &argidx,
			    const UT_Array<UT_Array< CE_Grid *> > &grids,
			    SIM_Object *obj,
			    UT_Array<cl::Buffer> &buffers,
			    fpreal timestep,
			    UT_StringHolder &geoname,
			    const SIM_Geometry *&geo,
			    SIM_GeometryCopy *&geocopy);

    void createOptionalAttributeOptions( SIM_Object *obj, UT_WorkBuffer &options );

    // Returns the number of workitems required to run the kernel in worksets mode
    // in a single workgroup, else zero if not possible,
    int maxSingleWorkgroupItems( SIM_Object *obj, cl::Kernel &kernel);

    // Main Kernel Execution versions

    bool executeOpenCLKernel(cl::Kernel &kernel, SIM_Object * obj,
			     fpreal time, fpreal timestep,
			     int singleworkitems);

    // Kernel Execution versions
    bool executeOpenCLKernelUnaligned(cl::Kernel &kernel, SIM_Object * obj, fpreal time, fpreal timestep);
    bool executeOpenCLKernelAligned(  cl::Kernel &kernel, SIM_Object * obj, fpreal time, fpreal timestep);
    bool executeOpenCLKernelAlignedSingleAlign( cl::Kernel &kernel,
					 	SIM_Object * obj, fpreal time, fpreal timestep);
    bool executeOpenCLKernelAlignedMultiAlign(  cl::Kernel &kernel,
						SIM_Object * obj, fpreal time, fpreal timestep);
    bool executeOpenCLKernelGeometry(cl::Kernel &kernel, SIM_Object * obj, fpreal time, fpreal timestep,
				     int singleworkitems);

#endif
    /// These macros are necessary to bind our node to the factory and
    /// ensure useful constants like BaseClass are defined.
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_OpenCL,
			GAS_SubSolver,
			"Gas OpenCL",
			getDopDescription());
};

#endif

