/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFIndexPair.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface class for index pair access methods
 */

#ifndef __GA_AIFIndexPair__
#define __GA_AIFIndexPair__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>


class GA_Attribute;

class UT_JSONParser;
class UT_JSONWriter;
class UT_StringArray;


class GA_API GA_AIFIndexPairObjects
{
public:
    GA_AIFIndexPairObjects();
    virtual ~GA_AIFIndexPairObjects();

    virtual int64 getMemoryUsage(bool inclusive) const = 0;

    /// @{
    /// Query and set data about the objects each index references.
    virtual int getObjectCount() const = 0;
    virtual void setObjectCount(int nobj) = 0;

    virtual int getPropertyCount() const = 0;
    virtual bool getProperty(int propid, const char *&name, GA_Storage &storage, int &size) const = 0;

    virtual int addProperty(const char *name, const UT_FloatArray &def) = 0;
    virtual int addProperty(const char *name, const UT_IntArray &def) = 0;
    virtual int addProperty(const char *name, const UT_StringArray &def) = 0;

    virtual bool getPropertyDefaults(int propid, UT_FloatArray &def) const = 0;
    virtual bool getPropertyDefaults(int propid, UT_IntArray &def) const = 0;
    virtual bool getPropertyDefaults(int propid, UT_StringArray &def) const = 0;

    virtual void getObjectValue(int objid, int propid, fpreal32 &v, int component = 0) const = 0;
    virtual void getObjectValue(int objid, int propid, int32 &v, int component = 0) const = 0;
    virtual void getObjectValue(int objid, int propid, const char *&v, int component = 0) const = 0;

    virtual void getObjectValues(int objid, int propid, fpreal32 *v, int tuple_size) const = 0;
    virtual void getObjectValues(int objid, int propid, int32 *v, int tuple_size) const = 0;
    virtual void getObjectValues(int objid, int propid, const char **v, int tuple_size) const = 0;

    virtual void setObjectValue(int objid, int propid, fpreal32 v, int component = 0) = 0;
    virtual void setObjectValue(int objid, int propid, int32 v, int component = 0) = 0;
    virtual void setObjectValue(int objid, int propid, const char *v, int component = 0) = 0;

    virtual void setObjectValues(int objid, int propid, const fpreal32 *v, int tuple_size) = 0;
    virtual void setObjectValues(int objid, int propid, const int32 *v, int tuple_size) = 0;
    virtual void setObjectValues(int objid, int propid, const char * const*v, int tuple_size) = 0;

    virtual bool jsonSave(UT_JSONWriter &w) const = 0;
    virtual bool jsonLoad(UT_JSONParser &p) = 0;

    virtual bool copyProperties(const GA_AIFIndexPairObjects *src) = 0;
    virtual bool hasSameProperties(const GA_AIFIndexPairObjects *other) const = 0;
    virtual bool mergeAppendData(const GA_AIFIndexPairObjects *src, UT_IntArray &mapping) = 0;
    virtual bool mergeAppendDataSecondary(const GA_AIFIndexPairObjects *src, int offset, UT_IntArray &mapping) = 0;
    virtual void appendObject(const GA_AIFIndexPairObjects *src, int i) = 0;

    virtual void clear() = 0;
    /// @}

    int findProperty(const char *name, GA_Storage storage, int size) const;
    int findObjectValue(int propid, const char *value, int component=0) const;
};

