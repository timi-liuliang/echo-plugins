/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Clip.h ( Clip Library, C++)
 *
 * COMMENTS:
 *		A collection of raw data tracks
 */

#ifndef __CL_Clip__
#define __CL_Clip__

#include "CL_API.h"
#include "CL_Track.h"

#include <UT/UT_IntArray.h>
#include <UT/UT_NameManager.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_String.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_XformOrder.h>

#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

#undef Bool

class UT_IStream;
class UT_OStream;
class UT_InfoTree;
class UT_WorkBuffer;
class CL_ClipPrivateIO;
class UT_VariantMap;

typedef void (*CL_InfoCallback)(UT_String &result, void *data, int idx);
typedef UT_VariantMap CL_ClipAttributes;

class CL_API CL_Clip {

public:

     CL_Clip(int size);
     CL_Clip(const CL_Clip &from);
    ~CL_Clip();

    CL_Clip	&operator= (const CL_Clip &v);

    // a very efficient copy constructor which checks existing
    // data to minimize memory calls

    void	 copyContents(const CL_Clip &v, int data, int slerps);


    // these methods when used with caution can increase efficiency
    // when adding tracks

    int		 	getCheckDuplicateNames() const
		 	{ return myCheckDuplicateNames; }

    void	 	setCheckDuplicateNames(int val)
		 	{ myCheckDuplicateNames = val; }

    //
    // access/query methods
    //

    void		 clearAndDestroy();

    void		 setStart(fpreal start)
			 { myStart = start; }

    fpreal		 getStart(void) const
			 { return myStart; }

    fpreal		 getEnd(void) const
			 { return myStart + myTrackLength - 1; }

    int			 getTrackLength(void) const
			 { return myTrackLength; }

    void		 setTrackLength(int length, int clear_extra=0);

    void		 setSampleRate(fpreal rate)
			 { mySampleRate = SYSmax(rate, 1.0e-4); }

    fpreal		 getSampleRate(void) const
			 { return mySampleRate; }

    int			 getTrackNames(UT_ValArray<const char *> &list) const;
    int			 getTracks(CL_TrackListC &list, 
	    			   const char *pattern = 0) const;
    int			 getTracks(UT_IntArray &list, const char *pat) const;


    int			 isTimeDependent() const;

    //
    // quaternion (slerp) methods
    // each track can belong to at most 1 slerp triplet
    //

    int			 numSlerps() const;

    int			 copySlerpAttributes(const CL_Clip &v,
	    				     int by_name = 0);

    int			 resetSlerps();

    int			 findSlerp(const CL_Track *track, int &idx) const;

    int			 getSlerp(int slerp_idx,
				  int &x_idx,
				  int &y_idx,
				  int &z_idx) const;

    int			 getSlerp(const CL_Track *track,
	    			  int &x_idx,
				  int &y_idx,
				  int &z_idx) const;

    void		 setSlerpCapacity(int capacity)
			 {
			     mySXTracks.setCapacity(capacity);
			     mySYTracks.setCapacity(capacity);
			     mySZTracks.setCapacity(capacity);
			 }

    int			 setSlerp(int x_idx, int y_idx, int z_idx);

    int			 slerpBlend(int slerp_idx,
			            const fpreal *xvalues,
				    const fpreal *yvalues,
				    const fpreal *zvalues,
				    fpreal start, int len, fpreal base, 
				    const CL_Track *effect_track,
				    const UT_XformOrder &order);

    int			 slerpBlend(int slerp_idx,
			            const fpreal *xvalues,
				    const fpreal *yvalues,
				    const fpreal *zvalues,
				    fpreal start, int len, fpreal base,
				    const CL_SubRange *effect,
				    const UT_XformOrder &order);

    int			 validateSlerps();

    void		 	setSlerpOrder(UT_XformOrder::xyzOrder o)
				{ mySRotOrder = o; } 

    UT_XformOrder::xyzOrder	getSlerpOrder() const
			        { return mySRotOrder; }
    
    //
    // track access/creation
    // returns index if name was added, else already exists and returns -1
    //

