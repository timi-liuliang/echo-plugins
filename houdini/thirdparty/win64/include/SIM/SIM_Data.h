/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Data_h__
#define __SIM_Data_h__

#include "SIM_API.h"
#include <iosfwd>
#include <UT/UT_Guid.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_VectorTypes.h>
#include <GU/GU_DetailHandle.h>
#include "SIM_Error.h"
#include "SIM_Time.h"
#include "SIM_DataFactory.h"
#include "SIM_CacheCompression.h"

class UT_String;
class UT_InfoTree;
class UT_IStream;
class PRM_Template;
class SIM_Options;
class SIM_Data;
class SIM_Query;
class SIM_Guide;
class SIM_Engine;
class SIM_DataFilter;
class SIM_DataFactory;
class SIM_DataThreadedIO;
class SIM_DopDescription;
class SIM_RootData;
class OP_Node;
class OBJ_Node;
class SOP_Node;
class POP_Node;
class DOP_Node;
class COP2_Node;
class CHOP_Node;
class DEP_MicroNode;

/// This definition holds on to a collection of SIM_Data pointers.
typedef UT_ValArray<SIM_Data *> SIM_DataArray;
/// This definition holds on to a collection of const SIM_Data pointers.
typedef UT_ValArray<const SIM_Data *> SIM_ConstDataArray;

/// This class defines the callback object that is passed to
/// SIM_Data::forEachSubData() and SIM_Data::forEachConstSubData().
class SIM_API SIM_EachDataCallback
{
public:
			 SIM_EachDataCallback()
			 { }
    virtual		~SIM_EachDataCallback()
			 { }

    /// If your callback functions require the names of the data, override
    /// this function to return true. Otherwise the name parameter to the
    /// callback functions will always be null. Sending the name information
    /// to the callback introduces a small performance penalty.
    virtual bool	 callbackNeedsName() const
			 { return false; }
    /// This is the callback function that is called for each data found
    /// by SIM_Data::forEachConstSubData(). The name parameter is the
    /// relative path to the data item from the starting data item for the
    /// search (the startfrom parameter to SIM_Data::forEachConstSubData()).
    virtual void	 callbackConst(const SIM_Data *data,
				       const char *name) = 0;
    /// This is the callback function that is called for each data found
    /// by SIM_Data::forEachSubData(). The default implementation calls
    /// the version for const SIM_Data pointers. The name parameter is the
    /// relative path to the data item from the starting data item for the
    /// search (the startfrom parameter to SIM_Data::forEachSubData()).
    virtual void	 callback(SIM_Data *data, const char *name)
			 { callbackConst(data, name); }
};

/// Signifies that an existing non-const sub-data of the parent data can be
/// returned instead of creating new data. This constant can be used in the
/// creationflags parameter in the SIM_Data::createNamedSubData() function.
/// If this value is in the setflags parameter to SIM_Data::setNamedSubData(),
/// and any data with the specified name already exists, it is not deleted
/// and the new data is not attached.
#define SIM_DATA_RETURN_EXISTING		0x01
/// Signifies that the new data should be given a unique new name. If any
/// data with the same name already exists, instead of overwriting that
/// existing data the data name is modified to create a unique name. The
/// data name has numbers appended to it to create the unique name. If
/// SIM_DATA_RETURN_EXISTING is also set, this flag takes precedence
/// and new name for the new data will be created rather than returning the
/// existing data. This constant can be used in the creationflags parameter
/// in the SIM_Data::createNamedSubData() function or the setflags parameter
/// in SIM_Data::setNamedSubData().
#define SIM_DATA_CREATE_UNIQUE_NAME		0x02
/// Signifies that if a new sub-data is created to replace existing data,
/// that the existing data should be copied into the new data.  If an existing
/// sub-data with the given name is found, but is the wrong data type or the
/// SIM_DATA_RETURN_EXISTING constant is not set, then a new data will be
/// created to replace the existing data. If this constant is not set, the new
/// data uses all default values instead of trying to copy information from
/// the existing data. This constant can be used in the creationflags
/// parameter in the SIM_Data::createNamedSubData() function. This flag
/// is ignored by SIM_Data::setNamedSubData().
#define SIM_DATA_ADOPT_EXISTING_ON_DELETE	0x04

