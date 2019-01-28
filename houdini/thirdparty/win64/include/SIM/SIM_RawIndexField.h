/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_RawendexField.h ( SIM Library, C++)
 *
 * COMMENTS:
 *		An RawIndexField tracks a per-voxel index so one can
 *		convert a raw field to a linear coordinate system
 *		and back again.
 */

#ifndef __SIM_RawIndexField__
#define __SIM_RawIndexField__

#include "SIM_API.h"

#include <UT/UT_VoxelArray.h>

#include <openvdb/tools/PointIndexGrid.h>

#include "SIM_RawField.h"

/// Voxel boxes are used to define voxel groups
struct SIM_API SIM_VoxelBox
{
    SIM_VoxelBox();
    SIM_VoxelBox(
        const int xMin, const int yMin, const int zMin,
        const int xEnd, const int yEnd, const int zEnd
    );

    bool contains(const int x, const int y, const int z) const;

    // Represent all voxels v with begin[d] <= v[d] < end[d] for d < 3.
    int begin[3];
    int end[3];
};

class SIM_API SIM_RawIndexField
{
public:
    typedef UT_Array<int64> Indices;
    typedef SIM_VoxelBox Box;
    typedef UT_Array<Box> Boxes;

	     SIM_RawIndexField();
    virtual ~SIM_RawIndexField();

    /// Copy constructor:
		SIM_RawIndexField(const SIM_RawIndexField &src);

    /// Assigment operator:
    const SIM_RawIndexField	&operator=(const SIM_RawIndexField &src);		

    /// Initializes the field.
    /// The resolution given is in terms of voxels, the actual dimensions
    /// of this field may be slightly different due to the sampling
    /// choice.
    void	init(SIM_FieldSample sample, 
		     const UT_Vector3 &orig, const UT_Vector3 &size,
		     int xres, int yres, int zres);
    void	init(SIM_FieldSample sample, 
		     const UT_Vector3 &orig, const UT_Vector3 &size,
		     int xres, int yres, int zres,
		     const UT_Vector3 &voxelsize);

    /// Initialize this to be the same dimension and sampling
    /// patern as the given field.
    void	match(const SIM_RawField &src);
    void	match(const SIM_RawIndexField &src);

    /// Sets this field to a constant value
    void	makeConstant(exint cval);
    
    /// Convert indices to world coordinates and vice-versa.  Note this uses
    /// this field's indices which change depending on sampling.
    bool		 indexToPos(int x, int y, int z, UT_Vector3 &pos) const;
    bool		 indexToPos(exint x, exint y, exint z, UT_Vector3D &pos) const;

    /// Converts a worldspace position into an integer index.
    bool		 posToIndex(UT_Vector3 pos, int &x, int &y, int &z) const;

    /// Returns true if the given field and this one match in terms
    /// of number of voxels and bounding box size.
    /// This means the voxel cells match - not necessarily the sample
    /// points!
    bool	isMatching(const SIM_RawIndexField *field) const;

    /// Returns true if the two fields are precisely aligned.  This
    /// means that samples are matched so a given integer index
    /// into either field would give the same result.
    bool	isAligned(const SIM_RawIndexField *field) const;
    bool	isAligned(const SIM_RawField *field) const;

    exint		 getMemoryUsage() const;
    
    /// Returns the resolution of the voxel grid that we are sampling.
    /// This is a count of voxels, so may differ for our different
    /// sampling methods.
    void		 getVoxelRes(int &xres, int &yres, int &zres) const;

    /// Returns the actual number of samples in each resolution.
    /// Preferred over field()->getXRes() as it doesn't require a
    /// copy of the CE fields.
    int			 getXRes() const { return myField->getXRes(); }
    int			 getYRes() const { return myField->getYRes(); }
    int			 getZRes() const { return myField->getZRes(); }

    /// Consistently compute a world origin for what world space 0,0,0
    /// would map to.  A bit tricky as we want to avoid round off
    /// if we line up to 0.5...
    /// This is not necessarily 0,0,0.
    void		 getWorldOrigin(int &origx, int &origy, int &origz) const;

    const UT_Vector3	&getVoxelSize() const { return myVoxelSize; }
    void		 setVoxelSize(const UT_Vector3 &voxelsize) 
			 { myVoxelSize = voxelsize; 
			   myVoxelDiameter = voxelsize.length(); }
    fpreal		 getVoxelDiameter() const { return myVoxelDiameter; }

    UT_VoxelBorderType	 getBorder() const { return myField->getBorder(); }
    exint	 	 getBorderValue() const { return myField->getBorderValue(); }
    void		 setBorder(UT_VoxelBorderType border, exint bval)
			 { myField->setBorder(border, bval); }