    void		 removeTrack(int idx);

    void		 setTrackCapacity(int capacity)
			 { myTracks.setCapacity(capacity); }

    CL_Track		*addTrack(const UT_StringHolder &name);
    CL_Track		*dupTrackInfo(const CL_Track *,int data_too = 0,
	    			      const UT_StringHolder &name
					= UT_StringHolder::theEmptyString);
    
    int			 getNumTracks(void) const
			 { return myTracks.entries(); }

    CL_Track		*getTrack(int index)
		 	 { return (index >= 0 && index < myTracks.entries()) ?
			      myTracks(index) : 0; }

    const CL_Track	*getTrack(int index) const
		 	 { return (index >= 0 && index < myTracks.entries()) ?
			      myTracks(index) : 0; }

    CL_Track		*getTrack(const UT_StringRef &name, int *index = 0);
    const CL_Track	*getTrack(const UT_StringRef &name, int *index = 0) const;


    //
    // track evaluation
    //


    // These three evaluat methods should only be used when appropriate,
    // it is much more efficient to use the range evaluations

    fpreal	 evaluateSingle(const CL_Track *track, fpreal index) const
		 { return track->evaluateSingle(index - myStart); }

    fpreal	 evaluateSingleTime(const CL_Track *track, fpreal t) const
		 { return evaluateSingle(track, getIndex(t)); }

    fpreal	 evaluateSingleFrame(const CL_Track *track, 
				     fpreal f, fpreal fps) const
		 { return evaluateSingle(track, getIndexF(f, fps)); }



    void	 evaluate(const CL_Track *track, 
	    		  fpreal start_index, fpreal stop_index,
	                  fpreal *data, int size) const;

    int		 evaluateSlerp(int slerp_idx,
			       fpreal start_index, fpreal stop_index,
			       fpreal *xdata, fpreal *ydata, fpreal *zdata,
			       int size) const;

    void	 evaluateTime(const CL_Track *track, 
	    		  fpreal start_time, fpreal stop_time,
	                  fpreal *data, int size) const
    		 { evaluate(track, getIndex(start_time), 
			    getIndex(stop_time), data, size); }

    void	 resampleTime(const CL_Track *track, 
			      fpreal start_time, fpreal stop_time,
			      fpreal *data, int size) const;

    // Find the max/min height of the track. 
    // If track is nil, find the min max of every track

    void	 getHeight(const CL_Track *track, 
	    		   fpreal start_index, fpreal stop_index,
			   int nsamples, fpreal *imin, fpreal *imax) const;

    // Find the min max of the graph over *each* step interval
    // data should be twice as long as size

    void	 evaluateHeightRange(const CL_Track *track, 
			    fpreal start_index, fpreal stop_index,
			    fpreal *data, int size) const;


    // This method extends the clip so that it contains a local index
    // at time t, and returns the index.  The clip may be extended
    // or cycled to accomodate the change.  If delta is given, it
    // returns the index delta of the clips start times.
    // Note all indices are truncated to ints

    int		 extendTime(fpreal t, int *delta);


    //
    // convenience methods
    //

    fpreal		 getStartTime() const
			 { return getTime(myStart); }

    fpreal		 getEndTime() const
			 { return getTime(myStart + myTrackLength - 1); }

    void		 dupAttributes(const CL_Clip &);
    
    //
    // unit conversion methods
    //

    fpreal		 getIndex(fpreal time) const
			 { return time*mySampleRate; }

    fpreal		 getTime(fpreal index) const
			 { return index / mySampleRate; }

    fpreal		 getFrame(fpreal index, fpreal fps) const
			 { return index*fps/mySampleRate + 1;}

    fpreal		 getIndexF(fpreal frame, fpreal fps) const
			 { return getTimeF(frame, fps)*mySampleRate; }

    static fpreal	 getTimeF(fpreal frame, fpreal fps)
			 { return (frame - 1) / fps; }

    static fpreal	 getFrameT(fpreal time, fpreal fps)
			 { return time*fps + 1; }


    //
    // Data operations
    //

    void		 trim(const CL_Clip *clip, fpreal leftpos, 
			      fpreal rightpos, int discard);

