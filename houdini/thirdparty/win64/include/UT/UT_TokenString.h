/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:	String class for quick exact comparison & hashing by using
 *		tokens (non-ASCII bytes). 
 *
 */
#ifndef __UT_TOKENSTRING_H__
#define __UT_TOKENSTRING_H__

#include "UT_API.h"
class UT_String;
class UT_WorkBuffer;
#include "UT_Endian.h"
#include "UT_Map.h"
#include "UT_Set.h"
#include "UT_UniquePtr.h"
#include <SYS/SYS_Inline.h>


// Turning on the following define will cause checks to be put in place to
// detect buffer over/underruns.
// 
//#define UT_TS_DETECT_BUFFER_ERRORS

class UT_API UT_TokenString
{
public:
    static UT_TokenString *allocString();
    static UT_TokenString *dupString(const UT_TokenString &);
    static void		   freeString(UT_TokenString *&);

    bool		operator==(const UT_TokenString &str) const;
    bool		operator!=(const UT_TokenString &str) const
    { return !(*this == str); }
    int			strcmp(const UT_TokenString &str) const;
    UT_TokenString	&operator=(const UT_TokenString &str);
    UT_TokenString	&operator+=(const UT_TokenString &str);

    void		append(int16 value, bool hashonly = false);
    void		append(uint16 value, bool hashonly = false);
    void		append(int32 value, bool hashonly = false);
    void		append(uint32 value, bool hashonly = false);
    void		append(int64 value, bool hashonly=false);
    void		append(uint64 value, bool hashonly=false);
    void		append(fpreal32 value, bool hashonly = false);
    void		append(fpreal64 value, bool hashonly = false);
    void		append(char value, bool hashonly = false);
    void		append(const char *value, bool hashonly = false);
    void		append(const UT_String &value, bool hashonly = false);
    void		append(const UT_WorkBuffer &value, bool hashonly=false);
    void		appendPointer(const void *, bool hashonly=false);

    void		clear();
    unsigned int	getHashCode() const { return myHashCode; }

    // NOTE: getMemoryUsage(true) must return a value large enough to be
    //       the length of a buffer passed to copyToMem.
    int64		getMemoryUsage(bool inclusive) const;

    // for writing to disk with direct IO
    int			copyToMem(void *mem, int *length) const;
    void		copyFromMem(void *mem, int *length);
    
    void		print() const; // for debug

    void		getPrintable(UT_String &string) const;

    void		compress();

    struct PtrDeleter
    {
        SYS_FORCE_INLINE
        void operator()(UT_TokenString *ts) { UT_TokenString::freeString(ts); }
    };

    typedef UT_UniquePtr<UT_TokenString, PtrDeleter> Ptr;

    struct PtrHashFunctor
    {
        SYS_FORCE_INLINE
        size_t operator()(const Ptr &p) const
        {
            if (!p)
                return 0;
            return p->getHashCode();
        }
    };
    struct PtrEqualFunctor
    {
        SYS_FORCE_INLINE
        bool operator()(const Ptr &a, const Ptr &b) const
        {
            if (!a)
                return !b;
            if (!b)
                return false;
            return *a == *b;
        }
    };
    typedef UT_Set<Ptr,PtrHashFunctor,PtrEqualFunctor> Set;

    template<typename V>
    class Map : public UT_Map<Ptr,V,PtrHashFunctor,PtrEqualFunctor>
    {};

private:
    UT_TokenString();
    ~UT_TokenString();

    // This will realloc myString and ensure it is at least the given
    // size.  It does nothing if there is already room, so call this
    // wherever you want to grow or are not sure.
    void	 resizeString(int size);
    
    void	 append(const void *str, int length, bool hashonly);

    unsigned int myHashCode;
    int		 myLength;
    int		 myTrueSize;

    union {
	unsigned char *myString;
	UT_TokenString *myNext;
    };

#ifdef UT_TS_DETECT_BUFFER_ERRORS
    unsigned char   *myMemString;
#endif

    friend class OP_Node;
};
#endif