    /// Returns true if the given x, y, z values lie inside the valid index.
    bool	 isValidIndex(int x, int y, int z) const
		 {
		     return field()->isValidIndex(x, y, z);
		 }

    /// Returns the set of samples in *this* field which correspond
    /// to the given location & sampling pattern.
    /// ix, iy, and iz should be size 8.
    /// If you want the deltas for the sampling pattern, call with
    /// x, y, z zero and clamp to false.
    void        getSamplePattern(SIM_FieldSample sample,
                                 int x, int y, int z,
                                 int &numsample,
                                 int *ix, int *iy, int *iz,
                                 bool clamp) const;

    /// Builds from a surface & collision field.
    /// -1 if inside collision, -2 if not in collision and not in
    /// surface, otherwise a unique number.
    /// Returns maximum index (which you can also get from getMaxIndex)
    exint	buildIndex(const SIM_RawField *surface,
			   const SIM_RawField *collision);
    
    // Partitions the voxels into sets.
    exint       buildPartitionedIndex(const SIM_RawField *surface,
		                      const SIM_RawField *collision);

    // Each voxel is an index of where we should copy our values
    // to perform a SAME boundary collision.  this should already
    // be inited to the desired resolution.
    // -1 for voxels that don't need collision lookups, otherwise
    // it is x+(y+z*yres)*xres
    // Will only write to non- -1 voxels, so call constant(-1) first.
    THREADED_METHOD1(SIM_RawIndexField, shouldMultiThread(), 
		    buildCollisionLookup,
		    const SIM_RawField *, collision)
    void	buildCollisionLookupPartial(const SIM_RawField *collision,
					    const UT_JobInfo &info);
    
    /// Computes the connected components of the given field.
    /// Anything with a value < 0 will be considered "inside" and
    /// connected to each other.  Voxels with a value >= 0 will be
    /// flagged as being in component -1.
    /// The maxIndex will store the number of connected components.
    exint	computeConnectedComponents(const SIM_RawField &surface);

    /// Computes the connected components according to the given index
    /// values.  Areas of -1, -2, and >=0 will be consdiered three different
    /// material types and connectivity computed respectively.
    exint	computeConnectedComponents(const SIM_RawIndexField &idx);

    exint   computeConnectedComponentsWeighted(const SIM_RawIndexField &idx,
                                               const SIM_RawField *weights[3],
					       const SIM_RawIndexField *sliceindex = 0,
					       int thisslice = -1);

    /// Computes connectivity of the -2 and >= 0 material types.
    void	computeMetaConnected(UT_IntArray &metagroups, 
				    const SIM_RawIndexField &idx) const;

    /// Returns true if this should be multithreaded.
    bool	shouldMultiThread() const
		{
		    return field()->numTiles() > 1;
		}

    exint	operator()(int x, int y, int z) const
		{
		    return (*field())(x, y, z);
		}
    exint	getIndex(const UT_VoxelArrayIteratorF &vit) const
		{
		    return (*field())(vit.x(), vit.y(), vit.z());
		}

    exint	getValue(const UT_Vector3 &pos) const;

    const UT_VoxelArrayI *field() const { return myField; }
    UT_VoxelArrayI *fieldNC() const { return myField; }

    exint	maxIndex() const { return myMaxIndex; }

    const UT_Vector3 &getOrig() const { return myOrig; }
    const UT_Vector3 &getSize() const { return mySize; }
    const UT_Vector3 &getBBoxOrig() const { return myBBoxOrig; }
    const UT_Vector3 &getBBoxSize() const { return myBBoxSize; }

    SIM_FieldSample		 getSample() const { return mySample; }

    void extrapolateClosestPoints(const SIM_RawIndexField *altclosept,
		    const GU_Detail *gdp,
		    const openvdb::tools::PointIndexGrid *ptgridvdb,
		    fpreal uniformradius,
		    fpreal bandwidth,
		    bool rebuildsdf,
		    SIM_RawField *dest = NULL,
		    const SIM_RawField::sim_particleToFieldParms *parms = NULL);

protected:

    /// Given a set of connectivity classes, collapse into the minimal
    /// set and renumber from 0.  myMaxIndex will be updated with the
    /// resulting maximum & the number of components returned.
    exint	collapseClassIndices();

    /// Determines if two indices should be connected using our empty
    /// cell merge rule.
    bool	shouldConnectIndices(exint idx1, exint idx2) const;

    THREADED_METHOD3(SIM_RawIndexField, shouldMultiThread(),
            initConnectedComponents,
            const SIM_RawIndexField &, idx,
	    const SIM_RawIndexField *, sliceindex,
	    int, thisslice);
    void initConnectedComponentsPartial(const SIM_RawIndexField &idx,
				       const SIM_RawIndexField *sliceindex,
				       int thisslice,
                                       const UT_JobInfo &info);

