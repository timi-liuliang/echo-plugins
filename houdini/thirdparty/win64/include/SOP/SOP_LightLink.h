/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_LightLink.h (SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_LightLink__
#define __SOP_LightLink__

#include "SOP_API.h"
#include <UT/UT_Map.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_Array.h>
#include <UT/UT_Pair.h>
#include <UT/UT_Set.h>
#include <UT/UT_TagManager.h>
#include <GEO/GEO_PackedTypes.h>

/// Class to manage visibility/material/light linking for packed primitives.
///
/// This class represents the paths for the SOP as a full tree.  Operations can
/// be performed on branches (including light linking operations).  After the
/// tree has been edited, you can get the operations by calling one of:
///	- getMaterialOps()
///	- getExprOps()
///	- getVisibleOps()
class SOP_API SOP_LightLink
{
public:
    /// Set operations are represented as (path, value), where @c path is the
    /// path to the node in the tree, and @c value is the value for the
    /// tree/subtree.
    struct SOP_API VisibleInfo
    {
	VisibleInfo(int v, GEO_ViewportLOD l)
	    : myVisible(v)
	    , myViewportLOD(l)
	{
	}
	int		myVisible;
	GEO_ViewportLOD myViewportLOD;
    };
    typedef UT_Pair<std::string, std::string>	SetOp;
    typedef UT_Pair<std::string, VisibleInfo>	VisibleOp;

    static std::string	theAsterisk;

    /// Node to represent paths in the link tree
    /// Each branch has one or more children.
    /// If all children of the branch have the same value, the branch will have
    ///		myValue set to the common child value
    ///		myKidsSame set to true
    class Node
    {
    public:
	typedef UT_Map<std::string, Node *>	NodeSet;

	/// Visibility states
	enum
	{
	    VISIBLE_UNCHANGED = -1,
	    VISIBLE_HIDE,
	    VISIBLE_SHOW
	};

	// Flags for common child values
	enum
	{
	    COMMON_LOD		= 0x01,
	    COMMON_MATERIAL	= 0x02,
	    COMMON_EXPR		= 0x04,

	    COMMON_ALL = (COMMON_MATERIAL|COMMON_LOD|COMMON_EXPR)
	};


	Node(const std::string &name, Node *parent)
	    : myName(name)
	    , myMaterial()
	    , myVisible(VISIBLE_UNCHANGED)
	    , myViewportLOD(GEO_VIEWPORT_INVALID_MODE)
	    , myExpr(theAsterisk)
	    , myParent(parent)
	    , myKids()
	    , myCommonKids(COMMON_ALL)
	{
	}
	~Node()
	{
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		delete it->second;
	}

	// Find or add a node
	Node	*addChild(const std::string &name)
	{
	    NodeSet::iterator	it = myKids.find(name);
	    if (it == myKids.end())
	    {
		Node	*kid = new Node(name, this);
		myKids[name] = kid;
		return kid;
	    }
	    return it->second;
	}
	Node	*find(const std::string &name) const
	{
	    NodeSet::const_iterator	it = myKids.find(name);
	    if (it == myKids.end())
		return NULL;
	    return it->second;
	}
	exint		 childCount() const
	{
	    return myKids.size();
	}
	void		 clearValue()
	{
	    myMaterial = std::string();
	    myVisible = true;
	    myViewportLOD = GEO_VIEWPORT_INVALID_MODE;
	    myExpr = theAsterisk;
	}
	void	 clearSubtree()
	{
	    clearValue();
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		it->second->clearSubtree();
	}
	void	 setMaterial(const std::string &m)
	{
	    myMaterial = m;
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		it->second->setMaterial(m);
	}
	void	setExpr(const std::string &expr)
	{
	    myExpr = expr;
	    myExprPtr.reset(NULL);
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
	    {
		it->second->setExpr(expr);
	    }
	}
	void	clearTagExpressions()
	{
	    if (myExprPtr)
		updateTagExpression();
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		it->second->clearTagExpressions();
	}
	void	addTag(UT_TagManager &mgr, const UT_TagList &tags)
	{
	    createTagExpression(mgr);
	    if (myExprPtr)
		myExprPtr = myExprPtr->addTags(tags);
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		it->second->addTag(mgr, tags);
	}
	void	rmTag(UT_TagManager &mgr, const UT_TagList &tags)
	{
	    createTagExpression(mgr);
	    if (myExprPtr)
		myExprPtr = myExprPtr->rmTags(tags);
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		it->second->rmTag(mgr, tags);
	}
	void	setVisible(int visibility)
	{
	    myVisible = visibility;
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		it->second->setVisible(visibility);
	}
	void	setViewportLOD(GEO_ViewportLOD lod)
	{
	    myViewportLOD = lod;
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
		it->second->setViewportLOD(lod);
	}
	void		optimizeTree()
	{
	    UT_ASSERT(!myParent);
	    pruneBranches();
	}
	void	extractLeafPaths(UT_WorkBuffer &path, UT_StringArray &paths) const;
	void	getMaterialOps(UT_WorkBuffer &path, UT_Array<SetOp> &ops) const;
	void	getExprOps(UT_WorkBuffer &path, UT_Array<SetOp> &ops) const;
	void	getVisibleOps(UT_WorkBuffer &path, UT_Array<VisibleOp> &ops) const;
	void	getMatchingExpr(UT_WorkBuffer &path,
			    UT_TagManager &mgr,
			    UT_Set<std::string> &selection,
			    const UT_TagList &tags);

	const Node		*parent() const	{ return myParent; }
	const std::string	&name() const	{ return myName; }
	const std::string	&material() const	{ return myMaterial; }
	const std::string	&expr() const		{ return myExpr; }
	int			 visible() const	{ return myVisible; }
	GEO_ViewportLOD		 viewportLOD() const	{ return myViewportLOD; }

	void	dump(int indent) const
	{
	    dumpNode(indent);
	    for (NodeSet::const_iterator it = myKids.begin();
		    it != myKids.end(); ++it)
		it->second->dump(indent+1);
	}

    private:
	void	createTagExpression(UT_TagManager &tags)
	{
	    if (!myExprPtr && UTisstring(myExpr.c_str()))
	    {
		UT_String	errs;
		myExprPtr = tags.createExpression(myExpr.c_str(), errs);
		if (errs.isstring())
		    myExprPtr.reset(NULL);
	    }
	}
	void	updateTagExpression()
	{
	    if (myExprPtr)
	    {
		UT_WorkBuffer	expr;
		myExprPtr->getExpressionString(expr);
		myExpr = expr.toStdString();
		myExprPtr.reset(NULL);
	    }
	}

	void	dumpNode(int indent) const;
	/// Check to see if all the kids have the same values.
	int	pruneBranches()
	{
	    Node	*kid = NULL;

	    myCommonKids = COMMON_ALL;
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
	    {
		kid = it->second;
		myCommonKids &= it->second->pruneBranches();
	    }
	    for (NodeSet::iterator it=myKids.begin(); it != myKids.end(); ++it)
	    {
		if (it->second->myMaterial != kid->myMaterial)
		    myCommonKids &= ~COMMON_MATERIAL;
		if (it->second->myVisible != kid->myVisible ||
			it->second->myViewportLOD != kid->myViewportLOD)
		{
		    myCommonKids &= ~COMMON_LOD;
		}
		else if (it->second->myExpr != kid->myExpr)
		{
		    myCommonKids &= ~COMMON_EXPR;
		}
		if (!myCommonKids)
		    break;
	    }
	    if (myCommonKids && kid)
	    {
		// Propagate up the tree
		if (myCommonKids & COMMON_LOD)
		{
		    myVisible = kid->myVisible;
		    myViewportLOD = kid->myViewportLOD;
		}
		if (myCommonKids & COMMON_EXPR)
		    myExpr = kid->myExpr;
		if (myCommonKids & COMMON_MATERIAL)
		    myMaterial = kid->myMaterial;
	    }
	    return myCommonKids;
	}
	std::string		 myName;
	std::string		 myMaterial;
	std::string		 myExpr;
	UT_TagExpressionPtr	 myExprPtr;
	Node			*myParent;
	NodeSet			 myKids;
	int			 myVisible;
	GEO_ViewportLOD		 myViewportLOD;
	int			 myCommonKids;
    };


     SOP_LightLink();
    ~SOP_LightLink();

    /// Add a node to the tree.  This will create all intermediate nodes.
    void	addPath(const char *path);

    /// Set the material for a given subtree.  The node specified by the path
    /// (and all its children) will be set to use the specified material.
    bool	setMaterial(const char *path, const std::string &value);
    /// Set the visibility for a given subtree
    bool	setVisible(const char *path, int visible);
    /// Set the viewport LOD for a given subtree
    bool	setViewportLOD(const char *path, GEO_ViewportLOD lod);

    /// Set the expression string for a given subtree.
    bool	setExpr(const char *path, const std::string &tag_expression);

    /// Add a light tag to the subtree specified by the given path.  This will
    /// convert a node's expression to a compiled form.  Please call
    /// @c clearTagExpressions() when you're finshed editing the tree.
    bool	addTag(UT_TagManager &m, const char *path, const UT_TagList &t);
    /// Remove a light tag from the entire subtree specified by the given path.
    /// This will convert a node's expression to a compiled form.  Please call
    /// @c clearTagExpressions() when you're finshed editing the tree.
    bool	rmTag(UT_TagManager &m, const char *path, const UT_TagList &t);

    /// Convert any tag expressions to their string values.  You must call this
    /// when you're finished calling addTag()/rmTag().
    void	clearTagExpressions()	{ myRoot->clearTagExpressions(); }
    /// Restore tree to default values
    void	clearValues()		{ myRoot->clearSubtree(); }

    /// Return a string array of all the unique paths
    bool	extractLeafPaths(UT_StringArray &paths) const;

    /// Find out which operations will set the appropriate materials
    void	getMaterialOps(UT_Array<SetOp> &ops);
    /// Find out which operations will set the proper light expressions
    void	getExprOps(UT_Array<SetOp> &ops);
    /// Find out which operations will set the proper visibility operations
    void	getVisibleOps(UT_Array<VisibleOp> &ops);

    /// Get the material for the given path
    bool	getMaterial(const char *path, std::string &expr) const;
    /// Get the expression for the given path
    bool	getExpr(const char *path, std::string &expr) const;

    /// Match all node expressions against the given tags.  The matching nodes
    /// will have their path's added to the @c selection.  Each path will be
    /// prefixed with the @c prepend text.
    bool	getMatchingExpr(UT_TagManager &tags,
			const std::string &prepend,
			UT_Set<std::string> &selection,
			const UT_TagList &t) const;

    void	dump(const char *msg);
    /// @private
    void	optimizeTree()
    {
	if (!myOptimized)
	{
	    myRoot->optimizeTree();
	    myOptimized = true;
	}
    }
private:
    Node	*findNode(const char *path, bool create) const;

    Node		*myRoot;
    bool		 myOptimized;
};

#endif
