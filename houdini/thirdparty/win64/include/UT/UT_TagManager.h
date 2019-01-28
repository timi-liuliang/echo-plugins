/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TagManager.h ( UT Library, C++)
 *
 * COMMENTS:	Manage lists of tags.  Tags are simply name strings.  A list of
 *	tags is a list of names.  The names may contain alphanumeric characters
 *	along with the characters:
 *		'_' (underscore)
 *		'.' (period)
 *		'-' (minus) (we also support ^)
 *		':' (colon)
 *	Names may NOT begin with '-' (minus).  Invalid characters will be
 *	forced to an '_' (underscore) character.
 *
 *	Pattern expression can also be created which can be matched against tag
 *	lists (see below for more documentation).
 *
 *	A global list of used tags are kept and may be retrieved.  The list is
 *	not cleared (TODO: it would be nice to have a garbage collection
 *	process)
 */

#ifndef __UT_TagManager__
#define __UT_TagManager__

#include "UT_API.h"
#include "UT_SymbolTable.h"
#include "UT_SmallObject.h"
#include "UT_IntrusivePtr.h"
#include "UT_IntArray.h"
#include "UT_BitArray.h"
#include "UT_Memory.h"
#include "UT_ConcurrentVector.h"
#include "UT_ConcurrentHashMap.h"
#include "UT_HashFunctor.h"

class UT_String;
class UT_StringArray;
class UT_TagManager;
class UT_TagList;
class UT_TagExpression;

// #define UT_TAG_USE_SMALL_OBJECTS

/// A comma separated list of tags (i.e. "fill,spot,red")
class UT_API UT_TagList
    : public UT_IntrusiveRefCounter<UT_TagList>
#if defined(UT_TAG_USE_SMALL_OBJECTS)
    , public UT_SmallObject<UT_TagList,
				UT_SMALLOBJECT_CLEANPAGES_ON,
				512,
				UT_SMALLOBJECT_THREADSAFE_ON>
#endif
{
public:
     UT_TagList(UT_TagManager &manager, const UT_IntArray &bits, int max);
     UT_TagList(UT_TagManager &manager, int single_name_index);
    ~UT_TagList();

    void	getListString(UT_WorkBuffer &buffer) const;
    bool	match(const UT_TagExpression &pattern) const;
    uint	hash() const;
    bool	compare(const UT_TagList &list) const;

    bool	operator==(const UT_TagList &list) const
		    { return compare(list); }
    bool	operator!=(const UT_TagList &list) const
		    { return !compare(list); }

    /// @{
    /// Get the tag manager
    const UT_TagManager	&getManager() const	{ return myManager; }
    UT_TagManager	&getManager()		{ return myManager; }
    /// @}

    int64	getMemUsage() const
		    { return sizeof(*this) + myBits.getMemoryUsage(); }

private:
    bool	isEmpty() const
		    { return mySingle < 0 && myBits.size() == 0; }
    bool	hasTag(int id) const
		{
		    return id == mySingle ||
			    (id < myBits.size() && myBits.getBit(id));
		}
    void	validateList(int namecount);
    void	setSingleBit(int bit);

private:
    UT_TagManager	&myManager;
    UT_BitArray		 myBits;
    int			 mySingle;
    friend class	 UT_TagManager;
    friend class	 UT_TagExpression;

};

/// An expression to match tags (i.e. "* & ^fill")
class UT_API UT_TagExpression
    : public UT_IntrusiveRefCounter<UT_TagExpression>
#if defined(UT_TAG_USE_SMALL_OBJECTS)
    , public UT_SmallObject<UT_TagExpression,
				UT_SMALLOBJECT_CLEANPAGES_ON,
				512,
				UT_SMALLOBJECT_THREADSAFE_ON>
#endif
{
public:
     UT_TagExpression(UT_TagManager &manager);
    ~UT_TagExpression();

    /// @{
    /// Allocate a new expression with the appropriate edits

    /// (expr) -> A|(expr).  However, we can also simplify some components of
    /// expression. For example, (A|-A) -> * (Complementary Law)
    UT_TagExpression	*addTags(const UT_TagList &list) const;

    /// (expr) -> -A & (expr).  Since | has a greater precedence, we need to
    /// expand using distributive property.  Some components of the expression
    /// can be simplified (i.e. -A & A -> -*).
    UT_TagExpression	*rmTags(const UT_TagList &list) const;

    /// @}

    /// @{
    /// Get the tag manager
    const UT_TagManager	&getManager() const	{ return myManager; }
    UT_TagManager	&getManager()		{ return myManager; }
    /// @}

    bool	compare(const UT_TagExpression &expr) const;
    bool	operator==(const UT_TagExpression &expr) const
		    { return compare(expr); }
    bool	operator!=(const UT_TagExpression &expr) const
		    { return !compare(expr); }
    uint	hash() const;

    void	getExpressionString(UT_WorkBuffer &buffer) const;
    bool	match(const UT_TagList &list) const;
    bool	isTautology() const;

    /// Determine whether any known names match.  The matching names will
    /// be returned in 'matching' and names that don't match are put in
    /// 'failing'.  'outmatch' will be set to true when unknown names will
    /// produce a match.
    void	matchAllNames(UT_Array<const char *> &matching,
			      UT_Array<const char *> &failing,
			      bool &outmatch);

    using SubExpressionList = UT_Array<UT_IntArray>;

    int64	getMemUsage() const
		    { return sizeof(*this) + myCode.getMemoryUsage(); }
private:
    // Break up a complex expression into multiple sub-expressions which can be
    // or'ed together.
    void	extractOrComponents(SubExpressionList &elist) const;

    // Combine multiple sub-expressions into a single expression by or'ing them
    // together.
    void	combineOrComponents(const SubExpressionList &elist);

    static bool	simplifyExpressions(SubExpressionList &elist,
				int bit,
				UT_TagExpression *expr,
				UT_BitArray &need_final_or);
    static void	addExpressionOr(UT_TagExpression *expr,
				int bit,
				const UT_BitArray &need_final_or);

    void	clear();
    void	validateExpr(int namecount);
    void	addOpCode(int opcode, int operand = 0);
    bool	addOpCode(UT_IntArray &stack)
		{
		    int		n;
		    n = stack.entries();
		    if (n < 2)
			return false;
		    addOpCode(stack(n-2), stack(n-1));
		    stack.entries(n-2);
		    return true;
		}


private:
    UT_TagManager	&myManager;
    UT_IntArray		 myCode;
    friend class	 UT_TagManager;
};

