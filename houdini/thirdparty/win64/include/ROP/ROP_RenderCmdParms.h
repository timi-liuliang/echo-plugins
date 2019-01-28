/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS: A class encapsulating the input and output parameters used by
 *           the render commands of both hscript and hom.
 */

#ifndef __ROP_RenderCmdParms_h__
#define __ROP_RenderCmdParms_h__


#include "ROP_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <iosfwd>

class ROP_Node;
class SOHO_ParmOverride;

class ROP_API ROP_RenderCmdParms
{
public:
    ROP_RenderCmdParms() :
	myOutputFile(NULL),
	myOutputFormat(NULL),
	myToFlipbook(false),
	myQuality(2),
	myIgnoreInputs(false),
	myInterleaveFrames(false),
	myIgnoreBypass(false),
	myIgnoreLock(false),
	myTrackDependencies(false),
	myPrintDependencies(false),
	myPrintFullPath(false),
	myCondenseRanges(false),
	myFrameMerge(false),
	myAlfred(false),
	myOutStream(NULL),
	myErrStream(NULL),
	mySohoParms(NULL),
	myValidationFlags(0)	{}
    ~ROP_RenderCmdParms();

    // Set and get the start and end frame of the frame range to be rendered.
    // If getFrameRange returns false then no valid frame range has been
    // set, and the values returned in start_fr and end_fr are undefined.
    void	 setFrameRange(fpreal start_fr, fpreal end_fr);
    bool	 getFrameRange(fpreal &start_fr, fpreal &end_fr);

    // Set and get the frame increment, which is only valid if there is
    // a frame range.  If getFrameIncrement returns false then no valid
    // frame increment has been set, and the value returned in finc is
    // undefined.
    void	 setFrameIncrement(fpreal finc);
    bool	 getFrameIncrement(fpreal &finc);

    SOHO_ParmOverride	*getSohoParms()	{ return mySohoParms; }

    // Set and get the output file where the rendered image will go.
    void	 setOutputFile(const char *output);
    const char	*getOutputFile() { return myOutputFile; }

    // Set and get the format of the rendered image.
    void	 setOutputFormat(const char *fmt);
    const char	*getOutputFormat() { return myOutputFormat; }

    // Set the resolution scale (for SOHO)
    void	 setResolutionScale(fpreal scale);

    // Set a specific resolution override
    void	 setResolution(int xres, int yres);

    // Set and test if we're rendering to a flipbook.
    void	 setToFlipbook(bool on = true);
    bool	 isToFlipbook() { return myToFlipbook; }

    // Set and get the render quality.
    void	 setQuality(int quality) { myQuality = quality; }
    int		 getQuality() { return myQuality; }

    // Set and test if we should ignore inputs to the ROP.
    void	 setIgnoreInputs(bool on = true) { myIgnoreInputs = on; }
    bool	 isIgnoreInputs() { return myIgnoreInputs; }

    // Set and test if we're rendering frame by frame instead of Rop by Rop.
    void	 setInterleaveFrames(bool on = true)
		 { myInterleaveFrames = on; }
    bool	 isInterleaveFrames() { return myInterleaveFrames; }

    // Set and test if we should ignore the bypass flags on the ROPs.
    void	 setIgnoreBypass(bool on = true) { myIgnoreBypass = on; }
    bool	 isIgnoreBypass() { return myIgnoreBypass; }

    // Set and test if we should ignore the lock flags on the ROPs.
    void	 setIgnoreLock(bool on = true) { myIgnoreLock = on; }
    bool	 isIgnoreLock() { return myIgnoreLock; }

    // Set and test if we should track dependencies.  If so, no rendering
    // will be done and we will instead store the ROPs and frames that
    // would have been rendered.  These can be obtained after completion
    // using getROPDendencies and getFrameDependencies.
    void	 setTrackDependencies(bool on = true)
		 { myTrackDependencies = on; }
    bool	 isTrackDependencies() { return myTrackDependencies; }

