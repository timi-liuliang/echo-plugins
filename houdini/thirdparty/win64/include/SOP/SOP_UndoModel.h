/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: SOP_UndoModel.h ( SOP Library, C++)
 *
 * COMMENTS:	class to handle undos for hard and soft modelling
 * 		changes
 */

#ifndef __SOP_UndoModel_h__
#define __SOP_UndoModel_h__

#include "SOP_API.h"
#include <UT/UT_Undo.h>
#include <UT/UT_String.h>

class SOP_SoftLockData;
class SOP_Node;
class SOP_SoftLockData;
class GU_Detail;

class SOP_API SOP_UndoModel : public UT_Undo
{
public:
	     SOP_UndoModel();
    virtual ~SOP_UndoModel();

    void		init(unsigned char prev_lock,
			     unsigned char new_lock,
			     SOP_Node *sop, 
			     SOP_SoftLockData *prev_slockdata = 0, 
			     SOP_SoftLockData *new_slockdata = 0);

    virtual  void	undo();
    virtual  void	redo();

    void		 setName(const char *name) { myName.harden(name); }
    const char		*getName() { return (const char *)myName; }

protected:
    typedef enum 
    { 
	SOPUNDOMODEL_UNDO,
	SOPUNDOMODEL_REDO
    } SOP_UndoModelOperation;

    int			 hasSopUndo() { return myHasSopUndo; }

    void		 doUndo(void *modeler);
    void		 doRedo(void *modeler);

private:
    void 		 loadForHardLock(SOP_UndoModelOperation operation,
					 void *modeler);
    void		 loadForSoftLock(SOP_UndoModelOperation operation,
					 void *modeler);
    void 		       unlockSop(SOP_UndoModelOperation operation,
					 void *modeler);

    // verb helper method
    void 		 createSopSlockData( SOP_SoftLockData &loaded_diff, 
					     SOP_Node *sop) const;

    int			 myOpId;

    UT_String		 myOldHardLockFileName;
    UT_String		 myNewHardLockFileName;
    UT_String		 mySoftLockFileName;

    unsigned char	 myPrevLock;
    unsigned char	 myNewLock;

    int			 myHasSopUndo;

    UT_String		 myName;
};
#endif