using UT_TagListPtr = UT_IntrusivePtr<UT_TagList>;
using UT_TagExpressionPtr = UT_IntrusivePtr<UT_TagExpression>;

/// A tag manager keeps track of lists of tags and expressions.  It stores them
/// very compactly for efficient processing.
class UT_API UT_TagManager
{
public:
    class WeakKey
    {
    public:
	// Copy in as a weak pointer when passed a const char *
	WeakKey(const char *key)
	    : myKey(UTmakeUnsafeRef(key))
	{
	}
	// Given a UT_StringHolder, we can just store the holder
	WeakKey(const UT_StringHolder &key)
	    : myKey(key)
	{
	}
	// The copy c-tor is used when inserting into the map, so we need to
	// ensure the key is hardened.
	WeakKey(const WeakKey &k)
	    : myKey(k.myKey.c_str())	// UT_StringHolder will duplicate
	{
	}
	WeakKey	&operator=(const WeakKey &key)
	{
	    // We need to harden the string in the source key
	    myKey = UT_StringHolder(key.c_str());
	    return *this;
	}
	const char	*c_str() const	{ return myKey.c_str(); }
	uint		 hash() const	{ return myKey.hash(); }
	bool		 operator==(const WeakKey &k) const
			 {
			     return myKey == k.myKey;
			 }
    private:
	UT_StringHolder	myKey;
    };
    using tag_ListType = UT_ConcurrentVector<const char *>;
    using tag_MapType = UT_ConcurrentHashMap<WeakKey, int,
				    UT_HashFunctor<WeakKey> >;
     UT_TagManager();
    ~UT_TagManager();

    /// getTagList() creates a tag-list object.  This is a list of names which
    /// are considered as tags for matching (see below).
    /// The names allow alphanumeric letters and characters from "_.:"
    UT_TagListPtr	createList(const char *list, UT_String &errs)
			{
			    return UT_TagListPtr(createTagList(list, errs));
			}
    UT_TagListPtr	createListForName(int name_index)
			{
			    return UT_TagListPtr(createTagListForName(name_index));
			}

    /// createTagExpression() is used to create a pattern to match against tag
    /// lists.
    ///
    /// The tag expression can be a very simple expression consisting of
    /// - @b name   @n Matches tag lists which have the name
    /// - @b -name  @n Matches tag lists which do NOT contain the name
    ///	- @b  +     @n Matches tag lists which have ANY entries
    /// - @b  -     @n Matches tag lists which have NO entries
    /// - @b  *     @n Matches all tag lists (Equivalent to "+|-")
    /// - @b -*     @n Matches no tag lists (Equivalend to "+&-")
    ///
    /// Expressions may be joined with & or | (AND or OR).  For example
    /// - <tt>-foo</tt>		@n Match all tags except those with "foo"
    /// - <tt>* & -foo</tt>	@n Equivalent to "-foo"
    /// - <tt>-foo & -bar</tt>	@n
    ///		Match all tags except those with "foo" or "bar"
    /// - <tt>foo & -bar</tt>	@n
    ///		Match all tags that have "foo" but don't have "bar"
    ///
    /// Expressions are processed left to right with AND at a higher
    /// precedence in the order of operations than OR.  Thus:
    /// @code
    ///	   a & b & c | d & e | f | g & h
    /// @endcode
    /// is equivalent to:
    /// @code
    ///	   (a & b & c) | (d & e) | f | (g & h)
    /// @endcode
    ///
    /// @note You are responsible for deleting this
    /// Smart pointer access to tag expressions
    UT_TagExpressionPtr		createExpression(const char *expr,
					UT_String &errs)
				{
				    return UT_TagExpressionPtr(
					    createTagExpression(expr, errs));
				}

    enum UT_TagEditOperation
    {
	TAG_OP_ADD,		// Add name to expression
	TAG_OP_RM,		// Remove name from expression
	TAG_OP_SET,		// Set the expression to the name
    };

    /// Apply an edit to an expression.  This operation may have bugs, but will
    /// always work on a simplified expression.
    UT_TagExpressionPtr		editExpression(const UT_TagExpressionPtr &expr,
				    const char *name,
				    UT_TagEditOperation op,
				    UT_String &errors);

    /// Get a list of all the tags used by any list or pattern we've
    /// constructed
    void		 getAllNames(UT_StringArray &tags);

    /// @private accessors
    const tag_ListType	&nameList() const { return myNameList; }

private:
    UT_TagList		*createTagListForName(int name_index);
    UT_TagList		*createTagList(const char *list,
					UT_String &errs);
    UT_TagExpression	*createTagExpression(const char *expression,
					UT_String &errs);

    // Returns the index of the the tag
    int		 findOrCreateTag(const char *tag);

    tag_MapType		myNameTable;
    tag_ListType	myNameList;

    friend class	UT_TagList;
    friend class	UT_TagExpression;
};

#endif
