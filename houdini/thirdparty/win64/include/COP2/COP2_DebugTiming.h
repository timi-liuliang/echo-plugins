/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_DebugTiming.h
 *
 * COMMENTS:
 *	This class allows for timing of specific nodes within a series of 
 *	cooks.
 */
#ifndef COP2_DebugTiming_H
#define COP2_DebugTiming_H

#include <iostream>
#include <UT/UT_Lock.h>
#include <UT/UT_String.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_StopWatch.h>
#include <UT/UT_ThreadSpecificValue.h>
#include <SYS/SYS_Math.h>
#include <OP/OP_Node.h>

#include "COP2_API.h"

class COP2_DebugTimingTable;

class COP2_API COP2_DebugTimingNode
{
public:
    COP2_DebugTimingNode(const char *nodepath)
	: myNodePath(nodepath, 1),
	  myNumCooks(0),
	  myTotalCookTime(0.0),
	  myMinCookTime(0.0),
	  myMaxCookTime(0.0)
	{ }

    void	addCookTime(double time)
    {
	if(myNumCooks == 0)
	{
	    myMinCookTime = time;
	    myMaxCookTime = time;
	}
	else
	{
	    myMinCookTime = SYSmin(time, myMinCookTime);
	    myMaxCookTime = SYSmax(time, myMaxCookTime);
	}
	myTotalCookTime += time;
	myNumCooks++;
    }

    void	print(std::ostream &os) const
    {
	if(myNumCooks > 0.0)
	    os << myNodePath << "\n  " << myNumCooks << "   "
	       << myTotalCookTime <<"s   min " << myMinCookTime << "s   max "
	       << myMaxCookTime << "s   avg "
	       << (myTotalCookTime / myNumCooks) << "s";
	else
	    os << myNodePath << " not cooked.";
    }

    UT_String myNodePath;
    int	      myNumCooks;
    double    myTotalCookTime;
    double    myMinCookTime;
    double    myMaxCookTime;
};

struct cop2_timing_entry_data
{
    std::ostream *os;
    double total_time;
};


inline int printEntry(COP2_DebugTimingNode *&node, const char *, void *vdata)
{
    cop2_timing_entry_data *pdata = (cop2_timing_entry_data *) vdata;
    
    node->print(*pdata->os);
    *pdata->os << "\n";
    pdata->total_time += node->myTotalCookTime;
    return 1;
}

inline int clearEntry(COP2_DebugTimingNode *&node, const char *, void *)
{
    delete node;
    return 1;
}
				  

class COP2_API COP2_DebugTimingTable
{
public:
    COP2_DebugTimingTable() : myTotal(0.0) {}
    ~COP2_DebugTimingTable()
    {
	myTable.traverse(clearEntry, NULL);
    }

    void	addCopTiming(OP_Node *copnode, double time)
    {
	UT_String path;
	COP2_DebugTimingNode *tnode = NULL;

	copnode->getFullPath(path);
	
	myTableLock.lock();

	if(!myTable.findSymbol(path, &tnode))
	{
	    tnode = new COP2_DebugTimingNode(path);
	    myTable.addSymbol(path, tnode);
	}	    

	tnode->addCookTime(time);

	myTableLock.unlock();
    }

    void	printTable(std::ostream &os)
    {
	cop2_timing_entry_data pdata;
	pdata.os = &os;
	pdata.total_time = 0.0;
	myTable.traverse(printEntry, &pdata);
	os.flush();
	os << "Total time: " << pdata.total_time << "s\n";
	os.flush();
    }

    UT_SymbolMap<COP2_DebugTimingNode *> myTable;
    UT_Lock		myTableLock;
    double		myTotal;
};


struct cop2_Watches
{
    void start(const char *name)
    {
	myWatches[name].resume();
    }

    void stop(const char *name)
    {
	// Don't create a stopwatch if none exists.
	if (myWatches.contains(name))
	    myWatches[name].stop();
    }

    UT_SymbolMap<UT_StopWatch>	myWatches;
};

class cop2_ThreadWatches
{
    typedef UT_ThreadSpecificValue<cop2_Watches>	ThreadWatches;
public:
    cop2_Watches &get() { return myWatches.get(); }

    void printTimes(const char *indent = "")
    {
	UT_SymbolMap<fpreal>	sum;

	using std::cerr;
	for (ThreadWatches::const_iterator it = myWatches.begin();
	    it != myWatches.end(); ++it)
	{
	    const cop2_Watches	&wl = it.get();

	    if (wl.myWatches.empty())
		continue;

	    cerr << indent << "Thread " << it.thread() << ":\n";

	    for (auto itW = wl.myWatches.begin(); !itW.atEnd(); ++itW)
	    {
		cerr << indent << "  " << itW.name() << " : " << itW.thing().getTime() << "s\n";
		if (!sum.contains(itW.name()))
		    sum[itW.name()] = itW.thing().getTime();
		else
		    sum[itW.name()] += itW.thing().getTime();
	    }
	}

	cerr << indent << "Thread Total:\n";
	for (auto itW = sum.begin(); !itW.atEnd(); ++itW)
	{
	    cerr << indent << "  " << itW.name() << " : " << itW.thing() << "s\n";
	}
    }

    void clear()
    {
	for (ThreadWatches::iterator it = myWatches.begin();
	    it != myWatches.end(); ++it)
	{
	    it.get().myWatches.clear();
	}
    }

private:    
    ThreadWatches myWatches;
};

class cop2_ScopedWatch
{
public:
    cop2_ScopedWatch(cop2_ThreadWatches &w, const char *name) :
	myWatches(w), myName(name)
    {
	myWatches.get().start(myName);
    }

    ~cop2_ScopedWatch()
    {
	myWatches.get().stop(myName);
    }

private:
    cop2_ThreadWatches &myWatches;
    const char *myName;
};

#endif
