/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SolverScript_h__
#define __SIM_SolverScript_h__

#include "SIMZ_API.h"
#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_Solver.h>
#include "SIMZ_Utils.h"

class SIM_GeometryCopy;
class SIM_SolverScript;

class SIMZ_API SIM_SolverScriptData
{
public:
     SIM_SolverScriptData()
    : mySolver(0),
      myObjects(0),
      myNewObjects(0),
      myTimestep(0),
      myTime(0)
    {}

     SIM_SolverScriptData(SIM_SolverScript *solver,
			  const SIM_ObjectArray *objects,
			  const SIM_ObjectArray *new_objects,
			  const SIM_Time *timestep,
			  const SIM_Time *time)
    : mySolver(solver),
      myObjects(objects),
      myNewObjects(new_objects),
      myTimestep(timestep),
      myTime(time)
    {}

    ~SIM_SolverScriptData()
    {}

    static UT_Array<SIM_SolverScriptData> &getSolverStack();

    SIM_SolverScript		*mySolver;
    const SIM_Time		*myTimestep;
    const SIM_Time		*myTime;
    const SIM_ObjectArray	*myObjects;
    const SIM_ObjectArray	*myNewObjects;
};

class SIMZ_API SIM_SolverScript : public SIM_Solver,
				  public SIM_OptionsUser
{
public:
    // Access functions for our specific data.
    GETSET_DATA_FUNCS_B("usesnippet", UseSnippet);
    GETSET_DATA_FUNCS_S(SIM_NAME_SCRIPT, Script);
    GETSET_DATA_FUNCS_S("pythonsnippet", PythonSnippet);

    void		 getDopnetPath(UT_String &path) const;
    const SIM_Engine	&getDopnetEngine() const;
    static void		 installCommands();

    static bool		 fetchBoilerplate(UT_String &boilerplate);

protected:
    explicit		 SIM_SolverScript(const SIM_DataFactory *factory);
    virtual		~SIM_SolverScript();

    virtual SIM_Result	 solveObjectsSubclass(SIM_Engine &engine,
					SIM_ObjectArray &objects,
					SIM_ObjectArray &newobjects,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep);

private:
    SIM_Result		 runScript(
			    const char *script,
			    const SIM_ObjectArray &objects,
			    const SIM_ObjectArray &new_objects,
			    const SIM_Time &timestep,
			    const SIM_Time &time);
    SIM_Result		 runHscriptScript(
			    const char *script,
			    const char *error_title);
    SIM_Result		 runPythonScriptFile(
			    const char *script_file,
			    const char *error_title);

    SIM_Result		 runPythonSnippet(
			    const char *snippet,
			    const char *error_title);

    static const SIM_DopDescription	*getSolverScriptDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SolverScript,
			SIM_Solver,
			"Script Solver",
			getSolverScriptDopDescription());
};

#endif

