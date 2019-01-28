/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Memory.h ( SYS Library, C++)
 *
 * COMMENTS:	Memory tracking utilities
 */

#ifndef __SYS_Memory__
#define __SYS_Memory__

#include "SYS_API.h"
#include "SYS_Align.h"
#include "SYS_AtomicInt.h"
#include "SYS_Types.h"

#include <cstddef>

/// Memory Tracking
///
/// This class provides a mechanism for tracking memory usage.  If all
/// memory allocation and freeing is registered with this class, then
/// the memory footprint and resource usage can be tracked.
///
/// There is a companion class at UT which allows for printing of the memory
/// statistics.
///
/// An example of how to use this might be:
///
/// enum {
///	MEM_DEBUG = 0,
///	MEM_TASK1,
///	MEM_TASK2,
///	MEM_MAX_TASKS
/// };
///
/// static SYS_MemoryUser theMemoryUsers[MEM_MAX_TASKS] = {
///	SYS_MemoryUser("Debug),
///	SYS_MemoryUser("Task One"),
///	SYS_MemoryUser("Task Two"),
/// };
///    
/// static SYS_MemoryTable	theMemoryTable(theMemoryUsers,
///			       sizeof(theMemoryUsers)/sizeof(SYS_MemoryTable));
///
/// int
/// main(int argc, char *argv[])
///    {
///	UT_WorkBuffer	 stats;
///	void		*mem;
///	mem = SYS_Memory::Malloc(theMemoryTable, MEM_TASK1, 128);
///	mem = SYS_Memory::Realloc(theMemoryTable, MEM_TASK1, mem, 128, 256);
///	SYS_Memory::Free(theMemoryTable, MEM_TASK1, mem, 256);
///	UT_Memory::printTable(stats, theMemoryTable);
///	cout << stats.buffer();
///	return 0;
/// }
///

class SYS_API SYS_MemoryUser {
public:
    SYS_MemoryUser(const char *label)
	: myUsed(0)
	, myPeak(0)
    {
	// WARNING:  The label is a reference to the string.  It's not hardened
	myLabel = label;
    }

    SYS_MemoryUser(const SYS_MemoryUser &rhs)
	: myUsed(rhs.myUsed.relaxedLoad())
	, myPeak(rhs.myPeak.relaxedLoad())
    {
	myLabel = rhs.myLabel;
    }

    const char		*getLabel() const	{ return myLabel; }
    size_t		 getUsed() const	{ return myUsed.relaxedLoad(); }
    size_t		 getPeak() const	{ return myPeak.relaxedLoad(); }

    void		 inc(size_t amount)
			 {
			     myPeak.maximum(myUsed.add(amount));
			 }
    void		 dec(size_t amount)
			 {
			     myUsed.add(-amount);
			 }

private:
    const char		*myLabel;
    SYS_AtomicCounter	 myUsed;
    SYS_AtomicCounter	 myPeak;
};

class SYS_API SYS_MemoryTable {
public:
    SYS_MemoryTable(SYS_MemoryUser *user_list, int number_of_users)
    {
	myTable = user_list;
	mySize = number_of_users;
    }

    SYS_MemoryUser	 &getUser(int index)	   { return myTable[index]; }
    const SYS_MemoryUser &getUser(int index) const { return myTable[index]; }

    int			 entries() const	{ return mySize; }
    size_t		 getUsed() const;
    size_t		 getPeak() const;

    void		 inc(int user, size_t amount)
			 {
			     myTable[user].inc(amount);
			 }
    void		 dec(int user, size_t amount)
			 {
			     myTable[user].dec(amount);
			 }

private:
    SYS_MemoryUser	*myTable;
    int			 mySize;
};

