/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Transformation frame with correction.
 * 
 */

#ifndef __GP_CorrectionFrame_h__
#define __GP_CorrectionFrame_h__

#include "GP_API.h"
#include "GP_Xform.h"
#include "GP_Frame.h"

class GP_FrameHierarchy;


class GP_API GP_CorrectionFrame : public GP_Frame
{
    friend class GP_FrameHierarchy;

public:
    // Class c-tors and d-tor.
		 GP_CorrectionFrame(void);
		 GP_CorrectionFrame(const GP_XformHandle &handle);
		 GP_CorrectionFrame(const GP_Frame &frame,
				    const GP_XformHandle &handle, int history);
		 GP_CorrectionFrame(const GP_CorrectionFrame &frame);
    virtual	~GP_CorrectionFrame(void);

    // Deep copy from source frame. Assume the frame is a correction frame.
    virtual void copyFrom(const GP_Frame &src);

    // Change the transformation and reset the correction xform to identity:
    virtual void changeXform(const GP_XformHandle &handle);

    // Query the history flag:
    int		 hasHistory(void) const	{ return myHistoryFlag; }

    // Query or set the correction transformation:
    const GP_XformHandle	&correction(void) const	{ return myCorrection; }


protected:
    // Set the history flag:
    void	 eraseHistory (void)	{ myHistoryFlag = 0;    }
    void	 createHistory(void)	{ myHistoryFlag = 1;    }

    // Set the correction transformation:
    void	 correction(const GP_XformHandle &xf)	{ myCorrection = xf; }


private:
    GP_XformHandle	myCorrection;	// computed upon insertion in hierarchy
    int			myHistoryFlag;	// 1 if affected by parent xformation
};

#endif
