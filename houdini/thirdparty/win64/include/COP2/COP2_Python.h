/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __COP2_Python_h__
#define __COP2_Python_h__

#include "COP2_API.h"
#include "COP2_Node.h"
#include "COP2_Error.h"
#include <UT/UT_Tuple.h>
#include <vector>
class PY_EvaluationContext;
class OP_ScriptOperator;

class COP2_API COP2_Python : public COP2_Node
{
public:
    virtual ~COP2_Python();

    static OP_ScriptOperator *createScriptOperator(
	const OP_OTLDefinition &definition);

    static OP_Node *myConstructor(
	OP_Network *network, const char *name, OP_Operator *op_type);

    virtual void getMaxNumThreadsInCook(
	COP2_Context &, int &maxp, int &maxn, int &op) const;

    virtual TIL_Sequence *cookSequenceInfo(OP_ERROR &error);

    virtual void getInputDependenciesForOutputArea(
	COP2_CookAreaInfo &output_area,
	const COP2_CookAreaList &input_areas,
	COP2_CookAreaList &needed_areas);

    virtual bool isTimeModifier() const;
    virtual float copTimeTransform(
	int input, COP2_Context &context, float time);

    virtual bool isSlowOperation() const;

    // These methods are used by HOMF_CopNode:
    bool isCookingThisNode() const
    { return myCookContext != NULL; }

    TIL_DataFormat formatOfCookingPlane() const;

    bool getInputPlaneWhileCookingFromPython(
	std::vector<float> &result, int input_index, const char *plane_name,
	const char *component, bool interleaved, UT_WorkBuffer &error_message);

    // If til_format is null, the input plane will be retrieved in the same
    // format as the currently cooking plane.
    bool getInputPlaneWhileCookingFromPython(
	std::string &result, int input_index, const char *plane_name,
	const char *component, const TIL_DataFormat *til_format,
	bool interleaved, UT_WorkBuffer &error_message);

    bool setOutputPlaneWhileCookingFromPython(
	const void *values, TIL_DataFormat til_format, int64 num_values,
	const char *component, bool interleaved, bool flip_vertically,
	UT_WorkBuffer &error_message);

protected:
    virtual void computeImageBounds(COP2_Context &context);
    virtual OP_ERROR cookMyTile(COP2_Context &context, TIL_TileList *tiles);

private:
    COP2_Python(OP_Network *parent, const char *name, OP_Operator *entry);

    void copySequenceInfoFromFirstInput();
    void matchFrameRangeOfInputs();
    float timeToCookInput(int input_index, float time);

    TIL_DataFormat runPythonToOverrideDepth(const TIL_Plane &output_plane);
    void runPythonToGetPlanesToCook();
    TIL_Plane *addPlaneToSequence(const char *name);
    void runPythonToOverrideFrameRange();
    void runPythonToOverrideResolution();

    void buildRequiredInputPlanes();
    void runPythonToGetRequiredInputPlanes(const TIL_Plane &output_plane);

    void cookUsingPython(COP2_Context &context, TIL_Region &output_region);
    void clearComponentsNotWrittenTo();

    bool runFunctionInsidePythonCookCode(
	const char *function_name, const char *parms_tuple,
	PY_Result::Type desired_result_type, PY_Result &result,
	float *time = NULL);
    bool hasFunctionInsidePythonCookCode(const char *function_name);
    bool ensurePythonCookCodeHasRun();

    bool isValidInputPlaneForOutputPlane(
	const char *output_plane_name,
	int input_index,
	const char *input_plane_name);

    template <typename T>
    bool templatedGetInputPlaneWhileCookingFromPython(
	T &result, int input_index, const char *plane_name,
	const char *component, TIL_DataFormat til_format, bool interleaved,
	UT_WorkBuffer &error_message);

    void addPythonError(const char *message,
	COP2_ErrorCodes error_code=COP2_ERROR_PYTHON_ERROR);
    void addPythonWarning(const char *message,
	COP2_ErrorCodes error_code=COP2_ERROR_PYTHON_ERROR);
    UT_String houNodeExpression();

    class PlaneToCook
    {
    public:
	PlaneToCook()
	: myFormat(TILE_FLOAT32)
	{}

	std::vector<UT_Tuple<int, std::string> > myInputIndicesAndPlanes;
	TIL_DataFormat myFormat;
    };

    PY_EvaluationContext *myPythonContext;
    std::map<std::string, PlaneToCook> myPlanesToCook;
    COP2_Context *myCookContext;
    TIL_Region *myCookOutputRegion;
    bool myCookPlaneComponentsWritten[PLANE_MAX_VECTOR_SIZE];
    UT_Lock myCookLock;
};

#endif