class SYS_API SYS_Memory {
public:
    /// Allocate memory and track the usage.
    /// This method will return a null pointer if the size requested is equal
    /// to 0.
    static void	*Malloc(SYS_MemoryTable &table,
			int which_entry,
			size_t amount)
		 {
		     if (amount)
		     {
			 table.inc(which_entry, amount);
			 return malloc(amount);
		     }
		     return 0;
		 }
    /// Allocate memory and track the usage.  The memory will filled with zero.
    /// This method will return a null pointer if the size requested is equal
    /// to 0.
    static void	*Calloc(SYS_MemoryTable &table,
			int which_entry,
			size_t amount)
		 {
		     if (amount)
		     {
			 table.inc(which_entry, amount);
			 return calloc(1, amount);
		     }
		     return 0;
		 }
    /// Re-allocate a previously allocated block.  This method handles
    /// the case where the initial block has not been allocated (i.e. is a
    /// null pointer).
    /// This method will return a null pointer if the new_amount size requested
    /// is equal to 0.
    static void	*Realloc(SYS_MemoryTable &table,
			int which_entry,
			void *original_memory,
			size_t old_amount,
			size_t new_amount)
		{
		    void	*mem;
		    if (new_amount)
		    {
			if (original_memory)
			{
			    // Do it as two separate operations since size_t
			    // may be an unsigned and taking the delta may
			    // cause an overflow.
			    table.dec(which_entry, old_amount);
			    table.inc(which_entry, new_amount);
			    mem = realloc(original_memory, new_amount);
			}
			else
			{
			    table.inc(which_entry, new_amount);
			    mem = malloc(new_amount);
			}
		    }
		    else
		    {
			mem = 0;
			if (original_memory)
			{
			    table.dec(which_entry, old_amount);
			    free(original_memory);
			}
		    }
		    return mem;
		}

    /// Free an allocated block (tracking the memory usage).
    /// If a null pointer is passed in, no tracking will be done.  Thus, it's
    /// safe to pass in a non-zero size without a valid pointer (and not
    /// destroy the tracking process).
    static void	 Free(SYS_MemoryTable &table,
			int which_entry,
			void *memory,
			size_t amount)
		 {
		     if (memory)
		     {
			 table.dec(which_entry, amount);
			 free(memory);
		     }
		 }

    /// Track a memory acquisition that wasn't performed by this class.
    static void	 acquire(SYS_MemoryTable &table,
			int which_entry,
			size_t amount)
		 {
		     table.inc(which_entry, amount);
		 }
    /// Track a memory free that wasn't performed by this class.
    static void	 release(SYS_MemoryTable &table,
			int which_entry,
			size_t amount)
		 {
		     table.dec(which_entry, amount);
		 }
};

/// When implementing in a different library, the following macro can be used
/// to implement a localized interface.  For example, we might have
///	SYS_MEMORY_SUBCLASS(, GU, theGUMemoryTable)
/// or
///	class GU_Memory {
///	public:
///	    SYS_MEMORY_SUBCLASS(static, GU, myMemoryTable)
///	private:
///	    static SYS_MemoryTable	myMemoryTable;
///	};
///
#define SYS_MEMORY_SUBCLASS(STATIC, prefix, table)		\
	    STATIC void	*prefix##Malloc(int i, size_t amount) \
			 { return SYS_Memory::Malloc(table, i, amount); } \
	    STATIC void	*prefix##Calloc(int i, size_t amount) \
			 { return SYS_Memory::Calloc(table, i, amount); } \
	    STATIC void	*prefix##Realloc(int i, void *m, size_t o, size_t n) \
			 { return SYS_Memory::Realloc(table, i, m, o, n); } \
	    STATIC void	 prefix##Free(int i, void *m, size_t amount) \
			 { SYS_Memory::Free(table, i, m, amount); } \
	    STATIC void	 prefix##acquire(int i, size_t amount) \
			 { SYS_Memory::acquire(table, i, amount); } \
	    STATIC void	 prefix##release(int i, size_t amount) \
			 { SYS_Memory::release(table, i, amount); } \
	    static inline SYS_MemoryTable &prefix##getTable() { return table; }

/// 
/// Convenience function to print memory in a consistent format (i.e. 12.3 KB).
/// To print "18446744073709551616.00 MB" requires 27 characters.  There are 5
/// extra characters for good measure in SYS_MEMPRINTSIZE
#define SYS_MEMPRINTSIZE	32
SYS_API void	SYSprintMemory(char buf[SYS_MEMPRINTSIZE], int64 memory,
				int field_width=-1);

#endif