    // When enabled, frame ranges are consolidated with previous ranges on the
    // same node if the dependencies are the same for the two ranges. This
    // makes the render list smaller, but it may not be appropriate for farm
    // jobs, where consolidation would reduce the benefit of parallel nature of
    // the farm.
    void	 setFrameMerge(bool merge = true) { myFrameMerge = merge; }
    bool         isFrameMerge() const { return myFrameMerge; }

    // Set and test if we should print dependencies.  If so, an output
    // stream must be given.  When printing dependencies we also have
    // the option to print the full path of the ROP and to condense
    // the frame ranges that are output.
    void	 setPrintDependencies(bool print_dependencies,
				      bool print_full_path,
				      bool condense_ranges)
    {
	myPrintDependencies = print_dependencies;
	myPrintFullPath = print_full_path;
	myCondenseRanges = condense_ranges;
    }
    bool	 isPrintDependencies()
    {
	if (myPrintDependencies)
	    UT_ASSERT(getOutStream());
	return myPrintDependencies;
    }
    bool	 isPrintFullPath()
    {
	if (myPrintFullPath)
	    UT_ASSERT(isPrintDependencies());
	return myPrintFullPath;
    }
    bool	 isCondenseRanges()
    {
	if (myCondenseRanges)
	    UT_ASSERT(isPrintDependencies());
	return myCondenseRanges;
    }

    void	  setOutStream(std::ostream *out) { myOutStream = out; }
    std::ostream* getOutStream() { return myOutStream; }

    void	  setErrStream(std::ostream *err) { myErrStream = err; }
    std::ostream* getErrStream() { return myErrStream; }

    /// @{
    /// Handle alfred-style progress
    void	 setAlfred(bool f)	{ myAlfred = f; }
    bool	 getAlfred() const	{ return myAlfred; }
    /// @}

    // Get an array of ROP that will be rendered by the command.  This
    // is only valid is isTrackDependencies() is true and only after
    // the operation has completed.
    const UT_ValArray<ROP_Node*>	&getROPDependencies()
    {
	UT_ASSERT(isTrackDependencies());
	return myROPDependencies;
    }

    // Get an array of frame lists indicating which frames will be
    // rendered by the command.  Each frame list corresponds to the
    // respective ROP_Node in the getROPDependencies array.
    const UT_Array<UT_FprealArray>	&getFrameDependencies()
    {
	UT_ASSERT(isTrackDependencies());
	return myFrameDependencies;
    }

    // Add a ROP_Node to the dependency array.  Returns an identifier which
    // should be used when adding frame dependencies.
    int		 addROPDependency(ROP_Node *node);

    // Add a frame to the dependency array, using the node_id returned by
    // addROPDependency.
    void	 addFrameDependency(int node_id, fpreal frame);

    void	 setSohoString(const char *parm, const char *value);
    void	 setSohoReal(const char*parm, fpreal *value, int vsize=1);
    void	 setSohoInt(const char*parm, int *value, int vsize=1);

private:
    void	 setValidationFlag(int flag, bool on = true);
    bool	 testValidationFlag(int flag);

    fpreal	 myStartFrame;
    fpreal	 myEndFrame;
    fpreal	 myFrameInc;
    const char	*myOutputFile;
    const char	*myOutputFormat;
    bool	 myToFlipbook;
    int		 myQuality;
    bool	 myIgnoreInputs;
    bool	 myInterleaveFrames;
    bool	 myIgnoreBypass;
    bool	 myIgnoreLock;
    bool	 myTrackDependencies;
    bool	 myPrintDependencies;
    bool	 myPrintFullPath;
    bool	 myCondenseRanges;
    bool	 myFrameMerge;
    bool	 myAlfred;
    std::ostream *myOutStream;
    std::ostream *myErrStream;
    SOHO_ParmOverride	*mySohoParms;

    UT_ValArray<ROP_Node*>	 myROPDependencies;
    UT_Array<UT_FprealArray>	 myFrameDependencies;

    int		 myValidationFlags;
};

#endif
