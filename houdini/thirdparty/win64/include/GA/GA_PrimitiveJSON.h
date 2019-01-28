/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveJSON.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveJSON__
#define __GA_PrimitiveJSON__

#include "GA_API.h"
#include <UT/UT_JSONValue.h>
#include <SYS/SYS_Types.h>

class UT_JSONWriter;
class UT_JSONParser;
class UT_FSA;
class GA_Primitive;
class GA_SaveMap;
class GA_LoadMap;
class UT_StringArray;

/// Structure to allow GA_JSONUniformBlock to delete whatever data may be
/// cached on it using a virtual destructor.
struct GA_API GA_JSONUniformData
{
    virtual ~GA_JSONUniformData() {}
};

/// This allows primitives cache parsed uniform data, to avoid having to
/// re-parse it for every primitive using the same data.  That had been
/// a significant bottleneck for curve bases, but it should save time
/// for the closed flag on all faces as well.
struct GA_API GA_JSONUniformBlock
{
    GA_JSONUniformBlock()
        : myMap(NULL)
        , myData(NULL)
    {}
    ~GA_JSONUniformBlock()
    {
        delete myMap;
        delete myData;
    }
    UT_JSONValue *myMap;
    mutable GA_JSONUniformData *myData;
};

/// @brief Provide a JSON interface to a primitive
///
/// This class is used in saving and loading of geometry.
///
/// @see GA_PrimitiveDefinition, UT_JSONWriter, UT_JSONParser
///
/// When sequences of primitives appear in order in the geometry, it's possible
/// to compact the storage by saving the primitive instance data in a "run".
/// This is done by storing the "fields" which appear for each primitive.  The
/// run data is an array with each item in the array representing the data
/// fields for the individual primitive.
///
/// This eliminates the repetition of the JSON keys for each primitive.
/// For example, the following two objects represent the same data:
/// @code
/// "primitives" : [
///    [ ["type":"polygon"], ["vertices":[0,1,2], "closed":true ] ],
///    [ ["type":"polygon"], ["vertices":[3,4,5], "closed":true ] ],
///    [ ["type":"polygon"], ["vertices":[6,7,8], "closed":true ] ],
///    [ ["type":"polygon"], ["vertices":[9,0,1], "closed":true ] ]
/// ]
/// 
/// "primitives" : [
///    [ "type"     :"run",
///      "runtype"  :"polygon",
///      "varyingfields":["vertices"],
///      "uniformfields":{"closed":true},
///    ],
///    [
///      [ [0,1,2] ],
///      [ [3,4,5] ],
///      [ [6,7,8] ],
///      [ [9,0,1] ]
///     ]
/// ]
/// @endcode
///
/// @section JSON-GA_Primitive JSON Schema: GA_Primitive
/// A primitive is stored as an array with two entries.  The first entry stores
/// the definition of the primitive.  The second entry stores the private data
/// for the primitive.
///
/// The private data is handled by the GA_PrimitiveJSON associated with the
/// primitive type.
///
/// @code
/// {
///     "name"        : "GA_Primitive",
///     "description" :
///           "A primitive is stored in an array of two items.  The first
///            item stores the definition (or run information).  The second
///            entry stores the private information for the primitive (or run
///            of primitives).",
///     "type"  : "array",
///     "items" : [
///        { "$ref" : "GA_Primitive-Definition" },  // Definition of primitive
///        {
///          "$ref" : "GA_Primitive-Data",      // Single primitive data
///          "$ref" : "GA_Primitive-RunData",   // Data for run of primitives
///        },
///     ],
/// }
/// @endcode
///
/// @section JSON-GA_Primitive-Definition JSON Schema: GA_Primitive-Definition
/// The primitive definition may be the definition for a single primitive, or
/// it may store run data.  The run fields are optional, but if one is defined,
/// all should be defined.
/// See also: @ref JSON-GA_Primitive
/// @code
/// {
///     "name"        : "GA_Primitive-Definition",
///     "description" : "Definition of a primitive (or run of primitives)",
///     "type"        : "orderedmap",
///     "properties":
///     {
///         "type":
///         {
///		"type" : "string"
///		"description"   :
///			"Type of primitive.  'run' indicates a run of
///			 primitives (see runtype, uniformfields and
///			 varyingfields)",
///	    }
///         "runtype" :
///         {
///		"type"		:"string",
///		"description"   :"Type of primitive in run",
///		"optional"	:true,
///         },
///         "uniformfields" :
///         {
///             "type"          : "object",
///             "description"   :
///			"A map of the fields.  The value specified is the same
///			 for all primitives in a run",
///		"optional"	:true,
///         },
///         "varyingfields" :
///         {
///             "type"          : "array",
///		"items"		: "string",
///             "description"   :
///			"A list of strings which are varying for over the run
///			 of primitives.  The run data will be stored in the
///			 order of the strings in this list."
///		"optional"	:true,
///         },
///	},
/// }
/// @endcode
///
/// @section JSON-GA_Primitive-RunData JSON Schema: GA_Primitive-RunData
/// This section stores the private data for a run of primitives.  The data is
/// processed by the GA_PrimitiveJSON class associated with the primitive.
/// The data is stored in an 
/// @code
/// {
///     "name"        : "GA_Primitive-Data",
///     "description" :
///		"Private data for a run of primitives.  Each entry represents
///		 a distinct primitive.",
///     "type":  "array",
///     "items": "any",
/// }
/// @endcode
///
/// @section JSON-GA_Primitive-HowTo
/// This section describes how to write load/save methods for a primitive.
///
/// The high-level interface to primitive save/load is provided by the methods
/// - <tt>getEntries()</tt>
/// - <tt>getKeyword(int i)</tt>
/// - <tt>saveField(const GA_Primitive*, int, UT_JSONWriter&, const GA_SaveMap&)</tt>
/// - <tt>saveField(const GA_Primitive*, int, UT_JSONValue&, const GA_SaveMap&)</tt>
/// - <tt>load(const GA_Primitive*, int, UT_JSONParser&, const GA_LoadMap&)</tt>
/// - <tt>load(const GA_Primitive*, int, UT_JSONParser&, UT_JSONValue&, const GA_LoadMap&)</tt>
/// - <tt>isEqual(int i, const GA_Primitive*, const GA_Primitive *)</tt>
///
/// The private schema is defined by a number of fields in a dictionary/map.
/// For example, a polygon might have two fields in its private schema:
/// # Vertex List
/// # Open/Closed Flag
/// which would have an interface which looked like: @code
///     int getEntries() const { return 2; }
///     const UT_StringHolder &getKeyword(int i) { return i == 0 ? "vertex"_sh : "closed"_sh; }
///     bool saveField(const GA_Primitive *prim, int i, UT_JSONWriter &w,
///                     const GA_SaveMap &map) const {
///             if (i == 0) return myVertexList->save(w, map);
///             return w.jsonBool(myClosedFlag);
///          }
///     bool saveField(const GA_Primitive *prim, int i, UT_JSONValue &v,
///                     const GA_SaveMap &map) const {
///             if (i == 0) return myVertexList->save(v, map);
///             v.setBool(myClosedFlag);
///             return true;
///          }
///     bool loadField(GA_Primitive *prim, int i, UT_JSONParser &p,
///                     const GA_LoadMap &map) const {
///             if (i == 0) return myVertexList->load(p, map);
///             return p.parseBool(myClosed);
///          }
///     bool loadField(const GA_Primitive *prim, int i, UT_JSONParser &p,
///                     const UT_JSONValue &v, const GA_SaveMap &map) const {
///             if (i == 0) return false; // Never load from a value
///             return v.import(myClosed);
///          }
///     bool isEqual(int i, const GA_Primitive *p0, const GA_Primitive *p1) {
///             if (i == 0) return false; // Never save runs of vertex lists
///             return p0->myClosed == p1->myClosed;
///          }
/// @endcode
///
class GA_API GA_PrimitiveJSON
{
public:
	     GA_PrimitiveJSON();
    virtual ~GA_PrimitiveJSON();

