/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFEdit.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_AIFEdit__
#define __GA_AIFEdit__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>


class GA_AIFIndexPairObjects;
class GA_Attribute;
class GA_IndexMap;
class GA_Range;

class UT_JSONParser;
class UT_JSONWriter;
class UT_String;
class UT_StringArray;


/// @brief Class for mapping between an index and an attribute data offset.
///
/// GA_AIFEditDelta objects store element deltas by an ordered index, not
/// necessarily corresponding to the element's order in an GA_IndexMap.
/// This class maps between that index and the corresponding data offset
/// in a particular attribute.
///
/// The simplest map is to use the ordered index from a GA_IndexMap as the
/// delta index.
/// TODO: Add a virtual getDeltaIndexTraverser which yields data offsets in
///       delta index order.  This may be more efficient for
///       GA_AIFEditDelta::apply

class GA_API GA_AIFEditDeltaMap 
{
public:
    virtual ~GA_AIFEditDeltaMap() {}

    virtual GA_Offset	 getDataOffset(GA_Index delta_index) const = 0;
    virtual GA_Index	 getDeltaIndex(GA_Offset data_offset) const = 0;
};

/// @brief Simple GA_AIFEditDeltaMap mapping data offset to element's ordered
//  index.

class GA_API GA_AIFEditDeltaIndexMap : public GA_AIFEditDeltaMap 
{
public:
    GA_AIFEditDeltaIndexMap(const GA_IndexMap &index_map)
	: myIndexMap(index_map) {}

    virtual GA_Offset	 getDataOffset(GA_Index delta_index) const;
    virtual GA_Index	 getDeltaIndex(GA_Offset data_offset) const;
private:
    const GA_IndexMap	&myIndexMap;
};

/// @brief Base class for supplying coordinate frames.
///
/// Coordinate frames are used to map between a delta relative to that
/// coordinate frame's local space and one relative to world space.  A
/// coordinate frame will typically be stored for each index stored in
/// the GA_AIFEditDelta.

class GA_API GA_AIFEditDeltaCoordFrames 
{
public:
    virtual ~GA_AIFEditDeltaCoordFrames() {}

    virtual UT_Matrix3R	 getLocalToWorldTransform(GA_Index) const = 0;
    virtual UT_Matrix3R	 getWorldToLocalTransform(GA_Index) const = 0;
};

/// @brief Base class for specifying symmetry for operations.
///
/// The symmetry class is used to generalize symmetry at the individual
/// element level.

class GA_API GA_AIFEditSymmetry 
{
public:
    virtual ~GA_AIFEditSymmetry() {}

    /// Return the delta index for the element symmetric to the element that
    /// corresponds to the given input_delta_index, -1 if none.  If symmetry
    /// is found, xform will be set to the transform that will map the input
    /// element's delta to the corresponding delta for the output element.
    virtual GA_Index	 findSymmetry(GA_Index input_delta_index,
				      const GA_AIFEditDeltaMap &map,
				      UT_Matrix3R &xform) = 0;
};

/// @brief Base class for storing snapshots during an edit operation.
///
/// Opaque base class for snapshots of attribute values taken during an edit
/// operation.

class GA_API GA_AIFEditSnapshot 
{
public:
    virtual ~GA_AIFEditSnapshot() {}
};

/// @brief Interface class for loading legacy tuple delta data.
///
/// An interface class to allow a GA_AIFEditDelta implementation to load a
/// legacy stream without specific knowledge of the stream format.
class GA_API GA_AIFEditDeltaTupleLegacyStream
{
public:
    virtual ~GA_AIFEditDeltaTupleLegacyStream() {}

    virtual int		 getTupleSize() const = 0;
    virtual GA_Size	 getDeltaCount() const = 0;

    /// @{
    /// Extracting a delta will consume it, so these methods should be called
    /// exactly getDeltaCount() times.
    virtual bool	 extractDelta(GA_Index &index,
				      UT_Fpreal64Array &delta) = 0;
    virtual bool	 extractDelta(GA_Index &index,
				      UT_Int64Array &delta) = 0;
    virtual bool	 extractDelta(GA_Index &index,
				      UT_Fpreal32Array &delta) = 0;
    virtual bool	 extractDelta(GA_Index &index,
				      UT_Int32Array &delta) = 0;
    /// @}
};

/// @brief Interface class for loading legacy index pair delta data.
///
/// An interface class to allow a GA_AIFEditDelta implementation to load a
/// legacy stream without specific knowledge of the stream format.
class GA_API GA_AIFEditDeltaIndexPairLegacyStream
{
public:
    virtual ~GA_AIFEditDeltaIndexPairLegacyStream() {}

    virtual int		 getObjectCount() const = 0;