/// This enumeration provides options on how to deal with subdata when
/// calling makeEqual() or initialize(). The options are to eliminate
/// any existing subdata and replace it with the source subdata; or
/// to leave existing subdata, and copy any source subdata on top;
/// or to leave the existing subdata and not copy over any of the
/// source subdata. For calls to initialize(), where there is no
/// source data, the ignore and add operations have the same behavior.
/// COPYING will not invoke the myOptions initialize from parm
/// on the theory you plan on overwriting this later.
typedef enum {
    SIM_SUBDATA_REPLACE,
    SIM_SUBDATA_ADD,
    SIM_SUBDATA_IGNORE,
    SIM_SUBDATA_COPYING,
} SIM_SubdataBehavior;

/// This is the base class for all data types that are part of a simulation.
/// To work within a simulation, any data that you want to attach to a
/// simulation object must be a subclass of this type. A number of useful
/// subclasses of this type are provided, such as SIM_Geometry and
/// SIM_Position. Every SIM_Data subclass that is attached to a parent data
/// must be assigned a unique name, so the more data that can be put into
/// a single SIM_Data subclass the more efficient it will be. At the same
/// time, providing small discrete classes makes those classes more likely
/// to be usable in different circumstances.
class SIM_API SIM_Data
{
public:
    /// Return the unique descriptive name for the class.
    /// The returned value will always be the most-derived class for the
    /// data object. The name is fetched from the SIM_DataFactory that
    /// created this data.
    const UT_String	&getDataType() const;

    /// Set this data to its default empty state.
    /// This function calls initializeSubclass() to initialize the state of
    /// the data. After calling this function, the data should be
    /// indistinguishable from a new instance of this data type. This
    /// function is always called when a new piece of data is created.
    void		 initialize(const SIM_SubdataBehavior &subdatabehavior);
    /// Sets our data to be the same as the source data.
    /// This function calls makeEqualSubclass() to do the actual assignment.
    /// Note that it is legal to call makeEqual() with any pair of SIM_Data
    /// subclasses, not just related types. It is up to the makeEqualSubclass
    /// implementation to determine if the passed in data type is acceptable.
    void		 makeEqual(const SIM_Data *source,
				   const SIM_SubdataBehavior &subdatabehavior);
    /// Given a SIM_Options, set up this data.
    /// This function calls setParametersSubclass() to do its work. It is
    /// used by automatically generated DOP nodes to convert parameter values
    /// into SIM_Options entries, then set up the data. This function can
    /// also be used by loadSubclass() if some of the data for this type is
    /// stored in a SIM_Options.
    void		 setParameters(const SIM_Options &parms);
    /// Set this data to be some linear interpolation between two input data.
    /// This method is used by the simulation engine to generate approximate
    /// data between timesteps. It can also be used by solvers that sub-sample
    /// the global timestep, and for display purposes if the global time step
    /// is greater than one step time. This function calls interpolateSubclass.
    /// 
    /// Warning: if the source1 or source2 pointers are equal to the this
    /// pointer, this function will only do an assignment (via makeEqual)
    /// to one of the sources. 
    void		 interpolate(const SIM_Data *source1,
				     const SIM_Data *source2,
				     fpreal interp);
    /// This function returns the amount of memory consumed by this data.
    /// The returned value will not include the size of any subdata.
    int64		 getMemorySize() const;
    /// This function returns true if this data is built on demand from some
    /// other data. This function calls getIsAlternateRepresentationSubclass().
    /// This value is usually a characteristic of the data type as a whole, and
    /// will be the same for all data of a particular type.
    bool		 getIsAlternateRepresentation() const;

    /// Saves this piece of data to a file. This is the only public method
    /// for saving SIM_Data. It does not save subdata or creation info. It
    /// simply saves the contents of this single piece of data to a file.
    bool		 saveToFile(const char *filename,
			    UT_CompressionType compressionType) const;
    bool		 saveToStream(std::ostream &os,
			    UT_CompressionType compressionType) const;

    /// Loads this piece of data from a file. This file must be in a format
    /// like that saved from saveToFile(). No subdata or creation info is
    /// loaded (because it isn't saved in the file).
    bool		 loadFromFile(const char *filename);
    /// skiptype is set if the stream is already past the type data,
    /// for example due to an earlier call to getDataTypeFromStream
    bool		 loadFromStream(UT_IStream &is, bool skiptype=false);

    /// Opens the specified file and returns the type of data stored in that
    /// file. Having a static function here to look up this information from
    /// the file saves any callers from requiring any knowledge of the single
    /// data format.
    static bool		 getDataTypeFromFile(const char *filename,
					     UT_String &datatype);
    static bool		 getDataTypeFromStream(UT_IStream &is,
					     UT_String &datatype);

