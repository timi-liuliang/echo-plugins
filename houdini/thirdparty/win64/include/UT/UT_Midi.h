/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Midi.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *		This is a generic midi class to contain midi
 *		methods without creating link dependencies
 */

#ifndef __UT_Midi__
#define __UT_Midi__

#include "UT_API.h"
#include <string.h>

#include "UT_Endian.h"
#include "UT_Array.h"
#include "UT_ValArray.h"
#include "UT_IntArray.h"

class UT_MidiFile;

#ifdef WIN32
    #define MIDI_IN_DEVICE_BASE		128
    #define MIDI_OUT_DEVICE_BASE	256
#else
    #define MIDI_IN_DEVICE_BASE		4096
    #define MIDI_OUT_DEVICE_BASE	8192
#endif
#define MIDI_IN_FILE_BASE	12288
#define MIDI_OUT_FILE_BASE	16384
#define MIDI_NUM_DEVICES	4095

/* timing modes */
#define MD_NOSTAMP           1
#define MD_DELTASTAMP        2
#define MD_RELATIVESTAMP     3
#define MD_DELTATICKS        4	    /* delta ticks, as in SMF 1.0 */
#define MD_RELATIVETICKS     5	    /* relative ticks */

#define MD_STAMPING_MASK     0xf

#define MD_CHANNELMASK	 	    0x0F
#define MD_STATUSMASK	 	    0xF0

#define MD_CHANNELVOICE		    0x80
#define MD_NOTEOFF		    0x80
#define MD_NOTEON		    0x90    /* note-off is also note-on w/velocity=0 */
#define MD_POLYKEYPRESSURE	    0xA0    /* polyphonic key pressure or aftertouch */
#define MD_CONTROLCHANGE	    0xB0
#define MD_CHANNELMODESELECT	    0xB0
#define MD_PROGRAMCHANGE	    0xC0
#define MD_CHANNELPRESSURE	    0xD0    /* aftertouch is the Yamaha term */ 
#define MD_PITCHBENDCHANGE	    0xE0

#ifdef MD_SYSEX
#undef MD_SYSEX
#endif
#define MD_SYSEX		    0xF0	/* System Exclusive */
#define MD_SYSTEMREALTIME	    0xF8
#define MD_TIMINGCLOCK		    0xF8
#define MD_UNDEFINED3		    0xF9
#define MD_START		    0xFA
#define MD_CONTINUE		    0xFB
#define MD_STOP			    0xFC
#define MD_UNDEFINED4		    0xFD
#define MD_ACTIVESENSING	    0xFE
#define MD_SYSTEMRESET		    0xFF
#define MD_META			    0xFF	/* MIDI Files only */
#define MD_SYSTEMCOMMON		    0xF1
#define MD_TIMECODEQUARTERFRAME	    0xF1
#define MD_SONGPOSITIONPOINTER	    0xF2
#define MD_SONGSELECT		    0xF3
#define MD_UNDEFINED1		    0xF4
#define MD_UNDEFINED2		    0xF5
#define MD_TUNEREQUEST		    0xF6
#define MD_EOX			    0xF7	/* End of System Exclusive */

#define MAX_SYSEXMSG_LEN	    128

#define SYSEX_TYPE_NONE			(0)
#define SYSEX_TYPE_BARMARKER		(1)

#define MESSAGE_VALUE_RAW		(-1)
#define MESSAGE_VALUE_NORM		(-2)
#define MESSAGE_DONTCARE		(-3)
#define MESSAGE_STOP			(-4)


class UT_API midiEvent
{
public:

    char		 msg[3];
    uint64		 time;
    char		 bar;
    float		 barvalue;
    int			 trackNum;
    char		 sysexmsg[MAX_SYSEXMSG_LEN];
    int			 sysexlen;
    

		midiEvent()		{ sysexlen = 0; trackNum=-1; }
		explicit midiEvent(const int)	{ sysexlen = 0; trackNum=-1; }

		midiEvent(const midiEvent &mdv)
		{ 
		    msg[0] = mdv.msg[0];
		    msg[1] = mdv.msg[1];
		    msg[2] = mdv.msg[2];
		    time = mdv.time;
		    bar = mdv.bar;
		    barvalue = mdv.barvalue;
		    trackNum = -1;

		    memcpy(sysexmsg, mdv.sysexmsg, mdv.sysexlen);
		    sysexlen = mdv.sysexlen;
		}