    /// Return the number of data fields in the primitive private schema
    virtual int		getEntries() const = 0;

    /// Return whether the i'th field should be saved to the JSON stream.  By
    /// default, all fields are saved.  However, this allows the object to
    /// "skip" some fields which don't make sense in some circumstances.
    /// @note At the current time, this method is ignored when saving run data.
    virtual bool	 shouldSaveField(const GA_Primitive *prim, int i,
					const GA_SaveMap &save) const;

    /// Return the keyword associated with the i'th entry of the schema.
    /// Keywords are case insensitive, but must be specified as @b lower case
    /// by this method.
    virtual const UT_StringHolder	&getKeyword(int i) const = 0;

    /// Save the i'th entry of the primitive's schema to the JSON stream.  You
    /// can assume that the GA_Primitive is the correct type (i.e. a static
    /// cast is ok).
    virtual bool	saveField(const GA_Primitive *prim, int i,
				UT_JSONWriter &w,
				const GA_SaveMap &save) const = 0;

    /// Save the i'th entry of the primitive's schema to a JSON Value.  This
    /// method should only be called when saving uniform data, which only
    /// happens if the equality checking returned true for the i'th entry.  If
    /// the comparison fails, then this method will never be called.
    ///
    /// The default method just calls: @code
    ///    UT_AutoJSONWriter	w(value);
    ///	   return saveField(prim, i, *w, save);
    /// @endcode
    virtual bool	saveField(const GA_Primitive *prim, int i,
				UT_JSONValue &value,
				const GA_SaveMap &save) const;

