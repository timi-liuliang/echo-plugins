/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IO table definition
 *
 * COMMENTS:	This class can be used to initialize an IO table for
 *		use in other libraries.  For example, to use this class
 *		you can have a static instansiation giving the desired
 *		table name:
 *			static UT_IOTable	fbIOtable("FBio", 3)
 *
 */

#ifndef __UT_IOTable_h__
#define __UT_IOTable_h__

#include "UT_API.h"
#include "UT_LinkList.h"
#include "UT_ValArray.h"
#include "UT_String.h"

// Instantiating a static UT_IOTable requires that the static data in
// <tools/henv.C> has already been initialized.  Our current mechanism
// for ensuring this is to include the header.
#include <tools/henv.h>

class UT_API UT_IOEntry : public UT_LinkNode
{
public:
    UT_IOEntry(const char *ext, const char *magic, const char *read,
		const char *write);
   ~UT_IOEntry();

    UT_String	extension;	// File extension
    unsigned	magic;		// Magic number
    UT_String	readFrom;	// Command to read from an external file type
    UT_String	writeTo;	// Command to write to the external file type

    //
    //  In cases where there are more than three arguments on each
    //	line of the table, the following structure is filled up with
    //	duplicates of each argument
    //
    char	**argv;
};

class UT_API UT_IOTable : public UT_LinkList
{
public:
	     UT_IOTable(const char *path, char commentChar='#');
    virtual ~UT_IOTable();
    
    const UT_IOEntry	*findExtension(const UT_String &filename) const;
    const UT_IOEntry	*findMagic(unsigned magic) const;
};

class UT_API UT_ExtensionList
{
public:
    UT_ExtensionList(const char *filepath, const char *iopath=0);
    int		findExtension(const char *extension) const;
    int		hasSuffix(const char *filename) const;
    /// Add given extension to the list, if it doesn't already exist.
    void	addExtension(const char *extension);
    int		getExtensions(UT_ValArray<const char *> &list) const;
    int		readFile(const char *filepath);
    int		readIOFile(const char *filepath);

private:
    char	**array;
    int		  entries;
};

UT_API extern UT_ExtensionList	*UTgetPicExtensions(void);
UT_API extern UT_ExtensionList	*UTgetRampExtensions(void);

UT_API extern UT_ExtensionList	*UTgetGeoExtensions(void);
UT_API extern UT_ExtensionList	*UTgetCaptExtensions(void);
UT_API extern UT_IOTable	*UTgetIOTable(const char *filename);

UT_API extern UT_ExtensionList	*UTgetChanExtensions(void);
UT_API extern UT_IOTable	*UTgetChanTable(const char *filename);

UT_API extern UT_ExtensionList	*UTgetLutExtensions(void);
UT_API extern UT_ExtensionList	*UTgetCmdExtensions(void);
UT_API extern UT_ExtensionList	*UTgetMidiExtensions(void);
UT_API extern UT_ExtensionList	*UTgetI3dExtensions(void);
UT_API extern UT_ExtensionList	*UTgetSimExtensions(void);
UT_API extern UT_ExtensionList	*UTgetSimdataExtensions(void);

#endif
