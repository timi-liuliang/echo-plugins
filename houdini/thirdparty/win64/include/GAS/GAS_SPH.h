/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SPH.h ( GSA Library, C++)
 *
 * COMMENTS:
 *	Defines a spherical particle hydronymics search structure.
 *	This provides a toolbox of things to do with SPH weighting functions.
 */

#ifndef __GAS_SPH__
#define __GAS_SPH__

#include "GAS_API.h"

#include <GA/GA_Handle.h>

#include <UT/UT_FloatArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_Vector3Array.h>
#include <UT/UT_HashGrid.h>

class GAS_SubSolver;

class GA_PointGroup;

class GEO_PointTree;

class GU_Detail;
class GU_NeighbourList;

class SIM_Engine;

class UT_String;

class GAS_API GAS_SPH
{
public:
	 GAS_SPH();
	~GAS_SPH();

    enum sphWeight
    {
	POLY6,
	SPIKY,
	VISCOSITY,
	SPIKY2,
	SPIKY5,
	POLY6_UNSCALED,
	SPIKY_UNSCALED
    };

    typedef UT_Array<GA_Offset> gas_PointList;

    // A static function which builds an appropriate warning message
    // for missing particle fluid attributes
    static void	missingAttribWarning(SIM_Engine &engine, GAS_SubSolver *solver,
    				const UT_String &objName,
				const UT_String &geometryName,
				const UT_String &attribName);

    // Returns false if initalization failed, due to missing attributes,
    // etc.  If fullinit == true, then we initialize cells by adding the
    // indices of all points in the cell and all points in neighbour cells.
    // If not, then on each query we have to examine multiple neighbour cells.
    // radiusScale is used to force an increase of the radius size used for
    // density queries and such.  needVolume indicates whether or not mass
    // and density attributes are required.  They are not need, for intance,
    // when surfacing a particle fluid.
    bool	initialize(const GU_Detail *gdp, bool fullinit = true,
			const GA_PointGroup *excludegroup = NULL,
			fpreal radiusScale = 1.0, bool needVolume = true);

    // Builds the neighbour list, required for the accessors that
    // take a neighbour list to run.
    // This is independent of initialize!
    bool	buildNeighbours(const GU_Detail *gdp,
			    fpreal radiusScale = 1.0,
			    bool needVolume = true);

    // Borrows the provided neighbour list and uses it for all lookups
    bool	setNeighbours(const GU_Detail *gdp, GU_NeighbourList *list,
			    fpreal radScale = 1.0,
			    bool needVolume = true);


    // Perform a simple sampling operation.
    // The color variable, if present, will be set to the total weight
    // of the kernels without taking the attribute into account.
    // This is useful for efficiently calculating (pi+pj)/2, for example,
    // as the constant term can be pulled out and multiplied by the color.
    fpreal	sampleF(const UT_Vector3 &p, 
			const GA_ROHandleF &attrib,
			sphWeight wfunc,
			fpreal *color = 0,
			bool normalize = false,
			bool volumeScale = true) const;
    fpreal	sampleF(const UT_Vector3 &p,
    			const gas_PointList &point_list,
			const GA_ROHandleF &attrib,
			sphWeight wfunc,
			fpreal *color = 0,
			bool normalize = false,
			bool volumeScale = true) const;

    fpreal	sampleF(const UT_Vector3 &p,
			UT_FloatArray &floats,
			sphWeight wfunc,
			bool volumescale = true,
			bool normalize = false) const;
    fpreal	sampleF(const UT_Vector3 &p,
    			const gas_PointList &point_list,
			UT_FloatArray &floats,
			sphWeight wfunc,
			bool volumescale = true,
			bool normalize = false) const;

    UT_Vector3	sampleV3(const UT_Vector3 &p,
			 const GA_ROHandleV3 &attrib,
			 sphWeight wfunc,
			 fpreal *color = 0,
			 bool normalize = false) const;
    UT_Vector3	sampleV3(const UT_Vector3 &p,
    			 const gas_PointList &point_list,
			 const GA_ROHandleV3 &attrib,
			 sphWeight wfunc,
			 fpreal *color = 0,
			 bool normalize = false) const;

    // A threadsafe velocity sampling function.  This assumes that
    // initVelocityData has been invoked on this sph object.
    // excludePoint is a point to exclude from the velocity
    // calculation (in case we want to sample the velocity around
    // a point, but ignore the contribution of that point).
    UT_Vector3	sampleVelocity(const UT_Vector3 &p,
			sphWeight wfunc,
			int excludePoint = -1) const;
    UT_Vector3	sampleVelocity(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			sphWeight wfunc,
			int excludePoint = -1) const;

