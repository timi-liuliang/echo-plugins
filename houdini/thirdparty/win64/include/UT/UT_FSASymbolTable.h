/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FSASymbolTable.h ( UT Library, C++)
 *
 * COMMENTS:	Insertion and deletion of symbols is incredibly expensive.
 *		Searching in the table is faster than other symbol tables (on
 *		most architectures).
 *
 */

#ifndef __UT_FSASymbolTable__
#define __UT_FSASymbolTable__

#include "UT_API.h"
#include "UT_FSA.h"
#include "UT_Array.h"
#include "UT_DeepString.h"

template <typename ITEM_T>
class UT_FSASymbolTable : public UT_NonCopyable {
    struct SymbolStorage
    {
	SymbolStorage(const char *str, const ITEM_T &data)
	    : myThing(data)
	    , myString(str) {}

	ITEM_T		myThing;
	UT_DeepString	myString;
    };

public:
     UT_FSASymbolTable() : myDirty(true) {}
    ~UT_FSASymbolTable() { clear(); }

    // Add a symbol to the table. Returns the string that would be returned
    // by getStringReference() (this will be valid even after rebuid)
    const char	*addSymbol(const char *symbol, const ITEM_T &data)
		{
		    myStorage.append(SymbolStorage(symbol, data));
		    myDirty = true;
		    return myStorage.last().myString;
		}
    int		deleteSymbol(const char *symbol)
		{
		    for (int i = 0; i < myStorage.entries(); i++)
		    {
			if (myStorage(i).myString == symbol)
			{
			    myStorage.removeIndex(i);
			    myDirty = true;
			    return 1;
			}
		    }
		    return 0;
		}
    void	clear()
		{
		    myStorage.setCapacity(0);
		    myTable.clear();
		    myDirty = true;
		}

    int		findSymbol(const char *symbol, ITEM_T *datap) const
		{
		    int		idx;
		    if (myDirty) rebuild();
		    idx = myTable.findSymbol(symbol);
		    if (idx >= 0)
		    {
			*datap = myStorage(idx).myThing;
			return 1;
		    }
		    return 0;
		}
    int		findSymbol(const UT_StringView &symbol, ITEM_T *datap) const
		{
		    int		idx;
		    if (myDirty) rebuild();
		    idx = myTable.findSymbol(symbol);
		    if (idx >= 0)
		    {
			*datap = myStorage(idx).myThing;
			return 1;
		    }
		    return 0;
		}

    unsigned	entries() const	{ return myStorage.entries(); }
    bool	empty() const	{ return !myStorage.entries(); }

    bool	contains(const char *symbol) const
		{ return myTable.contains(symbol); }
    bool	count(const char *symbol) const
		{ return myTable.count(symbol); }
    void	extractStrings(UT_StringArray &list) const
		{ myTable.extractStrings(list); }

    const char	*getStringReference(const char *symbol)
		 {
		     int	idx;
		     if (myDirty) rebuild();
		     idx = myTable.findSymbol(symbol);
		     return idx >= 0 ? myStorage(idx).myString : 0;
		 }

    int64	 getMemoryUsage(bool inclusive) const
		 {
		     if (myDirty) rebuild();
                     int64 mem = inclusive ? sizeof(*this) : 0;
		     mem += myTable.getMemoryUsage(false);
		     mem += myStorage.getMemoryUsage(false);
                     return mem;
		 }

    int		 traverseConst(
			int (*function)(const ITEM_T &, const char *, void *),
			void *data) const
		 {
		     for (int i = 0; i < myStorage.entries(); i++)
		     {
			 if (!function(myStorage(i).myThing,
				      myStorage(i).myString, data))
			     return 0;
		     }
		     return 1;
		 }
    int		 traverse(int (*function)(ITEM_T &, const char *, void *),
			void *data)
		 {
		     for (int i = 0; i < myStorage.entries(); i++)
		     {
			 if (!function(myStorage(i).myThing,
				      myStorage(i).myString, data))
			     return 0;
		     }
		     return 1;
		 }

    void	 rebuild()
		 {
		     ut_FSA_Token	*tokens;
		     int			 i;

		     if (myDirty)
		     {
			 tokens = (ut_FSA_Token *)::malloc(
				 sizeof(ut_FSA_Token)*myStorage.entries());
			 for (i = 0; i < myStorage.entries(); i++)
			     tokens[i].set(i, myStorage(i).myString);

			 myTable.build(myStorage.entries(), tokens, -1);
			 ::free(tokens);
			 myDirty = false;
		     }
		 }

private:
    void	 rebuild() const { (SYSconst_cast(this))->rebuild(); }

    UT_FSA			myTable;
    UT_Array<SymbolStorage>	myStorage;
    bool			myDirty;
};

#endif
