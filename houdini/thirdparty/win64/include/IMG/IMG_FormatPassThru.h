/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FormatPassThru.h ( IMG Library, C++)
 *
 * COMMENTS:	Creates a format which is used for creating a pass-thru image
 *		format.  For example a flip or scale format which gets
 *		information from the parent format.
 *
 * !!grep virtual IMG_Format.h
    virtual ~IMG_Format();
    virtual const char	*getFormatName() const = 0;
    virtual const char	*getFormatLabel() const;
    virtual const char	*getFormatDescription() const;
    virtual bool	 isPassThrough() const { return false; }
    virtual const char	*getDefaultExtension() const;
    virtual const char	*getFormatLocation() const	{ return myLocation; }
    virtual int		 checkExtension(const char *filename) const = 0;
    virtual int		 checkMagic(unsigned int) const;
    virtual int		 checkMagicSeekable(UT_IStream &is) const;
    virtual int		 checkDevice(const char *) const;
    virtual IMG_DataType	 getAllowedTypes() const;
    virtual IMG_ColorModel	 getColorModel() const;
    virtual unsigned		 getMaxImages() const = 0;
    virtual void		 getMaxResolution(unsigned &x,
    virtual int			 isReadRandomAccess() const;
    virtual int			 isWriteRandomAccess() const;
    virtual int			 isTopFirst() const;
    virtual const IMG_FileOptionList	*getOptions() const;
    virtual IMG_File	*createFile()	 const = 0;
    virtual int		 isFormatOk(const IMG_Stat &stat) const;
 */

#ifndef __IMG_FormatPassThru__
#define __IMG_FormatPassThru__

#include "IMG_API.h"
#include "IMG_Format.h"

#include <stddef.h> // For NULL

class IMG_API IMG_FormatPassThru : public IMG_Format {
public:
    // Ensure we pass through to the non-list based img format so
    // we don't have to remove ourselves from the list.
    // This also avoids threading problems if we create pass throughs
    // in a multi-threaded environment as the list isn't locked.
    IMG_FormatPassThru(const IMG_File *parent_file) : IMG_Format(false)
		{
		    myParent = parent_file;
		}

    virtual ~IMG_FormatPassThru() {}

    virtual const char	*getFormatName() const
			 { return getParent()->getFormatName(); }
    virtual const char	*getFormatLabel() const
			 { return getParent()->getFormatLabel(); }
    virtual const char	*getFormatDescription() const
			 { return getParent()->getFormatDescription(); }
    virtual const char	*getDefaultExtension() const
			 { return getParent()->getDefaultExtension(); }
    virtual const char	*getFormatLocation() const
			 { return getParent()->getFormatLocation(); }

    virtual int		 checkExtension(const char *filename) const
			 { return getParent()->checkExtension(filename); }
    virtual int		 checkMagic(unsigned int magic) const
			 { return getParent()->checkMagic(magic); }
    virtual int		 checkMagicSeekable(UT_IStream &is) const
			 { return getParent()->checkMagicSeekable(is); }

    virtual bool	 isPassThrough() const { return true; }

    virtual int		 checkDevice(const char *filename) const
			 { return getParent()->checkDevice(filename); }

    virtual IMG_DataType	getSupportedTypes() const
			 { return getParent()->getSupportedTypes(); }
    virtual IMG_ColorModel	getSupportedColorModels() const
		         { return getParent()->getSupportedColorModels(); }
    virtual void		getMaxResolution(unsigned &x,
					         unsigned &y) const
			 { getParent()->getMaxResolution( x, y ); }


    virtual int		isReadRandomAccess() const
			 { return getParent()->isReadRandomAccess(); }
    virtual int		isWriteRandomAccess() const
			 { return getParent()->isWriteRandomAccess(); }
    virtual int		isTopFirst() const
			 { return getParent()->isTopFirst(); }
    virtual int		isLeftFirst() const
			 { return getParent()->isLeftFirst(); }

    virtual bool	isDataWindowSupported() const
			 { return getParent()->isDataWindowSupported(); }
    virtual bool	isDataWindowStreakSupported() const
			 { return getParent()->isDataWindowStreakSupported(); }
    
    virtual bool	isDeepRasterSupported() const
			 { return getParent()->isDeepRasterSupported(); }
    
    virtual bool	isDataInterleaved() const
			 { return getParent()->isDataInterleaved(); }
    
    virtual bool	canPlaneTypesDiffer() const
			 { return getParent()->canPlaneTypesDiffer(); } 
    
    virtual const IMG_FileOptionList	*getOptions() const
			 { return getParent()->getOptions(); }

    virtual PXL_ColorSpace	formatColorSpace() const
				{ return getParent()->formatColorSpace(); }
    virtual fpreal		formatColorSpaceGamma() const
				{ return getParent()->formatColorSpaceGamma(); }
    virtual bool		formatStoresColorSpace() const
    				{ return getParent()->formatStoresColorSpace();}

protected:

    virtual IMG_File	*createFile()	 const
			 { return NULL; }
    virtual int		 isFormatOk(const IMG_Stat &) const
			 { return 0; }

    const IMG_Format    *getParent() const
	{ return myParent->getBaseFile()->getFormat(); }
    
    const IMG_File	*myParent;
    
private:
    // Forbid the plain declaration of this class as we want to always
    // invoked IMG_Format(false).
    IMG_FormatPassThru() { }
};

#endif
