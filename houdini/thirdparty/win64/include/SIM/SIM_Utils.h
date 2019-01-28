/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Utils_h__
#define __SIM_Utils_h__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_DataFilter.h"
#include "SIM_Time.h"
#include "SIM_Names.h"
#include "SIM_CacheCompression.h"
#include <GEO/GEO_Primitive.h>
#include <UT/UT_SysSpecific.h>
#include <UT/UT_Array.h>

class SIM_Solver;
class SIM_Object;
class SIM_Relationship;

/// This macros simplifies calling and casting the return value from the
/// SIM_Data::getPointerToType() function. The returned value is a DataClass
/// pointer.
#define SIM_DATA_CAST(Data, DataClass) \
    ((DataClass *)(SIM_Data::castPointerToType( \
		   static_cast<SIM_Data *>(Data), #DataClass)))
/// This macros simplifies calling and casting the return value from the
/// SIM_Data::getPointerToType() function. The returned value is a const
/// DataClass pointer.
#define SIM_DATA_CASTCONST(Data, DataClass) \
    ((const DataClass *)(SIM_Data::castConstPointerToType( \
			 static_cast<const SIM_Data *>(Data), #DataClass)))
/// This macro simplifies the calling and casting the return value from
/// SIM_Data::getNamedSubData(). The returned value is a DataClass
/// pointer.
#define SIM_DATA_GET(Parent, DataName, DataClass) \
    SIM_DATA_CAST((Parent).getNamedSubData(DataName), DataClass)
/// This macro simplifies the calling and casting the return value from
/// SIM_Data::getConstNamedSubData(). The returned value is a const DataClass
/// pointer.
#define SIM_DATA_GETCONST(Parent, DataName, DataClass) \
    SIM_DATA_CASTCONST((Parent).getConstNamedSubData(DataName), DataClass)
/// This macro simplifies the calling and casting the return value from
/// SIM_Data::getNthSubData(), using SIM_DataFilterByType. The returned
/// value is a DataClass pointer.
#define SIM_DATA_GETNTH(Parent, DataClass, N, StartFrom, RecurseFilter) \
    SIM_DATA_CAST((Parent).getNthSubData( \
			   NULL, SIM_DataFilterByType(#DataClass), N, \
			   StartFrom, RecurseFilter), DataClass)
/// This macro simplifies the calling and casting the return value from
/// SIM_Data::getNthConstSubData(), using SIM_DataFilterByType. The returned
/// value is a const DataClass pointer.
#define SIM_DATA_GETNTHCONST(Parent, DataClass, N, StartFrom, RecurseFilter) \
    SIM_DATA_CASTCONST((Parent).getNthConstSubData( \
				NULL, SIM_DataFilterByType(#DataClass), N, \
			        StartFrom, RecurseFilter), DataClass)
/// This macro simplifies calling and casting the return value of a call to
/// the SIM_Data::createNamedSubData function.
#define SIM_DATA_CREATE(Parent, DataName, DataClass, Flags) \
    ((DataClass *)(static_cast<SIM_Data &>(Parent)). \
    createNamedSubData(DataName, #DataClass, Flags))
/// This macro simplifies the calling and casting the return value from
/// SIM_Data::getorCreateAlternateRepresentation(). The returned value is a
/// DataClass pointer.
#define SIM_DATA_GETORCREATE_ALT(Parent, DataName, DataClass) \
    SIM_DATA_CAST((Parent).getOrCreateAlternateRepresentation( \
			   DataName, #DataClass), DataClass)

/// This function converts const SIM_Data pointers in an array to pointers
/// of any const SIM_Data subclass. If one of the SIM_Data pointers cannot be
/// cast to the requested new type, it is not added to the new array.
template <class DataClass>
void
SIMconvertConstArray(const SIM_ConstDataArray &src,
		     UT_ValArray<const DataClass *> &dest,
		     const char *DataClassName,
		     bool includenulls = false)
{
    const DataClass	*converted;

    dest.entries(0);
    for( int i = 0; i < src.entries(); i++ )
    {
	converted = src(i) ?
		    (const DataClass *)src(i)->
		    getConstPointerToType(DataClassName) :
		    0;
	if( converted || includenulls )
	    dest.append(converted);
    }
}

/// This function converts SIM_Data pointers in an array to pointers
/// of any SIM_Data subclass. If one of the SIM_Data pointers cannot be
/// cast to the requested new type, it is not added to the new array.
template <class DataClass>
void
SIMconvertArray(const SIM_DataArray &src,
		UT_ValArray<DataClass *> &dest,
		const char *DataClassName,
		bool includenulls = false)
{
    DataClass	*converted;
    int		 i;

    dest.entries(0);
    for( i = 0; i < src.entries(); i++ )
    {
	converted = src(i) ?
		    (DataClass *)src(i)->getPointerToType(DataClassName) :
		    0;
	if( converted || includenulls )
	    dest.append(converted);
    }
}

/// A utility method for computing the transform from the local-space of an
/// object to world-space co-ordinates.
SIM_API void SIMgetPositionTransform(UT_DMatrix4 &result, const SIM_Object &);
/// A utility method for computing the transform from the local-space of an
/// object's geometry to world-space co-ordinates.
SIM_API void SIMgetGeometryTransform(UT_DMatrix4 &result, const SIM_Object &);

/// Returns object properties at the given point (if non-negative) or position.
SIM_API void SIMgetObjectProperties(
    const SIM_Object &obj,
    const UT_Vector3 &pos,
    const GA_Index ptnum,
    fpreal &friction,
    fpreal &bounce,
    fpreal &dynamicfriction,
    fpreal &bounceforward);

inline void SIMgetObjectProperties(
    const SIM_Object &obj,
    const UT_Vector3 &pos,
    const GA_Index ptnum,
    fpreal &friction,
    fpreal &bounce,
    fpreal &dynamicfriction)
{
    fpreal		dontcare = 0;
    SIMgetObjectProperties(obj, pos, ptnum, friction, bounce, dynamicfriction,
			    dontcare);
}

/// Returns true if the given primitive is a polygon with three
/// vertices.
SIM_API bool SIMisTri(const GEO_Primitive &prim);



/// Information about a single edge.
class SIM_API sim_EdgeDataEntry
{
public:
         sim_EdgeDataEntry(int pid_e0, int pid_e1, int fid0, int fid1,
                      int pid_f0, int pid_f1)
            : myPidE0(pid_e0), myPidE1(pid_e1),
            myFid0(fid0), myFid1(fid1),
            myPidF0(pid_f0), myPidF1(pid_f1)
         { }
         sim_EdgeDataEntry()
         { }

    /// The ids of the points on the edge.
    int  myPidE0, myPidE1;
    /// The ids of the faces adjacent to the edge. Second id may be <0 if
    /// only one face is adjacent.
    int  myFid0, myFid1;
    /// The ids of the other points in the respective faces. Only
    /// meaningful if the faces are triangles (in which case there's only one
    /// other point).
    int  myPidF0, myPidF1;
};
typedef UT_Array<sim_EdgeDataEntry> SIM_EdgeData;

/// Calculate information about triangle edges in the given gdp.
/// This is intended as a lightweight alternative to GQ_Detail, with only
/// very basic edge information.
SIM_API void SIMcalcEdgeData(SIM_EdgeData &edgeData, const GU_Detail &gdp);




/// Declaration of the function that must be implemented by custom SIM
/// library extensions.
extern "C" {
    SYS_VISIBILITY_EXPORT extern void initializeSIM(void *data);
}

/// Call this initialization function to register all data types, solvers,
/// and colliders in this library.
SIM_API void	 initializeSIM();

/// Constructs guide geometry to illustrate connection between an anchored
//position and an object.
SIM_API void
SIMbuildAnchorGuideObjectLink(GU_Detail *gdp,
			      GA_Offset ptoff,
			      const SIM_Object &object,
			      const SIM_Options &options,
			      const SIM_Relationship &rel);
SIM_API void
SIMbuildAnchorGuideObjectLink(GU_Detail* gdp,
                           GA_Offset ptoff,
                           const UT_Vector3& obj_pos,
                           fpreal scale,
                           const GA_RWHandleV3& color_attrib,
                           const UT_Vector3& color,
			   const UT_Vector3& affcolor);

/// Used in SIM_ConAnchorObjPointGroupPos and
/// SIM_ConAnchorObjPointGroupRot to generate a point list WITH
/// duplicates from a pattern string.
SIM_API bool 
SIMtryCachePointList(UT_IntArray& points, const char* pattern);

#endif

