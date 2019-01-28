/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_VectorField.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_VectorField__
#define __SIM_VectorField__

#include "SIM_API.h"

#include <UT/UT_VoxelArray.h>
#include <UT/UT_DMatrix3.h>
#include <UT/UT_ThreadedAlgorithm.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"
#include "SIM_RawField.h"

class UT_IStream;
class SIM_Geometry;
class SIM_RawField;
class SIM_ScalarField;
class SIM_MatrixField;
class SIM_IndexField;
class GEO_PrimVDB;

/// This class holds a three dimensional vector field.  
class SIM_API SIM_VectorField : public SIM_Data,
			public SIM_OptionsUser
{
public:
    typedef SIM_RawField	 rawfield_type;

    /// Accesses the relative path to the position data associated with
    /// this geometry.
    GETSET_DATA_FUNCS_S(SIM_NAME_POSITIONPATH, PositionPath);

    /// Control the number of divisions.
    GETSET_DATA_FUNCS_I(SIM_NAME_UNIFORMVOXELS, UniformVoxels);
    GETSET_DATA_FUNCS_B(SIM_NAME_TWOD, TwoDField);
    GETSET_DATA_FUNCS_I(SIM_NAME_VOXELPLANE, VoxelPlane);
    GETSET_DATA_FUNCS_V3(SIM_NAME_DIV, RawDivisions);
    GETSET_DATA_FUNCS_I("uniformdiv", RawUniformDivisions);
    GETSET_DATA_FUNCS_F("divsize", RawDivisionSize);
    GETSET_DATA_FUNCS_V3(SIM_NAME_CENTER, RawCenter);
    GETSET_DATA_FUNCS_V3(SIM_NAME_SIZE, RawSize);

    GETSET_DATA_FUNCS_V3("slicediv", SliceDivisions);
    GETSET_DATA_FUNCS_V3("sliceoverlapneg", SliceOverlapNeg);
    GETSET_DATA_FUNCS_V3("sliceoverlappos", SliceOverlapPos);
    GETSET_DATA_FUNCS_I("slice", Slice)
    exint	getNumSlices() const { UT_Vector3D nslice = getSliceDivisions(); return exint(nslice.x() * nslice.y() * nslice.z()); }

    GETSET_DATA_FUNCS_I("totalvoxels", TotalVoxels);
    GETSET_DATA_FUNCS_V3("totalvoxelres", TotalVoxelRes);

    GETSET_DATA_FUNCS_I(SIM_NAME_VOXELSAMPLE, VoxelSampleRaw);
    GETSET_DATA_FUNCS_B("closedends", ClosedEnds);
    GETSET_DATA_FUNCS_B("closexneg", CloseXNeg);
    GETSET_DATA_FUNCS_B("closeyneg", CloseYNeg);
    GETSET_DATA_FUNCS_B("closezneg", CloseZNeg);
    GETSET_DATA_FUNCS_B("closexpos", CloseXPos);
    GETSET_DATA_FUNCS_B("closeypos", CloseYPos);
    GETSET_DATA_FUNCS_B("closezpos", CloseZPos);

    GETSET_DATA_FUNCS_V3(SIM_NAME_DIRECTION, ExternalDirection);
    GETSET_DATA_FUNCS_F(SIM_NAME_TOLERANCE, Tolerance);
    GETSET_DATA_FUNCS_B("usefp16", UseFP16);
    GETSET_DATA_FUNCS_I("border", RawBorder);
    UT_VoxelBorderType	getBorder() const { return (UT_VoxelBorderType) getRawBorder(); }
    void		setBorder(UT_VoxelBorderType border) { setRawBorder(border); }

    /// Controls the dimensions of where the field is properly defined
    /// in the field space.
    void		 getBBox(UT_BoundingBox &bbox) const;

    UT_Vector3		 getOrig() const
			 {
			     return getCenter() - getSize()/2;
			 }

    /// Calculate the size and divisions according to options
    /// such as 2d or equal sized voxels.
    UT_Vector3		 getDivisions() const;
    UT_Vector3		 getSize() const;
    UT_Vector3		 getCenter() const;

    /// Adjusts the size/divisions of this field, overriding
    /// and twod or uniform voxel settings.
    void		 setDivisions(const UT_Vector3 &div);
    void		 setSize(const UT_Vector3 &div);
    void		 setCenter(const UT_Vector3 &div);
    
    /// Resizes our field keeping our field data.
    /// The final size will be an integer number of voxels matching
    /// our current voxel size.  The final center will be an integer
    /// number of voxel offset from our current center.  This allows
    /// us to do a perfect copy of the data.
    void		 resizeKeepData(const UT_Vector3 &size, const UT_Vector3 &center, bool keepdata, const char *address = 0, int port = -1);

    /// Match this field to the given reference field.  We will
    /// end up with the same size/divisions/twod/uniform,
    /// but not the same sampling pattern
    void		 matchField(const SIM_ScalarField *field);
    void		 matchField(const SIM_VectorField *field, bool matchsample = false);
    void		 matchField(const SIM_MatrixField *field);
    void		 matchField(const SIM_IndexField *field);

    void		 matchVolume(const GEO_PrimVolume *vol, const UT_DMatrix4 &xform);
    void		 matchVDB(const GEO_PrimVDB *vdb, const UT_DMatrix4 &xform);

    bool		 isAligned(const SIM_ScalarField *field) const;
    bool		 isAligned(const SIM_MatrixField *field) const;
    /// True if we are component-wise aligned, our x/y/z fields
    /// may still be unaligned with respect to each other.
    bool		 isAligned(const SIM_VectorField *field) const;
    bool		 isAligned(const SIM_RawField *field) const;

    /// Determines if we match in terms of voxel cells - same bounding
    /// box and number of cells.  Due to sampling, this does not mean
    /// the sample points will match
    /// Because our internal fields are always matching by definition,
    /// we can just test the first field.
    bool		 isMatching(const SIM_VectorField *field) const
    { return getField(0)->isMatching(field->getField(0)); }

    bool		 isFaceSampled() const
    { return getVoxelSample(0) == SIM_SAMPLE_FACEX &&
	     getVoxelSample(1) == SIM_SAMPLE_FACEY &&
	     getVoxelSample(2) == SIM_SAMPLE_FACEZ; }

    bool		 isCenterSampled() const
    { return getVoxelSample(0) == SIM_SAMPLE_CENTER &&
	     getVoxelSample(1) == SIM_SAMPLE_CENTER &&
	     getVoxelSample(2) == SIM_SAMPLE_CENTER; }

    /// True if our internal fields are aligned.
    bool		 isSelfAligned() const
    { return getVoxelSample(0) == getVoxelSample(1) &&
	     getVoxelSample(1) == getVoxelSample(2); }


    SIM_FieldSample	 getVoxelSample(int axis) const;
    const UT_Vector3	&getVoxelSize(int axis) const { return myFields[axis]->getVoxelSize(); }
    fpreal		 getVoxelDiameter(int axis) const { return myFields[axis]->getVoxelDiameter(); }
    // Independent of sampling pattern, so constant between fields.
    UT_Vector3		 getVoxelSize() const { return getVoxelSize(0); }
    void		 setVoxelSize(const UT_Vector3 &voxelsize)
			 { myFields[0]->setVoxelSize(voxelsize);
			   myFields[1]->setVoxelSize(voxelsize);
			   myFields[2]->setVoxelSize(voxelsize);
			 }

    /// Access the field value given a world space location.
    /// This does trilinear interpolation.
    UT_Vector3		 getValue(const UT_Vector3 &pos) const;

    /// Computes the gradient of the vector field in world space..
    UT_DMatrix3		 getGradient(const UT_Vector3 &pos) const;

    /// Computes the curl at a given worldspace
    UT_Vector3		 getCurl(const UT_Vector3 &pos) const;

    /// Gets the velocity at the given *voxel* location, interpolating
    /// if we have corner or face velocities.
    UT_Vector3		 getCellValue(int x, int y, int z) const;

    /// Adds a velocity to the given *voxel*.  If this is face,
    /// it is divided in two and spread on each of 6 faces.  If it is
    /// corner, it is divided by 8 and spread along each of 8 corners.
    void		 addToCell(int x, int y, int z, const UT_Vector3 &dv);

    /// Treats this field as a velocity field and advects the given
    /// position for the given length of time.
    /// Uses first order explicit euler integration
    void		 advect(UT_Vector3 &pos, float time, float cfl = 1.0f) const
    {
	SIM_RawField::advect(pos, getXField(), getYField(), getZField(), time, 0, cfl);
    }

    /// Uses second order explicit runge-kutta integration.
    void		 advectMidpoint(UT_Vector3 &pos, float time, float cfl = 1.0f) const
    {
	SIM_RawField::advectMidpoint(pos, getXField(), getYField(), getZField(), time, 0, cfl);
    }

    /// Uses third order explicit runge-kutta integration.
    void		 advectRK3(UT_Vector3 &pos, float time, float cfl = 1.0f) const
    {
	SIM_RawField::advectRK3(pos, getXField(), getYField(), getZField(), time, 0, cfl);
    }

    /// Uses fourth order explicit runge-kutta integration.
    void		 advectRK4(UT_Vector3 &pos, float time, float cfl = 1.0f) const
    {
	SIM_RawField::advectRK4(pos, getXField(), getYField(), getZField(), time, 0, cfl);
    }

    /// Advects this field by the other given field.  Handles the possibility
    /// that the other field is this field.
    void		 advect(const SIM_VectorField *vel, float timestep,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);
    void		 advect(sim_PointVelocity getVelocity, float timestep,
				float voxelsize,
				const SIM_RawField *collision = 0,
				float cfl = 1.0f);

    /// Advects this by the velocity field, storing our min/max
    /// interpolants into the min/max fields
    void		 advectMinMax(SIM_VectorField *minfield,
				SIM_VectorField *maxfield,
				const SIM_VectorField *vel, float timestep,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);

    /// Projects the field into the space of non-divergent fields.
    /// All divergent components of this field are removed.
    void		 projectToNonDivergent(const SIM_RawField *pressureboundary = 0, const SIM_RawField *collision = 0, SIM_RawField *pressureout = 0, bool preservebubble = true, const SIM_RawField *goaldiv = 0, SIM_RawIndexField *compout = 0, bool ghostfluid = true, bool variational = true, SIM_RawField::PCG_METHOD pcgmethod = SIM_RawField::PCG_MIC, int numiterforcenter = 20);
    void		 projectToNonDivergentCenter(const SIM_RawField *pressureboundary, const SIM_RawField *goaldiv, int numiter);
    void		 projectToNonDivergentFace(const SIM_RawField *pressureboundary, const SIM_RawField *collision, SIM_RawField *pressureout = 0, bool preservebubble = true, const SIM_RawField *goaldiv = 0, SIM_RawIndexField *compout = 0, bool ghostfluid = true, bool variational = true, SIM_RawField::PCG_METHOD pcgmethod = SIM_RawField::PCG_MIC);

    /// Evaluates the divergence field for this velocity field.
    THREADED_METHOD1_CONST(SIM_VectorField, getField(0)->shouldMultiThread(),
		     buildDivergenceCenter,
		     SIM_RawField &, div)
    void		 buildDivergenceCenterPartial(SIM_RawField &div,
				    const UT_JobInfo &info) const;

    /// Evaluates the divergence field for this velocity field.
    THREADED_METHOD1_CONST(SIM_VectorField, getField(0)->shouldMultiThread(),
		     buildDivergenceCorner,
		     SIM_RawField &, div)
    void		 buildDivergenceCornerPartial(SIM_RawField &div,
				    const UT_JobInfo &info) const;

    /// Applies a pressure differential to the given component of our
    /// velocity field
    THREADED_METHOD5(SIM_VectorField, getField(axis)->shouldMultiThread(),
		     applyPressureGradientFace,
		     int, axis,
		     const SIM_RawField *, pressure,
		     const SIM_RawField *, surface,
		     const SIM_RawIndexField *, comp,
		     bool, pressureBoundaryAtFace)
    void		 applyPressureGradientFacePartial(int axis,
				    const SIM_RawField *pressure,
				    const SIM_RawField *surface,
				    const SIM_RawIndexField *comp,
                    bool  pressureBoundaryAtFace,
				    const UT_JobInfo &info);


    THREADED_METHOD1(SIM_VectorField, getField(0)->shouldMultiThread(),
		    applyPressureGradientCenter,
		    const SIM_RawField *, pressure)
    void		applyPressureGradientCenterPartial(
				const SIM_RawField *pressure,
				const UT_JobInfo &info);

    THREADED_METHOD1(SIM_VectorField, getField(0)->shouldMultiThread(),
		    applyPressureGradientCorner,
		    const SIM_RawField *, pressure)
    void		applyPressureGradientCornerPartial(
				const SIM_RawField *pressure,
				const UT_JobInfo &info);

    /// Distributes any divergence in the boundary condition among
    /// all voxels equally.
    void		 distributeBoundaryDivergenceToVolumeFace(SIM_RawField &div, const SIM_RawField *pressureboundary = 0, int compnum = -1, const SIM_RawIndexField *comp = 0);

    /// Determines if the given component has any surface cells.
    /// result should be preloaded with false.
    THREADED_METHOD6_CONST(SIM_VectorField, div.shouldMultiThread(),
		    hasSurfaceCell,
		    bool *, result,
		    SIM_RawField &, div, 
		    const SIM_RawField *, pressureboundary, 
		    const SIM_RawField *, collision, 
		    int, compnum, 
		    const SIM_RawIndexField &, comp)
    void	 hasSurfaceCellPartial(bool *result, SIM_RawField &div, const SIM_RawField *pressureboundary, const SIM_RawField *collision, int compnum, const SIM_RawIndexField &comp, const UT_JobInfo &info) const;

    /// Distributes any divergence in the boundary condition among
    /// all voxels that are on the boundary of the pressureboundary
    /// but not on the collision boundary.
    void		 distributeBoundaryDivergenceToSurfaceFace(SIM_RawField &div, const SIM_RawField *pressureboundary, const SIM_RawField *collision, int compnum, const UT_VoxelArray<int64> &comp);

    /// Enforces boundary conditions on the array.
    /// Boundary line should be given in world space
    void		 enforceBoundary(const SIM_ScalarField *collision=0,
			    const SIM_VectorField *cvel = 0,
                            const SIM_VectorField *bvel = 0, // Apply velocity to the SIM_BOUNDARY 
			    const SIM_BoundaryLine &worldbline = SIM_BoundaryLine());

    typedef UT_Vector3 (*ut_velocityCalc)(const UT_Vector3 &pos, void *vp);
    typedef void (*ut_physParmCalc)(const UT_Vector3 &pos, void *vp,
                                    fpreal &bounce,
                                    fpreal &friction,
                                    fpreal &dynfriction);


    /// Enforces boundary conditions on a velocity field by making
    /// sure we are lifting from the surface, allowing tangential
    /// motion.
    /// forbidinterference keeps the normal enforcement behaviour of
    /// explicitly setting all side boundaries that have a inward
    /// pointing velocity to zero relative motion
    /// SIM_BoundaryLine is given in world space and allows for an
    /// open boundary to exist above a "line" in an otherwise closed
    /// boundary.

    void		 enforceVelBoundary(const SIM_ScalarField *collision,
                                    ut_velocityCalc calcVelocity,
			                        void *vvp,
                                    ut_physParmCalc calcPhysParms,
                                    void *vpp,
                                    bool forbidinterference,
				    const SIM_BoundaryLine &worldbline = SIM_BoundaryLine());

    /// Enforces boundary conditions on a velocity field by making
    /// sure we are lifting from the surface, allowing tangential
    /// motion.
    /// forbidinterference keeps the normal enforcement behaviour of
    /// explicitly setting all side boundaries that have a inward
    /// pointing velocity to zero relative motion.
    void         enforceVelBoundaryVariational(const SIM_ScalarField *collision,
                                               const SIM_VectorField *weights,
                                               ut_velocityCalc calcVelocity,
                                               void *vvp,
                                               ut_physParmCalc calcPhysParms,
                                               void *vpp,
                                               bool threadsafe,
                                               fpreal bandwidth);

    class enforceVelBoundaryVariationalParms
    {
    public:
        SIM_RawField *u;
        const SIM_VectorField *velocity;
        const SIM_ScalarField *collision;
        ut_velocityCalc calcVelocity;
        void *vvp;
        ut_physParmCalc calcPhysParms;
        void *vpp;
        const SIM_RawField *weights;
        fpreal bandwidth;
        int axis;
    };

    THREADED_METHOD1(SIM_VectorField, parms.u->shouldMultiThread(),
            enforceVelBoundaryVariationalAxis,
            enforceVelBoundaryVariationalParms&, parms);


    void         enforceVelBoundaryVariationalAxisPartial(
                                enforceVelBoundaryVariationalParms &parms,
                                const UT_JobInfo &info);

    /// Converts an integer index into a worldspace position.
    bool		 indexToPos(int axis, int x, int y, int z, UT_Vector3 &pos) const;

    /// Converts a worldspace position into an integer index.
    bool		 posToIndex(int axis, const UT_Vector3 &pos, int &x, int &y, int &z) const;

    /// Retrieve raw field.
    SIM_RawField		*getField(int axis) const { return myFields[axis]; }
    SIM_RawField		*getXField() const { return myFields[0]; }
    SIM_RawField		*getYField() const { return myFields[1]; }
    SIM_RawField		*getZField() const { return myFields[2]; }

    /// Sets the field to the given field, gaining ownership of it.
    /// The new field must already match the field it will replace.
    void			 setField(int axis, SIM_RawField *field);

    /// True if we contain any NANs
    bool			 hasNan() const 
    { return getField(0)->hasNan() || getField(1)->hasNan() || getField(2)->hasNan(); }

    /// True if we have a constant value.  Ignores end conditions
    /// in determining this.  Used as a rough guess that the field
    /// is unused.
    bool			 appearsToBeUnused() const
    { 
	for (int i = 0; i < 3; i++) 
	    if (!getField(i)->field()->isConstant(0)) return false;
	return true;
    }

    /// Steals the field, replacing this copy with an empty field and
    /// returning the old version.
    SIM_RawField		*stealField(int axis);

    void			 testForNan() const;

    void		 pubHandleModification()
			 {
			     testForNan();
			     handleModification();
			 }

    void		 markGridAsChanged()
			 { for (int i = 0; i < 3; i++) getField(i)->markGridAsChanged(); }

    /// Recomputes total number of voxels to be stored
    /// on our options data for ease of reading
    void		updateTotalVoxels();

    /// Creates a GDP with us as a Volume Primitive inside it.
    GU_ConstDetailHandle	 createSmokeRepresentation(const SIM_RootData &root) const;

    /// Adds a volume primitive version of our field to the given
    /// gdp.
    void			 addSmokeRepresentation(const SIM_RootData &root, GU_Detail *gdp) const;

protected:
    explicit		 SIM_VectorField(const SIM_DataFactory *factory);
    virtual		~SIM_VectorField();

    /// Overrides to properly implement this class as a SIM_Data.
    virtual void	 initializeSubclass();
    /// myField aware copy constructor.
    virtual void	 makeEqualSubclass(const SIM_Data *source);

    /// Saves our attributes, and our internal data if it has been set.
    virtual void	 saveSubclass(std::ostream &os) const;
    /// Loads our attributes and internal data if it was set when we saved.
    virtual bool	 loadSubclass(UT_IStream &is);

    virtual int64	 getMemorySizeSubclass() const;

    /// Override the setDivisions to rebuild our voxel array on demand.
    virtual void	 optionChangedSubclass(const char *name);

private:
    static const SIM_DopDescription	*getVectorFieldDopDescription();

    SIM_RawField	*myFields[3];

    void		 rebuildFields();

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_VectorField,
			SIM_Data,
			"VectorField",
			getVectorFieldDopDescription());
};
#endif

