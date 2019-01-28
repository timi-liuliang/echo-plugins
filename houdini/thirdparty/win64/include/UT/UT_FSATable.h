/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FSATable.h ( UT Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_FSATable__
#define __UT_FSATable__

#include "UT_API.h"
#include "UT_FSASymbolTable.h"
#include <SYS/SYS_StaticAssert.h>
#include <stdarg.h>

template <typename T, int DEFAULT_TOKEN = -1>
class UT_FSATableT
{
public:
    /// Empty table constructor.
    UT_FSATableT()
    {
    }

    /// Create a new token map. Variadic arguments are provided as pairs -
    /// for example:
    ///		UT_FSATableT map(1, "one", 2, "two", -1, NULL);
    /// The argument list must be terminated by (-1, NULL).
    UT_FSATableT(T token, const char *symbol, ...)
    {
	va_list	args;
	va_start(args, symbol);

	while (1)
	{
	    if (!symbol)
		break;

	    symbol = myFSA.addSymbol(symbol, token); 

	    int idx = (int)token;
	    if (idx >= 0)
	    {
		if (idx < myTokens.entries())
		{
		    if (!myTokens(idx))
			myTokens(idx) = symbol;
		}
		else
		    myTokens.insert(symbol, idx);
	    }

	    SYS_STATIC_ASSERT(sizeof(T) == sizeof(int));
	    token = (T)va_arg(args, int);
	    symbol = va_arg(args, const char *);
	}

	va_end(args);

	myFSA.rebuild();
    }

    /// @{
    /// Find the integer id corresponding to the symbol.
    /// Returns DEFAULT_TOKEN if not found
    T		findSymbol(const char *symbol) const
		{
		    T	token;
		    if (symbol && myFSA.findSymbol(symbol, &token))
			return token;
		    return (T)DEFAULT_TOKEN;
		}
    T		findSymbol(const UT_StringView &symbol) const
		{
		    T	token;
		    if (symbol && myFSA.findSymbol(symbol, &token))
			return token;
		    return (T)DEFAULT_TOKEN;
		}
    /// @}

    /// Return the name associated with the integer id.  If there are duplicate
    /// strings which map to the same id, the first string will be returned.
    /// Returns NULL if not valid
    const char	*getToken(T token) const
		 {
		     int idx = (int)token;
		     if (idx >= 0 && idx < myTokens.entries() && myTokens(idx))
			 return myTokens(idx);
		     return NULL;
		 }

    /// Returns the memory usage of this object
    int64	 getMemUsage() const
		 { return myFSA.getMemoryUsage(true) +
			  myTokens.getMemoryUsage(true); }

    /// Extract all strings
    void	extractStrings(UT_StringArray &list) const
		{ myFSA.extractStrings(list); }

    /// @{
    /// Return whether a symbol exists
    bool	contains(const char *symbol) const
		{ return myFSA.contains(symbol); }
    bool	count(const char *symbol) const
		{ return myFSA.count(symbol); }
    /// @}

    
private:
    UT_FSASymbolTable<T>	myFSA;
    UT_Array<const char *>	myTokens;
};

typedef UT_FSATableT<int, -1> UT_FSATable;

#endif

