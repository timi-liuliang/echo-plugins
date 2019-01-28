/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PointCapture.h ( GEO Library, C++)
 *
 * COMMENTS:  This class handles the data management of point capture 
 * 	      attributes.  Point capture attributes consist of two numbers,
 * 	      an index and a weight.
 * 	      It transfers this data to a gdp using the transferToGdp() 
 * 	      method
 */

#ifndef __GEO_CaptureData__
#define __GEO_CaptureData__

#include "GEO_API.h"
#include <GA/GA_Types.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <iosfwd>

struct GEO_CaptureBoneStorage;

#define  GEO_CAPTUREDATA_DEFAULT_VALUE -1.0F

enum GEO_CaptureData_PtAtr_Entry
{
    I_GEO_CAPTUREDATA_INDEX  = 0,
    I_GEO_CAPTUREDATA_WEIGHT,
    GEO_CAPTUREDATA_PT_ATR_ENTRY_SZ		// should always be last
};

class GEO_Detail;
class GA_PointGroup;


class GEO_API GEO_CaptureRegionData
{
public:
	         GEO_CaptureRegionData() {};
		~GEO_CaptureRegionData() {};

    const UT_String  &getPath() const    { return myPath; }
    int		 changeString( const char *from, const char *to, bool fullword )
		    { return myPath.changeString( from, to, fullword ); }
    const UT_Matrix4 &getXform() const   { return myXform; }
    float        getXtaper() const       { return myXtaper; }
    float        getZtaper() const       { return myZtaper; }
    float        getTopCapHeight() const { return myTopCapHeight; }
    float        getBotCapHeight() const { return myBotCapHeight; }
    float        getMinWeight() const	 { return myMinWeight; }
    float        getMaxWeight() const	 { return myMaxWeight; }
    int          getID() const           { return myID; }
    unsigned     getPrimNum() const      { return myPrimNum; }

    void	 setPath (const UT_String &path) { myPath.harden(path); }
    void	 setXform(const UT_Matrix4 &xf)  { myXform = xf; }
    void	 setXZtapers(float x, float z)   { myXtaper = x; myZtaper=z; }
    void	 setTopCapHeight(float h)        { myTopCapHeight = h; }
    void	 setBotCapHeight(float h)        { myBotCapHeight = h; }
    void	 setMinWeight(float m)		 { myMinWeight = m; }
    void	 setMaxWeight(float m)		 { myMaxWeight = m; }
    void	 setID(int id)        		 { myID = id; }
    void	 setPrimNum(unsigned pn)  	 { myPrimNum = pn; }

    // this operators are need for UT_Array<GEO_CaptureRegionData> to work
    GEO_CaptureRegionData &operator=  (const GEO_CaptureRegionData &d);
    bool		   operator== (const GEO_CaptureRegionData &d) const;
    bool		   operator!=(const GEO_CaptureRegionData &d) const
    { return !(*this == d); }

private:
    UT_String	 myPath;	// tube full paths
    UT_Matrix4	 myXform;	// tube xform
    float	 myXtaper;	// tube tapers
    float	 myZtaper;
    float	 myTopCapHeight;	// tube end cap heights
    float	 myBotCapHeight;
    float	 myMinWeight;
    float	 myMaxWeight;
    int	 	 myID;		// tube unique ID
    unsigned  	 myPrimNum;	// tube prim number
};

class GEO_API GEO_CaptureData
{
public:
	   GEO_CaptureData();
	  ~GEO_CaptureData();

    void   initialize( const UT_String &sop_path,
		       float capture_frame, int num_pts=0, int num_regions=0,
		       int num_entries_per_pnt=0 );

    ///
    /// Capture Region Methods
    ///
    unsigned int getNumRegions() const { return myRegionData.entries(); }

    const UT_String  &regionPath (unsigned i) const
		      { return myRegionData(i).getPath();}
    const UT_Matrix4 &regionXform(unsigned i)const
		      { return myRegionData(i).getXform();}
    int	      	      regionID   (unsigned i) const 
		      { return myRegionData(i).getID();}
    int	      	      regionPrimNum(unsigned i) const
		      { return (int)myRegionData(i).getPrimNum(); }
    float	      taperX(unsigned i) const 
		      { return myRegionData(i).getXtaper(); }
    float	      taperZ(unsigned i) const 
		      { return myRegionData(i).getZtaper(); }
    float	      topCap(unsigned i) const 
		      { return myRegionData(i).getTopCapHeight(); }
    float	      botCap(unsigned i) const 
		      { return myRegionData(i).getBotCapHeight(); }
    float	      minWeight(unsigned i) const 
		      { return myRegionData(i).getMinWeight(); }
    float	      maxWeight(unsigned i) const 
		      { return myRegionData(i).getMaxWeight(); }

    int		      changeString( const char *from, const char *to,
				    bool fullword );

    void	      updateRegionPath(unsigned int i,
				       const UT_String &new_path,
				       int new_op_id);

    /// returns the CaptureData index for the given OP path/ID
    int		 findRegion(const UT_String &region_path,unsigned primnum)const;
    int		 findRegion(int OPuniqueID, unsigned primnum )const;

