/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_MultiChannel.h (Operator library, C++)
 *
 * COMMENTS:	Derives from OP_MultiChannel to know about node owners as a
 *		complement to OP_Channels.
 *
 */

#ifndef __OP_MultiChannel_h__
#define __OP_MultiChannel_h__

#include "OP_API.h"
#include <CH/CH_MultiChannel.h>

class OP_Node;

class OP_API OP_MultiChannel : public CH_MultiChannel
{
public:
    OP_MultiChannel();
    explicit OP_MultiChannel( const char *name );

    typedef void (*OwnerCallback)( void *data, OP_Node *owner );
    void    traverseOwners( OwnerCallback callback, void *data ) const;
    void    handleNodeDeleted( OP_Node *node );
};

typedef UT_Array<OP_MultiChannel> OP_MultiChannelRefArray;

#endif // __OP_MultiChannel_h__