    /// Return whether we are of the specified type, or a subclass of it.
    /// This function calls the getPointerToType() function to make this
    /// determination.
    bool		 getIsOfType(const char *datatype) const;
    /// Return a const pointer that can be cast directly to the specified
    /// type. This function calls getDistanceToType(). It will return null if
    /// we can't be converted to the specified type. This function should
    /// normally not be called directly. Instead, the SIM_DATA_CASTCONST
    /// macro provides a safer method of using this function.
    const void		*getConstPointerToType(const char *datatype) const;
    /// Return a pointer that can be cast directly to the specified type.
    /// This function calls getDistanceToType(). It will return null if
    /// we can't be converted to the specified type. This function should
    /// normally not be called directly. Instead, the SIM_DATA_CAST
    /// macro provides a safer method of using this function.
    void		*getPointerToType(const char *datatype);
    /// This static function checks the passed in value for a null pointer
    /// then calls getConstPointerToType().
    static const void	*castConstPointerToType(const SIM_Data *data,
						const char *datatype);
    /// This static function checks the passed in value for a null pointer
    /// then calls getPointerToType().
    static void		*castPointerToType(SIM_Data *data,
					   const char *datatype);

    /// Get the number of sub-data items attached to this data.
    int			 getNumSubData() const;
    /// Get the name of the sub-data at a particular index location.
    const char		*getSubDataName(int index) const;
    /// Returns the index for a piece of sub-data. The return value is -1
    /// if the sub-data can't be found.
    int			 getSubDataIndex(const SIM_Data *subdata) const;

    /// Finds the given data as subdata.  Returns true if found and
    /// populates the path required to get there.  There is no preceding
    /// '/' on the returned path.  If subdata == this, "" is the path.
    bool		 buildSubDataPath(const SIM_Data *subdata, UT_String &pathto) const;

    /// Get a SIM_Query object associated with this data. The returned
    /// reference will generally be destroyed when the data is next modified.
    const SIM_Query	&getQueryObject() const;

    /// Returns a number that indicates the current state of the guide parms
    /// on our source node (or nodes). This value is used to automatically
    /// refresh our guide geometry if one of our guide parms changes. This
    /// function just calls getGuideParmVersionSubclass().
    long		 getGuideParmVersion() const;
    /// Adds a dependency to our guide from the given micronode,
    /// meant to be invoked inside of buildGuideGeometrySubclass
    void		 addGuideDep(DEP_MicroNode &src) const;
    /// Get the guide geometry list associated with this data. The returned value
    /// is true if at least one guide geometry has been generated, 
    /// false otherwise. Note, the detail and xform arrays will be cleared
    /// prior to use.
    virtual bool getGuideGeometryList(const SIM_RootData &root,
				      const char *datapath,
				      const SIM_Time &t,
				      UT_Array< GU_ConstDetailHandle > &detailArray,
				      UT_Array< UT_DMatrix4 > &xformArray) const;
    /// Get the guide geometry associated with this data. The returned value
    /// may be null if there is no guide geometry.
    GU_ConstDetailHandle getGuideGeometry(const SIM_RootData &root,
					  const char *datapath,
					  UT_DMatrix4 &xform,
					  const SIM_Time &t) const;
    /// Collect the information in our guide templates and store to a
    /// SIM_Options structure.
    void		 buildGuideOptions(SIM_Options &options,
					   const SIM_Time &time) const;

    /// Get the sub-data at a particular index location. If the found data
    /// is shared, then the data is copied before it is returned.
    SIM_Data		*getSubData(int index);
    /// This is the same as getSubData(), but it returns a const pointer.
    /// Also, the returned data is not copied even if it is shared.
    const SIM_Data	*getConstSubData(int index) const;
    /// Get the sub-data attached to this data with a particular name. If no
    /// sub-data with the specified name exists, null is returned. If the
    /// found data is shared, then the data is copied before it is returned.
    SIM_Data		*getNamedSubData(const char *dataname);
    /// Get the sub-data attached to this data with a particular name. If no
    /// sub-data with the specified name exists, null is returned. The
    /// returned data is not copied even if it is shared.
    const SIM_Data	*getConstNamedSubData(const char *dataname) const;

