/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_OSD3.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_OSD3__
#define __GT_OSD3__

#if defined(WIN32)
    // MSVC does not provide readable alternatives to operators by default
    // (i.e. && => 'and') so we have to include this header since OpenSubdiv
    // makes use of some of the alternatives such as 'not', 'and' and 'or'.
    #include <iso646.h>
#endif

#include "GT_API.h"
#include "GT_Types.h"
#include "GT_PrimPolygonMesh.h"
#include <SYS/SYS_Hash.h>
#include <UT/UT_UniquePtr.h>
#include <opensubdiv/far/primvarRefiner.h>
#include <opensubdiv/far/patchTableFactory.h>
#include <opensubdiv/far/patchMap.h>

class UT_JSONWriter;

/// @file
/// This file contains code to support OpenSubdiv 3.x for the GT library
/// The code in here is primarily used in GT_UtilOpenSubdiv

/// Options for topology and refinement
class GT_API GT_OSDOptions
{
public:
    using SdcOptions = OpenSubdiv::Sdc::Options;
    using VtxBoundaryInterpolation = SdcOptions::VtxBoundaryInterpolation;
    using FVarLinearInterpolation = SdcOptions::FVarLinearInterpolation;
    using CreasingMethod = SdcOptions::CreasingMethod;
    using TriangleSubdivision = SdcOptions::TriangleSubdivision;

    GT_OSDOptions(GT_Scheme scheme = GT_CATMULL_CLARK,
		int level = 2,
		bool adaptive = false,
		fpreal crease_override = -1,
		bool remove_holes = true)
	: myOptions()
	, myScheme(scheme)
	, myCreaseOverride(crease_override)
	, myLevel(level)
	, myAdaptive(adaptive)
	, myRemoveHoles(remove_holes)
    {
	// Set default behaviour
	setVtxBoundaryInterpolation(SdcOptions::VTX_BOUNDARY_EDGE_AND_CORNER);
	setFVarLinearInterpolation(SdcOptions::FVAR_LINEAR_BOUNDARIES);
    }

    const SdcOptions	&options() const { return myOptions; }
    void		 setOptions(const SdcOptions &o) { myOptions = o; }

    SYS_HashType	hash() const;
    bool		isEqual(const GT_OSDOptions &opts) const;
    bool		operator==(const GT_OSDOptions &s) const
			    { return isEqual(s); }
    bool		operator!=(const GT_OSDOptions &s) const
			    { return !isEqual(s); }

    // Access this object as it were an OpenSubdiv::Sdc::Options
    SdcOptions		&operator->() { return myOptions; }
    const SdcOptions	&operator->() const { return myOptions; }
    SdcOptions		&operator*() { return myOptions; }
    const SdcOptions	&operator*() const { return myOptions; }

    GT_Scheme	scheme() const { return myScheme; }
    void	setScheme(GT_Scheme t) { myScheme = t; }

    int		level() const { return myLevel; }
    void	setLevel(int l) { myLevel = l; }

    bool	adaptive() const { return myAdaptive; }
    void	setAdaptive(bool b) { myAdaptive = b; }

    bool	removeHoles() const { return myRemoveHoles; }
    void	setRemoveHoles(bool v) { myRemoveHoles = v; }

    bool	enableCreaseOverride() const { return myCreaseOverride >= 0; }
    fpreal	creaseOverride() const	{ return myCreaseOverride; }
    void	setCreaseOverride(fpreal value=-1) { myCreaseOverride = value; }

#define GETSET(TYPE, GET) \
    TYPE	GET() const { return myOptions.Get##TYPE(); } \
    void	set##TYPE(TYPE v) { myOptions.Set##TYPE(v); }

    /// @{
    /// Simple wrappers on the SdcOptions
    GETSET(VtxBoundaryInterpolation, vtxBoundaryInterpolation);
    GETSET(FVarLinearInterpolation, fvarLinearInterpolation);
    GETSET(CreasingMethod, creasingMethod);
    GETSET(TriangleSubdivision, triangleSubdivision);
    /// @}
#undef GETSET

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

private:
    SdcOptions	myOptions;
    GT_Scheme	myScheme;
    fpreal	myCreaseOverride;
    int		myLevel;
    bool	myAdaptive;
    bool	myRemoveHoles;
};

/// Options for setting up limit surface evaluation.
class GT_API GT_OSDPatchTableOptions
{
public:
    using FactoryOptions = OpenSubdiv::Far::PatchTableFactory::Options;

