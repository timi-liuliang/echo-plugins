/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_GeometryVex.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_GeometryVex__
#define __GAS_GeometryVex__

#include "SIMZ_API.h"

#include <UT/UT_WeakPtr.h>
#include <UT/UT_SharedPtr.h>

#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>
#include <SIM/SIM_RawIndexField.h>
#include <SIM/SIM_PhysicalParms.h>

#include <VEX/VEX_GeoCommand.h>
#include <VEX/VEX_VexResolver.h>

#include <CVEX/CVEX_Context.h>
#include <CVEX/CVEX_Value.h>

#include <GVEX/GVEX_Execute.h>

#include <GAS/GAS_SubSolver.h>

#include <SIM/SIM_Query.h>
#include <SIM/SIM_QueryCombine.h>
#include "GAS_GeometryVexParms.proto.h"

class SIM_GeometryCopy;

class GVEX_GeoBlockData;
class GU_VexGeoInputs;
class GA_ElementGroup;

class OP_Caller;

class gas_SimpleGroupCache;

class SIMZ_API gas_GeometryVexCache
{
public:
    gas_GeometryVexCache()
    {
	myScriptParsed = false;
	myGroupParsed = false;
	mySimpleGroupCache.reset();
	myCurrentNodeId = -1;
	myVexNodeTimestamp = -1;
	myVexNodeId = -1;
	myDontRecomputeNormals = false;
	myComputeNormals = false;
    }
    ~gas_GeometryVexCache();
    void			clearBlocks();

    GAS_GeometryVexParms	myParam;
    
    int				myCurrentNodeId;

    // We have to hold the script
    bool			myScriptParsed;
    UT_String			myScript;
    UT_WorkArgs			myScriptArgs;

    // Parsed group.
    bool			myGroupParsed;
    bool			myGroupSimple;
    UT_StringArray		myGroupNames;
    UT_WeakPtr<gas_SimpleGroupCache>	mySimpleGroupCache;

    // Our cached bindings.  Provided our new detail
    // has a matching set of attributes we assume our
    // bindings will match.
    UT_UniquePtr<GVEX_AttribHash>	myAttribHash;
    UT_UniquePtr<GVEX_AttribHash>	myDetailAttribHash;

    UT_Array<GVEX_GeometryMap>	myGeoMap;
    UT_Array<GVEX_GeoBlockData *> myBlocks;

    int				myVexNodeTimestamp;
    int				myVexNodeId;

    // Tracks if we have an implicit normal that needs
    // recomputing every time.
    bool			myComputeNormals;
    // Tracks if we potentially need to recompute normals due
    // to deforming P, but have determined we shouldn't.
    bool			myDontRecomputeNormals;
};


class SIMZ_API GAS_GeometryVex : public GAS_SubSolver
{
public:
    GA_AttributeOwner	 targetClass() const { return (GA_AttributeOwner) param().getAttribClass(); }

    GAS_GeometryVexParms	&param() { return myParms; }
    const GAS_GeometryVexParms	&param() const { return myParms; }

protected:
    explicit		 GAS_GeometryVex(const SIM_DataFactory *factory);
    virtual		~GAS_GeometryVex();

    /// Creates a SIM_QueryArrays object to treat impact as a record
    virtual SIM_Query	*createQueryObjectSubclass() const
    {
	return new SIM_QueryCombine(BaseClass::createQueryObjectSubclass(), param().createQueryObject(this));
    }
    virtual void makeEqualSubclass(const SIM_Data *source)
    {
	BaseClass::makeEqualSubclass(source);
	const GAS_GeometryVex *src = SIM_DATA_CASTCONST(source, GAS_GeometryVex);
	if (src)
	{
	    param() = src->param();

	    // We steal forward in time any caches.
	    if (myCache) delete myCache;
	    myCache = src->myCache;
	    SYSconst_cast(src)->myCache = 0;
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

    bool		 shouldMultiThread(SIM_RawField *field) const 
			 { return field->field()->numTiles() > 1; }

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    bool		 runVexOnGeometry(SIM_Engine &engine, SIM_Object *obj, SIM_GeometryCopy *geo, const char *geoname);

    void 		 executeVex(SIM_Object *obj, GU_Detail *gdp, 
				int argc, const char *const *argv, 
				OP_Caller &caller,
				GU_VexGeoInputs *geoinputs,
				const UT_Array<UT_Matrix4D> *inputxform,
				const UT_IntArray *inputxformid) const;

    THREADED_METHOD1_CONST(GAS_GeometryVex, parms.multithread, processVexGeometry,
		    GVEX_GeometryParms &, parms)
    void		 processVexGeometryPartial(GVEX_GeometryParms &parms,
			    const UT_JobInfo &info) const;

    bool		 processVexBlock(
				const GU_Detail *gdp,
				GVEX_GeoBlockData &data,
				CVEX_Context &context,
				int argc, const char *const*argv,
				const UT_Array<GVEX_GeometryMap> &geomap,
				int num, GA_Offset *elementoffsets,
				bool triviallist,
                                fpreal t,
				const UT_JobInfo &info) const;

    void		 addInputName(CVEX_Context &context, int idx, CVEX_StringArray &inputnames) const;
private:
    static const SIM_DopDescription	*getDopDescription();

    fpreal		 myTime, mySopTime, myTimeInc;
    fpreal		 mySimTime, mySimFrame;

    GAS_GeometryVexParms	myParms;
    gas_GeometryVexCache	*myCache;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_GeometryVex,
			GAS_SubSolver,
			"Gas Geometry Vex",
			getDopDescription());
};

#endif
