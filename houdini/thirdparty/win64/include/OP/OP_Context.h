#ifndef __OP_Context__
#define __OP_Context__

#include "OP_API.h"
#include "OP_DataTypes.h"		// For OP_DATA_TYPE
#include "OP_Version.h"			// For OP_VERSION
#include <DEP/DEP_ContextOptions.h>
#include <UT/UT_NonCopyable.h>
#include <SYS/SYS_Types.h>
#include <stdio.h>

#define OP_TIME_TOL (0.00001)

class OP_CookTask;
class OP_Context;

class OP_API OP_ContextData
{
public:
    OP_ContextData()		{}
    virtual ~OP_ContextData()	{}
};

class OP_API OP_ContextOptionsWriteScope : public UT_NonCopyable
{
public:
			 OP_ContextOptionsWriteScope(OP_Context &context,
				 bool push_options);
			~OP_ContextOptionsWriteScope();

    DEP_ContextOptions	&operator*() const
			 { return *myHandle; }
    DEP_ContextOptions	*operator->() const
			 { return myHandle.operator->(); }
    DEP_ContextOptions	*get() const
			 { return myHandle.get(); }

private:
    DEP_ContextOptionsWriteHandle	 myHandle;
    OP_Context				&myContext;
    bool				 myOptionsPushed;
};

class OP_API OP_Context
{
public:
    struct CurrentEvalTimeTag		{};
    static CurrentEvalTimeTag		CurrentEvalTime;
    struct CurrentEvalContextTag	{};
    static CurrentEvalContextTag	CurrentEvalContext;

			 OP_Context();
    explicit		 OP_Context(fpreal t);
			 OP_Context(fpreal t, int thread);
			 OP_Context(CurrentEvalTimeTag);
			 OP_Context(CurrentEvalContextTag);
			 OP_Context(const OP_Context &from);
			~OP_Context();

    fpreal		 getTime() const	{ return myTime; }
    void		 setTime(fpreal t)	{ myTime = t; }
#if SYS_SIZEOF_FPREAL > 4
    // Maintain double precision when setting time - we do this by deprecating
    // a method to set 32 bit time.
    SYS_DEPRECATED(17.0) void	setTime(fpreal32 t) { myTime = t; }
#endif

    int			 xres() const	{ return myXres; }
    int			 yres() const	{ return myYres; }
    void		 setXres(int xres)	{ myXres = xres; }
    void		 setYres(int yres)	{ myYres = yres; }
    void		 setRes(int x, int y)	{ myXres = x; myYres = y; }

    exint		 getFrame()   const;
    fpreal		 getFloatFrame() const;
    void		 setFrame(fpreal frame);

    // The "thread" member is the sequential thread returned by SYSgetSTID()
    // (UT_Thread.h) for the current thread.
    void		 setThread(int thr) { myThread = thr; }
    int			 getThread() const  { return myThread; }

    // A cook task will be available when the node is asked to be cooked in
    // parallel.
    void		 setCookTask(OP_CookTask *cook_task)
			    { myCookTask = cook_task; }
    OP_CookTask		*cookTask()
			    { return myCookTask; }
    const OP_CookTask	*cookTask() const
			    { return myCookTask; }

    OP_ContextData	*getData() const		{ return myData; }
    void		 setData(OP_ContextData *data)	{ myData = data; }

    OP_VERSION		 getVersion() const { return myVersion; }
    void		 setVersion(OP_VERSION version) { myVersion = version; }

    bool		 isCookOverride() const
			    { return myFlags.myCookOverride; }
    void		 setCookOverride(bool override)
			    { myFlags.myCookOverride = override; }

    void		 setCompareOnlyTime()
			 {
			     myFlags.myTimeDep = true;
			     myFlags.myCompareContextOptions = false;
			 }

    bool		 isTimeDep() const
			    { return myFlags.myTimeDep; }
    void		 setTimeDep(bool timedep)
			    { myFlags.myTimeDep = timedep; }

    void		 setCompareContextOptions(bool compare)
			    { myFlags.myCompareContextOptions = compare; }
    bool		 isCompareContextOptions() const
			    { return myFlags.myCompareContextOptions; }

    void		 clearContextOptionDeps()
			    { myContextOptionDeps.reset(); }
    void		 addContextOptionDeps(const DEP_ContextOptionDeps &deps)
			    { DEPcreateContextOptionDepsFromPtr(
				myContextOptionDeps).
				    insert(deps.begin(), deps.end()); }
    const DEP_ContextOptionsStack *getContextOptionsStack() const
			    { return myContextOptionsStack.get(); }
    DEP_ContextOptionsReadHandle getContextOptions() const
			    { return myContextOptions; }
    DEP_ContextOptionsWriteHandle editContextOptions();
    DEP_ContextOptionsWriteHandle pushContextOptions();
    bool		 popContextOptions();

    void		 copyPointer(const OP_Context &from);

    int			 operator==( const OP_Context &to ) const;
    OP_Context		&operator= ( const OP_Context &to );

    void		 print(FILE *) const;

private: // data
    // 0 based time in seconds
    fpreal					 myTime;

    // SYSgetSTID() when this context was created.
    int						 myThread;

    // These fields are used to define the kind of COOK or GET that is to take
    // place.
    //
    // When fetching data, this is the version number of the fetched data.
    OP_VERSION					 myVersion;
    // User-specific cook data
    OP_ContextData				*myData;

    // Match criteria. These fields define the parameters for getCookedData(),
    // or needToCook() or cook().
    //
    // Arbitrary extra options
    DEP_ContextOptionsHandle			 myContextOptions;
    // Stack of context options
    UT_UniquePtr<DEP_ContextOptionsStack>	 myContextOptionsStack;
    // Which context options matter
    DEP_ContextOptionDepsPtr			 myContextOptionDeps;

    // Pixel resolution
    int						 myXres;
    int						 myYres;

    // Current cook task
    OP_CookTask					*myCookTask;

    // Flags (default value is in square brackets)
    struct Flags
    {
		// [false] Cook only if it is quick
	uint64  myCookOverride:1,
		// [true] Am I time dependent?
		myTimeDep:1,
		// [true] Compare context options when testing for equality?
		myCompareContextOptions:1;
    } myFlags;
};

#endif
