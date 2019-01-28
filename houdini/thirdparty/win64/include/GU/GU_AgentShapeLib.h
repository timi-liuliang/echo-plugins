/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentShapeLib.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentShapeLib__
#define __GU_AgentShapeLib__

#include "GU_API.h"
#include "GU_DetailHandle.h"
#include "GU_LinearSkinDeformer.h"
#include <GA/GA_Types.h>
#include <UT/UT_Lock.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <SYS/SYS_AtomicInt.h>
#include <iosfwd>

class UT_JSONWriter;
class UT_JSONParser;
class GEO_Primitive;

class GU_AgentShapeLib;
typedef UT_IntrusivePtr<GU_AgentShapeLib>	GU_AgentShapeLibPtr;
typedef UT_IntrusivePtr<const GU_AgentShapeLib>	GU_AgentShapeLibConstPtr;
typedef UT_Array<GU_AgentShapeLibConstPtr>	GU_AgentShapeLibArray;

/// Library for Agent Shapes
///
/// An agent shape library stores multiple shapes. There are various ways to add
/// shapes to the library (add a full piece of geometry, add geometry and a
/// range of primitives, etc.).
///
/// Shapes are identified by "name" which must be unique within the library.
///
class GU_API GU_AgentShapeLib : public UT_IntrusiveRefCounter<GU_AgentShapeLib>
{
public:
    class Shape;

    typedef UT_IntrusivePtr<const Shape>    ShapePtr;
    typedef UT_StringMap<ShapePtr>          ShapeMap;
    typedef ShapeMap::const_iterator        const_iterator;
    typedef ShapeMap::iterator	            iterator;
    typedef void (*ShapeRemovedCallback)(void *data, int shape_id);

public:
    /// Create a new library using the given name.
    static GU_AgentShapeLibPtr	addLibrary(const char *name);

    /// Load a new library from disk, named using the filename. Returns NULL if
    /// failed to load.
    static GU_AgentShapeLibPtr	addLibraryFromFile(
				    const char *filename,
				    UT_StringArray& errors);

    /// Create a clone of a library for modification. The clone will have
    /// isFile() return false.
    /// If keep_external_ref is true and the source library was a file
    /// reference, the source library will be marked as an included file of the
    /// new library. Otherwise, the new library will be standalone.
    static GU_AgentShapeLibPtr  addLibraryCopy(const GU_AgentShapeLib &src,
                                               bool keep_external_ref);

    /// Register a callback to be invoked when a shape is no longer referenced
    /// by any shape library.
    static void                 registerShapeRemovedCallback(
                                    ShapeRemovedCallback cb, void *data = 0);
    static void                 unregisterShapeRemovedCallback(
                                    ShapeRemovedCallback cb, void *data = 0);

private:
    /// Use the static addLibrary() method to create shape libraries.
				GU_AgentShapeLib(
				    const char *name,
				    bool is_file);

public:
				~GU_AgentShapeLib();

    int64                       getMemoryUsage(bool inclusive) const;

    /// Save the shape library.
    bool			save(UT_JSONWriter &w) const;
    /// Load a shape library.
    bool			load(UT_JSONParser &p);

    /// Add entire geometry as a shape in the library.
    bool			addShape(
				    const UT_StringHolder &key,
				    const GU_ConstDetailHandle &gdp,
				    bool replace_existing=true);
    /// Add a disk file as a shape.
    bool			addShape(
				    const UT_StringHolder &key,
				    const char *filename,
				    bool replace_existing=true);

    /// Remove a shape from the library
    bool			removeShape(const UT_StringRef &key);

    /// @{
    /// Query information
    bool			isEmpty() const
				    { return myShapeCache.empty(); }
    exint			entries() const
				    { return myShapeCache.size(); }
    /// @}

    /// Get a reference to the underlying geometry (which holds all the shapes)
    GU_ConstDetailHandle	detail() const	{ return myDetail; }

