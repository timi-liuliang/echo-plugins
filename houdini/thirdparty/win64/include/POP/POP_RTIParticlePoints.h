/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	POP_RTIParticlePoints.h ( POP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __POP_RTIParticlePoints__
#define __POP_RTIParticlePoints__

#include "POP_API.h"
#include <GU/GU_Detail.h>
#include <GA/GA_RangeTypeInterface.h>
#include <GA/GA_OffsetList.h>
#include "POP_ParticleList.h"
#include <GEO/GEO_PrimPart.h>
#include <UT/UT_IntArray.h>

/// Class to iterate over points in particle systems
class POP_API POP_RTIParticlePoints : public GA_RangeTypeInterface
{
public:
    POP_RTIParticlePoints(const GU_Detail &geo,
		    const POP_ParticleList &plist);

    POP_RTIParticlePoints(const GU_Detail &geo,
		    const UT_IntArray &pagestarts,
		    const GA_OffsetList &offsetlist,
		    exint start, exint end);
    virtual ~POP_RTIParticlePoints();

    /// @{
    /// RangeTypeInterface API
    virtual GA_RangeTypeInterface	*copy() const
			{
			    return new POP_RTIParticlePoints(myGeo,
						myPageStarts,
						myOffsetList,
						myStart, myEnd);
			}
    virtual bool	isSplittable() const { return true; }
    virtual bool	isEmpty() const	 { return myEnd - myStart == 0; }
    virtual bool	split(GA_RangeTypeInterface *list[2]) const;
    virtual GA_Size	getPageCount() const
			    { return myPageStarts.entries(); }
    virtual bool	getPageElementRange(GA_Range &range,
				GA_Size start, GA_Size npages) const;
    virtual GA_Offset	getFirstOffsetInPage(GA_Size page) const;

    virtual bool	isEqual(const GA_RangeTypeInterface &src) const
			{
			    const POP_RTIParticlePoints	*psrc;
			    psrc = dynamic_cast<const POP_RTIParticlePoints *>(&src);
			    if (!psrc)
				return false;
			    return &myGeo == &(psrc->myGeo) &&
				    myOffsetList.isEqual(psrc->myOffsetList, myStart, myEnd) &&
				    myStart == psrc->myStart &&
				    myEnd == psrc->myEnd;
			}
    virtual GA_Size	getMaxEntries() const
			    { return myEnd - myStart; }
    virtual GA_Size	getEntries() const
			    { return myEnd - myStart; }
    /// @}

    class IState
    {
	public:
	    IState(const GA_OffsetList &offsets, int start, int end)
	    {
		myOffsets = offsets;
		myStart = start;
		myEnd = end;
		myIdx = start;
	    }
	    IState(const IState &src)
		: myOffsets(src.myOffsets)
		, myStart(src.myStart)
		, myEnd(src.myEnd)
		, myIdx(src.myIdx)
	    { }
	    ~IState() {}

	    void	rewind()
			{
			    myIdx = myStart;
			}
	    bool	atEnd() const
			{
			    return myIdx == myEnd;
			}

	    void	extractRange(GA_Offset &s, GA_Offset &e)
			{
			    if (atEnd() || myEnd <= myStart)
			    {
				s = GA_INVALID_OFFSET;	// Invalid range
				e = GA_INVALID_OFFSET;
			    }
			    else
			    {
				s = myOffsets.get(myIdx);
				e = GA_Offset(s + 1);
				for(;;)
				{
				    myIdx++;
				    if(myIdx >= myEnd)
				    {
					break;
				    }
				    if (e != myOffsets.get(myIdx))
					break;
				    ++e;
				}
			    }
			}

	private:
	    GA_OffsetList	 myOffsets;
	    exint		 myStart, myEnd;
	    exint		 myIdx;
    };


    /// @{
    /// Iterator state information
    virtual void	iterateCreate(GA_IteratorState &state) const
			{
			    // Copy over my particle list
			    state.myPointer = new IState(myOffsetList, myStart, myEnd);
			}
    virtual void	iterateDestroy(GA_IteratorState &state) const
			{
			    // Copy over my particle list
			    delete static_cast<IState *>(state.myPointer);
			}
    virtual void	iterateCopy(GA_IteratorState &dest,
			    const GA_IteratorState &src) const
			{
			    const IState	*state;
			    state = static_cast<IState *>(src.myPointer);
			    dest.myPointer = new IState(*state);
			}
    virtual void	iterateRewind(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const
			{
			    IState	*it;
			    it = static_cast<IState *>(state.myPointer);
			    it->rewind();
			    it->extractRange(start, end);
			}
    virtual void	iterateNext(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const
			{
			    IState	*it;
			    it = static_cast<IState *>(state.myPointer);
			    it->extractRange(start, end);
			}
    /// @}

private:
    exint		getPageStart(exint page) const
			{
			    return myPageStarts(page);
			}
    exint		getPageEnd(exint page) const
			{
			    return page < myPageStarts.entries()-1
				    ? myPageStarts(page+1)
				    : myOffsetList.entries();
			}
    const GU_Detail		&myGeo;
    GA_OffsetList		 myOffsetList;
    UT_IntArray			 myPageStarts;
    // [start, end)
    exint			 myStart, myEnd;
};

#endif

