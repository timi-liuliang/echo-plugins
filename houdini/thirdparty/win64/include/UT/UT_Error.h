/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Error.h (Utility Library, C++)
 *
 * COMMENTS:
 *		This class is used to store an error or warning, it is
 *		used in conjunction with a global UT_ErrorManager.
 *
 */

#ifndef __UT_Error__
#define __UT_Error__

#include "UT_API.h"
#include "UT_String.h"
#include "UT_SourceLocation.h"

class UT_Color;
class UT_StringHolder;
class UT_WorkBuffer;

enum UT_ErrorSeverity
{
    UT_ERROR_NONE = 0,
    UT_ERROR_MESSAGE,	// just for communication (eg "ignoring point normals")
    UT_ERROR_PROMPT,	// something that should catch the user's attention
    UT_ERROR_WARNING,	// something bad happened but continuing is an option
    UT_ERROR_ABORT,	// bad error which meant discontinuing current action
    UT_ERROR_FATAL,	// something bad from which we can't recover
    UT_NUM_ERROR_SEVERITIES
};

// NOTE:
// UT_ERROR_JUST_STRING is meant for convenience for times when you need
// to list things, or to use more than one string in an error. eg:
//	Error: Missing token "{"
//	  ref: Animated Parameter Load

enum UT_CommonErrorCode
{
    UT_ERROR_FILE_NOT_FOUND = 0,
    UT_ERROR_EOF,
    UT_ERROR_PARSING,
    UT_ERROR_FILE_FORMAT,
    UT_ERROR_OUTSTREAM,		// output to stream failed in %s.
    UT_ERROR_INSTREAM,		// input from stream failed in %s.
    UT_ERROR_EARLY_STREAM_END,	// unexpected end of input stream is %s.
    UT_ERROR_SKIP_PACKET_CLASS,	// ignoring unrecog data packet (class) in %s.
    UT_ERROR_SKIP_PACKET_ID,	// ignoring unrecog data packet (id) in %s.
    UT_ERROR_SKIP_TOKEN,	// ignoring unrecog token "%s".
    UT_ERROR_BAD_TOKEN,		// unrecog token "%s". Unable to continue
    UT_ERROR_MISSING_TOKEN,	// missing token "%s".
    UT_ERROR_BAD_INT,		// Invalid integer specification (%s)
    UT_ERROR_BAD_FLOAT,		// Invalid float specification (%s)
    UT_ERROR_JUST_STRING,	// %s		-- see note above
    UT_ERROR_BAD_EXTENSION,	// Unknown file extension (%s)
    UT_ERROR_BAD_CPIO_HEADER,	// Bad header in CPIO packet
    UT_ERROR_BAD_CPIO_CONTENTS,	// Bad contents in CPIO packet
    UT_ERROR_CANT_MKDIR_FILE_EXISTS,	// Can't make directory, file exists
    UT_ERROR_FILE_NOT_WRITEABLE,	// Unable to write to file (%s)
    UT_ERROR_ABORT_BY_USER,	// Action aborted by user: %s
    UT_ERROR_LOW_VIDEO_RAM,	// Houdini is running low on graphics VRAM
    UT_ERROR_DEADLOCK,		// Deadlock
    UT_ERROR_TOO_MANY,		// (Too many errors detected)
    UT_ERROR_HOTKEY_REPORT,	// Hotkey pressed; echo command.
    UT_ERROR_MINIMUM_CPU,	// minimum cpu requirement failure
};

#define UT_CE_EARLY_INPUT_END   UT_ERROR_EOF
#define UT_CE_PARSE_ERROR       UT_ERROR_PARSING
#define UT_CE_FILE_ERROR        UT_ERROR_FILE_FORMAT

class UT_API UT_Error
{
public:

    // NOTE: A copy of string is stored. String is just an optional
    // message associated with the error (eg a file name).
    // "type" is usually a library prefix or it can be either "System"
    // or "Common".
    UT_Error(const char *error_type,
	     const int error_code,
	     const char *error_string = 0,
	     const UT_ErrorSeverity = UT_ERROR_ABORT,
	     const UT_SourceLocation *location = NULL);
    virtual		~UT_Error();

    virtual int64	 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myErrorType.getMemoryUsage(false);
        mem += myErrorString.getMemoryUsage(false);
        mem += myLocation.getMemoryUsage(false);
        return mem;
    }

    // If you're stringing several errors together, you should set
    // last to the severity of the last error for which you got the
    // error message, so that if the severity hasn't changed, we won't
    // print the "Error/Warning/Message" headers before the message.
    //
    // if the use headers flag is off, then the headers (nor spaces in
    // their place) will be prepended to the message
    void		 getErrorMessage(UT_String &error_str,
				 UT_ErrorSeverity last_severity = UT_ERROR_NONE,
				 bool useheaders = true) const;
    void		 getErrorFormattedString(
				UT_WorkBuffer &error_str) const;

    const UT_String	&getErrorType() const	{ return myErrorType; }
    UT_ErrorSeverity	 getSeverity()  const	{ return mySeverity; }
    int			 getErrorCode() const	{ return myErrorCode; }
    const UT_String	&getString()    const	{ return myErrorString; }
    const UT_SourceLocation &getLocation() const { return myLocation; }

    void		 setErrorType(const char *type);
    void		 setSeverity(UT_ErrorSeverity);
    void		 setErrorString(const char *string);
    void		 setErrorCode(int code);
    void		 setErrorLocation(const UT_SourceLocation &loc);
 
    // getFileString will open the file specified in the filename parameter
    // and search for the entry with the given error_code. The entry is
    // placed in the buffer.
    //
    // The format of the file is:
    // # comments
    // <error_code><single character (non digit)><error message>
    //
    // in other words, an error code (string of digits)
    // a character (typically a tab)
    // and a newline terminated string
    // eg:
    // 145	Generic Error Message.
    //
    // Note: You might have a %s in the message somewhere. And also,
    // only the first nondigit is ignored. All other chars until
    // the newline, are significant.
    // Also, all lines with the same code are appended to each other.
    //
    // If the error is not found, 0 is returned.
    //
    // Note: These files are cached after reading, you have to restart
    // Houdini to get them to clear out (or write a clearErrorCache method)
    static bool	getErrorFormat(UT_StringHolder &format,
			const char *error_file,
			int error_code);
    static bool	getFileString(UT_String &error_str,
			const char *error_file,
			int error_code,
			UT_ErrorSeverity severity,
			UT_ErrorSeverity last_severity,
			bool useheaders);

private:
    UT_String		 myErrorType;
    UT_String		 myErrorString;
    UT_ErrorSeverity	 mySeverity;
    int			 myErrorCode;
    UT_SourceLocation	 myLocation;
};

#endif
