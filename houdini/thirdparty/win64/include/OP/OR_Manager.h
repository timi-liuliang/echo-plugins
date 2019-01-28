/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OR_Manager.h ( OP Library, C++)
 *
 * COMMENTS:
 *
 *	manager to update/remove/add parameter override data
 */

#ifndef __OR_Manager__
#define __OR_Manager__

#include "OP_API.h"
#include "OP_Node.h"
#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>

class PRM_Parm;
class orData;
class CL_Clip;
class CH_Collection;
class OR_Mapping;
class CH_ActiveLayerContrib;
class UT_StringRef;

//
// This class is used to minimize string parsing and node globbing
//

class OP_API orPath
{

public:

     orPath(OP_Node *src);
    ~orPath();

    void	setExport(const UT_String &xport);
    int		globNodes(OP_NodeList &globbed);
    void	forceDirty() { myDirty = 1; }

    int64	getMemoryUsage(bool inclusive) const;

private:

    void	reset();

    UT_String		  myExport;
    OP_Node		 *mySource;
    int			  myDirty;
    int			  myArgc;
    char		**myArgv;
    int			  myArgvSize;
};


class OP_API orUnit
{
public:

    OP_Node		*myDestOp;
    int			 myTrackIndex;
    int			 myParmIndex;
    int			 myVecIndex;
    const CL_Track	*myEnableTrack;
};

typedef UT_Array<orUnit *>		orUnitList;

///// an exported data unit

class OP_API orData
{
public:
    OP_Node		*dest;
    int			 parm_idx;
    int			 vec_idx;
    OP_Node		*source;
    OP_Node		*conflict;
    int			 track_idx;
    int			 used;
    int			 current;
    const CL_Track	*enable_track;
};


class OP_API OR_Manager
{

public:

     OR_Manager();
    ~OR_Manager();

    int			 getConflicts(const OP_Node *src, OP_NodeList &list);
    const OP_Node	*getFirstConflict(const OP_Node *src);

    void	 	 removeDestination(OP_Node *dest);
    void	 	 removeDestination(OP_Node *dest, int parm_idx);

    bool		 findDestination(UT_String &result,
					 const OP_Node *, int idx)const;

    void		removeSource(OP_Node *src,
				     OP_NodeList &affected,
				     UT_IntArray &indices,
				     int previous);

    int		 	 override(OP_Node *src, const CL_Clip *clip,
				  const UT_String &export_path, 
				  OP_NodeList &affected,
				  UT_IntArray &indices,
				  orPath &pathparm,
				  OR_Mapping *mapping);

    void		 override(OP_Node *src, const CL_Clip *clip,
				  const orUnitList &units,
				  OP_NodeList &affected,
				  UT_IntArray &indices,
				  orPath &pathparm,
				  OR_Mapping *mapping);
    
    /// Copy the channel collection corresponding to the given clip to the
    /// export destination.
    int		 	 copy(OP_Node *src,
			      const CL_Clip *clip,
			      const CH_Collection *coll,
			      const UT_String &export_path, 
			      OP_NodeList &affected,
			      UT_IntArray &indices,
			      orPath &pathparm);

    void		 getTrackDestinations(
				const OP_Node *src,
				UT_IntArray &track_indices,
				OP_NodeList &nodes,
				UT_ValArray<PRM_Parm *> &parm_list,
				UT_IntArray &vec_indices);

    void		 getDestinations( const OP_Node *src,
					  UT_ValArray<OP_Node *> &op_list );
    void		 getSources( const OP_Node *dest,
				     UT_ValArray<OP_Node *> &op_list,
				     bool append );

    void		 unexportWithSameParmValues(fpreal t, OP_Node *src );


    // these methods must be called with caution.
    // ie, only called by the static callback

    int			getNumExports() const 
			{ return myData.entries(); }
    
    const orData	*getData(int idx) const
			{ return myData(idx); }
    
    orData		*getData(int idx)
			{ return myData(idx); }

    // Fills CH_ActiveLayerContrib with the layer contribution values from a Parm
    void getActiveLayerContribution( CH_ActiveLayerContrib& c, PRM_Parm *p, int i, fpreal t );


    bool getTrackDestination(const OP_Node *src,
				     int track_idx,
				     OP_Node **out_node,
				     PRM_Parm **out_parm,
				     int &vec_idx);
private:

    UT_ValArray<orData *>	myData;


    // Exactly one of clip/coll must be non-nil
    // If clip is given, exporting is done
    // If coll is given, copying is done

    int			processDestNodes(OP_Node *src,
	    				  const CL_Clip *clip,
					  const CH_Collection *coll,
					  OP_NodeList &dest_ops,
					  OP_NodeList &affected,
					 UT_IntArray &indices,
					 OR_Mapping *mapping);

    int			processParmList(OP_Node *src,
					OP_Node *dest_op,
					const UT_StringRef &channel_name,
					int track_idx,
					OP_NodeList &affected,
					UT_IntArray &indices,
					const CL_Clip *clip,
					const CH_Collection *coll);

    int			processSingle(OP_Node *src,
					const CL_Clip *chop,
					int track_idx,
					OP_Node *dest_op,
					int parm_index,
					int vec_index,
					const CL_Track *enable_track,
					OP_NodeList &affected,
					UT_IntArray &indices
					);

    int			processMapping(OP_Node *src,
				       const CL_Clip *clip,
				       OP_NodeList &affected,
				       UT_IntArray &indices,
				       OR_Mapping *mapping);


    int			getNextDataIndex(OP_Node *dest,
	    				 int parm_index, int vec_idx);

    int			addData(OP_Node *dest, int parm_idx, int vec_idx,
	                        OP_Node *source, int track_idx,
				const CL_Track *enable_track);

    int			getOverrideType( OP_Node *src ) const;

    int			myAddedUndoLevel;

};

OP_API extern OR_Manager  *ORgetManager();

#endif