    // shorten the tracklength to newlen, if add is 1 then the
    // remaining samples are added to the beginning
    // Returns 1 if newlen is smaller than the beginning length

    int			 crop(int newlen, int add);


    //
    // I/O methods
    // Note: 0 is success, -ve is fail
    //

    int			 save(UT_OStream &os,
			      const CL_ClipPrivateIO *priv = 0,
			      bool use_blosc_compresssion = false) const;
    int			 save(UT_OStream &os, UT_StringArray& parms,
			      const CL_ClipPrivateIO *priv = 0,
			      bool use_blosc_compresssion = false) const;
    int			 save(const char *nm) const;

    // Returns true on success and false otherwise.
    bool		 load(UT_IStream &is, const char *path=0);

    int			 load(const char *nm);

    int64		 getMemoryUsage(bool inclusive) const;
    int			 info(UT_OStream &os, fpreal fps, fpreal t,
			      bool limit, CL_InfoCallback cb,
			      void *cbdata) const;
    int			 info(UT_InfoTree &tree, fpreal fps, fpreal t,
			      CL_InfoCallback cb, void *cbdata) const;

    static fpreal	 fixedDecimal(fpreal num, int digits = 2);
    static void		 printNum(UT_OStream &os,fpreal num);

    //
    // This method will reorder our tracks given a new list
    // Should be used with caution.

    void		 reorder(CL_TrackList &list);

    // Given 3 track indices compute the sets of rotations that will
    // eliminate any discontinuites introduced by static cracking.
    // Negative indices are considered constant 0 tracks.
    // Optionally supply a desired beginning rotation

    void		 crackSmooth(int xidx, int yidx, int zidz,
				     const UT_XformOrder &order,
				     int use_hint,
				     fpreal hx, fpreal hy, fpreal hz);

    // This method will return true if the clips lie over the same
    // range (ie, sample-to-sample correspondence

    int			 isSameRange(const CL_Clip &v) const;

    // These methods can be used to increase performance by reducing
    // memory management

    void		 stashAll();
    void		 destroyStashed();

    // Generate a new unique name based on an existing name.
    // oldname is the string to test and the method assumes the name is already being used.
    //         This means the method will always generate a new name even if oldname was valid unique name already.
    // newname workbuffer is used to generate and return a new name.
    void		 newName(const UT_StringRef &oldname, UT_WorkBuffer& newname) const;

    void		 setDefaults(void);

    // Called by CL_Track to update the name hash table when loaded.
    void		 trackRenamed(CL_Track *track,
				      const UT_StringRef &oldname);


    // Attributes
    int setAttribute( const UT_StringHolder &name, const UT_StringHolder &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const fpreal32 &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const fpreal64 &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const int &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const int64 &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Vector2  &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Vector2D &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Vector3  &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Vector3D &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Vector4  &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Vector4D &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Matrix2  &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Matrix2D &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Matrix3  &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Matrix3D &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Matrix4  &v, int track=-1, int sample=-1 );
    int setAttribute( const UT_StringHolder &name, const UT_Matrix4D &v, int track=-1, int sample=-1 );

    int getAttribute( const UT_StringHolder &name, UT_StringHolder &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, fpreal32 &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, fpreal64 &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, int &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, int64 &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Vector2  &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Vector2D &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Vector3  &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Vector3D &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Vector4  &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Vector4D &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Matrix2  &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Matrix2D &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Matrix3  &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Matrix3D &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Matrix4  &v, int track=-1, int sample=-1 ) const;
    int getAttribute( const UT_StringHolder &name, UT_Matrix4D &v, int track=-1, int sample=-1 ) const;

    const CL_ClipAttributes *getAttributes() const { return myAttributes; }
    void setAttributes( const CL_ClipAttributes *attrs );
    void updateAttributes( const UT_StringSet &keys, const CL_ClipAttributes *attrs );

    // Removes a single attribute instance in the attribute map.
    int removeAttribute(const UT_StringHolder &name, int track=-1, int sample=-1);
    static int removeAttribute(CL_ClipAttributes *&attributes, const UT_StringHolder &name, int track=-1, int sample=-1);

