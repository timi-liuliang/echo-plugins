#ifndef _OP_Cache_
#define _OP_Cache_

#include "OP_API.h"
#include "OP_Context.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Array.h>

class OP_Node;
class OP_CacheInfo;
class op_CacheEntry;

class OP_API OP_Cache : public UT_NonCopyable
{
public:
		 OP_Cache(OP_Node *client,
			const char *cop_name,
			const char *cache_name);
    virtual	~OP_Cache();

    void	 setSize(int size);
    int		 getSize() const
		 { return( myMaxSize ); }

    void	*getData(OP_Context &context);

    void	 putData(void *data, OP_Context &c);
    void	 updateContextDeps(const DEP_ContextOptionDeps &deps);

    bool	 ownsData(void *);	// Does the cache own the data
    void	 clear();		// Deletes all data in the cache.

    // These are public; however, they're meant for diagnostics
    void		 print(FILE *);
    const void		*getDataPtr(int idx);
    const OP_Context	*getContextPtr(int idx);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myCacheData.getMemoryUsage(false);
        mem += myOpName.getMemoryUsage(false);
        mem += myCacheName.getMemoryUsage(false);
        return mem;
    }

private:
    void		deleteData(op_CacheEntry *entry) const;
    bool		deleteReference(void *);

    OP_Node			*myClient;
    UT_StringHolder		 myOpName;
    UT_StringHolder		 myCacheName;
    UT_Array<op_CacheEntry *>	 myCacheData;
    int				 myMaxSize;
};

#endif