    // Find the XSPH velocity for the given point.  We take "velocities"
    // in an input array in order to support a more general notion
    // of velocity.
    UT_Vector3 xsphVelocity(int ptnum, sphWeight wfunc,
    			const UT_Vector3Array &velocities,
			fpreal xsph_constant) const;

    // Calculates the density field at a point.  This is special
    // because it ignores particle density and just sums the mass
    // weighting...
    fpreal	sampleDensity(const UT_Vector3 &p,
			sphWeight wfunc,
			const bool massScale = true) const;
    fpreal	sampleDensity(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			sphWeight wfunc,
			const bool massScale = true) const;

    // A convenience function which samples two densities (using the
    // two given kernels) 
    void	 sampleDoubleDensity(const UT_Vector3 &p,
    			 const gas_PointList &ptlist,
			 UT_FloatArray &rad2list,
			 sphWeight wfunc1,
			 sphWeight wfunc2,
			 fpreal &density1,
			 fpreal &density2,
			 const bool massScale = true) const;

    // Calculate the surface density at a point using the given
    // surface distance attribute.  This uses the surface definition
    // from the "Adaptively Sampled Particle Fluids" paper.
    fpreal	sampleSurfaceDensity(const UT_Vector3 &p,
			const GA_ROHandleF &distanceattrib) const;
    fpreal	sampleSurfaceDensity(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			const GA_ROHandleF &distanceattrib) const;

    // This function is similar to the one above, but uses a fixed
    // distance from each particle.  If distanceModifier is true then
    // the given surface distance is just treated as a multiplier of
    // each particles interaction radius.
    fpreal	sampleSurfaceDensity(const UT_Vector3 &p,
			fpreal surfaceDistance,
			const bool distanceModifier = false) const;
    fpreal	sampleSurfaceDensity(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			fpreal surfaceDistance,
			const bool distanceModifier = false) const;

    // Calculate the gradient of the surface density field at
    // the given point.
    UT_Vector3	sampleSurfaceDensityGrad(const UT_Vector3 &p,
			const GA_ROHandleF &distanceattrib) const;
    UT_Vector3	sampleSurfaceDensityGrad(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			const GA_ROHandleF &distanceattrib) const;

    // This function is similar to the one above, but since a fixed
    // distance from each particle is used, the vector ends up being
    // somewhat different.
    UT_Vector3	sampleSurfaceDensityGrad(const UT_Vector3 &p) const;
    UT_Vector3	sampleSurfaceDensityGrad(const UT_Vector3 &p,
    			const gas_PointList &ptlist) const;

    // Computes the color field - 1 inside the area handled by
    // particles, 0 outside.  This uses an implied attribute that
    // is 1 wherever particles are and zero everywhere else.
    fpreal	sampleColor(const UT_Vector3 &p,
			sphWeight wfunc) const;
    fpreal	sampleColor(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			sphWeight wfunc) const;

    // Functions to compute the gradient and laplacian of the
    // color field.  Used in the calculation of surface tension
    // forces.
    UT_Vector3	colorGradient(const UT_Vector3 &p,
	    		sphWeight wfunc) const;
    UT_Vector3	colorGradient(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
	    		sphWeight wfunc) const;

    // Computes an unscaled color gradient (eg. no scaling by
    // particle size, density, mass, etc.)
    UT_Vector3	colorGradientUnscaled(const UT_Vector3 &p,
			bool scale = false) const;
    UT_Vector3	colorGradientUnscaled(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			bool scale = false) const;

