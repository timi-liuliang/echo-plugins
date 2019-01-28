/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_HashTable.h
 *
 * COMMENTS:
 */

#ifndef __UT_HashTable_h__
#define __UT_HashTable_h__

#include "UT_API.h"
#include <stdio.h>
#include <iosfwd>
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Types.h>
#include "UT_ThingList.h"
#include "UT_Hash.h"

SYS_DEPRECATED_PUSH_DISABLE()

class SYS_DEPRECATED_REPLACE(16.0, "UT_Map") UT_API UT_HashTable 
{
public:
    explicit UT_HashTable();
    virtual ~UT_HashTable();

    // Set this to true before adding any items to the hash table if your
    // hash function implements a proper strcmp() (returning -1 for <, 0 for =
    // and 1 for >). This can shorten adds and queries to buckets with more
    // than 1 entry.
    void	 useOrderedLists(bool ordered);

    void    	 addSymbol(const UT_Hash& symbol, const UT_Thing &data);
    int	   	 findSymbol(const UT_Hash& symbol, UT_Thing *datap,
			    const UT_Hash **hash_symbol = 0) const;

    int          contains(const UT_Hash &symbol) const
		    { UT_Thing value; return findSymbol(symbol, &value); }

    int		 count(const UT_Hash &symbol) const
		    { return contains(symbol) ? 1 : 0; }

    template <typename T>
    T		*findPointer(const UT_Hash &symbol) const
    {
	UT_Thing thing;
	if (!findSymbol(symbol, &thing))
	    return NULL;
	return thing.template asPointer<T>();
    }

    int	 	 deleteSymbol(const UT_Hash& symbol);
    int		 moveSymbol(const UT_Hash &oldsymbol,
			    const UT_Hash &newsymbol);
    void	 clear();

    unsigned	 entries() const	{ return myNbrEntries; }
    unsigned	 size() const		{ return myNbrEntries; }
    bool	 empty() const		{ return !myNbrEntries; }

    // Retrieve an entry in the table, creating it if it doesn't already exist.
    UT_Thing	&operator[](const UT_Hash& symbol);

    // Call this method when you're adding entries to a table inside a loop
    // and you know how many entries you'll be adding.
    void	 reserveTableSize(int size);

    // The table will automatically rebuild itself to keep the load factor
    // within the specified range.  A minimum load factor of 0 means that
    // the table will never shrink when elements are deleted from it.
    float	 getMaxLoadFactor() const { return myMaxLoadFactor; }
    void	 setMaxLoadFactor(float max_load_factor);
    float	 getMinLoadFactor() const { return myMinLoadFactor; }
    void	 setMinLoadFactor(float min_load_factor);

    // Merge the specified table with ourselves.
    void	 mergeTable(const UT_HashTable &table);

    // "getHashReference" returns a pointer to the actual UT_Hash that
    // is maintained in the hash table. This can be used to implement
    // a shared hash symbol table.
    const UT_Hash	*getHashReference(const UT_Hash &hash);

    void	 outputStats(FILE *output) const;
    void	 outputStats(std::ostream &os) const;
    void	 dumpTable(FILE *output) const;

    int64	 getMemoryUsage(bool inclusive) const;

    // "traverse" invokes the given function with every member of the
    // hash table. The order of traversal is undefined. If the function
    // returns a non-zero value the traversal continues. If it returns
    // zero then traversal stops. The return value is zero if the traversal
    // was interupted and one otherwise.
    //
    // Call traverseConst() if you will not be removing things from the table
    // while traversing, and traverse() if you might be.  If you call traverse()
    // and add something to the table, you may or may not encounter it later
    // in the traversal.
    int		 traverseConst(
			  int (*function)(UT_Thing &, const UT_Hash&, void *),
			  void *data) const;
    int		 traverse(int (*function)(UT_Thing &, const UT_Hash&, void *),
			  void *data);

    class UT_API traverser
    {
	public:
	    traverser()
		: myCurrList(0), myEndList(0)
			 { }
	    traverser(const traverser &src)
			 { *this = src; }
	    ~traverser()
			 { }

	    UT_Thing	&thing() const
			 { return myCurrEntry.thing(); }
	    UT_Hash	*hash() const
			 { return myCurrEntry.hash(); }

	    int		 operator==(const traverser &cmp) const
			 {
			     return ((!myCurrList && !cmp.myCurrList) ||
				     ((myCurrList == cmp.myCurrList) &&
				      (myCurrEntry == cmp.myCurrEntry)));
			 }
	    int		 operator!=(const traverser &cmp) const
			 { return !(*this == cmp); }

	    bool	 atEnd() const
			 {
			     return !myCurrList;
			 }

	    traverser	&operator++()
			 {
			     advance();
			     return *this;
			 }
	    traverser	&operator++(int)
			 {
			     advance();
			     return *this;
			 }

	    traverser	&operator=(const traverser &src)
			{
			     myCurrList = src.myCurrList;
			     myEndList = src.myEndList;
			     myCurrEntry = src.myCurrEntry;
			     return *this;
			}

	    void	advance()
			{
			     if( ++myCurrEntry == myCurrList->end() )
			     {
				 ++myCurrList;
				 advanceToNextList();
			     }
			}

	private:
	    traverser(const UT_ThingList *list, const UT_ThingList *endlist)
		: myCurrList(list), myEndList(endlist)
			 { advanceToNextList(); }

	    void	 advanceToNextList()
			 {
			     while( myCurrList != myEndList &&
				    myCurrList->begin() == myCurrList->end() )
				 ++myCurrList;
			     if( myCurrList == myEndList )
				 myCurrList = 0;
			     else
				 myCurrEntry = myCurrList->begin();
			 }

	    const UT_ThingList		*myCurrList;
	    const UT_ThingList		*myEndList;
	    UT_ThingList::traverser	 myCurrEntry;

	    friend class UT_HashTable;
    };

    traverser	 begin() const
    {
	if( myTable )
	    return traverser(myTable, myTable + mySize);
	return traverser();	// same as end()
    }
    traverser	 end() const
    { return traverser(); }

private:
    enum UT_AdjustType { UT_ADJUST_INCREASE, UT_ADJUST_DECREASE };
    void	 adjustTableSize(UT_AdjustType adjust_type);
    void	 allocateTableOfNewSize(int size);

    // Data:
    UT_ThingList *myTable;
    int		 mySize;
    unsigned int myNbrEntries;
    fpreal	 myMaxLoadFactor;
    fpreal	 myMinLoadFactor;
    bool	 myUseOrdered;
    bool	 myIsTraversing;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
