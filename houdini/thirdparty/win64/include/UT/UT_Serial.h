/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Serial.h ( Channel Operator Library, C++)
 *
 * COMMENTS:
 *
 *	Implements an interface to the serial port interface.
 *
 */
#ifndef UT_SERIAL_H
#define UT_SERIAL_H

#include "UT_API.h"
#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#else
#define _OLD_TERMIOS
#include <termios.h>
#endif

#define SERIAL_NO_PARITY	0
#define SERIAL_ODD_PARITY	1
#define SERIAL_EVEN_PARITY	2

class UT_API UT_Serial
{
public:
		 explicit UT_Serial(int rate	= 9600, 
			   int databits = 8,
			   int parity	= SERIAL_NO_PARITY,
			   int stopbits = 1);
    
    virtual	~UT_Serial();
		    
    int		 open(const char *port, int parms);
    void	 close();

    int		 changeRate(int rate);

    int		 dataAvailable();
    int		 read(void *buffer, int num);
    int		 write(const void *buffer, int num);

    void	 flush();
    
private:
    int		  mySpeed;
    int		  myDataBits;
    int		  myStopBits;
    int		  myParity;

    // platform specific
#ifdef WIN32
    HANDLE	   myFD;
    COMMCONFIG	   myPreviousState;
    COMMCONFIG	   mySerialState;
    int		   mySerialStateSaved;
    int		   myDataAvailable;
    char	   myStoredChar;
#else
    int		   myFD;
    struct termios myPreviousState;
    struct termios mySerialState;
#endif
};

#endif
