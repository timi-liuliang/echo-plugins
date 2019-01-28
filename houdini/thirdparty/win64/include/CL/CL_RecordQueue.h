/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL Library (C++)
 *
 * COMMENTS:	Records samples, pushing old ones out of the queue if no
 *		space is available.
 *
 */
#ifndef CL_RECORD_QUEUE_H
#define CL_RECORD_QUEUE_H

#include "CL_API.h"
#include <SYS/SYS_Types.h>

#define CL_RECORD_MODE_OVERWRITE	0
#define CL_RECORD_MODE_EXTEND		1
#define CL_RECORD_MODE_ADD		2
#define CL_RECORD_MODE_CYCLE		3

class UT_IStream;
class UT_OStream;

class CL_API CL_RecordQueue
{
public:
		 CL_RecordQueue(int mode, int size);
    virtual	~CL_RecordQueue();

    void	 reset();
    
    void	 setRecordMode(int mode);
    void	 addSamples(int num, const fpreal *samples);

    int		 getFullRangeSize();
    int		 getTimeSliceSize();

    int		 getFullRange(int max, fpreal *samples);
    int		 getTimeSlice(int max, fpreal *samples);
    fpreal	 getCurrentFrame();

    fpreal	 getSample(fpreal index);

    fpreal	*getData() { return myData; }
    int		 size()    { return myRecordPos+1; }
    int		 actualSize() { return mySize; }
    
    void	 resize(int size, int clear = 0);

    void	 save(UT_OStream &os);
    bool	 load(UT_IStream &is, int version);

private:

    
    int		 myRecordMode;
    int		 mySize;
    int		 myChunk;
    int		 myRecordPos;
    int		 myLastRecordPos;
    int		 myLastTimeSliceLength;
    int		 myCumulativeLength;
    fpreal	*myData;
};


#endif