    /// Fills an array with all subdata pointers accepted by a filter.
    /// The startfrom parameter, if not null, is the name of a subdata
    /// item which acts as the root of the search. Data items accepted
    /// by the filter will be copied if they point to shared data.
    /// When searching recursively, this means that the parents of
    /// any returned data must also be copied. The names parameter,
    /// if not null, will be filled with the paths to the returned
    /// data items relative to the startfrom data. The recurseFilter
    /// determines which data items have their subdata items searched
    /// (and which of those subdata items have their subdata items
    /// searched, and so on).
    void		 filterSubData(SIM_DataArray &ncdp,
				UT_StringArray *names,
				const SIM_DataFilter &ncfilter,
				const char *startfrom,
				const SIM_DataFilter &recursefilter);
    /// Fills an array with all subdata pointers accepted by a filter.
    /// The startfrom parameter, if not null, is the name of a subdata
    /// item which acts as the root of the search. No data items will
    /// be copied by this function, even if they are shared. The names
    /// parameter, if not null, will be filled with the paths to the
    /// returned data items relative to the startfrom data. The
    /// recurseFilter determines which data items have their subdata
    /// items searched (and which of those subdata items have their
    /// subdata items searched, and so on).
    void		 filterConstSubData(SIM_ConstDataArray &dp,
				UT_StringArray *names,
				const SIM_DataFilter &filter,
				const char *startfrom,
				const SIM_DataFilter &recurseFilter) const;
    /// Returns the nth data item which matches the filter criteria.
    /// The startfrom parameter, if not null, is the name of a subdata
    /// item which acts as the root of the search. If the recurse
    /// parameter is true, then subdata are checked recursively.
    /// The data item returned by this function will be copied if it points
    /// to shared data. When searching recursively, this means that
    /// the parents of the returned data must also be copied.
    /// The name parameter, if not null, will be filled with the path to
    /// the returned data item relative to the startfrom data.
    /// Note that n starts at zero, in typical C++ fashion.
    SIM_Data		*getNthSubData(UT_String *name,
				const SIM_DataFilter &filter,
				int n,
				const char *startfrom,
				const SIM_DataFilter &recurseFilter);
    /// Returns the nth data item which matches the filter criteria.
    /// The startfrom parameter, if not null, is the name of a subdata
    /// item which acts as the root of the search. If the recurse
    /// parameter is true, then subdata are checked recursively. No
    /// data items will be copied by this function, even if they are shared.
    /// The name parameter, if not null, will be filled with the path to
    /// the returned data item relative to the startfrom data.
    /// Note that n starts at zero, in typical C++ fashion.
    const SIM_Data	*getNthConstSubData(UT_String *name,
				const SIM_DataFilter &filter,
				int n,
				const char *startfrom,
				const SIM_DataFilter &recurseFilter) const;
    /// Executes a callback function for each sub-data item that matches
    /// the filter criteria. The callback is called with each data item as a
    /// parameter. If the recurse parameter is true, this function searches
    /// each sub-data recursively, looking for sub-data items of its own.
    /// Data items accepted by the filter will be copied if they point
    /// to shared data. When searching recursively, this means that
    /// the parents of any returned data must also be copied.
    void		 forEachSubData(SIM_EachDataCallback &cb,
				const SIM_DataFilter &filter,
				const char *startfrom,
				const SIM_DataFilter &recurseFilter);
    /// Executes a callback function for each sub-data item that matches
    /// the filter criteria. The callback is called with each data item as a
    /// parameter. If the recurse parameter is true, this function searches
    /// each sub-data recursively, looking for sub-data items of its own. No
    /// data items will be copied by this function, even if they are shared.
    void		 forEachConstSubData(SIM_EachDataCallback &cb,
				const SIM_DataFilter &filter,
				const char *startfrom,
				const SIM_DataFilter &recurseFilter) const;

    /// Adds a new sub-data item to this data. The returned value is a pointer
    /// to the newly created sub-data. The creationflags parameter can be
    /// any combination of the SIM_DATA flags defined in this file.
    /// The SIM_DATA_CREATE macro provides an easy way to call this function
    /// and cast the return value in a single step.
    SIM_Data		*createNamedSubData(const char *dataname,
					    const char *datatype,
					    int creationflags,
					    UT_String *newdatanameptr=NULL);
    /// This function is a special version of the function above. It can
    /// only be used to create subdata that represents an alternate
    /// representation of this data. Because the new subdata is just
    /// another representation of this data, it can be added to a const
    /// SIM_Data. There is no creationflags parameter because this parameter
    /// is always implicitly SIM_DATA_RETURN_EXISTING.
    SIM_Data		*getOrCreateAlternateRepresentation(
					    const char *dataname,
					    const char *datatype) const;
    /// Creates a new sub-data item for this data by interpolating between
    /// two reference sub-data items. This function provides an easy way
    /// to interpolate certain sub-data items after making a copy of some
    /// existing data.
    void		 interpolateSubData(const SIM_Data &source1,
					    const SIM_Data &source2,
					    fpreal interp,
					    const SIM_DataFilter &interpdata,
					    const SIM_DataFilter &recurse);
    /// Add a new piece of sub-data to this data. If data with the given name
    /// already exists it will be deleted and replaced with the new data. The
    /// data pointer will most likely come from a call to createNamedData()
    /// or getSubData() on another parent data. The setflags parameter
    /// can be any combination of the SIM_DATA flags defined in this file.
    void		 setNamedSubData(const char *dataname,
					 const SIM_Data *data,
					 int setflags);
    /// Generates a unique sub-data name. Using the passed in dataname
    /// parameter as a starting point. this function generates a subdata
    /// name that is not currently in use. This function usually does not
    /// need to be called explicitly since it is called by setNamedSubData()
    /// and createNamedSubData() if the SIM_DATA_CREATE_UNIQUE_NAME flag
    /// is passed in.
    void		 createUniqueSubDataName(const SIM_Data *subdata,
						 UT_String &dataname) const;

