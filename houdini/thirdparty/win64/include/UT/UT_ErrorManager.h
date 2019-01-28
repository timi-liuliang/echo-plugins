/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ErrorManager.h (Utility library)
 *
 * COMMENTS:
 *		This class implements an error manager which will
 *		manage a list of error messages.
 *		NOTE: It assumes ownership of the errors that it is givin.
 *
 */

#ifndef __UT_ErrorManager_h__
#define __UT_ErrorManager_h__

#include "UT_API.h"
#include "UT_Error.h"
#include "UT_NonCopyable.h"
#include "UT_ValArray.h"
#include <SYS/SYS_Deprecated.h>


typedef UT_ValArray<UT_Error *> UT_ErrorList;

class UT_API UT_ErrorManager
{
    enum {
	FLAG_ERRORS_READ = 0x01,
	FLAG_SCAN_NEEDED = 0x02,
	FLAG_DISABLED	 = 0x04,
	FLAG_DEADLOCK	 = 0x08,
    };

public:
    UT_ErrorManager();
   ~UT_ErrorManager();

    UT_ErrorSeverity addMessage (const char *type, int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0);
    UT_ErrorSeverity addPrompt  (const char *type, int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0);
    UT_ErrorSeverity addWarning (const char *type, int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0);
    UT_ErrorSeverity addError   (const char *type, int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0);
    UT_ErrorSeverity addFatal   (const char *type, int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0);

    UT_ErrorSeverity sopAddMessage (int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0)
    { return addMessage("SOP", code, msg, loc); }
    UT_ErrorSeverity sopAddPrompt  (int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0)
    { return addPrompt("SOP", code, msg, loc); }
    UT_ErrorSeverity sopAddWarning (int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0)
    { return addWarning("SOP", code, msg, loc); }
    UT_ErrorSeverity sopAddError   (int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0)
    { return addError("SOP", code, msg, loc); }
    UT_ErrorSeverity sopAddFatal   (int code, const char *msg=0,
                                 const UT_SourceLocation *loc=0)
    { return addFatal("SOP", code, msg, loc); }

    UT_ErrorSeverity systemError(const char *msg=0);
    UT_ErrorSeverity systemWarning(const char *msg=0);

    UT_ErrorSeverity commonWarning(UT_CommonErrorCode what, const char *msg=0);
    UT_ErrorSeverity commonError(UT_CommonErrorCode what, const char *msg=0);

    UT_ErrorSeverity addDeadlockError();

    UT_ErrorSeverity addGeneric(const char *type, int code, const char *msg,
				UT_ErrorSeverity sev,
				const UT_SourceLocation *loc=0);

    UT_ErrorSeverity addGeneric(UT_Error *error);

    void 	clearAndDestroyErrors();

    int		getNumErrors() const	{ return myList.entries(); }
    const UT_Error &getError(int index) const { return *myList(index); }

    int	 	stealErrors(UT_ErrorManager &victim, 
			    int rangestart = 0, int rangeend = -1,
			    UT_ErrorSeverity severity = UT_ERROR_NONE,
			    bool borrow_only = false);

    int	 	removeError(int index);
    int		findError(const char *type, int code) const;

//  passing in a severity of NONE will return all errors
//
    int		getErrorMessages(UT_String &messages, 
				 UT_ErrorSeverity severity = UT_ERROR_NONE,
				 bool use_headers = true);

//  Returns the worst error state
//
    UT_ErrorSeverity	getSeverity()
			{
			    if( getNeedScan() )
				computeSeverity();
			    return mySeverity;
			}

//  This is for backward compatibility: use "getSeverity" !!!
    UT_ErrorSeverity	getErrorState() { return getSeverity(); }

    void		pruneDuplicates(); 

    bool		isDisabled() const
			{
			    return ((myFlags & FLAG_DISABLED) != 0);
			}
    void		setDisabled(bool f)
			{
			    if (f)
				myFlags |=  FLAG_DISABLED;
			    else
				myFlags &= ~FLAG_DISABLED;
			}

    UT_ErrorSeverity	updateSeverity(UT_ErrorSeverity sev)
			{
			    if (sev > mySeverity)
				mySeverity = sev;
			    return mySeverity;
			}

			// You typically do not need to call this method unless
			// you want to restore the error severity back to a
			// level that matches the errors recorded in the log.
    void		recomputeSeverity()
			{
			    setNeedScan(1);
			    computeSeverity();
			}

    bool		hasDeadlockError() const
			{
			    return ((myFlags & FLAG_DEADLOCK) != 0);
			}

    int64	getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myList.getMemoryUsage(false);
        for (exint i = 0; i < myList.entries(); ++i)
            mem += myList(i)->getMemoryUsage(true);
        return mem;
    }

    class Scope;

protected:
    void		setReadFlag(int state);
    int			getReadFlag() const
			{
			    return (myFlags & FLAG_ERRORS_READ) != 0;
			}

    void		setNeedScan(int state);
    int			getNeedScan() const
			{
			    return (myFlags & FLAG_SCAN_NEEDED) != 0;
			}

    void		setDeadlockError(bool onoff)
			{
			    if (onoff)
				myFlags |=  FLAG_DEADLOCK;
			    else
				myFlags &= ~FLAG_DEADLOCK;
			}
