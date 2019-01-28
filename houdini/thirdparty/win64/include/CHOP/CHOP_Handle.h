/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP_Handle.h ( CHOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CHOP_Handle__
#define __CHOP_Handle__

#include "CHOP_API.h"
#include "CHOP_Node.h"
#include "CHOP_Types.h"
#include <CL/CL_Defines.h>
#include <UT/UT_Color.h>

class CHOP_Handle;
class CHOP_Node;

enum CHOP_HandleLook {
    
    HANDLE_GUIDE = 0,
    HANDLE_BOX,
    HANDLE_SQUARE
};

enum CHOP_HandleMotion {
    
    HANDLE_HORIZONTAL = 0,
    HANDLE_VERTICAL,
    HANDLE_PLANE
};

#define	HANDLE_LABEL_LEFT	0x01
#define	HANDLE_LABEL_RIGHT	0

#define	HANDLE_LABEL_TOP	0x02
#define	HANDLE_LABEL_BOTTOM	0

#define HANDLE_BAR		0x04
#define HANDLE_GRAPH		0

#define HANDLE_WIDTH_END	0x08

#define DEFAULT_FLAGS (HANDLE_LABEL_LEFT | \
		       HANDLE_LABEL_BOTTOM | HANDLE_GRAPH)

class
CHOP_HandleData
{
public:

    int		shift;		// select ourselves as well
    fpreal	xoffset;	// x position in samples
    fpreal	yoffset;	// y position
    CL_Unit	unit;		// return value should be in this unit
};

class CHOP_API CHOP_Handle {

public:

     CHOP_Handle(CHOP_Node *dad,
	     	 const char *name, 
		 int id, 
	         fpreal xoffset,
	         fpreal yoffset = 0,
		 CHOP_HandleLook look = HANDLE_BOX,
		 CHOP_HandleMotion motion = HANDLE_HORIZONTAL,
		 int type_flag = DEFAULT_FLAGS
		 );



     CHOP_Handle(void);
    ~CHOP_Handle();
     unsigned		 operator== (const CHOP_Handle &v) const;
    CHOP_Handle		&operator= (const CHOP_Handle &v);

    void		 setColorOffset(int offset)
			 { myColorOffset = offset; }

    UT_Color		 getChopColor(const CL_Clip *clip=nullptr ) const
    			 { return myParent->getChopColor(myColorOffset,clip); }

    CHOP_Node		*getCHOP()		{ return myParent; }
    const char		*getName() const	{ return myName; }
    unsigned int	 getId() const		{ return myId; }
    fpreal		 getXoffset() const	{ return myXoffset; }
    fpreal		 getYoffset() const	{ return myYoffset; }
    CHOP_HandleLook	 getLook() const	{ return myLook; }
    CHOP_HandleMotion	 getMotion() const	{ return myMotion; }

    int			 isLabelLeft() const
    			 { return (myFlags & HANDLE_LABEL_LEFT) ? 1:0; }

    int			 isLabelTop() const
    			 { return (myFlags & HANDLE_LABEL_TOP) ? 1:0; }

    int			 isBar() const
    			 { return (myFlags & HANDLE_BAR) ? 1:0; }

    int			 isWidthEnd() const
    			 { return (myFlags & HANDLE_WIDTH_END) ? 1:0; }

    void		 setSubParent(int idx)
			 { mySubParentIdx = idx; }

    int			 getSubParent(void) const
			 { return mySubParentIdx; }

    int			 isSelected() const
			 { return mySelected; }

    void		 setSelected(int yesno)
			 { mySelected = yesno; }

    int64		 getMemoryUsage(bool inclusive) const;

private:

    CHOP_Node		*myParent;
    int			 mySubParentIdx;
    char 		*myName;
    unsigned int	 myId;

    fpreal		 myXoffset;	// measured in indices
    fpreal		 myYoffset;	// measured in indices
    CHOP_HandleLook	 myLook;
    CHOP_HandleMotion	 myMotion;
    int			 myColorOffset;

    unsigned int	 myFlags;
    char		 mySelected;
			 
};

#endif