    /// Load the i'th entry of the primitive's private schema from a JSON
    /// stream.
    virtual bool	loadField(GA_Primitive *prim, int i,
				UT_JSONParser &p,
				const GA_LoadMap &load) const = 0;

    /// Load the i'th entry of the primitive's private schema from a JSON value.
    /// The parser is passed in so that errors/warnings can be added.
    ///
    /// The default implementation just calls: @code
    ///	    UT_AutoJSONIStream	is(p, value);
    ///	    return loadField(prim, i, p, load);
    /// @endcode
    virtual bool	loadField(GA_Primitive *prim, int i,
				UT_JSONParser &p,
				const UT_JSONValue &value,
				const GA_LoadMap &load) const;

    /// Determine if the i'th entry of a primitive's private schema is equal to
    /// the value in another primitive.  This determines whether the data can
    /// be created as "uniform" data a run of sequential primitives.
    virtual bool	isEqual(int i,
				const GA_Primitive *prim1,
				const GA_Primitive *prim2) const = 0;

    /// Save a single primitve to the UT_JSONWriter
    /// By default, this simply saves an ordered dictionary (list) with the
    /// private schema.  That is: @code
    ///	w.beginArray();
    ///	for (int i = 0; i < getEntries(); i++) {
    ///	    w.jsonKeyToken(getKeyword(i));
    ///	    saveField(prim, i, w, map);
    /// }
    /// return json.endArray();
    /// @endcode
    virtual bool	save(const GA_Primitive *prim,
				UT_JSONWriter &w,
				const GA_SaveMap &map) const;

    /// Save a single primitive to a UT_JSONValue
    virtual bool	save(const GA_Primitive *prim,
				UT_JSONValue &v,
				const GA_SaveMap &map) const;

    /// Load a single primitive from the UT_JSONParser.  By default, this
    /// method loads the primitive from an ordered dictionary (as output by the
    /// save method).
    virtual bool	load(GA_Primitive *prim,
				UT_JSONParser &p,
				const GA_LoadMap &map) const;