/// @brief Generic Attribute Interface class to get/set data as index
/// pairs.
///
/// This class provides an interface to access attribute data.  Each
/// attribute type may provide this interface if it makes sense.
///
/// Index pair data can also contain a set of primary objects corresponding to
/// the indices.  These objects should be used to hold information about the
/// capture region for the associated index.  If the object contains a string
/// property, it should be unique (eg. capture path).  Use s=0 to manipulate
/// primary objects.  For example:
/// @code
///     // create primary object set
///     aif->setObjectSetCount(attrib, 1);
///     const GA_AIFIndexPairObjects *objects = aif->setObjects(attrib);
///     // create a string property for the objects called "path"
///     UT_StringArray sdef;
///     sdef.append("");
///     objects->addProperty("path", sdef);
///     // create a float16 property for the objects called "transform"
///     UT_FloatArray fdef;
///     for(int i = 0; i < 16; ++i)
///         fdef.append((i % 5 == 0) ? 1.0 : 0.0);
///     objects->addProperty("transform", fdef);
///     // add a new object
///     objects->setObjectCount(1);
/// @endcode
///
/// Index pair data can also contain a secondary set of objects for holding
/// multiple bits of subdata per index.  These objects should be used to hold
/// supplimentary data about objects such as the individual metaballs that make
/// up a muscle.  These secondary objects should also contain an integer
/// property identifying the index of the primary object.  Use s>0 to
/// manipulate secondary objects.
class GA_API GA_AIFIndexPair 
{
public:
	     GA_AIFIndexPair();
    virtual ~GA_AIFIndexPair();

    /// @{
    /// Query and set the number of object types (primary and secondary).
    virtual int		getObjectSetCount(const GA_Attribute *attrib) const = 0;
    virtual void	setObjectSetCount(GA_Attribute *attrib, int n) const = 0;
    /// @}

    /// @{
    /// Query and set data about the objects each index references.
    virtual const GA_AIFIndexPairObjects *getObjects(const GA_Attribute *attrib, int s = 0) const = 0;
    virtual GA_AIFIndexPairObjects *getObjects(GA_Attribute *attrib, int s = 0) const = 0;

    virtual bool        hasSameProperties(const GA_Attribute *attrib, const GA_Attribute *sattrib) const = 0;
    virtual bool        copyProperties(GA_Attribute *attrib, const GA_Attribute *sattrib) const = 0;
    virtual bool        mergeObjectValues(GA_Attribute *attrib, const GA_Attribute *sattrib, UT_IntArray &new_indices) const = 0;
    /// @}

    /// @{
    /// Query and set the entry count.
    virtual int		getEntries(const GA_Attribute *attrib) const = 0;
    virtual bool	setEntries(GA_Attribute *attrib, int n) const = 0;
    /// @}

    virtual int		getDataTupleSize(const GA_Attribute *attrib) const = 0;
    virtual bool	setDataTupleSize(GA_Attribute *attrib, int size) const = 0;

    virtual GA_Storage	getDataStorage(const GA_Attribute *attrib) const = 0;
    //virtual bool	setStorage(GA_Attribute *attrib, GA_Storage storage) const = 0;

    /// Get a specific (index, data[component]) pair from a given entry at a
    /// specified offset.
    /// @{
    virtual bool	getIndex(const GA_Attribute *attrib, GA_Offset ai, int entry, int32 &index) const = 0;
    virtual bool	getData(const GA_Attribute *attrib, GA_Offset ai, int entry, fpreal32 &data, int data_component = 0) const = 0;
    virtual bool	getData(const GA_Attribute *attrib, GA_Offset ai, int entry, fpreal64 &data, int data_component = 0) const = 0;
    virtual bool	getData(const GA_Attribute *attrib, GA_Offset ai, int entry, int32 &data, int data_component = 0) const = 0;
    virtual bool	getData(const GA_Attribute *attrib, GA_Offset ai, int entry, int64 &data, int data_component = 0) const = 0;
    /// @}

    /// Set a specific (index, data[component]) pair from a given entry at a
    /// specified offset.
    /// @{
    virtual bool	setIndex(GA_Attribute *attrib, GA_Offset ai, int entry, int32 index) const = 0;
    virtual bool	setData(GA_Attribute *attrib, GA_Offset ai, int entry, fpreal32 data, int data_component = 0) const = 0;
    virtual bool	setData(GA_Attribute *attrib, GA_Offset ai, int entry, fpreal64 data, int data_component = 0) const = 0;
    virtual bool	setData(GA_Attribute *attrib, GA_Offset ai, int entry, int32 data, int data_component = 0) const = 0;
    virtual bool	setData(GA_Attribute *attrib, GA_Offset ai, int entry, int64 data, int data_component = 0) const = 0;
    /// @}

    /// Removes all references to an index including per point references and
    /// primary and secondary objects.
    virtual bool	removeIndex(GA_Attribute *attrib, int index) const = 0;
};

#endif
