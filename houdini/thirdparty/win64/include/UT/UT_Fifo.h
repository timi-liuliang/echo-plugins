/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Fifo.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	A generic parent class for maintaining a list of fifo
 *	event generators
 */

#ifndef __UT_Fifo__
#define __UT_Fifo__

#include "UT_API.h"
#include "UT_String.h"


class UT_Fifo;

typedef void (*UT_FifoCallback)(const char *a, int len, void *data);
typedef UT_Fifo *(*UT_FifoCreator)(UT_FifoCallback func, void *data);
typedef UT_Fifo *(*UT_NetFifoCreator)(UT_String,int,UT_FifoCallback func,void *data);

class UT_API UT_Fifo
{

public:

     UT_Fifo(UT_FifoCallback func, void *data);
     UT_Fifo(UT_String address,int port, UT_FifoCallback func, void *data);
    virtual ~UT_Fifo();

    virtual int		 open(const char *name);
    virtual int		 open();
    
    virtual void	 connect();

    virtual void	 close();

    virtual int		 isValid();
};

//
// A round-about way of creating subclass versions without direct
// dependencies

UT_API extern void UTsetFifoCreator(UT_FifoCreator  func);
UT_API extern UT_Fifo * UTgetNewFifo(UT_FifoCallback func, void *data);

UT_API extern void UTsetNetFifoCreator(UT_NetFifoCreator  func);
UT_API extern UT_Fifo * UTgetNewNetFifo(UT_String address, int port,
					UT_FifoCallback func, void *data);

#endif