    GT_OSDPatchTableOptions()
	: myOptions()
    {
    }

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

    FactoryOptions		&operator*() { return myOptions; }
    const FactoryOptions	&operator*() const { return myOptions; }

    FactoryOptions::EndCapType	endCapType() const
		    { return (FactoryOptions::EndCapType)myOptions.endCapType; }
    bool	generateAllLevels() const
		    { return myOptions.generateAllLevels; }
    bool	triangulateQuads() const
		    { return myOptions.triangulateQuads; }
    bool	useSingleCreasePatch() const
		    { return myOptions.useSingleCreasePatch; }
    int		maxIsolationLevel() const
		    { return myOptions.maxIsolationLevel; }
    bool	shareEndCapPatchPoints() const
		    { return myOptions.shareEndCapPatchPoints; }
    bool	generateFVarTables() const
		    { return myOptions.generateFVarTables; }
    int		numFVarChannels() const
		    { return myOptions.numFVarChannels; }
    const int	*fvarChannelIndices() const
		    { return myOptions.fvarChannelIndices; }

    void	setEndCapType(FactoryOptions::EndCapType t)
		    { myOptions.endCapType = (FactoryOptions::EndCapType)t; }
    void	setGenerateAllLevels(bool v)
		    { myOptions.generateAllLevels = v; }
    void	setTriangulateQuads(bool v)
		    { myOptions.triangulateQuads = v; }
    void	setUseSingleCreasePatch(bool v)
		    { myOptions.useSingleCreasePatch = v; }
    void	setMaxIsolationLevel(int v)
		    { myOptions.maxIsolationLevel = v; }
    void	setShareEndCapPatchPoints(bool v)
		    { myOptions.shareEndCapPatchPoints = v; }
    void	setGenerateFVarTables(bool v)
		    { myOptions.generateFVarTables = v; }
    void	setNumFVarChannels(int n)
		    { myOptions.numFVarChannels = n; }
    void	setFVarChannelIndices(const int *indices)
		    { myOptions.fvarChannelIndices = indices; }
    void	setLegacyLinearPatches(bool b)
		    { myOptions.generateFVarLegacyLinearPatches = b; }
private:
    FactoryOptions	myOptions;
};

/// Topology definition for opensubdiv classes
class GT_API GT_OSDTopology
{
public:
    using TopologyRefiner = OpenSubdiv::Far::TopologyRefiner;
    using LevelInfo = OpenSubdiv::Far::TopologyLevel;

    /// This class stores a definition of the mesh & topology.  It can be used
    /// to determine whether the mesh can be updated with new attribute values.
    class MeshDescriptor
    {
    public:
	MeshDescriptor(const GT_PrimPolygonMesh &mesh,
		const GT_OSDOptions &options);

	bool	isValid() const { return myPointCount > 0; }

	bool	matchesTopology(const MeshDescriptor &other) const;
	bool	matchesAttributes(const MeshDescriptor &other) const;

    private:
	GT_Size			myPointCount;
	GT_CountArray		myFaceCounts;
	GT_DataArrayHandle	myVertexList;
	GT_AttributeMapHandle	myPointAttribs;
	GT_AttributeMapHandle	myVertexAttribs;
	GT_DataArrayHandle	myCreaseIndices;
	GT_DataArrayHandle	myCreaseWeights;
	GT_DataArrayHandle	myCornerIndices;
	GT_DataArrayHandle	myCornerWeights;
	GT_DataArrayHandle	myHoleIndices;
	fpreal			myCreaseOverride;
    };

