/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_PostIt.h (OP Library, C++)
 *
 * COMMENTS:	Defines post it notes.
 * 
 */

#ifndef __OP_PostIt__
#define __OP_PostIt__

#include "OP_API.h"
#include "OP_Error.h"
#include "OP_ItemId.h"
#include "OP_NetworkBoxItem.h"
#include "OP_PostItNoteFlags.h"
#include "OP_Stat.h"

#include <UT/UT_Color.h>
#include <UT/UT_ErrorManager.h>
#include <UT/UT_String.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>


class OP_Network;
class OP_SaveFlags;
class UT_IStream;
class UT_WorkBuffer;


#define OP_POSTIT_DEFAULT_W		2.5F
#define OP_POSTIT_DEFAULT_H		2.5F
#define OP_POSTIT_MINIMUM_W		0.3F
#define OP_POSTIT_MINIMUM_H		0.3F
#define OP_POSTIT_MINIMIZED_W		2.5F

class OP_API OP_PostIt : public OP_NetworkBoxItem
{
public:
    /// Create a network box with the specified name. If a duplicate name is 
    /// passed in, it will be altered to make it unique in the network.
    OP_PostIt(const UT_String &name, OP_Network *net);
    virtual ~OP_PostIt();

    virtual void	 setXY(fpreal x, fpreal y);
    virtual fpreal	 getX() const;
    virtual fpreal	 getY() const;
    virtual fpreal	 getW() const;
    virtual fpreal	 getH() const;
    fpreal		 getMaximizedW() const;
    fpreal		 getMaximizedH() const;

    bool setWWithBoundsChecks(fpreal h);
    bool setHWithBoundsChecks(fpreal h);

    const OP_PostItNoteFlags	&flags() const { return myFlags; }
    OP_PostItNoteFlags		&flags() { return myFlags; }


    /// Returns true if this box is currently picked
    virtual int			 getPicked() const;

    /// Set whether this box is picked; returns true if the picked
    /// status has changed. 
    virtual int			 setPicked(int on_off,
					bool propagate_parent_event = true);

    /// Each netbox has a unique id. This is used primarily for undos, as we
    /// also keep a list of notes in order of id, so lookup by id becomes
    /// quite quick.
    static OP_PostIt		*lookupPostItNote(int unique_id);
    int			 	 getUniqueId() const
				 { return myUniqueId; }
    virtual int64		 getItemUniqueId() const
				 { return myUniqueId; } 

    /// The name which is used for identification and in displays of this
    /// box. setName() returns true if the name change was successful, and 
    /// alters the name slightly if necessary to make it unique within this 
    /// network.
    bool		 	 setName(const UT_String &name);
    virtual const UT_String	&getItemName() const
				 { return myName; };
    virtual bool		 setItemName(const UT_String &name)
				 { return setName(name); }

    bool		 	 setText(const UT_String &text);
    virtual const UT_String     &getText() const
				 { return myText; };

    bool			 setTextSize(fpreal size);
    fpreal			 getTextSize() const
				 { return myTextSize; }

    bool			 setTextColor(const UT_Color &clr);
    const UT_Color		&getTextColor() const
				 { return myTextColor; }

    bool			 setDrawBackground(bool draw_background);
    bool			 getDrawBackground() const
				 { return flags().getDrawBackground(); }

    virtual OP_ItemType	 	 getItemType() const;
    virtual OP_Network		*getParentNetwork() const;

    /// Get and set the width and height of this box. Units are absolute
    /// as opposed to the relative units found in OPUI.
    /// If the width or height specified is smaller than that which will
    /// contain all of the items, the width or height respectively will only
    /// be adjusted to the minimum size required to contain the items. To avoid
    /// this automatic size check, set 'force' to true.
    void		 setW(fpreal w, bool force = false);
    void		 setH(fpreal h, bool force = false);

    /// Calculate the smallest bounds we're willing to accept as our size
    /// given our contents. By default, 'incl_buff_border' is set to add a small
    /// buffer border to make the layout look pretty. NOTE: If we contain no
    /// items, we return an inverted bounding box, see implementation for
    /// further details.
    void		 getItemBounds(fpreal &x1, fpreal &y1, fpreal &x2,
			               fpreal &y2, bool incl_buff_border = true)
				       const;