    /// Remove some existing sub-data by name.
    void		 removeNamedSubData(const char *dataname);
    /// Remove some existing sub-data by index.
    void		 removeSubData(int index);
    /// Change the name of some existing sub-data found by name. The newname
    /// may specify a completely different location than the old name.
    void		 moveNamedSubData(const char *oldname,
					  const char *newname);

    /// Returns the unique identifier for this data. The unique identifier
    /// will be unique for all data over all time in this simulations.
    const UT_Guid	&getUniqueId() const;
    /// Returns the reference count for this data. This is the number of
    /// other data items that hold a pointer to this data.
    long		 getReferenceCount() const;
    /// Get the ID of the node that created this data.
    int			 getCreatorId() const;
    /// Use the creator ID to look up the OP_Node that created this data.
    OP_Node		*getCreatorNode() const;
    /// Look up the owner node of our engine to get the DOP Network pointer.
    OP_Node		*getOwnerNetwork() const;
    /// Get the engine that created us (from our data factory).
    const SIM_Engine	&getEngine() const
    { return myFactory->getEngine(); }
    /// Get the output index of the creator node that generated this data.
    int			 getCreatorIndex() const;
    /// Get the creation time for this data.
    const SIM_Time	&getCreationTime() const;

    /// Get the selected flag for this data.
    bool		 getIsSelected() const;
    /// Set the selected flag for this data. Notice this method is const.
    /// Selection isn't really part of the data, it is a UI concept.
    void		 setIsSelected(bool selected) const;
    /// This function returns true if selecting this data is equivalent to
    /// selecting the DOP object to which it is attached. This is currently
    /// a property of the data type.
    bool		 getSelectionRepresentsObject() const;

    /// Copy the creation info from the specified data into this data.
    /// This optional call after makeEqual() copies the creation info
    /// from the source data which is appropriate if this data is meant
    /// to replace or precisely represent the source data.
    void		 copyCreationInfoFrom(const SIM_Data *source);

    /// Returns the node that relative node paths are relative to. This
    /// function returns the creator node if there is one, otherwise it
    /// returns the OP_Director.
    OP_Node		&getNodePathsRelativeTo() const;
    /// Given a path relative to our creator node, return an OBJ_Node.
    OP_Node		*getOPNode(const char *path, bool addinterest) const;
    /// Given a path relative to our creator node, return an OBJ_Node.
    OBJ_Node		*getOBJNode(const char *path, bool addinterest) const;
    /// Given a path relative to our creator node, return a SOP_Node.
    SOP_Node		*getSOPNode(const char *path, bool addinterest) const;
    /// Given a path relative to our creator node, return a POP_Node.
    POP_Node		*getPOPNode(const char *path, bool addinterest) const;
    /// Given a path relative to our creator node, return a DOP_Node.
    DOP_Node		*getDOPNode(const char *path, bool addinterest) const;
    /// Given a path relative to our creator node, return a COP2_Node.
    COP2_Node		*getCOP2Node(const char *path, bool addinterest) const;
    /// Given a path relative to our creator node, return a CHOP_Node.
    CHOP_Node		*getCHOPNode(const char *path, bool addinterest) const;
    /// Adds an interest in the specified node to our engine's owner node.
    void		 addOPInterest(OP_Node *node) const;

    /// Adds an error to our SIM_Engine.
    void		 addError(const SIM_RootData *root,
				  int errorcode,
				  const char *errorparm,
				  UT_ErrorSeverity severity) const;

protected:
    /// The SIM_Data constructor when being created by a SIM_DataFactory.
    /// All SIM_Data subclasses should have protected constructors to ensure
    /// that only the SIM_DataFactory can create objects of this type.
    explicit		 SIM_Data(const SIM_DataFactory *factory);
    /// The SIM_Data destructor. It is very important that SIM_Data
    /// created by a SIM_DataFactory only be deleted by that SIM_DataFactory.
    virtual		~SIM_Data();

