/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        BM_ResourceManager.h ( Direct Manipulation Library, C++)
 *
 * COMMENTS:
 *	This class maintains a table of all states, handles, op-handle links,
 *	and input selectors.  All state, handle, and op-handle link objects
 *	are created and deleted through a BM_ResourceManager.  The active one
 *	is obtained via the global function BMgetResourceManager().
 */

#ifndef __BM_ResourceManager_H__
#define __BM_ResourceManager_H__

#include "BM_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_SymbolTable.h>
#include <PI/PI_ResourceManager.h>

class BM_View;
class BM_State;
class BM_Handle;
class BM_OpHandleLink;
class BM_SceneManager;
class BM_InputSelector;

typedef BM_State	*(*BM_StateConstructor)(BM_View &, PI_StateTemplate &,
						BM_SceneManager *);
typedef BM_Handle	*(*BM_HandleConstructor)(BM_SceneManager &,
						 PI_HandleTemplate &);
typedef BM_OpHandleLink	*(*BM_PIConstructor)(BM_SceneManager &,
					     PI_PITemplate &);
typedef BM_InputSelector*(*BM_SelectorConstructor)(BM_View &,
						   PI_SelectorTemplate &);

class BM_API BM_ResourceManager : public PI_ResourceManager
{
public:
    BM_State		*newState(BM_View &view,
				  PI_StateTemplate &templ,
				  BM_SceneManager *scene = 0) const;
    virtual 
    BM_State		*newState(BM_View &view,
				  const char *name, const char *type,
				  BM_SceneManager *scene = 0) const;
    BM_State		*newState(BM_View &view,
				  PI_NetworkType nettype, int state_idx,
				  BM_SceneManager *scene = 0) const;

    BM_Handle		*newHandle(BM_SceneManager &sm,
				   const char *name) const;
    BM_OpHandleLink	*newOpHandleLink(BM_SceneManager &sm,
					 const char *name, 
					 const char *description) const;
    BM_InputSelector	*newSelector(BM_View &view,
				     const char *name) const;

    void		 deleteState(BM_State *state);
    void		 deleteHandle(BM_Handle *handle);
    void		 deleteOpHandleLink(BM_OpHandleLink *link);
    void		 deleteSelector(BM_InputSelector *selector);

    virtual void	 findVolatileKeys(const char *volatile_hotkey_name,
					  UT_Array<int> &volatile_keys,
					  UT_Array<int> &keys_modifiers) 
    					  const;
    virtual void	 updateViewports(int id, const char *opname,
					 bool open_only = false) const;
    virtual void	 removePISettingsFromTables(OP_Operator &op_type);

    static void		 startParmsCache();
    static void		 stopParmsCache();

    static void	 	 lookupVolatileKeys(const char *volatile_hotkey_name,
				            UT_Array<int> &volatile_keys,
				            UT_Array<int> &keys_modifiers); 
protected:
	     BM_ResourceManager();
    virtual ~BM_ResourceManager();
};

BM_API extern BM_ResourceManager *BMgetResourceManager();

extern "C" {
    SYS_VISIBILITY_EXPORT extern void newModelState(BM_ResourceManager *m);
    SYS_VISIBILITY_EXPORT extern void newHandle(BM_ResourceManager *m);
    SYS_VISIBILITY_EXPORT extern void newOpHandleLink(BM_ResourceManager *m);
    SYS_VISIBILITY_EXPORT extern void newOpHandleBinding(BM_ResourceManager *m);
    SYS_VISIBILITY_EXPORT extern void newSelector(BM_ResourceManager *m);
};

#endif