private:

    static const int	MAX_ERRORS_KEPT = 40;	// Max when doing regular add
    static const int	MAX_ERRORS_STEAL = 400;	// Max errors when stealing 

    UT_ErrorSeverity	append(UT_Error *error,
			       int max_errors = MAX_ERRORS_KEPT,
			       bool log = true);

    void		computeSeverity();

    UT_ErrorList	myList;
    UT_ErrorSeverity	mySeverity;
    char		myFlags;
};

/// A global error manager scope
class UT_API UT_ErrorManager::Scope : UT_NonCopyable
{
public:
    struct DelayTag	 {};
    static DelayTag	 Delay;

    /// Push empty error manager
    /// @{
			 Scope();
    explicit		 Scope(int thread);
    /// @}

    /// Push given error manager, must live beyond current scope. This renders
    /// all accessors on this class unusable.
			 Scope(UT_ErrorManager &errors);

    /// Construct scope which has not begun yet. Call begin() to start.
			 Scope(DelayTag)
			     : myThread(-1) {}

    /// Ends error scope if needed
			~Scope() { end(); }

    UT_ErrorManager	&getErrorManager() { return myErrorManager; }
    int			 getThread() const { return myThread; }

    UT_ErrorSeverity	 getSeverity() { return myErrorManager.getSeverity(); }

    /// Disable error accumulation for this scope, note however that the status
    /// will be maintained.
    void		 setDisabled(bool f)
			    { myErrorManager.setDisabled(f); }

    /// Get error messages 
    int			 getErrorMessages(
				UT_String &messages, 
				UT_ErrorSeverity severity = UT_ERROR_NONE,
				bool use_headers = true)
			 {
			     return myErrorManager.getErrorMessages(
					     messages, severity, use_headers);
			 }

    /// Manually begin the scope. Subsequent calls will do nothing until end()
    /// is called.
    /// @{
    void		 begin();
    void		 begin(int thread);
    /// @}

    /// Manually end the scope. Subsequent calls will do nothing.
    void		 end();

private:
    UT_ErrorManager	 myErrorManager;
    int			 myThread;
};

/// Alias UT_AutoErrorManager as UT_ErrorManager::Scope
using UT_AutoErrorManager = UT_ErrorManager::Scope;


UT_API UT_ErrorManager	*UTgetErrorManager();
UT_API UT_ErrorManager	*UTgetErrorManager(int thraed);

SYS_DEPRECATED_REPLACE(17.0, UT_AutoErrorManager)
UT_API void		 UTpushErrorManager();
SYS_DEPRECATED_REPLACE(17.0, UT_AutoErrorManager)
UT_API void		 UTpushErrorManager(int thread);
SYS_DEPRECATED_REPLACE(17.0, UT_AutoErrorManager)
UT_API void		 UTpopErrorManager();
SYS_DEPRECATED_REPLACE(17.0, UT_AutoErrorManager)
UT_API void		 UTpopErrorManager(int thread);

UT_API UT_ErrorSeverity UTaddWarning(const char *type, int code,
                                     const char *msg=0,
                                     const UT_SourceLocation *loc=0);
UT_API UT_ErrorSeverity UTaddPrompt(const char *type, int code,
				     const char *msg=0,
				     const UT_SourceLocation *loc=0);
UT_API UT_ErrorSeverity UTaddMessage(const char *type, int code,
				     const char *msg=0,
				     const UT_SourceLocation *loc=0);
UT_API UT_ErrorSeverity UTaddError  (const char *type, int code,
				     const char *msg=0,
				     const UT_SourceLocation *loc=0);
UT_API UT_ErrorSeverity UTaddFatal  (const char *type, int code,
				     const char *msg=0,
				     const UT_SourceLocation *loc=0);
UT_API UT_ErrorSeverity UTaddSystemError(const char *msg=0);
UT_API UT_ErrorSeverity UTaddCommonWarning(UT_CommonErrorCode what,
					   const char *msg=0);
UT_API UT_ErrorSeverity UTaddCommonError(UT_CommonErrorCode what,
					 const char *msg=0);
UT_API UT_ErrorSeverity UTaddDeadlockError();

/// Add a generic error message to the global error manager.
UT_API UT_ErrorSeverity UTaddGeneric(UT_ErrorSeverity sev,
                                     const char *type, int code,
                                     const char *msg = 0, 
                                     const UT_SourceLocation *loc = 0);

UT_API int		UTfindError(const char *type, int code);

UT_API UT_ErrorSeverity UTgetErrorSeverity();

#include <ostream>
UT_API int	 UTcheckOutStream(std::ostream &os, const char *m, const char *path=0);
class UT_OStream;
UT_API int	 UTcheckOutStream(UT_OStream &os, const char *m, const char *path=0);

#endif
