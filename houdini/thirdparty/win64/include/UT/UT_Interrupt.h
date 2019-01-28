/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Interrupt.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_Interrupt__
#define __UT_Interrupt__

#include "UT_API.h"

#include "UT_Array.h"
#include "UT_Lock.h"
#include "UT_Signal.h"
#include "UT_String.h"
#include "UT_StringArray.h"
#include "UT_StringHolder.h"
#include "UT_SysClone.h"
#include "UT_ThreadSpecificValue.h"
#include "UT_TimeGate.h"
#include "UT_ValArray.h"

#ifndef GAMEOS
    #include "UT_SharedMem.h"
#endif

#include <sys/types.h>
	
class ut_IserverInterface;
class ut_Iclient;
class UT_Interrupt;
class UT_InterruptMessage;

typedef void	(*UT_InterruptCB)(UT_Interrupt *me, int state, const char *op);
typedef void	(*OPUI_UpdateModeCB)();
typedef bool	(*UI_CheckForEscapeCB)();

typedef UT_StringHolder	(*UT_InterruptMessageCB)(const UT_InterruptMessage *me);

/// An intentionally light weight class to hold interrupt messages
/// The idea is people can format them with their callback methods.
/// This isn't virtual as we want to build an array of them by value
/// at this level.
/// You should harden any info you need.  Do not rely on your callback
/// being invoked in your thread or during your scope!
class UT_API UT_InterruptMessage
{
public:
    UT_InterruptMessage() { myBuildMessageCallback = 0; }
    UT_InterruptMessage(UT_InterruptMessageCB cb) { myBuildMessageCallback = cb; }
    UT_InterruptMessage(UT_InterruptMessageCB cb, const UT_StringHolder &str) { myBuildMessageCallback = cb; myText = str; }

    const UT_StringHolder	&buildMessage() const
    {
	if (myCachedText.isstring()) return myCachedText;
	if (!myBuildMessageCallback) return myCachedText;
	myCachedText = myBuildMessageCallback(this);
	return myCachedText;
    }

private:
    mutable UT_StringHolder	myCachedText;
    UT_InterruptMessageCB	myBuildMessageCallback;

    // These are public so the callbacks can use them.
public:
    UT_StringHolder		myText;

    // We have our values.  Intentionally *not* including
    // pointers since there is no destructor and no scope.
    // The point of this is to be *fast*.
    union {
	int		i;
	float		d;
    } value1;
    union {
	int		i;
	float		d;
    } value2;
    union {
	int		i;
	float		d;
    } value3;
    union {
	int		i;
	float		d;
    } value4;
};

class UT_API UT_InterruptHandler
{
public:
    virtual ~UT_InterruptHandler() {}
    virtual void	start(UT_Interrupt *intr,
			      const UT_InterruptMessage &msg,
			      const UT_StringRef &main_optext,
			      int priority) =0;
    virtual void	push(UT_Interrupt *intr,
			      const UT_InterruptMessage &msg,
			      const UT_StringRef &main_optext,
			      int priority) =0;
    virtual void	busyCheck(bool interrupted,
				  float percent,
				  float longpercent) = 0;
    virtual void	pop() =0;
    virtual void	stop() =0;

    virtual void	interruptAllowed(bool allowed, bool allow_ui) =0;
};

//
// These state values are passed to the active callback:
//
enum
{
    ACTIVE_STOP,		// Final opEnd
    ACTIVE_START,		// First opStart
    ACTIVE_PUSH,		// opStart
    ACTIVE_POP,			// opEnd
    ACTIVE_BUSY,		// opInterrupt
    ACTIVE_ALLOW_CHANGE		// myInterruptable has changed state
};


