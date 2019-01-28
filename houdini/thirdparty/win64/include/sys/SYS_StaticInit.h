/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way withoSYS_StaticInit written permission.
 *
 * NAME:	SYS_StaticInit.h (SYS Library, C++)
 *
 * COMMENTS:
 *	This file contains defines and classes to help with static initializers.
 */

#ifndef __SYS_StaticInit__
#define __SYS_StaticInit__

/// @{
///  Static initializers in C++ are very dangerous and the interactions
///  are undefined when one static initializer accesses another statically
///  defined object.
/// 
///  These macros allow you to use statically initialized objects safely, 
///  or, to call a static initialization function at startup. As well, these
///  macros handle destruction and cleanup when the last users of your 
///  code have themselved been destructed.
///
///  To avoid name conflicts, you may need to use descriptive names for
///  the static objects. Eg for an instance of a UT_Lock class, instead 
///  of the name theLock choose theSharedStringLock, when it is used in 
///  UT_SharedString.C
///
/// 
/// HOW TO USE:
/// 
///  FOR OBJECTS:
/// 
///      To have an staticly initialized object that is safe to access from 
///      other locations during static initialization, do this:
/// 
///    DECLARATION:
///
///      // In the header (.h) do this
///      SYSdeclareStaticObject(SYS_API,theFoobar);
///
///    IMPLEMENTATION
///
///      // In the implementation (.C) do this
///      SYSimplementStaticObject(theFoobar,SYS_Foobar);
/// 
///      // Then, later you can to 
///      theFoobar->doStuff();
///
///
///  FOR FUNCTIONS
/// 
///      You can also have just plain initialization and cleanup functions.
/// 
///    DECLARATION:
///
///      // In the header (.h) do this
///      SYSdeclareStaticInit(SYS_API,foobar);
///
///    IMPLEMENTATION
///
///      // Declare two void functions based on the name you chose
///      void foobarInit() {
///          // do stuff
///      }
///
///      void foobarCleanUp() {
///          // clean up stuff
///      }
///
///      // In the implementation (.C) do this
///      SYSimplementStaticInit(foobar);
///
/// HOW DOES THIS WORK?
///  
///  This works because a small statically initialized object is inserted
///  into each .C file that includes your header. That little object uses
///  reference counting to ensure that your static initialization is done
///  the first time a .o file that references your code is statically 
///  initialized.
///
///  That reference counting also makes sure that your static stuff is 
///  cleaned up when all .o's containing references have had their static
///  destructors run.
///
///

#define SYSdeclareStaticObject(API,NAME)    \
    static class API NAME##StaticInit {     \
    public:                                 \
        NAME##StaticInit ();                \
        ~NAME##StaticInit ();               \
    } NAME##Initializer;

#define SYSimplementStaticObject(NAME,TYPE) \
    static int NAME##StaticInitCounter = 0; \
    static TYPE * NAME = NULL;              \
    NAME##StaticInit::NAME##StaticInit ()   \
    {                                       \
        if (0 == NAME##StaticInitCounter++) \
            NAME = new TYPE();              \
    }                                       \
    NAME##StaticInit::~NAME##StaticInit ()  \
    {                                       \
        if (0 == --NAME##StaticInitCounter) \
            delete NAME;                    \
    }

#define SYSdeclareStaticInit(API,NAME)      \
    static class API NAME##StaticInit {     \
    public:                                 \
        NAME##StaticInit ();                \
        ~NAME##StaticInit ();               \
    } NAME##Initializer;

#define SYSimplementStaticInit(NAME)        \
    static int NAME##StaticInitCounter = 0; \
    NAME##StaticInit::NAME##StaticInit ()   \
    {                                       \
        if (0 == NAME##StaticInitCounter++) \
            NAME##Init();                   \
    }                                       \
    NAME##StaticInit::~NAME##StaticInit ()  \
    {                                       \
        if (0 == --NAME##StaticInitCounter) \
            NAME##CleanUp();                \
    }

/// @}

/// @brief Class to help deal with the "static initialization order fiasco"
///
/// Please see: http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.14
///
/// For example, something like @code
///  static UT_SymbolMap<void *>	theTable;
///  
///  void addSymbol(const char *n, void *v) { theTable.addSymbol(n, v); }
///  void delSymbol(const char *n) { theTable.deleteSymbol(n);
/// @endcode;
/// may cause problems if the @c addSymbol() method or the @c delSymbol()
/// method are called @b when the table isn't constructed (i.e. before
/// construction, after destruction).  This can happen if there's another
/// static object which has @c addSymbol() or @c delSymbol() in the code path
/// of its static destructor.
///
/// The way to "fix" this is to use the SYS_StaticInit class @code
///  static SYS_StaticInit<UT_SymbolMap<void *>> theTable;
///  
///  void addSymbol(const char *n, void *v) { theTable->addSymbol(n, v); }
///  void delSymbol(const char *n) { theTable->deleteSymbol(n);
/// @endcode;
/// @note The object will not be destructed (so there will be a memory leak)
/// @note The data object is created on first access.  This means that it may
///       not be 100% thread-safe.  Passing @c create == @c true in the
///       constructor can cause initial 

#if defined(__cplusplus)
    #include "SYS_Types.h"

    template <typename T>
    class SYS_StaticInit {
    public:
	///
	SYS_StaticInit(bool create_on_construction=false)
	{
	    if (create_on_construction)
		get();
	}
	~SYS_StaticInit()
	{
	}

	/// Allocate the object if needed.
	T	*get()
	     {
		 static T	*theObject = NULL;
		 if (!theObject)
		     theObject = new T();
		 return theObject;
	     }
	/// Accessor methods
	T	&operator*()	{ return *get(); }
	T	*operator->()	{ return  get(); }
    private:
    };
#endif

#endif
