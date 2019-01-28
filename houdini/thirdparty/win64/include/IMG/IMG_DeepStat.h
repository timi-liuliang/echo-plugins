/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_DeepStat.h (IMG Library, C++)
 *
 * COMMENTS:
 */

#ifndef __IMG_DeepStat__
#define __IMG_DeepStat__

#include "IMG_API.h"

#include <PXL/PXL_Forward.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Options.h>
#include "IMG_FileTypes.h"

class UT_JSONWriter;

enum IMG_DeepPlanes
{
    IMG_DEPTH_FRONT,
    IMG_DEPTH_BACK,
    IMG_DEPTH_DELTA,
    IMG_DEPTH_AVERAGE,		// Replaces front with (front+back)*.5
    IMG_DEPTH_COVERAGE,
    IMG_DEPTH_FLAGS,

    IMG_DEPTH_MAX_PLANES
};

enum IMG_DeepPlaneMask
{
    IMG_DEPTH_EMPTY_MASK	= 0,
    IMG_DEPTH_FRONT_MASK	= (1 << IMG_DEPTH_FRONT),
    IMG_DEPTH_BACK_MASK		= (1 << IMG_DEPTH_BACK),
    IMG_DEPTH_DELTA_MASK	= (1 << IMG_DEPTH_DELTA),
    IMG_DEPTH_AVERAGE_MASK	= (1 << IMG_DEPTH_AVERAGE),
    IMG_DEPTH_COVERAGE_MASK	= (1 << IMG_DEPTH_COVERAGE),
    IMG_DEPTH_FLAGS_MASK	= (1 << IMG_DEPTH_FLAGS),

    IMG_DEPTH_FRONT_BACK_MASK	= (IMG_DEPTH_FRONT_MASK
				    |IMG_DEPTH_BACK_MASK),
    IMG_DEPTH_FRONT_DELTA_MASK	= (IMG_DEPTH_FRONT_MASK
				    |IMG_DEPTH_DELTA_MASK),
    IMG_DEPTH_ALL_MASK		= (IMG_DEPTH_FRONT_MASK
				    | IMG_DEPTH_BACK_MASK
				    | IMG_DEPTH_COVERAGE_MASK
				    | IMG_DEPTH_FLAGS_MASK),
};

/// IMG_Stat information used when creating deep images
class IMG_API IMG_DeepStat
{
public:
     IMG_DeepStat();
    ~IMG_DeepStat();

    // Common keywords handled in parseOption()
    static const char	*ofStorageKeyword();
    static const char	*zStorageKeyword();
    static const char	*ofSizeKeyword();
    static const char	*depthInterpKeyword();
    static const char	*compositingKeyword();
    static const char	*depthModeKeyword();
    static const char	*compressionKeyword();
    static const char	*deepcompressionKeyword();
    static const char	*zbiasKeyword();

    static IMG_DeepPlaneMask	deepPlaneMask(const char *token);
    static UT_StringHolder	deepPlaneMask(IMG_DeepPlaneMask mask);

    // Parse "well known" keyword options and store the result in the stat
    bool	 parseOption(const char *option, const UT_StringHolder &value);

    // Store the options
    void	storeArgs(UT_Options &options) const;

    // Print options to a work buffer
    bool	save(UT_JSONWriter &j) const;
    void	saveArgs(UT_WorkBuffer &wbuf) const;

    // Dump options to stdout
    void	dump() const;

    const UT_Options	&options() const	{ return myOptions; }
    void	setOption(const UT_StringHolder &name, bool v)
		    { myOptions.setOptionB(name, v); }
    void	setOption(const UT_StringHolder &name, int v)
		    { myOptions.setOptionI(name, v); }
    void	setOption(const UT_StringHolder &name, fpreal v)
		    { myOptions.setOptionF(name, v); }
    void	setOption(const UT_StringHolder &name, const UT_StringHolder &v)
		    { myOptions.setOptionS(name, v); }

    const UT_StringHolder	&compressorName() const
				    { return myCompressorName; }
    void			 setCompressorName(const UT_StringHolder &v)
				    { myCompressorName = v; }
    const UT_StringHolder	&mipCompressorName() const
				    { return myMipCompressorName; }
    void			 setMipCompressorName(const UT_StringHolder &v)
				    { myMipCompressorName = v; }

    IMG_DeepPlaneMask	deepPlaneMask() const { return myDeepPlaneMask; }
    void		resetDeepPlaneMask()
			{
			    myDeepPlaneMask = IMG_DEPTH_FRONT_MASK;
			}
    void		setDeepPlaneMask(IMG_DeepPlaneMask m)
			{
			    myDeepPlaneMask = m;
			}
    void		addDeepPlaneMask(IMG_DeepPlaneMask m)
			{
			    uint	curr = uint(myDeepPlaneMask);
			    myDeepPlaneMask = IMG_DeepPlaneMask((uint)m|curr);
			}

    IMG_DepthInterp	depthInterp() const { return myDepthInterp; }
    void		setDepthInterp(IMG_DepthInterp v) { myDepthInterp = v; }
    IMG_DataType	zStorage() const { return myZStorage; }
    void		setZStorage(const IMG_DataType v) { myZStorage = v; }
    IMG_DataType	ofStorage() const { return myOfStorage; }
    void		setOfStorage(const IMG_DataType v) { myOfStorage = v; }

    int		deepCoverBits() const { return myDeepCoverBits; }
    void	setDeepCoverBits(int v) { myDeepCoverBits = SYSclamp(v, 1,64); }
    int		deepFlagBits() const { return myDeepFlagBits; }
    void	setDeepFlagBits(int v) { myDeepFlagBits = SYSclamp(v, 1, 64); }
    int		ofSize() const { return myOfSize; }
    void	setOfSize(int v) { myOfSize = v; }
    int		compressionLevel() const { return myCompressionLevel; }
    void	setCompressionLevel(int v) { myCompressionLevel = v; }
    void	setDeepCompressionLevel(int v) { myDeepCompressionLevel = v; }
    bool	samplesComposited() const { return mySamplesComposited; }
    void	setSamplesComposited(bool v) { mySamplesComposited = v; }
    bool	createMipMaps() const { return myCreateMipMaps; }
    void	setCreateMipMaps(bool v) { myCreateMipMaps = v; }

private:
    UT_Options		myOptions;
    UT_StringHolder	myCompressorName;	// "default"
    UT_StringHolder	myMipCompressorName;	// "mip"
    IMG_DeepPlaneMask	myDeepPlaneMask;	// front
    IMG_DepthInterp	myDepthInterp;		// discrete
    IMG_DataType	myZStorage;		// IMG_FLOAT32
    IMG_DataType	myOfStorage;		// IMG_FLOAT16
    int			myDeepCoverBits;	// 32
    int			myDeepFlagBits;		// 8
    int			myOfSize;		// 3
    int			myCompressionLevel;	// 9
    int			myDeepCompressionLevel;	// 5
    bool		mySamplesComposited;	// false
    bool		myCreateMipMaps;	// true
};

#endif
