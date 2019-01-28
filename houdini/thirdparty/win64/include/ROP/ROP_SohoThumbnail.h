/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_SohoThumbnail.h ( ROP Library, C++)
 *
 * COMMENTS:	Class to render a thumbnail using soho without using a ROP
 *		output driver.
 */

#ifndef __ROP_SohoThumbnail__
#define __ROP_SohoThumbnail__

#include "ROP_API.h"
#include <iosfwd>
#include <UT/UT_String.h>

class GU_Detail;
class OP_Node;
class ROP_SohoOutput;

class ROP_API ROP_SohoThumbnail {
public:
	     ROP_SohoThumbnail(OP_Node *material);
    virtual ~ROP_SohoThumbnail();

    bool	 render(std::ostream &error, bool foreground);

    void	 setPicture(const char *fname)	{ myPicture.harden(fname); }
    void	 setRes(int res)		{ myRes = res; }

    // Determine the soho script and command for a given shop
    static bool		 determineShopType(OP_Node *material,
					   UT_String &soho_shopstyle,
					   UT_String &soho_script,
					   UT_String &soho_pipecmd);

    // Create sphere geometry used for thumbnails
    static GU_Detail	*createDefaultSphere();

private:
    OP_Node		*myMaterialNode;
    UT_String		 myPicture;
    int			 myRes;
};

#endif

