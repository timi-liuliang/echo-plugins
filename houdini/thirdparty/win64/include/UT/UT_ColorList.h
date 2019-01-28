/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	(UT_ColorList.h, C++)
 *
 * COMMENTS:
 */

#ifndef _UT_ColorList_
#define _UT_ColorList_

#include "UT_API.h"
#include "UT_String.h"
#include "UT_Color.h"
#include "UT_IndexedHashMapT.h"

class UT_API UT_ColorList
{
public:
    UT_ColorList();

    // Load the given file on startup
    UT_ColorList(const char* rgbfile);

    virtual ~UT_ColorList();

    // Load colors from rgbfile (in X11 rgb.txt format)
    void	loadFile(const char* rgbfile);

    // Was there an error parsing the file?
    bool	error() const { return myIsError; };

    // Get the number or errors, and the line and description of the first
    // error in the file
    void	getError(int& nerrors, int& errline, UT_String& errmsg) const;

    // How many colors are in the list
    uint	numColors() const;

    // Get a color by name
    UT_Color	getColor(const char* colorname) const;

    // Get a color by index
    UT_Color	getColor(uint i) const;

    // Get the name of the color at index i
    UT_String	getColorName(uint i) const;

protected:
    typedef UT_IndexedHashMapStringKey	MapKey;
    typedef UT_Color			MapItem;

    bool		myIsError;
    int			myNumErrors;
    int			myErrLine;
    UT_String		myErrMsg;
    UT_IndexedHashMapT<MapKey, MapItem>	myColors;
};

#endif