    /// Call this function whenever the internal data of this object changes.
    /// The code parameter can be used to signal what sort of modification
    /// has occurred. The meaning of this code can be different from one
    /// data type to the next. This function calls handleModificationSubclass.
    void		 handleModification(int code = -1);

    /// This function flags the data as needing to recalculate its memory
    /// size. The default handleModificationSubclass() implementation calls
    /// this function, but it can also be called directly when the memory
    /// size of a SIM_Data is changing without any changes that would
    /// require a handleModification() call.
    void		 setNeedsToRecalculateMemorySize() const;

    /// Deletes the query object for this data, if there is one.
    void		 deleteQueryObjectIfNotBuilding() const;

    /// Delete the guide geometry for this data, if it exists.
    void		 deleteGuideObjectIfNotBuilding() const;

    /// This function is called from getOrCreateAlternateRepresentation().
    /// It should be used to initialize the alternate representation from
    /// the parent data. It calls initAlternateRepresentationSubclass()
    /// to do the actual work.
    void		 initAlternateRepresentation() const;

    /// Flags an alternate representation subdata as requiring initialization.
    void		 setNeedsInitialization(bool needsinit) const;
    /// Tests whether an alternate representation requires initialization.
    bool		 getNeedsInitialization() const;

    /// This function saves a SIM_Options to a stream.
    /// Using a SIM_Options provides an easy way to allow for forward
    /// and backward compatibility when new data is added or removed.
    bool		 saveOptionPacket(std::ostream &os, const char *name,
					  const SIM_Options *opts) const;
    /// This function loads a SIM_Options from a stream.
    /// Using a SIM_Options provides an easy way to allow for forward
    /// and backward compatibility when new data is added or removed.
    bool		 loadOptionPacket(UT_IStream &is, const char *name,
					  SIM_Options *opts) const;
    /// Returns true if we are performing a simplified load or save. A
    /// simplified load or save is one performed from the saveToFile or
    /// loadFromFile functions.
    bool		 getIsDoingSimplifiedSaveLoad() const;

    /// This function looks through the parameter templates defined in
    /// our SIM_DopDescription, loads the default values into a SIM_Options
    /// structure, and calls setParameters(). This function is useful for
    /// initializing data types that use the SIM_DopDescription to create
    /// an automatic DOP node type.
    void		 initializeFromParmDefaults();

    /// Sets the flag indicating that selecting this object represents
    /// a selection of the object to which it is attached.
    void		 setSelectionRepresentsObject(bool representsobject);

    /// Override this method to invalidate cached data based on object
    /// geometry.
    virtual void setNeedsInitializationSubclass(bool needsinit) const;
    /// Override this method to set this data to its default empty state.
    /// Remember to call the base class implementation. The default
    /// implementation clears all subdata, and if the data is a subclass
    /// of SIM_OptionsUser, calls initializeFromParmDefaults().
    virtual void	 initializeSubclass();
    /// Override this method to set subclass data equal to the source data.
    /// The source parameter at this level is guaranteed to be non-null.
    /// Remember to call the base class implementation.
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    /// Override this method to write out subclass-specific data.
    /// Remember to call the base class implementation.
    virtual void	 saveSubclass(std::ostream &os) const;

    /// Save variant that allows you to delay part of your saving to
    /// a background thread.
    /// If not overloaded, passes through to saveSubclass.
    /// If io is NULL, shoudl not attempt threaded loading.
    virtual void	 saveIOSubclass(std::ostream &os, SIM_DataThreadedIO *io) const;
    /// Override this method to read in subclass-specific data.
    /// Remember to call the base class implementation.
    virtual bool	 loadSubclass(UT_IStream &is);

    /// Loading variant that allows you to request loading data at the
    /// end of the .sim file that was saved with saveIOSubclass
    /// If not overloaded, passes through to loadSubclass.
    /// If io is NULL, shoudl not attempt threaded loading.
    virtual bool	 loadIOSubclass(UT_IStream &is, SIM_DataThreadedIO *io);

