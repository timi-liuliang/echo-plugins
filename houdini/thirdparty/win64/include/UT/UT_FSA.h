/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FSA.h ( UT Library, C++)
 *
 * COMMENTS:	Finite State Automata String Table
 * 	This table takes a fixed set of strings and creates a FSA which is used
 * 	for scanning the table.  The integer value associated with each string
 * 	can be found quite quickly.  As well, a search for a string not in the
 * 	table is especially efficient.
 * 	
 * 	The string table used to build the FSA can be destroyed after the table
 * 	is constructed, unless, on the odd chance, you want to find out what
 * 	strings are stored in the table.
 */

#ifndef __UT_FSA__
#define __UT_FSA__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_VectorTypes.h"
#include "UT_ValArray.h"
#include "UT_IntArray.h"
#include "UT_NonCopyable.h"
#include "UT_StringArray.h"
#include "UT_StringView.h"

class UT_WorkBuffer;

class UT_API ut_FSA_Token
{
public:
    ut_FSA_Token(int id=0, const char *token=0)
    {
	 myId = id;
	 myToken = token;
    }
    ~ut_FSA_Token() { }

    void	 set(int id, const char *token)
		 {
		     myToken = token;
		     myId = id;
		 }
    int		 getId() const		{ return myId; }
    const char	*getToken() const	{ return myToken; }
private:
    const char	*myToken;
    int		 myId;
};

class UT_API UT_FSA : public UT_NonCopyable
{
public:
     UT_FSA();
    ~UT_FSA();

    bool	build(const UT_StringArray &strings, const UT_IntArray &indices,
			int notfound_index = -1);
    bool	build(int ntokens, const ut_FSA_Token tokens[],
			int notfound_index=-1);
    void	clear();

    /// Return the size of the symbol table
    exint	entries() const	{ return myEntries; }

    bool	empty() const	{ return (myEntries == 0); }

    /// @{
    /// Find a symbol in the map, returning the integer value associated with
    /// the key.
    int		findSymbol(const char *symbol) const;
    int		findSymbol(const UT_StringView &symbol) const;
    /// @}

    /// Return the integer associated with entries not in the map
    int		getNotFound() const	{ return myNotFound; }

    /// @{
    /// Return whether a symbol exists
    bool	contains(const char *symbol) const
		    { return findSymbol(symbol) != myNotFound; }
    bool	count(const char *symbol) const
		    { return findSymbol(symbol) ? 1 : 0; }
    /// @}

    int64	getMemoryUsage(bool inclusive) const;

    void	dumpTables() const;

    // This method is used for validation.  It is kind of expensive since it
    // iterates over the entire space of the FSA for extraction.
    void	extractStrings(UT_StringArray &list) const;

    /// @private
    /// Used internally
    void	 resolveTree(const UT_IntArray &nexts,
			    const UT_IntArray &states,
			    const UT_IntArray &accepts,
			    const UT_ValArray<const char *> &remainders);

private:
    int		 *myAccept;		// Acceptance states
    int		 *myNexts;		// Transition table
    int		 *myValidate;		// Verification states
    char	**myRemainder;		// Remaining, unique text
    int		  myNSize, myVSize, myASize, myRSize;	// Sizes of arrays
    int		  myEntries;
    int		  myNotFound;
};

#endif
