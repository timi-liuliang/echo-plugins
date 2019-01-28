/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_dop_h__
#define __HOM_dop_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include <vector>
class HOM_DopData;
class HOM_DopSimulation;
class HOM_Node;

SWIGOUT(%rename(dop) HOM_dop;)
class HOM_API HOM_dop
{
public:
    virtual ~HOM_dop()
    {}

    virtual std::string __repr__() = 0;

    virtual bool isScriptSolverRunning() = 0;

    SWIGOUT(%newobject scriptSolverData;)
    virtual HOM_DopData *scriptSolverData() = 0;

    SWIGOUT(%newobject scriptSolverNetwork;)
    virtual HOM_Node *scriptSolverNetwork() = 0;

    SWIGOUT(%newobject scriptSolverSimulation;)
    virtual HOM_DopSimulation *scriptSolverSimulation() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> > scriptSolverObjects() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_DopData> > scriptSolverNewObjects() = 0;

    virtual double scriptSolverTimestepSize() = 0;

    virtual double scriptSolverTime() = 0;
};

#endif