     GT_OSDTopology();
    ~GT_OSDTopology();

    /// @{
    /// Debug - dump the topology out
    void	dump() const;
    void	dump(UT_JSONWriter &w) const;
    /// @}

    /// Create the topology for the given polygonal hull and options.  Note
    /// that if the mesh is a GT_PrimSubdivisionMesh, crease weights will be
    /// picked up from the tags on the primitive.
    bool	create(const GT_PrimPolygonMesh &mesh,
			const GT_OSDOptions &options);

    /// Number of levels in refinement (note this is the max level + 1)
    exint		levelCount() const
			    { return myRefiner->GetNumLevels(); }

    /// Access to the level information for a given refinement level
    /// This returns the number of vertices, face-varying vertices, faces, etc.
    /// for each level.  Level 0 is the "coarse" mesh.
    /// - GetNumVertices() - Number of shared points (i.e. Houdini points)
    /// - GetNumFaces() - Number of faces
    /// - GetNumEdges() - Number of edges
    /// - GetNumFaceVertices() - Number of face varying (i.e. Houdini vertices)
    const LevelInfo	&levelInfo(int level) const
			    { return myRefiner->GetLevel(level); }
    const LevelInfo	&lastLevel() const
			    { return levelInfo(myRefiner->GetNumLevels()-1); }


    /// @{
    /// Return specific information about the coarse mesh (i.e. level 0)
    GT_Size	coarseFaceCount() const
		    { return levelInfo(0).GetNumFaces(); }
    GT_Size	coarsePointCount() const
		    { return levelInfo(0).GetNumVertices(); }
    GT_Size	coarseVertexCount() const
		    { return levelInfo(0).GetNumFaceVertices(); }
    GT_Size	coarseVertexCount(GT_Size face_index) const;
    /// @}

    /// Access to the refiner
    const TopologyRefiner	*refiner() const { return myRefiner; }

    /// Test whether this has been built for adaptive refinement
    bool		adaptive() const { return myAdaptive; }

    /// Create a polygon mesh for a given level
    GT_PrimitiveHandle	createMesh(int level,
				const GT_AttributeListHandle &point,
				const GT_AttributeListHandle &vertex,
				const GT_AttributeListHandle &uniform,
				const GT_AttributeListHandle &detail,
				const GT_FaceSetMapPtr &fsets=GT_FaceSetMapPtr()) const;
private:
    TopologyRefiner	       *myRefiner;
    bool		 	myAdaptive;
    bool		 	myRemoveHoles;
};

// Interface to create and look through a patch table
class GT_API GT_OSDPatchTable
{
public:
    using TopologyRefiner = OpenSubdiv::Far::TopologyRefiner;
    using PatchTable = OpenSubdiv::Far::PatchTable;
    using PatchTableFactory = OpenSubdiv::Far::PatchTableFactory;
    using PatchDescriptor = OpenSubdiv::Far::PatchDescriptor;
    using ConstIndexArray = OpenSubdiv::Far::ConstIndexArray;
    using PatchParam = OpenSubdiv::Far::PatchParam;
    using PatchType = OpenSubdiv::Far::PatchDescriptor::Type;
    using PtexIndices = OpenSubdiv::Far::PtexIndices;
    using Handle = PatchTable::PatchHandle;

    GT_OSDPatchTable();

    bool create(const GT_OSDTopology &top,
		bool createPtexMap=false,
		bool legacy_linear=true);

    // Array level queries
    int arrayCount() const;
    int arrayPatchCount(int arr) const;
    int numLocalPoints() const;
    int numLocalPointsVarying() const;
    ConstIndexArray vertexArray(int arr) const;
    ConstIndexArray varyingVertexArray(int arr) const;
    PatchDescriptor arrayPatchDesc(int arr) const;
    template<typename T>
    void computeLocalPoints(T *src, T *dest) const
    {
	myTable->ComputeLocalPointValues(src, dest);
    }
    template<typename T>
    void computeLocalFVPoints(T *src, T *dest) const
    {
	myTable->ComputeLocalPointValuesFaceVarying(src, dest);
    }

