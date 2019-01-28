/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:        OP_IndexConverter.h
*
* COMMENTS:
*
*/

#ifndef __OP_IndexConverter_h__
#define __OP_IndexConverter_h__

#include "OP_API.h"
#include <UT/UT_String.h>

class OP_ConnectorId;
class OP_Node;

class OP_API OP_IndexConverter
{
public:
    ~OP_IndexConverter() { }

    static int		 nameToIndex(const OP_ConnectorId &id,
				OP_Node *node);    
    static void		 inputIndexToName(int index,
				OP_Node *node,
				OP_ConnectorId &id_out,
				bool create_if_needed = false);

    static int		 outputNameToIndex(const OP_ConnectorId &id,
				OP_Node *node);    
    static void		 outputIndexToName(int index,
				OP_Node *node,
				OP_ConnectorId &id_out);

    static void		 convertOutputIdToOutputName(
				const OP_ConnectorId &output_id,
				OP_Node *node,
				UT_String &output_name_out);
    static void		 convertOutputNameToOutputId(const char *output_name,
				OP_Node *node,
				OP_ConnectorId &id_out);
    static void		 convertInputNameToInputId(const char *input_name,
				OP_Node *node,
				OP_ConnectorId &id_out);

private:
    // Does not construct.
    OP_IndexConverter() { }
};

#endif
