/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_PwHandle.h (GA Library, C++)
 *
 * COMMENTS:    Wrapper to be able to read P and Pw with a single handle.
 *              It is intentionally named inconsistently with GA_RWHandleXX
 *              and GA_ROHandleXX, so that it doesn't show up among them.
 */

#pragma once

#ifndef __GA_PwHandle__
#define __GA_PwHandle__

#include "GA_API.h"
#include "GA_Handle.h"
#include "GA_Types.h"
#include <UT/UT_Vector4.h>
#include <SYS/SYS_TypeTraits.h>

template<bool isconst=true>
class GA_PwHandle
{
public:
    typedef typename SYS_SelectType<GA_Detail,const GA_Detail,isconst>::type Detail;
    typedef typename SYS_SelectType<GA_RWHandleV3,GA_ROHandleV3,isconst>::type HandleV3;
    typedef typename SYS_SelectType<GA_RWHandleF,GA_ROHandleF,isconst>::type HandleF;

    GA_PwHandle()
        : myP()
        , myW()
    {}
    GA_PwHandle(Detail &gdp)
        : myP(gdp.getP())
        , myW(gdp.getPwAttribute())
    {}
    GA_PwHandle(const HandleV3 &p, const HandleF &w)
        : myP(p)
        , myW(w)
    {}
    UT_Vector4 get(GA_Offset ptoff, int component=0) const
    {
        UT_ASSERT_P(component == 0);
        return UT_Vector4(myP.get(ptoff), myW.isValid() ? myW.get(ptoff) : 1.0f);
    }
    const GA_Attribute *getAttribute() const
    {
        return myP.getAttribute();
    }
    bool isValid() const
    {
        return myP.isValid();
    }
protected:
    HandleV3 myP;
    HandleF myW;
};

typedef GA_PwHandle<true> GA_PwHandleRO;

class GA_PwHandleRW : public GA_PwHandle<false>
{
public:
    typedef GA_PwHandle<false> Base;
protected:
    using Base::myP;
    using Base::myW;
public:
    using Base::Detail;
    using Base::HandleV3;
    using Base::HandleF;

    GA_PwHandleRW()
        : Base()
    {}
    GA_PwHandleRW(Detail &gdp)
        : Base(gdp)
    {}
    GA_PwHandleRW(const HandleV3 &p, const HandleF &w)
        : Base(p, w)
    {}
    void set(GA_Offset ptoff, const UT_Vector4 &v) const
    {
        myP.set(ptoff, UT_Vector3(v));
        if (myW.isValid())
            myW.set(ptoff, v.w());
    }
    void set(GA_Offset ptoff, int component, const UT_Vector4 &v) const
    {
        UT_ASSERT_P(component == 0);
        set(ptoff, v);
    }
};

#endif
