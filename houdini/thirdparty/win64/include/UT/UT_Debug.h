#ifndef __UT_Debug__
#define __UT_Debug__

#include "UT_API.h"
/*
//  This is a convenience class for adding debugging code to your
//  source. It allows the use of run-time switches via environment
//  variables and can be compiled out by not defining the pre-processor
//  symbol UT_DEBUG.
//
//  Typical use of this class involves declaring a static variable of
//  type UT_Debug with the name of an environment variable switch that
//  will enable this object. Then use it as follows:
//  
//  static UT_Debug	debug("UT_DEBUG_THIS");
//  ...
//  if( debug.on() )
//	debug.output("This is %x\n", this);
*/

//  It's UGLY but... 

#ifdef UT_DEBUG

#include <stdlib.h>
#include <string.h>

#define UT_IF(s)	s
#define UT_IFNOT(s)
#define UT_IFELSE(t, f)	t

class UT_API UT_Debug
{
public:
    explicit UT_Debug(const char *name)	{ myState = getenv(name) != 0; }
    int		on() const	{ return myState; }
    void	setDebugFile(const char *) const;
    void	output(const char *fmt, ...) const;
private:
    int		myState;
};

#else

#define UT_IF(s)
#define UT_IFNOT(s)	s
#define UT_IFELSE(t, f)	f

class UT_API UT_Debug
{
public:
    explicit UT_Debug(const char *)			 {}
    int		on() const			 { return 0; }
    void	setDebugFile(const char *) const;
    void	output(const char *, ...) const;
};

#endif

// Used by UT_DBGOUT(())
UT_API void UTdbgout(const char *file, int lineno, const char *text);

UT_API const char *UTconsoleColorString(const char *colorname);

// this macro must be used with a double parentheses!
// eg. UT_DBGOUT(( "value = %d", myValue ));
// The UTdebugFormat does not require double parentheses. 
// Uses the same formatting codes as UTformat.
#if UT_ASSERT_LEVEL > 0
    #include "UT_WorkBuffer.h"
    #include <sstream>
    #include <stdio.h>

    #define UTdebugFormat(...)   do { \
				    UT_WorkBuffer dbostr; \
				    dbostr.format(__VA_ARGS__); \
				    UTdbgout(__FILE__, __LINE__, dbostr.buffer()); \
				} while (false)

    #define UTdebugFormatCd(color, ...)   do { \
				    UT_WorkBuffer dbostr, cdstr; \
				    dbostr.format(__VA_ARGS__); \
				    if (strcmp(#color, "none") == 0) \
					cdstr = dbostr; \
				    else \
					cdstr.sprintf("%s%s%s", UTconsoleColorString(#color), dbostr.buffer(), UTconsoleColorString("none")); \
				    UTdbgout(__FILE__, __LINE__, cdstr.buffer()); \
				} while (false)

    #define UTdebugPrint(...)   do { \
				    UT_WorkBuffer dbostr; \
				    dbostr.print(__VA_ARGS__); \
				    UTdbgout(__FILE__, __LINE__, dbostr.buffer()); \
				} while (false)

    #define UTdebugPrintCd(color, ...)   do { \
				    UT_WorkBuffer dbostr, cdstr; \
				    dbostr.print(__VA_ARGS__); \
				    if (strcmp(#color, "none") == 0) \
					cdstr = dbostr; \
				    else \
					cdstr.sprintf("%s%s%s", UTconsoleColorString(#color), dbostr.buffer(), UTconsoleColorString("none")); \
				    UTdbgout(__FILE__, __LINE__, cdstr.buffer()); \
				} while (false)

    #define UT_DBGOUT(ZZ)   do { \
				UT_WorkBuffer dbostr; \
				dbostr.sprintf ZZ ; \
				UTdbgout(__FILE__, __LINE__, dbostr.buffer()); \
			    } while (false)

    #define UT_DBGOS(ZZ)    do { \
				std::stringstream os; \
				os << ZZ ; \
				UTdbgout(__FILE__,__LINE__,os.str().c_str()); \
			    } while (false)

    #define UT_DBGPRINTF(ZZ)		printf ZZ

    #define UT_DBG_CHECKMEMORY		UTdebugCheckMemory()
    #define UT_DBG_CHECKPOINTER(PTR)	UTdebugCheckPointer(PTR)
#else
    #define UTdebugFormat(...)		((void)0)
    #define UTdebugFormatCd(...)	((void)0)
    #define UTdebugPrint(...) 		((void)0)
    #define UTdebugPrintCd(...) 	((void)0)
    #define UT_DBGOUT(ZZ)		((void)0)
    #define UT_DBGOS(ZZ)		((void)0)
    #define UT_DBGPRINTF(ZZ)		((void)0)
    #define UT_DBG_CHECKMEMORY		((void)0)
    #define UT_DBG_CHECKPOINTER(PTR)	((void)0)
#endif

// UTgetSESIlogFlag will use UT_ASSERT_LEVEL >= 1 to check if it should be
// enabled. If we don't have sufficient assertion level, then the appropriate
// environment variable passed in will be used instead.
// Note that it uses a UT_Symbol table to keep track of the different vars. 
// So it shouldn't be terribly inefficient to call multiple times.

UT_API int UTgetSESIlogFlag(int env_control);


// UTisSESImachine will efficiently check the IP address of this computer to
// see if it is one of SESI's IP addresses and returns 1 if so.
// WARNING: This can register false positive on customer machines depending on
// the network configuration.
UT_API int UTisSESImachine();

UT_API int UTdebugCheckMemory();

UT_API bool UTdebugCheckPointer(const void *ptr);

#endif /* __UT_Debug__ */
