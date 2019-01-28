/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Notifier.h	    UT library (C++)
 *
 * COMMENTS:	Notifier that invokes registered functor callbacks whenever
 *		an event notification is triggered.
 *
 *
 *
 * HOW TO SET UP THE NOTIFIER AND THE CALLBACK:
 *
 *	    1)	decide what kind of event is passed from UT_NotifierImpl
 *		to the callback method. It can be an int, an enumerated
 *		type, or a class reference. 
 *		For example,
 *		    OP_Event &
 *
 *	    2)	in the observer class, write a method that handles events
 *		For example,
 *		    void     OP_SomeObserver::someHandler( OP_Event &event ) {}
 *
 *	    3)	in the observer class, declare the UT_NotifierList instance
 *		For example in .h,
 *		    OP_SomeObserver
 *		    {
 *			...
 *			UT_NotifierList	    myNotifierList;
 *			...
 *		    };
 *
 *	    4)	in the emmitter class, instantiate the UT_NotifierImpl
 *		with <OP_Event &> as the type, and provide the accessor to it
 *		For example,
 *		    OP_SomeEventEmmiter
 *		    {
 *			UT_NotifierImpl<OP_Event&> &getNotifier()
 *					{ return myNotifier; }
 *			...
 *		    private:
 *			UT_NotifierImpl<OP_Event &> myNotifier;
 *		    };
 *	    
 *	    5)	in the observer class, create a functor that contains
 *		the callback method, and register the functor with notifier
 *		For example,
 *		    void
 *		    OP_SomeObserver::registerCallback( OP_SomeEventEmmiter *
 *							    someEventEmitter ) 
 *		    {
 *			UT_Functor1<void, OP_Event &>
 *				functor( this, &OP_SomeObserver::someHandler );
 *
 *			someEventEmmiter->getEventNotifier().addObserver( 
 *						    myNotifierList, functor );
 *		    }
 *
 *	    6)	whenever an event should be emitted, call notifiyObservers()
 *		on the notifier
 *		For example,
 *		    void
 *		    OP_SomeEventEmitter::someMethod()
 *		    {
 *			...
 *			OP_Event    event( whatever, things, it, takes );
 *			myNotifier.notifyObservers( event );
 *			...
 *		    }
 * 
 */

#ifndef __UT_Notifier_h__
#define __UT_Notifier_h__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_Functor.h"
#include "UT_Pair.h"
#include <SYS/SYS_Types.h>

// declare UT_NotifierList for the use in UT_NotifierImpl
class UT_NotifierList;

// a pure virtual base class to refer to by the notifier list
class UT_API UT_Notifier
{
public:
    virtual	    ~UT_Notifier() {};
    virtual void     removeObserver( UT_NotifierList & ) = 0;
};

// define UT_NotifierImpl that handles the distribution of the notifications
template <typename EVENT_TYPE>
class UT_NotifierImpl : public UT_Notifier
{
public:
		 UT_NotifierImpl();
    virtual	~UT_NotifierImpl();

    // a definition of the callback functor
    typedef UT_Functor1< void, EVENT_TYPE > Callback;


    // ----------------------------------------------------------------------
    // adds a callback functor to this notifier
    // INPUTS:
    //   list     - a list of notifiers the caller expressed interest in
    //              (which should not contain this notifier yet)
    //   callback - a functor invoked as a notification of an event
    // OUTPUTS:
    //   list     - the list is updated to include this notifier.
    //              If the list contained this notifier already, the
    //		    callback is not registered (i.e., the old callback
    //		    prevails)
    // ----------------------------------------------------------------------
    virtual void	addObserver( UT_NotifierList &list, Callback &callback);

    // ----------------------------------------------------------------------
    // removes a callback functor from this notifier
    // INPUTS:
    //   list	  - a list of notifiers the caller expressed interest in
    //              (which should also contain this notifier)
    // OUTPUTS:
    //   list     - this notifier is removed from the list. If this notifier
    //              is not found in the list, no action is taken.
    // ----------------------------------------------------------------------
    virtual void	removeObserver( UT_NotifierList &list );

    // ----------------------------------------------------------------------
    // checks if this notifier has an observer registered
    // INPUTS:
    //   list	  - a list based on which we check the observer registration
    // RETURNS: 
    //   true if the list was used to add an observer to this notifer, and
    //        the observer is still registered.
    //   false if this notifier has no callback functor was registered
    //        with the list.
    // ----------------------------------------------------------------------
    virtual bool	hasObserver( const UT_NotifierList &list ) const;

    // ----------------------------------------------------------------------
    // invokes all regisered functors using the event as argument
    // INPUTS:
    //   event	- an event passed to the functors 
    // NB:
    //   non-const because it needs to invoke a non-const functor, and if
    //   this method is const then only const functor is returned, which in
    //   turn tries to invoke operator() for which only non-const version
    //   is available
    // ----------------------------------------------------------------------
    virtual void	notifyObservers( EVENT_TYPE event );

    /// Returns the amount of memory owned by this UT_NotifierImpl
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myObservers.getMemoryUsage(false);
        for (exint i = 0; i < myObservers.entries(); ++i)
            mem += myObservers(i).mySecond.getMemoryUsage(false);
        return mem;
    }

private:
    // ----------------------------------------------------------------------
    // finds the index to the observer list that contains the argument 
    // (i.e., the notifier list)
    // INPUTS:
    //   list	  - a list based on which we check the observer registration
    // RETURNS: 
    //   index to the observer element containing "list"
    //        with the list.
    // ----------------------------------------------------------------------
    int			 getObserverIndex( const UT_NotifierList &list ) const;

    // ----------------------------------------------------------------------
    // removes all observers
    // ----------------------------------------------------------------------
    void		 removeAllObservers();

    // ----------------------------------------------------------------------
    // iterates through the whole list of observers and removes any entries
    // that have NULL as their list member of the pair (i.e., entries that are
    // marked for deletion)
    // ----------------------------------------------------------------------
    void		 pruneObservers();

private:
    // define a type that we are storing 
    typedef UT_Pair< UT_NotifierList*, Callback> ObserverPair;

    // list of registered callbacks to invoke during the distribution
    // of notifications. The observers is an array of pairs. The pair
    // contains a list pointer based on which we recognize observer,
    // and the callback functor which is invoked as the notification.
    UT_Array< ObserverPair >	     myObservers;

    // some state flags
    bool	 myNotifyingFlag;	// currently notifying observers
    bool	 myPendingRemovalFlag;	// list contains observers to remove
};

// include the templated implementation
#if defined( WIN32 ) || defined( LINUX ) || defined( MBSD ) || defined(GAMEOS)
    #include "UT_Notifier.C"
#endif

#endif	// __UT_Notifier_h__