    // Laplacian of the color field
    fpreal	colorLaplacian(const UT_Vector3 &p,
			sphWeight wfunc,
			const GA_ROHandleR &scaleattrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;
    fpreal	colorLaplacian(const UT_Vector3 &p,
			const gas_PointList &ptlist,
			sphWeight wfunc,
			const GA_ROHandleR &scaleattrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;

    // Gradient of a given attribute field
    UT_Vector3	gradient(const UT_Vector3 &p,
			const GA_ROHandleR &attrib,
			sphWeight wfunc,
			UT_Vector3 *color = 0,
			bool debugprint = false,
			const GA_ROHandleR &scaleattrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;
    UT_Vector3	gradient(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			const GA_ROHandleR &attrib,
			sphWeight wfunc,
			UT_Vector3 *color = 0,
			bool debugprint = false,
			const GA_ROHandleR &scaleattrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;

    // This function takes the gradient of a Vector3 attribute
    // along the given axis.
    UT_Vector3	gradient(const UT_Vector3 &p,
			const GA_ROHandleV3 &attrib,
			sphWeight wfunc,
			int axis, UT_Vector3 *color = 0,
			bool normalize = false) const;
    UT_Vector3	gradient(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			const GA_ROHandleV3 &attrib,
			sphWeight wfunc,
			int axis, UT_Vector3 *color = 0,
			bool normalize = false) const;

    // Samples the laplacian of a float[1] attribute.
    fpreal	laplacian(const UT_Vector3 &p,
			const GA_ROHandleR &attrib,
			sphWeight wfunc,
			fpreal *color = 0) const;
    fpreal	laplacian(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			const GA_ROHandleR &attrib,
			sphWeight wfunc,
			fpreal *color = 0) const;

    // Samples the laplacian of a float[n] attribute
    // where 1 <= n <= 4.
    UT_FloatArray laplacianF(const UT_Vector3 &p,
			const GA_ROHandleR &attrib,
			sphWeight wfunc, int size,
			fpreal *color = 0) const;
    UT_FloatArray laplacianF(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			const GA_ROHandleR &attrib,
			sphWeight wfunc, int size,
			fpreal *color = 0) const;

    UT_Vector3	laplacianV3(const UT_Vector3 &p,
			const GA_ROHandleV3 &attrib,
			sphWeight wfunc,
			fpreal *color = 0,
			const GA_ROHandleR &scale_attrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;
    UT_Vector3	laplacianV3(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
			const GA_ROHandleV3 &attrib,
			sphWeight wfunc,
			fpreal *color = 0,
			const GA_ROHandleR &scale_attrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;

    // Computes a damping term from the particle field, as
    // in [Becker and Teschner, 07].
    // gah can be any vector3 field, but is presumably velocity.
    UT_Vector3	dampingValue(const UT_Vector3 &p,
    			const GA_ROHandleV3 &attrib,
    			sphWeight wfunc,
			UT_Vector3 &point_velocity,
			fpreal point_density,
			const GA_ROHandleR &scale_attrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;
    UT_Vector3	dampingValue(const UT_Vector3 &p,
    			const gas_PointList &ptlist,
    			const GA_ROHandleV3 &attrib,
    			sphWeight wfunc,
			UT_Vector3 &point_velocity,
			fpreal point_density,
			const GA_ROHandleR &scale_attrib = GA_ROHandleR(),
			fpreal point_scale = 1.0) const;

    /// Returns all points close to the given point.
    /// Does a search using the points current positions
    void	findClosePoints(const UT_Vector3 &p,
			gas_PointList &ptlist) const;

    /// Finds all points close to the given point number.
    /// If the neighbourlist is built, that will be retrieved directly
    /// If it hasn't been built, the point's position will be searched
    /// using findClosePoints(Vector)
    void	findClosePoints(int ptidx,
			gas_PointList &ptlist) const;

    // Initializes information to allow fast velocity queries to
    // be made to this structure (without having to pass an
    // attribute handle in).  Also has the option of initializing
    // a KD tree of the points in the structure's gdp.  The purpose
    // of this is so that if no close points can be found during
    // a velocity query, the velocity of the nearest neighbour can
    // be taken instead.
    bool	initVelocityData(bool initKD = false);

    // Get the width of a particle cell.
    fpreal	getCellWidth() const { return myCellWidth; }

    UT_HashGrid<GA_Offset> *getGrid() const { return myGrid; }

protected:
    // These take the radius squared and determine the weight 
    // or gradient of the weight for the given function.
    // h is the radius of the kernel to sample.
    fpreal64	weight(fpreal64 r2, fpreal64 h, sphWeight wfunc) const;
    fpreal64	dweight(fpreal64 r, fpreal64 h, sphWeight wfunc) const;
    fpreal64	d2weight(fpreal64 r, fpreal64 h, sphWeight wfunc) const;
    fpreal64	scaleddweight(fpreal64 r, fpreal64 h, sphWeight wfunc) const;

    UT_HashGrid<GA_Offset>	*myGrid;

    fpreal		 myCellWidth;
    fpreal		 myCellWidth2;

    const GU_Detail	*myGdp;

    GA_ROHandleF myRadAttrib;
    GA_ROHandleF myMassAttrib;
    GA_ROHandleF myDensityAttrib;

    // We can optionally store a list of the mass, density and
    // velocity at each point in our geometry.  We do this to
    // allow multithreaded sampling of the same sph structure.
    UT_FloatArray	 myMasses;
    UT_FloatArray	 myDensities;
    UT_FloatArray	 myRadii;
    UT_Vector3Array	 myVelocities;

    // Also store a point tree to be used for velocity evaluations.
    // We need this so that a velocity can be computed regardless of
    // the given point in space.
    GEO_PointTree	*myPointTree;

    // Radius scale used for weight queries
    fpreal		 myRadiusScale;

    // A data structure storing lists of point neighbours for
    // fast lookup.
    GU_NeighbourList	*myNeighbourList;
    bool		 myOwnNeighbours;
};

#endif

