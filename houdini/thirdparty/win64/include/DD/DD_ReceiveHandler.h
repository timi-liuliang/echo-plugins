/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: DD_ReceiveHandler.h	
 *
 * COMMENTS:
 *
 * 	This class may be used to control/override drag'n'drop functionality 
 *	on UI gadgets.
 */

#ifndef __DD_ReceiveHandler__
#define __DD_ReceiveHandler__

#include "DD_API.h"
#include <UT/UT_ValArray.h>
#include "DD_Source.h"
#include "DD_ChoiceList.h"

class DD_API DD_ReceiveHandler
{
public:
			 DD_ReceiveHandler();
    virtual		~DD_ReceiveHandler();

    virtual int		acceptDragDrop(DD_Source &src, const char *label) = 0;
    virtual int		testDragDrop(DD_Source &src) = 0;
    virtual void	getDragDropChoice(DD_Source &src, DD_ChoiceList &c) = 0;
};

#endif