    /// Find a shape
    ShapePtr			findShape(const UT_StringRef &key) const;

    /// The name of the shape library (or filename, if loaded from disk).
    /// @{
    const UT_StringHolder&	name() const { return myName; }
    void                        setName(const UT_StringHolder &name)
                                    { myName = name; }
    /// @}

    /// Return whether the library was loaded from disk.
    bool			isFile() const { return myIsFile; }
    /// Clear the flag marking that the library references a file on disk.
    void                        clearIsFile();

    /// @{
    /// Set a user defined label for the library.  This doesn't have to be
    /// unique across libraries.
    const char*			label() const;
    void			setLabel(const char *label);
    /// @}

    /// Dump the contents of this shape library for debugging purposes.
    /// If save_file is supplied, the shape library's geometry will be saved
    /// using that name.
    void			dump(std::ostream& os,
				     const char* save_file = 0) const;

    /// Get the names of each dangling shape in the shape library. A dangling 
    /// shape is a shape that is only referenced by the shape library.
    void			getDanglingShapes(
				     UT_SortedStringSet &names) const;

    /// @{
    /// Iterators
    const_iterator	begin() const;
    const_iterator	end() const;
    iterator		begin();	
    iterator		end();
    /// @}


private: // methods
    /// Include the contents of another shape library. The included library
    /// should be an external reference to a file on disk.
    /// Saving this shape library will only write out the non-included shapes,
    /// plus external references to the included shape libraries.
    void                        includeShapeLib(
                                    const GU_AgentShapeLibConstPtr &shapelib);

    void			initAttributes(GU_Detail *gdp);
    void			destroyShape(const_iterator &it);
    bool			replaceExistingShape(
				    const UT_StringHolder &key,
				    bool replace_existing);
    bool			addShape(
				    const UT_StringHolder &key,
				    GEO_Primitive *prim);

private: // data
    UT_StringHolder		myName;
    bool			myIsFile;
    GU_DetailHandle		myDetail;
    ShapeMap			myShapeCache;

    GU_AgentShapeLibArray       myIncludedShapeLibs;
};

class GU_API GU_AgentShapeLib::Shape
    : public UT_IntrusiveRefCounter<GU_AgentShapeLib::Shape>
{
    /// Common data for shapes that are shared between different shape
    /// libraries. In addition to the packed primitive's geometry, the unique
    /// ID and GU_LinearSkinDeformerSource are shared between copies.
    class GU_API ShapeData : public UT_IntrusiveRefCounter<ShapeData>
    {
    public:
        ShapeData();
        ~ShapeData();

        int64 getMemoryUsage(bool inclusive) const;
        int uniqueId() const { return myUniqueId; }

        const GU_LinearSkinDeformerSource &
        getDeformerSource(const GU_ConstDetailHandle &gdh) const;

    private:
        int myUniqueId;
        mutable GU_LinearSkinDeformerSource *myDeformerSource;
    };
    typedef UT_IntrusivePtr<ShapeData> ShapeDataPtr;

public:
    Shape(GA_Offset offset);
    Shape(const Shape &shape);

    int64 getMemoryUsage(bool inclusive) const;

    GA_Offset offset() const { return myOffset; }
    void setOffset(GA_Offset offset) { myOffset = offset; }
    int uniqueId() const { return myShapeData->uniqueId(); }

    /// Given its owner shape library, return the geometry for this shape.
    GU_ConstDetailHandle shapeGeometry(const GU_AgentShapeLib& lib) const;

    typedef GU_LinearSkinDeformerSource DeformerSource;

    /// Get a GU_LinearSkinDeformerSource given the detail handle for this
    /// shape.
    /// @pre gdh.isValid()
    const DeformerSource &
    getDeformerSource(const GU_ConstDetailHandle &gdh) const
    {
        return myShapeData->getDeformerSource(gdh);
    }

private:
    GA_Offset myOffset;
    ShapeDataPtr myShapeData;
};

#endif