    // Patch level queries
    PatchParam patchParam(int arr, int pat) const;
    ConstIndexArray patchVertices(const Handle &patch) const;
    ConstIndexArray patchFVValues(const Handle &patch) const;
    void evaluateBasis(const Handle &patch, float u, float v,
		       float *w, float *du, float *dv) const;
    void evaluateBasisFV(const Handle &patch, float u, float v,
		       float *w, float *du, float *dv) const;

    // For ptex queries
    int numPtexFaces() const;
    const int *getPtexMap() const { return myPtexMap.array(); };
    const UT_Array<int> &getPtexArray() const { return myPtexMap; };

    PatchTable *getTable() const { return myTable.get(); };
    bool initialized() const { return myTable != nullptr; };

private:
    UT_UniquePtr<PatchTable> myTable;
    GT_OSDPatchTableOptions  myOptions;
    UT_Array<int> 	     myPtexMap;
};

/// Storage arrays for vertex data for opensubdiv classes
class GT_API GT_OSDAttributes
{
public:
    /// Identifier for an attribute.  The first item is the index in the
    /// attribute list, the second is whether it's a vertex or a point
    /// attribute.
    using AttribId = std::pair<int, bool>;

    GT_OSDAttributes();
    ~GT_OSDAttributes();

    void	dump() const;
    void	dump(UT_JSONWriter &w )const;

    bool	create(const GT_OSDTopology &topology,
		       const GT_PrimPolygonMesh &mesh,
		       const GT_OSDPatchTable *table=nullptr);

    bool	update(const GT_OSDTopology &topology,
		       const GT_PrimPolygonMesh &mesh,
		       bool skip_equality_check = false);

    /// Extract a subdivided mesh for the given topology level
    GT_PrimitiveHandle	extractMesh(const GT_OSDTopology &topology,
				bool harden,
				int level = -1);

    /// Extract the shared/point attributes for a mesh at the given refinement
    /// level.  If @c harden is @c false, the attributes lists will point to
    /// the temporary buffers (which are over-sized).  Passing @c true to @c
    /// harden will copy out the temporary data into compact arrays.
    GT_AttributeListHandle	extractShared(const GT_OSDTopology &top,
					int level_index,
					bool harden) const;
    /// Extract the vertex/face-varying attributes for a mesh at the given
    /// refinement level.  See @c extractShared for help on @c harden
    GT_AttributeListHandle	extractVertex(const GT_OSDTopology &top,
					int level_index,
					bool harden) const;
    /// Extract the primitive/face attributes for a mesh at the given
    /// refinement level.
    GT_AttributeListHandle	extractUniform(const GT_OSDTopology &top,
					int level_index,
					bool harden) const;

    /// Extract a face map set for a mesh at the given refinement level
    GT_FaceSetMapPtr		extractFaceSets(const GT_OSDTopology &top,
					int level_index) const;

    /// Extract a mapping from fine faces at a given refinement level to
    /// its corresponding coarse face
    GT_DataArrayHandle		extractFaceMap(const GT_OSDTopology &top,
					       int level_index) const;

    /// @{
    /// Find an attribute
    AttribId	findAttribute(const char *name) const;
    AttribId	findAttribute(const char *name, bool vertex_attrib) const;
    /// @}

    /// Return the size of a given attribute
    int		tupleSize(const AttribId &attrib) const;

    /// Extract attributes on the coarse mesh
    bool	coarseValues(const AttribId &attrib,
			const GT_Size *vertices,
			GT_Size nvtx,
			UT_Vector2 *values,
			int seg) const;

    class VertexStorage
    {
    public:
	VertexStorage()
	    : myAttribs(nullptr)
	    , myArrays(nullptr)
	    , mySizes(nullptr)
	    , myStorage(nullptr)
	    , myPointers(nullptr)
	    , mySize(0)
	    , myReadOnly(true)
	{
	}
	~VertexStorage()
	{
	    clear();
	}