    // Removes a list of attributes.
    // The string should contain space separated pairs of "attrclass:attrname"
    // where attrclass is one of "auto clip channel channelsample sample"
    int removeAttributes(const UT_StringHolder &names);
    static int removeAttributes(CL_ClipAttributes *&attributes, const UT_StringHolder &names);

    // Lists the attribute names from attrclass
    int getAttributeNames( const UT_StringHolder &attrclass, UT_StringArray &names ) const;

    // Lists the attribute names from attrclass
    static int getAttributeNames(const CL_ClipAttributes *attributes, const UT_StringHolder &attrclass, UT_StringArray &names );
    static inline bool
    buildKey( const UT_StringHolder &name, int track, int sample,
	      UT_WorkBuffer &key );

    bool isUsingCurrentFrame() const
	{ return myIsUsingCurrentFrame; }
    void setIsUsingCurrentFrame(bool b)
	{ myIsUsingCurrentFrame = b; myIsConstantSet = false; }

    bool isConstant() const
    {
	if (myIsUsingCurrentFrame)
	    return false;
	if (!myIsConstantSet)
	    const_cast<CL_Clip*>(this)->computeIsConstant();
	return myIsConstant;
    }
    void unsetIsConstant()
	{ myIsConstantSet = false; }

private:
    CL_Clip(); //prevent default ctor

    // Attributes
    template< class T >
    int setAttributeT( const UT_StringHolder &name, const T &v, int track=-1, int sample=-1 );

    template< class T >
    int getAttributeT( const UT_StringHolder &name, T &v, int track=-1, int sample=-1 ) const;

    void clearTrackNames(bool set_to_null=true);

    void computeIsConstant();

private:

    fpreal			 mySampleRate;
    fpreal			 myStart;

    int				 myTrackLength;
    CL_TrackList		 myTracks;
    CL_TrackList		 myStashedTracks;

    UT_IntArray	 		 mySXTracks;	// Quaternion tracks
    UT_IntArray			 mySYTracks;
    UT_IntArray			 mySZTracks;

    UT_XformOrder::xyzOrder	 mySRotOrder;
    CL_ClipAttributes		*myAttributes;

    UT_NameManager<CL_Track*>	 myTrackNames;

    int				 myStashedTracksNext;

    bool			 myCheckDuplicateNames;
    bool			 myIsConstantSet;

    unsigned			 myDisableRemovingTrackNames:1,
				 myIsUsingCurrentFrame:1,
				 myIsConstant:1;

    class ClearTrackNamesTask;
    friend class ClearTrackNamesTask;

};

typedef UT_ValArray<CL_Clip *>		CL_ClipList;
typedef UT_ValArray<const CL_Clip *>	CL_ClipListC;
typedef UT_SharedPtr<CL_Clip>		CL_ClipPtr;

///////////////////////////////////////////////////////////////////////////////
//
// Inline implementation
//

inline CL_Track *
CL_Clip::getTrack(const UT_StringRef &name, int *index)
{
    CL_Track *track = myTrackNames.findSymbol(name);
    if (index)
	*index = track ? track->getIndex() : -1;
    return track;
}

inline const CL_Track *
CL_Clip::getTrack(const UT_StringRef &name, int *index) const
{
    const CL_Track *track = myTrackNames.findSymbol(name);
    if (index)
	*index = track ? track->getIndex() : -1;
    return track;
}

inline bool
CL_Clip::buildKey(const UT_StringHolder &name, int track, int sample,
		  UT_WorkBuffer &key)
{
    if (!name.isstring())
	return false;

    if (track<0 && sample<0)
	key.sprintf("%s -1 -1", name.buffer());
    else if(track <0 && sample>=0)
	key.sprintf("%s -1 %d", name.buffer(), sample);
    else if(track >=0 && sample<0)
	key.sprintf("%s %d -1", name.buffer(), track);
    else
	key.sprintf("%s %d %d", name.buffer(), track, sample );
    return true;
}

#endif // __CL_Clip__
