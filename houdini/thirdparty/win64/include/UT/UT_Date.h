/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Date.h ( UT Library, C++)
 *
 * COMMENTS:	Time/Date handling
 */

#ifndef __UT_Date__
#define __UT_Date__

#include "UT_API.h"
#include <time.h>
#include <iosfwd>
#include <SYS/SYS_Types.h>

class UT_WorkBuffer;
class UT_String;

class UT_API UT_Date {
public:
    // Print the time given a format like the UNIX date command
    // a   locale's abbreviated weekday name
    // A   locale's full weekday name
    // b   locale's abbreviated month name
    // B   locale's full month name
    // c   locale's appropriate date and time representation
    // C   century (a year divided by 100 and truncated to an integer) as a
    //     decimal number [00-99]
    // d   day of month as a decimal number [01-31]
    // D   date as %m/%d/%y
    // e   day of month as a decimal number [1-31] (single digits are
    //     preceded by a blank)
    // h   locale's abbreviated month name (alias for %b)
    // H   hour (24-hour clock) as a decimal number [00-23]
    // I   hour (12-hour clock) as a decimal number [01-12]
    // j   day of year as a decimal number [001-366]
    // m   month of year as a decimal number [01-12]
    // M   minute as a decimal number [00-59]
    // n   insert a newline character
    // p   string containing ante-meridian or post-meridian indicator (by
    //     default, AM or PM)
    // r   12-hour clock time [01-12] using the AM/PM notation, as %I:%M:%S
    //     %p
    // R   time as %H:%M
    // S   seconds as a decimal number [00-61], allows for leap seconds
    // t   insert a tab character
    // T   24-hour clock time [00-23], as %H:%M:%S
    // u   weekday as a decimal number [1(Monday)-7]
    // U*  week number of year (Sunday as the first day of the week) as a
    //     decimal number [00-53]. All days in a new year preceding the
    //     first Sunday are considered to be in week 0.
    // V*  week of the year (Monday as the first day of the week) as a
    //     decimal number [01-53]. If the week containing January 1 has four
    //	   or more days in the new year, then it is considered week 1;
    //     otherwise, it is week 53 of the previous year, and the next week
    //     is week 1. ( See the ISO 8601:1988 standard)
    // w   weekday as a decimal number [0(Sunday)-6]
    // W*  week of year (Monday as the first day of the week) as a decimal
    //     number [00-53].  All days in a new year preceding the first
    //     Sunday are considered to be in week 0.
    // x   locale's appropriate date representation
    // X   locale's appropriate time representation
    // y   year within century (offset from %C) as a decimal number [00-99]
    // Y   year as ccyy (4 digits)
    // Z   timezone name, or no characters if no timezone is determinable
    // %   insert a percent sign character
    // Items marked with * are not implemented (i.e. %U, %V, %W)

    // Returns the number of items printed
    static int		 dprintf(UT_WorkBuffer &buf, const char *format, time_t tm);
    
    
    static int		 dprintf(char *buf, int buf_size, const char *format, 
              		         time_t tm);
    
    // Returns the output stream
    static std::ostream	&dprintf(std::ostream &os, const char *format, 
                       	         time_t tm, int *item_count = NULL);

    // Returns the number of items printed
    static int		 dprintf(UT_WorkBuffer &buf, const char *format,
				const struct tm *tm);
    
    
    static int		 dprintf(char *buf, int buf_size, const char *format,
				const struct tm *tm);
    // Returns the output stream
    static std::ostream	&dprintf(std::ostream &os, const char *format,
				 const struct tm *tm, int *item_count = NULL);

    /// Parse a time in the format
    ///	- HH:MM:SS.SS or HH:MM:SS
    ///	- MM:SS.SS or HH:MM
    ///	- SS.SS or just SS
    /// For example 21:33.4 would return 21*60+33.4 = 1293.4
    static fpreal	parseSeconds(const char *time);

    /// Print elapsed time in seconds in the form HH:MM:SS.SS where leading HH
    /// and MM may be omitted (unless force_hours or force_minutes is true)
    /// If force_hours is true, then force_minutes is automatically true
    ///
    /// The text is appended to the work-buffer
    static void		appendPrintSeconds(UT_WorkBuffer &buf, fpreal seconds,
					bool force_hours=false,
					bool force_minutes=false,
                                        bool force_hundredths=false);

    static void		printSeconds(UT_WorkBuffer &buf, fpreal seconds,
					bool force_hours=false,
					bool force_minutes=false,
                                        bool force_hundredths=false);
};

#endif
