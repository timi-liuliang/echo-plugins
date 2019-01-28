/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ChoiceList.h (Parameter Library)
 *
 * COMMENTS:
 *	Used to supply choices of values for parameters.
 *	In a UI environment, these will be used to create menus.
 *	You can use a PRM_Item list instead of a PRM_Name list if
 *	you want to include icons in the menu.
 *
 * 	It is important to be careful when using integer string tokens
 * 	in ordinal menus.  Strings that fail to match any token will be
 * 	processed as a string representation of an integer, which could
 * 	result in unexpected behaviour when attempting to load hip files
 * 	saved with obsolete menu tokens.  However, when the menu has a 
 * 	PRM_CHOICELIST_USE_TOKEN flag, it uses menu item token values, and 
 * 	it will always be processed as a string representation rather than 
 * 	an item index, thus making the behaviour uniform and expected.
 */

#ifndef __PRM_ChoiceList__
#define __PRM_ChoiceList__

#include "PRM_API.h"
#include <UT/UT_String.h>
#include <CH/CH_ExprLanguage.h>
#include "PRM_ParmOwner.h"

class PRM_Name;
class PRM_Item;
class PRM_Parm;
class PRM_ParmOwner;
class PRM_SpareData;

enum PRM_ChoiceListType
{
    PRM_CHOICELIST_REPLACE	= 0x0001,	// replaces value
    PRM_CHOICELIST_TOGGLE	= 0x0002,	// basically a bitfield (for
						// strings and ordinals only)
    PRM_CHOICELIST_APPEND	= 0x0004,	// appends to string
    PRM_CHOICELIST_WILD		= 0x0008,	// handles * intelligently
    PRM_CHOICELIST_USE_TOKEN	= 0x0010,	// token (not index) is value

    PRM_CHOICELIST_EXCLUSIVE	= 0x1000,	// user can only use choices

    // define some commonly used types
    PRM_CHOICELIST_SINGLE	= PRM_CHOICELIST_REPLACE
				  |PRM_CHOICELIST_EXCLUSIVE
};


/// This is a callback function that is used to generate dynamic items for 
/// PRM_ChoiceList.
///
/// @param thedata		Pointer to the node it was called on
/// @param thechoicenames[out]  Array of PRM_Name objects to be filled
/// @param thelistsize		Number of items in @c thechoicenames
/// @param thespareptr		PRM_SpareData object for the parameter this
///				was invoked from.
/// @param theparm		PRM_Parm object that this was invoked from
typedef void (* PRM_ChoiceGenFunc)(
		    void *thedata,
		    PRM_Name *thechoicenames,
		    int thelistsize,
		    const PRM_SpareData *thespareptr,
		    const PRM_Parm *theparm);
typedef void (* PRM_ChoiceItemGenFunc)(
		    void *thedata,
		    PRM_Item *thechoiceitems,
		    int thelistsize,
		    const PRM_SpareData *thespareptr,
		    const PRM_Parm *theparm);

class PRM_API PRM_ChoiceList
{
public:
    // Names
    PRM_ChoiceList(PRM_ChoiceListType thetype, PRM_Name *thechoicenames);
    PRM_ChoiceList(PRM_ChoiceListType thetype, PRM_ChoiceGenFunc thefunc);
    
    // Items
    PRM_ChoiceList(PRM_ChoiceListType thetype, PRM_Item *thechoiceitem);
    PRM_ChoiceList(PRM_ChoiceListType thetype, PRM_ChoiceItemGenFunc thefunc);

    // Script interface.
    PRM_ChoiceList(PRM_ChoiceListType thetype, const char *thescript,
		   CH_ScriptLanguage language = CH_HSCRIPT);

    
    bool		 usesItems() const { return myUseItems; }

    PRM_Name		*choiceNamesPtr()
			    { return myUseItems
					? (PRM_Name*)0 : myChoiceNames; }
    PRM_Item		*choiceItemsPtr()
			    { return myUseItems
					? myChoiceItems : (PRM_Item*)0; }

    void		 getChoiceNames(const PRM_Name *&thechoicenames,
					void *thedata = 0,
					const PRM_SpareData *theSparePtr = 0,
					const PRM_Parm *theParm = 0) const;

    void		 getChoiceItems(const PRM_Item *&thechoiceitems,
					void *thedata = 0,
					const PRM_SpareData *theSparePtr = 0,
					const PRM_Parm *theParm = 0) const;

    bool		 tokenFromIndex(
				UT_String &result,
				exint index,
				void *data = nullptr,
				const PRM_SpareData *spare = nullptr,
				const PRM_Parm *parm = nullptr) const;

    const UT_String	&getScript() const;
    CH_ScriptLanguage	 getScriptLanguage() const;
    bool		 isDynamic() const;

    PRM_ChoiceListType	 getType() const
			 { return myType; }
    PRM_ChoiceGenFunc	 getChoiceGenerator() const
			 { return myChoiceGenerator; }
    int			 getSize(const PRM_Parm *parm) const
			 {
			     // myStaticChoiceSize is used for static number of
			     // items, regardless of myUseItems.
			     if (myChoiceNames)
				 return myStaticChoiceSize;
			     else
				 return getSizeSlow(parm);
			 }

    /// NOTE: Based on the lack of destructor, PRM_ChoiceList does NOT
    ///       own *myChoiceNames or *myChoiceItems, but ownership semantics
    ///       are bonkers in these mysterious lands of PRM.
    int64		 getMemoryUsage(bool inclusive) const
    {
        return (inclusive ? sizeof(*this) : 0)
            + myScript.getMemoryUsage(false);
    }

private:
    void		 getChoiceNamesFromScript(
			    UT_Array<PRM_Name> &names,
			    const PRM_Parm *parm) const;
    void		 getChoiceNamesFromHscriptScript(
			    UT_Array<PRM_Name> &names,
			    const PRM_Parm &parm) const;
    void		 getChoiceNamesFromPythonScript(
			    UT_Array<PRM_Name> &names,
			    const PRM_Parm &parm) const;
    int			 getSizeSlow(const PRM_Parm *parm) const;

    PRM_ChoiceListType	 myType;
    
    bool		 myUseItems; // names or items
    
    int			 myStaticChoiceSize;

    union
    {
	PRM_Name		*myChoiceNames;
	PRM_Item		*myChoiceItems;
    };

    union
    {
	PRM_ChoiceGenFunc	 myChoiceGenerator;
	PRM_ChoiceItemGenFunc	 myChoiceItemGenerator;
    };
    
    UT_String		 myScript;
    CH_ScriptLanguage	 myScriptLanguage;
};

#endif

