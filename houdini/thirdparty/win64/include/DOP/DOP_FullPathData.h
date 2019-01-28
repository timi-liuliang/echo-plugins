/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_FullPathData.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_FullPathData_h__
#define __DOP_FullPathData_h__

#include "DOP_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <OP/OP_Node.h>
#include <GU/GU_DetailHandle.h>
#include "DOP_Engine.h"

class UT_WorkBuffer;
class UT_StringArray;
class GU_ConstDetailHandle;
class SIM_Data;
class SIM_Object;
class SIM_ConstObjectArray;
class SIM_SDF;
class OP_Context;
class OP_GlobContext;
class DOP_Parent;
class DOP_SimulationChangeSink;

class DOP_API DOP_FullPathData
{
public:
			 DOP_FullPathData()
			 {
			     myNode = 0;
			     myParent = 0;
			     myRootData = 0;
			     myData = 0;
			     myDataName = "";
			     myGlobalTime = 0;
			 }
			 DOP_FullPathData(const DOP_FullPathData &src)
			 {
			     *this = src;
			 }
			~DOP_FullPathData()
			 {
			 }

    static const SIM_Position *getBestPosition(const SIM_RootData *rootdata,
					const SIM_Data *data,
					const char *dataname)
    {
	const SIM_Position *position = 0;
	const SIM_Object	*obj;

	// Get our root data as a SIM_Object.
	obj = SIM_DATA_CASTCONST(rootdata, SIM_Object);

	// Get the transform from the SIM_Position data.
	if (data)
	    position = SIM_DATA_CASTCONST(data, SIM_Position);

	// If it's an object, get the "Position" data.
	if (!position && obj && obj == data)
	    position = obj->getPosition();

	// Check if we can get a position from the
	// data directly.
	if (!position && obj)
	    position = obj->getPositionForGeometry(dataname);

	// If it's an SDF, check the parent's position.
	if (!position && obj && SIM_DATA_CASTCONST(data, SIM_SDF))
	{
	    UT_String	geopath, sdfname;
	    UT_StringWrap(dataname).splitPath(geopath, sdfname);
	    position = obj->getPositionForGeometry(geopath);
	}

	return position;
    }

    const SIM_Position	*getBestPosition() const
    {
	return getBestPosition(myRootData, myData, myDataName);
    }

    static GU_ConstDetailHandle getBestGeometry(UT_DMatrix4 &xform,
					const SIM_RootData *rootdata,
					const SIM_Data *data,
					const char *dataname,
					fpreal simtime);

    GU_ConstDetailHandle getBestGeometry(UT_DMatrix4 &xform) const;

    bool		 operator==(const DOP_FullPathData &cmp) const
			 {
			     return ((myNode == cmp.myNode) &&
				     (myParent == cmp.myParent) &&
				     (myRootData == cmp.myRootData) &&
				     (myData == cmp.myData) &&
				     (myDataName == cmp.myDataName));
			 }

    const DOP_FullPathData &operator=(const DOP_FullPathData &src)
			 {
			     myNode = src.myNode;
			     myParent = src.myParent;
			     myRootData = src.myRootData;
			     myData = src.myData;
			     myDataName.harden(src.myDataName);

			     return *this;
			 }

    void		 clear()
			 {
			     myNode = 0;
			     myParent = 0;
			     myRootData = 0;
			     myData = 0;
			     myDataName = "";
			 }

    OP_Node		*myNode;
    DOP_Parent		*myParent;
    const SIM_RootData	*myRootData;
    const SIM_Data	*myData;
    UT_String		 myDataName;

    // Not simulation time.
    fpreal		 myGlobalTime;
};

class DOP_API DOP_FullPathDataArray : public UT_Array<DOP_FullPathData>
{
public:
			 DOP_FullPathDataArray();
    virtual		~DOP_FullPathDataArray();

    void		 buildFromPaths(const UT_StringArray &paths);
    void		 getObjectArray(SIM_ConstObjectArray &objects) const;
    void		 getObjectString(UT_WorkBuffer &buffer) const;
};

DOP_API bool	 DOPfindDataFromPath(const char *path,
				DOP_FullPathData &fulldata,
				fpreal time, OP_Node *relativetonode = 0,
				bool dopparentisenough = false);
DOP_API bool	 DOPfindAllDataFromPath(const char *path,
				DOP_FullPathDataArray &fulldata,
				fpreal time, OP_Node *relativetonode = 0,
				bool dopparentisenough = false);
DOP_API void	 DOPglobPathAndFindAllData(const char *path,
				DOP_FullPathDataArray &fulldata,
				OP_NodeList &nodatadopnets,
				fpreal time, OP_Node *relativetonode = 0,
				OP_GlobContext *context = 0);

/// If the dopparent is currently simulating it is not possible to
/// reset its time.  Similarly, if the desired time is within the last
/// timestep, we can't interpolate since the actual 'current' value
/// of the object is stored at the end time.
/// Returned from this is the new time to use for accesses.
/// In case of unsimulated networks, it is the same as
/// dopparent->setDOPTime(time); return time;
DOP_API SIM_Time DOPsetBestTime(DOP_Parent *dopparent, SIM_Time time);

/// The following functions are the only ones which are thread-safe for
/// accessing DOP data
// @{

/// Thread-safe method to find the owner node of a DOP data path
DOP_API OP_Node *   DOPgetNodeFromDataPath(
				const char *path,
				fpreal time, OP_Node *relativetonode = 0,
				bool dopparentisenough = false);

/// Thread-safe method to world transform of the a DOP data path, optionally
/// returning the geometry if there is any (and gdh is non-NULL). If given
/// an interested_node, then we will add an extra input on it to the path.
DOP_API bool	    DOPgetWorldTransformFromDataPath(
				UT_DMatrix4 &xform,
				GU_ConstDetailHandle *gdh,
				bool &timedep,
				const char *path,
				OP_Context &context,
				OP_Node *relativetonode = 0,
				bool dopparentisenough = false,
				OP_Node *interested_node = 0);

// @}

#endif

