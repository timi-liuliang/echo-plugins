/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_UndoGeometrySelection.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_UndoGeometrySelection__
#define __SOP_UndoGeometrySelection__

#include "SOP_API.h"

#include <GU/GU_SelectionSet.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_StringStream.h>
#include <UT/UT_Undo.h>

class SOP_API SOP_UndoGeometrySelection : public UT_Undo
{
public:
    SOP_UndoGeometrySelection(GU_SelectionSetHandle selection_set);
    virtual ~SOP_UndoGeometrySelection();
    
    // UT_Undo overrides
    void undo() override;
    void redo() override;
    
private:
    void swap();
    
    bool saveSelectionToStream(std::ostream &os);
    bool loadSelectionFromStream(std::istream &is);
    
    GU_SelectionSetHandle mySelectionSet;
    UT_UniquePtr<UT_StringStream> myUndoStream;
};

#endif // __SOP_UndoGeometrySelection__