	int	findAttrib(const char *name) const
	{
	    return myAttribs ? myAttribs->getIndex(name) : -1;
	}

	void	init(const GT_AttributeListHandle &list, GT_Size size);
	bool	update(const GT_AttributeListHandle &list,
		       bool skip_equality_check = false);

	void	clear()
	{
	    mySize = 0;
	    delete [] myArrays;
	    delete [] myStorage;
	    delete [] myPointers;
	    delete [] mySizes;
	    myArrays = nullptr;
	    myStorage = nullptr;
	    myPointers = nullptr;
	    mySizes = nullptr;
	}

	/// Extract the attributes for a given level.  Note that the array
	/// returned makes reference to the arrays including the temporary
	/// vertices.  Passing @c harden==true will cause the arrays to be
	/// duplicated (more memory, but more compact)
	GT_AttributeListHandle	extractLevel(const GT_OSDTopology &topology,
					int level,
					bool harden,
					bool fvar) const;

	/// Raw access to the attributes
	const GT_AttributeListHandle	&attribList() const { return myAttribs;}

	/// Access a specific data array in the attributes
	const GT_DataArrayHandle	&attribArray(int attrib) const
					    { return myAttribs->get(attrib); }

	exint		size() const { return mySize; }
	bool		readOnly() const { return myReadOnly; }
	GT_Size		attribSize(int attrib) const { return mySizes[attrib]; }
	bool		attribValid(int attrib) const
			    { return myPointers[attrib] != nullptr; }
	GT_Storage	attribStorage(int attrib) const
			    { return myStorage[attrib]; }
	GT_Type		attribType(int attrib) const
			    { return myType[attrib]; }
	int		motionSegments() const { return mySegments; }

	template <typename T>
	T	*attribData(int attrib, GT_Size index, int seg)
	{
	    UT_ASSERT(myPointers[attrib + mySize * seg]);
	    UT_ASSERT(!myReadOnly);
	    // Here, we cast away the const, but we know that this is not
	    // read-only
	    auto data = (T *)myPointers[attrib + mySize * seg];
	    return data + index*mySizes[attrib];
	}
	template <typename T>
	const T	*attribData(int attrib, GT_Size index, int seg) const
	{
	    UT_ASSERT(myPointers[attrib + mySize * seg]);
	    auto data = (const T *)myPointers[attrib + mySize * seg];
	    return data + index*mySizes[attrib];
	}
	void	dump() const;
	void	dump(UT_JSONWriter &w) const;
    private:
	GT_AttributeListHandle	  myAttribs;
	GT_DataArrayHandle	 *myArrays;
	GT_Size			 *mySizes;
	GT_Storage		 *myStorage;
	GT_Type			 *myType;
	const void		**myPointers;
	GT_Size			  mySize;
	int			  mySegments;
	bool			  myReadOnly;
    };

    class Vertex
    {
    public:
	Vertex()
	    : myStorage(nullptr)
	    , myIndex(-1)
	    , myMaxWeight(0)
	{
	}
	void	init(VertexStorage &storage, GT_Size index)
	{
	    myStorage = &storage;
	    myIndex = index;
	    myMaxWeight = 0;
	}

	/// Methods required for OpenSubdiv
	const VertexStorage	*storage() const { return myStorage; }
	GT_Size			 index() const { return myIndex; }

	template <typename T>
	const T	*attribData(int attrib, int seg) const
	{
	    // Make sure to call the const method on myStorage
	    return static_cast<const VertexStorage *>(myStorage)->attribData<T>(
		attrib, myIndex, seg);
	}

	// TODO: Templatize?
	void	extract(int attrib_idx, fpreal32 *data, int size,
		        int seg) const;