    /// Load a single primitive from a UT_JSONValue
    virtual bool	load(GA_Primitive *prim,
				UT_JSONParser &p,
				const UT_JSONValue &value,
				const GA_LoadMap &map) const;


protected:
    /// The GA library may optimize by saving multiple primitives of the same
    /// type in a "run".  This can be done if the primitive is represented by a
    /// simple map where all primitives share common fields.
    /// For example, spheres will all have:
    /// @li center     A vertex reference for the center of the sphere
    /// @li transform  The scale/rotate transform for the primitive
    ///
    /// getRunFields() is called only if there is a run of multiple primitives.
    /// The string array returned should be the list of all fields that the
    /// primitive needs to save.  For the sphere example, it would return:
    /// [ "center", "transform" ].
    ///
    /// @param list @n
    ///   List of primitives to check for uniform values.
    /// @param list_size @n
    ///   Size of the list of primitives in the run (see GA_SaveMap,
    ///   geo:primrunqueue).
    /// @param varying @n
    ///	  The list of fields which are different for each primitive
    /// @param uniform @n
    ///	  This will be a UT_JSONValue::JSON_MAP value.  Values which are
    ///   common to every primitive can be placed in this list (instead of
    ///   the varying list.  Only objects which can be easily loaded from
    ///   a UT_JSONValue object should be put into the uniform list, since the
    ///   uniform list is pre-loaded when loading runs of primitives.
    /// @param user_flags @n
    ///   This is an integer which can be set to any value.  It is passed
    ///   verbatim to matchRun() and saveRunFields().  This can be used to pass
    ///   information about the run fields.
    ///
    /// The base class returns a NULL pointer (indicating saving runs is
    /// impossible).
    ///
    /// The user_flags is an unsigned integer value which you can modify how
    /// you wish.  It's passed to the matchRun() and saveRunFields() verbatim.
    ///
    /// The default method returns @c false (no run is possible)
    virtual bool	getRunFields(const GA_Primitive **list,
					int list_size,
					const GA_SaveMap &save,
					UT_StringArray &varying,
					UT_JSONValue &uniform,
					uint &user_flags) const;

    /// When saving a list of primitives, the library attempts to create "runs"
    /// of like primitives.  The library will create an initial run of
    /// primitives (the length of which is determined by geo:primrununique in
    /// the GA_SaveMap).  This initial list determines the data elements which
    /// are uniform and varying in the run of primitives.
    ///
    /// After this initial run, additional primitives can be added to the run
    /// if they match.  This method is called to test whether the primitive
    /// matches the current run (prim0 is the first primitive in the run, prim1
    /// is the primitive to test).
    ///
    /// The @c user_flags are the flags set in @c getRunFields().
    virtual bool	matchRun(const GA_Primitive *prim0,
					const GA_Primitive *prim1,
					const UT_StringArray &varying,
					const UT_JSONValue &uniform,
					uint user_flags) const;

    /// saveRunFields() should save a JSON array of values, one for each field
    /// in the varying list of fields.
    /// The default method returns @c false.
    virtual bool	saveRunFields(const GA_Primitive *prim,
					const GA_SaveMap &save,
					const UT_StringArray &varying,
					UT_JSONWriter &w,
					uint user_flags) const;

    /// Load a single primitive from a run of primitives.
    /// Returns false if an error occurred, else true.
    virtual bool	load(GA_Primitive *prim,
					UT_JSONParser &p,
					const GA_LoadMap &load,
					const UT_StringArray &varying,
					const GA_JSONUniformBlock &uniform) const;

    /// Load, and optionally cache, the uniform data for a run of primitives.
    /// Returns false if an error occurred, else true.
    virtual bool        loadUniform(GA_Primitive *prim,
                                    UT_JSONParser &p,
                                    const GA_LoadMap &map,
                                    const GA_JSONUniformBlock &uniform) const;

    /// When the primitive loader hits an unknown map token, it will call this
    /// method to let the primitive process it.  The default behaviour is to
    /// add a warning to the parser and skip the next object.
    ///
    /// If the method returns false, parsing will stop and the load will fail.
    virtual bool	loadUnknownToken(GA_Primitive *prim,
					const char *token,
					UT_JSONParser &p,
					const GA_LoadMap &load) const;

protected:
    void		 init();

    UT_FSA		*myTokens;
    int			 myEntries;

    friend class	 GA_PrimitiveList;
    friend class	 GA_PrimitiveRun;
};

#endif
