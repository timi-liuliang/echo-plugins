/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_LightLink.h (OBJ Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OBJ_LightLink__
#define __OBJ_LightLink__

#include "OBJ_API.h"

#include <UT/UT_Set.h>
#include <UT/UT_Map.h>
#include <UT/UT_TagManager.h>
#include <GA/GA_Types.h>
#include <SOP/SOP_Node.h>
#include <SOP/SOP_ObjectAppearance.h>

class UT_JSONWriter;
class UT_JSONParser;
class OP_Network;
class OBJ_Node;

/// Helper class to assist with managing tags and expressions on objects.
///
/// The class contains a simple representation for objects that have tag
/// expressions (i.e. objects which have a light selection expression).  This
/// class allows you to make edits to the tag expressions using a list object
/// centric view.  That is, the object that stores the list of tags can
/// add/remove tags from the expression objects.
class OBJ_API OBJ_LightLink
{
public:
    typedef std::string		ObjectId;
    typedef std::string		SourceTagList;
    typedef UT_TagListPtr	CompiledTagList;
    typedef std::string		SourceTagExpression;
    typedef UT_TagExpressionPtr	CompiledTagExpression;
    typedef UT_Set<ObjectId>	ObjectSet;

    /// Open a chooser of all objects, selecting the objects with the given
    /// tag.  On completion, the user selected objects will be lit by the tag.
    void	runLightSelection(const char *title,
		    const char *tag,
		    bool include_packed=true,
		    OP_Network *parent = NULL);

    /// The @c node_path refers to the geometry object (or SOP).  When
    /// referring to a packed SOP, the @c packed_path refers to the path
    /// attribute.  As a note, when referring to a SOP, the SOP must support
    /// the SOP_ObjectAppearance interface.
    bool	runObjectCentric(const char *title,
		    OBJ_Node *node, const char *packed_path=NULL) const;

    class ObjectData
    {
    public:
	ObjectData()
	    : myInitial()
	    , myValue()
	    , myOpId(-1)
	{
	}
	ObjectData(const ObjectData &src)
	    : myInitial(src.myInitial)
	    , myValue(src.myValue)
	    , myOpId(src.myOpId)
	{
	}
	void	initialize(const CompiledTagExpression &expr, int opid)
	{
	    myOpId = opid;
	    myInitial = expr;
	    myValue = expr;
	}
	/// Indicate whether the object is changed from its original value
	bool	changed() const;

	/// Remove the given tag from the object, returning true if the
	/// object changed.
	bool	removeTag(const char *tag);

	/// Add the given tag to the object, returning true if the object
	/// changed.
	bool	addTag(const char *tag);

	/// Get the expression as a string
	void	exprSource(UT_WorkBuffer &expr) const;

	/// @{
	/// Accessors
	const CompiledTagExpression	&expr() const	{ return myValue; }
	int				 opid() const	{ return myOpId; }
	/// @}

    private:
	CompiledTagExpression	myInitial;
	CompiledTagExpression	myValue;
	int			myOpId;
    };
    /// There's one SOPData per SOP.  This is used for editing all paths in the
    /// SOP.
    class SOPData
    {
    public:
	/// The appearance pointer is used to edit light values.
	/// The path start is the offset of the SOP's path
	SOPData(const std::string &soppath,
		const SOP_ObjectAppearancePtr &app)
	    : myPath(soppath)
	    , myAppearance(app)
	{
	}
	SOPData(const SOPData &src)
	    : myPath(src.myPath)
	    , myAppearance(src.myAppearance)
	{
	}
	void	initialize(const std::string &soppath,
			    const SOP_ObjectAppearancePtr &app)
	{
	    myPath = soppath;
	    myAppearance = app;
	}
	bool	addSelectedPaths(UT_TagManager &tags,
				ObjectSet &selection,
				const char *tag)
	{
	    return myAppearance->getLightSelection(tags, myPath,
			selection, tag);
	}
	bool	editLights(UT_TagManager &tags,
			const ObjectSet &sel,
			const char *tag)
	{
	    return myAppearance->editLightSelection(tags, myPath, sel, tag);
	}
	const SOP_ObjectAppearancePtr	&appearance() const	{ return myAppearance; }
	const std::string	&path() const		{ return myPath; }
    private:
	SOP_ObjectAppearancePtr	myAppearance;
	std::string		myPath;
    };
    typedef UT_Map<ObjectId, ObjectData>	ObjectMap;
    typedef UT_Array<SOPData>			SOPList;

     OBJ_LightLink();
    ~OBJ_LightLink();

    /// Helper function to populate the light linker database with objects from
    /// the given network.
    void	populateFromNetwork(OP_Network *net, fpreal t);

    /// Add geometry which can be lit or might not be lit
    bool	addObject(const ObjectId &path,
			const SourceTagExpression &expr,
			int opid=-1);
    bool	addSOP(const std::string &soppath,
			const SOP_ObjectAppearancePtr &app);

    /// Get objects which match a given tag.
    /// Non-const since tag manager is modified.
    void	getMatching(ObjectSet &objects, const char *tag);

    /// The given set of @c objects will be set to match the @c tag.  All other
    /// objects will be adjusted to no longer match the given @c tag.  The list
    /// of @c changed objects is returned.
    void	setMatching(const ObjectSet &objects,
			const char *tag, fpreal t);

    /// Set all objects to be matching the given tag.
    void	setAllMatching(const char *tag, fpreal t);

    /// Get a list of all objects modified by set operations
    void	getChanged(ObjectSet &objects);

    /// Get the expression source for the given object
    bool	getSourceExpression(const ObjectId &path,
			UT_WorkBuffer &expr) const;

    /// Export lighting information to a file
    /// Save options:
    ///	   - "save_material" [True] @n
    ///		Save materials for objects/SOPs
    ///    - "save_visibility" [True] @n
    ///		Save visibility for objects/SOPs
    ///    - "save_linking" [True] @n
    ///		Save linking information for objects/SOPs
    ///    - "root" ["/obj"] @n
    ///		Root of network to save
    ///
    bool	exportLighting(UT_JSONWriter &os,
			const UT_Options &save_options,
			UT_StringArray &errors) const;
    /// Import lighting information from a file
    bool	importLighting(UT_JSONParser &is,
			const UT_Options &load_options,
			UT_StringArray &errors) const;

private:
    ObjectMap		myObjects;
    SOPList		mySOPs;
    UT_TagManager	myTags;
};

#endif