	void	Clear();
	void	AddWithWeight(const Vertex &src, float weight);
	void	dump() const;
	void	dump(UT_JSONWriter &w) const;
    private:
	VertexStorage	*myStorage;
	GT_Size		 myIndex;
	float		 myMaxWeight;
    };

    const VertexStorage		&coarseVertex() const
				    { return myCoarseStorage; }
    const VertexStorage		&coarseFaceVarying() const
				    { return myCoarseFVStorage; }

    const UT_Array<Vertex>	&vertexArray() const
				    { return myVertices; }

    const UT_Array<Vertex>	&facevaryingArray() const
				    { return myFVertices; }

#if 0
    /// Given a face on the coarse mesh, a u/v coordinate from an attribute,
    /// look up the corresponding patch and interpolant.  If the attribute is
    /// invalid, the uv's will be assumed to be the face interpolants using the
    /// scheme where (0,0) maps to the first vertex, (1,0) maps to nvtx-1, (0,
    /// 1) maps to vertex 2, (1,1) maps to nvtx-2 and the remaining vertices
    /// are fit linearly between (0, 1) and (1,1).
    bool	lookupPatch(const GT_OSDTopology &topology,
		    GT_Size coarse_id, fpreal coarse_u, fpreal coarse_v,
		    GT_Size &patch_id, fpreal &patch_u, fpreal &patch_v,
		    const AttribId &attrib) const;

    /// Inverse of @c lookupPatch.  Given a patch, lookup the coarse id and
    /// interpolants.
    bool	lookupFace(GT_Size patch_id, fpreal patch_u, fpreal patch_v,
		    GT_Size &coarse_id, fpreal &coarse_u, fpreal &coarse_v,
		    const AttribId &attrib) const;
#endif

    const VertexStorage &coarseStorage() const { return myCoarseStorage; }
    const VertexStorage &fineStorage() const { return myFineStorage; }
    const VertexStorage &coarseFVStorage() const { return myCoarseFVStorage; }
    const VertexStorage &fineFVStorage() const { return myFineFVStorage; }

    /// Return a pointer to the index-th entry in the given attribute array.
    /// If the index exceeds or equals the number of coarse vertices
    /// (myFineOffset), then the data is located in the fine attribute
    /// array. The first element of the fine attribute array has index
    /// equal to myFineOffset, so we do pointer arithmetic to shift the
    /// array by myFineOffset entries.
    template <typename T>
    const T *getData(int attrib, int index, int seg) const
    {
	if (index < myFineOffset)
	{
	    return myCoarseStorage.attribData<T>(attrib, index, seg);
	}
	else
	{
	    int offset = index - myFineOffset;
	    return myFineStorage.attribData<T>(attrib, offset, seg);
	}
    }

    /// Same idea as with getData, but with face varying data
    template <typename T>
    const T *getDataFV(int attrib, int index, int seg) const
    {
	if (index < myFineFVOffset)
	{
	    return myCoarseFVStorage.attribData<T>(attrib, index, seg);
	}
	else
	{
	    int offset = index - myFineFVOffset;
	    return myFineFVStorage.attribData<T>(attrib, offset, seg);
	}
    }

    int fineOffset() const { return myFineOffset; }
    int fineFVOffset() const { return myFineFVOffset; }

private:
    // Perform interpolation
    void		interpolateData(const GT_OSDTopology &topology);

    GT_AttributeListHandle	myUniform;
    GT_AttributeListHandle	myDetail;
    GT_FaceSetMapPtr		myFaceSets;
    VertexStorage		myCoarseStorage;
    VertexStorage		myFineStorage;
    VertexStorage		myCoarseFVStorage; // Face vertex coarse storage
    VertexStorage		myFineFVStorage; // Face vertex refined storage
    UT_Array<Vertex>		myVertices;
    UT_Array<Vertex>		myFVertices; // Face vertices
    exint			myFineOffset;
    exint			myFineFVOffset;
};

/// Interface to perform limit surface evaluation.
class GT_API GT_OSDLimitSurface
{
public:
    using TopologyRefiner = OpenSubdiv::Far::TopologyRefiner;
    using PtexIndices = OpenSubdiv::Far::PtexIndices;
    using PatchTable = OpenSubdiv::Far::PatchTable;
    using PatchMap = OpenSubdiv::Far::PatchMap;
    using PatchTableFactory = OpenSubdiv::Far::PatchTableFactory;
    using AttribId = GT_OSDAttributes::AttribId;
    using VertexStorage = GT_OSDAttributes::VertexStorage;
    using Vertex = GT_OSDAttributes::Vertex;

