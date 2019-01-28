/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_CmdLineReader.h
 *
 * COMMENTS:
 *	This class acts as a replacement to UT_Workbuffer::getline(cin).
 *	It supports command-line editing with arrow keys and it keeps
 *	track of a command-line history accessible with the up and down
 *	arrow keys.
 */

#ifndef __UT_CmdLineReader_h__
#define __UT_CmdLineReader_h__

#include "UT_API.h"
#include "UT_WorkBuffer.h"
#include "UT_StringArray.h"

typedef void (*UT_TabCallback)(UT_WorkBuffer &, int &);

class UT_API UT_CmdLineReader
{
public:
    // If a function is supplied to the constructor, it will be called when
    // tab is pressed.  The function is passed the current contents of the
    // line and the current cursor position.  It should change both these
    // values.
    UT_CmdLineReader(
        UT_TabCallback tab_callback=NULL,
        UT_TabCallback double_tab_callback=NULL);
    ~UT_CmdLineReader();

    // readLine() will display an editable command line and place the result
    // in a work buffer when the user presses enter.  If stdin is not a tty,
    // it will also behave properly.  If the line could not be read because
    // there is no more input available, false will be returned.
    bool readLine(UT_WorkBuffer &line);

    // readPartOfLine() is similar to readLine(), except it will only read the
    // next group of characters that are available in the input.  If you want
    // to continuously poll for other events while waiting for input (like
    // hscript does), you should check for data on stdin and call this method
    // when data is ready.  false will be returned if stdin is no longer open.
    // done_line will be set to true if an entire line was read, and only then
    // will the work buffer be set.  Note that if multiple lines are
    // available in the input, they will all be read in, the first line
    // will be placed in "line", done_line will be true, and hasUnhandledInput()
    // will return true.  Subsequent calls will first read from the unhandled
    // input before going to the read input.
    bool readPartOfLine(UT_WorkBuffer &line, bool &done_line);

    // Was there input as a result of the last read that hasn't been
    // handled yet?
    bool hasUnhandledInput() const { return myUnhandledInput.length() != 0; }

    // readLine() will disable line buffering when it is called.  However,
    // you may want to disable line buffering for the duration of your
    // application so that you can check if data is available on stdin.
    // (Otherwise, you would only know about data on stdin when a complete
    // line is entered.)
    void disableLineBuffering();
    void reenableLineBuffering();
    bool isLineBufferingEnabled() const;

    static UT_CmdLineReader *getInstance();

private:
    void construct();

    bool readAvailableInput(UT_WorkBuffer &buffer);
    bool handleNextBufferedKey(bool &done_line);

    void insertChar(char ch);
    void insertString(const char *str);
    void removeCharBefore();
    void removeCharAt();
    void removeWordBefore();

    void updateDisplay(const UT_WorkBuffer &old_line, int old_cursor_pos,
		       const UT_WorkBuffer &new_line, int new_cursor_pos);
    int tabLengthAtPosition(int tab_start_pos);
    int *buildCharIndexToLengthArray(const UT_WorkBuffer &line);
    void printExpandedChar(
		char chr, int index, int *char_lengths, char replacement='\0');

    void moveUpThroughHistory();
    void moveDownThroughHistory();


    // Data:
    bool		 myAllowCmdLineEditing;

    // The TerminalData opaque class avoids having to include termios.h
    // in the header, which pollutes the global namespace.
    class TerminalData;
    TerminalData	*myTerminalData;

    bool		 myLineBufferingIsDisabled;
    bool		 myLineBufferingWasDisabled;
    bool		 myIsReadingLine;
    UT_WorkBuffer	 myLine;
    UT_WorkBuffer	 myUnhandledInput;
    int			 myCursorPos;
    UT_StringArray	 myHistory;
    int			 myHistoryIndex;
    UT_TabCallback	 myTabCallback;
    UT_TabCallback	 myDoubleTabCallback;
    int			 myTabLength;
    bool                 myJustReadTab;

    UT_String		 myEscapeKey;
    UT_String		 myCtrlAKey;
    UT_String		 myCtrlDKey;
    UT_String		 myCtrlEKey;
    UT_String		 myCtrlWKey;
    UT_String		 myLeftArrowKey;
    UT_String		 myRightArrowKey;
    UT_String		 myUpArrowKey;
    UT_String		 myDownArrowKey;
    UT_String		 myBackspaceKey;
    UT_String		 myDeleteKey;
    UT_String		 myHomeKey;
    UT_String		 myEndKey;

    static UT_CmdLineReader *theInstance;
};

#endif
