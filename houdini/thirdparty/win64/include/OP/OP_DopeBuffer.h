
/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        OP_DopeBuffer.h (C++)
 *
 */

#ifndef __OP_DopeBuffer_h__
#define __OP_DopeBuffer_h__

#include "OP_API.h"
#include "OP_Channels.h"
#include "OP_MultiChannel.h"
#include <CH/CH_Types.h>

#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringArray.h>
#include <SYS/SYS_Types.h>

#include <utility>


// forward declaration
class CH_Channel;
class UT_IFStream;
class UT_OFStream;
class UT_StringHolder;


class OP_API OP_DopeBuffer
{
public:
    OP_DopeBuffer();
    virtual ~OP_DopeBuffer();

    void	clearAndDestroy();

    // frame_mask - parameter that masks which keys are copied from the rows
    //   If it is nullptr, all keys are copied. If it is not nullptr, this
    //   method copies only these keys from rows whose frame is specified
    //   in an int array (in frame_masks) corresponding to a given row.
    // PRE:
    //   This parameter should contain arrays sorted in ascending order. 
    //   All entries should have values no less than x1 and no greater than x2
    void	copyKeys(const OP_MultiChannelRefArray &rows, 
			 int x1, int y1, int x2, int y2, int forceKeys = 0,
			 const UT_Array<UT_IntArray> *row_frame_masks = nullptr,
			 bool clear_buffer = true,
			 const int row_frame_masks_offset = 0);
    void	cutKeys( const OP_MultiChannelRefArray &rows, 
			 int x1, int y1, int x2, int y2, int forceKeys = 0);
    static void	deleteKeys(const OP_MultiChannelRefArray &rows, 
			int x1, int y1, int x2, int y2);
    void 	replaceKeys(const OP_MultiChannelRefArray &rows, 
			    int x1, int y1, int x2, int y2);

    void 	pasteKeys(const OP_MultiChannelRefArray &rows, 
			  int x1, int y1 ) const;

    // An OP_DopeBuffer stores only displayed non-empty channels.
    // The paste row order mode uses displayed index to resolve the channels.
    void	updateDisplayedIndexMap( const CH_ChannelRefList &displayed );
    bool	validDisplayedIndexMap() const;
    bool	lookupDisplayedIndexMap( int id, int &row_id, int &ch_id ) const;

    bool	findFirstDisplayedRow(int &id, int &row_id, int & ch_id) const;
    bool	findLastDisplayedRow(int &id, int &row_id, int & ch_id) const;

    void	print();
    int		save();
    int 	load();

    const OP_MultiChannelRefArray & getRows() const	    { return myRows; }

    void	    getChannelList( CH_ChannelList &channels ) const;

    void	    getDimensions(int &x1, int &x2, int &numRows);
    void	    getMaxDimensions(int &x1, int &x2, int &numRows);
    void	    duplicate(OP_DopeBuffer &copy);

    // set the "window" of keyframes that we are interested in
    void	    setStartEnd(int start, int end)
		    { myStartFrame = start, myEndFrame = end; }
    int		    getStart() const	{ return myStartFrame; }
    int 	    getEnd() const	{ return myEndFrame; }

    CH_Channel *    dupChannel(const CH_Channel &from);

    void	    setTargetChannel( const CH_Channel *target ) const;

    bool	    getRowFullPath( UT_StringHolder &ret, int i ) const;

    const UT_Map<const CH_Channel*,UT_Array<fpreal> > &getPasteKeysMap() const
	{ return myPasteKeysMap; }

    // Flag to tell if the copyKeys call has been made using the current time because the
    // selection was empty. We use this to avoid reselecting the paste keys.
    void	    setCopyCurrentTime(bool b)
	{ myCopyCurrentTime=b; }
    bool	    getCopyCurrentTime()
	{ return myCopyCurrentTime; }

private:
    // load and save the copy buffer. 
    int			save(const char *filename);
    int			save(UT_OFStream &ofs);

    int			load(const char *filename);
    int			load(UT_IFStream &ifs);

    void		pasteChannelKeys( CH_Channel *from_chp,
					  CH_Channel *to_chp, int x1) const;

    OP_DopeBuffer( const OP_DopeBuffer &copy );          // not implemented
    OP_DopeBuffer &operator=(const OP_DopeBuffer &copy );// not implemented

private:
    OP_MultiChannelRefArray	myRows;
    UT_Map<const CH_Channel*,int> myOriginalMap;
    mutable UT_Map<const CH_Channel*,UT_Array<fpreal> > myPasteKeysMap;
    UT_Array< std::pair<int,int> > myDisplayedToRow;
    UT_StringArray		myRowsFullPaths;

    OP_Channels			myCollection;
    int			    	myStartFrame;
    int			    	myEndFrame;
    bool			myCopyCurrentTime;
};

#endif