    virtual int		 getPropertyCount() const = 0;
    virtual bool	 getPropertyInfo(int prop_index, UT_String &name,
					 GA_Storage &storage,
					 int &size) const = 0;
    virtual bool	 getPropertyDefault(int prop_index,
					    UT_StringArray &def) const = 0;
    virtual bool	 getPropertyDefault(int prop_index,
					    UT_IntArray &def) const = 0;
    virtual bool	 getPropertyDefault(int prop_index,
					    UT_FloatArray &def) const = 0;
    virtual bool	 getPropertyValue(int obj_index, int prop_index,
					  UT_StringArray &value) const = 0;
    virtual bool	 getPropertyValue(int obj_index, int prop_index,
					  UT_IntArray &value) const = 0;
    virtual bool	 getPropertyValue(int obj_index, int prop_index,
					  UT_FloatArray &value) const = 0;

    /// This method should be called once per object immediately before
    /// extracting that object's deltas.
    virtual GA_Size	 extractObjectDeltaCount() = 0;

    /// Extracting a delta will consume it, so this method should be called
    /// exactly extractObjectDeltaCount() times for each object.
    /// @code
    ///     for (i = 0; i < stream.getObjectCount(); i++)
    ///     {
    ///         GA_Size delta_count = stream.extractObjectDeltaCount();
    ///
    ///         for (j = 0; j < delta_count; j++)
    ///             if (!stream.extractObjectDelta(delta_index, delta_data))
    ///                 return error();
    ///     }
    ///
    /// @endcode
    virtual bool	 extractObjectDelta(GA_Index &index,
					    fpreal &delta) = 0;
};

/// @brief Base class to store recorder data.
///
/// This class is responsible for maintaining changes to an attribute.  The
/// class must be able to save/load to a JSON stream.
///
/// The deltas are used by painting, editing and other modelling operations.
/// For example, an undo mechanism might be implemented by:
/// @code
/// changeAttrib()
/// {
///	GA_AIFEditDelta		*delta;
///	GA_Attribute		*P;
///	GA_AIFEditSnapshot	*snapshot;
///	delta = P->getAIFEdit()->createDelta(); // TODO: NULL ptr check
///	snapshot = delta->startEdit(P, GA_Range(P->getIndexMap()));
///	letUserEditPoints(P);
///	delta->endEdit(snapshot, GA_AIFEditDeltaIndexMap(P->getIndexMap()));
/// }
///
/// doUndoRedo(GA_Attribute *P, GA_AIFEditDelta *delta, bool undo)
/// {
///	delta->applyEdit(P, GA_AIFEditDeltaIndexMap(P->getIndexMap()), undo);
/// }
/// @endcode
class GA_API GA_AIFEditDelta 
{
public:
	     GA_AIFEditDelta();
    virtual ~GA_AIFEditDelta();

    virtual int64   getMemoryUsage(bool inclusive=false) const = 0;

    /// Create a new (empty) instance of the derived class.
    virtual GA_AIFEditDelta	*createNew() const = 0;

    /// Create a copy of this delta.
    virtual GA_AIFEditDelta	*clone() const = 0;

    /// Test compatibility with a particular attribute.
    virtual bool	isCompatible(const GA_Attribute *attrib) const = 0;

    /// Test compatibility with another delta.  Compatibility in this case
    /// refers to ability to merge the deltas.
    virtual bool	isCompatible(const GA_AIFEditDelta *delta) const = 0;

    /// The @c startEdit() method allows the snapshot to see the state of
    /// the attribute @b before any edits are made to the data.
    virtual GA_AIFEditSnapshot	*startEdit(const GA_Attribute *attrib,
					  GA_Offset element) = 0;
    virtual GA_AIFEditSnapshot	*startEdit(const GA_Attribute *attrib,
					  const GA_Range &elements) = 0;
    /// After editing, the @c endEdit() method is called.  This allows the
    /// computation of the actual delta from the snapshot.  Do not use the
    /// snapshot again after calling this method.  The optional coordinate
    /// frames will be used to store the delta relative to the local frame
    /// if supported.
    virtual bool	endEdit(GA_AIFEditSnapshot *snapshot,
				const GA_AIFEditDeltaMap &map,
				const GA_AIFEditDeltaCoordFrames *frames=0) = 0;

    /// Merge a separate edit into this edit, matching elements by the
    /// internal ordered index.  The optional coordinate frames will be
    /// used to store the delta relative to the local frame if supported.
    virtual bool	mergeEdit(const GA_AIFEditDelta *other,
				  const GA_AIFEditDeltaCoordFrames *otherfr = 0,
				  const GA_AIFEditDeltaCoordFrames *localfr = 0,
				  bool inverse = false) = 0;

    /// Merge the edits symmetric to those in a separate edit into this edit,
    /// matching elements by internal ordered index.  The optional coordinate
    /// frames will be used to store the delta relative to the local frame if
    /// supported.
    virtual bool	mergeSymmetricEdit(const GA_AIFEditDelta *other,
				  const GA_AIFEditDeltaCoordFrames *otherfr,
				  GA_AIFEditSymmetry &symmetry,
				  const GA_AIFEditDeltaMap &map,
				  const GA_AIFEditDeltaCoordFrames *localfr,
				  bool inverse = false) = 0;

    /// Apply this snapshot's edit delta to the attribute, optionally
    /// restricting which delta indices get applied.  The optional coordinate
    /// frames will be used to apply deltas previously mapped to local space
    /// in world space (if supported).
    virtual bool	applyEdit(GA_Attribute *attrib,
				  const GA_AIFEditDeltaMap &map,
				  const UT_Int64Array *subset = 0,
				  const GA_AIFEditDeltaCoordFrames *frames = 0,
				  bool inverse = false) const = 0;