    void	 getRegionList(int OPuniqueID, UT_IntArray &regions) const;

    /// create space for new region and return the new index
    int		 appendRegion( int opID, const UT_String &path, 
			       int tube_prim_num, const UT_Matrix4 &xform,
			       float taperx, float taperz, 
			       float bot_cap, float top_cap,
			       float min_weight, float max_weight );
    
    int 	 saveOverrideFile( const UT_String &filename, 
				   const GA_PointGroup *pt_group=0,
				   const GEO_Detail *gdp=0,
				   int   binary=0) const;
    int 	 saveOverrideFile( std::ostream &os, 
				   const GA_PointGroup *pt_group=0,
				   const GEO_Detail *gdp=0,
				   int binary=0) const;
    // returns 0 if file not found, the negative of the line_number 
    // if there was a problem with the file format.  Otherwise return
    // the number of lines read from the file.
    bool	 loadOverrideFile( const char *filename, int &line_num );

    ///
    /// Per point Data methods
    ///

    /// adds a non-zero weighted point entry. assumes that we're inserting
    /// in sorted order without duplicates.
    void   appendSortedPtEntry(GA_Index point_num, int idx, float weight);

    /// Sort and normalize weights
    void   sortAndNormalizePtWeights();

    int    getNumEntriesPerPt() const	{ return myMaxEntriesPerPt; }
    int    getNumStoredPts() const	{ return myPtEntry.entries(); }

    /// puts our data into the given gdp's point and detail attributes
    /// returns 1 on success, 0 otherwise
    /// transferToGdp doesn't have any failure case so doesn't return anything.
    void   transferToGdp( GEO_Detail *gdp, bool destroy, float blend,
			  bool relative_skel_root ) const;
    int    transferFromGdp(const GEO_Detail *gdp,const GA_PointGroup *pt_group);

    /// verb method to get the region data out of the raw float array data from
    /// a gdp's detail attribute
    static void   parsePointData( const float *data, int idx,
				  int &region_idx, float &weight );

    /// Retrieves the weight of the point at the specified index. The weight
    /// corresponds to the region at region_idx index.
    /// @param	    prev_array_idx - an optional speed optimization param. If 
    ///		    this is called in a loop for consecutive points, set to 0
    ///		    initially and pass the same variable to this function while
    ///		    in the loop.
    float getPointWeight(int point_idx, int region_idx, int *prev_array_idx = NULL);

    /// constructs a capture region detail attribute data using the given
    /// parameters into tube_data which must have
    /// GB_AttributeCaptureRegion::getBoneSize() number of floats
    static int	 constructCaptureRegionDetailData(
				    const char *cregion_name,
				    const UT_Matrix4 &parent_xform,
				    const GEO_Detail *tube_gdp,
				    GEO_CaptureBoneStorage &tube_data );

    int	   getDetailDataFromGdp( const GEO_Detail *gdp );

private: // methods
    bool 	 loadOverrideFile( UT_IStream &is, int &line_num );

    int    savePointEntry(std::ostream &os, int i, const float *pt_data,
			  bool binary) const;

    /// helper methods for transferDataToGdp()
    void   computeSkelRootPath( UT_String &root_path ) const;
    int	   putDetailDataInGdp( GEO_Detail *gdp, bool replace,
			       bool relative_skel_root,
			       UT_IntArray &region_map ) const;
    int	   putPointDataInGdp( GEO_Detail *gdp, bool replace, float blend,
			      const UT_IntArray &region_map ) const;
    int	   getPointDataFromGdp( const GEO_Detail *gdp, 
			        const GA_PointGroup *pt_group );

private: // data

    struct PtEntry
    {
	PtEntry()   { }		 // ctor leaves data uninitialized for speed
	GA_Index    pt_num;	 // point number
	int	    num_entries; // number of non-zero weighted entries for pt
    };

    float				myCaptureFrame;
    UT_String				mySopPath;
    UT_Array<GEO_CaptureRegionData>	myRegionData;
    UT_Array<PtEntry>			myPtEntry;
    UT_FloatArray			myPtData;
    int					myMaxEntriesPerPt;
};

///////////////////////////////////////////////////////////////////////////////
//
// Inline Implementations
//

inline void
GEO_CaptureData::appendSortedPtEntry(GA_Index point_num, int idx, float weight)
{
    // this functions assume that we're appending in sorted order
    // and without duplicates
    UT_ASSERT_P(myPtEntry.entries() == 0
		|| point_num >= myPtEntry.last().pt_num);

    if (myPtEntry.entries() == 0 || point_num != myPtEntry.last().pt_num)
    {
	myPtEntry.append();
	myPtEntry.last().pt_num = point_num;
	myPtEntry.last().num_entries = 0;
    }

    myPtData.append(float(idx));    // Append the capture region index
    myPtData.append(weight);	    // Append the corresponding weight

    myPtEntry.last().num_entries++;
    if (myPtEntry.last().num_entries > myMaxEntriesPerPt)
	myMaxEntriesPerPt = myPtEntry.last().num_entries;
}

#endif