    /// ALWAYS call this with 'resizing' == true before you do any resizing 
    /// operations (e.g. setW()), and call it after you're done resizing with 
    /// 'resizing' == false.  You should only do this once for each group of 
    /// resizing operations on the same netbox. 
    ///
    /// This method: 
    /// 1) ensures we get a proper undo block around all the resize ops, and 
    /// 2) allows you to use setXY() to stretch the box to the left (otherwise,
    ///    setXY() just moves the box)
    void		 setResizing(bool resizing);
    bool		 getResizing();

    /// Same as setXY(), but here we can force the box to take on these X/Y 
    /// coordinates, even if our contents don't fit us any more.  The
    /// propagate_parent_event tells this method whether or not it should send
    /// and event after moving the network box -- this should normally be set
    /// to true.
    void		 setXY(fpreal x, fpreal y,
				bool force,
				bool propagate_parent_event);

    virtual OP_ERROR	 save(std::ostream &os,
				const OP_SaveFlags &sflags,
				const char *path_prefix);
    virtual OP_ERROR	 saveIntrinsic(std::ostream &os,
				const OP_SaveFlags &sflags);

    /// Load the contents of the stream into the attributes of this post-it 
    /// note; if binary is nonzero, load in binary mode.
    /// Loading doesn't send the OP_POSTIT_NOTE_CREATED message to the network;
    /// the caller is responsible for doing that.
    virtual bool  	 load(UT_IStream &is,
				const char *ext="",
				const char *path=0);
     
    void		 getFullPath(UT_String &str) const;
    void		 getFullPath(UT_WorkBuffer &str) const;

    /// Get whether this box is currently minimized
    bool		 getMinimized() const;

    /// Set whether this box is currently minimized
    /// If update_position is true, then the box is moved to accomodate the
    /// minimized status.  If propagate_parent_event is true, an event is sent
    /// to the network about our minimized state.
    bool		 setMinimized(bool isminimized,
				      bool update_position,
				      bool propagate_parent_event);

    /// Used by opscript, this outputs the sequence of hscript commands
    /// necessary to recreate this sticky note.
    int			 saveCommand(std::ostream &os) const;

    /// Returns the amount of memory owned by this OP_PostIt
    int64		 getMemoryUsage(bool inclusive) const;

    /// Control the default color used on new post it notes.
    static const UT_Color	&defaultColor();
    static void			 setDefaultColor(const UT_Color *color);
    static const UT_Color	&defaultTextColor();
    static void			 setDefaultTextColor(const UT_Color *color);

private:
    /// This should only be called by our loading mechanism while we load, right
    /// after we get created!
    void		 setUniqueId(int id);

    OP_ERROR		 saveIntrinsicError();
    OP_ERROR		 saveError();
    OP_ERROR		 error() const
			 { return UTgetErrorSeverity(); }

    bool		 loadPostItFlags(UT_IStream &is, const char *path);
    bool		 loadNetworkBox(UT_IStream &is, const char *path);
    bool		 loadIntrinsic(UT_IStream &is, const char *path);
    virtual bool	 loadPacket(UT_IStream &is, short class_id,
				short signature, const char *path = 0);
    virtual bool	 loadPacket(UT_IStream &is, const char *token,
				const char *path=0);

    OP_ERROR		 saveNetworkBox(std::ostream &os, 
            		                const OP_SaveFlags &sflags, 
            		                const char *path_prefix);

    friend class OP_Network;

private:
    // Note that although netboxes share their namespace with nodes, they do not
    // share their "ID space".
    int		 	 myUniqueId;
    OP_Network		*myNetwork;

    UT_String	 	 myName; 
    UT_String	 	 myText; 
    fpreal		 myTextSize;
    UT_Color		 myTextColor;

    /// Keeps info like timestamps for last modification time, creation time,
    /// etc
    OP_Stat	 	 myStats;

    /// ALWAYS set these through accessors to ensure proper undo creation
    fpreal 	 	 myX, myY;
    fpreal	 	 myW, myH; 

    OP_PostItNoteFlags	 myFlags;
};

#endif