    GT_OSDLimitSurface(const GT_OSDTopology &topology,
			const GT_OSDAttributes &attribs,
			const GT_OSDPatchTableOptions &options,
			const GT_OSDPatchTable &patchTable,
			const GT_PrimPolygonMesh &mesh);
    ~GT_OSDLimitSurface();

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

    bool	update(const GT_OSDAttributes &attribs);

    /// Return the number of patches
    int		ptexFaceCount() const;

    /// Return the first ptex patch associated with the given coarse face
    int		ptexFromFace(GT_Size face_index) const;
    /// @{
    /// Return the number of ptex patches for the given coarse face
    int		numPtexPatches(const TopologyRefiner &refiner,
			GT_Size face_index) const;
    int		numPtexPatches(const GT_OSDTopology &top,
			GT_Size face_index) const
		    { return numPtexPatches(*top.refiner(), face_index); }
    /// @}

    /// Return the coarse face associated with the given ptex id
    GT_Size	faceFromPtex(int ptex_id, int &offset) const;

    /// @{
    /// Query the points/vertices for a face in the coarse mesh
    bool	coarsePoints(const GT_OSDTopology &topology,
			    GT_Size coarse_face_index,
			    GT_Size *vertices,
			    GT_Size buffer_size) const;
    bool	coarseVertices(const GT_OSDTopology &topology,
			    GT_Size coarse_face_index,
			    GT_Size *vertices,
			    GT_Size buffer_size) const;
    /// @}

    /// Evaluate the limit surface (and derivatives) at a given coordinates.
    /// The @c faces array should have @c npts entries.  The @c u and @c v
    /// arrays should have also have @c npts entries.  Each @c u and @c v
    /// coordinate will be offset by @c uv_stride entries.  So, if you have
    /// uv's that are interleaved (i.e. [u0 v0 u1 v1 u2 v2 ...]), you can
    /// offset the @c v pointer and set the stride to 2.
    ///
    /// Passing a @c nullptr for any of the results will disable evaluation of
    /// that property.
    ///
    /// The method returns the tuple size of the attribute evaluated.
    int			limitSurface(GT_DataArrayHandle *result,
				GT_DataArrayHandle *result_du,
				GT_DataArrayHandle *result_dv,
				const GT_OSDAttributes &alist,
				const AttribId &attrib,
				int npts,
				const int32 *faces,
				const fpreal32 *u,
				const fpreal32 *v,
				int uv_stride = 1,
				int seg = 0) const;

    /// Evaluation of face-varying attributes on the limit surface
    int			limitSurfaceFVar(const GT_OSDTopology &topology,
				GT_DataArrayHandle *result,
				GT_DataArrayHandle *result_du,
				GT_DataArrayHandle *result_dv,
				const GT_OSDAttributes &alist,
				const AttribId &attrib,
				int npts,
				const int32 *faces,
				const fpreal32 *u,
				const fpreal32 *v,
				int uv_stride = 1,
				int seg = 0) const;

private:
    // Maps coarse face -> ptex
    UT_UniquePtr<PtexIndices>	myPtexIndices;
    // Holds all patches and also a map for ptex -> face
    const GT_OSDPatchTable	&myPatches;
    // Maps (coarse face + (u,v)) -> sub patch
    UT_UniquePtr<PatchMap>	myPatchMap;
    VertexStorage		myStorage;
    GT_CountArray		myFaceOffsets;
};

#endif
