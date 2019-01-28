/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OR_Mapping.h ( OR Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OR_Mapping__
#define __OR_Mapping__

#include "OP_API.h"
#include <UT/UT_ValArray.h>


#define OR_MAP_AUTONAME		0
#define OR_MAP_MANUAL		1

#define OR_MAP_DEFAULT	OR_MAP_AUTONAME

class orMapElement;
class OP_Node;
class opParmData;
class UT_IStream;
class UT_OStream;

class OP_API OR_Mapping
{

public:

     OR_Mapping();
    ~OR_Mapping();

    void	setMapping(int idx, int type, const char *label = 0);
    int		getMapping(int idx, const char *&label) const;

    int		getNumMappings() const
		{ return myElements.entries(); }

    int			 findManualParms(int index, OP_Node *src);
    const opParmData	*getManualParm(int index, int i);

    void	buildOpDependencies( OP_Node *src );
    void	changeOpRef(const char *new_fullpath, const char *old_fullpath,
			    const char *old_cwd);

    bool	save(UT_OStream &os);
    bool	load(UT_IStream &is);

    int64	getMemoryUsage(bool inclusive) const;

private:

    void	reset();


    UT_ValArray<orMapElement *>	myElements;

};

#endif