    THREADED_METHOD1(SIM_RawIndexField, shouldMultiThread(),
            applyLookup,
            const UT_VoxelArrayI &, lut);
    void applyLookupPartial(const UT_VoxelArrayI &lut,
                            const UT_JobInfo &info);

    UT_VoxelArrayI		*myField;
    SIM_FieldSample		 mySample;
    exint			 myMaxIndex;

    // This is the size and offset which converts our UT_VoxelArray into
    // world coordinates.
    UT_Vector3			 myOrig, mySize;

    // This is our actual official size.
    UT_Vector3			 myBBoxOrig, myBBoxSize;

    // Cached metrics.
    UT_Vector3			 myVoxelSize;
    fpreal			 myVoxelDiameter;

    // Find ranges of indices for valid voxels
    void findRange
    (
        const SIM_RawField* surface,
        const SIM_RawField* collision,
        int begin[3],
        int end[3]
    ) const;

    // Count all valid voxels in a box
    exint countVoxelsInBox
    (
        const SIM_RawField* surface,
        const SIM_RawField* collision,
        const Box& box
    ) const;


    // Closest point extrapolation functions.
    typedef exint sim_extrapelem;

    static sim_extrapelem indexToElement(const UT_VoxelArrayI &U,
                                         int x, int y, int z);

    static void elementToIndex(const UT_VoxelArrayI &U, sim_extrapelem idx,
                               int &x, int &y, int &z);

    void computeQueryOffsets(const SIM_RawIndexField *nindex,
                             int nsamples,
                             const int *dx, const int *dy, const int *dz,
                             UT_Vector3Array &queryoffsets,
                             UT_ValArray<fpreal> &radii) const;

    struct sim_buildActiveParms
    {
	const UT_VoxelArrayI 				*origpt;
	SIM_RawField 					*dest;
	const SIM_RawIndexField				*altclosept;
	const GU_Detail					*gdp;
	const openvdb::tools::PointIndexGrid		*ptgridvdb;
	fpreal						uniformradius;
	fpreal						bandwidth;
	bool						rebuildsdf;
	UT_ValArray<UT_ValArray<sim_extrapelem> > 	*lists;
	UT_ValArray<bool> 				*tileoccupied;
    };

    THREADED_METHOD1(SIM_RawIndexField, shouldMultiThread(),
                     buildActiveLists,
                     const sim_buildActiveParms &, parms);

    void buildActiveListsPartial(const sim_buildActiveParms &parms,
                                 const UT_JobInfo &info);

    THREADED_METHOD4(SIM_RawIndexField, tileoccupied.entries() > 10,
                     uncompressActiveTiles,
                     UT_VoxelArrayI &, closept,
                     UT_VoxelArrayI &, newclosept,
                     UT_VoxelArrayF *, dest,
                     const UT_ValArray<bool> &, tileoccupied);

    void uncompressActiveTilesPartial(UT_VoxelArrayI &closept,
                                      UT_VoxelArrayI &newclosept,
                                      UT_VoxelArrayF *dest,
                                      const UT_ValArray<bool> &tileoccupied,
                                      const UT_JobInfo &info);

    struct sim_extrapActiveParms
    {
        const GU_Detail *gdp;
        SIM_RawField *dest;
        bool rebuildsdf;
        const SIM_RawField::sim_particleToFieldParms *ptfparms;
        fpreal uniformradius;
        fpreal bandwidth;
        const UT_ValArray<sim_extrapelem> *elements;
        UT_ValArray<UT_ValArray<sim_extrapelem> >* newelemlists;
        UT_VoxelArrayI *closept;
        UT_VoxelArrayI *newclosept;
        UT_ValArray<bool> *tileoccupied;
    };

    THREADED_METHOD1(SIM_RawIndexField, parms.elements->entries() > 50,
                     extrapolateActiveElements,
                     const sim_extrapActiveParms &, parms);

    void extrapolateActiveElementsPartial(const sim_extrapActiveParms &parms,
                                          const UT_JobInfo &info);

    THREADED_METHOD4(SIM_RawIndexField, elements.entries() > 100,
                    applyExtrapolatedParticleToField,
                    const UT_ValArray<sim_extrapelem> &, elements,
                    const GU_Detail *, gdp,
                    SIM_RawField *, dest,
                    const SIM_RawField::sim_particleToFieldParms &, ptfparms);

    void applyExtrapolatedParticleToFieldPartial(
                        const UT_ValArray<sim_extrapelem> &e,
                        const GU_Detail *gdp,
                        SIM_RawField *dest,
                        const SIM_RawField::sim_particleToFieldParms &ptfparms,
                        const UT_JobInfo &info);
};

#endif

