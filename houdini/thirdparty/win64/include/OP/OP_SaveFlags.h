/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 */

#ifndef __OP_SaveFlags_h__
#define __OP_SaveFlags_h__

#include "OP_API.h"

class OP_API OP_SaveFlags
{
public:
    OP_SaveFlags();

    bool	getBinary() const
		{ return myBinary; }
    void	setBinary(bool save)
		{ myBinary = save; }
    bool	getIntrinsics() const
		{ return myIntrinsics; }
    void	setIntrinsics(bool save)
		{ myIntrinsics = save; }
    bool	getParameters() const
		{ return myParameters; }
    void	setParameters(bool save)
		{ myParameters = save; }
    bool	getChannels() const
		{ return myChannels; }
    void	setChannels(bool save)
		{ myChannels = save; }
    bool	getCompact() const
		{ return myCompact; }
    void	setCompact(bool save)
		{ myCompact = save; }
    bool	getSelected() const
		{ return mySelected; }
    void	setSelected(bool save)
		{ mySelected = save; }
    bool	getValues() const
		{ return myValues; }
    void	setValues(bool save)
		{ myValues = save; }
    bool	getData() const
		{ return myData; }
    void	setData(bool save)
		{ myData = save; }
    bool	getGroups() const
		{ return myGroups; }
    void	setGroups(bool save)
		{ myGroups = save; }
    bool	getIndirects() const
		{ return myIndirects; }
    void	setIndirects(bool save)
		{ myIndirects = save; }
    bool	getItemIds() const
		{ return myItemIds; }
    void	setItemIds(bool save)
		{ myItemIds = save; }
    bool	getChannelAliases() const
		{ return myChannelAliases; }
    void	setChannelAliases(bool save)
		{ myChannelAliases = save; }
    bool	getNetworkBoxes() const
		{ return myNetworkBoxes; }
    void	setNetworkBoxes(bool save)
		{ myNetworkBoxes = save; }
    bool	getPostIts() const
		{ return myPostIts; }
    void	setPostIts(bool save)
		{ myPostIts = save; }
    bool	getDots() const
		{ return myDots; }
    void	setDots(bool save)
		{ myDots = save; }
    bool	getNetworkBoxMembership() const
		{ return myNetworkBoxMembership; }
    void	setNetworkBoxMembership(bool save)
		{ myNetworkBoxMembership = save; }
    bool	getSpareParms() const
		{ return mySpareParms; }
    void	setSpareParms(bool save)
		{ mySpareParms = save; }
    bool	getMaterialIcon() const
		{ return myMaterialIcon; }
    void	setMaterialIcon(bool save)
		{ myMaterialIcon = save; }

    bool	getWriteTrailer() const
		{ return myWriteTrailer; }
    void	setWriteTrailer(bool save) const
		{ myWriteTrailer = save; }

    bool	getCompiledBasic() const
		{ return myCompiledBasic; }
    void	setCompiledBasic(bool save)
		{ myCompiledBasic = save; }
    bool	getCompiledVopnets() const
		{ return myCompiledVopnets; }
    void	setCompiledVopnets(bool save)
		{ myCompiledVopnets = save; }
    bool	getCompiledChannels() const
		{ return myCompiledChannels; }
    void	setCompiledChannels(bool save)
		{ myCompiledChannels = save; }
    bool	getCompiledNodeNames() const
		{ return myCompiledNodeNames; }
    void	setCompiledNodeNames(bool save)
		{ myCompiledNodeNames = save; }

private:
    // Save binary format
    bool		myBinary;
    // Save Op intrinsics (i.e. name/type)
    bool		myIntrinsics;
    // Save parameters to node
    bool		myParameters;
    // Save channels for node
    bool		myChannels;
    // Save compactly (i.e. no thumbnail)
    bool		myCompact;
    // Save only selected channels/nodes
    bool		mySelected;
    // Save values only
    bool		myValues;
    // Save OP cooked data (if flagged)
    bool		myData;
    // Save OP groups
    bool		myGroups;
    // Save Indirect inputs
    bool		myIndirects;
    // Save unique ids of network box items
    bool		myItemIds;
    // Save channel aliases for node
    bool		myChannelAliases;
    // Save network boxes (if im a network)
    bool		myNetworkBoxes;
    // Save post-it notes (if in a network)
    bool		myPostIts;
    // Save dots (if in a network)
    bool		myDots;
    // Remember which netbox i'm in.  Generally should be on for the 
    // top-level node for undos, but off for copy ops (we don't know in
    // which network we might get pasted)
    bool		myNetworkBoxMembership;
    // spare parm definitions
    bool		mySpareParms;
    // material icon
    bool		myMaterialIcon;
    // Save a compiled hip file
    bool		myCompiledBasic;
    // Save compiled Vop Networks
    bool		myCompiledVopnets;
    // Save compiled channels
    bool		myCompiledChannels;
    // Save compiled node names
    bool		myCompiledNodeNames;
    // Write the CPIO trailer
    mutable bool	myWriteTrailer;
};

#endif
