/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_FileIterator.h (TIL library, C++)
 *
 * COMMENTS:
 *	A quick iterator class for writing numbered files. Will prompt the
 *	user if an existing file with a matching name is present.
 */
#ifndef TIL_FILE_ITERATOR_H
#define TIL_FILE_ITERATOR_H

#include "TIL_API.h"
#include <UT/UT_String.h>

enum  {
    TIL_SAVE_OK,
    TIL_OVERWRITE,
    TIL_SAVE_ABORT,
    TIL_FILE_SKIP
};

class TIL_API TIL_FileIteratorUI
{
public:
    virtual ~TIL_FileIteratorUI();
    virtual	bool errorAbort(const char *file,bool &continue_always,
				bool multiple) = 0;
    
    virtual	int overwriteConfirm(const char *file,bool &continue_always,
				     bool multiple) = 0;
    virtual	void showMessage(const char *message) = 0;
};

class TIL_API TIL_FileIterator
{
public:
     TIL_FileIterator(const char *filepat);
    ~TIL_FileIterator();
    
    void	init(int fstart, int fend, int step = 1);

    const char *iterate();
    int		getFrame() const { return myCurrentFrame -myStep; }
    bool	isLastFrame() const { return myCurrentFrame > myEnd; }
    float	getFractionDone() const;
    
    static	bool errorAbort(const char *file,
				bool &continue_always,
				bool multiple);
    
    static	int overwriteConfirm(const char *file,
				     bool &continue_always,
				     bool multiple);
    static	int overwrite(const char *file,
			      bool &continue_always,
			      bool multiple);

    static	void showMessage(const char *message);

    static	void setUI(TIL_FileIteratorUI *ui);
    
private:
    UT_String	myFilePat;
    UT_String	myPrefix;
    UT_String	mySuffix;
    UT_String	myCurrentFile;
    int		myPad;
    int		myStart, myEnd, myStep;
    int		myCurrentFrame;
    int		myCurrentIter;
    bool	myMultipleFiles;
    bool	myMultipleFilesUseIter;
};

#endif 
