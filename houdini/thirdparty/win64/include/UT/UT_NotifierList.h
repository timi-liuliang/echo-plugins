/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NotifierList.h ( UT Library, C++)
 *
 * COMMENTS:	An list that keeps track of the notifiers. It is
 *		used by classes that register callbacks with notifiers.
 *		Its main function is to unregister any callbacks with
 *		all the notifiers that still would invoke them at the
 *		destruction time of the notifier list. 
 *
 *		Notifier list should be a member variable (not a reference
 *		or a pointer to a list) of the class that owns
 *		the methods registered with the notifiers. This way the
 *		destructor of the UT_NotifierList will be invoked when
 *		the class with callbacks is destroyed, and the notifiers
 *		will unregister the callbacks through the UT_NotifierList
 *		destructor.
 *
 *
 * HOW TO SET UP THE NOTIFIER AND THE CALLBACK:
 *
 *		see UT_Notifier.h for the instructions
 */

#ifndef __UT_NotifierList_h__
#define __UT_NotifierList_h__

#include "UT_API.h"
#include "UT_ValArray.h"

class UT_Notifier;

class UT_API UT_NotifierList
{
public:
    // constructor
		 UT_NotifierList();
    virtual	~UT_NotifierList();

    // ----------------------------------------------------------------------
    // Removes all notifiers from the private list of notifiers.
    // Also asks each of these notifiers to remove the observer 
    // associated with this list from the notifier's own list of observers.
    // ----------------------------------------------------------------------
    virtual void	 removeAllNotifiers();


protected:

    // NB: only UT_NotifierImpl should call addNotifier() and removeNotifier()
    // when it registers and unregisters callbacks. We thus make these
    // two methods protected and make the UT_NotifierImpl the friend 
    // of this class (UT_NotifierList). But, UT_NotifierImpl should not
    // use any private members, especially the private list itself.
    template <typename T> friend class UT_NotifierImpl;

    // adds the notifier to private list of notifiers
    virtual void         addNotifier(    UT_Notifier &notifier );

    // removes the notifier from private list of notifiers
    virtual void	 removeNotifier( UT_Notifier &notifier );

private:

    // a list of notifiers in which we expressed an interst.
    // This list is kept around so that when we are destructed, we know
    // from which notifiers we need to remove interest. Otherwise,
    // notifiers may use nonexistent observers leading to core dump.
    UT_ValArray< UT_Notifier * >	myNotifiers;
};

#endif	//__UT_NotifierList_h__
