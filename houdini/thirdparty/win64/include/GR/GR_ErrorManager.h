/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_ErrorManager.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Class for managing viewport messages, warnings and errors.
 */
#ifndef GR_ErrorManager_h
#define GR_ErrorManager_h

#include <UT/UT_Array.h>
#include <UT/UT_Lock.h>
#include <UT/UT_Pair.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UI/UI_Value.h>

#include "GR_API.h"
#include "GR_Error.h"

class GR_API GR_ErrorManager
{
public:
    GR_ErrorManager();
		    
    enum Source
    {
	GEOMETRY=0,
	SHADER,
	MATERIAL,
	STYLESHEET,
	VIEWER,

	NUM_SOURCES
    };

    enum Severity
    {
	MESSAGE = 1,	
	WARNING,
	REGULAR_ERROR,
	FATAL_ERROR
    };

    enum Scope
    {
	REDRAW,
	UPDATE,
	REFINE,
	SESSION
    };

    int64	logMessage(Source		src,
			   UT_StringHolder	src_name,
			   Severity		sev,
			   Scope		scope,
			   GR_ErrorCode		gr_code,
			   UT_StringHolder	msg = UT_StringHolder());

    void	clearMessages(Source		src,
			      UT_StringHolder	src_name,
			      Scope		scope);

    void	clearMessages(Source	src,
			      UT_StringHolder src_name);
    void	clearMessages(Scope	scope);
    bool	clearMessage(int64 tag);
    void	clearAllMessages();

    UI_Value	&getSeverityValue() { return mySeverity; }
    UI_Value	&getChangedValue()  { return myChanged; }


    // return the total number of messages, including suppressions.
    int	 getMessages(UT_Array< UT_Pair<UT_StringHolder,Severity> > &msgs,
		     int max_errors = -1);

private:
    UI_Value	mySeverity;
    UI_Value	myChanged;
    UT_Lock	myLock;
    int64	myTagSerial;

    class gr_Error
    {
    public:
	gr_Error(Source src, Scope sc, Severity sev, GR_ErrorCode c,
		 UT_StringHolder txt, UT_StringHolder sname, int64 t)
	    : source(src), scope(sc), severity(sev), code(c), extra_text(txt),
	      source_name(sname), tag(t)
	    {}
	gr_Error()
	    : source(GEOMETRY), scope(REDRAW), severity(WARNING),
	      code(GR_NO_MESSAGE)
	    {} 
	Source		source;
	Scope		scope;
	Severity	severity;
	GR_ErrorCode	code;
	UT_StringHolder extra_text;
	UT_StringHolder source_name;
	int64		tag;
    };
    typedef UT_StringMap< UT_Array<gr_Error> *> gr_ErrorMap;
    
    gr_ErrorMap		myErrors[NUM_SOURCES];


    void recomputeSeverity();
    void processMessages(UT_Array<UT_Pair<UT_StringHolder,Severity> > &msgs,
			 const UT_Array<gr_Error *> &errors,
			 int max_errors);

};

#endif
