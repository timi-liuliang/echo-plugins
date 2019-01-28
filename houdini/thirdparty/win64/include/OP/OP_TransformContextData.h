#ifndef __OP_TransformContext__
#define __OP_TransformContext__

#include "OP_Context.h"

#include <stdio.h>
class OP_API OP_TransformContextData : public OP_ContextData
{
public:
    OP_TransformContextData(OP_Node *caller);
    virtual ~OP_TransformContextData();

    OP_Node *getCallerNode() const;
private:
    OP_Node *myCaller; // Node to add dependencies to.
};
#endif

