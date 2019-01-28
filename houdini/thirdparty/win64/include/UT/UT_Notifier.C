/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Notifier.C	    UT library (C++)
 *
 * COMMENTS:	Notifier that invokes registered functor callbacks whenever
 *		an event notification is triggered.
 *
 */

#ifndef __UT_Notifier_C__
#define __UT_Notifier_C__

#include "UT_Notifier.h"
#include "UT_NotifierList.h"
#include "UT_Assert.h"

template <typename EVENT_TYPE>
UT_NotifierImpl<EVENT_TYPE>::UT_NotifierImpl()
	    :	myNotifyingFlag( false ),
		myPendingRemovalFlag( false )
{
}

template <typename EVENT_TYPE>
UT_NotifierImpl<EVENT_TYPE>::~UT_NotifierImpl()
{
    removeAllObservers();
}


template <typename EVENT_TYPE> 
int
UT_NotifierImpl<EVENT_TYPE>::getObserverIndex( const UT_NotifierList &list)const
{
    int		i;

    for( i = 0; i < myObservers.entries(); i ++  )
	if( myObservers(i).myFirst == &list ) //pointer comparison
	    return i;

    return -1;
}


template <typename EVENT_TYPE> 
bool
UT_NotifierImpl<EVENT_TYPE>::hasObserver( const UT_NotifierList &list ) const
{
    return ( 0 <= getObserverIndex( list ));
}


template <typename EVENT_TYPE> 
void
UT_NotifierImpl<EVENT_TYPE>::addObserver( UT_NotifierList &list, 
					  Callback &callback)
{
    bool	already_registered;

    // check if we already contain callback associated with the list
    already_registered = hasObserver( list );
    UT_ASSERT( ! already_registered );

    // if not registered yet 
    if( ! already_registered )
    {
	ObserverPair	pair( &list, callback );

	// add to our own list and add ourseleves to the UT_NotifierList
	myObservers.append( pair );
	list.addNotifier( *this );
    }
}


template <typename EVENT_TYPE> 
void
UT_NotifierImpl<EVENT_TYPE>::removeObserver( UT_NotifierList &list )
{
    int		i;

    // find an observer
    i = getObserverIndex( list );
    if( 0 <= i )
    {
	// remove the observer form our list and remove ourseleves form the list
	list.removeNotifier( *this );
	if( myNotifyingFlag )
	{
	    myObservers(i).myFirst = NULL;
	    myPendingRemovalFlag   = true;
	}
	else
	{
	    myObservers.removeIndex( i );
	}
    }
    else
    {
	UT_ASSERT( ! "no observer to remove" );
    }
}


template <typename EVENT_TYPE>
void
UT_NotifierImpl<EVENT_TYPE>::removeAllObservers()
{
    int		i;

    // descending order so that we do not shift any memory in the array
    for( i = myObservers.entries(); i -- > 0; )
    {
	if( myObservers(i).myFirst )
	    myObservers(i).myFirst->removeNotifier( *this );
	myObservers.removeIndex( i );
    }
}

template <typename EVENT_TYPE>
void
UT_NotifierImpl<EVENT_TYPE>::pruneObservers()
{
    int		i;

    // descending order so that we do not shift any memory in the array
    for( i = myObservers.entries(); i -- > 0; )
	if( myObservers(i).myFirst == NULL )
	    myObservers.removeIndex( i );

    // flag that anything that was to be removed has been removed
    myPendingRemovalFlag = false;
}

template <typename EVENT_TYPE> 
void
UT_NotifierImpl<EVENT_TYPE>::notifyObservers( EVENT_TYPE event )
{
    int		i, n;
    bool	old_notifying;

    // flag that we are in the notification loop
    old_notifying   = myNotifyingFlag;
    myNotifyingFlag = true;

    // go through the list and invoke the functor with the event as an argument
    // NB: we obtain n prior to the iteration so that if any new observer is
    //     added during the loop, they are not notified right away.
    n = myObservers.entries();
    for( i = 0; i < n; i++ )
	if( myObservers(i).myFirst )
	    myObservers(i).mySecond( event );

    // restore flag (NB this allows recursive notifications)
    myNotifyingFlag = old_notifying;

    // if finished notifying, and any observer was removed, then prune the list
    if( !myNotifyingFlag && myPendingRemovalFlag )
	pruneObservers();
}

#endif // __UT_Notifier_C__
