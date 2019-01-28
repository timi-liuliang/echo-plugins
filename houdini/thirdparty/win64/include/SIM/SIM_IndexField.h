/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_IndexField.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_IndexField__
#define __SIM_IndexField__

#include "SIM_API.h"

#include <UT/UT_VoxelArray.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"
#include "SIM_RawField.h"
#include "SIM_RawIndexField.h"

class UT_IStream;
class SIM_Geometry;
class SIM_ScalarField;
class SIM_VectorField;
class SIM_MatrixField;

/// This class holds a three dimensional scalar field.  
class SIM_API SIM_IndexField : public SIM_Data,
			public SIM_OptionsUser
{
public:
    typedef SIM_RawIndexField	 rawfield_type;

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
    GETSET_DATA_FUNCS_I("initialvalue", InitialValue);
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
    void		 matchField(const SIM_VectorField *field);
    void		 matchField(const SIM_MatrixField *field);
    void		 matchField(const SIM_IndexField *field, bool matchsamples = false);

    SIM_FieldSample	 getVoxelSample() const;

    const UT_Vector3	&getVoxelSize() const;
    fpreal		 getVoxelDiameter() const;
    void		 setVoxelSize(const UT_Vector3 &voxelsize)
			 { myField->setVoxelSize(voxelsize); }

    /// Access the field value given a world space location.
    /// This does closest point matching.
    exint		 getValue(const UT_Vector3 &pos) const;

    /// Converts an integer index into a worldspace position.
    bool		 indexToPos(int x, int y, int z, UT_Vector3 &pos) const;
    bool		 indexToPos(exint x, exint y, exint z, UT_Vector3D &pos) const;

    /// Converts a worldspace position into an integer index.
    bool		 posToIndex(const UT_Vector3 &pos, int &x, int &y, int &z) const;

    /// Retrieve raw field.
    SIM_RawIndexField		*getField() const { return myField; };

    /// Sets the field to the given field, gaining ownership of it.
    void			 setField(SIM_RawIndexField *field);

    /// Can't have NANs in integer fields.
    bool			testForNan() const { return false; }

    /// True if we have a constant value.  Ignores end conditions
    /// in determining this.  Used as a rough guess that the field
    /// is unused.
    bool			 appearsToBeUnused() const
    { return getField()->field()->isConstant(0); }

    /// Steals the field, replacing this copy with an empty field and
    /// returning the old version.
    SIM_RawIndexField		*stealField();

    /// Signals to the field that it has been altered externally.
    void		 pubHandleModification()
			 {
			     handleModification();
			 }

    /// Recomputes total number of voxels to be stored
    /// on our options data for ease of reading
    void		updateTotalVoxels();

    /// Creates a GDP with us as a Volume Primitive inside it.
    GU_ConstDetailHandle	 createSmokeRepresentation(const SIM_RootData &root) const;

    /// Adds a volume primitive version of our field to the given
    /// gdp.
    void			 addSmokeRepresentation(const SIM_RootData &root, GU_Detail *gdp) const;
protected:
    explicit		 SIM_IndexField(const SIM_DataFactory *factory);
    virtual		~SIM_IndexField();

    /// Overrides to properly implement this class as a SIM_Data.
    virtual void	 initializeSubclass();
    /// myField aware copy constructor.
    virtual void	 makeEqualSubclass(const SIM_Data *source);

    /// Saves our attributes, and our internal data if it has been set.
    virtual void	 saveSubclass(std::ostream &os) const;
    /// Loads our attributes and internal data if it was set when we saved.
    virtual bool	 loadSubclass(UT_IStream &is);

    virtual exint	 getMemorySizeSubclass() const;

    /// Override the setDivisions to rebuild our voxel array on demand.
    virtual void	 optionChangedSubclass(const char *name);

private:
    static const SIM_DopDescription	*getIndexFieldDopDescription();

    /// Rebuilds our raw field so it matches our current parameters.
    void		 		 rebuildField();

    SIM_RawIndexField			*myField;

    /// When creating a new scalar field, we do the following:
    /// 1) Load default values, triggering rebuildField()
    /// 2) Call initialize()
    /// 3) Load actual parameters values, triggering rebuildField()
    /// We need to set the initial value in the third stage.  The
    /// problem is that if there is a non-zero default and the other
    /// parameters are all default, stage 3 will be skipped.  We thus
    /// track our last default to avoid this case.
    exint				 myStashedInitialValue;

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_IndexField,
			SIM_Data,
			"IndexField",
			getIndexFieldDopDescription());
};
#endif

