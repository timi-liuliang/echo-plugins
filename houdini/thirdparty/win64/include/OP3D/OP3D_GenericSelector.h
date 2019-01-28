/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        OP3D_GenericSelector.h
 *
 * COMMENTS:
 * 	This selector is designed to work with any op type.  The desired
 *	selection type, parameter fields, and other information that varies
 *	from op to op is passed in the template.
 */

#ifndef __OP3D_GenericSelector_h__
#define __OP3D_GenericSelector_h__

#include "OP3D_API.h"
#include <GU/GU_SelectType.h>
#include <PI/PI_ClassManager.h>
#include "OP3D_InputSelector.h"

OP3D_API extern PI_SelectionEntry	OP3DthePrimSelOnly[];
OP3D_API extern PI_SelectionEntry	OP3DthePrimSelTypes[];
OP3D_API extern PI_SelectionEntry	OP3DthePointSelTypes[];
OP3D_API extern PI_SelectionEntry	OP3DtheEdgeSelTypes[];
OP3D_API extern PI_SelectionEntry	OP3DtheBreakpointSelTypes[];
OP3D_API extern PI_SelectionEntry	OP3DthePrimEdgeSelTypes[];
OP3D_API extern PI_SelectionEntry	OP3DthePrimPointSelTypes[];
OP3D_API extern PI_SelectionEntry	OP3DtheUVSelTypes[];
OP3D_API extern PI_SelectionEntry	OP3DtheAllSelTypes[];

class OP3D_API OP3D_GenericSelector : public OP3D_InputSelector
{
public:
    OP3D_GenericSelector(OP3D_View &view,
			 PI_SelectorTemplate &templ);
    virtual ~OP3D_GenericSelector();

    static BM_InputSelector	*ourConstructor(BM_View &viewer,
				    PI_SelectorTemplate &templ);

    virtual const char		*className() const override;

protected:
    virtual void updateGroupMenuValue(PI_GeometryType geo_type, 
                                      int &group_menu_type) override;
    PI_GeometryType		 groupMenuValueToGeometryType(
					int group_menu_type) const;
    
private:
    SELECTOR_SETUP_TABLE()

    // Return the list of valid selection types for this selector.
    const PI_SelectionEntry	*selectionTypes() const
	{ return (PI_SelectionEntry *)getTemplate().data(); }
};

#endif
