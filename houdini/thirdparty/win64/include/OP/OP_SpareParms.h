/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_SpareParms.h ( OP Library, C++)
 *
 * COMMENTS:	Cache of spare parameter dialog scripts.  Each cache entry has:
 * 		- The PRM_ScriptPage
 * 		- The PRM_ScriptImports
 * 		- The PRM_Template list
 *
 *  This class is kept in a per-operator cache, allowing multiple nodes to
 *  share templates etc. if they have the same spare parameters.
 */

#ifndef __OP_SpareParms__
#define __OP_SpareParms__

#include "OP_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_Array.h>
#include <PRM/PRM_Default.h>

class UT_IStream;
class OP_SpareParmCache;
class OP_Operator;
class PRM_ScriptPage;
class PRM_ScriptImports;
class PRM_Template;

class OP_API OP_SpareParms
{
public:
			 OP_SpareParms(OP_SpareParmCache *cache,
				       UT_IStream &is,
				       OP_Operator *op,
				       const char *key);

    bool		 isValid() const	{ return myPage != 0; }
    int			 bumpRef(int dir);
    PRM_ScriptPage	*getScriptPage()	{ return myPage; }
    PRM_Template	*getSpareTemplates()	{ return mySpareTemplates; }
    PRM_ScriptImports	*getSpareImports()	{ return 0; }
    PRM_Template	*getLayoutTemplates()	{ return myLayoutTemplates; }
    PRM_ScriptImports	*getLayoutImports()	{ return myLayoutImports; }
    const char		*getKey()		{ return myKey; }

    // The Id for this class is a unique integer value >= 0 that allows us
    // to safely and uniquely identify an instance of this class without
    // worrying about pointer reuse.
    int			 getId() const		{ return myId; }

    // We track the number of expression inputs present to avoid expensive
    // searches in the usual case of nodes lacking any.
    int			 numSpareInputs() const 	{ return myNumSpareInputs; }

private:
    // These should only be deleted through bumpRef calls.
			~OP_SpareParms();

    // Turn our parsed script into spare and layout template arrays. We use
    // the operator to supply the base parameter information.
    void		 createTemplates(OP_Operator *op);
    // Use the base templates to fix up the layout templates, and possibly
    // fill in a spare templates array. Works for top level or multiparm
    // templates. Returns true if the layout templates and base templates
    // are exactly the same.
    bool		 createTemplates(PRM_Template *basetplate,
					 PRM_Template *laytplate,
					 PRM_Template *sparetplate,
					 bool doingmultiparm);
    // Clears the key value, for when we stash this spare parms.
    void		 clearKey();

    OP_SpareParmCache	*myCache;
    PRM_ScriptPage	*myPage;
    PRM_ScriptImports	*myLayoutImports;
    PRM_Template	*mySpareTemplates;
    PRM_Template	*myLayoutTemplates;
    PRM_Default		 myStandardFolderDefaults[2];
    PRM_Default		 myEmptyFolderDefaults[2];
    UT_String		 myKey;
    int			 myRefCount;
    int			 myId;
    int			 myNumSpareInputs;

    friend class	 OP_SpareParmCache;
};

class OP_API OP_SpareParmCache
{
public:
			 OP_SpareParmCache();

    OP_SpareParms	*createSpareParms(UT_IStream &is, OP_Operator *op);

    // Called only by bumpRef()
    void		 removeSpareParms(OP_SpareParms *parms);

    // Increment or decrement the ref count on this cache.
    int			 bumpRef(int dir);

    // Move all our spare parms from myCache into myStash.
    void		 stashSpareParms();

    // Recreate the parm templates for all OP_SpareParms in this cache.
    void		 recreateStashedSpareParms(OP_Operator *op);

private:
    // These should only be deleted through bumpRef calls.
			~OP_SpareParmCache();

    UT_StringMap<OP_SpareParms *>	 myCache;
    UT_Array<OP_SpareParms *>	 	 myStash;
    int					 myRefCount;
};

#endif

