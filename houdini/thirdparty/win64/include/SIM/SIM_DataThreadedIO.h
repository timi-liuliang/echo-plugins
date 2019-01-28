/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_DataThreadedIO_h__
#define __SIM_DataThreadedIO_h__

#include "SIM_API.h"

#include <UT/UT_Array.h>
#include <SYS/SYS_BoostStreams.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>

class UT_IStream;


class SIM_API SIM_DataThreadedIOTask
{
public:
    SIM_DataThreadedIOTask() {}
    virtual ~SIM_DataThreadedIOTask() {}

    virtual	bool finishLoad(UT_IStream &is) = 0;
    virtual 	void finishSave(std::ostream &os) = 0;
    virtual	exint memoryEstimate() const = 0;
};

class SIM_API SIM_DataThreadedIO
{
public:
    SIM_DataThreadedIO();
    virtual ~SIM_DataThreadedIO();

    void		enqueueLoad(SIM_DataThreadedIOTask *task);
    void		enqueueSave(SIM_DataThreadedIOTask *task);

    void		completeAllSaves(std::ostream &os);
    bool		completeAllLoads(UT_IStream &is);

    bool		anyTasks() const { return myTaskList.entries() != 0; }

    exint		memoryEstimate() const;

    void		setCompressedOS(bios::filtering_stream<bios::output> *compressout) { myCompressedOS = compressout; }
    bios::filtering_stream<bios::output> *getCompressedOS() const { return myCompressedOS; }
    void		clearCompressedOS() 
    { 
	if (myCompressedOS)
	    myCompressedOS->reset();
	delete myCompressedOS;
	myCompressedOS = 0;
    }

protected:
    enum		IO_MODE
    {
	MODE_UNASSIGNED,
	MODE_SAVE,
	MODE_LOAD
    };

    IO_MODE				myMode;
    UT_Array<SIM_DataThreadedIOTask *>	myTaskList;

    bios::filtering_stream<bios::output>	*myCompressedOS;

};

#endif
