/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_ReData.h ( SHOP Library, C++)
 *
 * COMMENTS:	Interface for getting RE information out of a SHOP
 */

#ifndef __SHOP_ReData__
#define __SHOP_ReData__

#include <RE/RE_Material.h>

class SHOP_ReData
{
public:
    SHOP_ReData(const SHOP_ReData &src)
	: myMaterial(src.myMaterial)
    {
    }
    SHOP_ReData()
	: myMaterial()
    { }
    ~SHOP_ReData()
    {
    }

    const SHOP_ReData		&operator=(const SHOP_ReData &src)
				 {
				    myMaterial = src.myMaterial;
				    return *this;
				 }

    const RE_MaterialPtr	&getMaterial() const
				 { return myMaterial; }

    void			 clear()
				 { setMaterial(NULL); }
    void			 setMaterial(const RE_MaterialPtr &mat)
				 { myMaterial = mat; }

private:
    RE_MaterialPtr		 myMaterial;
};

#endif

