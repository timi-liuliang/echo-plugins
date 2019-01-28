/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MidiFile.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *		This class reads a MIDI file. The events 
 *		are stored until the object is destroyed.
 */
#ifndef __UT_MidiFile__
#define __UT_MidiFile__

#include "UT_API.h"
#include <stdio.h>

#include "UT_Midi.h"
#include "UT_Endian.h"
#include "UT_ValArray.h"
#include "UT_IntArray.h"
#include "UT_Array.h"

class UT_MidiFile;

// event class (different from UT_Midi's midiEvent because of file
// meta-events)
//
class UT_API utMidiFileEvent
{
public:
    utMidiFileEvent();
    ~utMidiFileEvent();

    void	read(FILE *,UT_MidiFile *from);
    void	write(FILE *,UT_MidiFile *from);

    int		isARealEvent();
    int		isEnd();
    int		isTempoChange();

    int		getChannel();
    uint64	getTime();

    void	toMidiEvent(midiEvent &, UT_Midi *);
    void	toMidiFileEvent(midiEvent &, UT_Midi *);
    long	getTempo();

    static long readVariableLength(FILE *);
    static void writeVariableLength(FILE *,long);

    friend class UT_MidiFile;
private:
    unsigned char	 myStatus;
    unsigned char	 myByte1;
    unsigned char	 myByte2;
    int			 myTempo;

    // system exclusives
    long		 myMaxSysLength;
    long		 mySysLength;
    unsigned char	*mySys;
    int			 myBarPending;
    float		 myBarValue;
    
    uint64		 myTime;
};


class UT_API UT_MidiFile
{
public:
    UT_MidiFile(UT_Midi *myMidi);
    ~UT_MidiFile();

    void	rewind();
    uint64	getLastTime();

    int		open(const char *name,int read,fpreal st=0,fpreal end=0);
    int		openWrite(const char *, UT_Array<midiEvent> &events);
    
    int		hasMidiInput(uint64 up_to_time,
			     UT_IntArray &channel);
    
    void	getMidiInput(uint64 up_to_time,
			     UT_Array<midiEvent> &midi_input,
			     UT_IntArray &channel);

    int		sendMidiOutput(UT_Array<midiEvent> &midi_output,
			       int channel);
    
    void	clearQueue();

    // called by utMidiEvent-----
    uint64		currentTime();
    void		setCurrentTime(uint64);
    unsigned char	lastCommand();
    void		setLastCommand(unsigned char);
    uint64		convertFromDeltaTicks(unsigned long delta);
    long		convertToDeltaTicks(uint64 ticks);
    // -----
    
private:
    int		openRead(const char *,fpreal start,fpreal end);

    int		readHeader(FILE *);
    void	readTrack(FILE *,int tindex,fpreal start,fpreal end);
    void	sequenceEvents();

    void	writeHeader(FILE *);
    void	writeTrack(FILE *, UT_Array<midiEvent> &events);

    unsigned short		  myFormat;
    unsigned long		  myTicksPerSec;
    int				  myResolution;
    uint64			  myCurrentTime;
    long			  myLastEvent;
    unsigned char		  myLastCommand;
    fpreal64			  myCumulativeError;
    UT_Midi			 *myMidi;

    UT_ValArray<utMidiFileEvent*>			myEvents;
    UT_ValArray< UT_ValArray<utMidiFileEvent *> *>	myTrackEvents;
};
    
#endif