    /// Override this method to create a custom SIM_Query for this class.
    /// By creating a custom SIM_Query, it is possible to customize the
    /// expression function interface to the data.
    virtual SIM_Query	*createQueryObjectSubclass() const;
    /// Override this if the guide geometry for this data depends on
    /// parameters on nodes other than our creator node. Otherwise the
    /// guide geometry won't refresh when changing those parameters.
    virtual long	 getGuideParmVersionSubclass() const;
    /// Override this to create a custom SIM_Guide subclass. The SIM_Guide
    /// class controls how requests for guide geometry are translated into
    /// actual geometry creation.
    virtual SIM_Guide	*createGuideObjectSubclass() const;
    /// Override this function to create custom guide geometry for this class.
    /// This method should add geometry to the gdp (if supplied), not
    /// alter anything that is already there. The xform will always default
    /// to an identity matrix. The gdp pointer or the xform pointer
    /// may be null, in which case the other should still be calculated.
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;
    /// Override this method to process our DOP node parameter values.
    /// Automatically created DOP nodes turn their parameter values into
    /// SIM_Options entries, which are passed to this function. This
    /// function can also be used to help load data from a stream if it
    /// is stored in a SIM_Options. Remember to call the base class
    /// implementation.
    virtual void	 setParametersSubclass(const SIM_Options &parms);
    /// Override this virtual to do special processing when adding subdata.
    /// One example would be to cache certain pieces of subdata for quicker
    /// access through member functions. Remember to also handle the
    /// removeNamedSubDataSubclass function to handle the removal of
    /// subdata.
    virtual void	 setNamedSubDataSubclass(const char *dataname,
						 const SIM_Data *data);
    /// Override this function to do special processing when removing subdata.
    /// One example would be to eliminate a cached value set up in the
    /// setNamedSubDataSubclass() function.
    virtual void	 removeNamedSubDataSubclass(const char *dataname);
    /// Override this method to customize the interpolation behavior.
    /// The interp parameter will always be greater than 0 and less than
    /// one in this call. Values outside this range are handled by the
    /// interpolate() function by calling makeEqual() on one source or
    /// the other. The default implementation calls makeEqualSubclass() on
    /// source1 or source2 depending on whether interp is less or greater
    /// than 0.5. Both source1 and source2 are guaranteed to be non-null in
    /// this call.  Subclasses should not call the base class implementation,
    /// except in the case of an error condition which prevents the custom
    /// interpolation from functioning properly. You should also not call
    /// makeEqual() from this function, though makeEqualSubclass() is
    /// acceptable.
    virtual void	 interpolateSubclass(const SIM_Data *source1,
					     const SIM_Data *source2,
					     fpreal interp);
    /// Override this function to return an accurate representation of
    /// the amount of memory used by this piece of data. The size of
    /// subdata should not be included in this calculation.
    virtual int64	 getMemorySizeSubclass() const;
    /// Override this method to customize the behavior when our internal
    /// data changes. The code parameter is a data type specific value which
    /// can be used to indicate the type of change that has occurred. The
    /// default implementation deletes all subdata that was created with
    /// the deleteifmodified flag turned on. Remember to call the base
    /// class implementation.
    virtual void	 handleModificationSubclass(int code);
    /// Override this method to return true if your data type is built on
    /// demand. This is true for data that is just an alternative
    /// representation of some other data. This has several consequences.
    /// First, it implies that the data can be rebuilt from other data
    /// on the object, therefore it is not saved to disk. Also, it is
    /// assumed to be represent its parent data, so any modifications to
    /// the parent data cause this data to be deleted. If you override
    /// this method, do not call the base class implementation.
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    /// Override this function if getIsAlternateRepresentationSubclass()
    /// is overridden to return true. This function is used to initialize
    /// the alternate representation based on the data to which it was
    /// just attached.
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);
    /// Return this pointer cast us to another data type. All subclasses
    /// should implement this function, but generally the
    /// DECLARE_STANDARD_GETCASTTOYPE() macro will work. Only if your
    /// class uses multiple inheritance will it be necessary to write a
    /// custom version of this function. If you write your own custom
    /// implementation of this method, remember to call the base class
    /// implementation.
    virtual void	*getCastToType(const char *datatype) const;
    /// Returns the class name for this data type. This function is
    /// automaticaly implemented by the DECLARE_CLASSNAME() and
    /// DECLARE_DATAFACTORY() macros.
    virtual const UT_String	&getDataTypeSubclass() const;

    const SIM_Data *getAlternateRepresentationOf() const
			{ return myAlternateRepresentationOf; }

    /// An empty parm template.
    /// This provides a simple default value to use for those SIM_Data
    /// subclasses that don't want to define a parm template.
    static const PRM_Template		*getEmptyTemplateList();
    /// A DOP description that says not to create an automatic DOP.
    static const SIM_DopDescription	*getEmptyDopDescription();

    static void	getDataTypeSuperclasses(UT_StringArray& /*classes*/) { }

