/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_ScriptOperator.h (OBJ Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __OBJ_ScriptOperator_h__
#define __OBJ_ScriptOperator_h__

#include "OBJ_API.h"
#include <PI/PI_ScriptOperator.h>
#include "OBJ_Node.h"

class UT_IStream;

typedef PRM_Template *(*OBJ_ScriptTemplateFunc)(OBJ_ParmsStyle style);

class OBJ_API OBJ_ScriptOperator : public PI_ScriptOperator
{
public:
    OBJ_ScriptOperator(const OP_OTLDefinition &definition,
		       const char *defaulticon,
		       OP_Constructor construct,
		       const char *child_table_name,
		       int controlsswitcherindex,
		       int controlsswitcherpagenum,
		       OBJ_ScriptTemplateFunc gettemplatefunc);
    virtual ~OBJ_ScriptOperator();

    virtual PRM_Template	*loadParmTemplates(UT_IStream &is,
					    const char *filename,
					    UT_String &scriptname,
					    PRM_ScriptImports *&imports);

    bool			 getHideDefaultParms() const
				    { return myHideDefaultParms; }

private:
    OBJ_ScriptTemplateFunc	 myGetTemplateFunc;
    int				 myControlsSwitcherIndex;
    int				 myControlsSwitcherPageNum;
    bool			 myHideDefaultParms;
};

#endif