#ifdef GAMEOS
class UT_API UT_Interrupt
#else
class UT_API UT_Interrupt : protected UT_SharedMem
#endif
{
public:
    /// When interrupts nest, it is not always useful to display the
    /// entire stack of interrupts.  Priority is used to control if
    /// the interrupt should be shown.
    /// 1) All PERMANENT messages are shown.
    /// 2) Only the topmost NODE is shown, provided it isn't shadowed
    /// by a PERMANENT.
    /// 3) Only the topmost TRANSIENT is shown, provided it isn't shadowed
    /// by a PERMANENT or NODE.
    /// Node cooking in Houdini uses the NODE level, so for most ad-hoc
    /// interrupt messages the TRANSIENT level is best.
    enum Priority
    {
	PRIORITY_TRANSIENT = 0,
	PRIORITY_NODE = 1,
	PRIORITY_PERMANENT = 2,
    };
    explicit UT_Interrupt(const char *app_title = 0);
    virtual ~UT_Interrupt();

    // The interrupt object must be enabled for interrupts to be
    // allowed. The initial state is disabled.
    // If print_longtext is 1, then each time the "long" text is changed
    // with setLongOpText(), the text will be printed onto stdout.
    void	setEnabled(int state, int print_longtext = 0);
    bool	isEnabled() const { return (bool) myEnableFlag; }

    // If this class is being used in a single-threaded app then call
    // setUseLocks(0); Default is 1.
    void	setUseLocks(int state) { myUseLockFlag = (state != 0); }

    // If this flag is set to false, the idialog is started in uninterruptable
    // mode. This makes idialog basically a status display. Also, on threaded
    // UI, hitting ESC will do nothing.
    //
    // setInterruptable(true) returns false if UT_Interrupt was already
    // interrupted, and hence will fail to turn off the interruptable flag. If
    // prev_enabled is non-NULL, then it is set to the previous flag state,
    // regardless of the return value.
    // NB: setInterruptable() will perform locking using myInterruptableLock.
    bool	setInterruptable(bool allow, bool *prev_enabled = NULL,
				 bool allow_ui = false);
    bool	getInterruptable() const	{ return myInterruptable; }
    UT_Lock &	getInterruptableLock()		{ return myInterruptableLock; }
    bool	getAllowUI() const		{ return myAllowUI; }

    // Normally the myInterruptedFlag is reset as soon as the outermost
    // opStart/opEnd pair is closed.  This does not work very well
    // if we need to track the interrupt flag across returning into
    // the event loop.  Similarly, we may wish to leave the interrupt
    // in a sticky state until someone manually re-enables cooking.
    // clearStickyInterrupts() will both clear the sticky interrupt
    // flag *AND* clear the interrupt flag.
    void	clearStickyInterrupts();
    bool	stickyInterrupts() const { return myInterruptSticky; }
    void	setStickyInterrupts();

    int		getOpDepth() const { return myOpDepth; }

    /// To use the long operation mechanism invoke "opStart" once,
    /// giving the name of the operation and (if possible) an estimate
    /// of the number of calls which are planned to be made to the
    /// opInterrupt method. The start method will return zero if the
    /// operation has already been terminated (possibly from a containing long
    /// operation) and should not even start. In that case, you *must* still
    /// call opEnd(). It will return one if the operation can proceed normally.
    ///
    /// During the long operation, invoke the "opInterrupt" method
    /// (if possible, giving a value between 0 and 100 indicating the
    /// percentage of the operation that is complete). This method will
    /// return zero if the operation should continue and one if a request
    /// was made to terminate. When the operation is complete, invoke the
    /// "opEnd" method. This should be called even if the operation was
    /// interrupted.
    ///
    /// The 'immediate_dialog' flag causes the idialog program to be started
    /// immediately instead of waiting for a timeout. This is the only way to
    /// start the idialog progress monitor, and should only be used for very
    /// long operations where the user would minimize Houdini (like rendering)
    ///
    /// The opid can be used to verify nesting of interrupts.  It can be
    /// passed to opEnd to verify that the nesting is proper...
    ///
    /// These methods are now thread safe. Only the main thread will start
    /// the interrupt server. 
    // @{
    int		 opStart(const char *opname = 0, int npoll = 0,
			 int immediate_dialog=0, int *opid=0);
    int		 opStartPriority(int priority, const char *opname = 0, int npoll = 0,
			 int immediate_dialog=0, int *opid=0);
    int		 opStartMessage(int priority, const UT_InterruptMessage &msg, int immediate_dialog = 0, int *opid = 0);

    int		 opInterrupt(int percent = -1);

    void	 opEnd(int opid=-1);
    // @}

    /// Call this before a new set of opStart()s are called. It will also 
    /// cause idialog to configure the user interface with two text boxes
    /// instead of the usual single line. After that, you can call
    /// setLongOpText() to modify the second line message.
    // @{
    void	 setLongOpText(const UT_StringHolder &longoptext);
    void	 setLongPercent(float percent);
    // @}

    // The application title string will be placed in the percent
    // completion dialog.
    void	 setAppTitle(const char *title);
    UT_StringHolder getAppTitle() const { return myAppTitle; }

    // The active callback is invoked whenever opStart and opEnd are called.
    // This lets someone else keep track of when we're inside an interruptible
    // loop. The Interrupt Handler is similar but C++ based.
    void	 setActiveCallback(UT_InterruptCB function)
    		 { myActiveCallback = function; }
    
    void	 setInterruptHandler(UT_InterruptHandler *hand)
		 {
		     myUIHandler = hand;
		     if(hand)
			 hand->interruptAllowed(myInterruptable, myAllowUI);
		 }
    UT_InterruptHandler *getInterruptHandler() { return myUIHandler; }

    // Set the 'ourMemoryThresholdCallback' ptr to 'function'. See the
    // declaration of ourMemoryThresholdCallback for more details.
    static void	 setUpdateModeCallback(OPUI_UpdateModeCB function)
		 { ourMemoryThresholdCallback  = function; }
    
    // Set the 'ourEscapeCheckCallback' ptr to 'function'.  See the 
    // declaration of ourEscapeCheckCallback for more details
    static void	 setEscapeCheckCallback(UI_CheckForEscapeCB function)
		 { ourEscapeCheckCallback  = function; }
    static bool	 getIsEscapeCheckCallbackSet()
		 { return ourEscapeCheckCallback; }
    
    // The interrupt callbacks are called when the user interrupts Houdini.
    // Add an interrupt callback when the operation being called needs to
    // do extra work to interrupt the main thread.  For example, to stop
    // python execution, an interrupt callback is needed to raise a python
    // exception.
    //
    // Only the callback that was pushed last will be called.
    void	 pushInterruptCallback(void (*callback)());
    void	 popInterruptCallback();

    // You can manually interrupt an active loop by calling this method.
    // If calling from a thread other than the looping thread then set
    // the use_semaphore flag.
    void	 interrupt(int use_semaphore = 0);

    void	 pause(bool dopause);

    bool	 isPaused() const;

    // This is the signal handler that gets used when we're not in an
    // interruptable state.
    void	 setSignalCallback(UTsigHandler function);

    // test if we're within the memory threshold, and if not, return true.
    bool	 testMemoryThreshold()
		 {
		     return ourMemoryThresholdCallback
				?  doTestMemoryThreshold() : false;
		 }
protected:
    // These methods are used by the interrupt server to get access
    // to the shared memory segment.
    const char		*getSharedKey() const;

    virtual int		 detach();

#ifdef GAMEOS
    void	*getMemory(int reuse=1)	{ return 0; }
    void	 destroy()	{}
    int		 setKey(const char *, int id=0)	{ return -1; }
    int		 setKey(key_t key)	{ return -1; }
    int		 setSize(int size)	{ return size; }
    UT_String	 ourKeyPath;
#endif

private:
    bool		 doTestMemoryThreshold();
    void		 testIdialogInterrupt();
    void		 testEscCallbackInterrupt();

    void		 updateTitle(const char *title);
    void		 operationComplete();
    void		 startServer();
    void		 stopServer();
    UT_StringHolder	 getOpName() const;
    void		 callInterruptCB();


    UT_Array<UT_InterruptMessage> 	 myOpNameStack;

    UT_Array<void(*)()> myInterruptCBStack;

    UT_StringHolder	 myAppTitle;
    UT_StringHolder	 myLongOpText;
    UT_InterruptCB	 myActiveCallback;
    UT_InterruptHandler *myUIHandler;
    char		*myServerPath;
    char		 myPidString[16];
    const char		*myArgv[16];
    int			 myOpDepth;
    UT_Interrupt	*myPreviousInterrupt;
    UT_Signal		*mySignal;
    UT_Signal		*mySignalSubstitute;
    UTsigHandler	 mySignalCallback;
    pid_t		 myServerPID;

    int			 myStartTime;

    short		 myEnableFlag;
    short		 myStartedFlag;
    short		 myInterruptedFlag;
    char		 myPrintLongTextFlag;
    char		 myUseLockFlag;
    bool		 myInterruptable;
    bool		 myAllowUI;
    bool		 myInterruptSticky;
    float		 myPercent;
    float		 myLongPercent;
    volatile bool	 myPaused;

    UT_ThreadSpecificValue<int> myTLSDepth;

    // This is how we let OPUI_App know we'd like to change the update mode
    // to OPUI_UPDATE_ON_DEMAND when we're running out of virtual addresses
    static OPUI_UpdateModeCB 	 ourMemoryThresholdCallback;
    
    // This is how we ask the UI layer to check whether an escape key has
    // been pressed in the case where we do not have threaded UI.
    static UI_CheckForEscapeCB   ourEscapeCheckCallback;

    UT_Lock		 myLock;
    UT_Lock		 myInterruptableLock;

    UT_TimeGate<100>	 myTimeGate;

    static void		 ut_interruptHandler(UTsignalHandlerArg);
};