    /// Scale and apply this snapshot's edit delta to the attribute, optionally
    /// restricting which delta indices get applied.  This operation is not
    /// supported by all attributes supporting AIF_Edit.  The optional
    /// coordinate frames will be used to apply deltas previously mapped to
    /// local space in world space (if supported).
    virtual bool	applyScaledEdit(GA_Attribute *attrib,
				  fpreal scale,
				  const GA_AIFEditDeltaMap &map,
				  const UT_Int64Array *subset = 0,
				  const GA_AIFEditDeltaCoordFrames *frames = 0,
				  bool inverse = false) const = 0;

    /// @{
    /// Clear currently stored deltas.  The first method, zeroDeltas(), is a
    /// lighter-weight clear that will endeavour to hold onto and reuse most
    /// allocated memory.
    virtual void	zeroDeltas() = 0;
    virtual void	clearDeltas() = 0;
    /// @}

    /// Query the number of indices that have recorded deltas.
    virtual GA_Size	getDeltaCount() const = 0;

    /// @section JSON-GA_AIFEditDelta JSON Schema: GA_AIFEditDelta
    ///
    /// The schema for a delta consists of an ordered map (array) with two
    /// elements.  The first, "type", is an ordered pair (array) providing
    /// the information needed to allocate a subclass of GA_AIFEditDelta.
    /// The second, "data", holds. the private data for the delta, which is
    /// saved and loaded by the GA_AIFEditDelta subclass.
    ///
    /// @code
    /// [
    ///    "name"   : "GA_AIFEditDelta",
    ///    "description" :
    ///          "A delta is defined by an ordered map (array) with two
    ///           elements, the definition ("type") and the private data
    ///           ("data").
    ///     "type" : "orderedmap",
    ///     "properties": {
    ///         "type": {
    ///             "type"          : "string",
    ///             "description"   : "Ordered pair (type name, args)",
    ///         },
    ///         "data": {
    ///             "type"        : "string",
    ///             "description" : "Internal delta data",
    ///         },
    ///     }
    /// ]
    /// @endcode
    ///
    /// @section JSON-GA_AIFEditDelta-Type JSON Schema: GA_AIFEditDelta-Type
    /// The delta "type" field is an ordered pair containing the attribute type
    /// and an object holding additional type arguments needed to recreate the
    /// delta.
    /// @code
    /// {
    ///     "name"          : "GA_AIFEditDelta-Type",
    ///     "description"   : "An ordered pair defining the attribute",
    ///     "type"          : "array",
    ///     "items": [
    ///         <"Attribute Type Name">, <"Type Arguments">
    ///     ],
    /// }
    /// @endcode
    /// @see @ref GA_AttributeType
    static bool		jsonSaveDelta(UT_JSONWriter &w,
				      const GA_AIFEditDelta *delta);
    static bool		jsonLoadDelta(UT_JSONParser &p,
				      GA_AIFEditDelta *&delta);

    /// GA_AttributeType::jsonLoadDeltaDefinition() is the counterpart to this
    /// method.
    virtual bool	jsonSaveDefinition(UT_JSONWriter &w) const = 0;

    /// Save the delta object to a JSON stream.
    virtual bool	jsonSave(UT_JSONWriter &w) const=0;
    /// Load a delta object from a JSON stream
    virtual bool	jsonLoad(UT_JSONParser &p) = 0;

    /// Load a legacy tuple delta.  Only supported by some subclasses.
    virtual bool	legacyLoadTuple(
				    GA_AIFEditDeltaTupleLegacyStream &);
    /// Load a legacy index pair delta.  Only supported by some subclasses.
    virtual bool	legacyLoadIndexPair(
				    GA_AIFEditDeltaIndexPairLegacyStream &);

    /// @{
    /// Query and set information about objects referenced by index pair data.
    /// Only supported by some subclasses.
    virtual int getObjectSetCount() const;
    virtual void setObjectSetCount(int n);
    virtual const GA_AIFIndexPairObjects *getObjects(int s = 0) const;
    virtual GA_AIFIndexPairObjects *getObjects(int s = 0);
    /// @}

    /// @{
    /// The new & delete operators are overloaded to help with memory tracking.
    /// Allocate new object
    static void	*operator	new(size_t size);
    /// In-place construction
    static void	*operator	new(size_t size, void *p);
    /// Deletion of an allocated object
    static void	 operator	delete(void *p, size_t size);
    /// Get memory statistics
    static void			getAllocationStats(int64 &used, int64 &peak);
    /// @}
};

/// @brief AIF to record changes to attribute values
class GA_API GA_AIFEdit 
{
public:
    /// Constructor
    GA_AIFEdit();
    /// Destructor
    virtual ~GA_AIFEdit();

    /// Allocate a delta object for the attribute
    virtual GA_AIFEditDelta	*createDelta(const GA_Attribute *a) const=0;
};

#endif

