/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_RenderItem.h ( ROP Library, C++)
 *
 * COMMENTS:
 *    Defines a simple structure to describe what and how to render a ROP.
 */
#ifndef ROP_RenderItem_h
#define ROP_RenderItem_h

#include "ROP_API.h"
#include "ROP_Verbose.h"

#include <UT/UT_Array.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_IntrusivePtr.h>


class ROP_Node;

class ROP_RenderItem;
typedef UT_IntrusivePtr<ROP_RenderItem> ROP_RenderItemHandle;



class ROP_API ROP_RenderItem : public UT_IntrusiveRefCounter<ROP_RenderItem>
{
public:
    explicit ROP_RenderItem(ROP_Node *node);
    explicit ROP_RenderItem(const ROP_RenderItem &item);
    ~ROP_RenderItem();

    // methods to set the frame range
    void	 setSingleFrame(fpreal frame)
		 { mySingleFrame = true; myEndFrame = myStartFrame = frame; }
    
    void	 setFrameRange(fpreal start, fpreal end, fpreal finc);

    // Initially used.
    void	 setUsed(bool used) { myUsedFlag = used; }

    // calling will render the ROP attached to this object.
    bool	 render(ROP_Verbose *verbose=0, int count=0, int total=0) const;

    // returns true if 'item' would be covered by this ROP, making it a
    // duplicate entry.
    bool	 containedBy(const ROP_RenderItemHandle &item);

    // removes any overlapping frames. This should be called after contains(),
    // which will remove any total overlaps.
    void	 removeOverlap(const ROP_RenderItemHandle &item);

    bool	 mergeWith(const ROP_RenderItemHandle &item);

    // dump the render information to os.
    void	 print(std::ostream &os, bool full_path, bool condense_range) const;

    // returns the number of frames that would be rendered by this item
    int		 getNumFrames() const;

    // accessors
    ROP_Node	*getNode() const	{ return myNode; }
    bool	 isSingleFrame() const	{ return mySingleFrame; }
    fpreal	 getFrame() const	{ return myStartFrame; }
    fpreal	 getStartFrame() const	{ return myStartFrame; }
    fpreal	 getEndFrame() const	{ return myEndFrame; }
    fpreal	 getFrameInc() const    { return myFrameInc; }
    bool	 getUsed() const	{ return myUsedFlag; }

    // a regular sequence is something that can be represented by
    // start/end/finc. However, in removeOverlap(), removing overlapping
    // frames may create a non-regular sequence, in which case, we need to
    // render the frames individually.
    bool	 isRegularSequence() const { return myRegularSequence; }
    const UT_FprealArray &getFrameList() const { return myFrames; }


    // Resets the render ids so that they start back at the beginning.  Be
    // careful where you call this from.
    static void	  resetRenderIds() { ourRenderIdCount = 0; }

    int		  getRenderId() const { return myRenderId; }

    // Adds an item as a dependency, which means that this item must render
    // before we do.
    void	  addDependency(const ROP_RenderItemHandle &item) const;

    // Returns true if this item has a dependency on 'item'.
    bool	  hasDependency(const ROP_RenderItemHandle &item) const;

    // clear all dependencies this item has.
    void	  clearDependencies();

    // Take the dependencies of item and add them to our list
    void	  mergeDependencies(const ROP_RenderItemHandle &item);

    // Removes all dependencies that have getUsed() == false
    void	  removeUnusedDependencies();
    
    // Goes through our dependents and make them depend on with_item instead
    // of this item.
    void	  replaceDependencies(const ROP_RenderItemHandle &with_item);

    UT_Array<ROP_RenderItemHandle> &getDependencies() 
					{ return myDependencies; }

    const UT_Array<ROP_RenderItemHandle> &getDependents() const
					{ return myDependents; }

    // Returns true if this item depends on some other render item.
    bool	  hasDependencies() const
					{ return myDependencies.entries() > 0; }

    // Returns true if some other render item depends on this item.
    bool	  hasDependents() const
					{ return myDependents.entries() > 0; }

    // only to be used by ROP_RenderList to reindex dependendies for easier
    // reading.
    void	  setRenderId(int id) { myRenderId = id; }
    
private:
    ROP_Node	*myNode;
    bool	 mySingleFrame;
    bool	 myRegularSequence;
    fpreal	 myStartFrame;
    fpreal	 myEndFrame;
    fpreal	 myFrameInc;
    UT_FprealArray myFrames;
    bool	 myUsedFlag;

    int		 myRenderId;

    // This is the list of direct items that have to be rendered before this 
    // item.  This list is usually very small.
    mutable UT_Array<ROP_RenderItemHandle> myDependencies;

    // This is the list of direct items that have to be rendered after this
    // item.  This list is usually very small.
    mutable UT_Array<ROP_RenderItemHandle> myDependents;


    static int   ourRenderIdCount;
};

#endif