/// Obtain global UT_Interrupt singleton.
UT_API extern UT_Interrupt	*UTgetInterrupt();
UT_API extern void		 UTsetInterrupt(UT_Interrupt *);

// a template for the comm layout for the shared memory link between
// UT_Interrupt and idialog.
struct ut_InterruptTransactionData
{
					// I/O is relative to UT_Interrupt.
    volatile int   myCommand;		// In: 0 none, 1 interrupt, 2 pause
    volatile float myPercent;		// Out: current task % complete
    volatile float myLongPercent;	// Out: primary task % complete
    volatile int   myNumMessages;	// Out: # of messages in message area
    volatile char  myMessage[128];      // Out: message
    volatile char  myLongMessage[128];  // Out: message for long ops
};

// A simple client which can be used in processes spawned by Houdini (such
// as mantra). It allows these processes to report the completion percentage,
// and read the interrupt/pause state.
#ifdef GAMEOS
class UT_API UT_InterruptClient
#else
class UT_API UT_InterruptClient : public UT_SharedMem
#endif
{
public:
    UT_InterruptClient(pid_t server_id);
    ~UT_InterruptClient();

    bool	isInterrupted();
    
    void	setPercent(float p);
    void	setLongPercent(float p);

    void	setMessage(const char *title);

#ifdef GAMEOS
    void	*getMemory(int reuse=1)	{ return 0; }
    void	 destroy()	{}
    int		 setKey(const char *, int id=0)	{ return -1; }
    int		 setKey(key_t key)	{ return -1; }
    int		 setSize(int size)	{ return size; }
    UT_String	 ourKeyPath;
#endif

};

/// Helper class to ensure that opEnd() is always called, even if you
/// return early from a function. It's similar to UT_AutoUndoBlock.
class UT_API UT_AutoInterrupt
{
public:
    UT_AutoInterrupt(const char* operation_name,
		     UT_Interrupt *boss=UTgetInterrupt())
    : myInterrupt(boss)
    {
	myRunOk = myInterrupt
		    ? (bool)myInterrupt->opStart(operation_name, 0, 0, &myId)
			      : true;
    }

    ~UT_AutoInterrupt()
    {
	if (myInterrupt)
	    myInterrupt->opEnd(myId);
    }

    bool wasInterrupted()
    {
	if (myRunOk && myInterrupt && myInterrupt->opInterrupt())
	    myRunOk = false;
	return !myRunOk;
    }

    bool wasInterrupted(int percent)
    {
	if (myRunOk && myInterrupt && myInterrupt->opInterrupt(percent))
	    myRunOk = false;
	return !myRunOk;
    }

    UT_Interrupt	*getInterrupt() const	{ return myInterrupt; }

private:
    UT_Interrupt	*myInterrupt;
    bool		 myRunOk;
    int			 myId;
};

#endif /* __UT_Interrupt__ */