private:
    /// Make copy constructor private to disable it.
			 SIM_Data(const SIM_Data &)
			 { }
    /// Make operator= private to disable it.
    const SIM_Data	&operator=(const SIM_Data &)
			 { return *this; }

    /// Save our data to a stream. This function can only be called by
    /// SIM_Data or SIM_SimulationState. This function calls saveSubclass()
    /// to do the actual save operation.
    void		 save(std::ostream &os, SIM_DataThreadedIO *io) const;
    /// Read in data from a file. This function can only be called by
    /// SIM_Data or SIM_SimulationState. This function calls loadSubclass()
    /// to load data from the stream into this object. This function also
    /// calls initialize() first, to ensure the data is in a pristine state
    /// before attempting the load.
    bool		 load(UT_IStream &is, SIM_DataThreadedIO *io);
    /// Creates a SIM_Query object associated with this data. This function
    /// is called by SIM_Data in the getQueryObject() function if no query
    /// object yet exists for this data. It calls createQueryObjectSubclass().
    SIM_Query		*createQueryObject() const;
    /// Creates the guide geometry controller for this data. This function
    /// can return null if no geuide geometry is desired.
    SIM_Guide		*createGuideObject() const;
    /// Actually builds the guide geometry and guide transform. This method
    /// is called by SIM_Guide. Either or the gdp or xform pointers may be
    /// null if only one piece of information is required.
    bool		 buildGuideGeometry(const SIM_RootData &root,
					    const GU_DetailHandle &gdh,
					    UT_DMatrix4 *xform,
					    const SIM_Time &t) const;

    /// Adjust the reference count and state references for a piece of
    /// subdata. The subdata must be attached to this data.
    void		 decrementReferenceCount();
    /// Adjust the global and state-specific reference counts for a piece
    /// of subdata. The subdata must be attached to this data.
    void		 incrementReferenceCount() const;
    /// This method goes through all our subdata and replaces all instances
    /// of from with to. No bookkeeping is done here, it is just a brute
    /// for search and replace. We also recurse into all subdata.
    void		 changeSubData(int pos, SIM_Data *from, SIM_Data *to);

    /// This is a helper function that gives us a place to put a lot of
    /// code that would otherwise need to be duplicated in filterSubData
    /// and filterConstSubData.
    void		 filterDataHelper(SIM_Data *caller,
					  SIM_ConstDataArray *dp,
					  SIM_DataArray *ncdp,
					  UT_StringArray *names,
					  const SIM_DataFilter *filter,
					  const SIM_DataFilter *ncfilter,
					  const char *startfrom,
					  const SIM_DataFilter &rFilter) const;
    /// Clear out all our sub-data.
    void		 clearAllSubData();
    /// Sets the unique id for this piece of data. This is used when we
    /// need to set the unique id to a particular value (as when loading
    /// data from a file) or to set the unique id to a new value if we
    /// are doing a non-const access to some existing data.
    void		 setUniqueId(const UT_Guid &newid);

    /// Increment the reference count on a particular sub data name.
    static void		 incrementNameReferenceCount(const char *subdataname);
    /// Decrement the reference count on a particular sub data name.
    static void		 decrementNameReferenceCount(const char *subdataname);

    /// This class stores information about our subdata if we have any.
    class SIM_API SIM_SubDataInfo
    {
	public:
	    UT_ValArray<SIM_Data *>	 mySubData;
	    UT_StringArray               mySubDataNames;
    };

    const SIM_DataFactory		*myFactory;
    SIM_SubDataInfo			*mySubDataInfo;
    SIM_Options				*myOptions;
    UT_Guid				 myUniqueId;
    int					 myCreatorId;
    int					 myCreatorIndex;
    SIM_Time				 myCreationTime;
    bool				 myHandlingModification;
    bool				 mySelectionRepresentsObject;
    mutable int64			 myMemorySize;
    mutable long			 myReferenceCount;
    mutable SIM_Query			*myQueryObject;
    mutable SIM_Guide			*myGuideObject;
    mutable const SIM_Data		*myAlternateRepresentationOf;
    mutable bool			 myNeedsInitialization : 1;
    mutable bool			 myBuildingQueryObject : 1;
    mutable bool			 myBuildingGuideObject : 1;
    mutable bool			 myDoingSimplifiedSaveLoad : 1;
    mutable bool			 myIsSelected : 1;
    mutable bool			 myExistsOnDisk : 1;
    static UT_StringMap<exint> 		 theSubDataNames;

    friend class			 SIM_DataFactory;
    friend class			 SIM_SimulationState;
    friend class			 SIM_OptionsUser;
    friend class			 SIM_Guide;
};

#endif
