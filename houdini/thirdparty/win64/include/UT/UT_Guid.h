/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Guid.h ( UT Library, C++)
 *
 * COMMENTS:	This class is a globally unique identifier. Simply creating
 * 		one of these objects assigns a new unique value. These values
 * 		are keyed to the host machine, process id, time, and are
 * 		incremented on each request.
 *
 * 		****************** IMPORTANT NOTE: *********************
 * 		This implementation isn't guaranteed to produce unique values
 * 		because we use the IP address rather than a MAC address. It
 * 		has a privacy hole because the IP address is hashed in a way
 * 		that allows the original address to be recovered (which
 * 		would be more of a concern if we actualy used the MAC
 * 		address). 
 *
 * 		Current usage of this class is in the SIM library only,
 * 		which doesn't require true global uniqueness or thread
 * 		safety. But future uses of this class should beware of
 * 		these limitations and fix these problems as required.
 */

#ifndef __UT_Guid__
#define __UT_Guid__

#include "UT_API.h"
#include <iosfwd>
#include "UT_Hash.h"

class UT_String;
class UT_StringHolder;
class UT_WorkBuffer;
class UT_IStream;
class UT_Lock;

class UT_API UT_Guid
{
public:
			 UT_Guid();
			 UT_Guid(const UT_Guid &src);
			~UT_Guid();

    enum		 { theStringSize = 43 };
    enum		 { theBinarySize = 16 };
    bool		 setString(const char *str);
    void		 getString(UT_String &str) const;
    void		 getString(UT_StringHolder &str) const;
    void		 getString(UT_WorkBuffer &str) const;

    bool		 operator==(const UT_Guid &cmp) const;
    bool		 operator!=(const UT_Guid &cmp) const;
    const UT_Guid	&operator=(const UT_Guid &src);

    void		 writeBinary(std::ostream &os) const;
    bool		 readBinary(UT_IStream &is);
    bool		 readAscii(UT_IStream &is);

    /// This hash only is unique within this session.
    size_t               hash() const { return SYSwang_inthash(myUniqueCount); }

    /// A unique number for this run of Houdini.
    int64		 sessionId() const;

    friend UT_API std::ostream	&operator<<(std::ostream &os, 
                              	            const UT_Guid &guid);

private:
    static void		 initializeValues();

    int			 myRandom;
    int			 myPid;
    int			 myTime;
    int			 myUniqueCount;

    static int		 theRandomBase;
    static int		 thePid;
    static int		 theStartTime;
    static bool		 theValuesInitialized;
    static UT_Lock	 theValuesInitializeLock;

    friend class	 UT_GuidHash;
};

inline size_t hash_value(const UT_Guid &guid)
{
    return guid.hash();
}

SYS_DEPRECATED_PUSH_DISABLE()

class SYS_DEPRECATED_REPLACE(16.0, "SYShash<UT_Guid>") UT_API UT_GuidHash : public UT_Hash
{
public:
			 UT_GuidHash(const UT_Guid &uniqueid)
			     : myGuid(uniqueid)
			 { }
    virtual		~UT_GuidHash()
			 { }

    virtual int		 compare(const UT_Hash &a) const
			 {
			     const UT_GuidHash &b = (const UT_GuidHash &)a;
			     int		d;

			     d = myGuid.myUniqueCount - b.myGuid.myUniqueCount;
			     if( d ) return ((d < 0) ? -1 : 1);
			     d = myGuid.myTime - b.myGuid.myTime;
			     if( d ) return ((d < 0) ? -1 : 1);
			     d = myGuid.myPid - b.myGuid.myPid;
			     if( d ) return ((d < 0) ? -1 : 1);
			     d = myGuid.myRandom - b.myGuid.myRandom;
			     if( d ) return ((d < 0) ? -1 : 1);
				 
			     return 0;
			 }
    virtual void	 copy(const UT_Hash &a)
			 {
			     myGuid = static_cast<const UT_GuidHash&>(a).myGuid;
			 }
    virtual unsigned	 hash() const
			 {
			     return SYSwang_inthash(myGuid.myUniqueCount);
			 }
    virtual UT_Hash	*copy() const
			 {
			     return new UT_GuidHash(myGuid);
			 }
    virtual int64	 getMemoryUsage(bool inclusive) const
			 {
			     return inclusive ? sizeof(*this) : 0;
			 }

private:
    UT_Guid		 myGuid;
};

SYS_DEPRECATED_POP_DISABLE()

#endif

