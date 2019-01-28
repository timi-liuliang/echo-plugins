/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GD_IOClassic.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GD_IOClassic__
#define __GD_IOClassic__

#include "GD_API.h"
#include <GA/GA_IO.h>

/// GU implementation of I/O
class GD_API GD_IOClassic : public GA_IO
{
public:
    GD_IOClassic();
    virtual ~GD_IOClassic();

    /// @{
    /// Methods required to implement GA_IO interface
    virtual const char		*getToken() const;
    virtual const char		*getLabel() const;
    virtual bool		 checkFilename(const char *filename,
					const GA_SaveOptions *opts) const;
    virtual bool		 checkFilename(const char *filename,
					const GA_LoadOptions *opts) const;
    virtual bool		 checkByteMagic(uint8 first_byte,
					const GA_LoadOptions *opts) const;
    virtual bool		 statStream(UT_IStream &is,
					GA_Stat &stat, uint stat_level) const;
    virtual bool		 readStream(GA_Detail &g,
					UT_IStream &is,
					const GA_LoadOptions *opts,
					UT_StringArray *errors) const;
    virtual bool		 writeFile(const GA_Detail &g,
					const char *filename,
					const GA_SaveOptions *opts,
					UT_StringArray *errors) const;
    virtual bool		 writeStream(const GA_Detail &g,
					std::ostream &os,
					bool binary,
					const GA_SaveOptions *opts,
					UT_StringArray *errors) const;
    /// @}

protected:
    /// @{
    /// Configuration from GA_IO
    virtual bool	readCheckStdin() const		{ return true; }
    virtual bool	readCheckCompressed() const		{ return true; }
    virtual bool	writeCheckStdout() const	{ return true; }
    /// @}
};

#endif