		~midiEvent() 
		 { };

    midiEvent  &operator=(const midiEvent &mdv)
		{ 
		    msg[0] = mdv.msg[0];
		    msg[1] = mdv.msg[1];
		    msg[2] = mdv.msg[2];

		    time = mdv.time;
		    bar = mdv.bar;
		    barvalue = mdv.barvalue;
		    trackNum = mdv.trackNum;

		    memcpy(sysexmsg, mdv.sysexmsg, mdv.sysexlen);
		    sysexlen = mdv.sysexlen;

		    return *this;
		}

    int		operator==(const midiEvent &mdv)
		{
		    if (msg[0] == mdv.msg[0] &&
		        msg[1] == mdv.msg[1] &&
		        msg[2] == mdv.msg[2] &&
			time == mdv.time &&
			sysexlen == mdv.sysexlen)
		    {
			return 1;
		    }
		    else
		    {
			return 0;
		    }
		}
};

class UT_API UT_Midi 
{

public:

     UT_Midi();
    virtual ~UT_Midi();

    virtual int		numInterfaces(int indevices);
    virtual const char *interfaceName(int n, int indevices);
    
    virtual int		openPort(const char *portname, int read);
    virtual int		openFile(const char *filename, int read,
	    			 fpreal start, fpreal end);
    virtual int		openFile(const char *filename, 
	    			 UT_Array<midiEvent> &events);

    // these five methods have both a file and a device version.
    // the device versions are implemented in $MIDI/MIDI_Interface.
    // the file versions are implemented in this file.
    //
	    void	closePort(int port);
    
	    int		hasMidiInput(int port, uint64 up_to_time,
				     UT_IntArray &channel);
    
	    void	getMidiInput(int port,
				     uint64 up_to_time,
				     UT_Array<midiEvent> &midi_input,
				     UT_IntArray &channel);

	    int		sendMidiOutput(int port,
				       UT_Array<midiEvent> &midi_output,
				       int channel);

	    void	clearQueue(int port);
    ////

	    uint64	getLastFileTime(int port);
    
    virtual uint64	getTime(int port);
    virtual void	setTimerCallback(void (*callback)(void *));

    virtual void	startRecordTimer(void *, int time);
    virtual void	stopRecordTimer(void *);
	    void	setPrintMessage(int);

    virtual uint64	markOffsetTime(int port);

	    int		identifySysex(const char *, int len, float *value=0);

	    void	printMessage(midiEvent *);
	    int		printMessages() 	{ return myPrintMessageFlag; }

	    void	setBarMarkerMessage(const int *a)
	    		{ myBarMarkerMessage = a; }

protected:
    // All Device methods are empty at this level. Implemented in
    // MIDI_Interface.{C,h}
    //
    virtual int		hasDeviceInput(int port,uint64 up_to_time,
				     UT_IntArray &channel);
	    int		hasFileInput(int port,uint64 up_to_time,
				     UT_IntArray &channel);
  
    virtual void	getDeviceInput(int port,
				     uint64 up_to_time,
				     UT_Array<midiEvent> &midi_input,
				     UT_IntArray &channel);
	    void	getFileInput(int port,
				     uint64 up_to_time,
				     UT_Array<midiEvent> &midi_input,
				     UT_IntArray &channel);
    virtual int		sendDeviceOutput(int port,
				       UT_Array<midiEvent> &midi_output,
				       int channel);

	    int		sendFileOutput(int port,
				       UT_Array<midiEvent> &midi_output,
				       int channel);

    virtual void	clearDeviceQueue(int port);
	    void	clearFileQueue(int port);

    virtual void	closeDevice(int port);
	    void	closeFile(int port);

            int		myPrintMessageFlag;

    uint64			 myLastTime;
private:

    UT_ValArray<UT_MidiFile *>	 myInputFiles;
    UT_ValArray<UT_MidiFile *>   myOutputFiles;
    const int			*myBarMarkerMessage;
    void			 printSysexMessage(int *array);


};

UT_API extern UT_Midi		*UTgetMidi(); // Get the app interrupt ptr
UT_API extern void		 UTsetMidi(UT_Midi *);


#endif

